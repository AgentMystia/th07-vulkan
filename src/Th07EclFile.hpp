#pragma once

#include "inttypes.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace th07 {

inline constexpr std::uint32_t kTh07EclHeaderSize = 0x0040;
inline constexpr std::uint32_t kTh07EclSubOffsetTableOffset = 0x0040;
inline constexpr std::uint32_t kTh07EclSubOffsetTableSentinelEntries = 1;
inline constexpr std::uint32_t kTh07EclSubHeaderSize = 0x0004;
inline constexpr std::uint32_t kTh07EclRawInstructionHeaderSize = 0x0008;
inline constexpr std::uint32_t kTh07EclTimelineInstructionHeaderSize = 0x0008;
inline constexpr std::uint32_t kTh07EclTimelineInstructionMaxSize = 0x0020;
inline constexpr std::uint32_t kTh07EclTimelineArgumentMaxSize = 0x0018;
inline constexpr i16 kTh07EclRawInstructionTerminatorOpcode = -1;
inline constexpr std::uint16_t kTh07EclRawInstructionTerminatorSize = 0x000c;
inline constexpr i16 kTh07EclTimelineTerminatorTime = -1;
inline constexpr std::uint16_t kTh07EclMaxSubCount = 512;
inline constexpr std::uint16_t kTh07EclMaxTimelineCount = 3;
inline constexpr std::uint16_t kTh07EclMaxOpcode = 0x00a1;
inline constexpr std::uint16_t kTh07EclMaxTimelineOpcode = 0x000c;

using Th07EclOpcodeMask = std::bitset<kTh07EclMaxOpcode + 1>;
using Th07EclTimelineOpcodeMask = std::bitset<kTh07EclMaxTimelineOpcode + 1>;

struct Th07EclHeader {
    std::uint16_t subCount = 0;
    std::uint16_t timelineCount = 0;
    std::uint32_t timelineOffsets[kTh07EclMaxTimelineCount] = {};
};

struct Th07EclRawInstruction {
    std::uint32_t offset = 0;
    i16 opcode = 0;
    std::uint16_t byteSize = 0;
    i16 time = 0;
    std::uint16_t operandMask = 0;
    bool isTerminator = false;
    std::vector<u8> arguments;
};

struct Th07EclSub {
    std::uint16_t index = 0;
    std::uint32_t offset = 0;
    std::uint32_t prologue = 0;
    std::vector<Th07EclRawInstruction> instructions;
};

struct Th07EclTimelineInstruction {
    std::uint32_t offset = 0;
    i16 time = 0;
    i16 argument0 = 0;
    i16 opcode = 0;
    std::uint16_t byteSize = 0;
    bool isTerminator = false;
    std::vector<u8> arguments;
};

struct Th07EclTimeline {
    std::uint16_t index = 0;
    std::uint32_t offset = 0;
    std::vector<Th07EclTimelineInstruction> instructions;
};

struct Th07EclFile {
    Th07EclHeader header;
    std::vector<std::uint32_t> subOffsets;
    std::vector<Th07EclSub> subs;
    std::vector<Th07EclTimeline> timelines;
};

bool ParseTh07EclFile(std::span<const u8> data, Th07EclFile *outFile,
                      std::string *errorMessage = nullptr);
const Th07EclSub *FindTh07EclSub(const Th07EclFile &file, std::uint16_t subIndex);
const Th07EclTimeline *FindTh07EclTimeline(const Th07EclFile &file, std::uint16_t timelineIndex);
std::size_t CountTh07EclSubInstructions(const Th07EclFile &file);
std::size_t CountTh07EclSubTerminators(const Th07EclFile &file);
std::size_t CountTh07EclSubOpcode(const Th07EclFile &file, std::uint16_t opcode);
std::uint32_t CountTh07EclSubArgumentBytes(const Th07EclFile &file);
Th07EclOpcodeMask BuildTh07EclSubOpcodeMask(const Th07EclFile &file);
std::size_t CountTh07EclTimelineInstructions(const Th07EclFile &file);
std::size_t CountTh07EclTimelineTerminators(const Th07EclFile &file);
std::size_t CountTh07EclTimelineOpcode(const Th07EclFile &file, std::uint16_t opcode);
std::uint32_t CountTh07EclTimelineArgumentBytes(const Th07EclFile &file);
Th07EclTimelineOpcodeMask BuildTh07EclTimelineOpcodeMask(const Th07EclFile &file);

} // namespace th07
