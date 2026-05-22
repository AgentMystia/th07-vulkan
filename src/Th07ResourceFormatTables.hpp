#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace th07 {

inline constexpr std::uint16_t kTh07AnmFileCount = 63;
inline constexpr std::uint16_t kTh07AnmEntryCount = 63;
inline constexpr std::uint16_t kTh07AnmSpriteCount = 1187;
inline constexpr std::uint16_t kTh07AnmScriptCount = 727;
inline constexpr std::uint16_t kTh07AnmVmScriptInstructionCount = 6214;
inline constexpr std::uint16_t kTh07AnmVmScriptTerminatorCount = 727;
inline constexpr std::uint8_t kTh07AnmVmOpcodeCount = 44;
inline constexpr std::uint8_t kTh07AnmVmMaxOpcode = 80;
inline constexpr std::uint8_t kTh07AnmVmOpcodeUsageCount = 44;
inline constexpr std::uint8_t kTh07AnmVmOpcodeArgShapeCount = 44;
inline constexpr std::uint16_t kTh07AnmVmTotalArgByteCount = 38724;
inline constexpr std::uint8_t kTh07AnmVmInstructionSizeClassCount = 6;

inline constexpr std::uint8_t kTh07StdFileCount = 8;
inline constexpr std::uint16_t kTh07StdBinaryObjectCount = 45;
inline constexpr std::uint16_t kTh07StdBinaryQuadCount = 162;
inline constexpr std::uint16_t kTh07StdBinaryInstanceCount = 906;
inline constexpr std::uint16_t kTh07StdBinaryScriptInstructionCount = 378;
inline constexpr std::uint8_t kTh07StdBinaryOpcodeUsageCount = 25;
inline constexpr std::uint8_t kTh07StdBinaryOpcodeArgShapeCount = 25;
inline constexpr std::uint16_t kTh07StdBinaryTotalArgByteCount = 4536;
inline constexpr std::uint8_t kTh07ReadableStdDumpCount = 7;
inline constexpr std::uint8_t kTh07EmptyReadableStdDumpStage = 6;

inline constexpr std::uint8_t kTh07MsgDumpCount = 8;
inline constexpr std::uint16_t kTh07MsgEntryCount = 60;
inline constexpr std::uint16_t kTh07MsgTimelineLabelCount = 1098;
inline constexpr std::uint16_t kTh07MsgBinaryEntryPointerCount = 180;
inline constexpr std::uint16_t kTh07MsgBinaryUniqueEntryCount = 60;
inline constexpr std::uint16_t kTh07MsgBinaryInstructionCount = 4883;
inline constexpr std::uint16_t kTh07MsgBinaryTextByteCount = 27832;
inline constexpr std::uint32_t kTh07MsgBinaryWaitTotalFrames = 313330;
inline constexpr std::uint8_t kTh07MsgBinaryMaxOpcode = 14;
inline constexpr std::uint8_t kTh07MsgBinaryOpcodeUsageCount = 15;
inline constexpr std::uint8_t kTh07MsgBinaryOpcodeArgShapeCount = 15;
inline constexpr std::uint32_t kTh07MsgBinaryTotalArgByteCount = 46572;

struct Th07AnmFileSummary {
    std::string_view fileName;
    std::uint16_t sprites;
    std::uint16_t scripts;
    std::uint16_t textureWidth;
    std::uint16_t textureHeight;
    std::uint8_t version;
    bool hasTextureData;
};

struct Th07AnmVmInstructionSizeSummary {
    std::uint16_t size;
    std::uint16_t instructions;
};

struct Th07AnmVmOpcodeUsage {
    std::uint8_t opcode;
    std::uint16_t instructions;
};

struct Th07AnmVmOpcodeArgShape {
    std::uint8_t opcode;
    std::uint16_t instructions;
    std::uint8_t minArgSize;
    std::uint8_t maxArgSize;
    std::uint16_t totalArgBytes;
};

struct Th07StdBinarySummary {
    std::uint8_t stage;
    std::uint16_t objects;
    std::uint16_t quads;
    std::uint16_t instances;
    std::uint16_t scriptInstructions;
    std::uint64_t opcodeMask;
    bool readableDumpAvailable;

    bool hasOpcode(std::uint8_t opcode) const;
};

struct Th07StdBinaryOpcodeUsage {
    std::uint8_t opcode;
    std::uint16_t instructions;
};

struct Th07StdBinaryOpcodeArgShape {
    std::uint8_t opcode;
    std::uint16_t instructions;
    std::uint8_t minArgSize;
    std::uint8_t maxArgSize;
    std::uint16_t totalArgBytes;
};

struct Th07MsgDumpSummary {
    std::uint8_t stage;
    std::uint16_t entries;
    std::uint16_t timelineLabels;
};

struct Th07MsgBinarySummary {
    std::uint8_t stage;
    std::uint32_t fileSize;
    std::uint16_t entryPointers;
    std::uint16_t uniqueEntries;
    std::uint16_t instructions;
    std::uint32_t opcodeMask;
    std::uint16_t textBytes;
    std::uint32_t waitTotalFrames;

    bool hasOpcode(std::uint8_t opcode) const;
};

