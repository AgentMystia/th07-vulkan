#include "ReferencePaths.hpp"
#include "PlayerBombTables.hpp"
#include "ShtFile.hpp"
#include "Th07EffectTables.hpp"
#include "Th07ResourceManifest.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void PushU16(std::vector<std::uint8_t> &bytes, std::uint16_t value)
{
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
}

void PushI16(std::vector<std::uint8_t> &bytes, std::int16_t value)
{
    PushU16(bytes, static_cast<std::uint16_t>(value));
}

void PushU32(std::vector<std::uint8_t> &bytes, std::uint32_t value)
{
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xff));
}

void PushI32(std::vector<std::uint8_t> &bytes, std::int32_t value)
{
    PushU32(bytes, static_cast<std::uint32_t>(value));
}

void PushF32(std::vector<std::uint8_t> &bytes, float value)
{
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    std::uint32_t raw = 0;
    std::memcpy(&raw, &value, sizeof(raw));
    PushU32(bytes, raw);
}

std::int32_t F32Bits(float value)
{
    static_assert(sizeof(float) == sizeof(std::uint32_t));
    std::uint32_t raw = 0;
    std::memcpy(&raw, &value, sizeof(raw));
    return static_cast<std::int32_t>(raw);
}

void PushShot(std::vector<std::uint8_t> &bytes, std::int16_t wait, std::int16_t frame,
              float x, float y, float width, float height, float angle, float speed,
              std::uint16_t damage, std::uint8_t spawnPosition, std::uint8_t bulletType,
              std::int16_t anm, std::int16_t sound, std::int32_t spawnCallback = 0,
              std::int32_t updateCallback = 0, std::int32_t drawCallback = 0,
              std::int32_t collisionCallback = 0)
{
    PushI16(bytes, wait);
    PushI16(bytes, frame);
    PushF32(bytes, x);
    PushF32(bytes, y);
    PushF32(bytes, width);
    PushF32(bytes, height);
    PushF32(bytes, angle);
    PushF32(bytes, speed);
    PushU16(bytes, damage);
    bytes.push_back(spawnPosition);
    bytes.push_back(bulletType);
    PushI16(bytes, anm);
    PushI16(bytes, sound);
    PushI32(bytes, spawnCallback);
    PushI32(bytes, updateCallback);
    PushI32(bytes, drawCallback);
    PushI32(bytes, collisionCallback);
}

bool NearlyEqual(float lhs, float rhs)
{
    return std::fabs(lhs - rhs) < 0.0001f;
}

std::uint16_t ReadU16(const std::vector<std::uint8_t> &bytes, std::size_t offset)
{
    assert(offset + 2 <= bytes.size());
    return static_cast<std::uint16_t>(bytes[offset]) |
           static_cast<std::uint16_t>(bytes[offset + 1] << 8);
}

std::uint32_t ReadU32(const std::vector<std::uint8_t> &bytes, std::size_t offset)
{
    assert(offset + 4 <= bytes.size());
    return static_cast<std::uint32_t>(bytes[offset]) |
           (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

std::vector<std::uint8_t> ReadWholeFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("unable to open " + path.string());
    }

    file.seekg(0, std::ios::end);
    const std::streamoff size = file.tellg();
    assert(size >= 0);
    file.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    if (!bytes.empty()) {
        file.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        assert(file);
    }
    return bytes;
}

std::size_t PeVirtualAddressToFileOffset(const std::vector<std::uint8_t> &bytes, std::uint32_t address)
{
    const std::uint32_t peHeader = ReadU32(bytes, 0x3c);
    assert(std::memcmp(bytes.data() + peHeader, "PE\0\0", 4) == 0);

    const std::uint16_t sectionCount = ReadU16(bytes, peHeader + 0x06);
    const std::uint16_t optionalHeaderSize = ReadU16(bytes, peHeader + 0x14);
    const std::size_t optionalHeader = peHeader + 0x18;
    const std::uint32_t imageBase = ReadU32(bytes, optionalHeader + 0x1c);
    const std::uint32_t rva = address - imageBase;
    const std::size_t sectionTable = optionalHeader + optionalHeaderSize;

    for (std::uint16_t i = 0; i < sectionCount; ++i) {
        const std::size_t section = sectionTable + static_cast<std::size_t>(i) * 0x28;
        const std::uint32_t virtualSize = ReadU32(bytes, section + 0x08);
        const std::uint32_t virtualAddress = ReadU32(bytes, section + 0x0c);
        const std::uint32_t rawSize = ReadU32(bytes, section + 0x10);
        const std::uint32_t rawOffset = ReadU32(bytes, section + 0x14);
        const std::uint32_t mappedSize = virtualSize > rawSize ? virtualSize : rawSize;
        if (virtualAddress <= rva && rva < virtualAddress + mappedSize) {
            return rawOffset + (rva - virtualAddress);
        }
    }

    throw std::runtime_error("unable to map PE virtual address");
}

std::uint32_t ReadPeU32AtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address)
{
    return ReadU32(bytes, PeVirtualAddressToFileOffset(bytes, address));
}

std::string ReadPeCStringAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address)
{
    std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    std::string value;
    while (offset < bytes.size() && bytes[offset] != 0) {
        value.push_back(static_cast<char>(bytes[offset]));
        ++offset;
    }
    return value;
}

void CheckPeBytesAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                      std::initializer_list<std::uint8_t> expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + expected.size() <= bytes.size());
    const bool matches =
        std::equal(expected.begin(), expected.end(), bytes.begin() + static_cast<std::ptrdiff_t>(offset));
    if (!matches) {
        std::cerr << "PE byte mismatch at VA 0x" << std::hex << address << std::dec << "\n";
    }
    assert(matches);
}

std::size_t C7ImmediateOffset(const std::vector<std::uint8_t> &bytes, std::size_t offset)
{
    assert(bytes[offset] == 0xc7);
    const std::uint8_t modrm = bytes[offset + 1];
    assert(((modrm >> 3) & 7) == 0);

    std::size_t cursor = offset + 2;
    const std::uint8_t mod = modrm >> 6;
    const std::uint8_t rm = modrm & 7;
    if (rm == 4) {
        const std::uint8_t sib = bytes[cursor++];
        if (mod == 0 && (sib & 7) == 5) {
            cursor += 4;
        }
    }
    if (mod == 0) {
        if (rm == 5) {
            cursor += 4;
        }
    }
    else if (mod == 1) {
        cursor += 1;
    }
    else if (mod == 2) {
        cursor += 4;
    }

    return cursor;
}

void CheckPeC7ImmediateAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address, std::uint32_t expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    const std::size_t immediateOffset = C7ImmediateOffset(bytes, offset);
    assert(immediateOffset + 4 <= bytes.size());
    assert(ReadU32(bytes, immediateOffset) == expected);
}

void CheckPePushImmediateAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address, std::uint32_t expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + 5 <= bytes.size());
    assert(bytes[offset] == 0x68);
    assert(ReadU32(bytes, offset + 1) == expected);
}

void CheckPeNearCallTargetAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                               std::uint32_t expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + 5 <= bytes.size());
    assert(bytes[offset] == 0xe8);
    const auto displacement = static_cast<std::int32_t>(ReadU32(bytes, offset + 1));
    assert(address + 5 + displacement == expected);
}

void CheckPeMovEdxImmediateAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                                std::uint32_t expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + 5 <= bytes.size());
    assert(bytes[offset] == 0xba);
    assert(ReadU32(bytes, offset + 1) == expected);
}

void CheckPlayerBombSpeedMultiplierWrite(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                                         std::uint32_t playerOffset, std::uint32_t multiplierBits)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + 10 <= bytes.size());
    assert(bytes[offset] == 0xc7);
    assert(ReadU32(bytes, offset + 2) == playerOffset);
    assert(ReadU32(bytes, offset + 6) == multiplierBits);
}

void CheckEffectAnmLoadAtVa(const std::vector<std::uint8_t> &bytes, const th07::Th07EffectAnmLoad &load)
{
    assert(!load.archivePath.empty());
    assert(ReadPeCStringAtVa(bytes, load.archivePathAddress) == load.archivePath);
    CheckPePushImmediateAtVa(bytes, load.instructionAddress, static_cast<std::uint32_t>(load.scriptBase));
    CheckPePushImmediateAtVa(bytes, load.instructionAddress + 5, load.archivePathAddress);

    const std::size_t slotPushOffset = PeVirtualAddressToFileOffset(bytes, load.instructionAddress + 10);
    assert(slotPushOffset + 2 <= bytes.size());
    assert(bytes[slotPushOffset] == 0x6a);
    assert(bytes[slotPushOffset + 1] == static_cast<std::uint8_t>(load.anmFileSlot));
}

void CheckEffectAnmBankCountWriteAtVa(const std::vector<std::uint8_t> &bytes,
                                      const th07::Th07EffectStageAnmLoad &load)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, load.anmBankCountWriteAddress);
    assert(offset + 10 <= bytes.size());
    assert(bytes[offset] == 0xc7);
    assert(bytes[offset + 1] == 0x05);
    assert(ReadU32(bytes, offset + 2) == 0x01348020);
    assert(ReadU32(bytes, offset + 6) == static_cast<std::uint32_t>(load.anmBankCount));
}

