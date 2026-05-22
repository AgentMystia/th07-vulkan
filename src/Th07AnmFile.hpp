#pragma once

#include "inttypes.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace th07 {

inline constexpr std::uint32_t kTh07AnmHeaderMinimumSize = 0x00b8;
inline constexpr std::uint32_t kTh07AnmSpriteOffsetTableOffset = 0x0040;
inline constexpr std::uint32_t kTh07AnmSpriteRecordMinimumSize = 0x0014;
inline constexpr std::uint32_t kTh07AnmScriptTableEntrySize = 0x0008;
inline constexpr std::uint32_t kTh07AnmVmInstructionHeaderSize = 0x0008;
inline constexpr i16 kTh07AnmVmTerminatorOpcode = -1;
inline constexpr std::uint16_t kTh07AnmVmTerminatorSize = 0;
inline constexpr std::uint16_t kTh07AnmMaxSpriteCount = 2048;
inline constexpr std::uint16_t kTh07AnmMaxScriptCount = 2048;
inline constexpr std::uint16_t kTh07AnmMaxTextureDimension = 4096;
inline constexpr std::uint16_t kTh07AnmVmParserMaxOpcode = 0x0050;

using Th07AnmVmOpcodeMask = std::bitset<kTh07AnmVmParserMaxOpcode + 1>;

struct Th07AnmHeader {
    i32 spriteCount = 0;
    i32 scriptCount = 0;
    i32 textureWidth = 0;
    i32 textureHeight = 0;
    std::uint32_t nameOffset = 0;
    std::uint32_t mipmapNameOffset = 0;
    std::uint32_t version = 0;
    std::uint32_t textureOffset = 0;
    bool hasTextureData = false;
};

struct Th07AnmSprite {
    std::uint32_t offset = 0;
    std::uint32_t localId = 0;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 width = 0.0f;
    f32 height = 0.0f;
};

struct Th07AnmScriptEntry {
    std::uint32_t localId = 0;
    std::uint32_t offset = 0;
};

struct Th07AnmVmInstruction {
    i16 opcode = 0;
    std::uint16_t byteSize = 0;
    i16 time = 0;
    std::uint16_t operandFlags = 0;
    std::vector<u8> arguments;
};

struct Th07AnmScript {
    std::uint32_t offset = 0;
    std::vector<Th07AnmVmInstruction> instructions;
};

struct Th07AnmFile {
    Th07AnmHeader header;
    std::vector<Th07AnmSprite> sprites;
    std::vector<Th07AnmScriptEntry> scriptEntries;
    std::vector<Th07AnmScript> uniqueScripts;
};

bool ParseTh07AnmFile(std::span<const u8> data, Th07AnmFile *outFile, std::string *errorMessage = nullptr);
const Th07AnmScript *FindTh07AnmScript(const Th07AnmFile &file, std::uint32_t localScriptId);
const Th07AnmScript *FindTh07AnmScriptByOffset(const Th07AnmFile &file, std::uint32_t offset);
const Th07AnmSprite *FindTh07AnmSprite(const Th07AnmFile &file, std::uint32_t localSpriteId);
std::size_t CountTh07AnmVmInstructions(const Th07AnmFile &file);
std::size_t CountTh07AnmVmTerminators(const Th07AnmFile &file);
std::size_t CountTh07AnmVmOpcode(const Th07AnmFile &file, std::uint16_t opcode);
std::size_t CountTh07AnmVmInstructionSize(const Th07AnmFile &file, std::uint16_t byteSize);
std::uint32_t CountTh07AnmVmArgumentBytes(const Th07AnmFile &file);
Th07AnmVmOpcodeMask BuildTh07AnmVmOpcodeMask(const Th07AnmFile &file);

} // namespace th07
