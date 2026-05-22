#include "Th07StdFile.hpp"

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

std::string ReadFixedString(std::span<const u8> data, std::size_t offset)
{
    std::size_t length = 0;
    while (length < kTh07StdFixedStringSize && data[offset + length] != 0) {
        length++;
    }
    return std::string(reinterpret_cast<const char *>(data.data() + offset), length);
}

Th07StdVector3 ReadVector3(std::span<const u8> data, std::size_t offset)
{
    return {ReadF32(data, offset), ReadF32(data, offset + 4), ReadF32(data, offset + 8)};
}

Th07StdVector2 ReadVector2(std::span<const u8> data, std::size_t offset)
{
    return {ReadF32(data, offset), ReadF32(data, offset + 4)};
}

bool ParseObjectQuads(std::span<const u8> data, std::size_t objectOffset, Th07StdObject *object,
                      std::string *errorMessage)
{
    std::size_t cursor = objectOffset + kTh07StdObjectFirstQuadOffset;
    while (HasRange(data, cursor, sizeof(i16))) {
        const i16 type = ReadI16(data, cursor);
        if (type < 0) {
            return true;
        }

        if (!HasRange(data, cursor, kTh07StdQuadMinimumSize)) {
            return Fail(errorMessage, "STD quad header exceeds file bounds");
        }
        const i16 signedSize = ReadI16(data, cursor + 2);
        if (signedSize < static_cast<i16>(kTh07StdQuadMinimumSize)) {
            return Fail(errorMessage, "STD quad size is smaller than the TH07 minimum");
        }
        const auto byteSize = static_cast<std::uint16_t>(signedSize);
        if (!HasRange(data, cursor, byteSize)) {
            return Fail(errorMessage, "STD quad body exceeds file bounds");
        }

        Th07StdQuad quad;
        quad.type = type;
        quad.byteSize = byteSize;
        quad.anmScript = ReadI16(data, cursor + 4);
        quad.vmIndex = ReadI16(data, cursor + 6);
        quad.position = ReadVector3(data, cursor + 8);
        quad.size = ReadVector2(data, cursor + 20);
        object->quads.push_back(quad);
        cursor += byteSize;
    }

    return Fail(errorMessage, "STD object quad list is missing its sentinel");
}

bool ParseObjects(std::span<const u8> data, Th07StdFile *outFile, std::string *errorMessage)
{
    for (i16 index = 0; index < outFile->header.objectCount; index++) {
        const std::size_t tableOffset = kTh07StdObjectOffsetTableOffset + static_cast<std::size_t>(index) * 4;
        const std::uint32_t objectOffset = ReadU32(data, tableOffset);
        if (objectOffset < kTh07StdHeaderSize ||
            !HasRange(data, objectOffset, kTh07StdObjectHeaderSize)) {
            return Fail(errorMessage, "STD object offset is outside the file");
        }

        Th07StdObject object;
        object.id = ReadI16(data, objectOffset);
        object.zLevel = static_cast<i8>(data[objectOffset + 2]);
        object.flags = data[objectOffset + 3];
        object.position = ReadVector3(data, objectOffset + 4);
        object.size = ReadVector3(data, objectOffset + 16);
        if (!ParseObjectQuads(data, objectOffset, &object, errorMessage)) {
            return false;
        }
        outFile->objects.push_back(std::move(object));
    }
    return true;
}

bool ParseInstances(std::span<const u8> data, Th07StdFile *outFile, std::string *errorMessage)
{
    bool sawSentinel = false;
    for (std::size_t cursor = outFile->header.instancesOffset;
         HasRange(data, cursor, kTh07StdObjectInstanceSize);
         cursor += kTh07StdObjectInstanceSize) {
        const i16 objectId = ReadI16(data, cursor);
        if (objectId < 0) {
            sawSentinel = true;
            break;
        }
        if (objectId >= outFile->header.objectCount) {
            return Fail(errorMessage, "STD object instance references an unknown object");
        }

        Th07StdObjectInstance instance;
        instance.objectId = objectId;
        instance.unknown2 = ReadI16(data, cursor + 2);
        instance.position = ReadVector3(data, cursor + 4);
        outFile->instances.push_back(instance);
    }
    if (!sawSentinel) {
        return Fail(errorMessage, "STD object-instance list is missing its sentinel");
    }
    return true;
}

