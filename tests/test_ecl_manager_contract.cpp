#include "ReferencePaths.hpp"
#include "Th07EclFile.hpp"
#include "Th07EclManagerTables.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
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

} // namespace

int main()
{
    static_assert(th07::kTh07EclManagerLoadFunctionAddress == 0x0040e420);
    static_assert(th07::kTh07EclManagerUnloadFunctionAddress == 0x0040e4f0);
    static_assert(th07::kTh07EclManagerCallSubFunctionAddress == 0x0040e530);
    static_assert(th07::kTh07EclManagerOwnerFileOffset == 0x0000);
    static_assert(th07::kTh07EclManagerOwnerSubOffsetTableOffset == 0x0004);
    static_assert(th07::kTh07EclManagerLoadHeaderRelocationBaseOffset == 0x0004);
    static_assert(th07::kTh07EclManagerLoadHeaderRelocationEntryCount == 0x0010);
    static_assert(th07::kTh07EclManagerLoadHeaderRelocationLastOffset ==
                  th07::kTh07EclManagerSubOffsetSentinelFileOffset);
    static_assert(th07::kTh07EclManagerRuntimeSubOffsetTableFileOffset ==
                  th07::kTh07EclSubOffsetTableOffset + sizeof(u32));
    static_assert(th07::kTh07EclManagerRuntimeSubOffsetTableSkipsSentinel);
    static_assert(th07::kTh07EclManagerContextCurrentInstrOffset == 0x0000);
    static_assert(th07::kTh07EclManagerContextPrimaryTimerPreviousOffset == 0x0004);
    static_assert(th07::kTh07EclManagerContextPrimaryTimerSubframeOffset == 0x0008);
    static_assert(th07::kTh07EclManagerContextPrimaryTimerCurrentOffset == 0x000c);
    static_assert(th07::kTh07EclManagerContextInterruptTimerPreviousOffset == 0x0080);
    static_assert(th07::kTh07EclManagerContextInterruptTimerSubframeOffset == 0x0084);
    static_assert(th07::kTh07EclManagerContextInterruptTimerCurrentOffset == 0x0088);
    static_assert(th07::kTh07EclManagerContextSubIdOffset == 0x0214);
    static_assert(th07::kTh07EclManagerContextMinimumKnownSpan == 0x0216);
    static_assert(th07::kTh07EclManagerTimerPreviousSentinel == -999);
    static_assert(th07::Th07EclManagerHeaderRelocationEntryFileOffset(0) == 0x0004);
    static_assert(th07::Th07EclManagerHeaderRelocationEntryFileOffset(15) == 0x0040);
    static_assert(th07::Th07EclManagerRuntimeSubOffsetEntryFileOffset(0) == 0x0044);

    const std::vector<u8> exe = ReadWholeFile(th07::OriginalReferenceRoot() / "th07.exe");
    CheckPeBytesAtVa(exe, th07::kTh07EclManagerLoadFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07EclManagerLoadOpenPathCallAddress,
                              th07::kTh07EclManagerOpenPathFunctionAddress);
    CheckPePushImmediateAtVa(exe, th07::kTh07EclManagerLoadErrorStringPushAddress,
                             th07::kTh07EclManagerLoadFailureStringAddress);
    CheckPePushImmediateAtVa(exe, th07::kTh07EclManagerLoadErrorBufferPushAddress,
                             th07::kTh07EclManagerLogBufferAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07EclManagerLoadErrorLogCallAddress,
                              th07::kTh07EclManagerLogFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0040e46c, {0x83, 0x7d, 0xfc, 0x10});
    CheckPeBytesAtVa(exe, 0x0040e47a, {0x8b, 0x4c, 0x82, 0x04});
    CheckPeBytesAtVa(exe, 0x0040e48b, {0x89, 0x4c, 0x82, 0x04});
    CheckPeBytesAtVa(exe, 0x0040e496, {0x83, 0xc2, 0x44});
    CheckPeBytesAtVa(exe, 0x0040e49c, {0x89, 0x50, 0x04});
    CheckPeBytesAtVa(exe, 0x0040e4b6, {0x0f, 0xbf, 0x08});
    CheckPeBytesAtVa(exe, 0x0040e4c7, {0x8b, 0x14, 0x88});
    CheckPeBytesAtVa(exe, 0x0040e4d8, {0x89, 0x14, 0x88});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07EclManagerUnloadFreeCallAddress,
                              th07::kTh07EclManagerFreeFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07EclManagerCallSubFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0040e539, {0x0f, 0xbf, 0x45, 0x0c});
    CheckPeBytesAtVa(exe, 0x0040e540, {0x8b, 0x51, 0x04});
    CheckPeBytesAtVa(exe, 0x0040e549, {0x89, 0x11});
    CheckPeBytesAtVa(exe, 0x0040e54e, {0x83, 0xc0, 0x04});
    CheckPeBytesAtVa(exe, 0x0040e557, {0xc7, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040e561, {0xc7, 0x42, 0x04, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040e56b, {0xc7, 0x00, 0x19, 0xfc, 0xff, 0xff});
    CheckPeBytesAtVa(exe, 0x0040e574, {0x81, 0xc1, 0x80, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040e580, {0xc7, 0x42, 0x08, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040e58a, {0xc7, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, 0x0040e594, {0xc7, 0x01, 0x19, 0xfc, 0xff, 0xff});
    CheckPeBytesAtVa(exe, 0x0040e5a1, {0x66, 0x89, 0x82, 0x14, 0x02, 0x00, 0x00});

    const std::vector<u8> stage1Bytes =
        ReadWholeFile(th07::ExtractedReferenceRoot() / "ecldata1.ecl");
    th07::Th07EclFile stage1;
    assert(th07::ParseTh07EclFile(stage1Bytes, &stage1));
    assert(ReadU16(stage1Bytes, th07::kTh07EclManagerSubCountOffset) == stage1.header.subCount);
    assert(ReadU32(stage1Bytes, th07::kTh07EclManagerSubOffsetSentinelFileOffset) == 0);
    assert(ReadU32(stage1Bytes, th07::kTh07EclManagerRuntimeSubOffsetTableFileOffset) ==
           stage1.subOffsets.front());
    assert(ReadU32(stage1Bytes, th07::Th07EclManagerHeaderRelocationEntryFileOffset(0)) ==
           stage1.header.timelineOffsets[0]);
    assert(ReadU32(stage1Bytes, th07::Th07EclManagerHeaderRelocationEntryFileOffset(1)) ==
           stage1.header.timelineOffsets[1]);
    assert(ReadU32(stage1Bytes, th07::Th07EclManagerRuntimeSubOffsetEntryFileOffset(1)) ==
           stage1.subOffsets[1]);
    assert(stage1.subOffsets.front() ==
           th07::kTh07EclManagerRuntimeSubOffsetTableFileOffset +
               static_cast<u32>(stage1.header.subCount) * sizeof(u32));

    return 0;
}
