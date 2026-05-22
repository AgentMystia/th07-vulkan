#include "EclOpcodeTables.hpp"
#include "ReferencePaths.hpp"
#include "Th07EclFile.hpp"

#include <array>
#include <cassert>
#include <cstdio>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct EclStageSummary {
    std::uint8_t stage;
    std::uint32_t fileSize;
    std::uint16_t subCount;
    std::uint16_t timelineCount;
    std::uint32_t firstSubOffset;
    std::uint32_t firstTimelineOffset;
    std::uint16_t subInstructions;
    std::uint16_t subTerminators;
    std::uint32_t subArgumentBytes;
    std::uint16_t subOpcodeCount;
    std::uint16_t maxSubOpcode;
    std::uint16_t timelineInstructions;
    std::uint16_t timelineTerminators;
    std::uint32_t timelineArgumentBytes;
    std::uint16_t timelineOpcodeCount;
    std::uint16_t maxTimelineOpcode;
};

constexpr std::array<EclStageSummary, 8> kEclStageSummaries = {{
    {1, 44920, 58, 2, 0x012c, 0x8a00, 1443, 58, 22788, 96, 160, 305, 1, 7112, 6, 12},
    {2, 67384, 77, 3, 0x0178, 0xdda4, 2202, 77, 37824, 97, 160, 335, 3, 7856, 6, 12},
    {3, 53356, 60, 2, 0x0134, 0xae34, 1797, 60, 29192, 98, 154, 278, 2, 6464, 6, 12},
    {4, 131804, 153, 2, 0x02a8, 0x1aa1c, 4798, 153, 68184, 112, 154, 714, 2, 16936, 6, 12},
    {5, 48312, 78, 2, 0x017c, 0xaea8, 1885, 78, 28316, 93, 154, 118, 2, 2584, 6, 12},
    {6, 59232, 74, 2, 0x016c, 0xe050, 2347, 74, 37396, 102, 154, 62, 2, 1240, 5, 12},
    {7, 66152, 126, 2, 0x023c, 0xde18, 2438, 126, 35268, 111, 159, 296, 2, 6856, 6, 12},
    {8, 82372, 136, 2, 0x0264, 0x11234, 3009, 136, 43880, 110, 161, 386, 2, 9016, 6, 12},
}};

std::vector<u8> ReadWholeFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    assert(file.good());
    return std::vector<u8>(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
}

std::string ReadTextFile(const std::filesystem::path &path)
{
    std::ifstream file(path);
    assert(file.good());
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

std::size_t CountSubstring(std::string_view text, std::string_view needle)
{
    std::size_t count = 0;
    for (std::size_t pos = text.find(needle); pos != std::string_view::npos;
         pos = text.find(needle, pos + needle.size())) {
        count++;
    }
    return count;
}

std::size_t CountSetBits(const th07::Th07EclOpcodeMask &mask)
{
    std::size_t count = 0;
    for (std::size_t opcode = 0; opcode < mask.size(); opcode++) {
        if (mask.test(opcode)) {
            count++;
        }
    }
    return count;
}

std::size_t CountTimelineSetBits(const th07::Th07EclTimelineOpcodeMask &mask)
{
    std::size_t count = 0;
    for (std::size_t opcode = 0; opcode < mask.size(); opcode++) {
        if (mask.test(opcode)) {
            count++;
        }
    }
    return count;
}

} // namespace

