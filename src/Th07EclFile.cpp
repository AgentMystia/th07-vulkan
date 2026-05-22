#include "Th07EclFile.hpp"

#include <algorithm>
#include <sstream>

namespace th07 {
namespace {

bool Fail(std::string *errorMessage, const std::string &message)
{
    if (errorMessage != nullptr) {
        *errorMessage = message;
    }
    return false;
}

bool HasRange(std::span<const u8> data, std::size_t offset, std::size_t size)
{
    return offset <= data.size() && size <= data.size() - offset;
}

std::uint16_t ReadU16(std::span<const u8> data, std::size_t offset)
{
    return static_cast<std::uint16_t>(data[offset]) |
           (static_cast<std::uint16_t>(data[offset + 1]) << 8);
}

i16 ReadI16(std::span<const u8> data, std::size_t offset)
{
    return static_cast<i16>(ReadU16(data, offset));
}

std::uint32_t ReadU32(std::span<const u8> data, std::size_t offset)
{
    return static_cast<std::uint32_t>(data[offset]) |
           (static_cast<std::uint32_t>(data[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(data[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(data[offset + 3]) << 24);
}

bool FailWithOffset(std::string *errorMessage, const char *context, std::size_t offset)
{
    std::ostringstream message;
    message << context << " at 0x" << std::hex << offset;
    return Fail(errorMessage, message.str());
}

std::uint32_t SubOffsetTableByteSize(std::uint16_t subCount)
{
    return (static_cast<std::uint32_t>(subCount) + kTh07EclSubOffsetTableSentinelEntries) * 4;
}

std::uint32_t SubOffsetTableEnd(std::uint16_t subCount)
{
    return kTh07EclSubOffsetTableOffset + SubOffsetTableByteSize(subCount);
}

std::uint32_t TimelineEndOffset(const Th07EclHeader &header, std::uint16_t timelineIndex,
                                std::size_t fileSize)
{
    if (timelineIndex + 1 < header.timelineCount) {
        return header.timelineOffsets[timelineIndex + 1];
    }
    if (header.timelineCount < kTh07EclMaxTimelineCount &&
        header.timelineOffsets[header.timelineCount] > header.timelineOffsets[timelineIndex]) {
        return header.timelineOffsets[header.timelineCount];
    }
    return static_cast<std::uint32_t>(fileSize);
}

bool ParseSub(std::span<const u8> data, std::uint16_t subIndex, std::uint32_t subOffset,
              std::uint32_t subRegionEnd, Th07EclSub *outSub, std::string *errorMessage)
{
    if (!HasRange(data, subOffset, kTh07EclSubHeaderSize)) {
        return FailWithOffset(errorMessage, "ECL sub header is outside the file", subOffset);
    }

    outSub->index = subIndex;
    outSub->offset = subOffset;
    outSub->prologue = ReadU32(data, subOffset);

    std::size_t cursor = subOffset + kTh07EclSubHeaderSize;
    bool sawTerminator = false;
    while (HasRange(data, cursor, kTh07EclRawInstructionHeaderSize) && cursor < subRegionEnd) {
        Th07EclRawInstruction instruction;
        instruction.offset = static_cast<std::uint32_t>(cursor);
        instruction.opcode = ReadI16(data, cursor);
        instruction.byteSize = ReadU16(data, cursor + 2);
        instruction.time = ReadI16(data, cursor + 4);
        instruction.operandMask = ReadU16(data, cursor + 6);
        if (instruction.opcode == kTh07EclRawInstructionTerminatorOpcode) {
            if (instruction.byteSize != kTh07EclRawInstructionTerminatorSize) {
                return FailWithOffset(errorMessage, "ECL sub terminator size is not 0x0c", cursor);
            }
            instruction.isTerminator = true;
            outSub->instructions.push_back(std::move(instruction));
            sawTerminator = true;
            break;
        }

        if (instruction.opcode < 0 || instruction.opcode > static_cast<i16>(kTh07EclMaxOpcode)) {
            return FailWithOffset(errorMessage, "ECL sub opcode is outside the current TH07 evidence range",
                                  cursor);
        }
        if (instruction.byteSize < kTh07EclRawInstructionHeaderSize ||
            instruction.byteSize % 4 != 0) {
            return FailWithOffset(errorMessage, "ECL sub instruction size is invalid", cursor);
        }
        if (!HasRange(data, cursor, instruction.byteSize) ||
            cursor + instruction.byteSize > subRegionEnd) {
            return FailWithOffset(errorMessage, "ECL sub instruction body exceeds the file", cursor);
        }

        instruction.arguments.assign(data.begin() + static_cast<std::ptrdiff_t>(cursor + kTh07EclRawInstructionHeaderSize),
                                     data.begin() + static_cast<std::ptrdiff_t>(cursor + instruction.byteSize));
        outSub->instructions.push_back(std::move(instruction));
        cursor += outSub->instructions.back().byteSize;
    }

    if (!sawTerminator) {
        return FailWithOffset(errorMessage, "ECL sub is missing its terminator", subOffset);
    }
    return true;
}

bool ParseTimeline(std::span<const u8> data, const Th07EclHeader &header, std::uint16_t timelineIndex,
                   Th07EclTimeline *outTimeline, std::string *errorMessage)
{
    const std::uint32_t start = header.timelineOffsets[timelineIndex];
    const std::uint32_t end = TimelineEndOffset(header, timelineIndex, data.size());
    if (start >= end || !HasRange(data, start, kTh07EclTimelineInstructionHeaderSize)) {
        return FailWithOffset(errorMessage, "ECL timeline offset is outside the file", start);
    }

    outTimeline->index = timelineIndex;
    outTimeline->offset = start;
    for (std::size_t cursor = start;
         HasRange(data, cursor, kTh07EclTimelineInstructionHeaderSize) && cursor < end;) {
        Th07EclTimelineInstruction instruction;
        instruction.offset = static_cast<std::uint32_t>(cursor);
        instruction.time = ReadI16(data, cursor);
        instruction.argument0 = ReadI16(data, cursor + 2);
        instruction.opcode = ReadI16(data, cursor + 4);
        instruction.byteSize = ReadU16(data, cursor + 6);
        if (instruction.time == kTh07EclTimelineTerminatorTime) {
            instruction.isTerminator = true;
            outTimeline->instructions.push_back(std::move(instruction));
            break;
        }

        if (instruction.opcode < 0 ||
            instruction.opcode > static_cast<i16>(kTh07EclMaxTimelineOpcode)) {
            return FailWithOffset(errorMessage, "ECL timeline opcode is outside the current TH07 evidence range",
                                  cursor);
        }
        if (instruction.byteSize < kTh07EclTimelineInstructionHeaderSize ||
            instruction.byteSize > kTh07EclTimelineInstructionMaxSize ||
            instruction.byteSize % 4 != 0) {
            return FailWithOffset(errorMessage, "ECL timeline instruction size is invalid", cursor);
        }
        if (!HasRange(data, cursor, instruction.byteSize) || cursor + instruction.byteSize > end) {
            return FailWithOffset(errorMessage, "ECL timeline instruction body exceeds the file", cursor);
        }

        instruction.arguments.assign(data.begin() + static_cast<std::ptrdiff_t>(cursor + 8),
                                     data.begin() + static_cast<std::ptrdiff_t>(cursor + instruction.byteSize));
        outTimeline->instructions.push_back(std::move(instruction));
        cursor += instruction.byteSize;
    }
    return true;
}

} // namespace

bool ParseTh07EclFile(std::span<const u8> data, Th07EclFile *outFile, std::string *errorMessage)
{
    if (outFile == nullptr) {
        return Fail(errorMessage, "output pointer is null");
    }
    *outFile = {};
    if (!HasRange(data, 0, kTh07EclHeaderSize)) {
        return Fail(errorMessage, "ECL file is too small for a TH07 header");
    }

    Th07EclHeader &header = outFile->header;
    header.subCount = ReadU16(data, 0);
    header.timelineCount = ReadU16(data, 2);
    for (std::uint16_t idx = 0; idx < kTh07EclMaxTimelineCount; idx++) {
        header.timelineOffsets[idx] = ReadU32(data, 4 + static_cast<std::size_t>(idx) * 4);
    }

    if (header.subCount == 0 || header.subCount > kTh07EclMaxSubCount) {
        return Fail(errorMessage, "ECL sub count is outside the current TH07 evidence range");
    }
    if (header.timelineCount == 0 || header.timelineCount > kTh07EclMaxTimelineCount) {
        return Fail(errorMessage, "ECL timeline count is outside the current TH07 evidence range");
    }
    for (std::uint16_t idx = 0; idx < header.timelineCount; idx++) {
        if (header.timelineOffsets[idx] <= kTh07EclHeaderSize ||
            !HasRange(data, header.timelineOffsets[idx], 1)) {
            return FailWithOffset(errorMessage, "ECL timeline offset is outside the file",
                                  header.timelineOffsets[idx]);
        }
        if (idx > 0 && header.timelineOffsets[idx] <= header.timelineOffsets[idx - 1]) {
            return Fail(errorMessage, "ECL timeline offsets are not strictly increasing");
        }
    }

    const std::uint32_t tableEnd = SubOffsetTableEnd(header.subCount);
    if (!HasRange(data, kTh07EclSubOffsetTableOffset, SubOffsetTableByteSize(header.subCount))) {
        return Fail(errorMessage, "ECL sub offset table exceeds the file");
    }
    if (ReadU32(data, kTh07EclSubOffsetTableOffset) != 0) {
        return Fail(errorMessage, "ECL sub offset table sentinel is not zero");
    }
    if (tableEnd != ReadU32(data, kTh07EclSubOffsetTableOffset + 4)) {
        return Fail(errorMessage, "ECL first sub offset does not begin after the offset table");
    }
    if (tableEnd > header.timelineOffsets[0]) {
        return Fail(errorMessage, "ECL sub offset table overlaps the first timeline");
    }

    outFile->subOffsets.reserve(header.subCount);
    for (std::uint16_t idx = 0; idx < header.subCount; idx++) {
        const std::uint32_t offset =
            ReadU32(data, kTh07EclSubOffsetTableOffset +
                              (static_cast<std::size_t>(idx) + kTh07EclSubOffsetTableSentinelEntries) * 4);
        if (offset < tableEnd || offset >= header.timelineOffsets[0]) {
            return FailWithOffset(errorMessage, "ECL sub offset is outside the sub region", offset);
        }
        if (idx > 0 && offset <= outFile->subOffsets.back()) {
            return Fail(errorMessage, "ECL sub offsets are not strictly increasing");
        }
        outFile->subOffsets.push_back(offset);
    }

    outFile->subs.reserve(header.subCount);
    for (std::uint16_t idx = 0; idx < header.subCount; idx++) {
        Th07EclSub sub;
        if (!ParseSub(data, idx, outFile->subOffsets[idx], header.timelineOffsets[0], &sub, errorMessage)) {
            *outFile = {};
            return false;
        }
        outFile->subs.push_back(std::move(sub));
    }

    outFile->timelines.reserve(header.timelineCount);
    for (std::uint16_t idx = 0; idx < header.timelineCount; idx++) {
        Th07EclTimeline timeline;
        if (!ParseTimeline(data, header, idx, &timeline, errorMessage)) {
            *outFile = {};
            return false;
        }
        outFile->timelines.push_back(std::move(timeline));
    }

    return true;
}

const Th07EclSub *FindTh07EclSub(const Th07EclFile &file, std::uint16_t subIndex)
{
    for (const Th07EclSub &sub : file.subs) {
        if (sub.index == subIndex) {
            return &sub;
        }
    }
    return nullptr;
}

const Th07EclTimeline *FindTh07EclTimeline(const Th07EclFile &file, std::uint16_t timelineIndex)
{
    for (const Th07EclTimeline &timeline : file.timelines) {
        if (timeline.index == timelineIndex) {
            return &timeline;
        }
    }
    return nullptr;
}

std::size_t CountTh07EclSubInstructions(const Th07EclFile &file)
{
    std::size_t count = 0;
    for (const Th07EclSub &sub : file.subs) {
        for (const Th07EclRawInstruction &instruction : sub.instructions) {
            if (!instruction.isTerminator) {
                count++;
            }
        }
    }
    return count;
}

std::size_t CountTh07EclSubTerminators(const Th07EclFile &file)
{
    std::size_t count = 0;
    for (const Th07EclSub &sub : file.subs) {
        for (const Th07EclRawInstruction &instruction : sub.instructions) {
            if (instruction.isTerminator) {
                count++;
            }
        }
    }
    return count;
}

std::size_t CountTh07EclSubOpcode(const Th07EclFile &file, std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07EclSub &sub : file.subs) {
        for (const Th07EclRawInstruction &instruction : sub.instructions) {
            if (!instruction.isTerminator && static_cast<std::uint16_t>(instruction.opcode) == opcode) {
                count++;
            }
        }
    }
    return count;
}

std::uint32_t CountTh07EclSubArgumentBytes(const Th07EclFile &file)
{
    std::uint32_t count = 0;
    for (const Th07EclSub &sub : file.subs) {
        for (const Th07EclRawInstruction &instruction : sub.instructions) {
            count += static_cast<std::uint32_t>(instruction.arguments.size());
        }
    }
    return count;
}

Th07EclOpcodeMask BuildTh07EclSubOpcodeMask(const Th07EclFile &file)
{
    Th07EclOpcodeMask mask;
    for (const Th07EclSub &sub : file.subs) {
        for (const Th07EclRawInstruction &instruction : sub.instructions) {
            if (!instruction.isTerminator && instruction.opcode >= 0 &&
                instruction.opcode <= static_cast<i16>(kTh07EclMaxOpcode)) {
                mask.set(static_cast<std::size_t>(instruction.opcode));
            }
        }
    }
    return mask;
}

std::size_t CountTh07EclTimelineInstructions(const Th07EclFile &file)
{
    std::size_t count = 0;
    for (const Th07EclTimeline &timeline : file.timelines) {
        for (const Th07EclTimelineInstruction &instruction : timeline.instructions) {
            if (!instruction.isTerminator) {
                count++;
            }
        }
    }
    return count;
}

std::size_t CountTh07EclTimelineTerminators(const Th07EclFile &file)
{
    std::size_t count = 0;
    for (const Th07EclTimeline &timeline : file.timelines) {
        for (const Th07EclTimelineInstruction &instruction : timeline.instructions) {
            if (instruction.isTerminator) {
                count++;
            }
        }
    }
    return count;
}

std::size_t CountTh07EclTimelineOpcode(const Th07EclFile &file, std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07EclTimeline &timeline : file.timelines) {
        for (const Th07EclTimelineInstruction &instruction : timeline.instructions) {
            if (!instruction.isTerminator && static_cast<std::uint16_t>(instruction.opcode) == opcode) {
                count++;
            }
        }
    }
    return count;
}

std::uint32_t CountTh07EclTimelineArgumentBytes(const Th07EclFile &file)
{
    std::uint32_t count = 0;
    for (const Th07EclTimeline &timeline : file.timelines) {
        for (const Th07EclTimelineInstruction &instruction : timeline.instructions) {
            count += static_cast<std::uint32_t>(instruction.arguments.size());
        }
    }
    return count;
}

Th07EclTimelineOpcodeMask BuildTh07EclTimelineOpcodeMask(const Th07EclFile &file)
{
    Th07EclTimelineOpcodeMask mask;
    for (const Th07EclTimeline &timeline : file.timelines) {
        for (const Th07EclTimelineInstruction &instruction : timeline.instructions) {
            if (!instruction.isTerminator && instruction.opcode >= 0 &&
                instruction.opcode <= static_cast<i16>(kTh07EclMaxTimelineOpcode)) {
                mask.set(static_cast<std::size_t>(instruction.opcode));
            }
        }
    }
    return mask;
}

} // namespace th07
