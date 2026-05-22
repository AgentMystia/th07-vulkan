#include "ReferencePaths.hpp"
#include "Th07ArchiveReader.hpp"
#include "Th07FileSystemRuntime.hpp"
#include "Th07FileSystemTables.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr std::uint32_t kTextVa = 0x00401000;
constexpr std::uint32_t kTextFileOffset = 0x00000400;
constexpr std::uint32_t kTextFileSize = 0x0008be00;
constexpr std::uint32_t kRdataVa = 0x0048d000;
constexpr std::uint32_t kRdataFileOffset = 0x0008c200;
constexpr std::uint32_t kRdataFileSize = 0x00019a00;

std::size_t PeOffsetForVa(std::uint32_t address)
{
    if (address >= kTextVa && address < kTextVa + kTextFileSize) {
        return kTextFileOffset + (address - kTextVa);
    }
    if (address >= kRdataVa && address < kRdataVa + kRdataFileSize) {
        return kRdataFileOffset + (address - kRdataVa);
    }
    throw std::runtime_error("address is outside mapped TH07 PE sections");
}

std::vector<std::uint8_t> ReadWholeFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("failed to open " + path.string());
    }
    return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(file),
                                    std::istreambuf_iterator<char>());
}

void CheckPeBytesAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                      std::initializer_list<std::uint8_t> expected)
{
    const std::size_t offset = PeOffsetForVa(address);
    assert(offset + expected.size() <= bytes.size());
    std::size_t i = 0;
    for (std::uint8_t value : expected) {
        assert(bytes[offset + i] == value);
        i += 1;
    }
}

std::uint32_t ReadLeU32(const std::vector<std::uint8_t> &bytes, std::size_t offset)
{
    assert(offset + 4 <= bytes.size());
    return static_cast<std::uint32_t>(bytes[offset]) |
           (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

void CheckPePushImmediateAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                              std::uint32_t expected)
{
    const std::size_t offset = PeOffsetForVa(address);
    assert(bytes[offset] == 0x68);
    assert(ReadLeU32(bytes, offset + 1) == expected);
}

void CheckPeMovEcxImmediateAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                                std::uint32_t expected)
{
    const std::size_t offset = PeOffsetForVa(address);
    assert(bytes[offset] == 0xb9);
    assert(ReadLeU32(bytes, offset + 1) == expected);
}

void CheckPeNearCallTargetAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                               std::uint32_t expectedTarget)
{
    const std::size_t offset = PeOffsetForVa(address);
    assert(bytes[offset] == 0xe8);
    const auto relative = static_cast<std::int32_t>(ReadLeU32(bytes, offset + 1));
    const auto target = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(address) + 5 + static_cast<std::int64_t>(relative));
    assert(target == expectedTarget);
}

void CheckPeStringAtVa(const std::vector<std::uint8_t> &bytes, std::uint32_t address,
                       const char *expected)
{
    const std::size_t offset = PeOffsetForVa(address);
    const std::size_t length = std::strlen(expected);
    assert(offset + length + 1 <= bytes.size());
    assert(std::memcmp(bytes.data() + offset, expected, length) == 0);
    assert(bytes[offset + length] == 0);
}

} // namespace

