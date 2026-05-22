#include "Th07AnmFile.hpp"

#include <algorithm>
#include <cstring>
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

i32 ReadI32(std::span<const u8> data, std::size_t offset)
{
    return static_cast<i32>(ReadU32(data, offset));
}

f32 ReadF32(std::span<const u8> data, std::size_t offset)
{
    const std::uint32_t bits = ReadU32(data, offset);
    f32 value = 0.0f;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool ParseScript(std::span<const u8> data, std::uint32_t startOffset, std::uint32_t endOffset,
                 Th07AnmScript *script, std::string *errorMessage)
{
    script->offset = startOffset;

    for (std::size_t cursor = startOffset; cursor + sizeof(i16) <= endOffset;) {
        const i16 opcode = ReadI16(data, cursor);
        if (opcode < 0) {
            if (opcode != kTh07AnmVmTerminatorOpcode) {
                return Fail(errorMessage, "ANM script terminator opcode changed");
            }
            if (cursor + sizeof(i32) <= endOffset &&
                ReadU16(data, cursor + sizeof(i16)) != kTh07AnmVmTerminatorSize) {
                return Fail(errorMessage, "ANM script terminator size changed");
            }
            return true;
        }

        if (!HasRange(data, cursor, kTh07AnmVmInstructionHeaderSize) ||
            cursor + kTh07AnmVmInstructionHeaderSize > endOffset) {
            return Fail(errorMessage, "ANM instruction header exceeds script bounds");
        }
        const std::uint16_t byteSize = ReadU16(data, cursor + 2);
        if (byteSize < kTh07AnmVmInstructionHeaderSize || (byteSize % 4) != 0) {
            return Fail(errorMessage, "ANM instruction size out of range");
        }
        if (!HasRange(data, cursor, byteSize) || cursor + byteSize > endOffset) {
            return Fail(errorMessage, "ANM instruction exceeds script bounds");
        }
        if (static_cast<std::uint16_t>(opcode) > kTh07AnmVmParserMaxOpcode) {
            std::ostringstream message;
            message << "ANM opcode " << opcode << " exceeds current TH07 evidence";
            return Fail(errorMessage, message.str());
        }

        Th07AnmVmInstruction instruction;
        instruction.opcode = opcode;
        instruction.byteSize = byteSize;
        instruction.time = ReadI16(data, cursor + 4);
        instruction.operandFlags = ReadU16(data, cursor + 6);
        const std::size_t argumentOffset = cursor + kTh07AnmVmInstructionHeaderSize;
        instruction.arguments.assign(data.begin() + static_cast<std::ptrdiff_t>(argumentOffset),
                                     data.begin() + static_cast<std::ptrdiff_t>(cursor + byteSize));
        script->instructions.push_back(std::move(instruction));
        cursor += byteSize;
    }

    return Fail(errorMessage, "ANM script is missing its terminator");
}

} // namespace

bool ParseTh07AnmFile(std::span<const u8> data, Th07AnmFile *outFile, std::string *errorMessage)
{
    if (outFile == nullptr) {
        return Fail(errorMessage, "output pointer is null");
    }
    *outFile = {};
    if (!HasRange(data, 0, kTh07AnmHeaderMinimumSize)) {
        return Fail(errorMessage, "ANM file is too small for a TH07 entry header");
    }

    Th07AnmHeader &header = outFile->header;
    header.spriteCount = ReadI32(data, 0);
    header.scriptCount = ReadI32(data, 4);
    header.textureWidth = ReadI32(data, 12);
    header.textureHeight = ReadI32(data, 16);
    header.nameOffset = ReadU32(data, 28);
    header.mipmapNameOffset = ReadU32(data, 36);
    header.version = ReadU32(data, 40);
    header.textureOffset = ReadU32(data, 48);
    header.hasTextureData = ReadU32(data, 52) != 0;

    if (header.spriteCount < 0 || header.spriteCount > kTh07AnmMaxSpriteCount) {
        return Fail(errorMessage, "ANM sprite count out of range");
    }
    if (header.scriptCount < 0 || header.scriptCount > kTh07AnmMaxScriptCount) {
        return Fail(errorMessage, "ANM script count out of range");
    }
    if (header.textureWidth < 0 || header.textureWidth > kTh07AnmMaxTextureDimension) {
        return Fail(errorMessage, "ANM texture width out of range");
    }
    if (header.textureHeight < 0 || header.textureHeight > kTh07AnmMaxTextureDimension) {
        return Fail(errorMessage, "ANM texture height out of range");
    }
    if (header.version != 0 && header.version != 2) {
        return Fail(errorMessage, "ANM version is unexpected");
    }
    if (header.nameOffset != 0 && header.nameOffset >= data.size()) {
        return Fail(errorMessage, "ANM name offset out of range");
    }
    if (header.mipmapNameOffset != 0 && header.mipmapNameOffset >= data.size()) {
        return Fail(errorMessage, "ANM mipmap name offset out of range");
    }
    if (header.hasTextureData && header.textureOffset >= data.size()) {
        return Fail(errorMessage, "ANM texture data offset out of range");
    }

    const std::size_t scriptTableOffset =
        kTh07AnmSpriteOffsetTableOffset + static_cast<std::size_t>(header.spriteCount) * sizeof(std::uint32_t);
    if (!HasRange(data, scriptTableOffset,
                  static_cast<std::size_t>(header.scriptCount) * kTh07AnmScriptTableEntrySize)) {
        return Fail(errorMessage, "ANM sprite/script table exceeds file bounds");
    }

    for (i32 index = 0; index < header.spriteCount; index++) {
        const std::uint32_t spriteOffset =
            ReadU32(data, kTh07AnmSpriteOffsetTableOffset + static_cast<std::size_t>(index) * sizeof(std::uint32_t));
        if (!HasRange(data, spriteOffset, kTh07AnmSpriteRecordMinimumSize)) {
            return Fail(errorMessage, "ANM sprite offset out of range");
        }

        Th07AnmSprite sprite;
        sprite.offset = spriteOffset;
        sprite.localId = ReadU32(data, spriteOffset);
        sprite.x = ReadF32(data, spriteOffset + 4);
        sprite.y = ReadF32(data, spriteOffset + 8);
        sprite.width = ReadF32(data, spriteOffset + 12);
        sprite.height = ReadF32(data, spriteOffset + 16);
        outFile->sprites.push_back(sprite);
    }

    std::vector<std::uint32_t> uniqueScriptOffsets;
    for (i32 index = 0; index < header.scriptCount; index++) {
        const std::size_t entryOffset =
            scriptTableOffset + static_cast<std::size_t>(index) * kTh07AnmScriptTableEntrySize;
        Th07AnmScriptEntry entry;
        entry.localId = ReadU32(data, entryOffset);
        entry.offset = ReadU32(data, entryOffset + 4);
        if (!HasRange(data, entry.offset, sizeof(i32))) {
            return Fail(errorMessage, "ANM script offset out of range");
        }
        outFile->scriptEntries.push_back(entry);
        uniqueScriptOffsets.push_back(entry.offset);
    }

    std::sort(uniqueScriptOffsets.begin(), uniqueScriptOffsets.end());
    uniqueScriptOffsets.erase(std::unique(uniqueScriptOffsets.begin(), uniqueScriptOffsets.end()),
                              uniqueScriptOffsets.end());
    const std::uint32_t scriptEndBoundary =
        header.hasTextureData ? header.textureOffset : static_cast<std::uint32_t>(data.size());
    for (std::size_t index = 0; index < uniqueScriptOffsets.size(); index++) {
        const std::uint32_t startOffset = uniqueScriptOffsets[index];
        const std::uint32_t endOffset =
            index + 1 < uniqueScriptOffsets.size() ? uniqueScriptOffsets[index + 1] : scriptEndBoundary;
        if (startOffset >= endOffset || endOffset > data.size()) {
            return Fail(errorMessage, "ANM script range is empty or inverted");
        }

        Th07AnmScript script;
        if (!ParseScript(data, startOffset, endOffset, &script, errorMessage)) {
            *outFile = {};
            return false;
        }
        outFile->uniqueScripts.push_back(std::move(script));
    }

    return true;
}

