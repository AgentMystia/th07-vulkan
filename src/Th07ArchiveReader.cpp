#include "Th07ArchiveReader.hpp"

#include "Th07FileSystemTables.hpp"

#include <array>
#include <cctype>
#include <fstream>
#include <iterator>

namespace th07
{
namespace {

constexpr std::size_t kTh07ArchiveLzssDictionarySize = 0x2000;
constexpr std::size_t kTh07ArchiveLzssDictionaryMask = 0x1fff;

u32 ReadLeU32(const std::vector<u8> &bytes, std::size_t offset)
{
    return static_cast<u32>(bytes[offset]) |
           (static_cast<u32>(bytes[offset + 1]) << 8) |
           (static_cast<u32>(bytes[offset + 2]) << 16) |
           (static_cast<u32>(bytes[offset + 3]) << 24);
}

bool EqualsIgnoreAsciiCase(std::string_view lhs, std::string_view rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t i = 0; i < lhs.size(); i += 1) {
        const auto left = static_cast<unsigned char>(lhs[i]);
        const auto right = static_cast<unsigned char>(rhs[i]);
        if (std::tolower(left) != std::tolower(right)) {
            return false;
        }
    }
    return true;
}

} // namespace

bool DecodeTh07ArchiveLzss(const u8 *packedData, std::size_t packedSize, std::size_t decodedSize,
                            std::vector<u8> *decoded)
{
    if (decoded == nullptr || (packedData == nullptr && packedSize != 0)) {
        return false;
    }

    decoded->assign(decodedSize, 0);
    if (decodedSize == 0) {
        return true;
    }

    u8 currentByte = 0;
    u8 inputMask = 0x80;
    std::size_t inputCursor = 0;
    std::size_t outputCursor = 0;
    std::size_t dictionaryHead = 1;
    std::array<u8, kTh07ArchiveLzssDictionarySize> dictionary{};

    const auto fetchByteIfNeeded = [&]() {
        if (inputMask == 0x80) {
            if (inputCursor < packedSize) {
                currentByte = packedData[inputCursor];
                inputCursor += 1;
            } else {
                currentByte = 0;
            }
        }
    };
    const auto advanceMask = [&]() {
        inputMask >>= 1;
        if (inputMask == 0) {
            inputMask = 0x80;
        }
    };
    const auto readBits = [&](u32 firstMask, u32 *value) {
        *value = 0;
        for (u32 outputMask = firstMask; outputMask != 0; outputMask >>= 1) {
            fetchByteIfNeeded();
            if ((currentByte & inputMask) != 0) {
                *value |= outputMask;
            }
            advanceMask();
        }
    };
    const auto writeByte = [&](u8 value) {
        if (outputCursor >= decoded->size()) {
            return false;
        }
        (*decoded)[outputCursor] = value;
        outputCursor += 1;
        dictionary[dictionaryHead] = value;
        dictionaryHead = (dictionaryHead + 1) & kTh07ArchiveLzssDictionaryMask;
        return true;
    };

    for (;;) {
        fetchByteIfNeeded();
        const bool isLiteral = (currentByte & inputMask) != 0;
        advanceMask();

        if (isLiteral) {
            u32 literal = 0;
            readBits(0x80, &literal);
            if (!writeByte(static_cast<u8>(literal))) {
                return false;
            }
            continue;
        }

        u32 matchOffset = 0;
        readBits(0x1000, &matchOffset);
        if (matchOffset == 0) {
            break;
        }

        u32 matchLengthBits = 0;
        readBits(0x08, &matchLengthBits);
        for (u32 i = 0; i <= matchLengthBits + 2; i += 1) {
            const u8 value = dictionary[(matchOffset + i) & kTh07ArchiveLzssDictionaryMask];
            if (!writeByte(value)) {
                return false;
            }
        }
    }

    while (inputMask != 0x80) {
        fetchByteIfNeeded();
        advanceMask();
    }

    return outputCursor == decodedSize;
}

bool Th07ArchiveReader::LoadFromFile(const std::filesystem::path &path)
{
    entries.clear();
    archiveBytes.clear();
    packedIndexOffset = 0;
    decodedIndexSize = 0;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    archiveBytes.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    if (archiveBytes.size() < kTh07ArchiveHeaderSize) {
        return false;
    }

    if (ReadLeU32(archiveBytes, kTh07ArchiveHeaderMagicOffset) != kTh07ArchiveMagicPbg4) {
        return false;
    }
    const u32 entryCount = ReadLeU32(archiveBytes, kTh07ArchiveHeaderEntryCountOffset);
    packedIndexOffset = ReadLeU32(archiveBytes, kTh07ArchiveHeaderPackedIndexOffsetOffset);
    decodedIndexSize = ReadLeU32(archiveBytes, kTh07ArchiveHeaderDecodedIndexSizeOffset);
    if (packedIndexOffset > archiveBytes.size()) {
        return false;
    }

    std::vector<u8> decodedIndex;
    if (!DecodeTh07ArchiveLzss(archiveBytes.data() + packedIndexOffset,
                               archiveBytes.size() - packedIndexOffset,
                               decodedIndexSize, &decodedIndex)) {
        return false;
    }

    std::size_t cursor = 0;
    entries.reserve(entryCount);
    for (u32 i = 0; i < entryCount; i += 1) {
        const std::size_t nameStart = cursor;
        while (cursor < decodedIndex.size() && decodedIndex[cursor] != 0) {
            cursor += 1;
        }
        if (cursor >= decodedIndex.size()) {
            entries.clear();
            return false;
        }

        Th07ArchiveReaderEntry entry;
        entry.name.assign(reinterpret_cast<const char *>(decodedIndex.data() + nameStart),
                          cursor - nameStart);
        cursor += 1;
        if (cursor + 12 > decodedIndex.size()) {
            entries.clear();
            return false;
        }
        entry.packedOffset = ReadLeU32(decodedIndex, cursor);
        entry.decodedSize = ReadLeU32(decodedIndex, cursor + 4);
        entry.metadata = ReadLeU32(decodedIndex, cursor + 8);
        cursor += 12;

        if (entry.packedOffset < kTh07ArchiveHeaderSize || entry.packedOffset > packedIndexOffset) {
            entries.clear();
            return false;
        }
        if (!entries.empty() && entry.packedOffset < entries.back().packedOffset) {
            entries.clear();
            return false;
        }
        entries.push_back(entry);
    }

    return true;
}

const Th07ArchiveReaderEntry *Th07ArchiveReader::FindEntry(std::string_view name) const
{
    for (const Th07ArchiveReaderEntry &entry : entries) {
        if (EqualsIgnoreAsciiCase(entry.name, name)) {
            return &entry;
        }
    }
    return nullptr;
}

bool Th07ArchiveReader::ReadEntry(std::string_view name, std::vector<u8> *decoded) const
{
    if (decoded == nullptr) {
        return false;
    }
    for (std::size_t i = 0; i < entries.size(); i += 1) {
        const Th07ArchiveReaderEntry &entry = entries[i];
        if (!EqualsIgnoreAsciiCase(entry.name, name)) {
            continue;
        }

        const u32 nextPackedOffset = i + 1 < entries.size() ? entries[i + 1].packedOffset : packedIndexOffset;
        if (nextPackedOffset < entry.packedOffset || nextPackedOffset > archiveBytes.size()) {
            return false;
        }
        return DecodeTh07ArchiveLzss(archiveBytes.data() + entry.packedOffset,
                                     nextPackedOffset - entry.packedOffset,
                                     entry.decodedSize, decoded);
    }
    return false;
}
} // namespace th07
