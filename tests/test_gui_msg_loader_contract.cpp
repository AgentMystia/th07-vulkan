#include "ReferencePaths.hpp"
#include "Th07GuiMsgLoaderTables.hpp"
#include "Th07MsgFile.hpp"
#include "Th07ResourceFormatTables.hpp"

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
    static_assert(th07::kTh07GuiLoadMsgFunctionAddress == 0x00429935);
    static_assert(th07::kTh07GuiFreeMsgFunctionAddress == 0x004299f8);
    static_assert(th07::kTh07GuiLoadMsgFreeMsgCallAddress == 0x00429940);
    static_assert(th07::kTh07GuiLoadMsgOpenPathCallAddress == 0x0042994a);
    static_assert(th07::kTh07GuiLoadMsgFileStoreAddress == 0x00429955);
    static_assert(th07::kTh07GuiLoadMsgNullCompareAddress == 0x00429961);
    static_assert(th07::kTh07GuiLoadMsgErrorStringPushAddress == 0x0042996d);
    static_assert(th07::kTh07GuiLoadMsgErrorBufferPushAddress == 0x00429972);
    static_assert(th07::kTh07GuiLoadMsgErrorLogCallAddress == 0x00429977);
    static_assert(th07::kTh07GuiLoadMsgCurrentIndexInitAddress == 0x0042998a);
    static_assert(th07::kTh07GuiLoadMsgCurrentInstructionInitAddress == 0x00429997);
    static_assert(th07::kTh07GuiLoadMsgEntryPointerReadAddress == 0x004299d3);
    static_assert(th07::kTh07GuiLoadMsgEntryPointerRelocateAddress == 0x004299d7);
    static_assert(th07::kTh07GuiLoadMsgEntryPointerWriteAddress == 0x004299ec);
    static_assert(th07::kTh07GuiLoadMsgOpenPathFunctionAddress == 0x00431330);
    static_assert(th07::kTh07GuiLoadMsgLogFunctionAddress == 0x004315f0);
    static_assert(th07::kTh07GuiFreeMsgFreeFunctionAddress == 0x0047d285);
    static_assert(th07::kTh07GuiLoadMsgLogBufferAddress == 0x00624210);
    static_assert(th07::kTh07GuiLoadMsgFailureStringAddress == 0x00498108);
    static_assert(th07::kTh07GuiLoadMsgOpenPathExternalFlag == 0);
    static_assert(th07::kTh07GuiLoadMsgOwnerPointerOffset == 0x0008);
    static_assert(th07::kTh07GuiLoadMsgMsgFileOffset == 0x1fba4);
    static_assert(th07::kTh07GuiLoadMsgCurrentInstructionOffset == 0x1fba8);
    static_assert(th07::kTh07GuiLoadMsgCurrentIndexOffset == 0x1fbac);
    static_assert(th07::kTh07GuiLoadMsgInvalidCurrentIndex == -1);
    static_assert(th07::kTh07GuiLoadMsgInitialCurrentInstruction == 0);
    static_assert(th07::kTh07GuiLoadMsgHeaderEntryCountOffset == 0x0000);
    static_assert(th07::kTh07GuiLoadMsgEntryPointerTableOffset == 0x0004);
    static_assert(th07::kTh07GuiLoadMsgEntryPointerSize == 0x0004);
    static_assert(th07::Th07GuiMsgEntryPointerFileOffset(0) == 0x0004);
    static_assert(th07::Th07GuiMsgEntryPointerFileOffset(7) == 0x0020);
    static_assert(th07::Th07GuiMsgPointerTableByteSize(23) == 0x0060);
    static_assert(th07::Th07GuiMsgRelocatedEntryPointer(0x10000000, 0x25c0) == 0x100025c0);

    const std::vector<u8> exe = ReadWholeFile(th07::OriginalReferenceRoot() / "th07.exe");
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07GuiLoadMsgFreeMsgCallAddress,
                              th07::kTh07GuiFreeMsgFunctionAddress);
    CheckPeBytesAtVa(exe, 0x00429945, {0x33, 0xd2});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07GuiLoadMsgOpenPathCallAddress,
                              th07::kTh07GuiLoadMsgOpenPathFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgFileStoreAddress,
                     {0x89, 0x81, 0xa4, 0xfb, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgNullCompareAddress,
                     {0x83, 0xb8, 0xa4, 0xfb, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgPathPushAddress, {0xff, 0x75, 0x08});
    CheckPePushImmediateAtVa(exe, th07::kTh07GuiLoadMsgErrorStringPushAddress,
                             th07::kTh07GuiLoadMsgFailureStringAddress);
    CheckPePushImmediateAtVa(exe, th07::kTh07GuiLoadMsgErrorBufferPushAddress,
                             th07::kTh07GuiLoadMsgLogBufferAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07GuiLoadMsgErrorLogCallAddress,
                              th07::kTh07GuiLoadMsgLogFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgCurrentIndexInitAddress,
                     {0x83, 0x88, 0xac, 0xfb, 0x01, 0x00, 0xff});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgCurrentInstructionInitAddress,
                     {0x83, 0xa0, 0xa8, 0xfb, 0x01, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgEntryCountCompareAddress, {0x3b, 0x08});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgEntryPointerReadAddress,
                     {0x8b, 0x44, 0x90, 0x04});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgEntryPointerRelocateAddress,
                     {0x03, 0x81, 0xa4, 0xfb, 0x01, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgEntryPointerWriteAddress,
                     {0x89, 0x44, 0x91, 0x04});
    CheckPeBytesAtVa(exe, th07::kTh07GuiLoadMsgSuccessReturnAddress, {0x33, 0xc0});
    CheckPeBytesAtVa(exe, th07::kTh07GuiFreeMsgFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, th07::kTh07GuiFreeMsgNullCompareAddress,
                     {0x83, 0xb8, 0xa4, 0xfb, 0x01, 0x00, 0x00});
    CheckPeNearCallTargetAtVa(exe, th07::kTh07GuiFreeMsgFreeCallAddress,
                              th07::kTh07GuiFreeMsgFreeFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07GuiFreeMsgFileClearAddress,
                     {0x83, 0xa0, 0xa4, 0xfb, 0x01, 0x00, 0x00});

    constexpr std::uint32_t fakeFileBase = 0x10000000;
    for (std::uint8_t stage = 1; stage <= th07::kTh07MsgDumpCount; stage++) {
        const std::vector<u8> bytes =
            ReadWholeFile(th07::ExtractedReferenceRoot() /
                          ("msg" + std::to_string(stage) + ".dat"));
        th07::Th07MsgFile file;
        assert(th07::ParseTh07MsgFile(bytes, &file));

        const std::uint32_t entryPointerCount =
            ReadU32(bytes, th07::kTh07GuiLoadMsgHeaderEntryCountOffset);
        assert(entryPointerCount == file.entryOffsets.size());
        assert(th07::Th07GuiMsgPointerTableByteSize(entryPointerCount) <= bytes.size());
        for (std::uint32_t index = 0; index < entryPointerCount; index++) {
            const std::uint32_t rawOffset =
                ReadU32(bytes, th07::Th07GuiMsgEntryPointerFileOffset(index));
            assert(rawOffset == file.entryOffsets[index]);
            assert(th07::Th07GuiMsgRelocatedEntryPointer(fakeFileBase, rawOffset) ==
                   fakeFileBase + file.entryOffsets[index]);
        }
        for (const th07::Th07MsgEntry &entry : file.uniqueEntries) {
            assert(entry.offset >= th07::Th07GuiMsgPointerTableByteSize(entryPointerCount));
            assert(entry.offset < bytes.size());
            assert(!entry.instructions.empty());
            assert(ReadU16(bytes, entry.offset) == entry.instructions.front().time);
            assert(bytes[entry.offset + 2] == entry.instructions.front().opcode);
            assert(bytes[entry.offset + 3] == entry.instructions.front().argumentSize);
        }
    }

    return 0;
}
