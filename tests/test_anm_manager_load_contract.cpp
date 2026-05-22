#include "ReferencePaths.hpp"
#include "Th07AnmFile.hpp"
#include "Th07AnmManagerLoadTables.hpp"
#include "Th07ResourceManifest.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace {

std::uint16_t ReadU16(const std::vector<u8> &bytes, std::size_t offset)
{
    assert(offset + 2 <= bytes.size());
    return static_cast<std::uint16_t>(bytes[offset]) |
           (static_cast<std::uint16_t>(bytes[offset + 1]) << 8);
}

std::uint32_t ReadU32(const std::vector<u8> &bytes, std::size_t offset)
{
    assert(offset + 4 <= bytes.size());
    return static_cast<std::uint32_t>(bytes[offset]) |
           (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

std::vector<u8> ReadWholeFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("failed to open " + path.string());
    }
    return std::vector<u8>(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
}

std::filesystem::path ExtractedPathForArchivePath(std::string_view archivePath)
{
    const std::size_t slash = archivePath.find_last_of('/');
    if (slash == std::string_view::npos) {
        return th07::ExtractedReferenceRoot() / std::string(archivePath);
    }
    return th07::ExtractedReferenceRoot() /
           std::string(archivePath.substr(slash + 1));
}

std::size_t PeVirtualAddressToFileOffset(const std::vector<u8> &bytes, std::uint32_t address)
{
    const std::uint32_t peHeader = ReadU32(bytes, 0x3c);
    assert(std::equal(bytes.begin() + static_cast<std::ptrdiff_t>(peHeader),
                      bytes.begin() + static_cast<std::ptrdiff_t>(peHeader + 4),
                      "PE\0\0"));

    const std::uint16_t sectionCount = ReadU16(bytes, peHeader + 0x06);
    const std::uint16_t optionalHeaderSize = ReadU16(bytes, peHeader + 0x14);
    const std::size_t optionalHeader = peHeader + 0x18;
    const std::uint32_t imageBase = ReadU32(bytes, optionalHeader + 0x1c);
    const std::uint32_t rva = address - imageBase;
    const std::size_t sectionTable = optionalHeader + optionalHeaderSize;

    for (std::uint16_t i = 0; i < sectionCount; i++) {
        const std::size_t section = sectionTable + static_cast<std::size_t>(i) * 0x28;
        const std::uint32_t virtualSize = ReadU32(bytes, section + 0x08);
        const std::uint32_t virtualAddress = ReadU32(bytes, section + 0x0c);
        const std::uint32_t rawSize = ReadU32(bytes, section + 0x10);
        const std::uint32_t rawOffset = ReadU32(bytes, section + 0x14);
        const std::uint32_t mappedSize = std::max(virtualSize, rawSize);
        if (virtualAddress <= rva && rva < virtualAddress + mappedSize) {
            return rawOffset + (rva - virtualAddress);
        }
    }

    throw std::runtime_error("unable to map TH07 PE virtual address");
}

void CheckPeBytesAtVa(const std::vector<u8> &bytes, std::uint32_t address,
                      std::initializer_list<u8> expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(offset + expected.size() <= bytes.size());
    if (!std::equal(expected.begin(), expected.end(),
                    bytes.begin() + static_cast<std::ptrdiff_t>(offset))) {
        std::ostringstream message;
        message << "PE byte check failed at VA 0x" << std::hex << address;
        throw std::runtime_error(message.str());
    }
}

void CheckPePushImmediateAtVa(const std::vector<u8> &bytes, std::uint32_t address,
                              std::uint32_t expected)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(bytes[offset] == 0x68);
    assert(ReadU32(bytes, offset + 1) == expected);
}

void CheckPeNearCallTargetAtVa(const std::vector<u8> &bytes, std::uint32_t address,
                               std::uint32_t expectedTarget)
{
    const std::size_t offset = PeVirtualAddressToFileOffset(bytes, address);
    assert(bytes[offset] == 0xe8);
    const auto relative = static_cast<std::int32_t>(ReadU32(bytes, offset + 1));
    const auto target = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(address) + 5 + static_cast<std::int64_t>(relative));
    assert(target == expectedTarget);
}

