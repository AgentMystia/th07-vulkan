#pragma once

#include "Th07ArchiveReader.hpp"
#include "inttypes.hpp"

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace th07
{
enum class Th07FileSystemOpenStatus
{
    Success,
    ArchiveNotOpen,
    ArchiveOpenFailed,
    ArchiveVersionProbeMissing,
    EntryNotFound,
    ExternalOpenFailed,
};

struct Th07FileSystemOpenResult
{
    Th07FileSystemOpenStatus status = Th07FileSystemOpenStatus::EntryNotFound;
    std::vector<u8> data;
    std::string lookupName;
    u32 lastFileSize = 0;
    bool externalResource = false;
};

class Th07FileSystemRuntime
{
  public:
    bool OpenArchive(const std::filesystem::path &archivePath);
    bool OpenDefaultArchive();
    void CloseArchive();

    bool IsArchiveOpen() const { return archiveOpen; }
    const std::filesystem::path &ArchivePath() const { return archivePath; }
    const std::vector<u8> &VersionProbeData() const { return versionProbeData; }
    u32 VersionProbeSize() const { return versionProbeSize; }

    Th07FileSystemOpenResult OpenPath(std::string_view filepath, bool isExternalResource) const;

  private:
    Th07ArchiveReader archive;
    std::filesystem::path archivePath;
    std::vector<u8> versionProbeData;
    u32 versionProbeSize = 0;
    bool archiveOpen = false;
};
} // namespace th07
