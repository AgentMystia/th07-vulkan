#include "ReferencePaths.hpp"

#include <array>

namespace th07 {
namespace {

const std::array<ReferenceFile, 3> kRequiredOriginalFiles = {{
    {"TH07 executable", "th07.exe"},
    {"TH07 archive", "th07.dat"},
    {"TH07 BGM archive", "thbgm.dat"},
}};

bool IsProjectRoot(const std::filesystem::path &path)
{
    return std::filesystem::exists(path / "AGENTS.md") &&
           std::filesystem::exists(path / "CMakeLists.txt") &&
           std::filesystem::exists(path / "src");
}

} // namespace

std::filesystem::path FindProjectRoot(std::filesystem::path start)
{
    start = std::filesystem::absolute(start);
    if (std::filesystem::is_regular_file(start)) {
        start = start.parent_path();
    }

    for (std::filesystem::path current = start; !current.empty(); current = current.parent_path()) {
        if (IsProjectRoot(current)) {
            return current;
        }
        if (current == current.root_path()) {
            break;
        }
    }

    return std::filesystem::current_path();
}

std::filesystem::path ReferenceRoot(std::filesystem::path projectRoot)
{
    return projectRoot / "reference";
}

std::filesystem::path OriginalReferenceRoot(std::filesystem::path projectRoot)
{
    return ReferenceRoot(projectRoot) / "original";
}

std::filesystem::path ExtractedReferenceRoot(std::filesystem::path projectRoot)
{
    return ReferenceRoot(projectRoot) / "extracted";
}

std::span<const ReferenceFile> RequiredOriginalFiles()
{
    return kRequiredOriginalFiles;
}

bool HasRequiredOriginalFiles(std::filesystem::path projectRoot)
{
    const auto originalRoot = OriginalReferenceRoot(projectRoot);
    for (const ReferenceFile &file : kRequiredOriginalFiles) {
        if (!std::filesystem::exists(originalRoot / file.relativePath)) {
            return false;
        }
    }
    return true;
}

} // namespace th07