std::uint32_t MaxLocalSpriteScriptId(const th07::Th07AnmFile &file)
{
    std::uint32_t maxLocalId = 0;
    for (const th07::Th07AnmSprite &sprite : file.sprites) {
        if (sprite.localId > maxLocalId) {
            maxLocalId = sprite.localId;
        }
    }
    for (const th07::Th07AnmScriptEntry &script : file.scriptEntries) {
        if (script.localId > maxLocalId) {
            maxLocalId = script.localId;
        }
    }
    return maxLocalId;
}

std::uint32_t CheckExtractedAnmLoadContract(const th07::RuntimeAnmLoadContract &contract)
{
    const std::vector<u8> bytes = ReadWholeFile(ExtractedPathForArchivePath(contract.file.archivePath));
    std::size_t cursor = 0;
    std::uint32_t slot = contract.anmFileSlot;
    std::uint32_t spriteScriptOffset = contract.spriteScriptOffset;
    const std::uint32_t initialSlot = slot;

    for (;;) {
        assert(th07::IsTh07AnmManagerSlotInRange(slot));
        assert(cursor + th07::kTh07AnmHeaderMinimumSize <= bytes.size());
        assert(ReadU32(bytes, cursor + th07::kTh07AnmHeaderVersionOffset) ==
               th07::kTh07AnmHeaderExpectedVersion);

        const std::uint32_t nextEntryOffset =
            ReadU32(bytes, cursor + th07::kTh07AnmHeaderNextEntryOffsetOffset);
        const std::size_t segmentSize =
            nextEntryOffset == 0 ? bytes.size() - cursor : nextEntryOffset;
        assert(segmentSize >= th07::kTh07AnmHeaderMinimumSize);
        assert(cursor + segmentSize <= bytes.size());

        th07::Th07AnmFile segment;
        std::string error;
        assert(th07::ParseTh07AnmFile(
            std::span<const u8>(bytes.data() + static_cast<std::ptrdiff_t>(cursor), segmentSize),
            &segment, &error));
        assert(error.empty());
        assert(segment.header.version == th07::kTh07AnmHeaderExpectedVersion);

        const std::uint32_t maxLocalId = MaxLocalSpriteScriptId(segment);
        assert(th07::IsTh07AnmManagerGlobalSpriteScriptIdInRange(spriteScriptOffset + maxLocalId));
        assert(th07::Th07AnmManagerScriptPointerOffset(spriteScriptOffset + maxLocalId) <
               th07::kTh07AnmManagerSpriteIndicesOffset);
        assert(th07::Th07AnmManagerSpriteIndexOffset(spriteScriptOffset + maxLocalId) <
               th07::kTh07AnmManagerSlotInfoOffset);
        assert(th07::Th07AnmManagerTextureNamePointerOffset(slot) <
               th07::kTh07AnmManagerScriptsOffset);
        assert(th07::Th07AnmManagerSlotFilePointerOffset(slot) ==
               th07::Th07AnmManagerSlotInfoFieldOffset(
                   slot, th07::kTh07AnmManagerSlotFilePointerFieldOffset));

        if (nextEntryOffset == 0) {
            return slot - initialSlot + 1;
        }

        cursor += nextEntryOffset;
        spriteScriptOffset = th07::Th07AnmManagerAdvanceSpriteScriptOffset(spriteScriptOffset,
                                                                           maxLocalId + 1);
        slot++;
    }
}

} // namespace

