#include "Th07MsgFile.hpp"

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

std::uint32_t ReadU32(std::span<const u8> data, std::size_t offset)
{
    return static_cast<std::uint32_t>(data[offset]) |
           (static_cast<std::uint32_t>(data[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(data[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(data[offset + 3]) << 24);
}

i32 ReadI32(std::span<const u8> data, std::size_t offset)
{
    return static_cast<i32>(ReadU32(data, offset));
}

bool IsTextOpcode(std::uint8_t opcode)
{
    return opcode == kTh07MsgOpcodeTextDialogue || opcode == kTh07MsgOpcodeTextIntro;
}

bool ParseEntry(std::span<const u8> data, std::uint32_t startOffset, std::uint32_t endOffset,
                Th07MsgEntry *entry, std::string *errorMessage)
{
    entry->offset = startOffset;

    for (std::size_t cursor = startOffset; cursor < endOffset;) {
        if (!HasRange(data, cursor, kTh07MsgInstructionHeaderSize) ||
            cursor + kTh07MsgInstructionHeaderSize > endOffset) {
            return Fail(errorMessage, "MSG instruction header exceeds entry bounds");
        }

        Th07MsgInstruction instruction;
        instruction.time = ReadU16(data, cursor);
        instruction.opcode = data[cursor + 2];
        instruction.argumentSize = data[cursor + 3];
        if (instruction.opcode > kTh07MsgMaxOpcode) {
            std::ostringstream message;
            message << "MSG opcode " << static_cast<int>(instruction.opcode)
                    << " exceeds current TH07 evidence";
            return Fail(errorMessage, message.str());
        }

        const std::size_t argumentOffset = cursor + kTh07MsgInstructionHeaderSize;
        if (!HasRange(data, argumentOffset, instruction.argumentSize) ||
            argumentOffset + instruction.argumentSize > endOffset) {
            return Fail(errorMessage, "MSG instruction args exceed entry bounds");
        }
        if (IsTextOpcode(instruction.opcode) && instruction.argumentSize < sizeof(i32)) {
            return Fail(errorMessage, "MSG text opcode is missing text metadata");
        }
        if (instruction.opcode == kTh07MsgOpcodeWait && instruction.argumentSize != sizeof(i32)) {
            return Fail(errorMessage, "MSG wait opcode argument size changed");
        }

        instruction.arguments.assign(data.begin() + static_cast<std::ptrdiff_t>(argumentOffset),
                                     data.begin() + static_cast<std::ptrdiff_t>(argumentOffset +
                                                                                instruction.argumentSize));
        entry->instructions.push_back(std::move(instruction));
        cursor = argumentOffset + entry->instructions.back().argumentSize;
    }

    return true;
}

} // namespace

bool ParseTh07MsgFile(std::span<const u8> data, Th07MsgFile *outFile, std::string *errorMessage)
{
    if (outFile == nullptr) {
        return Fail(errorMessage, "output pointer is null");
    }
    *outFile = {};
    if (!HasRange(data, 0, kTh07MsgHeaderSize)) {
        return Fail(errorMessage, "MSG file is too small for a TH07 header");
    }

    // TH07 msg06 shape consumed by Gui::LoadMsg and FUN_00429c42:
    // u32 entry pointer count, u32 entry offsets, then u16/u8/u8 instruction records.
    const std::uint32_t entryPointerCount = ReadU32(data, 0);
    if (entryPointerCount == 0 || entryPointerCount > kTh07MsgMaxEntryPointers) {
        return Fail(errorMessage, "MSG entry pointer count out of range");
    }
    const std::size_t pointerTableEnd =
        kTh07MsgHeaderSize + static_cast<std::size_t>(entryPointerCount) * kTh07MsgEntryPointerSize;
    if (!HasRange(data, kTh07MsgHeaderSize,
                  static_cast<std::size_t>(entryPointerCount) * kTh07MsgEntryPointerSize)) {
        return Fail(errorMessage, "MSG pointer table exceeds file bounds");
    }

    std::vector<std::uint32_t> uniqueOffsets;
    for (std::uint32_t index = 0; index < entryPointerCount; index++) {
        const std::uint32_t offset =
            ReadU32(data, kTh07MsgHeaderSize + static_cast<std::size_t>(index) * kTh07MsgEntryPointerSize);
        if (offset < pointerTableEnd || offset >= data.size()) {
            return Fail(errorMessage, "MSG entry offset out of range");
        }
        outFile->entryOffsets.push_back(offset);
        uniqueOffsets.push_back(offset);
    }

    std::sort(uniqueOffsets.begin(), uniqueOffsets.end());
    uniqueOffsets.erase(std::unique(uniqueOffsets.begin(), uniqueOffsets.end()), uniqueOffsets.end());
    for (std::size_t index = 0; index < uniqueOffsets.size(); index++) {
        const std::uint32_t startOffset = uniqueOffsets[index];
        const std::uint32_t endOffset =
            index + 1 < uniqueOffsets.size() ? uniqueOffsets[index + 1] : static_cast<std::uint32_t>(data.size());
        if (startOffset >= endOffset) {
            return Fail(errorMessage, "MSG entry range is empty or inverted");
        }

        Th07MsgEntry entry;
        if (!ParseEntry(data, startOffset, endOffset, &entry, errorMessage)) {
            *outFile = {};
            return false;
        }
        outFile->uniqueEntries.push_back(std::move(entry));
    }

    return true;
}

const Th07MsgEntry *FindTh07MsgEntry(const Th07MsgFile &file, std::uint32_t entryIndex)
{
    if (entryIndex >= file.entryOffsets.size()) {
        return nullptr;
    }
    return FindTh07MsgEntryByOffset(file, file.entryOffsets[entryIndex]);
}

const Th07MsgEntry *FindTh07MsgEntryByOffset(const Th07MsgFile &file, std::uint32_t offset)
{
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        if (entry.offset == offset) {
            return &entry;
        }
    }
    return nullptr;
}

std::size_t CountTh07MsgInstructions(const Th07MsgFile &file)
{
    std::size_t count = 0;
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        count += entry.instructions.size();
    }
    return count;
}

std::uint32_t BuildTh07MsgOpcodeMask(const Th07MsgFile &file)
{
    std::uint32_t mask = 0;
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        for (const Th07MsgInstruction &instruction : entry.instructions) {
            mask |= 1u << instruction.opcode;
        }
    }
    return mask;
}

std::size_t CountTh07MsgOpcode(const Th07MsgFile &file, std::uint8_t opcode)
{
    std::size_t count = 0;
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        for (const Th07MsgInstruction &instruction : entry.instructions) {
            if (instruction.opcode == opcode) {
                count++;
            }
        }
    }
    return count;
}

std::uint32_t CountTh07MsgTextBytes(const Th07MsgFile &file)
{
    std::uint32_t count = 0;
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        for (const Th07MsgInstruction &instruction : entry.instructions) {
            if (IsTextOpcode(instruction.opcode)) {
                count += instruction.argumentSize - sizeof(i32);
            }
        }
    }
    return count;
}

std::uint32_t CountTh07MsgWaitTotalFrames(const Th07MsgFile &file)
{
    std::uint32_t frames = 0;
    for (const Th07MsgEntry &entry : file.uniqueEntries) {
        for (const Th07MsgInstruction &instruction : entry.instructions) {
            if (instruction.opcode == kTh07MsgOpcodeWait) {
                const std::span<const u8> arguments(instruction.arguments.data(), instruction.arguments.size());
                frames += static_cast<std::uint32_t>(ReadI32(arguments, 0));
            }
        }
    }
    return frames;
}

} // namespace th07