const Th07AnmScript *FindTh07AnmScript(const Th07AnmFile &file, std::uint32_t localScriptId)
{
    for (const Th07AnmScriptEntry &entry : file.scriptEntries) {
        if (entry.localId == localScriptId) {
            return FindTh07AnmScriptByOffset(file, entry.offset);
        }
    }
    return nullptr;
}

const Th07AnmScript *FindTh07AnmScriptByOffset(const Th07AnmFile &file, std::uint32_t offset)
{
    for (const Th07AnmScript &script : file.uniqueScripts) {
        if (script.offset == offset) {
            return &script;
        }
    }
    return nullptr;
}

const Th07AnmSprite *FindTh07AnmSprite(const Th07AnmFile &file, std::uint32_t localSpriteId)
{
    for (const Th07AnmSprite &sprite : file.sprites) {
        if (sprite.localId == localSpriteId) {
            return &sprite;
        }
    }
    return nullptr;
}

std::size_t CountTh07AnmVmInstructions(const Th07AnmFile &file)
{
    std::size_t count = 0;
    for (const Th07AnmScript &script : file.uniqueScripts) {
        count += script.instructions.size();
    }
    return count;
}

std::size_t CountTh07AnmVmTerminators(const Th07AnmFile &file)
{
    return file.uniqueScripts.size();
}

std::size_t CountTh07AnmVmOpcode(const Th07AnmFile &file, std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07AnmScript &script : file.uniqueScripts) {
        for (const Th07AnmVmInstruction &instruction : script.instructions) {
            if (static_cast<std::uint16_t>(instruction.opcode) == opcode) {
                count++;
            }
        }
    }
    return count;
}

std::size_t CountTh07AnmVmInstructionSize(const Th07AnmFile &file, std::uint16_t byteSize)
{
    std::size_t count = 0;
    for (const Th07AnmScript &script : file.uniqueScripts) {
        for (const Th07AnmVmInstruction &instruction : script.instructions) {
            if (instruction.byteSize == byteSize) {
                count++;
            }
        }
    }
    return count;
}

std::uint32_t CountTh07AnmVmArgumentBytes(const Th07AnmFile &file)
{
    std::uint32_t count = 0;
    for (const Th07AnmScript &script : file.uniqueScripts) {
        for (const Th07AnmVmInstruction &instruction : script.instructions) {
            count += instruction.byteSize - kTh07AnmVmInstructionHeaderSize;
        }
    }
    return count;
}

Th07AnmVmOpcodeMask BuildTh07AnmVmOpcodeMask(const Th07AnmFile &file)
{
    Th07AnmVmOpcodeMask mask;
    for (const Th07AnmScript &script : file.uniqueScripts) {
        for (const Th07AnmVmInstruction &instruction : script.instructions) {
            mask.set(static_cast<std::uint16_t>(instruction.opcode));
        }
    }
    return mask;
}

} // namespace th07