bool ParseScript(std::span<const u8> data, Th07StdFile *outFile, std::string *errorMessage)
{
    bool sawSentinel = false;
    for (std::size_t cursor = outFile->header.scriptOffset;
         HasRange(data, cursor, kTh07StdScriptInstructionSize);
         cursor += kTh07StdScriptInstructionSize) {
        const i32 frame = ReadI32(data, cursor);
        if (frame < 0) {
            sawSentinel = true;
            break;
        }

        Th07StdScriptInstruction instruction;
        instruction.frame = frame;
        instruction.opcode = ReadI16(data, cursor + 4);
        instruction.argumentSize = ReadU16(data, cursor + 6);
        if (instruction.argumentSize != kTh07StdScriptArgumentSize) {
            return Fail(errorMessage, "STD script instruction argument size is not 0x0c");
        }
        if (instruction.opcode < 0 || instruction.opcode > 0xff) {
            return Fail(errorMessage, "STD script opcode is outside the current TH07 evidence range");
        }
        for (std::size_t idx = 0; idx < instruction.arguments.size(); idx++) {
            instruction.arguments[idx] = ReadI32(data, cursor + 8 + idx * 4);
        }
        outFile->script.push_back(instruction);
    }
    if (!sawSentinel) {
        return Fail(errorMessage, "STD script is missing its sentinel");
    }
    return true;
}

} // namespace

bool ParseTh07StdFile(std::span<const u8> data, Th07StdFile *outFile, std::string *errorMessage)
{
    if (outFile == nullptr) {
        return Fail(errorMessage, "output pointer is null");
    }
    *outFile = {};
    if (!HasRange(data, 0, kTh07StdHeaderSize)) {
        return Fail(errorMessage, "STD file is too small for a TH07 header");
    }

    Th07StdHeader &header = outFile->header;
    header.objectCount = ReadI16(data, 0);
    header.declaredQuadCount = ReadI16(data, 2);
    header.instancesOffset = ReadU32(data, 4);
    header.scriptOffset = ReadU32(data, 8);
    header.unknown0c = ReadI32(data, 12);
    if (header.objectCount <= 0 || header.objectCount > 64) {
        return Fail(errorMessage, "STD object count is outside the current TH07 evidence range");
    }
    if (header.declaredQuadCount < 0 || header.declaredQuadCount > 512) {
        return Fail(errorMessage, "STD quad count is outside the current TH07 evidence range");
    }
    if (header.instancesOffset < kTh07StdHeaderSize || !HasRange(data, header.instancesOffset, 1)) {
        return Fail(errorMessage, "STD instance offset is outside the file");
    }
    if (header.scriptOffset < kTh07StdHeaderSize || !HasRange(data, header.scriptOffset, 1)) {
        return Fail(errorMessage, "STD script offset is outside the file");
    }
    if (!HasRange(data, kTh07StdObjectOffsetTableOffset, static_cast<std::size_t>(header.objectCount) * 4)) {
        return Fail(errorMessage, "STD object offset table exceeds the file");
    }

    header.stageName = ReadFixedString(data, 0x10);
    for (std::size_t idx = 0; idx < kTh07StdSongCount; idx++) {
        header.songNames[idx] = ReadFixedString(data, 0x90 + idx * kTh07StdFixedStringSize);
        header.songPaths[idx] = ReadFixedString(data, 0x290 + idx * kTh07StdFixedStringSize);
    }

    if (!ParseObjects(data, outFile, errorMessage) ||
        !ParseInstances(data, outFile, errorMessage) ||
        !ParseScript(data, outFile, errorMessage)) {
        *outFile = {};
        return false;
    }

    if (CountTh07StdQuads(*outFile) != static_cast<std::size_t>(header.declaredQuadCount)) {
        std::ostringstream message;
        message << "STD declared " << header.declaredQuadCount << " quads but parsed "
                << CountTh07StdQuads(*outFile);
        *outFile = {};
        return Fail(errorMessage, message.str());
    }

    return true;
}

std::size_t CountTh07StdQuads(const Th07StdFile &file)
{
    std::size_t count = 0;
    for (const Th07StdObject &object : file.objects) {
        count += object.quads.size();
    }
    return count;
}

std::uint64_t BuildTh07StdScriptOpcodeMask(const Th07StdFile &file)
{
    std::uint64_t mask = 0;
    for (const Th07StdScriptInstruction &instruction : file.script) {
        mask |= 1ull << static_cast<std::uint16_t>(instruction.opcode);
    }
    return mask;
}

std::size_t CountTh07StdScriptOpcode(const Th07StdFile &file, std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07StdScriptInstruction &instruction : file.script) {
        if (static_cast<std::uint16_t>(instruction.opcode) == opcode) {
            count++;
        }
    }
    return count;
}

const Th07StdObject *FindTh07StdObject(const Th07StdFile &file, i16 objectId)
{
    for (const Th07StdObject &object : file.objects) {
        if (object.id == objectId) {
            return &object;
        }
    }
    return nullptr;
}

} // namespace th07