void CheckCallbackTablesAgainstOriginalExeIfAvailable()
{
    const std::filesystem::path exePath = th07::OriginalReferenceRoot() / "th07.exe";
    if (!std::filesystem::exists(exePath)) {
        std::cout << "TH07 executable not present; callback table checks skipped\n";
        return;
    }

    const std::vector<std::uint8_t> exe = ReadWholeFile(exePath);

    for (std::size_t i = 0; i < th07::kShtOnSpawnCallbacks.size(); ++i) {
        assert(ReadPeU32AtVa(exe, th07::kShtOnSpawnCallbackTableAddress + static_cast<std::uint32_t>(i * 4)) ==
               th07::kShtOnSpawnCallbacks[i].originalAddress);
    }
    for (std::size_t i = 0; i < th07::kShtOnUpdateCallbacks.size(); ++i) {
        assert(ReadPeU32AtVa(exe, th07::kShtOnUpdateCallbackTableAddress + static_cast<std::uint32_t>(i * 4)) ==
               th07::kShtOnUpdateCallbacks[i].originalAddress);
    }
    for (std::size_t i = 0; i < th07::kShtOnDrawCallbacks.size(); ++i) {
        assert(ReadPeU32AtVa(exe, th07::kShtOnDrawCallbackTableAddress + static_cast<std::uint32_t>(i * 4)) ==
               th07::kShtOnDrawCallbacks[i].originalAddress);
    }
    for (std::size_t i = 0; i < th07::kShtOnCollisionCallbacks.size(); ++i) {
        assert(ReadPeU32AtVa(exe, th07::kShtOnCollisionCallbackTableAddress + static_cast<std::uint32_t>(i * 4)) ==
               th07::kShtOnCollisionCallbacks[i].originalAddress);
    }

    for (std::int32_t effectIdx = 0; effectIdx < th07::kTh07EffectTableCount; ++effectIdx) {
        const th07::Th07EffectTableEntry *effectInfo = th07::FindTh07EffectTableEntry(effectIdx);
        const std::uint32_t tableAddress =
            th07::kTh07EffectTableAddress + static_cast<std::uint32_t>(effectIdx * 0x0c);
        assert(effectInfo != nullptr);
        assert(ReadPeU32AtVa(exe, tableAddress) == static_cast<std::uint32_t>(effectInfo->anmScript));
        assert(ReadPeU32AtVa(exe, tableAddress + 4) == effectInfo->updateCallbackAddress);
        assert(ReadPeU32AtVa(exe, tableAddress + 8) == effectInfo->initCallbackAddress);
    }

    CheckPeBytesAtVa(exe, th07::kTh07EffectInitRadialDirectionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateOrbitingRenderMode2Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041a766, {0x05, 0x94, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a77b, {0x8b, 0x82, 0xb4, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a7c7, {0xd9, 0x98, 0xa0, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a871, {0xd8, 0x1d, 0x58, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a8fe, {0xd8, 0x0d, 0x54, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a98b, {0x0f, 0xbe, 0x82, 0xce, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041a9c2, {0x83, 0xfa, 0x10});
    CheckPeBytesAtVa(exe, 0x0041aa0b, {0xd8, 0x0d, 0xf8, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041aa27, {0xd9, 0x05, 0x70, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041aaa1, {0xd8, 0x0d, 0x60, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041aaa7, {0xd8, 0x25, 0x64, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateRadialInwardShortAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041ab16, {0xd8, 0x35, 0x48, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdatePlayerPositionWhileAnmActiveAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041abec, {0x83, 0xb8, 0xe0, 0x01, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041ac01, {0x81, 0xc2, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041ac07, {0xa1, 0x08, 0xe4, 0x4b, 0x00});
    CheckPeBytesAtVa(exe, 0x0041ac0e, {0x8b, 0x0d, 0x0c, 0xe4, 0x4b, 0x00});
    CheckPeBytesAtVa(exe, 0x0041ac17, {0xa1, 0x10, 0xe4, 0x4b, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateRadialInwardSlowAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041ac56, {0xd8, 0x35, 0x50, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateRadialOutwardAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041ad36, {0xd8, 0x35, 0x00, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdatePerspectiveMotionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041aefc, {0x05, 0x70, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041af07, {0x81, 0xc1, 0x64, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041af52, {0x05, 0x7c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041af99, {0x81, 0xc2, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041afc1, {0xd8, 0x25, 0x00, 0xcd, 0x34, 0x01});
    CheckPeBytesAtVa(exe, 0x0041b018, {0xd9, 0x05, 0x24, 0xcd, 0x34, 0x01});
    CheckPeBytesAtVa(exe, 0x0041b03d, {0xd8, 0x1d, 0x4c, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041b075, {0x83, 0xc8, 0x04});
    CheckPeBytesAtVa(exe, 0x0041b08a, {0xd8, 0x1d, 0x4c, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitPerspective0041b0b0Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041b149, {0x05, 0x7c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041b3b0, {0xc6, 0x80, 0xd0, 0x02, 0x00, 0x00, 0x01});
    CheckPeBytesAtVa(exe, 0x0041b46e, {0xc6, 0x81, 0xba, 0x01, 0x00, 0x00, 0xff});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitPerspective0041b4a0Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041b4b3, {0xd8, 0x0d, 0xc0, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041b514, {0xd8, 0xb1, 0x58, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitPerspective0041b770Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitPerspective0041b9f0Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041b9ba, {0xc7, 0x81, 0x70, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitPerspective0041bc20Address, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041bc71, {0xd8, 0x0d, 0xf8, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041be63, {0x8b, 0x45, 0xa4, 0xd9, 0x40, 0x14});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateCameraSnapAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041c190, {0xc7, 0x81, 0x54, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c19d, {0xc6, 0x82, 0xd0, 0x02, 0x00, 0x00, 0x03});
    CheckPeBytesAtVa(exe, 0x004074ca, {0xc7, 0x45, 0xfc, 0x00, 0x7b, 0x34, 0x01});
    CheckPeBytesAtVa(exe, 0x004074d1, {0xb9, 0xad, 0x14, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00407538, {0x89, 0x0d, 0xf0, 0xcd, 0x34, 0x01});
    CheckPeBytesAtVa(exe, 0x00406524, {0x81, 0xc2, 0x04, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00406531, {0x05, 0x1c, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00406537, {0xe8, 0xd0, 0xb9, 0x05, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07SetupDefaultCameraAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07SetupStageCameraAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x004082bc, {0x05, 0xf8, 0x51, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004082c7, {0x81, 0xc1, 0x04, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408310, {0x81, 0xc2, 0x10, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004083c3, {0x81, 0xc1, 0x28, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004083db, {0x81, 0xc2, 0x28, 0x52, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07RenderProjectedAnmQuadAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0044fed0, {0x68, 0x28, 0xcd, 0x34, 0x01});
    CheckPeBytesAtVa(exe, th07::kTh07EffectInitEasedRadialAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041bed2, {0xdc, 0x1d, 0xe0, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041beff, {0xd8, 0x0d, 0x60, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041bf05, {0xd8, 0x25, 0x64, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041bf7b, {0xd8, 0x0d, 0xdc, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041bf81, {0xd8, 0x05, 0x54, 0x8a, 0x49, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateEasedRadialAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041bff0, {0xd8, 0x35, 0x48, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c059, {0xd8, 0x0d, 0x24, 0x8b, 0x49, 0x00});

    CheckPeBytesAtVa(exe, th07::kTh07EffectSpawnFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07EffectSpawnWithAuxVectorFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07EffectSpawnDedicatedFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041c1f0, {0x81, 0x7d, 0xf8, 0x90, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c1ce, {0x69, 0xc9, 0xd8, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c1d7, {0x8d, 0x44, 0x0a, 0x1c});
    CheckPeBytesAtVa(exe, 0x0041c3da, {0x81, 0xc2, 0x5c, 0x88, 0x04, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c509, {0x8b, 0x55, 0xfc, 0x8b, 0x45, 0x18});
    CheckPeBytesAtVa(exe, 0x0041c56d, {0x81, 0xc2, 0x58, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c61c, {0x05, 0x90, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c621, {0x69, 0xc0, 0xd8, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c62a, {0x8d, 0x54, 0x01, 0x1c});
    CheckPeBytesAtVa(exe, 0x0041c634, {0xc6, 0x80, 0xd0, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c63e, {0xc6, 0x81, 0xcc, 0x02, 0x00, 0x00, 0x01});
    CheckPeBytesAtVa(exe, 0x0041c64b, {0x88, 0x82, 0xcd, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c657, {0x81, 0xc2, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c673, {0x8b, 0x82, 0xc0, 0xef, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c68c, {0x66, 0x89, 0x82, 0xd8, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c6b6, {0x0d, 0x00, 0x10, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c6ca, {0x89, 0x82, 0xb8, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c6df, {0x89, 0x82, 0xc8, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c708, {0xc7, 0x01, 0x19, 0xfc, 0xff, 0xff});
    CheckPeBytesAtVa(exe, 0x0041c711, {0xc6, 0x82, 0xce, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c71b, {0xc6, 0x80, 0xcf, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectUpdateFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041c7a5, {0xc7, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c7af, {0x81, 0xc2, 0x34, 0x8b, 0x04, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c7f6, {0xc7, 0x81, 0x08, 0x8e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c844, {0x81, 0x7d, 0xf8, 0x98, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c873, {0x83, 0xb8, 0xc8, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c882, {0xff, 0x92, 0xc8, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c888, {0x83, 0xf8, 0x01});
    CheckPeBytesAtVa(exe, 0x0041c8be, {0x05, 0xb8, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c8ec, {0xc7, 0x80, 0xd4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c9d7, {0xb9, 0x2c, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0041c9de, {0x83, 0xfa, 0x64});
    CheckPeBytesAtVa(exe, 0x0041c9f1, {0xb8, 0x04, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07EffectLoadStageAnmFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0041cdf1, {0xc7, 0x05, 0x24, 0x80, 0x34, 0x01, 0x00, 0x00, 0x00, 0x00});
    for (const th07::Th07EffectStageAnmLoad &load : th07::kTh07EffectStageAnmLoads) {
        CheckEffectAnmBankCountWriteAtVa(exe, load);
        for (const th07::Th07EffectAnmLoad &anmLoad : load.loads) {
            if (!anmLoad.archivePath.empty()) {
                CheckEffectAnmLoadAtVa(exe, anmLoad);
            }
        }
    }

    CheckPeBytesAtVa(exe, th07::kPlayerBombUpdateUnfocusedCallAddress,
                     {0xff, 0x92, 0x3c, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombUpdateFocusedCallAddress,
                     {0xff, 0x90, 0x44, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombStartupUnfocusedCallAddress,
                     {0xff, 0x92, 0x3c, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombStartupFocusedCallAddress,
                     {0xff, 0x90, 0x44, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombDrawUnfocusedCallAddress,
                     {0xff, 0x92, 0x40, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombDrawFocusedCallAddress,
                     {0xff, 0x90, 0x48, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x00440bf1, {0x89, 0x8a, 0x24, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x00440bfa, {0xc7, 0x80, 0x20, 0x6a, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00440c3d, {0xc7, 0x80, 0x28, 0x6a, 0x01, 0x00, 0xe7, 0x03, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00440ca5, {0x83, 0xc0, 0x06});
    CheckPeBytesAtVa(exe, 0x00442043, {0x50, 0x8b, 0x0d, 0x44, 0x9e, 0x4b, 0x00});
    CheckPeBytesAtVa(exe, 0x00442060, {0x05, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442074, {0x81, 0xc1, 0x98, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442114, {0xd9, 0x98, 0xc8, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004421a2, {0xd9, 0x9a, 0x14, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004421df, {0xd9, 0x99, 0x60, 0x06, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044220d, {0x81, 0xc2, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442222, {0x05, 0x98, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442505, {0x66, 0xc7, 0x80, 0xd8, 0x01, 0x00, 0x00, 0x00, 0x04});
    CheckPeBytesAtVa(exe, 0x0044253c, {0xd9, 0x9a, 0x30, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442551, {0xd9, 0x98, 0x34, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044255a, {0xc7, 0x81, 0x38, 0x09, 0x00, 0x00, 0x48, 0xe1, 0xfa, 0x3e});
    CheckPeBytesAtVa(exe, 0x00442567, {0xc7, 0x82, 0xbc, 0x09, 0x00, 0x00, 0x48, 0xe1, 0xfa, 0x3e});
    CheckPeBytesAtVa(exe, 0x00442574, {0xc7, 0x80, 0xc8, 0x09, 0x00, 0x00, 0x48, 0xe1, 0xfa, 0x3e});
    CheckPeBytesAtVa(exe, 0x004425c1, {0xd9, 0x9a, 0x94, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004425d3, {0x89, 0x90, 0x90, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004425dc, {0xc7, 0x80, 0x98, 0x09, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x40});
    CheckPeBytesAtVa(exe, 0x004425f8, {0xd9, 0x9a, 0xa0, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044260a, {0x89, 0x90, 0x9c, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442613, {0xc7, 0x80, 0xa4, 0x09, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x40});
    CheckPeBytesAtVa(exe, 0x00442620, {0xc7, 0x81, 0xa8, 0x09, 0x00, 0x00, 0x00, 0x00, 0x40, 0x41});
    CheckPeBytesAtVa(exe, 0x0044262d, {0xc7, 0x82, 0xac, 0x09, 0x00, 0x00, 0x00, 0x00, 0x40, 0x41});
    CheckPeBytesAtVa(exe, 0x0044263a, {0xc7, 0x80, 0xb0, 0x09, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x40});
    CheckPeBytesAtVa(exe, 0x004426a4, {0x66, 0xc7, 0x81, 0xd8, 0x01, 0x00, 0x00, 0x80, 0x04});
    CheckPeBytesAtVa(exe, 0x004426e3, {0x66, 0xc7, 0x82, 0xd8, 0x01, 0x00, 0x00, 0x81, 0x04});
    CheckPeBytesAtVa(exe, th07::kPlayerInitFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kPlayerMovementFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kPlayerKillBoxCollisionFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kPlayerGrazeCollisionFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kPlayerItemBoxCollisionFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kPlayerLaserHitboxFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0043e2e7, {0xd9, 0x82, 0x48, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e30d, {0xd9, 0x81, 0x54, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e471, {0xd9, 0x80, 0x60, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e484, {0xd9, 0x81, 0x6c, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e647, {0xd9, 0x80, 0x78, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e65d, {0xd9, 0x81, 0x84, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e6c5, {0x05, 0x30, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043e7af, {0x81, 0xc2, 0x90, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f38d, {0xd9, 0x99, 0xcc, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f3ae, {0xd9, 0x98, 0xd0, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f3ba, {0x81, 0xc1, 0x30, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f4de, {0x81, 0xc1, 0x90, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f55d, {0x05, 0x48, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f5fd, {0x81, 0xc1, 0x54, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f61a, {0x05, 0x9c, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f69e, {0x81, 0xc2, 0x60, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f75c, {0x81, 0xc2, 0xa8, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f7e0, {0x81, 0xc1, 0x78, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f7fd, {0x05, 0xa8, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f881, {0x81, 0xc2, 0x84, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f8aa, {0x81, 0xc2, 0xb4, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0043f8d2, {0x05, 0xc0, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kPlayerBombCommonDrawBackdropAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x00408402, {0x83, 0xb8, 0x38, 0x6a, 0x01, 0x00, 0x3c});
    CheckPeBytesAtVa(exe, 0x0040841f, {0x6b, 0xc0, 0x50});
    CheckPeBytesAtVa(exe, 0x00408423, {0xb9, 0x3c, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408478, {0x6b, 0xc0, 0x50});
    CheckPeBytesAtVa(exe, 0x0040847c, {0xb9, 0x3c, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040849b, {0xc6, 0x45, 0xfc, 0x30});
    CheckPeBytesAtVa(exe, 0x004084c1, {0xc7, 0x00, 0x80, 0x80, 0x80, 0x80});
    CheckPeBytesAtVa(exe, th07::kPlayerBombCommonStartEffectAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x004084fc, {0x83, 0xb8, 0x68, 0x7e, 0x0b, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040850e, {0xc6, 0x82, 0xcc, 0x02, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408515, {0x6a, 0xff, 0x6a, 0x01, 0x6a, 0x00});
    CheckPeBytesAtVa(exe, 0x0040851e, {0x05, 0x30, 0x09, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408524, {0x6a, 0x19});
    CheckPeBytesAtVa(exe, 0x00408526, {0xb9, 0x50, 0xe2, 0x2f, 0x01});
    CheckPeBytesAtVa(exe, 0x0040852b, {0xe8, 0xe0, 0x40, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x0040853f, {0xc7, 0x42, 0x08, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408549, {0xc7, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00408553, {0xc7, 0x01, 0x19, 0xfc, 0xff, 0xff});
    CheckPeBytesAtVa(exe, 0x0040855c, {0x81, 0xc2, 0x00, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x0040857d, {0xc6, 0x80, 0xc4, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040859f, {0xc7, 0x82, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3d});
    CheckPeBytesAtVa(exe, 0x004085ac, {0xc7, 0x80, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3d});
    CheckPeBytesAtVa(exe, 0x004085b9, {0x8b, 0x91, 0x08, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x004085d4, {0xd8, 0x0d, 0x5c, 0x8b, 0x49, 0x00});
    CheckPeBytesAtVa(exe, 0x004085e3, {0xc6, 0x81, 0xba, 0x01, 0x00, 0x00, 0xff});
    CheckPeBytesAtVa(exe, 0x004085ed, {0xc6, 0x82, 0xb9, 0x01, 0x00, 0x00, 0x40});
    CheckPeBytesAtVa(exe, 0x004085f7, {0xc6, 0x80, 0xb8, 0x01, 0x00, 0x00, 0x40});
    CheckPeBytesAtVa(exe, 0x00408604, {0x89, 0x91, 0x68, 0x7e, 0x0b, 0x00});
    CheckPeBytesAtVa(exe, 0x00441986, {0xc6, 0x81, 0x0d, 0x24, 0x00, 0x00, 0x02});
    CheckPeBytesAtVa(exe, 0x00441995, {0x8a, 0x82, 0x08, 0x24, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004419c4, {0x83, 0xba, 0xf8, 0x23, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004419fa, {0xc7, 0x42, 0x08, 0x1c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441a38, {0xc6, 0x80, 0x0d, 0x24, 0x00, 0x00, 0x01});
    CheckPeBytesAtVa(exe, 0x00441a42, {0xc6, 0x81, 0x08, 0x24, 0x00, 0x00, 0x04});
    CheckPeBytesAtVa(exe, 0x00441a4c, {0x83, 0xba, 0x6c, 0x7e, 0x0b, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441a68, {0x83, 0xba, 0x68, 0x7e, 0x0b, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441a8e, {0x6a, 0xff, 0x6a, 0x01, 0x6a, 0x04});
    CheckPeBytesAtVa(exe, 0x00441a9d, {0x6a, 0x1c});
    CheckPeBytesAtVa(exe, 0x00441a9f, {0xb9, 0x50, 0xe2, 0x2f, 0x01});
    CheckPeBytesAtVa(exe, 0x00441b13, {0xc7, 0x81, 0x1c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f});
    CheckPeBytesAtVa(exe, 0x00441b2d, {0xc7, 0x80, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3e});
    CheckPeBytesAtVa(exe, 0x00441b74, {0x89, 0x82, 0x6c, 0x7e, 0x0b, 0x00});
    CheckPeBytesAtVa(exe, 0x00441bdc, {0x83, 0xb8, 0x6c, 0x7e, 0x0b, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441c02, {0x6a, 0xff, 0x6a, 0x01, 0x6a, 0x04});
    CheckPeBytesAtVa(exe, 0x00441c12, {0x6a, 0x1c});
    CheckPeBytesAtVa(exe, 0x00441c55, {0xc7, 0x41, 0x08, 0x1e, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441c7c, {0xc7, 0x82, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3d});
    CheckPeBytesAtVa(exe, 0x00441c96, {0xc7, 0x81, 0x20, 0x02, 0x00, 0x00, 0x66, 0x66, 0xa6, 0x3f});
    CheckPeBytesAtVa(exe, 0x00441d35, {0x89, 0x82, 0x6c, 0x7e, 0x0b, 0x00});
    CheckPeBytesAtVa(exe, 0x00441d52, {0xc6, 0x81, 0x0d, 0x24, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441d5c, {0xc6, 0x82, 0x08, 0x24, 0x00, 0x00, 0x03});
    CheckPeBytesAtVa(exe, 0x00441d71, {0xc7, 0x41, 0x08, 0x28, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441d8e, {0xc7, 0x81, 0xfc, 0x23, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441705, {0x0f, 0xbe, 0x88, 0x08, 0x24, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441756, {0xc7, 0x40, 0x08, 0xf0, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00441785, {0xc6, 0x80, 0x08, 0x24, 0x00, 0x00, 0x03});
    CheckPeBytesAtVa(exe, 0x0044179b, {0xc7, 0x42, 0x08, 0x28, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004417b8, {0xc7, 0x82, 0xfc, 0x23, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004417c5, {0xc6, 0x80, 0x0d, 0x24, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004417cf, {0x83, 0xb9, 0x6c, 0x7e, 0x0b, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004417eb, {0xc7, 0x81, 0x6c, 0x7e, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004429da, {0xc7, 0x45, 0xfc, 0xd8, 0xda, 0x4b, 0x00});
    CheckPeBytesAtVa(exe, 0x004429e1, {0xb9, 0x9e, 0xdf, 0x02, 0x00});
    CheckPeBytesAtVa(exe, 0x004429f0, {0x05, 0x00, 0x6a, 0x01, 0x00});
    CheckPeBytesAtVa(exe, 0x004429fb, {0xc7, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442a05, {0xc7, 0x42, 0x04, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442a0f, {0xc7, 0x00, 0x19, 0xfc, 0xff, 0xff});
    const std::uint32_t commonEffectTableAddress =
        th07::kPlayerBombCommonEffectTableAddress +
        static_cast<std::uint32_t>(th07::kPlayerBombCommonEffectId * 0x0c);
    assert(ReadPeU32AtVa(exe, commonEffectTableAddress) == th07::kPlayerBombCommonEffectAnmScript);
    assert(ReadPeU32AtVa(exe, commonEffectTableAddress + 4) ==
           th07::kPlayerBombCommonEffectUpdateCallbackAddress);
    assert(ReadPeU32AtVa(exe, commonEffectTableAddress + 8) ==
           th07::kPlayerBombCommonEffectInitCallbackAddress);
    for (const th07::PlayerStageColorWriteProducer &producer : th07::kPlayerStageColorWriteProducers) {
        CheckPeBytesAtVa(exe, producer.originalFunctionAddress, {0x55, 0x8b, 0xec});
        CheckPeBytesAtVa(exe, producer.callInstructionAddress - 5, {0xb9, 0x00, 0x7b, 0x34, 0x01});
        CheckPeNearCallTargetAtVa(exe, producer.callInstructionAddress,
                                  th07::kTh07StageAnmManagerColorWriteFunctionAddress);
    }
    CheckPeBytesAtVa(exe, 0x00441593, {0x81, 0x3d, 0xe0, 0x44, 0x4d, 0x00, 0xfe, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004415f3, {0x6b, 0xc0, 0x50});
    CheckPeBytesAtVa(exe, 0x004415f7, {0xb9, 0x1e, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044229f, {0x81, 0x3d, 0xe0, 0x44, 0x4d, 0x00, 0xfe, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00442303, {0xb9, 0x1e, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07GameManagerUpdateFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07GameManagerRegisterFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07GameManagerCleanupFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07ScreenEffectShakeFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0042e1bf, {0x8b, 0x80, 0xe8, 0x95, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0042e1c9, {0x89, 0x81, 0xe8, 0x95, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0042f3c9, {0xc7, 0x45, 0xfc, 0x70, 0x62, 0x62, 0x00});
    CheckPeBytesAtVa(exe, 0x0042f3ba, {0x83, 0x25, 0x58, 0xf8, 0x62, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0042f407, {0x83, 0xa0, 0xe8, 0x95, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044b0fe, {0x83, 0x3d, 0x58, 0xf8, 0x62, 0x00, 0x01});
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040933c, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3f19999a);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040934c, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3f19999a);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040a533, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040a540, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040b073, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040b083, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040b97d, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040b98a, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3ecccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040bf96, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3e4ccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040bfa3, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3e4ccccd);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040cbc5, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3e99999a);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040cbd5, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3e99999a);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040d62c, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x40000000);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040d639, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x40000000);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040dd2c, th07::kPlayerBombVerticalSpeedMultiplierOffset, 0x3fc00000);
    CheckPlayerBombSpeedMultiplierWrite(exe, 0x0040dd3c, th07::kPlayerBombHorizontalSpeedMultiplierOffset, 0x3fc00000);

    for (std::size_t shot = 0; shot < th07::kPlayerBombRoutineSets.size(); ++shot) {
        const th07::PlayerBombRoutineSet &set = th07::kPlayerBombRoutineSets[shot];
        assert(th07::FindPlayerBombRoutineSet(set.shotType) == &set);
        for (std::size_t routine = 0; routine < set.routines.size(); ++routine) {
            const th07::PlayerBombRoutineTarget &target = set.routines[routine];
            const std::uint32_t address =
                th07::kPlayerBombRoutineTableAddress + static_cast<std::uint32_t>(shot * 0x10 + routine * 4);
            assert(ReadPeU32AtVa(exe, address) == target.originalAddress);
            assert(static_cast<std::size_t>(target.slot) == routine);
            if (target.durationFrames >= 0) {
                CheckPeC7ImmediateAtVa(exe, target.durationWriteAddress,
                                       static_cast<std::uint32_t>(target.durationFrames));
                CheckPeC7ImmediateAtVa(exe, target.commonEffectDurationWriteAddress,
                                       static_cast<std::uint32_t>(target.commonEffectDurationFrames));
                CheckPeMovEdxImmediateAtVa(exe, target.minimumScoreDrainTotalLoadAddress,
                                           static_cast<std::uint32_t>(target.minimumScoreDrainTotal));
                CheckPePushImmediateAtVa(exe, target.scoreDrainFactorPushAddress, target.scoreDrainFactorBits);
                if (target.verticalSpeedMultiplierWriteAddress != 0 || target.horizontalSpeedMultiplierWriteAddress != 0) {
                    CheckPlayerBombSpeedMultiplierWrite(exe, target.verticalSpeedMultiplierWriteAddress,
                                                        th07::kPlayerBombVerticalSpeedMultiplierOffset,
                                                        target.speedMultiplierBits);
                    CheckPlayerBombSpeedMultiplierWrite(exe, target.horizontalSpeedMultiplierWriteAddress,
                                                        th07::kPlayerBombHorizontalSpeedMultiplierOffset,
                                                        target.speedMultiplierBits);
                }
            }
        }
    }
}

void CheckSyntheticFixture()
{
    std::vector<std::uint8_t> bytes;
    PushU16(bytes, 0);
    PushU16(bytes, 2);
    PushF32(bytes, 3.0f);
    PushI32(bytes, 15);
    for (const float value : {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}) {
        PushF32(bytes, value);
    }
    assert(bytes.size() == th07::kShtHeaderSize);

    PushU32(bytes, 0x44);
    PushI32(bytes, 8);
    PushU32(bytes, 0x7c);
    PushI32(bytes, 999);
    assert(bytes.size() == 0x44);

    PushShot(bytes, 5, 0, 0.0f, 0.0f, 12.0f, 12.0f, -1.5707964f, 12.0f, 48, 0, 0, 0x440, 0,
             0, 1, 1, 2);
    PushI32(bytes, -1);
    assert(bytes.size() == 0x7c);

    PushShot(bytes, 30, 0, 0.0f, 0.0f, 12.0f, 12.0f, -2.0943952f, 10.0f, 14, 1, 1, 0x441, -1,
             0, 1, 0, 3);
    PushI32(bytes, -1);

    const th07::ShtFile parsed = th07::ParseShtFile(bytes);
    assert(parsed.header.formatId == 0);
    assert(parsed.header.powerLevelCount == 2);
    assert(parsed.header.deathbombWindowFrames == 15);
    assert(NearlyEqual(parsed.header.hitboxDiagonalExtent(), 1.65f));
    assert(NearlyEqual(parsed.header.grazeDiagonalExtent(), 2.8f));
    assert(NearlyEqual(parsed.header.orthogonalMovementSpeed(), 4.0f));
    assert(NearlyEqual(parsed.header.focusedOrthogonalMovementSpeed(), 1.6f));
    assert(parsed.powerLevels.size() == 2);
    assert(parsed.powerLevels[0].recordOffset == 0x44);
    assert(parsed.powerLevels[0].terminatorOffset == 0x78);
    assert(parsed.powerLevels[0].powerThreshold == 8);
    assert(parsed.powerLevels[0].shots.size() == 1);
    assert(parsed.powerLevels[1].shots[0].callbacks.onCollision == 3);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnSpawn, 5)->originalAddress == 0x0043c1c0);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnSpawn, 5)->semantic ==
           th07::ShtCallbackSemantic::FixedAnglePeriodicSpawn);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnUpdate, 5)->ghidraName == "FUN_0043c940");
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnUpdate, 5)->semantic ==
           th07::ShtCallbackSemantic::Mode3TrailUpdate);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnDraw, 1)->originalAddress == 0x0043ccb0);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnDraw, 1)->semantic ==
           th07::ShtCallbackSemantic::TrailDraw);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnCollision, 2)->originalAddress == 0x0043d0e0);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnCollision, 2)->semantic ==
           th07::ShtCallbackSemantic::CollisionCounterSound);
    assert(th07::FindShtCallback(th07::ShtCallbackRole::OnCollision, 3) == nullptr);
    assert(th07::ShtCallbackSemanticName(th07::ShtCallbackSemantic::TrailDraw) == "trail draw");
    const th07::ResolvedShtCallbacks syntheticCallbacks = th07::ResolveShtCallbacks(parsed.powerLevels[0].shots[0]);
    assert(syntheticCallbacks.spawn->originalAddress == 0x00000000);
    assert(syntheticCallbacks.update->originalAddress == 0x0043c250);
    assert(syntheticCallbacks.draw->originalAddress == 0x0043ccb0);
    assert(syntheticCallbacks.collision->originalAddress == 0x0043d0e0);
    assert(th07::CountShtShotRecords(parsed) == 2);
    assert(th07::FindShtPowerLevelForPower(parsed, 7)->powerThreshold == 8);
    assert(th07::FindShtPowerLevelForPower(parsed, 8)->powerThreshold == 999);

    const th07::PlayerBombRoutineTarget *reimuAFocusedCalc =
        th07::FindPlayerBombRoutine(th07::Th07ShotType::ReimuA, th07::PlayerBombRoutineSlot::FocusedCalc);
    assert(reimuAFocusedCalc != nullptr);
    assert(reimuAFocusedCalc->originalAddress == 0x004091b0);
    assert(reimuAFocusedCalc->portraitAnmScript == 0x4a1);
    assert(reimuAFocusedCalc->durationFrames == 300);
    assert(reimuAFocusedCalc->commonEffectDurationFrames == 360);
    assert(reimuAFocusedCalc->minimumScoreDrainTotal == 5000);
    assert(NearlyEqual(reimuAFocusedCalc->scoreDrainFactor(), 0.22f));
    assert(NearlyEqual(reimuAFocusedCalc->speedMultiplier(), 0.6f));
    assert(reimuAFocusedCalc->durationWriteAddress == 0x00409266);
    assert(reimuAFocusedCalc->commonEffectDurationWriteAddress == 0x0040927f);
    assert(reimuAFocusedCalc->minimumScoreDrainTotalLoadAddress == 0x00409326);
    assert(reimuAFocusedCalc->scoreDrainFactorPushAddress == 0x00409321);
    assert(reimuAFocusedCalc->verticalSpeedMultiplierWriteAddress == 0x0040933c);
    assert(reimuAFocusedCalc->horizontalSpeedMultiplierWriteAddress == 0x0040934c);
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::SakuyaB, th07::PlayerBombRoutineSlot::FocusedCalc)
               ->commonEffectDurationFrames == 420);
    assert(NearlyEqual(th07::FindPlayerBombRoutine(th07::Th07ShotType::SakuyaB, th07::PlayerBombRoutineSlot::UnfocusedCalc)
                           ->speedMultiplier(),
                       2.0f));
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::MarisaA, th07::PlayerBombRoutineSlot::UnfocusedDraw)
               ->portraitAnmScript == -1);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Easy) == 1000);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Normal) == 1000);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Hard) == 500);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Lunatic) == 250);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Extra) == 333);
    assert(th07::ApplyPlayerBombScoreDrainDifficultyScale(1000, th07::Th07Difficulty::Phantasm) == 333);
    assert(th07::ComputePlayerBombScoreDrainStep(500000, th07::Th07Difficulty::Normal, 140, 0.2f, 4000) == 710);
    assert(th07::ComputePlayerBombScoreDrainStep(500000, th07::Th07Difficulty::Hard, 140, 0.2f, 4000) == 350);
    assert(th07::ComputePlayerBombScoreDrainStep(500000, th07::Th07Difficulty::Lunatic, 140, 0.2f, 4000) == 170);
    assert(th07::ComputePlayerBombScoreDrainStep(500000, th07::Th07Difficulty::Extra, 140, 0.2f, 4000) == 230);
    assert(th07::ComputePlayerBombScoreDrainStep(1000, th07::Th07Difficulty::Normal, 140, 0.2f, 4000) == 20);
    assert(th07::ComputePlayerBombScoreDrainStep(1000000, th07::Th07Difficulty::Normal, *reimuAFocusedCalc) == 730);
    assert(th07::kPlayerInitFunctionAddress == 0x004423e0);
    assert(th07::kPlayerMovementFunctionAddress == 0x0043ee50);
    assert(th07::kPlayerKillBoxCollisionFunctionAddress == 0x0043e260);
    assert(th07::kPlayerGrazeCollisionFunctionAddress == 0x0043e3b0);
    assert(th07::kPlayerItemBoxCollisionFunctionAddress == 0x0043e4e0);
    assert(th07::kPlayerLaserHitboxFunctionAddress == 0x0043e6b0);
    assert(th07::kPlayerVectorSize == 0x000c);
    assert(th07::kPlayerPositionXOffset == 0x0930);
    assert(th07::kPlayerPositionYOffset == 0x0934);
    assert(th07::kPlayerPositionZOffset == 0x0938);
    assert(th07::kPlayerHitboxMinOffset == 0x0948);
    assert(th07::kPlayerHitboxMaxOffset == 0x0954);
    assert(th07::kPlayerGrazeBoxMinOffset == 0x0960);
    assert(th07::kPlayerGrazeBoxMaxOffset == 0x096c);
    assert(th07::kPlayerGrabItemBoxMinOffset == 0x0978);
    assert(th07::kPlayerGrabItemBoxMaxOffset == 0x0984);
    assert(th07::kPlayerHitboxSizeOffset == 0x0990);
    assert(th07::kPlayerGrazeBoxSizeOffset == 0x099c);
    assert(th07::kPlayerGrabItemSizeOffset == 0x09a8);
    assert(th07::kPlayerLeftOptionPositionOffset == 0x09b4);
    assert(th07::kPlayerRightOptionPositionOffset == 0x09c0);
    assert(th07::kPlayerMovementDeltaXOffset == 0x09cc);
    assert(th07::kPlayerMovementDeltaYOffset == 0x09d0);
    assert(th07::kPlayerInitialZBits == 0x3efae148);
    assert(th07::kPlayerHitboxDepthBits == 0x40a00000);
    assert(th07::kPlayerGrazeBoxDepthBits == 0x40a00000);
    assert(th07::kPlayerGrabItemSizeXBits == 0x41400000);
    assert(th07::kPlayerGrabItemSizeYBits == 0x41400000);
    assert(th07::kPlayerGrabItemSizeZBits == 0x40a00000);
    assert(th07::kTh07EffectTableAddress == 0x0049efc0);
    assert(th07::kTh07EffectTableCount == 34);
    assert(th07::kTh07BombCommonEffectId == 0x19);
    assert(th07::kTh07EffectManagerAddress == th07::kPlayerBombCommonEffectManagerAddress);
    assert(th07::kTh07EffectSpawnFunctionAddress == 0x0041c1c0);
    assert(th07::kTh07EffectSpawnWithAuxVectorFunctionAddress == 0x0041c400);
    assert(th07::kTh07EffectSpawnDedicatedFunctionAddress == th07::kPlayerBombCommonEffectSpawnFunctionAddress);
    assert(th07::kTh07EffectUpdateFunctionAddress == 0x0041c790);
    assert(th07::kTh07EffectDrawMainFunctionAddress == 0x0041ca10);
    assert(th07::kTh07EffectDrawIndexedFunctionAddress == 0x0041cb80);
    assert(th07::kTh07EffectLoadStageAnmFunctionAddress == 0x0041cde0);
    assert(th07::kTh07EffectReleaseAnmFunctionAddress == 0x0041d050);
    assert(th07::kTh07EffectSlotSize == 0x2d8);
    assert(th07::kTh07EffectArrayOffset == 0x1c);
    assert(th07::kTh07EffectPrimarySlotCount == 400);
    assert(th07::kTh07EffectDedicatedSlotBaseIndex == 400);
    assert(th07::kTh07EffectDedicatedSlotCount == 8);
    assert(th07::kTh07EffectUpdateSlotCount == 0x198);
    assert(th07::kTh07EffectFallbackSlotOffset == 0x4885c);
    assert(th07::kTh07EffectActiveCountOffset == 0x8);
    assert(th07::kTh07EffectFrameCounterOffset == 0x496a4);
    assert(th07::kTh07EffectSlotColorOffset == 0x1b8);
    assert(th07::kTh07EffectSlotRenderFlagsOffset == 0x1c0);
    assert(th07::kTh07EffectSlotDrawPositionOffset == 0x1c8);
    assert(th07::kTh07EffectSlotAnmScriptOffset == 0x1d8);
    assert(th07::kTh07EffectSlotAnmInstructionPointerOffset == 0x1e0);
    assert(th07::kTh07EffectSlotPositionOffset == 0x24c);
    assert(th07::kTh07EffectSlotAuxVectorOffset == 0x258);
    assert(th07::kTh07EffectSlotVelocityOffset == 0x264);
    assert(th07::kTh07EffectSlotAccelerationOffset == 0x270);
    assert(th07::kTh07EffectSlotOriginOffset == 0x288);
    assert(th07::kTh07EffectSlotDirectionOffset == 0x294);
    assert(th07::kTh07EffectSlotTimerPreviousFrameOffset == 0x2b8);
    assert(th07::kTh07EffectSlotTimerSubframeOffset == 0x2bc);
    assert(th07::kTh07EffectSlotTimerFrameOffset == 0x2c0);
    assert(th07::kTh07EffectSlotUpdateCallbackOffset == 0x2c8);
    assert(th07::kTh07EffectSlotActiveFlagOffset == 0x2cc);
    assert(th07::kTh07EffectSlotEffectIdOffset == 0x2cd);
    assert(th07::kTh07EffectSlotFadeFlagOffset == 0x2ce);
    assert(th07::kTh07EffectSlotFadeFrameOffset == 0x2cf);
    assert(th07::kTh07EffectSlotRenderModeOffset == 0x2d0);
    assert(th07::kTh07EffectSlotNextDrawOffset == 0x2d4);
    assert(th07::Th07EffectSlotOffset(0) == 0x1c);
    assert(th07::Th07EffectSlotOffset(399) == 0x46ec4);
    assert(th07::Th07EffectSlotOffset(400) == 0x4719c);
    assert(th07::Th07EffectSlotOffset(407) == 0x48584);
    assert(th07::Th07EffectSlotOffset(408) == th07::kTh07EffectFallbackSlotOffset);
    assert(th07::Th07EffectSlotOffset(-1) == th07::kTh07EffectFallbackSlotOffset);
    assert(th07::Th07EffectDedicatedSlotOffset(0) == 0x4719c);
    assert(th07::Th07EffectDedicatedSlotOffset(7) == 0x48584);
    assert(th07::kTh07EffectDrawLists[0].headOffset == th07::kTh07EffectDrawListScrollHeadOffset);
    assert(th07::kTh07EffectDrawLists[0].tailOffset == th07::kTh07EffectDrawListScrollTailOffset);
    assert(th07::kTh07EffectDrawLists[0].sentinelOffset == th07::kTh07EffectDrawListScrollSentinelOffset);
    assert(th07::kTh07EffectDrawLists[1].headOffset == th07::kTh07EffectDrawListIndexedHeadOffset);
    assert(th07::kTh07EffectDrawLists[2].headOffset == th07::kTh07EffectDrawListNoScrollHeadOffset);
    assert(th07::kTh07EffectDrawLists[3].headOffset == th07::kTh07EffectDrawListScrollFlaggedHeadOffset);
    const th07::Th07EffectSpawnContract bombEffectContract =
        th07::BuildTh07DedicatedEffectSpawnContract(th07::kTh07BombCommonEffectId, 0, 0xffffffff);
    assert(bombEffectContract.valid);
    assert(bombEffectContract.effectId == th07::kTh07BombCommonEffectId);
    assert(bombEffectContract.slotIndex == 400);
    assert(bombEffectContract.slotOffset == th07::Th07EffectDedicatedSlotOffset(0));
    assert(bombEffectContract.color == 0xffffffff);
    assert(bombEffectContract.anmScript == 0x02da);
    assert(bombEffectContract.updateCallbackAddress == 0x0041c1b0);
    assert(bombEffectContract.initCallbackAddress == 0);
    assert(bombEffectContract.renderFlagBitsSet == 0x1000);
    assert(bombEffectContract.timerPreviousFrame == 0xfffffc19);
    assert(bombEffectContract.timerSubframe == 0);
    assert(bombEffectContract.timerFrame == 0);
    assert(!th07::BuildTh07DedicatedEffectSpawnContract(-1, 0, 0).valid);
    assert(!th07::BuildTh07DedicatedEffectSpawnContract(0, -1, 0).valid);
    assert(!th07::BuildTh07DedicatedEffectSpawnContract(0, th07::kTh07EffectDedicatedSlotCount, 0).valid);
    assert(th07::FindTh07EffectTableEntry(-1) == nullptr);
    assert(th07::FindTh07EffectTableEntry(th07::kTh07EffectTableCount) == nullptr);
    assert(th07::FindTh07EffectTableEntry(0)->anmScript == 0x02ab);
    assert(th07::FindTh07EffectTableEntry(0)->updateCallbackAddress == 0);
    assert(th07::FindTh07EffectTableEntry(3)->anmScript == 0x02ae);
    assert(th07::FindTh07EffectTableEntry(3)->updateCallbackAddress == 0x0041a4f0);
    assert(th07::FindTh07EffectTableEntry(3)->initCallbackAddress == 0x0041a5a0);
    assert(th07::kTh07EffectInitRandomSplashAddress == 0x0041a370);
    assert(th07::kTh07EffectUpdateLinearAddress == 0x0041a4f0);
    assert(th07::kTh07EffectInitRandomSplashBigAddress == 0x0041a5a0);
    assert(th07::kTh07EffectInitRenderMode2Address == 0x0041a730);
    assert(th07::kTh07EffectUpdateOrbitingRenderMode2Address == 0x0041a750);
    assert(th07::kTh07EffectUpdatePlayerPositionWhileAnmActiveAddress == 0x0041abe0);
    assert(th07::kTh07EffectUpdatePerspectiveMotionAddress == 0x0041aef0);
    assert(th07::kTh07EffectInitPerspective0041b0b0Address == 0x0041b0b0);
    assert(th07::kTh07EffectInitPerspective0041b4a0Address == 0x0041b4a0);
    assert(th07::kTh07EffectInitPerspective0041b770Address == 0x0041b770);
    assert(th07::kTh07EffectInitPerspective0041b9f0Address == 0x0041b9f0);
    assert(th07::kTh07EffectInitPerspective0041bc20Address == 0x0041bc20);
    assert(th07::kTh07EffectInitEasedRadialAddress == 0x0041bec0);
    assert(th07::kTh07EffectUpdateEasedRadialAddress == 0x0041bfd0);
    assert(th07::kTh07EffectUpdateCameraSnapAddress == 0x0041c100);
    assert(th07::kTh07EffectPerspectiveWhiteAnmScript == 0x02d8);
    assert(th07::kTh07SetupDefaultCameraAddress == 0x00408180);
    assert(th07::kTh07SetupStageCameraAddress == 0x004082b0);
    assert(th07::kTh07AnmVmSetScriptFunctionAddress == 0x0044ea20);
    assert(th07::kTh07AnmVmSetActiveSpriteFunctionAddress == 0x0044e8e0);
    assert(th07::kTh07AnmVmResolveFloatOperandFunctionAddress == 0x00450a50);
    assert(th07::kTh07AnmVmResolveIntOperandFunctionAddress == 0x00450b20);
    assert(th07::kTh07AnmVmResolveFloatPointerFunctionAddress == 0x00450c10);
    assert(th07::kTh07AnmVmResolveIntPointerFunctionAddress == 0x00450ca0);
    assert(th07::kTh07AnmVmDrawScreenQuadFunctionAddress == 0x0044f9a0);
    assert(th07::kTh07AnmVmDrawScreenQuadNoRotationFunctionAddress == 0x0044f770);
    assert(th07::kTh07AnmVmSubmitQuadFunctionAddress == 0x0044efb0);
    assert(th07::kTh07RenderProjectedAnmQuadAddress == 0x0044fe00);
    assert(th07::kTh07AnmVmDrawCameraProjectedFunctionAddress == 0x00450130);
    assert(th07::kTh07AnmVmBuildTransformedQuadFunctionAddress == 0x004501a0);
    assert(th07::kTh07AnmVmDrawTransformedCameraProjectedFunctionAddress == 0x004504b0);
    assert(th07::kTh07StageObjectAddress == 0x01347b00);
    assert(th07::kTh07StageObjectClearDwordCount == 0x14ad);
    assert(th07::kTh07StageObjectClearSize == 0x52b4);
    assert(th07::kTh07StageUpdateFunctionAddress == 0x00405690);
    assert(th07::kTh07StageSetAnmVmScriptFunctionAddress == 0x00404f30);
    assert(th07::kTh07StageAnmVmExecuteFunctionAddress == 0x00450d60);
    assert(th07::kTh07StageDrawHighPrioFunctionAddress == 0x004069d0);
    assert(th07::kTh07AnmVmTickTimerFunctionAddress == 0x0043958d);
    assert(th07::kTh07AnmVmRandomU32FunctionAddress == 0x004318d0);
    assert(th07::kTh07AnmVmRandomFloatZeroToOneFunctionAddress == 0x00431900);
    assert(th07::kTh07AnmVmNormalizeAngleFunctionAddress == 0x00431930);
    assert(th07::kTh07AnmVmFloatToIntFunctionAddress == 0x0048b8a0);
    assert(th07::kTh07AnmVmRoundFloatToByteFunctionAddress == 0x0048b8a0);
    assert(th07::kTh07AnmVmMathHelper0047eca0Address == 0x0047eca0);
    assert(th07::kTh07AnmVmMathHelper0048b920Address == 0x0048b920);
    assert(th07::kTh07AnmVmMathHelper0048ba20Address == 0x0048ba20);
    assert(th07::kTh07AnmVmMathHelper0048bb0aAddress == 0x0048bb0a);
    assert(th07::kTh07AnmVmMathHelper0048bb40Address == 0x0048bb40);
    assert(th07::kTh07AnmVmMathHelper0048bbf0Address == 0x0048bbf0);
    assert(th07::kTh07StageFirstAnmVmOffset == 0x0004);
    assert(th07::kTh07StageSecondAnmVmOffset == 0x0250);
    assert(th07::kTh07StageAnmVmSlotSize == 0x024c);
    assert(th07::kTh07AnmVmRotationXOffset == 0x0000);
    assert(th07::kTh07AnmVmRotationYOffset == 0x0004);
    assert(th07::kTh07AnmVmRotationZOffset == 0x0008);
    assert(th07::kTh07AnmVmRotationOffset == 0x0000);
    assert(th07::kTh07AnmVmAngularVelocityXOffset == 0x000c);
    assert(th07::kTh07AnmVmAngularVelocityYOffset == 0x0010);
    assert(th07::kTh07AnmVmAngularVelocityZOffset == 0x0014);
    assert(th07::kTh07AnmVmAngularVelocityOffset == 0x000c);
    assert(th07::kTh07AnmVmScaleXOffset == 0x0018);
    assert(th07::kTh07AnmVmScaleYOffset == 0x001c);
    assert(th07::kTh07AnmVmScaleVelocityXOffset == 0x0020);
    assert(th07::kTh07AnmVmScaleVelocityYOffset == 0x0024);
    assert(th07::kTh07AnmVmAngleField28Offset == 0x0028);
    assert(th07::kTh07AnmVmAngleField2cOffset == 0x002c);
    assert(th07::kTh07AnmVmScriptTimerPreviousOffset == 0x0030);
    assert(th07::kTh07AnmVmScriptTimerSubframeOffset == 0x0034);
    assert(th07::kTh07AnmVmScriptTimerCurrentOffset == 0x0038);
    assert(th07::kTh07AnmVmWaitTimerPreviousOffset == 0x003c);
    assert(th07::kTh07AnmVmWaitTimerSubframeOffset == 0x0040);
    assert(th07::kTh07AnmVmWaitTimerCurrentOffset == 0x0044);
    assert(th07::kTh07AnmVmPositionInterpolationTimerOffset == 0x0048);
    assert(th07::kTh07AnmVmColorInterpolationTimerOffset == 0x0054);
    assert(th07::kTh07AnmVmVisibilityInterpolationTimerOffset == 0x0060);
    assert(th07::kTh07AnmVmRotationInterpolationTimerOffset == 0x006c);
    assert(th07::kTh07AnmVmScaleInterpolationTimerOffset == 0x0078);
    assert(th07::kTh07AnmVmPositionInterpolationDurationOffset == 0x008c);
    assert(th07::kTh07AnmVmColorInterpolationDurationOffset == 0x0098);
    assert(th07::kTh07AnmVmVisibilityInterpolationDurationOffset == 0x00a4);
    assert(th07::kTh07AnmVmRotationInterpolationDurationOffset == 0x00b0);
    assert(th07::kTh07AnmVmScaleInterpolationDurationOffset == 0x00bc);
    assert(th07::kTh07AnmVmPositionInterpolationModeOffset == 0x00c0);
    assert(th07::kTh07AnmVmColorInterpolationModeOffset == 0x00c1);
    assert(th07::kTh07AnmVmVisibilityInterpolationModeOffset == 0x00c2);
    assert(th07::kTh07AnmVmRotationInterpolationModeOffset == 0x00c3);
    assert(th07::kTh07AnmVmScaleInterpolationModeOffset == 0x00c4);
    assert(th07::kTh07AnmVmAngleField28VelocityOffset == 0x00f0);
    assert(th07::kTh07AnmVmAngleField2cVelocityOffset == 0x00f4);
    assert(th07::kTh07AnmVmColorOffset == 0x01b8);
    assert(th07::kTh07AnmVmVisibilityByteOffset == 0x01bb);
    assert(th07::kTh07AnmVmAlternateColorOffset == 0x01bc);
    assert(th07::kTh07AnmVmRenderFlagsOffset == 0x01c0);
    assert(th07::kTh07AnmVmField1c4Offset == 0x01c4);
    assert(th07::kTh07AnmVmPendingInterruptOffset == 0x01c6);
    assert(th07::kTh07StageAnmVmDrawPositionOffset == 0x01c8);
    assert(th07::kTh07StageAnmVmActiveSpriteIndexOffset == 0x01d4);
    assert(th07::kTh07AnmVmBaseSpriteIndexOffset == 0x01d6);
    assert(th07::kTh07StageAnmVmScriptIndexOffset == 0x01d8);
    assert(th07::kTh07AnmVmBeginningOfScriptOffset == 0x01dc);
    assert(th07::kTh07AnmVmCurrentInstructionOffset == 0x01e0);
    assert(th07::kTh07AnmVmLoadedSpriteOffset == 0x01e4);
    assert(th07::kTh07AnmVmPositionInterpolationInitialOffset == 0x01e8);
    assert(th07::kTh07AnmVmPositionInterpolationTargetOffset == 0x01f4);
    assert(th07::kTh07AnmVmRotationInterpolationInitialOffset == 0x0200);
    assert(th07::kTh07AnmVmRotationInterpolationTargetOffset == 0x020c);
    assert(th07::kTh07AnmVmScaleInterpolationInitialXOffset == 0x0218);
    assert(th07::kTh07AnmVmScaleInterpolationInitialYOffset == 0x021c);
    assert(th07::kTh07AnmVmScaleInterpolationFinalXOffset == 0x0220);
    assert(th07::kTh07AnmVmScaleInterpolationFinalYOffset == 0x0224);
    assert(th07::kTh07AnmVmColorInterpolationInitialOffset == 0x0228);
    assert(th07::kTh07AnmVmVisibilityInterpolationInitialOffset == 0x022b);
    assert(th07::kTh07AnmVmColorInterpolationTargetOffset == 0x022c);
    assert(th07::kTh07AnmVmVisibilityInterpolationTargetOffset == 0x022f);
    assert(th07::kTh07StageAnmVmVector230Offset == 0x0230);
    assert(th07::kTh07AnmVmLastSpriteSetFrameOffset == 0x023c);
    assert(th07::kTh07StageAnmVmByte240Offset == 0x0240);
    assert(th07::kTh07StageAnmVmByte241Offset == 0x0241);
    assert(th07::kTh07AnmVmRenderFlagActive == 1);
    assert(th07::kTh07AnmVmRenderFlagDrawable == 2);
    assert(th07::kTh07AnmVmRenderFlagRotationDirty == 0x4);
    assert(th07::kTh07AnmVmRenderFlagScaleDirty == 0x8);
    assert(th07::kTh07AnmVmRenderFlagBit4 == 0x10);
    assert(th07::kTh07AnmVmRenderFlagPositionTargetsAux == 0x80);
    assert(th07::kTh07AnmVmRenderFlagAnchorX == 0x400);
    assert(th07::kTh07AnmVmRenderFlagAnchorY == 0x800);
    assert(th07::kTh07AnmVmRenderFlagAnchorTopLeft == 0xc00);
    assert(th07::kTh07AnmVmRenderFlagBit12 == 0x1000);
    assert(th07::kTh07AnmVmRenderFlagInterruptPending == 0x2000);
    assert(th07::kTh07AnmVmRenderFlagBit14 == 0x4000);
    assert(th07::kTh07AnmVmRenderFlagUseAlternateColor == 0x10000);
    assert(th07::kTh07AnmVmInterruptLabelOpcode == 0x15);
    assert(th07::kTh07AnmVmFallbackInterruptLabel == -1);
    assert(th07::kTh07AnmVmInterpolationModeFromInstruction == -1);
    assert(th07::kTh07AnmVmOperandFlagArg0IsReference == 1);
    assert(th07::kTh07AnmVmOperandFlagArg1IsReference == 2);
    assert(th07::kTh07AnmVmOperandFlagArg2IsReference == 4);
    assert(th07::kTh07AnmVmInstructionOpcodeOffset == 0x00);
    assert(th07::kTh07AnmVmInstructionSizeOffset == 0x02);
    assert(th07::kTh07AnmVmInstructionTimeOffset == 0x04);
    assert(th07::kTh07AnmVmInstructionOperandFlagsOffset == 0x06);
    assert(th07::kTh07AnmVmInstructionOperandsOffset == 0x08);
    assert(th07::kTh07AnmVmInstructionHeaderSize == 0x08);
    assert(th07::kTh07AnmVmVariableIdBase == 10000);
    assert(th07::kTh07AnmVmIntVariable0Id == 10000);
    assert(th07::kTh07AnmVmIntVariable1Id == 0x2711);
    assert(th07::kTh07AnmVmIntVariable2Id == 0x2712);
    assert(th07::kTh07AnmVmIntVariable3Id == 0x2713);
    assert(th07::kTh07AnmVmFloatVariable0Id == 0x2714);
    assert(th07::kTh07AnmVmFloatVariable1Id == 0x2715);
    assert(th07::kTh07AnmVmFloatVariable2Id == 0x2716);
    assert(th07::kTh07AnmVmFloatVariable3Id == 0x2717);
    assert(th07::kTh07AnmVmIntVariable4Id == 0x2718);
    assert(th07::kTh07AnmVmIntVariable5Id == 0x2719);
    assert(th07::kTh07AnmVmIntVariable0Offset == 0x00c8);
    assert(th07::kTh07AnmVmIntVariable1Offset == 0x00cc);
    assert(th07::kTh07AnmVmIntVariable2Offset == 0x00d0);
    assert(th07::kTh07AnmVmIntVariable3Offset == 0x00d4);
    assert(th07::kTh07AnmVmFloatVariable0Offset == 0x00d8);
    assert(th07::kTh07AnmVmFloatVariable1Offset == 0x00dc);
    assert(th07::kTh07AnmVmFloatVariable2Offset == 0x00e0);
    assert(th07::kTh07AnmVmFloatVariable3Offset == 0x00e4);
    assert(th07::kTh07AnmVmIntVariable4Offset == 0x00e8);
    assert(th07::kTh07AnmVmIntVariable5Offset == 0x00ec);
    assert(th07::kTh07AnmVmVariableSlotCount == 10);
    assert(th07::kTh07AnmVmVariableSlots.size() == th07::kTh07AnmVmVariableSlotCount);
    const th07::Th07AnmVmVariableSlot *intVariable0 =
        th07::FindTh07AnmVmVariableSlot(th07::kTh07AnmVmIntVariable0Id);
    assert(intVariable0 != nullptr);
    assert(intVariable0->fieldOffset == th07::kTh07AnmVmIntVariable0Offset);
    assert(intVariable0->valueKind == th07::Th07AnmVmVariableValueKind::Int);
    const th07::Th07AnmVmVariableSlot *floatVariable0 =
        th07::FindTh07AnmVmVariableSlot(th07::kTh07AnmVmFloatVariable0Id);
    assert(floatVariable0 != nullptr);
    assert(floatVariable0->fieldOffset == th07::kTh07AnmVmFloatVariable0Offset);
    assert(floatVariable0->valueKind == th07::Th07AnmVmVariableValueKind::Float);
    const th07::Th07AnmVmVariableSlot *intVariable5 =
        th07::FindTh07AnmVmVariableSlot(th07::kTh07AnmVmIntVariable5Id);
    assert(intVariable5 != nullptr);
    assert(intVariable5->fieldOffset == th07::kTh07AnmVmIntVariable5Offset);
    assert(intVariable5->valueKind == th07::Th07AnmVmVariableValueKind::Int);
    assert(th07::FindTh07AnmVmVariableSlot(0x270f) == nullptr);
    assert(th07::TruncateTh07AnmVmFloatToI32(1.7f) == 1);
    assert(th07::TruncateTh07AnmVmFloatToI32(1.0f) == 1);
    assert(th07::TruncateTh07AnmVmFloatToI32(-1.7f) == -1);
    assert(th07::TruncateTh07AnmVmFloatToI32(-1.0f) == -1);
    const std::array<std::uint8_t, 0x18> anmInstruction = {{
        0x43, 0x00, 0x18, 0x00, 0x0a, 0x00, 0x03, 0x00,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00,
    }};
    th07::Th07AnmVmInstructionHeader anmInstructionHeader = {};
    assert(th07::ReadTh07AnmVmInstructionHeader(anmInstruction, &anmInstructionHeader));
    assert(anmInstructionHeader.opcode == 0x43);
    assert(anmInstructionHeader.size == 0x18);
    assert(anmInstructionHeader.time == 10);
    assert(anmInstructionHeader.operandFlags == 3);
    assert(th07::Th07AnmVmInstructionNextOffset(anmInstructionHeader) == 0x18);
    assert(th07::Th07AnmVmInstructionOperandOffset(0) == 0x08);
    assert(th07::Th07AnmVmInstructionOperandOffset(3) == 0x14);
    std::int32_t anmOperandI32 = 0;
    assert(th07::ReadTh07AnmVmInstructionI32Operand(anmInstruction, 0, &anmOperandI32));
    assert(anmOperandI32 == 0x44332211);
    const std::array<std::uint8_t, 0x0c> floatOperandInstruction = {{
        0x07, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x80, 0x3f,
    }};
    float anmOperandF32 = 0.0f;
    assert(th07::ReadTh07AnmVmInstructionF32Operand(floatOperandInstruction, 0, &anmOperandF32));
    assert(NearlyEqual(anmOperandF32, 1.0f));
    assert(!th07::ReadTh07AnmVmInstructionI32Operand(floatOperandInstruction, 1, &anmOperandI32));
    assert(!th07::ReadTh07AnmVmInstructionF32Operand(floatOperandInstruction, 0, nullptr));
    const std::array<std::uint8_t, th07::kTh07AnmVmInstructionHeaderSize> invalidSizeInstruction = {{
        0x43, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x03, 0x00,
    }};
    assert(!th07::ReadTh07AnmVmInstructionHeader(invalidSizeInstruction, &anmInstructionHeader));
    const std::array<std::uint8_t, th07::kTh07AnmVmInstructionHeaderSize - 1> truncatedInstruction = {};
    assert(!th07::ReadTh07AnmVmInstructionHeader(truncatedInstruction, &anmInstructionHeader));
    assert(!th07::ReadTh07AnmVmInstructionHeader(anmInstruction, nullptr));
    assert(th07::kTh07StageAnmVmScriptBase == 0x0300);
    assert(th07::kTh07CameraPositionGlobalAddress == 0x0134ccf8);
    assert(th07::kTh07CameraOffsetGlobalAddress == 0x0134cd04);
    assert(th07::kTh07CameraForwardGlobalAddress == 0x0134cd1c);
    assert(th07::kTh07CameraProjectionReferencePointGlobalAddress == 0x0134cd28);
    assert(th07::kTh07StageAnmManagerColorResetGuardGlobalAddress == 0x0134cdb0);
    assert(th07::kTh07StageCameraPositionOffset == 0x51f8);
    assert(th07::kTh07StageCameraOffsetOffset == 0x5204);
    assert(th07::kTh07StageCameraUpOffset == 0x5210);
    assert(th07::kTh07StageCameraForwardOffset == 0x521c);
    assert(th07::kTh07StageCameraSideVectorOffset == 0x5228);
    assert(th07::kTh07StageCameraFovOffset == 0x5234);
    assert(th07::kTh07StageCameraInitialPositionZBits == 0x447a0000);
    assert(th07::kTh07StageCameraInitialUpYBits == 0x3f800000);
    assert(th07::kTh07StageCameraInitialFovBits == 0x3f060a92);
    assert(th07::kTh07CameraPositionGlobalAddress - th07::kTh07StageObjectAddress ==
           th07::kTh07StageCameraPositionOffset);
    assert(th07::kTh07CameraOffsetGlobalAddress - th07::kTh07StageObjectAddress ==
           th07::kTh07StageCameraOffsetOffset);
    assert(th07::kTh07CameraForwardGlobalAddress - th07::kTh07StageObjectAddress ==
           th07::kTh07StageCameraForwardOffset);
    assert(th07::kTh07StageAnmManagerColorResetGuardGlobalAddress - th07::kTh07StageObjectAddress ==
           th07::kTh07StageAnmManagerColorResetGuardOffset);
    assert(th07::kTh07CameraProjectionReferencePointGlobalAddress - th07::kTh07StageObjectAddress ==
           th07::kTh07StageCameraSideVectorOffset);
    const th07::Th07StageCameraState initialCamera = th07::BuildTh07InitialStageCameraState();
    assert(NearlyEqual(initialCamera.position.x, 0.0f));
    assert(NearlyEqual(initialCamera.position.y, 0.0f));
    assert(NearlyEqual(initialCamera.position.z, 1000.0f));
    assert(NearlyEqual(initialCamera.offset.x, 0.0f));
    assert(NearlyEqual(initialCamera.up.y, 1.0f));
    assert(NearlyEqual(initialCamera.forward.x, 0.0f));
    assert(NearlyEqual(initialCamera.sideVector.x, 0.0f));
    const th07::Th07StageCameraSetupState stageCamera =
        th07::BuildTh07StageCameraSetupState({10.0f, 20.0f, 30.0f},
                                             {0.0f, 0.0f, -5.0f},
                                             {0.0f, 1.0f, 0.0f},
                                             0.5f);
    assert(NearlyEqual(stageCamera.target.x, 10.0f));
    assert(NearlyEqual(stageCamera.target.y, 20.0f));
    assert(NearlyEqual(stageCamera.target.z, 25.0f));
    assert(NearlyEqual(stageCamera.camera.forward.x, 0.0f));
    assert(NearlyEqual(stageCamera.camera.forward.y, 0.0f));
    assert(NearlyEqual(stageCamera.camera.forward.z, -1.0f));
    assert(NearlyEqual(stageCamera.camera.sideVector.x, 1.0f));
    assert(NearlyEqual(stageCamera.camera.sideVector.y, 0.0f));
    assert(NearlyEqual(stageCamera.camera.sideVector.z, 0.0f));
    assert(NearlyEqual(stageCamera.effectCameraBasis.position.z, 30.0f));
    assert(NearlyEqual(stageCamera.effectCameraBasis.offset.z, -5.0f));
    assert(th07::kTh07StageCameraPositionTargetOffset == 0x50f8);
    assert(th07::kTh07StageCameraOffsetTargetOffset == 0x5104);
    assert(th07::kTh07StageCameraUpTargetOffset == 0x5110);
    assert(th07::kTh07StageCameraFovTargetOffset == 0x5134);
    assert(th07::kTh07StageCameraPositionInterpStartOffset == 0x5138);
    assert(th07::kTh07StageCameraOffsetInterpStartOffset == 0x5144);
    assert(th07::kTh07StageCameraUpInterpStartOffset == 0x5150);
    assert(th07::kTh07StageCameraFovInterpStartOffset == 0x5174);
    assert(th07::kTh07StageCameraPositionCurvePoint3Offset == 0x5178);
    assert(th07::kTh07StageCameraOffsetCurvePoint3Offset == 0x5184);
    assert(th07::kTh07StageCameraUpCurvePoint3Offset == 0x5190);
    assert(th07::kTh07StageCameraPositionCurvePoint2Offset == 0x51b8);
    assert(th07::kTh07StageCameraOffsetCurvePoint2Offset == 0x51c4);
    assert(th07::kTh07StageCameraUpCurvePoint2Offset == 0x51d0);
    assert(th07::kTh07StageCameraPositionInterpDurationOffset == 0x5238);
    assert(th07::kTh07StageCameraOffsetInterpDurationOffset == 0x523c);
    assert(th07::kTh07StageCameraUpInterpDurationOffset == 0x5240);
    assert(th07::kTh07StageCameraFovInterpDurationOffset == 0x5244);
    assert(th07::kTh07StageCameraPositionInterpTimerOffset == 0x5248);
    assert(th07::kTh07StageCameraOffsetInterpTimerOffset == 0x5254);
    assert(th07::kTh07StageCameraUpInterpTimerOffset == 0x5260);
    assert(th07::kTh07StageCameraFovInterpTimerOffset == 0x526c);
    assert(th07::kTh07StageCameraPositionInterpModeOffset == 0x5278);
    assert(th07::kTh07StageCameraOffsetInterpModeOffset == 0x527c);
    assert(th07::kTh07StageCameraUpInterpModeOffset == 0x5280);
    assert(th07::kTh07StageCameraFovInterpModeOffset == 0x5284);
    assert(th07::kTh07StageStdDataOffset == 0x49c);
    assert(th07::kTh07StageScriptOffset == 0x4b0);
    assert(th07::kTh07StageScriptTimerOffset == 0x4b4);
    assert(th07::kTh07StageInstructionIndexOffset == 0x4c0);
    assert(th07::kTh07StageFrameCounterOffset == 0x4c4);
    assert(th07::kTh07StageRouteOffset == 0x4c8);
    assert(th07::kTh07StagePositionOffset == 0x4cc);
    assert(th07::kTh07StageField4d8Offset == 0x4d8);
    assert(th07::kTh07StageFogNearOffset == 0x4dc);
    assert(th07::kTh07StageFogFarOffset == 0x4e0);
    assert(th07::kTh07StageFogColorOffset == 0x4e4);
    assert(th07::kTh07StageFogInterpInitialOffset == 0x4e8);
    assert(th07::kTh07StageFogInterpFinalOffset == 0x4f4);
    assert(th07::kTh07StageFogInterpDurationOffset == 0x500);
    assert(th07::kTh07StageFogInterpTimerOffset == 0x504);
    assert(th07::kTh07StageSpellcardStateOffset == 0x514);
    assert(th07::kTh07StageSpellcardTimerOffset == 0x518);
    assert(th07::kTh07StageSpellcardBackgroundVmCountOffset == 0x520);
    assert(th07::kTh07StageSpellcardBackgroundVmOffset == 0x528);
    assert(th07::kTh07StagePositionInterpFinalOffset == 0x5288);
    assert(th07::kTh07StagePositionInterpEndFrameOffset == 0x5294);
    assert(th07::kTh07StagePositionInterpInitialOffset == 0x5298);
    assert(th07::kTh07StagePositionInterpStartFrameOffset == 0x52a4);
    assert(th07::kTh07StageCameraSetupPendingOffset == 0x52a8);
    assert(th07::kTh07StageCameraCurveInterpolationMode == 7);
    assert(th07::kTh07StageCameraVectorChannels.size() == 3);
    assert(th07::kTh07StageCameraOpcodes.size() == 23);
    const th07::Th07StageCameraVectorChannelLayout *positionChannel =
        th07::FindTh07StageCameraVectorChannel(th07::Th07StageCameraChannel::Position);
    assert(positionChannel != nullptr);
    assert(positionChannel->currentOffset == th07::kTh07StageCameraPositionOffset);
    assert(positionChannel->linearTargetOffset == th07::kTh07StageCameraPositionTargetOffset);
    assert(positionChannel->interpStartOffset == th07::kTh07StageCameraPositionInterpStartOffset);
    assert(positionChannel->curvePoint2Offset == th07::kTh07StageCameraPositionCurvePoint2Offset);
    assert(positionChannel->curvePoint3Offset == th07::kTh07StageCameraPositionCurvePoint3Offset);
    assert(positionChannel->interpDurationOffset == th07::kTh07StageCameraPositionInterpDurationOffset);
    assert(positionChannel->interpTimerOffset == th07::kTh07StageCameraPositionInterpTimerOffset);
    assert(positionChannel->interpModeOffset == th07::kTh07StageCameraPositionInterpModeOffset);
    assert(th07::kTh07StageCameraFovChannel.currentOffset == th07::kTh07StageCameraFovOffset);
    assert(th07::kTh07StageCameraFovChannel.linearTargetOffset == th07::kTh07StageCameraFovTargetOffset);
    assert(th07::kTh07StageCameraFovChannel.interpStartOffset == th07::kTh07StageCameraFovInterpStartOffset);
    assert(th07::kTh07StageCameraFovChannel.interpDurationOffset ==
           th07::kTh07StageCameraFovInterpDurationOffset);
    const th07::Th07StageCameraOpcodeLayout *positionLinearOpcode =
        th07::FindTh07StageCameraOpcode(0x05);
    assert(positionLinearOpcode != nullptr);
    assert(positionLinearOpcode->channel == th07::Th07StageCameraChannel::Position);
    assert(positionLinearOpcode->semantic == th07::Th07StageCameraOpcodeSemantic::SetLinearTarget);
    assert(positionLinearOpcode->primaryFieldOffset == th07::kTh07StageCameraPositionTargetOffset);
    const th07::Th07StageCameraOpcodeLayout *offsetCurveOpcode =
        th07::FindTh07StageCameraOpcode(0x15);
    assert(offsetCurveOpcode != nullptr);
    assert(offsetCurveOpcode->channel == th07::Th07StageCameraChannel::Offset);
    assert(offsetCurveOpcode->semantic == th07::Th07StageCameraOpcodeSemantic::SetCurvePoint2);
    assert(offsetCurveOpcode->primaryFieldOffset == th07::kTh07StageCameraOffsetCurvePoint2Offset);
    const th07::Th07StageCameraOpcodeLayout *upCurveStartOpcode =
        th07::FindTh07StageCameraOpcode(0x1c);
    assert(upCurveStartOpcode != nullptr);
    assert(upCurveStartOpcode->channel == th07::Th07StageCameraChannel::Up);
    assert(upCurveStartOpcode->semantic == th07::Th07StageCameraOpcodeSemantic::StartCurveInterpolation);
    assert(upCurveStartOpcode->primaryFieldOffset == th07::kTh07StageCameraUpInterpDurationOffset);
    assert(th07::FindTh07StageCameraOpcode(0x0d) == nullptr);
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 0), 0.5f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 1), 0.75f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 2), 0.875f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 3), 0.9375f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 4), 0.25f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 5), 0.125f));
    assert(NearlyEqual(th07::ApplyTh07StageCameraInterpolationCurve(0.5f, 6), 0.0625f));
    assert(NearlyEqual(th07::BuildTh07StageCameraCurveComponent(0.0f, 10.0f, 20.0f, 30.0f, 0.0f),
                       0.0f));
    assert(NearlyEqual(th07::BuildTh07StageCameraCurveComponent(0.0f, 10.0f, 20.0f, 30.0f, 1.0f),
                       10.0f));
    assert(NearlyEqual(th07::BuildTh07StageCameraCurveComponent(0.0f, 10.0f, 20.0f, 30.0f, 0.5f),
                       3.75f));
    const th07::Th07EffectVector3 linearCamera =
        th07::BuildTh07StageCameraInterpolatedVector({0.0f, 0.0f, 0.0f},
                                                     {10.0f, 20.0f, 30.0f},
                                                     {},
                                                     {},
                                                     0.5f,
                                                     1);
    assert(NearlyEqual(linearCamera.x, 7.5f));
    assert(NearlyEqual(linearCamera.y, 15.0f));
    assert(NearlyEqual(linearCamera.z, 22.5f));
    const th07::Th07EffectVector3 curveCamera =
        th07::BuildTh07StageCameraInterpolatedVector({0.0f, 0.0f, 0.0f},
                                                     {10.0f, 20.0f, 30.0f},
                                                     {20.0f, 40.0f, 60.0f},
                                                     {30.0f, 60.0f, 90.0f},
                                                     0.5f,
                                                     th07::kTh07StageCameraCurveInterpolationMode);
    assert(NearlyEqual(curveCamera.x, 3.75f));
    assert(NearlyEqual(curveCamera.y, 7.5f));
    assert(NearlyEqual(curveCamera.z, 11.25f));
    assert(NearlyEqual(th07::BuildTh07StageCameraInterpolatedScalar(0.0f, 100.0f, 0.5f, 4),
                       25.0f));
    th07::Th07StageCameraRuntimeState cameraRuntime = th07::BuildTh07InitialStageCameraRuntimeState();
    assert(cameraRuntime.interpolationTimers[0].previousFrame ==
           th07::kTh07StageCameraInitialTimerPreviousFrame);
    assert(NearlyEqual(cameraRuntime.current.position.z, 1000.0f));
    assert(NearlyEqual(cameraRuntime.target.position.z, 1000.0f));
    assert(th07::ApplyTh07StageCameraOpcode(
        &cameraRuntime,
        {0x05, {F32Bits(10.0f), F32Bits(0.0f), F32Bits(1000.0f)}}));
    assert(NearlyEqual(cameraRuntime.interpolationStart.position.x, 0.0f));
    assert(NearlyEqual(cameraRuntime.target.position.x, 10.0f));
    assert(NearlyEqual(cameraRuntime.current.position.x, 10.0f));
    assert(th07::ApplyTh07StageCameraOpcode(&cameraRuntime, {0x06, {10, 0, 0}}));
    th07::AdvanceTh07StageCameraRuntimeState(&cameraRuntime, 1.0f, 1.0f);
    assert(cameraRuntime.interpolationTimers[0].previousFrame == 0);
    assert(cameraRuntime.interpolationTimers[0].currentFrame == 1);
    assert(NearlyEqual(cameraRuntime.current.position.x, 1.0f));
    cameraRuntime = {};
    assert(th07::ApplyTh07StageCameraOpcode(
        &cameraRuntime,
        {0x13, {F32Bits(0.0f), F32Bits(0.0f), F32Bits(0.0f)}}));
    assert(th07::ApplyTh07StageCameraOpcode(
        &cameraRuntime,
        {0x14, {F32Bits(10.0f), F32Bits(0.0f), F32Bits(0.0f)}}));
    assert(th07::ApplyTh07StageCameraOpcode(
        &cameraRuntime,
        {0x15, {F32Bits(20.0f), F32Bits(0.0f), F32Bits(0.0f)}}));
    assert(th07::ApplyTh07StageCameraOpcode(
        &cameraRuntime,
        {0x16, {F32Bits(30.0f), F32Bits(0.0f), F32Bits(0.0f)}}));
    assert(th07::ApplyTh07StageCameraOpcode(&cameraRuntime, {0x17, {10, 0, 0}}));
    for (int idx = 0; idx < 5; idx++) {
        th07::AdvanceTh07StageCameraRuntimeState(&cameraRuntime, 1.0f, 1.0f);
    }
    assert(NearlyEqual(cameraRuntime.current.offset.x, 3.75f));
    assert(NearlyEqual(cameraRuntime.current.forward.x, 1.0f));
    cameraRuntime = {};
    assert(th07::ApplyTh07StageCameraOpcode(&cameraRuntime, {0x0b, {F32Bits(100.0f), 0, 0}}));
    assert(th07::ApplyTh07StageCameraOpcode(&cameraRuntime, {0x0c, {2, 4, 0}}));
    th07::AdvanceTh07StageCameraRuntimeState(&cameraRuntime, 1.0f, 1.0f);
    assert(NearlyEqual(cameraRuntime.current.fov, 25.0f));
    assert(!th07::ApplyTh07StageCameraOpcode(&cameraRuntime, {0x0d, {1, 0, 0}}));
    assert(th07::IsTh07StageNonCameraOpcode(0));
    assert(th07::IsTh07StageNonCameraOpcode(4));
    assert(th07::IsTh07StageNonCameraOpcode(0x0d));
    assert(th07::IsTh07StageNonCameraOpcode(0x1d));
    assert(th07::IsTh07StageNonCameraOpcode(0x1e));
    assert(!th07::IsTh07StageNonCameraOpcode(0x05));
    const th07::Th07StageAnmVmOpcodeEffect primaryStageVm =
        th07::BuildTh07StageAnmVmOpcodeEffect({10, 0x1d, {5, 0, 0}});
    assert(primaryStageVm.handled);
    assert(primaryStageVm.slot == th07::Th07StageAnmVmSlot::Primary);
    assert(primaryStageVm.loadScript);
    assert(!primaryStageVm.clearPrimaryActiveSprite);
    assert(primaryStageVm.scriptIndex == 0x305);
    const th07::Th07StageAnmVmOpcodeEffect secondaryStageVm =
        th07::BuildTh07StageAnmVmOpcodeEffect({10, 0x1e, {9, 0, 0}});
    assert(secondaryStageVm.handled);
    assert(secondaryStageVm.slot == th07::Th07StageAnmVmSlot::Secondary);
    assert(secondaryStageVm.loadScript);
    assert(secondaryStageVm.scriptIndex == 0x309);
    const th07::Th07StageAnmVmOpcodeEffect negativeStageVm =
        th07::BuildTh07StageAnmVmOpcodeEffect({10, 0x1e, {-1, 0, 0}});
    assert(negativeStageVm.handled);
    assert(!negativeStageVm.loadScript);
    assert(negativeStageVm.clearPrimaryActiveSprite);
    assert(!th07::BuildTh07StageAnmVmOpcodeEffect({10, 0x0d, {0, 0, 0}}).handled);
    const th07::Th07AnmVmRenderHelperContract *screenDraw =
        th07::FindTh07AnmVmRenderHelper(th07::kTh07AnmVmDrawScreenQuadFunctionAddress);
    assert(screenDraw != nullptr);
    assert(screenDraw->kind == th07::Th07AnmVmRenderHelperKind::ScreenQuad);
    assert(screenDraw->quadBuilderAddress == th07::kTh07AnmVmDrawScreenQuadNoRotationFunctionAddress);
    assert(screenDraw->checksActiveFlag);
    assert(screenDraw->checksDrawableFlag);
    assert(screenDraw->checksVisibilityByte);
    assert(screenDraw->usesDrawPosition);
    assert(screenDraw->usesLoadedSprite);
    assert(!screenDraw->usesCameraProjection);
    assert(screenDraw->submitsQuad);
    const th07::Th07AnmVmRenderHelperContract *projectedDraw =
        th07::FindTh07AnmVmRenderHelper(th07::kTh07AnmVmDrawCameraProjectedFunctionAddress);
    assert(projectedDraw != nullptr);
    assert(projectedDraw->kind == th07::Th07AnmVmRenderHelperKind::CameraProjected);
    assert(projectedDraw->quadBuilderAddress == th07::kTh07RenderProjectedAnmQuadAddress);
    assert(projectedDraw->usesCameraProjection);
    assert(projectedDraw->submitsQuad);
    const th07::Th07AnmVmRenderHelperContract *transformedDraw =
        th07::FindTh07AnmVmRenderHelper(th07::kTh07AnmVmDrawTransformedCameraProjectedFunctionAddress);
    assert(transformedDraw != nullptr);
    assert(transformedDraw->kind == th07::Th07AnmVmRenderHelperKind::TransformedCameraProjected);
    assert(transformedDraw->quadBuilderAddress == th07::kTh07AnmVmBuildTransformedQuadFunctionAddress);
    assert(transformedDraw->usesCameraProjection);
    assert(th07::FindTh07AnmVmRenderHelper(0x00450d60) == nullptr);
    const th07::Th07AnmVmExecuteOpcodeContract *endOpcode =
        th07::FindTh07AnmVmExecuteOpcode(-1);
    assert(endOpcode != nullptr);
    assert(endOpcode->semantic == th07::Th07AnmVmExecuteOpcodeSemantic::EndScript);
    assert(endOpcode->clearsActiveFlag);
    assert(endOpcode->clearsCurrentInstruction);
    const th07::Th07AnmVmExecuteOpcodeContract *setSpriteOpcode =
        th07::FindTh07AnmVmExecuteOpcode(3);
    assert(setSpriteOpcode != nullptr);
    assert(setSpriteOpcode->semantic == th07::Th07AnmVmExecuteOpcodeSemantic::SetActiveSprite);
    assert(setSpriteOpcode->setsActiveFlag);
    assert(setSpriteOpcode->writesActiveSprite);
    assert(setSpriteOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagActive);
    const th07::Th07AnmVmExecuteOpcodeContract *jumpOpcode =
        th07::FindTh07AnmVmExecuteOpcode(4);
    assert(jumpOpcode != nullptr);
    assert(jumpOpcode->semantic == th07::Th07AnmVmExecuteOpcodeSemantic::Jump);
    assert(jumpOpcode->jumpsToScriptOffset);
    const th07::Th07AnmVmExecuteOpcodeContract *positionOpcode =
        th07::FindTh07AnmVmExecuteOpcode(6);
    assert(positionOpcode != nullptr);
    assert(positionOpcode->semantic == th07::Th07AnmVmExecuteOpcodeSemantic::SetDrawPositionOrAux);
    assert(positionOpcode->writesDrawPosition);
    assert(positionOpcode->writesAuxVector);
    const th07::Th07AnmVmExecuteOpcodeContract *scaleOpcode =
        th07::FindTh07AnmVmExecuteOpcode(7);
    assert(scaleOpcode != nullptr);
    assert(scaleOpcode->writesScale);
    assert(scaleOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagScaleDirty);
    const th07::Th07AnmVmExecuteOpcodeContract *visibilityOpcode =
        th07::FindTh07AnmVmExecuteOpcode(8);
    assert(visibilityOpcode != nullptr);
    assert(visibilityOpcode->writesVisibilityByte);
    const th07::Th07AnmVmExecuteOpcodeContract *rotationOpcode =
        th07::FindTh07AnmVmExecuteOpcode(0x0c);
    assert(rotationOpcode != nullptr);
    assert(rotationOpcode->writesRotation);
    assert(rotationOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagRotationDirty);
    const th07::Th07AnmVmExecuteOpcodeContract *angularVelocityOpcode =
        th07::FindTh07AnmVmExecuteOpcode(0x0d);
    assert(angularVelocityOpcode != nullptr);
    assert(angularVelocityOpcode->writesAngularVelocity);
    assert(th07::FindTh07AnmVmExecuteOpcode(0x52) == nullptr);
    const th07::Th07AnmVmFollowupOpcodeContract *scaleVelocityOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x0e);
    assert(scaleVelocityOpcode != nullptr);
    assert(scaleVelocityOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::SetScaleVelocity);
    assert(scaleVelocityOpcode->primaryFieldOffset == th07::kTh07AnmVmScaleVelocityXOffset);
    assert(scaleVelocityOpcode->secondaryFieldOffset == th07::kTh07AnmVmScaleVelocityYOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *visibilityInterpolationOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x0f);
    assert(visibilityInterpolationOpcode != nullptr);
    assert(visibilityInterpolationOpcode->semantic ==
           th07::Th07AnmVmFollowupOpcodeSemantic::StartVisibilityInterpolation);
    assert(visibilityInterpolationOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Visibility);
    assert(visibilityInterpolationOpcode->modeOffset == th07::kTh07AnmVmVisibilityInterpolationModeOffset);
    assert(visibilityInterpolationOpcode->timerOffset == th07::kTh07AnmVmVisibilityInterpolationTimerOffset);
    assert(visibilityInterpolationOpcode->durationOffset ==
           th07::kTh07AnmVmVisibilityInterpolationDurationOffset);
    assert(visibilityInterpolationOpcode->primaryFieldOffset == th07::kTh07AnmVmVisibilityByteOffset);
    assert(visibilityInterpolationOpcode->secondaryFieldOffset ==
           th07::kTh07AnmVmVisibilityInterpolationInitialOffset);
    assert(visibilityInterpolationOpcode->targetFieldOffset ==
           th07::kTh07AnmVmVisibilityInterpolationTargetOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *renderFlagBit4Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x10);
    assert(renderFlagBit4Opcode != nullptr);
    assert(renderFlagBit4Opcode->controlledRenderFlag == th07::kTh07AnmVmRenderFlagBit4);
    const th07::Th07AnmVmFollowupOpcodeContract *positionInterpolationOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x11);
    assert(positionInterpolationOpcode != nullptr);
    assert(positionInterpolationOpcode->semantic ==
           th07::Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation);
    assert(positionInterpolationOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Position);
    assert(positionInterpolationOpcode->interpolationMode == 0);
    assert(positionInterpolationOpcode->modeOffset == th07::kTh07AnmVmPositionInterpolationModeOffset);
    assert(positionInterpolationOpcode->timerOffset == th07::kTh07AnmVmPositionInterpolationTimerOffset);
    assert(positionInterpolationOpcode->durationOffset == th07::kTh07AnmVmPositionInterpolationDurationOffset);
    assert(positionInterpolationOpcode->primaryFieldOffset == th07::kTh07AnmVmDrawPositionOffset);
    assert(positionInterpolationOpcode->secondaryFieldOffset == th07::kTh07AnmVmAuxVectorOffset);
    assert(positionInterpolationOpcode->targetFieldOffset == th07::kTh07AnmVmPositionInterpolationTargetOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *positionMode4Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x12);
    assert(positionMode4Opcode != nullptr);
    assert(positionMode4Opcode->interpolationMode == 4);
    const th07::Th07AnmVmFollowupOpcodeContract *positionMode6Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x13);
    assert(positionMode6Opcode != nullptr);
    assert(positionMode6Opcode->interpolationMode == 6);
    const th07::Th07AnmVmFollowupOpcodeContract *stopOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x14);
    assert(stopOpcode != nullptr);
    assert(stopOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::StopUntilInterrupt);
    assert(stopOpcode->stopsUntilInterrupt);
    assert(stopOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagInterruptPending);
    assert(th07::FindTh07AnmVmFollowupExecuteOpcode(0x15) == nullptr);
    const th07::Th07AnmVmFollowupOpcodeContract *anchorOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x16);
    assert(anchorOpcode != nullptr);
    assert(anchorOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::SetAnchorTopLeft);
    assert(anchorOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagAnchorTopLeft);
    const th07::Th07AnmVmFollowupOpcodeContract *stopHideOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x17);
    assert(stopHideOpcode != nullptr);
    assert(stopHideOpcode->clearsActiveFlag);
    assert(stopHideOpcode->stopsUntilInterrupt);
    const th07::Th07AnmVmFollowupOpcodeContract *positionTargetSelectionOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x18);
    assert(positionTargetSelectionOpcode != nullptr);
    assert(positionTargetSelectionOpcode->controlledRenderFlag ==
           th07::kTh07AnmVmRenderFlagPositionTargetsAux);
    const th07::Th07AnmVmFollowupOpcodeContract *field1c4Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x19);
    assert(field1c4Opcode != nullptr);
    assert(field1c4Opcode->primaryFieldOffset == th07::kTh07AnmVmField1c4Offset);
    const th07::Th07AnmVmFollowupOpcodeContract *angle28Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1a);
    assert(angle28Opcode != nullptr);
    assert(angle28Opcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::AddAngleField28);
    assert(angle28Opcode->primaryFieldOffset == th07::kTh07AnmVmAngleField28Offset);
    const th07::Th07AnmVmFollowupOpcodeContract *angle2cOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1b);
    assert(angle2cOpcode != nullptr);
    assert(angle2cOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::AddAngleField2c);
    assert(angle2cOpcode->primaryFieldOffset == th07::kTh07AnmVmAngleField2cOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *activeFlagOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1c);
    assert(activeFlagOpcode != nullptr);
    assert(activeFlagOpcode->controlledRenderFlag == th07::kTh07AnmVmRenderFlagActive);
    const th07::Th07AnmVmFollowupOpcodeContract *scaleInterpolationOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1d);
    assert(scaleInterpolationOpcode != nullptr);
    assert(scaleInterpolationOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::StartScaleInterpolation);
    assert(scaleInterpolationOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Scale);
    assert(scaleInterpolationOpcode->modeOffset == th07::kTh07AnmVmScaleInterpolationModeOffset);
    assert(scaleInterpolationOpcode->timerOffset == th07::kTh07AnmVmScaleInterpolationTimerOffset);
    assert(scaleInterpolationOpcode->durationOffset == th07::kTh07AnmVmScaleInterpolationDurationOffset);
    assert(scaleInterpolationOpcode->primaryFieldOffset == th07::kTh07AnmVmScaleXOffset);
    assert(scaleInterpolationOpcode->secondaryFieldOffset == th07::kTh07AnmVmScaleInterpolationInitialXOffset);
    assert(scaleInterpolationOpcode->targetFieldOffset == th07::kTh07AnmVmScaleInterpolationFinalXOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *bit12Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1e);
    assert(bit12Opcode != nullptr);
    assert(bit12Opcode->controlledRenderFlag == th07::kTh07AnmVmRenderFlagBit12);
    const th07::Th07AnmVmFollowupOpcodeContract *bit14Opcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x1f);
    assert(bit14Opcode != nullptr);
    assert(bit14Opcode->controlledRenderFlag == th07::kTh07AnmVmRenderFlagBit14);
    const th07::Th07AnmVmFollowupOpcodeContract *positionDynamicOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x20);
    assert(positionDynamicOpcode != nullptr);
    assert(positionDynamicOpcode->semantic ==
           th07::Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation);
    assert(positionDynamicOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Position);
    assert(positionDynamicOpcode->interpolationMode == th07::kTh07AnmVmInterpolationModeFromInstruction);
    assert(positionDynamicOpcode->modeOffset == th07::kTh07AnmVmPositionInterpolationModeOffset);
    assert(positionDynamicOpcode->timerOffset == th07::kTh07AnmVmPositionInterpolationTimerOffset);
    assert(positionDynamicOpcode->durationOffset == th07::kTh07AnmVmPositionInterpolationDurationOffset);
    assert(positionDynamicOpcode->secondaryFieldOffset == th07::kTh07AnmVmPositionInterpolationInitialOffset);
    assert(positionDynamicOpcode->targetFieldOffset == th07::kTh07AnmVmPositionInterpolationTargetOffset);
    assert(positionDynamicOpcode->controlledRenderFlag == th07::kTh07AnmVmRenderFlagPositionTargetsAux);
    const th07::Th07AnmVmFollowupOpcodeContract *colorDynamicOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x21);
    assert(colorDynamicOpcode != nullptr);
    assert(colorDynamicOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::StartColorInterpolation);
    assert(colorDynamicOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Color);
    assert(colorDynamicOpcode->interpolationMode == th07::kTh07AnmVmInterpolationModeFromInstruction);
    assert(colorDynamicOpcode->modeOffset == th07::kTh07AnmVmColorInterpolationModeOffset);
    assert(colorDynamicOpcode->timerOffset == th07::kTh07AnmVmColorInterpolationTimerOffset);
    assert(colorDynamicOpcode->durationOffset == th07::kTh07AnmVmColorInterpolationDurationOffset);
    assert(colorDynamicOpcode->primaryFieldOffset == th07::kTh07AnmVmColorOffset);
    assert(colorDynamicOpcode->secondaryFieldOffset == th07::kTh07AnmVmColorInterpolationInitialOffset);
    assert(colorDynamicOpcode->targetFieldOffset == th07::kTh07AnmVmColorInterpolationTargetOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *visibilityDynamicOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x22);
    assert(visibilityDynamicOpcode != nullptr);
    assert(visibilityDynamicOpcode->semantic ==
           th07::Th07AnmVmFollowupOpcodeSemantic::StartVisibilityInterpolation);
    assert(visibilityDynamicOpcode->interpolationMode == th07::kTh07AnmVmInterpolationModeFromInstruction);
    assert(visibilityDynamicOpcode->targetFieldOffset == th07::kTh07AnmVmVisibilityInterpolationTargetOffset);
    const th07::Th07AnmVmFollowupOpcodeContract *rotationDynamicOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x23);
    assert(rotationDynamicOpcode != nullptr);
    assert(rotationDynamicOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::StartRotationInterpolation);
    assert(rotationDynamicOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Rotation);
    assert(rotationDynamicOpcode->modeOffset == th07::kTh07AnmVmRotationInterpolationModeOffset);
    assert(rotationDynamicOpcode->timerOffset == th07::kTh07AnmVmRotationInterpolationTimerOffset);
    assert(rotationDynamicOpcode->durationOffset == th07::kTh07AnmVmRotationInterpolationDurationOffset);
    assert(rotationDynamicOpcode->secondaryFieldOffset == th07::kTh07AnmVmRotationInterpolationInitialOffset);
    assert(rotationDynamicOpcode->targetFieldOffset == th07::kTh07AnmVmRotationInterpolationTargetOffset);
    assert(rotationDynamicOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagRotationDirty);
    const th07::Th07AnmVmFollowupOpcodeContract *scaleDynamicOpcode =
        th07::FindTh07AnmVmFollowupExecuteOpcode(0x24);
    assert(scaleDynamicOpcode != nullptr);
    assert(scaleDynamicOpcode->semantic == th07::Th07AnmVmFollowupOpcodeSemantic::StartScaleInterpolation);
    assert(scaleDynamicOpcode->interpolationLane == th07::Th07AnmVmInterpolationLane::Scale);
    assert(scaleDynamicOpcode->interpolationMode == th07::kTh07AnmVmInterpolationModeFromInstruction);
    assert(scaleDynamicOpcode->setRenderFlags == th07::kTh07AnmVmRenderFlagScaleDirty);
    assert(th07::FindTh07AnmVmFollowupExecuteOpcode(0x25) == nullptr);
    assert(th07::FindTh07AnmVmFollowupExecuteOpcode(0x52) == nullptr);
    assert(th07::kTh07AnmVmVariableExecuteOpcodes.size() == 30);
    const th07::Th07AnmVmVariableOpcodeContract *assignIntOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x25);
    assert(assignIntOpcode != nullptr);
    assert(assignIntOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::Assign);
    assert(assignIntOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Int);
    assert(assignIntOpcode->writesViaIntResolver);
    assert(assignIntOpcode->usesIntSourceResolver);
    assert(!assignIntOpcode->readsCurrentDestination);
    const th07::Th07AnmVmVariableOpcodeContract *assignFloatOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x26);
    assert(assignFloatOpcode != nullptr);
    assert(assignFloatOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Float);
    assert(assignFloatOpcode->writesViaFloatResolver);
    assert(assignFloatOpcode->usesFloatSourceResolver);
    const th07::Th07AnmVmVariableOpcodeContract *addAssignIntOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x27);
    assert(addAssignIntOpcode != nullptr);
    assert(addAssignIntOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::AddAssign);
    assert(addAssignIntOpcode->readsCurrentDestination);
    const th07::Th07AnmVmVariableOpcodeContract *divideAssignFloatOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x2e);
    assert(divideAssignFloatOpcode != nullptr);
    assert(divideAssignFloatOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::DivideAssign);
    assert(divideAssignFloatOpcode->writesViaFloatResolver);
    const th07::Th07AnmVmVariableOpcodeContract *helperUnaryOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x30);
    assert(helperUnaryOpcode != nullptr);
    assert(helperUnaryOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::MathHelperAssign);
    assert(helperUnaryOpcode->helperFunctionAddress == th07::kTh07AnmVmMathHelper0048bb0aAddress);
    const th07::Th07AnmVmVariableOpcodeContract *addOperandsIntOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x31);
    assert(addOperandsIntOpcode != nullptr);
    assert(addOperandsIntOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::AddOperands);
    assert(addOperandsIntOpcode->sourceOperandCount == 2);
    assert(!addOperandsIntOpcode->readsCurrentDestination);
    const th07::Th07AnmVmVariableOpcodeContract *divideOperandsFloatOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x38);
    assert(divideOperandsFloatOpcode != nullptr);
    assert(divideOperandsFloatOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::DivideOperands);
    assert(divideOperandsFloatOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Float);
    const th07::Th07AnmVmVariableOpcodeContract *randomIntOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x3b);
    assert(randomIntOpcode != nullptr);
    assert(randomIntOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::RandomIntModulo);
    assert(randomIntOpcode->guardsZeroDivisor);
    assert(randomIntOpcode->helperFunctionAddress == th07::kTh07AnmVmRandomU32FunctionAddress);
    const th07::Th07AnmVmVariableOpcodeContract *randomFloatOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x3c);
    assert(randomFloatOpcode != nullptr);
    assert(randomFloatOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::RandomFloatScaled);
    assert(randomFloatOpcode->helperFunctionAddress == th07::kTh07AnmVmRandomFloatZeroToOneFunctionAddress);
    const th07::Th07AnmVmVariableOpcodeContract *helper0047eca0Opcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x40);
    assert(helper0047eca0Opcode != nullptr);
    assert(helper0047eca0Opcode->helperFunctionAddress == th07::kTh07AnmVmMathHelper0047eca0Address);
    const th07::Th07AnmVmVariableOpcodeContract *normalizeAngleOpcode =
        th07::FindTh07AnmVmVariableExecuteOpcode(0x42);
    assert(normalizeAngleOpcode != nullptr);
    assert(normalizeAngleOpcode->semantic == th07::Th07AnmVmVariableOpcodeSemantic::NormalizeAngle);
    assert(normalizeAngleOpcode->helperFunctionAddress == th07::kTh07AnmVmNormalizeAngleFunctionAddress);
    assert(th07::FindTh07AnmVmVariableExecuteOpcode(0x24) == nullptr);
    assert(th07::FindTh07AnmVmVariableExecuteOpcode(0x43) == nullptr);
    assert(th07::kTh07AnmVmConditionalJumpExecuteOpcodes.size() == 12);
    const th07::Th07AnmVmConditionalJumpOpcodeContract *jumpEqualIntOpcode =
        th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x43);
    assert(jumpEqualIntOpcode != nullptr);
    assert(jumpEqualIntOpcode->comparison == th07::Th07AnmVmConditionalJumpComparison::Equal);
    assert(jumpEqualIntOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Int);
    assert(jumpEqualIntOpcode->usesIntSourceResolver);
    assert(!jumpEqualIntOpcode->usesFloatSourceResolver);
    assert(jumpEqualIntOpcode->jumpOffsetOperandIndex == 2);
    assert(jumpEqualIntOpcode->jumpFrameOperandIndex == 3);
    assert(jumpEqualIntOpcode->jumpsToScriptRelativeOffset);
    assert(jumpEqualIntOpcode->resetsScriptTimer);
    const th07::Th07AnmVmConditionalJumpOpcodeContract *jumpNotEqualFloatOpcode =
        th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x46);
    assert(jumpNotEqualFloatOpcode != nullptr);
    assert(jumpNotEqualFloatOpcode->comparison == th07::Th07AnmVmConditionalJumpComparison::NotEqual);
    assert(jumpNotEqualFloatOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Float);
    assert(jumpNotEqualFloatOpcode->usesFloatSourceResolver);
    const th07::Th07AnmVmConditionalJumpOpcodeContract *jumpLessOrEqualIntOpcode =
        th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x49);
    assert(jumpLessOrEqualIntOpcode != nullptr);
    assert(jumpLessOrEqualIntOpcode->comparison == th07::Th07AnmVmConditionalJumpComparison::LessOrEqual);
    const th07::Th07AnmVmConditionalJumpOpcodeContract *jumpGreaterFloatOpcode =
        th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x4c);
    assert(jumpGreaterFloatOpcode != nullptr);
    assert(jumpGreaterFloatOpcode->comparison == th07::Th07AnmVmConditionalJumpComparison::GreaterThan);
    const th07::Th07AnmVmConditionalJumpOpcodeContract *jumpGreaterOrEqualIntOpcode =
        th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x4d);
    assert(jumpGreaterOrEqualIntOpcode != nullptr);
    assert(jumpGreaterOrEqualIntOpcode->comparison == th07::Th07AnmVmConditionalJumpComparison::GreaterOrEqual);
    assert(th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x42) == nullptr);
    assert(th07::FindTh07AnmVmConditionalJumpExecuteOpcode(0x4f) == nullptr);
    assert(th07::kTh07AnmVmControlExecuteOpcodes.size() == 3);
    const th07::Th07AnmVmControlOpcodeContract *waitOpcode =
        th07::FindTh07AnmVmControlExecuteOpcode(0x4f);
    assert(waitOpcode != nullptr);
    assert(waitOpcode->semantic == th07::Th07AnmVmControlOpcodeSemantic::WaitTimer);
    assert(waitOpcode->valueKind == th07::Th07AnmVmVariableValueKind::Int);
    assert(waitOpcode->writesFieldOffset == th07::kTh07AnmVmWaitTimerCurrentOffset);
    assert(waitOpcode->timerPreviousOffset == th07::kTh07AnmVmWaitTimerPreviousOffset);
    assert(waitOpcode->timerSubframeOffset == th07::kTh07AnmVmWaitTimerSubframeOffset);
    assert(waitOpcode->timerCurrentOffset == th07::kTh07AnmVmWaitTimerCurrentOffset);
    assert(waitOpcode->usesIntSourceResolver);
    assert(waitOpcode->pausesInstructionWhilePositive);
    assert(waitOpcode->clearsTimerWhenNonPositive);
    const th07::Th07AnmVmControlOpcodeContract *angle28VelocityOpcode =
        th07::FindTh07AnmVmControlExecuteOpcode(0x50);
    assert(angle28VelocityOpcode != nullptr);
    assert(angle28VelocityOpcode->semantic == th07::Th07AnmVmControlOpcodeSemantic::SetAngleField28Velocity);
    assert(angle28VelocityOpcode->writesFieldOffset == th07::kTh07AnmVmAngleField28VelocityOffset);
    assert(angle28VelocityOpcode->pairedAngleFieldOffset == th07::kTh07AnmVmAngleField28Offset);
    assert(angle28VelocityOpcode->usesFloatSourceResolver);
    assert(angle28VelocityOpcode->postStepAddsToAngleField);
    const th07::Th07AnmVmControlOpcodeContract *angle2cVelocityOpcode =
        th07::FindTh07AnmVmControlExecuteOpcode(0x51);
    assert(angle2cVelocityOpcode != nullptr);
    assert(angle2cVelocityOpcode->semantic == th07::Th07AnmVmControlOpcodeSemantic::SetAngleField2cVelocity);
    assert(angle2cVelocityOpcode->writesFieldOffset == th07::kTh07AnmVmAngleField2cVelocityOffset);
    assert(angle2cVelocityOpcode->pairedAngleFieldOffset == th07::kTh07AnmVmAngleField2cOffset);
    assert(th07::FindTh07AnmVmControlExecuteOpcode(0x4e) == nullptr);
    assert(th07::FindTh07AnmVmControlExecuteOpcode(0x52) == nullptr);
    assert(th07::kTh07AnmVmPostStepActions.size() == 13);
    const th07::Th07AnmVmPostStepActionContract *postAngularVelocityX =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::ApplyAngularVelocity,
                                          th07::kTh07AnmVmRotationXOffset);
    assert(postAngularVelocityX != nullptr);
    assert(postAngularVelocityX->velocityFieldOffset == th07::kTh07AnmVmAngularVelocityXOffset);
    assert(postAngularVelocityX->valueHelperFunctionAddress == th07::kTh07AnmVmNormalizeAngleFunctionAddress);
    assert(postAngularVelocityX->usesFramerateMultiplier);
    assert(postAngularVelocityX->wrapsAngle);
    assert(postAngularVelocityX->setRenderFlags == th07::kTh07AnmVmRenderFlagRotationDirty);
    const th07::Th07AnmVmPostStepActionContract *postPosition =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
                                          th07::kTh07AnmVmDrawPositionOffset);
    assert(postPosition != nullptr);
    assert(postPosition->interpolationLane == th07::Th07AnmVmInterpolationLane::Position);
    assert(postPosition->timerOffset == th07::kTh07AnmVmPositionInterpolationTimerOffset);
    assert(postPosition->durationOffset == th07::kTh07AnmVmPositionInterpolationDurationOffset);
    assert(postPosition->modeOffset == th07::kTh07AnmVmPositionInterpolationModeOffset);
    assert(postPosition->secondaryFieldOffset == th07::kTh07AnmVmPositionInterpolationInitialOffset);
    assert(postPosition->targetFieldOffset == th07::kTh07AnmVmPositionInterpolationTargetOffset);
    assert(postPosition->selectsAuxPositionByRenderFlag);
    assert(postPosition->clearsDurationWhenComplete);
    const th07::Th07AnmVmPostStepActionContract *postColor =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
                                          th07::kTh07AnmVmColorOffset);
    assert(postColor != nullptr);
    assert(postColor->timerHelperFunctionAddress == th07::kTh07AnmVmTickTimerFunctionAddress);
    assert(postColor->valueHelperFunctionAddress == th07::kTh07AnmVmRoundFloatToByteFunctionAddress);
    const th07::Th07AnmVmPostStepActionContract *postRotationInterpolation =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
                                          th07::kTh07AnmVmRotationOffset);
    assert(postRotationInterpolation != nullptr);
    assert(postRotationInterpolation->timerHelperFunctionAddress == th07::kTh07AnmVmTickTimerFunctionAddress);
    assert(postRotationInterpolation->valueHelperFunctionAddress == th07::kTh07AnmVmNormalizeAngleFunctionAddress);
    assert(postRotationInterpolation->setRenderFlags == th07::kTh07AnmVmRenderFlagRotationDirty);
    assert(postRotationInterpolation->wrapsAngle);
    const th07::Th07AnmVmPostStepActionContract *postScaleVelocityX =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::ApplyScaleVelocity,
                                          th07::kTh07AnmVmScaleXOffset);
    assert(postScaleVelocityX != nullptr);
    assert(postScaleVelocityX->velocityFieldOffset == th07::kTh07AnmVmScaleVelocityXOffset);
    assert(postScaleVelocityX->setRenderFlags ==
           (th07::kTh07AnmVmRenderFlagScaleDirty | th07::kTh07AnmVmRenderFlagRotationDirty));
    const th07::Th07AnmVmPostStepActionContract *postAngle28Velocity =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::ApplyAngleFieldVelocity,
                                          th07::kTh07AnmVmAngleField28Offset);
    assert(postAngle28Velocity != nullptr);
    assert(postAngle28Velocity->velocityFieldOffset == th07::kTh07AnmVmAngleField28VelocityOffset);
    assert(postAngle28Velocity->wrapsAngle);
    const th07::Th07AnmVmPostStepActionContract *postScriptTimer =
        th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::AdvanceScriptTimer,
                                          th07::kTh07AnmVmScriptTimerCurrentOffset);
    assert(postScriptTimer != nullptr);
    assert(postScriptTimer->timerOffset == th07::kTh07AnmVmScriptTimerPreviousOffset);
    assert(postScriptTimer->timerHelperFunctionAddress == th07::kTh07AnmVmTickTimerFunctionAddress);
    assert(postScriptTimer->incrementsManagerStepCounter);
    assert(th07::FindTh07AnmVmPostStepAction(th07::Th07AnmVmPostStepActionSemantic::ApplyAngleFieldVelocity,
                                             th07::kTh07AnmVmScaleXOffset) == nullptr);
    th07::Th07StageScriptRuntimeState stageScript = {};
    stageScript.scriptTimer.currentFrame = 10;
    assert(th07::IsTh07StageScriptInstructionDue(stageScript, {10, 0, {0, 0, 0}}));
    assert(!th07::IsTh07StageScriptInstructionDue(stageScript, {11, 0, {0, 0, 0}}));
    assert(!th07::IsTh07StageScriptInstructionDue(stageScript, {-1, 0, {0, 0, 0}}));
    assert(th07::ApplyTh07StageScriptOpcode(
               &stageScript,
               {10, 0, {F32Bits(10.0f), F32Bits(20.0f), F32Bits(30.0f)}},
               {40, 0, {F32Bits(40.0f), F32Bits(50.0f), F32Bits(60.0f)}}) ==
           th07::Th07StageScriptStepResult::Applied);
    assert(stageScript.instructionIndex == 1);
    assert(NearlyEqual(stageScript.position.x, 10.0f));
    assert(NearlyEqual(stageScript.positionInterpolationInitial.y, 20.0f));
    assert(stageScript.positionInterpolationStartFrame == 10);
    assert(stageScript.positionInterpolationEndFrame == 40);
    assert(NearlyEqual(stageScript.positionInterpolationFinal.z, 60.0f));
    stageScript.fog = {100.0f, 200.0f, 0xff000000};
    assert(th07::ApplyTh07StageScriptOpcode(&stageScript, {10, 2, {4, 0, 0}}, {}) ==
           th07::Th07StageScriptStepResult::Applied);
    assert(stageScript.fogInterpolationDuration == 4);
    assert(stageScript.fogInterpolationTimer.previousFrame ==
           th07::kTh07StageCameraInitialTimerPreviousFrame);
    assert(th07::ApplyTh07StageScriptOpcode(
               &stageScript,
               {10, 1, {-1, F32Bits(200.0f), F32Bits(400.0f)}},
               {}) == th07::Th07StageScriptStepResult::Applied);
    th07::AdvanceTh07StageFogInterpolation(&stageScript, 1.0f, 1.0f);
    assert(stageScript.fogInterpolationTimer.previousFrame == 0);
    assert(stageScript.fogInterpolationTimer.currentFrame == 1);
    assert(NearlyEqual(stageScript.fog.nearPlane, 125.0f));
    assert(NearlyEqual(stageScript.fog.farPlane, 250.0f));
    assert(stageScript.fog.color == 0xff3f3f3f);
    stageScript.cameraResumeInstruction = 0;
    const int pauseIndex = stageScript.instructionIndex;
    assert(th07::ApplyTh07StageScriptOpcode(&stageScript, {10, 3, {0, 0, 0}}, {}) ==
           th07::Th07StageScriptStepResult::Paused);
    assert(stageScript.instructionIndex == pauseIndex);
    stageScript.cameraResumeInstruction = 7;
    assert(th07::ApplyTh07StageScriptOpcode(&stageScript, {10, 3, {0, 0, 0}}, {}) ==
           th07::Th07StageScriptStepResult::Applied);
    assert(stageScript.cameraResumeInstruction == 0);
    assert(th07::ApplyTh07StageScriptOpcode(&stageScript, {10, 0x0d, {123, 0, 0}}, {}) ==
           th07::Th07StageScriptStepResult::Applied);
    assert(stageScript.field4d8 == 123);
    assert(th07::ApplyTh07StageScriptOpcode(&stageScript, {10, 4, {7, 250, 0}}, {}) ==
           th07::Th07StageScriptStepResult::Jumped);
    assert(stageScript.instructionIndex == 7);
    assert(stageScript.scriptTimer.currentFrame == 250);
    assert(stageScript.scriptTimer.previousFrame == th07::kTh07StageCameraInitialTimerPreviousFrame);
    assert(stageScript.cameraSetupPending);
    const th07::Th07EffectLinearMotion splashMotion =
        th07::BuildTh07EffectRandomSplashMotion(1.0f, 0.0f, 1.0f);
    assert(NearlyEqual(splashMotion.velocity.x, 128.0f / 12.0f));
    assert(NearlyEqual(splashMotion.velocity.y, -128.0f / 12.0f));
    assert(NearlyEqual(splashMotion.velocity.z, 0.0f));
    assert(NearlyEqual(splashMotion.acceleration.x, -(128.0f / 12.0f) / 19.0f));
    assert(NearlyEqual(splashMotion.acceleration.y, (128.0f / 12.0f) / 19.0f));
    assert(NearlyEqual(splashMotion.acceleration.z, 0.0f));
    const th07::Th07EffectLinearMotion slowedSplashMotion =
        th07::BuildTh07EffectRandomSplashMotion(1.0f, 0.0f, 0.5f);
    assert(NearlyEqual(slowedSplashMotion.velocity.x, splashMotion.velocity.x * 0.5f));
    assert(NearlyEqual(slowedSplashMotion.acceleration.y, splashMotion.acceleration.y * 0.5f));
    const th07::Th07EffectLinearMotion bigSplashMotion =
        th07::BuildTh07EffectRandomSplashBigMotion(0.75f, 0.25f, 1.0f);
    assert(NearlyEqual(bigSplashMotion.velocity.x, 64.0f * 4.0f / 33.0f));
    assert(NearlyEqual(bigSplashMotion.velocity.y, -64.0f * 4.0f / 33.0f));
    assert(NearlyEqual(bigSplashMotion.acceleration.x, -(64.0f * 4.0f / 33.0f) / 20.0f));
    assert(NearlyEqual(bigSplashMotion.acceleration.y, (64.0f * 4.0f / 33.0f) / 20.0f));
    const th07::Th07EffectRadialState radialState =
        th07::BuildTh07EffectRadialState({10.0f, 20.0f, 30.0f}, 0.5f);
    assert(NearlyEqual(radialState.origin.x, 10.0f));
    assert(NearlyEqual(radialState.origin.y, 20.0f));
    assert(NearlyEqual(radialState.origin.z, 0.0f));
    assert(NearlyEqual(radialState.direction.x, 0.0f));
    assert(NearlyEqual(radialState.direction.y, 1.0f));
    assert(NearlyEqual(radialState.direction.z, 0.0f));
    const th07::Th07EffectRadialState leftRadialState =
        th07::BuildTh07EffectRadialState({10.0f, 20.0f, 30.0f}, 0.25f);
    assert(NearlyEqual(leftRadialState.direction.x, -1.0f));
    assert(NearlyEqual(leftRadialState.direction.y, 0.0f));
    const th07::Th07EffectVector3 inwardStart =
        th07::BuildTh07EffectRadialInwardPosition(radialState.origin, radialState.direction, 0.0f,
                                                  th07::kTh07EffectRadialInwardShortFrames, true);
    assert(NearlyEqual(inwardStart.x, 10.0f));
    assert(NearlyEqual(inwardStart.y, 276.0f));
    assert(NearlyEqual(inwardStart.z, 0.0f));
    const th07::Th07EffectVector3 inwardHalf =
        th07::BuildTh07EffectRadialInwardPosition(radialState.origin, radialState.direction, 30.0f,
                                                  th07::kTh07EffectRadialInwardShortFrames, true);
    assert(NearlyEqual(inwardHalf.y, 148.0f));
    const th07::Th07EffectVector3 slowInwardWithZ =
        th07::BuildTh07EffectRadialInwardPosition({1.0f, 2.0f, 3.0f}, {0.0f, 0.0f, 1.0f}, 120.0f,
                                                  th07::kTh07EffectRadialInwardSlowFrames, false);
    assert(NearlyEqual(slowInwardWithZ.z, 131.0f));
    const th07::Th07EffectVector3 outwardHalf =
        th07::BuildTh07EffectRadialOutwardPosition(radialState.origin, radialState.direction, 15.0f);
    assert(NearlyEqual(outwardHalf.x, 10.0f));
    assert(NearlyEqual(outwardHalf.y, 148.0f));
    assert(NearlyEqual(outwardHalf.z, 0.0f));
    const th07::Th07EffectOrbitState orbitZero =
        th07::BuildTh07EffectOrbitingRenderMode2State({10.0f, 20.0f, 30.0f},
                                                      {0.0f, 1.0f, 0.0f},
                                                      5.0f,
                                                      0.0f);
    assert(NearlyEqual(orbitZero.position.x, 5.0f));
    assert(NearlyEqual(orbitZero.position.y, 20.0f));
    assert(NearlyEqual(orbitZero.position.z, 30.0f));
    assert(NearlyEqual(orbitZero.quaternion.x, 0.0f));
    assert(NearlyEqual(orbitZero.quaternion.y, 1.0f));
    assert(NearlyEqual(orbitZero.quaternion.z, 0.0f));
    assert(NearlyEqual(orbitZero.quaternion.w, 0.0f));
    const th07::Th07EffectOrbitState orbitQuarter =
        th07::BuildTh07EffectOrbitingRenderMode2State({10.0f, 20.0f, 30.0f},
                                                      {0.0f, 1.0f, 0.0f},
                                                      5.0f,
                                                      th07::kTh07EffectRadialRandomAngleBias / 2.0f);
    assert(NearlyEqual(orbitQuarter.position.x, 15.0f));
    assert(NearlyEqual(orbitQuarter.position.y, 20.0f));
    assert(NearlyEqual(orbitQuarter.position.z, 30.0f));
    assert(NearlyEqual(orbitQuarter.quaternion.w, 1.0f));
    const th07::Th07EffectOrbitState orbitVertical =
        th07::BuildTh07EffectOrbitingRenderMode2State({10.0f, 20.0f, 30.0f},
                                                      {0.0f, 0.0f, 1.0f},
                                                      5.0f,
                                                      0.0f);
    assert(NearlyEqual(orbitVertical.position.x, 10.0f));
    assert(NearlyEqual(orbitVertical.position.y, 20.0f));
    assert(NearlyEqual(orbitVertical.position.z, 30.0f));
    const th07::Th07EffectOrbitFadeStep fadeStep =
        th07::BuildTh07EffectOrbitFadeStep(0, 0xaa123456);
    assert(fadeStep.active);
    assert(fadeStep.nextFrame == 1);
    assert(fadeStep.color == 0xef123456);
    assert(NearlyEqual(fadeStep.scale, 1.0625f));
    const th07::Th07EffectOrbitFadeStep fadeStop =
        th07::BuildTh07EffectOrbitFadeStep(15, 0xaa123456);
    assert(!fadeStop.active);
    assert(fadeStop.nextFrame == 16);
    assert(fadeStop.color == 0xaa123456);
    const th07::Th07EffectPerspectiveMotionState perspectiveAlive =
        th07::BuildTh07EffectPerspectiveMotionState({0.0f, 0.0f, -1.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    1.0f,
                                                    4.0f,
                                                    0x10,
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, -1.0f});
    assert(NearlyEqual(perspectiveAlive.velocity.z, -1.0f));
    assert(NearlyEqual(perspectiveAlive.targetPosition.z, -1.0f));
    assert(NearlyEqual(perspectiveAlive.position.z, -1.0f));
    assert(NearlyEqual(perspectiveAlive.rotationZ,
                       5.0f - th07::kTh07EffectRadialRandomAngleScale));
    assert(perspectiveAlive.renderFlags == 0x14);
    assert(perspectiveAlive.inForwardCone);
    assert(perspectiveAlive.active);
    const th07::Th07EffectPerspectiveMotionState perspectiveBehind =
        th07::BuildTh07EffectPerspectiveMotionState({0.0f, 0.0f, -1.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    1.0f,
                                                    4.0f,
                                                    0x10,
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, 1.0f});
    assert(!perspectiveBehind.inForwardCone);
    assert(!perspectiveBehind.active);
    assert(NearlyEqual(perspectiveBehind.rotationZ, 4.0f));
    assert(perspectiveBehind.renderFlags == 0x10);
    const th07::Th07EffectPerspectiveMotionState perspectivePastZ =
        th07::BuildTh07EffectPerspectiveMotionState({0.0f, 0.0f, 1.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, 0.0f},
                                                    0.0f,
                                                    0.0f,
                                                    0,
                                                    {0.0f, 0.0f, 0.0f},
                                                    {0.0f, 0.0f, 1.0f});
    assert(perspectivePastZ.inForwardCone);
    assert(!perspectivePastZ.active);
    assert(perspectivePastZ.renderFlags == th07::kTh07EffectPerspectiveRenderFlagBits);
    assert(th07::BuildTh07EffectPerspectiveRgbWhiteGate(100, 50, 100, 50));
    assert(!th07::BuildTh07EffectPerspectiveRgbWhiteGate(100, 50, 100, 51));
    const th07::Th07EffectPerspectiveCameraBasis perspectiveCamera = {
        {10.0f, 20.0f, 30.0f},
        {2.0f, 4.0f, 6.0f},
    };
    const th07::Th07EffectPerspectiveInitRandoms perspectiveRandoms = {
        1.0f,
        0.0f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        1.0f,
        0.0f,
        0.5f,
        1.0f,
    };
    const th07::Th07EffectPerspectiveInitState perspectiveInitWide =
        th07::BuildTh07EffectPerspectiveInitState(th07::Th07EffectPerspectiveInitKind::Init0041b0b0,
                                                  {2.0f, 4.0f, 6.0f},
                                                  perspectiveCamera,
                                                  perspectiveRandoms,
                                                  0.5f,
                                                  true);
    assert(NearlyEqual(perspectiveInitWide.targetPosition.x, 71.0f));
    assert(NearlyEqual(perspectiveInitWide.targetPosition.y, -78.0f));
    assert(NearlyEqual(perspectiveInitWide.targetPosition.z, -17.0f));
    assert(NearlyEqual(perspectiveInitWide.velocity.x, 1.015f));
    assert(NearlyEqual(perspectiveInitWide.velocity.y, 1.985f));
    assert(NearlyEqual(perspectiveInitWide.velocity.z, 3.04f));
    assert(NearlyEqual(perspectiveInitWide.acceleration.x, 0.00005f));
    assert(NearlyEqual(perspectiveInitWide.acceleration.y, -0.00005f));
    assert(NearlyEqual(perspectiveInitWide.rotationX,
                       th07::kTh07EffectPerspectiveSmallRotationXCenter));
    assert(NearlyEqual(perspectiveInitWide.rotationZ, 0.0f));
    assert(perspectiveInitWide.renderMode == th07::kTh07EffectPerspectiveRenderMode);
    assert(perspectiveInitWide.forceRgbWhite);
    assert(!perspectiveInitWide.doubleAngleVelocityZ);
    const th07::Th07EffectPerspectiveInitState perspectiveInitPositive =
        th07::BuildTh07EffectPerspectiveInitState(th07::Th07EffectPerspectiveInitKind::Init0041b4a0,
                                                  {2.0f, 4.0f, 6.0f},
                                                  perspectiveCamera,
                                                  perspectiveRandoms,
                                                  0.5f,
                                                  false);
    assert(NearlyEqual(perspectiveInitPositive.targetPosition.x, 91.0f));
    assert(NearlyEqual(perspectiveInitPositive.targetPosition.y, -58.0f));
    assert(NearlyEqual(perspectiveInitPositive.targetPosition.z, 33.0f));
    assert(NearlyEqual(perspectiveInitPositive.velocity.x, 20.0f));
    assert(NearlyEqual(perspectiveInitPositive.velocity.y, 20.0f));
    assert(NearlyEqual(perspectiveInitPositive.velocity.z, 0.07f));
    assert(!perspectiveInitPositive.forceRgbWhite);
    const th07::Th07EffectPerspectiveInitState perspectiveInitNegative =
        th07::BuildTh07EffectPerspectiveInitState(th07::Th07EffectPerspectiveInitKind::Init0041b770,
                                                  {2.0f, 4.0f, 6.0f},
                                                  perspectiveCamera,
                                                  perspectiveRandoms,
                                                  0.5f,
                                                  false);
    assert(NearlyEqual(perspectiveInitNegative.velocity.z, -0.08f));
    assert(perspectiveInitNegative.forceRgbWhite);
    const th07::Th07EffectPerspectiveInitState perspectiveInitDrift =
        th07::BuildTh07EffectPerspectiveInitState(th07::Th07EffectPerspectiveInitKind::Init0041b9f0,
                                                  {2.0f, 4.0f, 6.0f},
                                                  perspectiveCamera,
                                                  perspectiveRandoms,
                                                  0.5f,
                                                  false);
    assert(NearlyEqual(perspectiveInitDrift.targetPosition.z, -17.0f));
    assert(NearlyEqual(perspectiveInitDrift.velocity.x, 2.03f));
    assert(NearlyEqual(perspectiveInitDrift.velocity.y, 3.97f));
    assert(NearlyEqual(perspectiveInitDrift.velocity.z, 6.02f));
    assert(NearlyEqual(perspectiveInitDrift.acceleration.z, 0.0f));
    assert(perspectiveInitDrift.forceRgbWhite);
    const th07::Th07EffectPerspectiveInitState perspectiveInitFall =
        th07::BuildTh07EffectPerspectiveInitState(th07::Th07EffectPerspectiveInitKind::Init0041bc20,
                                                  {2.0f, 4.0f, 6.0f},
                                                  perspectiveCamera,
                                                  perspectiveRandoms,
                                                  0.5f,
                                                  false);
    assert(NearlyEqual(perspectiveInitFall.targetPosition.z, 133.0f));
    assert(NearlyEqual(perspectiveInitFall.velocity.z, 2.975f));
    assert(NearlyEqual(perspectiveInitFall.acceleration.z,
                       th07::kTh07EffectPerspectiveFallAccelerationZ));
    assert(perspectiveInitFall.doubleAngleVelocityZ);
    const th07::Th07EffectCameraSnapState cameraSnap =
        th07::BuildTh07EffectCameraSnapState(perspectiveCamera);
    assert(NearlyEqual(cameraSnap.targetPosition.x, 12.0f));
    assert(NearlyEqual(cameraSnap.targetPosition.y, 24.0f));
    assert(NearlyEqual(cameraSnap.targetPosition.z, 36.0f));
    assert(NearlyEqual(cameraSnap.position.x, 12.0f));
    assert(NearlyEqual(cameraSnap.position.y, 24.0f));
    assert(NearlyEqual(cameraSnap.position.z, 0.0f));
    assert(cameraSnap.renderMode == th07::kTh07EffectCameraSnapRenderMode);
    assert(cameraSnap.active);
    const th07::Th07EffectAnchoredPosition inactiveAnchor =
        th07::BuildTh07EffectPlayerAnchoredPosition(false, {1.0f, 2.0f, 3.0f});
    assert(!inactiveAnchor.active);
    const th07::Th07EffectAnchoredPosition activeAnchor =
        th07::BuildTh07EffectPlayerAnchoredPosition(true, {1.0f, 2.0f, 3.0f});
    assert(activeAnchor.active);
    assert(NearlyEqual(activeAnchor.position.x, 1.0f));
    assert(NearlyEqual(activeAnchor.position.y, 2.0f));
    assert(NearlyEqual(activeAnchor.position.z, 3.0f));
    assert(NearlyEqual(th07::NormalizeTh07EffectAngle(4.0f),
                       4.0f - th07::kTh07EffectRadialRandomAngleScale));
    const th07::Th07EffectRadialState easedProvided =
        th07::BuildTh07EffectEasedRadialStateFromAngle({10.0f, 20.0f, 30.0f}, 0.0f, 0.0f);
    assert(NearlyEqual(easedProvided.origin.x, 10.0f));
    assert(NearlyEqual(easedProvided.origin.y, 20.0f));
    assert(NearlyEqual(easedProvided.origin.z, 0.0f));
    assert(NearlyEqual(easedProvided.direction.x, 0.0f));
    assert(NearlyEqual(easedProvided.direction.y, 1.0f));
    assert(NearlyEqual(easedProvided.direction.z, 0.0f));
    const th07::Th07EffectRadialState easedProvidedWide =
        th07::BuildTh07EffectEasedRadialStateFromAngle({10.0f, 20.0f, 30.0f},
                                                       th07::kTh07EffectRadialRandomAngleBias / 2.0f,
                                                       0.5f);
    assert(NearlyEqual(easedProvidedWide.direction.x, 1.75f));
    assert(NearlyEqual(easedProvidedWide.direction.y, 0.0f));
    const th07::Th07EffectRadialState easedRandom =
        th07::BuildTh07EffectEasedRadialStateFromRandomAngle({10.0f, 20.0f, 30.0f}, 0.5f, 1.0f);
    assert(NearlyEqual(easedRandom.direction.x, 0.0f));
    assert(NearlyEqual(easedRandom.direction.y, 2.5f));
    const th07::Th07EffectVector3 easedStart =
        th07::BuildTh07EffectEasedRadialPosition(easedProvided.origin, easedProvided.direction, 0.0f);
    assert(NearlyEqual(easedStart.x, 10.0f));
    assert(NearlyEqual(easedStart.y, 20.0f));
    assert(NearlyEqual(easedStart.z, 0.0f));
    const th07::Th07EffectVector3 easedHalf =
        th07::BuildTh07EffectEasedRadialPosition(easedProvided.origin, easedProvided.direction, 45.0f);
    assert(NearlyEqual(easedHalf.x, 10.0f));
    assert(NearlyEqual(easedHalf.y, 116.0f));
    assert(NearlyEqual(easedHalf.z, 0.0f));
    const th07::Th07EffectVector3 easedFull =
        th07::BuildTh07EffectEasedRadialPosition(easedProvided.origin, easedProvided.direction, 90.0f);
    assert(NearlyEqual(easedFull.x, 10.0f));
    assert(NearlyEqual(easedFull.y, 148.0f));
    assert(NearlyEqual(easedFull.z, 0.0f));
    assert(th07::FindTh07EffectTableEntry(th07::kTh07BombCommonEffectId)->anmScript ==
           th07::kPlayerBombCommonEffectAnmScript);
    assert(th07::FindTh07EffectTableEntry(th07::kTh07BombCommonEffectId)->updateCallbackAddress ==
           th07::kPlayerBombCommonEffectUpdateCallbackAddress);
    assert(th07::FindTh07EffectTableEntry(th07::kTh07BombCommonEffectId)->initCallbackAddress == 0);
    assert(th07::FindTh07EffectTableEntry(33)->anmScript == 0x02b1);
    assert(th07::FindTh07EffectTableEntry(33)->updateCallbackAddress == 0x0041aaf0);
    assert(th07::FindTh07EffectTableEntry(33)->initCallbackAddress == 0x0041aa60);
    assert(th07::FindTh07EffectStageAnmLoad(-1) == nullptr);
    assert(th07::FindTh07EffectStageAnmLoad(9) == nullptr);
    assert(th07::FindTh07EffectStageAnmLoad(0)->loads[0].archivePath == "data/eff01.anm");
    assert(th07::FindTh07EffectStageAnmLoad(0)->anmBankCount == 1);
    assert(th07::FindTh07EffectStageAnmLoad(4)->anmBankCount == 2);
    assert(th07::FindTh07EffectStageAnmLoad(4)->loads[0].archivePath == "data/eff04.anm");
    assert(th07::FindTh07EffectStageAnmLoad(4)->loads[1].archivePath == "data/eff04b.anm");
    assert(th07::FindTh07EffectStageAnmLoad(5)->anmBankCount == 2);
    assert(th07::FindTh07EffectStageAnmLoad(5)->loads[0].archivePath == "data/eff05.anm");
    assert(th07::FindTh07EffectStageAnmLoad(5)->loads[1].archivePath.empty());
    assert(th07::FindTh07EffectStageAnmLoad(6)->loads[1].archivePath == "data/eff06.anm");
    assert(th07::FindTh07EffectStageAnmLoad(7)->anmBankCount == 1);
    assert(th07::FindTh07EffectStageAnmLoad(7)->loads[0].archivePath == "data/eff02.anm");
    assert(th07::FindTh07EffectStageAnmLoad(7)->loads[1].archivePath == "data/eff07.anm");
    assert(th07::FindTh07EffectStageAnmLoad(8)->loads[0].archivePath == "data/eff07.anm");
    assert(th07::FindTh07EffectStageAnmLoad(8)->loads[1].archivePath == "data/eff08.anm");
    const th07::PlayerBombCommonEffectConfig commonEffect =
        th07::BuildPlayerBombCommonEffectConfig(th07::kPlayerBombCommonEffectResetXBits, 0, 360);
    assert(commonEffect.effectId == 0x19);
    assert(commonEffect.spawnSlotArgument == 0);
    assert(commonEffect.spawnFlagArgument == 1);
    assert(commonEffect.initialColor == 0xffffffff);
    assert(commonEffect.anmScript == 0x02da);
    assert(commonEffect.updateCallbackAddress == 0x0041c1b0);
    assert(commonEffect.initCallbackAddress == 0);
    assert(commonEffect.targetXBits == th07::kPlayerBombCommonEffectResetXBits);
    assert(commonEffect.targetYBits == 0);
    assert(commonEffect.durationFrames == 360);
    assert(commonEffect.resetXBits == th07::kPlayerBombCommonEffectResetXBits);
    assert(commonEffect.resetYBits == 0);
    assert(commonEffect.resetZBits == 0);
    assert(commonEffect.interpScaleBits == 0x3d800000);
    assert(commonEffect.postSpawnMultiplierBits == 0xbf800000);
    assert(commonEffect.tintColor == 0xffff4040);
    const th07::PlayerModeTransitionEffectConfig mode4Effect =
        th07::BuildPlayerMode4TransitionEffectConfig();
    assert(mode4Effect.effectId == 0x1c);
    assert(mode4Effect.spawnSlotArgument == 4);
    assert(mode4Effect.spawnFlagArgument == 1);
    assert(mode4Effect.initialColor == 0xffffffff);
    assert(mode4Effect.commonEffectDurationFrames == 0x21c);
    assert(mode4Effect.effectInterpolationDurationFrames == 0x21c);
    assert(mode4Effect.targetXBits == th07::kPlayerBombCommonEffectResetXBits);
    assert(mode4Effect.targetYBits == 0);
    assert(mode4Effect.initialScaleBits == 0x3f800000);
    assert(mode4Effect.finalScaleBits == 0x3e800000);
    assert(mode4Effect.activePointerOffset == 0x0b7e6c);
    const th07::PlayerModeTransitionEffectConfig mode3Effect =
        th07::BuildPlayerMode3TransitionEffectConfig();
    assert(mode3Effect.effectId == 0x1c);
    assert(mode3Effect.spawnSlotArgument == 4);
    assert(mode3Effect.spawnFlagArgument == 1);
    assert(mode3Effect.initialColor == 0xffffffff);
    assert(mode3Effect.commonEffectDurationFrames == 0x28);
    assert(mode3Effect.effectInterpolationDurationFrames == 0x1e);
    assert(mode3Effect.targetXBits == th07::kPlayerBombCommonEffectResetXBits);
    assert(mode3Effect.targetYBits == 0);
    assert(mode3Effect.initialScaleBits == 0x3d800000);
    assert(mode3Effect.finalScaleBits == 0x3fa66666);
    assert(mode3Effect.activePointerOffset == 0x0b7e6c);
    assert(th07::ComputePlayerBombBackdropColor(0, 300) == 0x80808080);
    assert(th07::ComputePlayerBombBackdropColor(30, 300) == 0x80585858);
    assert(th07::ComputePlayerBombBackdropColor(59, 300) == 0x80323232);
    assert(th07::ComputePlayerBombBackdropColor(60, 300) == th07::kPlayerBombBackdropPlateauColor);
    assert(th07::ComputePlayerBombBackdropColor(239, 300) == th07::kPlayerBombBackdropPlateauColor);
    assert(th07::ComputePlayerBombBackdropColor(240, 300) == th07::kPlayerBombBackdropPlateauColor);
    assert(th07::ComputePlayerBombBackdropColor(270, 300) == 0x80585858);
    assert(th07::ComputePlayerBombBackdropColor(299, 300) == 0x807f7f7f);
    assert(th07::ComputePlayerMode4StageColor(0) == 0x80808080);
    assert(th07::ComputePlayerMode4StageColor(15) == 0x80585858);
    assert(th07::ComputePlayerMode4StageColor(29) == 0x80333333);
    assert(th07::ComputePlayerMode4StageColor(30) == th07::kPlayerMode4StageColorPlateauColor);
    assert(th07::ComputePlayerMode4StageColor(509) == th07::kPlayerMode4StageColorPlateauColor);
    assert(th07::ComputePlayerMode4StageColor(510) == th07::kPlayerMode4StageColorPlateauColor);
    assert(th07::ComputePlayerMode4StageColor(525) == 0x80585858);
    assert(th07::ComputePlayerMode4StageColor(539) == 0x807e7e7e);
    assert(th07::FindPlayerStageColorWriteProducer(th07::PlayerStageColorWriteKind::CommonBombBackdrop)
               ->callInstructionAddress == th07::kPlayerStageColorWriteCommonBackdropCallAddress);
    assert(th07::FindPlayerStageColorWriteProducer(th07::PlayerStageColorWriteKind::Mode4Update)
               ->originalFunctionAddress == 0x00441330);
    assert(th07::FindPlayerStageColorWriteProducer(th07::PlayerStageColorWriteKind::Mode4Draw)
               ->originalFunctionAddress == 0x004420b0);
    assert(th07::PlayerBombRoutineSlotName(th07::PlayerBombRoutineSlot::FocusedDraw) == "focused draw");
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::ReimuA, false, false)->originalAddress == 0x00408710);
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::ReimuA, true, false)->originalAddress == 0x004091b0);
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::ReimuA, false, true)->originalAddress == 0x00408e10);
    assert(th07::FindPlayerBombRoutine(th07::Th07ShotType::ReimuA, true, true)->originalAddress == 0x00409990);
    assert(th07::kPlayerObjectAddress == 0x004bdad8);
    assert(th07::kPlayerObjectClearDwordCount == 0x2df9e);
    assert(th07::kPlayerObjectClearSize == 0x0b7e78);
    assert(th07::kPlayerBombActiveOffset == 0x16a20);
    assert(th07::kPlayerBombCapturedFocusOffset == 0x16a24);
    assert(th07::kPlayerBombDurationOffset == 0x16a28);
    assert(th07::kPlayerBombScoreDrainStepOffset == 0x16a2c);
    assert(th07::kPlayerBombLastProcessedFrameOffset == 0x16a30);
    assert(th07::kPlayerBombTimerSubframeOffset == 0x16a34);
    assert(th07::kPlayerBombTimerFrameOffset == 0x16a38);
    assert(th07::kPlayerBombCommonEffectXOffset == 0x16a00);
    assert(th07::kPlayerBombCommonEffectYOffset == 0x16a04);
    assert(th07::kPlayerBombCommonEffectDurationOffset == 0x16a08);
    assert(th07::kPlayerBombCommonEffectXGlobalAddress == 0x004d44d8);
    assert(th07::kPlayerBombCommonEffectYGlobalAddress == 0x004d44dc);
    assert(th07::kPlayerBombCommonEffectDurationGlobalAddress == 0x004d44e0);
    assert(th07::kPlayerBombCommonEffectActivePointerOffset == 0x0b7e68);
    assert(th07::kPlayerPositionOffset == 0x930);
    assert(th07::kPlayerAnmVmSlotSize == 0x024c);
    assert(th07::kPlayerAnmVmSlotSize == th07::kTh07StageAnmVmSlotSize);
    assert(th07::kPlayerPrimaryAnmVmOffset == 0);
    assert(th07::kPlayerLeftOptionAnmVmOffset == 0x024c);
    assert(th07::kPlayerRightOptionAnmVmOffset == 0x0498);
    assert(th07::kPlayerOriginalAnmVmPrefixSize == 0x0930);
    assert(th07::kPlayerOriginalAnmVmPrefixSize == th07::kPlayerPositionOffset);
    assert(th07::kPlayerPrimaryAnmVmScriptIndexOffset == 0x01d8);
    assert(th07::kPlayerLeftOptionAnmVmScriptIndexOffset == 0x0424);
    assert(th07::kPlayerRightOptionAnmVmScriptIndexOffset == 0x0670);
    assert(th07::kPlayerPrimaryAnmVmDrawPositionOffset == 0x01c8);
    assert(th07::kPlayerLeftOptionAnmVmDrawPositionOffset == 0x0414);
    assert(th07::kPlayerRightOptionAnmVmDrawPositionOffset == 0x0660);
    assert(th07::kPlayerLeftOptionPositionOffset == 0x09b4);
    assert(th07::kPlayerRightOptionPositionOffset == 0x09c0);
    assert(th07::kPlayerPrimaryAnmScript == 0x0400);
    assert(th07::kPlayerLeftOptionAnmScript == 0x0480);
    assert(th07::kPlayerRightOptionAnmScript == 0x0481);
    assert(th07::kPlayerBombHorizontalSpeedMultiplierOffset == 0x23f0);
    assert(th07::kPlayerBombVerticalSpeedMultiplierOffset == 0x23f4);
    assert(th07::kPlayerCurrentPowerOffset == 0x23f8);
    assert(th07::kPlayerMode3LockoutTimerOffset == 0x23fc);
    assert(th07::kPlayerModeStateOffset == 0x2408);
    assert(th07::kPlayerModeDrawEnabledOffset == 0x240a);
    assert(th07::kPlayerModeTransitionRequestOffset == 0x240d);
    assert(th07::kPlayerModeTransitionEffectActivePointerOffset == 0x0b7e6c);
    assert(th07::kPlayerBombStartupFallbackDurationFrames == 999);
    assert(th07::kPlayerBombPowerBonusOnStart == 6);
    assert(th07::kPlayerBombDefaultSpeedMultiplierBits == 0x3f800000);
    assert(th07::kPlayerBombBackdropFadeFrames == 60);
    assert(th07::kPlayerBombBackdropFadeDelta == 80);
    assert(th07::kPlayerBombBackdropAlpha == 0x80);
    assert(th07::kPlayerBombBackdropPlateauChannel == 0x30);
    assert(th07::kPlayerBombBackdropBaseColor == 0x80808080);
    assert(th07::kPlayerBombBackdropPlateauColor == 0x80303030);
    assert(th07::kPlayerStageColorWriteCommonBackdropCallAddress == 0x004084d0);
    assert(th07::kPlayerStageColorWriteMode4UpdateCallAddress == 0x0044162f);
    assert(th07::kPlayerStageColorWriteMode4DrawCallAddress == 0x0044233b);
    assert(th07::kPlayerModeCleanupFunctionAddress == 0x00441670);
    assert(th07::kPlayerModeEnterMode4FunctionAddress == 0x00441960);
    assert(th07::kPlayerModeEnterMode3FunctionAddress == 0x00441bd0);
    assert(th07::kPlayerMode4StateOffset == 0x2408);
    assert(th07::kPlayerMode4DrawEnabledOffset == 0x240a);
    assert(th07::kPlayerModeStateIdle == 0);
    assert(th07::kPlayerModeState1 == 1);
    assert(th07::kPlayerModeState2 == 2);
    assert(th07::kPlayerModeState3 == 3);
    assert(th07::kPlayerModeState4 == 4);
    assert(th07::kPlayerModeTransitionRequestIdle == 0);
    assert(th07::kPlayerModeTransitionRequestStartMode4 == 1);
    assert(th07::kPlayerModeTransitionRequestCleanup == 2);
    assert(th07::kPlayerMode4StageColorFrameGlobalAddress == 0x004d44e0);
    assert(th07::kPlayerBombCommonEffectDurationGlobalAddress ==
           th07::kPlayerMode4StageColorFrameGlobalAddress);
    assert(th07::kPlayerMode4CommonEffectDurationFrames == 0x21c);
    assert(th07::kPlayerMode3CommonEffectDurationFrames == 0x28);
    assert(th07::kPlayerMode3EffectInterpolationDurationFrames == 0x1e);
    assert(th07::kPlayerMode1CleanupCommonEffectDurationFrames == 0xf0);
    assert(th07::kPlayerMode4StageColorFadeInFrames == 30);
    assert(th07::kPlayerMode4StageColorFadeOutStartFrame == 510);
    assert(th07::kPlayerMode4StageColorTotalFrames == 540);
    assert(th07::kPlayerMode4StageColorFadeDelta == 80);
    assert(th07::kPlayerMode4StageColorPlateauChannel == 0x30);
    assert(th07::kPlayerMode4StageColorPlateauColor == 0x80303030);
    assert(th07::kTh07GameManagerObjectAddress == 0x00626270);
    assert(th07::kTh07GameManagerFrameCounterOffset == 0x95e8);
    assert(th07::kTh07GameManagerFrameCounterGlobalAddress == 0x0062f858);
    assert(th07::kTh07GameManagerUpdateFunctionAddress == 0x0042d8d5);
    assert(th07::kTh07GameManagerRegisterFunctionAddress == 0x0042f3c5);
    assert(th07::kTh07GameManagerCleanupFunctionAddress == 0x0042f2e4);
    assert(th07::kTh07ScreenEffectShakeFunctionAddress == 0x0044b0e0);
    assert(th07::kTh07ScreenShakeRequiredGameFrameCount == 2);
    assert(th07::kPlayerBombCommonEffectSpawnFunctionAddress == 0x0041c610);
    assert(th07::kPlayerBombCommonEffectManagerAddress == 0x012fe250);
    assert(th07::kPlayerBombCommonEffectTableAddress == 0x0049efc0);
    assert(th07::kPlayerBombCommonEffectId == 0x19);
    assert(th07::kPlayerBombCommonEffectAnmScript == 0x02da);
    assert(th07::kPlayerBombCommonEffectUpdateCallbackAddress == 0x0041c1b0);
    assert(th07::kPlayerBombCommonEffectInitCallbackAddress == 0);
    assert(th07::kPlayerBombCommonEffectSpawnSlotArgument == 0);
    assert(th07::kPlayerBombCommonEffectSpawnFlagArgument == 1);
    assert(th07::kPlayerBombCommonEffectInitialColor == 0xffffffff);
    assert(th07::kPlayerBombCommonEffectTintColor == 0xffff4040);
    assert(th07::kPlayerBombCommonEffectResetXBits == 0xfffffc19);
    assert(th07::kPlayerBombCommonEffectResetYBits == 0);
    assert(th07::kPlayerBombCommonEffectResetZBits == 0);
    assert(th07::kPlayerBombCommonEffectInterpScaleBits == 0x3d800000);
    assert(th07::kPlayerBombCommonEffectPostSpawnMultiplierBits == 0xbf800000);
    assert(th07::kPlayerModeTransitionEffectId == 0x1c);
    assert(th07::kPlayerModeTransitionEffectSpawnSlotArgument == 4);
    assert(th07::kPlayerModeTransitionEffectSpawnFlagArgument == 1);
    assert(th07::kPlayerModeTransitionEffectInitialColor == 0xffffffff);
    assert(th07::kPlayerMode4EffectInitialScaleBits == 0x3f800000);
    assert(th07::kPlayerMode4EffectFinalScaleBits == 0x3e800000);
    assert(th07::kPlayerMode3EffectInitialScaleBits == 0x3d800000);
    assert(th07::kPlayerMode3EffectFinalScaleBits == 0x3fa66666);
}

struct OriginalShtExpectation {
    const char *filename;
    th07::Th07ShotType shotType;
    bool focused;
    std::uintmax_t fileSize;
    std::size_t shotRecords;
    std::int32_t deathbombWindowFrames;
    std::array<float, 10> tuningFloats;
};

void CheckOriginalFilesIfAvailable()
{
    const std::filesystem::path extracted = th07::ExtractedReferenceRoot();
    if (!std::filesystem::exists(extracted / "ply00a.sht")) {
        std::cout << "TH07 extracted SHT files not present; original-file checks skipped\n";
        return;
    }

    constexpr std::array<int, 9> kThresholds = {8, 16, 32, 48, 64, 80, 96, 128, 999};
    std::array<int, 6> spawnCallbackUsage = {};
    std::array<int, 6> updateCallbackUsage = {};
    std::array<int, 2> drawCallbackUsage = {};
    std::array<int, 3> collisionCallbackUsage = {};

    constexpr std::array<OriginalShtExpectation, 12> kExpected = {{
        {"ply00a.sht", th07::Th07ShotType::ReimuA, false, 3280, 60, 15, {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
        {"ply00b.sht", th07::Th07ShotType::ReimuB, false, 3436, 63, 15, {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
        {"ply01a.sht", th07::Th07ShotType::MarisaA, false, 2604, 47, 8, {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
        {"ply01b.sht", th07::Th07ShotType::MarisaB, false, 2136, 38, 8, {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
        {"ply02a.sht", th07::Th07ShotType::SakuyaA, false, 4268, 79, 6, {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
        {"ply02b.sht", th07::Th07ShotType::SakuyaB, false, 3748, 69, 6, {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
        {"ply00as.sht", th07::Th07ShotType::ReimuA, true, 1928, 34, 15, {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
        {"ply00bs.sht", th07::Th07ShotType::ReimuB, true, 1564, 27, 15, {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
        {"ply01as.sht", th07::Th07ShotType::MarisaA, true, 1408, 24, 8, {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
        {"ply01bs.sht", th07::Th07ShotType::MarisaB, true, 1564, 27, 8, {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
        {"ply02as.sht", th07::Th07ShotType::SakuyaA, true, 2812, 51, 6, {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
        {"ply02bs.sht", th07::Th07ShotType::SakuyaB, true, 3436, 63, 6, {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
    }};

    for (const OriginalShtExpectation &expected : kExpected) {
        const th07::ShtFile file = th07::LoadShtFile(extracted / expected.filename);
        const th07::ShtFileBinding *binding = th07::FindShtFile(expected.shotType, expected.focused);

        assert(binding != nullptr);
        assert(std::filesystem::path(std::string(binding->file.archivePath)).filename() == expected.filename);
        assert(std::filesystem::file_size(extracted / expected.filename) == expected.fileSize);
        assert(file.header.formatId == 0);
        assert(file.header.powerLevelCount == 9);
        assert(file.header.deathbombWindowFrames == expected.deathbombWindowFrames);
        for (std::size_t i = 0; i < expected.tuningFloats.size(); ++i) {
            assert(NearlyEqual(file.header.tuningFloats[i], expected.tuningFloats[i]));
        }
        assert(NearlyEqual(file.header.hitboxDiagonalExtent(), expected.tuningFloats[0]));
        assert(NearlyEqual(file.header.grazeDiagonalExtent(), expected.tuningFloats[1]));
        assert(NearlyEqual(file.header.orthogonalMovementSpeed(), expected.tuningFloats[6]));
        assert(NearlyEqual(file.header.focusedOrthogonalMovementSpeed(), expected.tuningFloats[7]));
        assert(file.powerLevels.size() == kThresholds.size());
        assert(th07::CountShtShotRecords(file) == expected.shotRecords);
        assert(th07::FindShtPowerLevelForPower(file, 0)->powerThreshold == 8);
        assert(th07::FindShtPowerLevelForPower(file, 128)->powerThreshold == 999);

        for (std::size_t i = 0; i < kThresholds.size(); ++i) {
            assert(file.powerLevels[i].powerThreshold == kThresholds[i]);
            assert(file.powerLevels[i].recordOffset >= th07::kShtHeaderSize + kThresholds.size() * th07::kShtPowerLevelEntrySize);
            assert(file.powerLevels[i].terminatorOffset < expected.fileSize);
            assert(!file.powerLevels[i].shots.empty());
            for (const th07::ShtShotRecord &shot : file.powerLevels[i].shots) {
                const th07::ResolvedShtCallbacks callbacks = th07::ResolveShtCallbacks(shot);
                assert(callbacks.spawn != nullptr);
                assert(callbacks.update != nullptr);
                assert(callbacks.draw != nullptr);
                assert(callbacks.collision != nullptr);
                assert(callbacks.spawn->semantic != th07::ShtCallbackSemantic::None || shot.callbacks.onSpawn == 0);
                assert(callbacks.update->semantic != th07::ShtCallbackSemantic::None || shot.callbacks.onUpdate == 0);
                assert(callbacks.draw->semantic != th07::ShtCallbackSemantic::None || shot.callbacks.onDraw == 0);
                assert(callbacks.collision->semantic != th07::ShtCallbackSemantic::None || shot.callbacks.onCollision == 0);

                assert(shot.callbacks.onSpawn >= 0);
                assert(shot.callbacks.onUpdate >= 0);
                assert(shot.callbacks.onDraw >= 0);
                assert(shot.callbacks.onCollision >= 0);
                assert(static_cast<std::size_t>(shot.callbacks.onSpawn) < spawnCallbackUsage.size());
                assert(static_cast<std::size_t>(shot.callbacks.onUpdate) < updateCallbackUsage.size());
                assert(static_cast<std::size_t>(shot.callbacks.onDraw) < drawCallbackUsage.size());
                assert(static_cast<std::size_t>(shot.callbacks.onCollision) < collisionCallbackUsage.size());
                ++spawnCallbackUsage[static_cast<std::size_t>(shot.callbacks.onSpawn)];
                ++updateCallbackUsage[static_cast<std::size_t>(shot.callbacks.onUpdate)];
                ++drawCallbackUsage[static_cast<std::size_t>(shot.callbacks.onDraw)];
                ++collisionCallbackUsage[static_cast<std::size_t>(shot.callbacks.onCollision)];
            }
        }
    }

    assert((spawnCallbackUsage == std::array<int, 6>{386, 9, 18, 9, 51, 109}));
    assert((updateCallbackUsage == std::array<int, 6>{461, 36, 17, 41, 18, 9}));
    assert((drawCallbackUsage == std::array<int, 2>{573, 9}));
    assert((collisionCallbackUsage == std::array<int, 3>{514, 41, 27}));

    const th07::ShtFile reimuA = th07::LoadShtFile(extracted / "ply00a.sht");
    const th07::ShtShotRecord &firstShot = reimuA.powerLevels[0].shots[0];
    assert(firstShot.waitBetweenBullets == 5);
    assert(firstShot.bulletFrame == 0);
    assert(NearlyEqual(firstShot.sizeX, 12.0f));
    assert(NearlyEqual(firstShot.sizeY, 12.0f));
    assert(firstShot.damage == 48);
    assert(firstShot.spawnPositionIndex == 0);
    assert(firstShot.bulletType == 0);
    assert(firstShot.anmScriptIndex == 0x440);
    assert(firstShot.soundIndex == 0);
    assert(firstShot.callbacks.onSpawn == 0);
    assert(firstShot.callbacks.onUpdate == 0);
    assert(firstShot.callbacks.onDraw == 0);
    assert(firstShot.callbacks.onCollision == 0);
}

} // namespace

int main()
{
    CheckSyntheticFixture();
    CheckCallbackTablesAgainstOriginalExeIfAvailable();
    CheckOriginalFilesIfAvailable();
    return 0;
}
