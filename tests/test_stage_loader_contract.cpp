#include "ReferencePaths.hpp"
#include "Th07ResourceFormatTables.hpp"
#include "Th07StageLoaderTables.hpp"
#include "Th07StdFile.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::uint16_t ReadU16(const std::vector<u8> &bytes, std::size_t offset)
{
    assert(offset + 2 <= bytes.size());
    return static_cast<std::uint16_t>(bytes[offset]) |
           (static_cast<std::uint16_t>(bytes[offset + 1]) << 8);
}

std::int16_t ReadI16(const std::vector<u8> &bytes, std::size_t offset)
{
    return static_cast<std::int16_t>(ReadU16(bytes, offset));
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
    assert(std::equal(expected.begin(), expected.end(),
                      bytes.begin() + static_cast<std::ptrdiff_t>(offset)));
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

std::size_t CountRawStdQuads(const std::vector<u8> &bytes, std::uint32_t objectOffset)
{
    std::size_t quads = 0;
    std::size_t quadOffset = objectOffset + th07::kTh07StageLoadStdObjectFirstQuadOffset;
    while (ReadI16(bytes, quadOffset + th07::kTh07StageLoadStdQuadTypeOffset) >= 0) {
        const std::uint16_t byteSize =
            ReadU16(bytes, quadOffset + th07::kTh07StageLoadStdQuadByteSizeOffset);
        assert(byteSize >= th07::kTh07StdQuadMinimumSize);
        quads++;
        quadOffset += byteSize;
        assert(quadOffset <= bytes.size());
    }
    return quads;
}

} // namespace