int main()
{
    static_assert(th07::kTh07EclHeaderSize == 0x0040);
    static_assert(th07::kTh07EclSubOffsetTableOffset == 0x0040);
    static_assert(th07::kTh07EclSubOffsetTableSentinelEntries == 1);
    static_assert(th07::kTh07EclSubHeaderSize == 0x0004);
    static_assert(th07::kTh07EclRawInstructionHeaderSize == 0x0008);
    static_assert(th07::kTh07EclTimelineInstructionHeaderSize == 0x0008);
    static_assert(th07::kTh07EclTimelineInstructionMaxSize == 0x0020);
    static_assert(th07::kTh07EclTimelineArgumentMaxSize == 0x0018);
    static_assert(th07::kTh07EclRawInstructionTerminatorOpcode == -1);
    static_assert(th07::kTh07EclRawInstructionTerminatorSize == 0x000c);
    static_assert(th07::kTh07EclTimelineTerminatorTime == -1);
    static_assert(th07::kTh07EclMaxTimelineCount == 3);
    static_assert(th07::kTh07EclMaxOpcode == 0x00a1);
    static_assert(th07::kTh07EclMaxTimelineOpcode == 0x000c);

    std::array<std::size_t, th07::kTh07EclMaxOpcode + 1> aggregateSubOpcodes{};
    std::size_t totalSubs = 0;
    std::size_t totalTimelines = 0;
    std::size_t totalSubInstructions = 0;
    std::size_t totalSubTerminators = 0;
    std::size_t totalSubArgumentBytes = 0;
    std::size_t totalTimelineInstructions = 0;
    std::size_t totalTimelineTerminators = 0;
    std::size_t totalTimelineArgumentBytes = 0;
    th07::Th07EclOpcodeMask aggregateSubMask;
    th07::Th07EclTimelineOpcodeMask aggregateTimelineMask;

    for (const EclStageSummary &summary : kEclStageSummaries) {
        const std::filesystem::path binaryPath =
            th07::ExtractedReferenceRoot() / ("ecldata" + std::to_string(summary.stage) + ".ecl");
        const std::filesystem::path declPath =
            th07::ReferenceRoot() / "ecl" / ("ecldata" + std::to_string(summary.stage) + ".decl");
        const std::vector<u8> bytes = ReadWholeFile(binaryPath);
        const std::string declText = ReadTextFile(declPath);

        th07::Th07EclFile file;
        std::string error;
        if (!th07::ParseTh07EclFile(bytes, &file, &error)) {
            std::fprintf(stderr, "stage %u parse failed: %s\n", summary.stage, error.c_str());
            assert(false);
        }
        assert(error.empty());
        assert(bytes.size() == summary.fileSize);
        assert(file.header.subCount == summary.subCount);
        assert(file.header.timelineCount == summary.timelineCount);
        assert(file.subOffsets.front() == summary.firstSubOffset);
        assert(file.header.timelineOffsets[0] == summary.firstTimelineOffset);
        assert(file.subs.size() == summary.subCount);
        assert(file.timelines.size() == summary.timelineCount);
        assert(th07::CountTh07EclSubInstructions(file) == summary.subInstructions);
        assert(th07::CountTh07EclSubTerminators(file) == summary.subTerminators);
        assert(th07::CountTh07EclSubArgumentBytes(file) == summary.subArgumentBytes);
        assert(th07::CountTh07EclTimelineInstructions(file) == summary.timelineInstructions);
        assert(th07::CountTh07EclTimelineTerminators(file) == summary.timelineTerminators);
        assert(th07::CountTh07EclTimelineArgumentBytes(file) == summary.timelineArgumentBytes);
        const th07::Th07EclOpcodeMask subMask = th07::BuildTh07EclSubOpcodeMask(file);
        const th07::Th07EclTimelineOpcodeMask timelineMask = th07::BuildTh07EclTimelineOpcodeMask(file);
        assert(CountSetBits(subMask) == summary.subOpcodeCount);
        assert(subMask.test(summary.maxSubOpcode));
        if (summary.maxSubOpcode < th07::kTh07EclMaxOpcode) {
            assert(!subMask.test(static_cast<std::size_t>(summary.maxSubOpcode) + 1));
        }
        assert(CountTimelineSetBits(timelineMask) == summary.timelineOpcodeCount);
        assert(timelineMask.test(summary.maxTimelineOpcode));
        assert(th07::CountTh07EclSubInstructions(file) +
                   th07::CountTh07EclTimelineInstructions(file) +
                   th07::CountTh07EclTimelineTerminators(file) ==
               CountSubstring(declText, "ins_"));

        for (std::size_t opcode = 0; opcode < aggregateSubOpcodes.size(); opcode++) {
            aggregateSubOpcodes[opcode] += th07::CountTh07EclSubOpcode(file, opcode);
            if (subMask.test(opcode)) {
                aggregateSubMask.set(opcode);
            }
        }
        for (std::size_t opcode = 0; opcode < aggregateTimelineMask.size(); opcode++) {
            if (timelineMask.test(opcode)) {
                aggregateTimelineMask.set(opcode);
            }
        }
        totalSubs += file.subs.size();
        totalTimelines += file.timelines.size();
        totalSubInstructions += th07::CountTh07EclSubInstructions(file);
        totalSubTerminators += th07::CountTh07EclSubTerminators(file);
        totalSubArgumentBytes += th07::CountTh07EclSubArgumentBytes(file);
        totalTimelineInstructions += th07::CountTh07EclTimelineInstructions(file);
        totalTimelineTerminators += th07::CountTh07EclTimelineTerminators(file);
        totalTimelineArgumentBytes += th07::CountTh07EclTimelineArgumentBytes(file);
    }

    assert(totalSubs == 762);
    assert(totalTimelines == 17);
    assert(totalSubInstructions == 19919);
    assert(totalSubTerminators == 762);
    assert(totalSubArgumentBytes == 302848);
    assert(totalTimelineInstructions == 2494);
    assert(totalTimelineTerminators == 16);
    assert(totalTimelineArgumentBytes == 58064);
    assert(CountSetBits(aggregateSubMask) == th07::kTh07DeclOpcodeCount);
    assert(aggregateSubMask.test(0));
    assert(aggregateSubMask.test(161));
    assert(CountTimelineSetBits(aggregateTimelineMask) == 6);
    assert(aggregateTimelineMask.test(0));
    assert(aggregateTimelineMask.test(2));
    assert(aggregateTimelineMask.test(8));
    assert(aggregateTimelineMask.test(9));
    assert(aggregateTimelineMask.test(10));
    assert(aggregateTimelineMask.test(12));

    for (const th07::Th07EclHighOpcodeCase &highOpcode : th07::kTh07HighEclOpcodeCases) {
        assert(aggregateSubOpcodes[highOpcode.opcode] == highOpcode.declCount);
    }

    const std::vector<u8> stage1Bytes = ReadWholeFile(th07::ExtractedReferenceRoot() / "ecldata1.ecl");
    th07::Th07EclFile stage1;
    assert(th07::ParseTh07EclFile(stage1Bytes, &stage1));
    const th07::Th07EclSub *stage1Sub0 = th07::FindTh07EclSub(stage1, 0);
    assert(stage1Sub0 != nullptr);
    assert(stage1Sub0->offset == 0x012c);
    assert(stage1Sub0->prologue == 0);
    assert(stage1Sub0->instructions.size() == 5);
    assert(stage1Sub0->instructions[0].opcode == 119);
    assert(stage1Sub0->instructions[0].byteSize == 0x10);
    assert(stage1Sub0->instructions[0].time == -256);
    assert(stage1Sub0->instructions[0].operandMask == 0x0001);
    assert(stage1Sub0->instructions[2].opcode == 154);
    assert(stage1Sub0->instructions.back().isTerminator);

    const th07::Th07EclTimeline *stage1Timeline0 = th07::FindTh07EclTimeline(stage1, 0);
    assert(stage1Timeline0 != nullptr);
    assert(stage1Timeline0->offset == 0x8a00);
    assert(stage1Timeline0->instructions[0].time == 600);
    assert(stage1Timeline0->instructions[0].argument0 == 3);
    assert(stage1Timeline0->instructions[0].opcode == 2);
    assert(stage1Timeline0->instructions[0].byteSize == th07::kTh07EclTimelineInstructionMaxSize);
    assert(th07::FindTh07EclSub(stage1, 58) == nullptr);
    assert(th07::FindTh07EclTimeline(stage1, 2) == nullptr);

    const std::vector<u8> stage8Bytes = ReadWholeFile(th07::ExtractedReferenceRoot() / "ecldata8.ecl");
    th07::Th07EclFile stage8;
    assert(th07::ParseTh07EclFile(stage8Bytes, &stage8));
    assert(th07::CountTh07EclSubOpcode(stage8, 161) == 7);
    assert(th07::BuildTh07EclSubOpcodeMask(stage8).test(161));

    return 0;
}
