#include "Th07FileSystemRuntime.hpp"

#include "ReferencePaths.hpp"
#include "Th07FileSystemTables.hpp"

#include <fstream>
#include <iterator>

namespace th07
{
namespace {

bool ReadExternalFile(const std::filesystem::path &path, std::vector<u8> *data)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    data->assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return true;
}

std::string ToNullTerminatedString(std::string_view value)
{
    return std::string(value.data(), value.size());
}

} // namespace

bool Th07FileSystemRuntime::OpenArchive(const std::filesystem::path &path)
{
    CloseArchive();
    archivePath = path;

    if (!archive.LoadFromFile(path)) {
        return false;
    }

    archiveOpen = true;
    versionProbeData.clear();
    if (!archive.ReadEntry(kTh07ArchiveVersionProbePath, &versionProbeData)) {
        versionProbeSize = 0;
        return false;
    }

    versionProbeSize = static_cast<u32>(versionProbeData.size());
    return true;
}

bool Th07FileSystemRuntime::OpenDefaultArchive()
{
    return OpenArchive(OriginalReferenceRoot() / kTh07ArchivePath);
}

void Th07FileSystemRuntime::CloseArchive()
{
    archive = Th07ArchiveReader{};
    archivePath.clear();
    versionProbeData.clear();
    versionProbeSize = 0;
    archiveOpen = false;
}

Th07FileSystemOpenResult Th07FileSystemRuntime::OpenPath(std::string_view filepath,
                                                          bool isExternalResource) const
{
    Th07FileSystemOpenResult result;
    result.externalResource = isExternalResource;
    const std::string path = ToNullTerminatedString(filepath);

    if (isExternalResource) {
        result.lookupName = path;
        if (!ReadExternalFile(path, &result.data)) {
            result.status = Th07FileSystemOpenStatus::ExternalOpenFailed;
            return result;
        }
        result.lastFileSize = static_cast<u32>(result.data.size());
        result.status = Th07FileSystemOpenStatus::Success;
        return result;
    }

    if (!archiveOpen) {
        result.lookupName = path;
        result.status = Th07FileSystemOpenStatus::ArchiveNotOpen;
        return result;
    }

    result.lookupName = ComputeTh07ArchiveLookupName(path.c_str());
    if (!archive.ReadEntry(result.lookupName, &result.data)) {
        result.status = Th07FileSystemOpenStatus::EntryNotFound;
        return result;
    }

    result.lastFileSize = static_cast<u32>(result.data.size());
    result.status = Th07FileSystemOpenStatus::Success;
    return result;
}
} // namespace th07