int main()
{
    static_assert(th07::kTh07StageLoadStdFunctionAddress == 0x00407610);
    static_assert(th07::kTh07StageLoadStdOpenPathCallAddress == 0x0040761e);
    static_assert(th07::kTh07StageLoadStdErrorStringPushAddress == 0x00407638);
    static_assert(th07::kTh07StageLoadStdErrorBufferPushAddress == 0x0040763d);
    static_assert(th07::kTh07StageLoadStdErrorLogCallAddress == 0x00407642);
    static_assert(th07::kTh07StageLoadStdMallocCallAddress == 0x00407730);
    static_assert(th07::kTh07StageLoadStdSetScriptCallAddress == 0x004077b3);
    static_assert(th07::kTh07StageLoadStdOpenPathFunctionAddress == 0x00431330);
    static_assert(th07::kTh07StageLoadStdLogFunctionAddress == 0x004315f0);
    static_assert(th07::kTh07StageLoadStdMallocFunctionAddress == 0x0047d39d);
    static_assert(th07::kTh07StageLoadStdSetScriptFunctionAddress == 0x00404f30);
    static_assert(th07::kTh07StageLoadStdAnmManagerPointerAddress == 0x004b9e44);
    static_assert(th07::kTh07StageLoadStdLogBufferAddress == 0x00624210);
    static_assert(th07::kTh07StageLoadStdFailureStringAddress == 0x0049888c);
    static_assert(th07::kTh07StageLoadStdOwnerVmArrayOffset == 0x0000);
    static_assert(th07::kTh07StageLoadStdOwnerStdDataOffset == 0x049c);
    static_assert(th07::kTh07StageLoadStdOwnerQuadCountOffset == 0x04a0);
    static_assert(th07::kTh07StageLoadStdOwnerObjectCountOffset == 0x04a4);
    static_assert(th07::kTh07StageLoadStdOwnerObjectTableOffset == 0x04a8);
    static_assert(th07::kTh07StageLoadStdOwnerObjectInstancesOffset == 0x04ac);
    static_assert(th07::kTh07StageLoadStdOwnerScriptOffset == 0x04b0);
    static_assert(th07::kTh07StageLoadStdObjectOffsetTableFileOffset == 0x0490);
    static_assert(th07::kTh07StageLoadStdObjectFlagsOffset == 0x0003);
    static_assert(th07::kTh07StageLoadStdObjectActiveFlag == 1);
    static_assert(th07::kTh07StageLoadStdObjectFirstQuadOffset == 0x001c);
    static_assert(th07::kTh07StageLoadStdQuadTypeOffset == 0x0000);
    static_assert(th07::kTh07StageLoadStdQuadByteSizeOffset == 0x0002);
    static_assert(th07::kTh07StageLoadStdQuadAnmScriptOffset == 0x0004);
    static_assert(th07::kTh07StageLoadStdQuadVmIndexOffset == 0x0006);
    static_assert(th07::kTh07StageLoadStdQuadVmSlotSize == 0x024c);
    static_assert(th07::kTh07StageLoadStdAnmScriptBase == 0x0300);
    static_assert(th07::Th07StageStdObjectOffsetTableEntryFileOffset(0) == 0x0490);
    static_assert(th07::Th07StageStdObjectOffsetTableEntryFileOffset(3) == 0x049c);
    static_assert(th07::Th07StageStdVmAllocationSize(4) == 0x0930);

    const std::vector<u8> exe = ReadWholeFile(th07::OriginalReferenceRoot() / "th07.exe");
    CheckPeBytesAtVa(exe, th07::kTh07StageLoadStdFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07StageLoadStdOpenPathCallAddress,
                              th07::kTh07StageLoadStdOpenPathFunctionAddress);
    CheckPeBytesAtVa(exe, 0x00407626, {0x89, 0x81, 0x9c, 0x04, 0x00, 0x00});
    CheckPePushImmediateAtVa(exe, th07::kTh07StageLoadStdErrorStringPushAddress,
                             th07::kTh07StageLoadStdFailureStringAddress);
    CheckPePushImmediateAtVa(exe, th07::kTh07StageLoadStdErrorBufferPushAddress,
                             th07::kTh07StageLoadStdLogBufferAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07StageLoadStdErrorLogCallAddress,
                              th07::kTh07StageLoadStdLogFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0040765b, {0x0f, 0xbf, 0x11});
    CheckPeBytesAtVa(exe, 0x00407661, {0x89, 0x90, 0xa4, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00407670, {0x0f, 0xbf, 0x42, 0x02});
    CheckPeBytesAtVa(exe, 0x00407677, {0x89, 0x81, 0xa0, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040768c, {0x03, 0x8a, 0x9c, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00407695, {0x89, 0x88, 0xac, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004076aa, {0x03, 0x81, 0x9c, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004076b3, {0x89, 0x82, 0xb0, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004076c2, {0x81, 0xc1, 0x90, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004076cb, {0x89, 0x8a, 0xa8, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004076e9, {0x3b, 0x91, 0xa4, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00407703, {0x03, 0x81, 0x9c, 0x04, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x00407715, {0x89, 0x04, 0x91});
    CheckPeBytesAtVa(exe, 0x00407723, {0x69, 0xc9, 0x4c, 0x02, 0x00, 0x00});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07StageLoadStdMallocCallAddress,
                              th07::kTh07StageLoadStdMallocFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0040773b, {0x89, 0x01});
    CheckPeBytesAtVa(exe, 0x00407779, {0xc6, 0x40, 0x03, 0x01});
    CheckPeBytesAtVa(exe, 0x00407780, {0x83, 0xc1, 0x1c});
    CheckPeBytesAtVa(exe, 0x00407793, {0x0f, 0xbf, 0x51, 0x04});
    CheckPeBytesAtVa(exe, 0x00407797, {0x81, 0xc2, 0x00, 0x03, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004077a1, {0x69, 0xc0, 0x4c, 0x02, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x004077ad, {0x8b, 0x0d, 0x44, 0x9e, 0x4b, 0x00});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07StageLoadStdSetScriptCallAddress,
                              th07::kTh07StageLoadStdSetScriptFunctionAddress);
    CheckPeBytesAtVa(exe, 0x004077bf, {0x66, 0x89, 0x42, 0x06});
    CheckPeBytesAtVa(exe, 0x004077cf, {0x0f, 0xbf, 0x42, 0x02});

    for (std::uint8_t stage = 1; stage <= th07::kTh07StdFileCount; stage++) {
        const std::vector<u8> bytes =
            ReadWholeFile(th07::ExtractedReferenceRoot() /
                          ("stage" + std::to_string(stage) + ".std"));
        th07::Th07StdFile file;
        assert(th07::ParseTh07StdFile(bytes, &file));
        assert(ReadI16(bytes, th07::kTh07StageLoadStdHeaderObjectCountOffset) ==
               file.header.objectCount);
        assert(ReadI16(bytes, th07::kTh07StageLoadStdHeaderQuadCountOffset) ==
               file.header.declaredQuadCount);
        assert(ReadU32(bytes, th07::kTh07StageLoadStdHeaderInstancesOffsetOffset) ==
               file.header.instancesOffset);
        assert(ReadU32(bytes, th07::kTh07StageLoadStdHeaderScriptOffsetOffset) ==
               file.header.scriptOffset);
        assert(th07::Th07StageStdVmAllocationSize(file.header.declaredQuadCount) ==
               static_cast<u32>(file.header.declaredQuadCount) *
                   th07::kTh07StageLoadStdQuadVmSlotSize);

        std::size_t rawQuadCount = 0;
        for (std::uint16_t objectIndex = 0; objectIndex < file.header.objectCount; objectIndex++) {
            const std::uint32_t objectOffset = ReadU32(
                bytes, th07::Th07StageStdObjectOffsetTableEntryFileOffset(objectIndex));
            assert(objectOffset >= th07::kTh07StdHeaderSize);
            assert(objectOffset < bytes.size());
            assert(ReadI16(bytes, objectOffset) == file.objects[objectIndex].id);
            rawQuadCount += CountRawStdQuads(bytes, objectOffset);
        }
        assert(rawQuadCount == th07::CountTh07StdQuads(file));
        assert(rawQuadCount == static_cast<std::size_t>(file.header.declaredQuadCount));
    }

    return 0;
}