int main()
{
    static_assert(th07::kTh07AnmManagerLoadAnmFunctionAddress == 0x0044df90);
    static_assert(th07::kTh07AnmManagerLoadAnmSegmentFunctionAddress == 0x0044e070);
    static_assert(th07::kTh07AnmManagerReleaseAnmFunctionAddress == 0x0044e4e0);
    static_assert(th07::kTh07AnmManagerLoadAnmOpenPathCallAddress == 0x0044df9e);
    static_assert(th07::kTh07AnmManagerLoadAnmFailureLogCallAddress == 0x0044dfc3);
    static_assert(th07::kTh07AnmManagerLoadAnmSegmentCallAddress == 0x0044dff3);
    static_assert(th07::kTh07AnmManagerOpenPathFunctionAddress == 0x00431330);
    static_assert(th07::kTh07AnmManagerLogFunctionAddress == 0x00431730);
    static_assert(th07::kTh07AnmManagerLogBufferAddress == 0x00624210);
    static_assert(th07::kTh07AnmManagerLoadFailureStringAddress == 0x00495c7c);
    static_assert(th07::kTh07AnmManagerInvalidSlotStringAddress == 0x00495c5c);
    static_assert(th07::kTh07AnmManagerInvalidVersionStringAddress == 0x00495c3c);
    static_assert(th07::kTh07AnmManagerTextureFailureStringAddress == 0x00495bf8);
    static_assert(th07::kTh07AnmManagerEmbeddedTextureFailureStringAddress == 0x00495bb8);
    static_assert(th07::kTh07AnmManagerSpriteRangeFailureStringAddress == 0x00495b80);
    static_assert(th07::kTh07AnmManagerScriptRangeFailureStringAddress == 0x00495b4c);
    static_assert(th07::kTh07AnmManagerLoadAnmInitialOwnedFlag == 1);
    static_assert(th07::kTh07AnmManagerLoadAnmChainedOwnedFlag == 0);
    static_assert(th07::kTh07AnmManagerMaxLoadedSlots == 0x32);
    static_assert(th07::kTh07AnmManagerMaxIndexedSpriteOrScript == 0x09ff);
    static_assert(th07::kTh07AnmManagerIndexedSpriteOrScriptLimit == 0x0a00);
    static_assert(th07::kTh07AnmManagerSpriteStride == 0x40);
    static_assert(th07::kTh07AnmManagerLoadedSpritesOffset == 0x000060);
    static_assert(th07::kTh07AnmManagerTexturesOffset == 0x282ac);
    static_assert(th07::kTh07AnmManagerTextureNamePointersOffset == 0x28acc);
    static_assert(th07::kTh07AnmManagerScriptsOffset == 0x28ef0);
    static_assert(th07::kTh07AnmManagerSpriteIndicesOffset == 0x2b6f0);
    static_assert(th07::kTh07AnmManagerSlotInfoOffset == 0x2def0);
    static_assert(th07::kTh07AnmManagerSlotInfoStride == 0x0c);
    static_assert(th07::kTh07AnmManagerCurrentTexturePointerOffset == 0x2e4cc);
    static_assert(th07::kTh07AnmManagerCurrentBlendModeOffset == 0x2e4d0);
    static_assert(th07::kTh07AnmManagerCurrentColorOpOffset == 0x2e4d1);
    static_assert(th07::kTh07AnmManagerCurrentVertexShaderOffset == 0x2e4d2);
    static_assert(th07::kTh07AnmHeaderTextureSlotOffset == 0x08);
    static_assert(th07::kTh07AnmHeaderSpriteScriptOffsetOffset == 0x20);
    static_assert(th07::kTh07AnmHeaderVersionOffset == 0x28);
    static_assert(th07::kTh07AnmHeaderEmbeddedTextureFlagOffset == 0x34);
    static_assert(th07::kTh07AnmHeaderOwnedFileFlagOffset == 0x35);
    static_assert(th07::kTh07AnmHeaderNextEntryOffsetOffset == 0x38);
    static_assert(th07::kTh07AnmHeaderSpriteOffsetTableOffset == th07::kTh07AnmSpriteOffsetTableOffset);
    static_assert(th07::kTh07AnmHeaderExpectedVersion == 2);
    static_assert(th07::kTh07AnmManagerLoadContract.maxLoadedSlots == 0x32);
    static_assert(th07::Th07AnmManagerSlotFilePointerOffset(0) == 0x2def0);
    static_assert(th07::Th07AnmManagerSlotSpriteOffsetOffset(0) == 0x2def4);
    static_assert(th07::Th07AnmManagerSlotLoadedCountOffset(0) == 0x2def8);
    static_assert(th07::Th07AnmManagerSlotLoadedCountOffset(0x31) == 0x2e144);
    static_assert(th07::Th07AnmManagerLoadedSpriteOffset(0x9ff) == 0x28020);
    static_assert(th07::Th07AnmManagerScriptPointerOffset(0x9ff) == 0x2b6ec);
    static_assert(th07::Th07AnmManagerSpriteIndexOffset(0x9ff) == 0x2deec);
    static_assert(th07::Th07AnmManagerTextureNamePointerOffset(0x31) == 0x28b90);
    static_assert(th07::IsTh07AnmManagerSlotInRange(0x31));
    static_assert(!th07::IsTh07AnmManagerSlotInRange(0x32));
    static_assert(th07::IsTh07AnmManagerGlobalSpriteScriptIdInRange(0x9ff));
    static_assert(!th07::IsTh07AnmManagerGlobalSpriteScriptIdInRange(0xa00));
    static_assert(th07::Th07AnmManagerAdvanceSpriteScriptOffset(0x200, 0x19) == 0x219);

    const std::vector<u8> exe = ReadWholeFile(th07::OriginalReferenceRoot() / "th07.exe");
    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0044df99, {0x33, 0xd2});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07AnmManagerLoadAnmOpenPathCallAddress,
                              th07::kTh07AnmManagerOpenPathFunctionAddress);
    CheckPePushImmediateAtVa(exe, 0x0044dfb9, th07::kTh07AnmManagerLoadFailureStringAddress);
    CheckPePushImmediateAtVa(exe, 0x0044dfbe, th07::kTh07AnmManagerLogBufferAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07AnmManagerLoadAnmFailureLogCallAddress,
                              th07::kTh07AnmManagerLogFunctionAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07AnmManagerLoadAnmSegmentCallAddress,
                              th07::kTh07AnmManagerLoadAnmSegmentFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmFailCountStoreAddress,
                     {0x89, 0x84, 0x0a, 0xf8, 0xde, 0x02, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmNextOffsetCompareAddress,
                     {0x83, 0x79, 0x38, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmSuccessCountStoreAddress,
                     {0x89, 0x94, 0x01, 0xf8, 0xde, 0x02, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmNextEntryAddAddress,
                     {0x03, 0x42, 0x38});

    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerLoadAnmSegmentFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPePushImmediateAtVa(exe, 0x0044e089, th07::kTh07AnmManagerLoadFailureStringAddress);
    CheckPeBytesAtVa(exe, 0x0044e0a3, {0x83, 0x7d, 0x08, 0x32});
    CheckPePushImmediateAtVa(exe, 0x0044e0a9, th07::kTh07AnmManagerInvalidSlotStringAddress);
    CheckPeNearCallTargetAtVa(exe, 0x0044e0ca, th07::kTh07AnmManagerReleaseAnmFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0044e0d8, {0x83, 0x7a, 0x28, 0x02});
    CheckPePushImmediateAtVa(exe, 0x0044e0de, th07::kTh07AnmManagerInvalidVersionStringAddress);
    CheckPeBytesAtVa(exe, 0x0044e0fe, {0x89, 0x48, 0x08});
    CheckPeBytesAtVa(exe, 0x0044e107, {0x88, 0x42, 0x35});
    CheckPePushImmediateAtVa(exe, 0x0044e17f, th07::kTh07AnmManagerTextureFailureStringAddress);
    CheckPePushImmediateAtVa(exe, 0x0044e217, th07::kTh07AnmManagerEmbeddedTextureFailureStringAddress);
    CheckPeBytesAtVa(exe, 0x0044e240, {0x89, 0x94, 0x81, 0xcc, 0x8a, 0x02, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e2ba, {0x89, 0x42, 0x20});
    CheckPeBytesAtVa(exe, 0x0044e3d2, {0x3d, 0x00, 0x0a, 0x00, 0x00});
    CheckPePushImmediateAtVa(exe, 0x0044e3d9, th07::kTh07AnmManagerSpriteRangeFailureStringAddress);
    CheckPeNearCallTargetAtVa(exe, 0x0044e403, th07::kTh07AnmManagerLoadSpriteFunctionAddress);
    CheckPePushImmediateAtVa(exe, 0x0044e442, th07::kTh07AnmManagerScriptRangeFailureStringAddress);
    CheckPeBytesAtVa(exe, 0x0044e47f, {0x89, 0x84, 0x91, 0xf0, 0x8e, 0x02, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e494, {0x89, 0x94, 0x81, 0xf0, 0xb6, 0x02, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e4ac, {0x89, 0x94, 0x01, 0xf0, 0xde, 0x02, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e4bf, {0x89, 0x94, 0x01, 0xf4, 0xde, 0x02, 0x00});

    CheckPeBytesAtVa(exe, th07::kTh07AnmManagerReleaseAnmFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0044e4eb, {0x83, 0x7d, 0x08, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e4f1, {0x83, 0x7d, 0x08, 0x32});
    CheckPeBytesAtVa(exe, 0x0044e505, {0x83, 0xbc, 0x01, 0xf0, 0xde, 0x02, 0x00, 0x00});
    CheckPeNearCallTargetAtVa(exe, 0x0044e58f, th07::kTh07AnmManagerReleaseAnmFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0044e5db, {0x8d, 0x7c, 0x16, 0x60});
    CheckPeBytesAtVa(exe, 0x0044e62a, {0xc7, 0x84, 0x90, 0xf0, 0x8e, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e640, {0xc7, 0x84, 0x90, 0xf0, 0xb6, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e656, {0xc7, 0x84, 0x0a, 0xf4, 0xde, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeNearCallTargetAtVa(exe, 0x0044e66b, th07::kTh07AnmManagerReleaseTextureFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0044e690, {0xc7, 0x84, 0x10, 0xf0, 0xde, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e69e, {0xc6, 0x81, 0xd0, 0xe4, 0x02, 0x00, 0xff});
    CheckPeBytesAtVa(exe, 0x0044e6a8, {0xc6, 0x82, 0xd1, 0xe4, 0x02, 0x00, 0xff});
    CheckPeBytesAtVa(exe, 0x0044e6b2, {0xc6, 0x80, 0xd2, 0xe4, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e6bc, {0xc7, 0x81, 0xcc, 0xe4, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0044e6cf, {0xc7, 0x84, 0x10, 0xf8, 0xde, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00});

    std::uint32_t multiEntryContracts = 0;
    for (const th07::RuntimeAnmLoadContract &contract : th07::kTh07RuntimeAnmLoadContracts) {
        const std::uint32_t slotCount = CheckExtractedAnmLoadContract(contract);
        assert(slotCount >= 1);
        if (slotCount > 1) {
            multiEntryContracts++;
        }
    }
    assert(multiEntryContracts > 0);

    const th07::RuntimeAnmLoadContract *front = th07::FindRuntimeAnmLoadContract("data/front.anm");
    assert(front != nullptr);
    assert(front->anmFileSlot == 0x15);
    assert(front->spriteScriptOffset == 0x0600);

    const th07::RuntimeAnmLoadContract *etama = th07::FindRuntimeAnmLoadContract("data/etama.anm");
    assert(etama != nullptr);
    assert(etama->anmFileSlot == 0x0b);
    assert(etama->spriteScriptOffset == 0x0200);

    const th07::RuntimeAnmLoadContract *title = th07::FindRuntimeAnmLoadContract("data/title01.anm");
    assert(title != nullptr);
    assert(title->anmFileSlot == 0x20);
    assert(title->spriteScriptOffset == 0x0900);

    return 0;
}