struct Th07MsgBinaryOpcodeUsage {
    std::uint8_t opcode;
    std::uint16_t instructions;
};

struct Th07MsgBinaryOpcodeArgShape {
    std::uint8_t opcode;
    std::uint16_t instructions;
    std::uint8_t minArgSize;
    std::uint8_t maxArgSize;
    std::uint32_t totalArgBytes;
};

extern const std::array<Th07AnmFileSummary, kTh07AnmFileCount> kTh07AnmFileSummaries;
extern const std::array<std::uint8_t, kTh07AnmVmOpcodeCount> kTh07AnmVmOpcodes;
extern const std::array<Th07AnmVmOpcodeUsage, kTh07AnmVmOpcodeUsageCount> kTh07AnmVmOpcodeUsage;
extern const std::array<Th07AnmVmOpcodeArgShape, kTh07AnmVmOpcodeArgShapeCount>
    kTh07AnmVmOpcodeArgShapes;
extern const std::array<Th07AnmVmInstructionSizeSummary, kTh07AnmVmInstructionSizeClassCount>
    kTh07AnmVmInstructionSizeSummaries;
extern const std::array<Th07StdBinarySummary, kTh07StdFileCount> kTh07StdBinarySummaries;
extern const std::array<Th07StdBinaryOpcodeUsage, kTh07StdBinaryOpcodeUsageCount>
    kTh07StdBinaryOpcodeUsage;
extern const std::array<Th07StdBinaryOpcodeArgShape, kTh07StdBinaryOpcodeArgShapeCount>
    kTh07StdBinaryOpcodeArgShapes;
extern const std::array<Th07MsgDumpSummary, kTh07MsgDumpCount> kTh07MsgDumpSummaries;
extern const std::array<Th07MsgBinarySummary, kTh07MsgDumpCount> kTh07MsgBinarySummaries;
extern const std::array<Th07MsgBinaryOpcodeUsage, kTh07MsgBinaryOpcodeUsageCount>
    kTh07MsgBinaryOpcodeUsage;
extern const std::array<Th07MsgBinaryOpcodeArgShape, kTh07MsgBinaryOpcodeArgShapeCount>
    kTh07MsgBinaryOpcodeArgShapes;

const Th07AnmFileSummary *FindTh07AnmFileSummary(std::string_view fileName);
const Th07AnmVmInstructionSizeSummary *FindTh07AnmVmInstructionSizeSummary(std::uint16_t size);
const Th07AnmVmOpcodeUsage *FindTh07AnmVmOpcodeUsage(std::uint8_t opcode);
const Th07AnmVmOpcodeArgShape *FindTh07AnmVmOpcodeArgShape(std::uint8_t opcode);
bool IsTh07AnmVmOpcodeUsed(std::uint8_t opcode);
const Th07StdBinarySummary *FindTh07StdBinarySummary(std::uint8_t stage);
const Th07StdBinaryOpcodeUsage *FindTh07StdBinaryOpcodeUsage(std::uint8_t opcode);
const Th07StdBinaryOpcodeArgShape *FindTh07StdBinaryOpcodeArgShape(std::uint8_t opcode);
const Th07MsgDumpSummary *FindTh07MsgDumpSummary(std::uint8_t stage);
const Th07MsgBinarySummary *FindTh07MsgBinarySummary(std::uint8_t stage);
const Th07MsgBinaryOpcodeUsage *FindTh07MsgBinaryOpcodeUsage(std::uint8_t opcode);
const Th07MsgBinaryOpcodeArgShape *FindTh07MsgBinaryOpcodeArgShape(std::uint8_t opcode);
std::uint16_t CountTh07AnmSprites();
std::uint16_t CountTh07AnmScripts();
std::uint16_t CountTh07AnmFilesWithTextureData();
std::uint16_t CountTh07AnmVmScriptInstructionsBySize();
std::uint16_t CountTh07AnmVmOpcodeUsageInstructions();
std::uint16_t CountTh07AnmVmOpcodeArgBytes();
std::uint16_t CountTh07StdBinaryObjects();
std::uint16_t CountTh07StdBinaryQuads();
std::uint16_t CountTh07StdBinaryInstances();
std::uint16_t CountTh07StdBinaryScriptInstructions();
std::uint16_t CountTh07StdBinaryOpcodeUsageInstructions();
std::uint16_t CountTh07StdBinaryOpcodeArgBytes();
std::uint64_t BuildTh07StdBinaryOpcodeMask();
std::uint16_t CountTh07MsgEntries();
std::uint16_t CountTh07MsgTimelineLabels();
std::uint16_t CountTh07MsgBinaryEntryPointers();
std::uint16_t CountTh07MsgBinaryUniqueEntries();
std::uint16_t CountTh07MsgBinaryInstructions();
std::uint16_t CountTh07MsgBinaryTextBytes();
std::uint32_t CountTh07MsgBinaryWaitTotalFrames();
std::uint32_t BuildTh07MsgBinaryOpcodeMask();
std::uint16_t CountTh07MsgBinaryOpcodeUsageInstructions();
std::uint32_t CountTh07MsgBinaryOpcodeArgBytes();

} // namespace th07
