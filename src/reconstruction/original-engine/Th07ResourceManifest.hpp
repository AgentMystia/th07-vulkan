#pragma once

#include <array>

namespace th07
{
struct ResourceStringAnchor
{
    const char *archivePath;
    const char *ghidraStringAddress;
};

extern const std::array<ResourceStringAnchor, 8> kTh07EclFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07StdFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07MsgFiles;

constexpr const char *kTh07VersionString = "` Perfect Cherry Blossom. ver 1.00b";
constexpr const char *kTh07VersionStringAddress = "00497ba9";
constexpr const char *kTh07BgmFmtPath = "bgm/thbgm.fmt";
constexpr const char *kTh07BgmDatPath = "./thbgm.dat";
} // namespace th07
