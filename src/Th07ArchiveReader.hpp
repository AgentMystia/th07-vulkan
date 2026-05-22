#pragma once

#include "inttypes.hpp"

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace th07
{
struct Th07ArchiveReaderEntry
{
    std::string name;
    u32 packedOffset;
    u32 decodedSize;
    u32 metadata;
};

bool DecodeTh07ArchiveLzss(const u8 *packedData, std::size_t packedSize, std::size_t decodedSize,
                            std::vector<u8> *decoded);

class Th07ArchiveReader
{
  public:
    bool LoadFromFile(const std::filesystem::path &path);
    bool ReadEntry(std::string_view name, std::vector<u8> *decoded) const;

    const Th07ArchiveReaderEntry *FindEntry(std::string_view name) const;
    const std::vector<Th07ArchiveReaderEntry> &Entries() const { return entries; }
    std::size_t EntryCount() const { return entries.size(); }
    u32 PackedIndexOffset() const { return packedIndexOffset; }
    u32 DecodedIndexSize() const { return decodedIndexSize; }

  private:
    std::vector<u8> archiveBytes;
    std::vector<Th07ArchiveReaderEntry> entries;
    u32 packedIndexOffset = 0;
    u32 decodedIndexSize = 0;
};
} // namespace th07
