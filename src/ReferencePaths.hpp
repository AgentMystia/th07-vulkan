#pragma once

#include <filesystem>
#include <span>
#include <string_view>

namespace th07 {

struct ReferenceFile {
    std::string_view name;
    std::filesystem::path relativePath;
};

std::filesystem::path FindProjectRoot(std::filesystem::path start = std::filesystem::current_path());
std::filesystem::path ReferenceRoot(std::filesystem::path projectRoot = FindProjectRoot());
std::filesystem::path OriginalReferenceRoot(std::filesystem::path projectRoot = FindProjectRoot());
std::filesystem::path ExtractedReferenceRoot(std::filesystem::path projectRoot = FindProjectRoot());

std::span<const ReferenceFile> RequiredOriginalFiles();
bool HasRequiredOriginalFiles(std::filesystem::path projectRoot = FindProjectRoot());

} // namespace th07
