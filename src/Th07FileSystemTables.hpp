#pragma once

#include "inttypes.hpp"

namespace th07
{
inline constexpr u32 kTh07OpenPathFunctionAddress = 0x00431330;
inline constexpr u32 kTh07ArchiveLookupObjectAddress = 0x00626258;
inline constexpr u32 kTh07ArchiveOwnerOpenFunctionAddress = 0x00438668;
inline constexpr u32 kTh07ArchiveOwnerOpenPathPushAddress = 0x00438671;
inline constexpr u32 kTh07ArchiveOwnerOpenThisMovAddress = 0x00438676;
inline constexpr u32 kTh07ArchiveOwnerOpenCallAddress = 0x0043867b;
inline constexpr u32 kTh07ArchiveOwnerCloseHostFunctionAddress = 0x00438de2;
inline constexpr u32 kTh07ArchiveOwnerCloseThisMovAddress = 0x00438f8b;
inline constexpr u32 kTh07ArchiveOwnerCloseCallAddress = 0x00438f90;
inline constexpr u32 kTh07OpenPathLastFileSizeGlobalAddress = 0x004b9e64;
inline constexpr u32 kTh07ArchiveOpenFunctionAddress = 0x0045f710;
inline constexpr u32 kTh07ArchiveCloseFunctionAddress = 0x0045f800;
inline constexpr u32 kTh07ArchiveMagicPbg4 = 0x34474250;
inline constexpr u32 kTh07ArchiveHeaderMagicOffset = 0x00;
inline constexpr u32 kTh07ArchiveHeaderEntryCountOffset = 0x04;
inline constexpr u32 kTh07ArchiveHeaderPackedIndexOffsetOffset = 0x08;
inline constexpr u32 kTh07ArchiveHeaderDecodedIndexSizeOffset = 0x0c;
inline constexpr u32 kTh07ArchiveHeaderSize = 0x10;
inline constexpr u32 kTh07ArchiveOwnerEntryTableOffset = 0x00;
inline constexpr u32 kTh07ArchiveOwnerEntryCountOffset = 0x04;
inline constexpr u32 kTh07ArchiveOwnerPathCopyOffset = 0x08;
inline constexpr u32 kTh07ArchiveOwnerStreamOffset = 0x0c;
inline constexpr u32 kTh07ArchiveOwnerFieldSpan = 0x10;
inline constexpr u32 kTh07ArchiveEntryStride = 0x10;
inline constexpr u32 kTh07ArchiveEntryNamePointerOffset = 0x00;
inline constexpr u32 kTh07ArchiveEntryPackedOffsetOffset = 0x04;
inline constexpr u32 kTh07ArchiveEntryDecodedSizeOffset = 0x08;
inline constexpr u32 kTh07ArchiveEntryMetadataOffset = 0x0c;
inline constexpr u32 kTh07ArchiveLoadIndexFunctionAddress = 0x0045fb50;
inline constexpr u32 kTh07ArchiveBuildEntryTableFunctionAddress = 0x0045fde0;
inline constexpr u32 kTh07ArchiveCopyStringFunctionAddress = 0x0045ffc0;
inline constexpr u32 kTh07ArchiveFindEntryFunctionAddress = 0x0045fae0;
inline constexpr u32 kTh07ArchiveEntrySizeFunctionAddress = 0x0045fab0;
inline constexpr u32 kTh07ArchiveReadEntryFunctionAddress = 0x0045f960;
inline constexpr u32 kTh07ArchiveMissLogBufferAddress = 0x00624210;
inline constexpr u32 kTh07ArchivePathStringAddress = 0x00497150;
inline constexpr u32 kTh07ArchiveVersionProbeFormatStringAddress = 0x00497140;
inline constexpr u32 kTh07ArchiveVersionProbeCode = 0x0100;
inline constexpr char kTh07ArchiveVersionProbeSuffix = 'b';
inline constexpr char kTh07ArchivePath[] = "th07.dat";
inline constexpr char kTh07ArchiveVersionProbeFormat[] = "th07_%.4x%c.ver";
inline constexpr char kTh07ArchiveVersionProbePath[] = "th07_0100b.ver";
inline constexpr u32 kTh07ArchiveOpenLogStringAddress = 0x00495100;
inline constexpr u32 kTh07ArchiveCloseLogStringAddress = 0x004950cc;
inline constexpr u32 kTh07ArchiveOpenMissingLogStringAddress = 0x004950e8;
inline constexpr u32 kTh07ArchiveOpenErrorLogStringAddress = 0x00495084;
inline constexpr u32 kTh07ArchiveMissStringAddress = 0x00497d38;
inline constexpr u32 kTh07ArchiveDecodeStringAddress = 0x00497d24;
inline constexpr u32 kTh07ExternalLoadStringAddress = 0x00497d14;
inline constexpr u32 kTh07ExternalMissingStringAddress = 0x00497cf8;

inline const char* ComputeTh07ArchiveLookupName(const char* filepath)
{
    const char* lookupName = filepath;
    for (const char* cursor = filepath; *cursor != '\0'; cursor += 1) {
        if (*cursor == '\\') {
            lookupName = cursor + 1;
        }
    }

    const char* slash = nullptr;
    for (const char* cursor = lookupName; *cursor != '\0'; cursor += 1) {
        if (*cursor == '/') {
            slash = cursor;
        }
    }

    if (slash == nullptr) {
        return filepath;
    }
    return slash + 1;
}
} // namespace th07
