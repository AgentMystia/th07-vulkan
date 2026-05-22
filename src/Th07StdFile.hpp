#pragma once

#include "inttypes.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace th07 {

inline constexpr std::uint32_t kTh07StdHeaderSize = 0x0490;
inline constexpr std::uint32_t kTh07StdObjectOffsetTableOffset = kTh07StdHeaderSize;
inline constexpr std::uint32_t kTh07StdObjectHeaderSize = 0x001c;
inline constexpr std::uint32_t kTh07StdObjectFirstQuadOffset = 0x001c;
inline constexpr std::uint32_t kTh07StdQuadMinimumSize = 0x001c;
inline constexpr std::uint32_t kTh07StdObjectInstanceSize = 0x0010;
inline constexpr std::uint32_t kTh07StdScriptInstructionSize = 0x0014;
inline constexpr std::uint32_t kTh07StdScriptArgumentSize = 0x000c;
inline constexpr std::uint32_t kTh07StdFixedStringSize = 0x0080;
inline constexpr std::uint32_t kTh07StdSongCount = 4;

struct Th07StdVector3 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
};

struct Th07StdVector2 {
    f32 x = 0.0f;
    f32 y = 0.0f;
};

struct Th07StdHeader {
    i16 objectCount = 0;
    i16 declaredQuadCount = 0;
    std::uint32_t instancesOffset = 0;
    std::uint32_t scriptOffset = 0;
    i32 unknown0c = 0;
    std::string stageName;
    std::array<std::string, kTh07StdSongCount> songNames;
    std::array<std::string, kTh07StdSongCount> songPaths;
};

struct Th07StdQuad {
    i16 type = 0;
    std::uint16_t byteSize = 0;
    i16 anmScript = 0;
    i16 vmIndex = 0;
    Th07StdVector3 position;
    Th07StdVector2 size;
};

struct Th07StdObject {
    i16 id = 0;
    i8 zLevel = 0;
    u8 flags = 0;
    Th07StdVector3 position;
    Th07StdVector3 size;
    std::vector<Th07StdQuad> quads;
};

struct Th07StdObjectInstance {
    i16 objectId = 0;
    i16 unknown2 = 0;
    Th07StdVector3 position;
};

struct Th07StdScriptInstruction {
    i32 frame = 0;
    i16 opcode = 0;
    std::uint16_t argumentSize = 0;
    std::array<i32, 3> arguments{};
};

struct Th07StdFile {
    Th07StdHeader header;
    std::vector<Th07StdObject> objects;
    std::vector<Th07StdObjectInstance> instances;
    std::vector<Th07StdScriptInstruction> script;
};

bool ParseTh07StdFile(std::span<const u8> data, Th07StdFile *outFile, std::string *errorMessage = nullptr);
std::size_t CountTh07StdQuads(const Th07StdFile &file);
std::uint64_t BuildTh07StdScriptOpcodeMask(const Th07StdFile &file);
std::size_t CountTh07StdScriptOpcode(const Th07StdFile &file, std::uint16_t opcode);
const Th07StdObject *FindTh07StdObject(const Th07StdFile &file, i16 objectId);

} // namespace th07
