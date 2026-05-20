#include "Th07Constants.hpp"

namespace th07 {

const PlayerSystemConstants kPlayerSystem = {
    {1.25f, 1.25f},
    {12.0f, 12.0f},
    20.0f,
    128,
};

const CherrySystemConstants kCherrySystem = {
    500000,
    50000,
    540,
    100000,
};

const std::array<const char*, 8> kStageEclFiles = {
    "ecldata1.ecl",
    "ecldata2.ecl",
    "ecldata3.ecl",
    "ecldata4.ecl",
    "ecldata5.ecl",
    "ecldata6.ecl",
    "ecldata7.ecl",
    "ecldata8.ecl",
};

const std::array<const char*, 8> kStageStdFiles = {
    "stage1.std",
    "stage2.std",
    "stage3.std",
    "stage4.std",
    "stage5.std",
    "stage6.std",
    "stage7.std",
    "stage8.std",
};

const std::array<const char*, 12> kShotFiles = {
    "ply00a.sht",
    "ply00b.sht",
    "ply01a.sht",
    "ply01b.sht",
    "ply02a.sht",
    "ply02b.sht",
    "ply00as.sht",
    "ply00bs.sht",
    "ply01as.sht",
    "ply01bs.sht",
    "ply02as.sht",
    "ply02bs.sht",
};

bool IsPointOfCollectionAvailable(int power, bool borderActive) {
    return borderActive || power >= kPlayerSystem.maxPower;
}

} // namespace th07