int main()
{
    assert(th07::kTh07OpenPathFunctionAddress == 0x00431330);
    assert(th07::kTh07ArchiveLookupObjectAddress == 0x00626258);
    assert(th07::kTh07ArchiveOwnerOpenFunctionAddress == 0x00438668);
    assert(th07::kTh07ArchiveOwnerOpenPathPushAddress == 0x00438671);
    assert(th07::kTh07ArchiveOwnerOpenThisMovAddress == 0x00438676);
    assert(th07::kTh07ArchiveOwnerOpenCallAddress == 0x0043867b);
    assert(th07::kTh07ArchiveOwnerCloseHostFunctionAddress == 0x00438de2);
    assert(th07::kTh07ArchiveOwnerCloseThisMovAddress == 0x00438f8b);
    assert(th07::kTh07ArchiveOwnerCloseCallAddress == 0x00438f90);
    assert(th07::kTh07OpenPathLastFileSizeGlobalAddress == 0x004b9e64);
    assert(th07::kTh07ArchiveOpenFunctionAddress == 0x0045f710);
    assert(th07::kTh07ArchiveCloseFunctionAddress == 0x0045f800);
    assert(th07::kTh07ArchiveMagicPbg4 == 0x34474250);
    assert(th07::kTh07ArchiveHeaderMagicOffset == 0x00);
    assert(th07::kTh07ArchiveHeaderEntryCountOffset == 0x04);
    assert(th07::kTh07ArchiveHeaderPackedIndexOffsetOffset == 0x08);
    assert(th07::kTh07ArchiveHeaderDecodedIndexSizeOffset == 0x0c);
    assert(th07::kTh07ArchiveHeaderSize == 0x10);
    assert(th07::kTh07ArchiveOwnerEntryTableOffset == 0x00);
    assert(th07::kTh07ArchiveOwnerEntryCountOffset == 0x04);
    assert(th07::kTh07ArchiveOwnerPathCopyOffset == 0x08);
    assert(th07::kTh07ArchiveOwnerStreamOffset == 0x0c);
    assert(th07::kTh07ArchiveOwnerFieldSpan == 0x10);
    assert(th07::kTh07ArchiveEntryStride == 0x10);
    assert(th07::kTh07ArchiveEntryNamePointerOffset == 0x00);
    assert(th07::kTh07ArchiveEntryPackedOffsetOffset == 0x04);
    assert(th07::kTh07ArchiveEntryDecodedSizeOffset == 0x08);
    assert(th07::kTh07ArchiveEntryMetadataOffset == 0x0c);
    assert(th07::kTh07ArchiveLoadIndexFunctionAddress == 0x0045fb50);
    assert(th07::kTh07ArchiveBuildEntryTableFunctionAddress == 0x0045fde0);
    assert(th07::kTh07ArchiveCopyStringFunctionAddress == 0x0045ffc0);
    assert(th07::kTh07ArchiveFindEntryFunctionAddress == 0x0045fae0);
    assert(th07::kTh07ArchiveEntrySizeFunctionAddress == 0x0045fab0);
    assert(th07::kTh07ArchiveReadEntryFunctionAddress == 0x0045f960);
    assert(th07::kTh07ArchiveMissLogBufferAddress == 0x00624210);
    assert(th07::kTh07ArchivePathStringAddress == 0x00497150);
    assert(th07::kTh07ArchiveVersionProbeFormatStringAddress == 0x00497140);
    assert(th07::kTh07ArchiveVersionProbeCode == 0x0100);
    assert(th07::kTh07ArchiveVersionProbeSuffix == 'b');
    assert(std::strcmp(th07::kTh07ArchivePath, "th07.dat") == 0);
    assert(std::strcmp(th07::kTh07ArchiveVersionProbeFormat, "th07_%.4x%c.ver") == 0);
    assert(std::strcmp(th07::kTh07ArchiveVersionProbePath, "th07_0100b.ver") == 0);
    assert(th07::kTh07ArchiveOpenLogStringAddress == 0x00495100);
    assert(th07::kTh07ArchiveCloseLogStringAddress == 0x004950cc);
    assert(th07::kTh07ArchiveOpenMissingLogStringAddress == 0x004950e8);
    assert(th07::kTh07ArchiveOpenErrorLogStringAddress == 0x00495084);
    assert(th07::kTh07ArchiveMissStringAddress == 0x00497d38);
    assert(th07::kTh07ArchiveDecodeStringAddress == 0x00497d24);
    assert(th07::kTh07ExternalLoadStringAddress == 0x00497d14);
    assert(th07::kTh07ExternalMissingStringAddress == 0x00497cf8);

    const std::vector<std::uint8_t> exe = ReadWholeFile(th07::OriginalReferenceRoot() / "th07.exe");
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveOwnerOpenFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPePushImmediateAtVa(exe, th07::kTh07ArchiveOwnerOpenPathPushAddress,
                             th07::kTh07ArchivePathStringAddress);
    CheckPeMovEcxImmediateAtVa(exe, th07::kTh07ArchiveOwnerOpenThisMovAddress,
                               th07::kTh07ArchiveLookupObjectAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07ArchiveOwnerOpenCallAddress,
                              th07::kTh07ArchiveOpenFunctionAddress);
    CheckPeBytesAtVa(exe, 0x00438687,
                     {0x6a, static_cast<std::uint8_t>(th07::kTh07ArchiveVersionProbeSuffix)});
    CheckPePushImmediateAtVa(exe, 0x00438689, th07::kTh07ArchiveVersionProbeCode);
    CheckPePushImmediateAtVa(exe, 0x0043868e,
                             th07::kTh07ArchiveVersionProbeFormatStringAddress);
    CheckPeNearCallTargetAtVa(exe, 0x004386a4, th07::kTh07OpenPathFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveOwnerCloseHostFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeMovEcxImmediateAtVa(exe, th07::kTh07ArchiveOwnerCloseThisMovAddress,
                               th07::kTh07ArchiveLookupObjectAddress);
    CheckPeNearCallTargetAtVa(exe, th07::kTh07ArchiveOwnerCloseCallAddress,
                              th07::kTh07ArchiveCloseFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveOpenFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeNearCallTargetAtVa(exe, 0x0045f731, th07::kTh07ArchiveCloseFunctionAddress);
    CheckPePushImmediateAtVa(exe, 0x0045f73a, th07::kTh07ArchiveOpenLogStringAddress);
    CheckPeNearCallTargetAtVa(exe, 0x0045f79f, th07::kTh07ArchiveLoadIndexFunctionAddress);
    CheckPeNearCallTargetAtVa(exe, 0x0045f7b2, th07::kTh07ArchiveCopyStringFunctionAddress);
    CheckPePushImmediateAtVa(exe, 0x0045f7ce, th07::kTh07ArchiveOpenMissingLogStringAddress);
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveCloseFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPePushImmediateAtVa(exe, 0x0045f810, th07::kTh07ArchiveCloseLogStringAddress);
    CheckPeBytesAtVa(exe, 0x0045f829, {0x8b, 0x48, th07::kTh07ArchiveOwnerPathCopyOffset});
    CheckPeBytesAtVa(exe, 0x0045f8bc,
                     {0xc7, 0x41, th07::kTh07ArchiveOwnerEntryCountOffset, 0x00, 0x00, 0x00, 0x00});
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveFindEntryFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeBytesAtVa(exe, 0x0045fb14, {0x83, 0xc0, th07::kTh07ArchiveEntryStride});
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveEntrySizeFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeNearCallTargetAtVa(exe, 0x0045fac0, th07::kTh07ArchiveFindEntryFunctionAddress);
    CheckPeBytesAtVa(exe, 0x0045fad1,
                     {0x8b, 0x41, th07::kTh07ArchiveEntryDecodedSizeOffset});
    CheckPeNearCallTargetAtVa(exe, 0x0045fcfc,
                              th07::kTh07ArchiveBuildEntryTableFunctionAddress);
    CheckPeBytesAtVa(exe, th07::kTh07ArchiveCopyStringFunctionAddress, {0x55, 0x8b, 0xec});
    CheckPeStringAtVa(exe, th07::kTh07ArchivePathStringAddress, th07::kTh07ArchivePath);
    CheckPeStringAtVa(exe, th07::kTh07ArchiveVersionProbeFormatStringAddress,
                      th07::kTh07ArchiveVersionProbeFormat);

    th07::Th07ArchiveReader archive;
    assert(archive.LoadFromFile(th07::OriginalReferenceRoot() / th07::kTh07ArchivePath));
    assert(archive.EntryCount() == 0x00c5);
    assert(archive.PackedIndexOffset() == 0x016b8fd4);
    assert(archive.DecodedIndexSize() == 0x1573);

    const th07::Th07ArchiveReaderEntry *versionEntry =
        archive.FindEntry(th07::kTh07ArchiveVersionProbePath);
    assert(versionEntry != nullptr);
    assert(versionEntry->name == th07::kTh07ArchiveVersionProbePath);
    assert(archive.FindEntry("TH07_0100B.VER") == versionEntry);

    std::vector<u8> decodedEntry;
    assert(archive.ReadEntry(th07::kTh07ArchiveVersionProbePath, &decodedEntry));
    assert(decodedEntry == ReadWholeFile(th07::ExtractedReferenceRoot() /
                                         th07::kTh07ArchiveVersionProbePath));

    assert(archive.ReadEntry("ecldata1.ecl", &decodedEntry));
    assert(decodedEntry == ReadWholeFile(th07::ExtractedReferenceRoot() / "ecldata1.ecl"));
    assert(!archive.ReadEntry("data/ecldata1.ecl", &decodedEntry));

    th07::Th07FileSystemRuntime fs;
    assert(!fs.IsArchiveOpen());
    assert(fs.OpenDefaultArchive());
    assert(fs.IsArchiveOpen());
    assert(fs.ArchivePath().filename() == th07::kTh07ArchivePath);
    assert(fs.VersionProbeSize() == fs.VersionProbeData().size());
    assert(fs.VersionProbeData() == ReadWholeFile(th07::ExtractedReferenceRoot() /
                                                  th07::kTh07ArchiveVersionProbePath));

    th07::Th07FileSystemOpenResult openedArchivePath = fs.OpenPath("data/ecldata1.ecl", false);
    assert(openedArchivePath.status == th07::Th07FileSystemOpenStatus::Success);
    assert(openedArchivePath.lookupName == "ecldata1.ecl");
    assert(!openedArchivePath.externalResource);
    assert(openedArchivePath.lastFileSize == openedArchivePath.data.size());
    assert(openedArchivePath.data == ReadWholeFile(th07::ExtractedReferenceRoot() / "ecldata1.ecl"));

    th07::Th07FileSystemOpenResult openedUppercase = fs.OpenPath("DATA/ECLDATA1.ECL", false);
    assert(openedUppercase.status == th07::Th07FileSystemOpenStatus::Success);
    assert(openedUppercase.lookupName == "ECLDATA1.ECL");
    assert(openedUppercase.data == openedArchivePath.data);

    th07::Th07FileSystemOpenResult missingArchivePath = fs.OpenPath("data/missing.bin", false);
    assert(missingArchivePath.status == th07::Th07FileSystemOpenStatus::EntryNotFound);
    assert(missingArchivePath.lookupName == "missing.bin");
    assert(missingArchivePath.data.empty());
    assert(missingArchivePath.lastFileSize == 0);

    const std::filesystem::path externalPath = th07::ExtractedReferenceRoot() / "ecldata1.ecl";
    th07::Th07FileSystemOpenResult openedExternal = fs.OpenPath(externalPath.string(), true);
    assert(openedExternal.status == th07::Th07FileSystemOpenStatus::Success);
    assert(openedExternal.externalResource);
    assert(openedExternal.lookupName == externalPath.string());
    assert(openedExternal.lastFileSize == openedExternal.data.size());
    assert(openedExternal.data == openedArchivePath.data);

    fs.CloseArchive();
    assert(!fs.IsArchiveOpen());
    th07::Th07FileSystemOpenResult closedArchivePath = fs.OpenPath("data/ecldata1.ecl", false);
    assert(closedArchivePath.status == th07::Th07FileSystemOpenStatus::ArchiveNotOpen);

    const char* archivePath = "data/ecldata1.ecl";
    assert(std::strcmp(th07::ComputeTh07ArchiveLookupName(archivePath), "ecldata1.ecl") == 0);

    const char* plainPath = "score.dat";
    assert(th07::ComputeTh07ArchiveLookupName(plainPath) == plainPath);

    const char* backslashOnly = "data\\ecldata1.ecl";
    assert(th07::ComputeTh07ArchiveLookupName(backslashOnly) == backslashOnly);

    const char* mixedPath = "data\\stage/ecldata2.ecl";
    assert(std::strcmp(th07::ComputeTh07ArchiveLookupName(mixedPath), "ecldata2.ecl") == 0);

    const char* forwardThenBackslash = "data/stage\\ecldata3.ecl";
    assert(th07::ComputeTh07ArchiveLookupName(forwardThenBackslash) == forwardThenBackslash);

    return 0;
}
