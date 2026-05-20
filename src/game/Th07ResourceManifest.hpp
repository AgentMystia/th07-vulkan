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
    ShotType shotType;
    bool focused;
    ResourceStringAnchor file;
};

extern const std::array<ResourceStringAnchor, 8> kTh07EclFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07StdFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07MsgFiles;
extern const std::array<StageResourceSet, 8> kTh07StageResources;
extern const std::array<ShtFileBinding, 12> kTh07ShtFiles;

const StageResourceSet* FindStageResources(int stageNumber);
const ShtFileBinding* FindShtFile(ShotType shotType, bool focused);

} // namespace th07
