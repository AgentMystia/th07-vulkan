#pragma once

#include "Th07Constants.hpp"

#include <array>
#include <string_view>

namespace th07 {

struct ResourceStringAnchor {
    std::string_view archivePath;
    std::string_view ghidraStringAddress;
};

struct StageResourceSet {
    int stageNumber;
    ResourceStringAnchor ecl;
    ResourceStringAnchor std;
    ResourceStringAnchor msg;
    std::string_view enemyAnm;
    std::string_view backgroundAnm;
};

struct ShtFileBinding {
    Th07ShotType shotType;
    bool focused;
    ResourceStringAnchor file;
};

extern const std::array<ResourceStringAnchor, 8> kTh07EclFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07StdFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07MsgFiles;
extern const std::array<StageResourceSet, 8> kTh07StageResources;
extern const std::array<ShtFileBinding, 12> kTh07ShtFiles;

constexpr std::string_view kTh07VersionString = "` Perfect Cherry Blossom. ver 1.00b";
constexpr std::string_view kTh07VersionStringAddress = "00497ba9";
constexpr std::string_view kTh07BgmFmtPath = "bgm/thbgm.fmt";
constexpr std::string_view kTh07BgmDatPath = "./thbgm.dat";

const StageResourceSet* FindStageResources(int stageNumber);
const ShtFileBinding* FindShtFile(Th07ShotType shotType, bool focused);

} // namespace th07
