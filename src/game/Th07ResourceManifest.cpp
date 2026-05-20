#include "Th07ResourceManifest.hpp"

namespace th07 {

const std::array<ResourceStringAnchor, 8> kTh07EclFiles = {{
    {"data/ecldata1.ecl", "00497f38"},
    {"data/ecldata2.ecl", "00497f24"},
    {"data/ecldata3.ecl", "00497f10"},
    {"data/ecldata4.ecl", "00497efc"},
    {"data/ecldata5.ecl", "00497ee8"},
    {"data/ecldata6.ecl", "00497ed4"},
    {"data/ecldata7.ecl", "00497ec0"},
    {"data/ecldata8.ecl", "00497eac"},
}};

const std::array<ResourceStringAnchor, 8> kTh07StdFiles = {{
    {"data/stage1.std", "00498a04"},
    {"data/stage2.std", "004989f4"},
    {"data/stage3.std", "004989e4"},
    {"data/stage4.std", "004989d4"},
    {"data/stage5.std", "004989c4"},
    {"data/stage6.std", "004989b4"},
    {"data/stage7.std", "004989a4"},
    {"data/stage8.std", "00498994"},
}};

const std::array<ResourceStringAnchor, 8> kTh07MsgFiles = {{
    {"data/msg1.dat", "004982c8"},
    {"data/msg2.dat", "00498290"},
    {"data/msg3.dat", "00498258"},
    {"data/msg4.dat", "00498220"},
    {"data/msg5.dat", "004981e8"},
    {"data/msg6.dat", "004981b0"},
    {"data/msg7.dat", "00498178"},
    {"data/msg8.dat", "00498140"},
}};

const std::array<StageResourceSet, 8> kTh07StageResources = {{
    {1, kTh07EclFiles[0], kTh07StdFiles[0], kTh07MsgFiles[0], "data/stg1enm.anm", "data/stg1bg.anm"},
    {2, kTh07EclFiles[1], kTh07StdFiles[1], kTh07MsgFiles[1], "data/stg2enm.anm", "data/stg2bg.anm"},
    {3, kTh07EclFiles[2], kTh07StdFiles[2], kTh07MsgFiles[2], "data/stg3enm.anm", "data/stg3bg.anm"},
    {4, kTh07EclFiles[3], kTh07StdFiles[3], kTh07MsgFiles[3], "data/stg4enm.anm", "data/stg4bg.anm"},
    {5, kTh07EclFiles[4], kTh07StdFiles[4], kTh07MsgFiles[4], "data/stg5enm.anm", "data/stg5bg.anm"},
    {6, kTh07EclFiles[5], kTh07StdFiles[5], kTh07MsgFiles[5], "data/stg6enm.anm", "data/stg6bg.anm"},
    {7, kTh07EclFiles[6], kTh07StdFiles[6], kTh07MsgFiles[6], "data/stg7enm.anm", "data/stg7bg.anm"},
    {8, kTh07EclFiles[7], kTh07StdFiles[7], kTh07MsgFiles[7], "data/stg8enm.anm", "data/stg8bg.anm"},
}};

const std::array<ShtFileBinding, 12> kTh07ShtFiles = {{
    {ShotType::ReimuA, false, {"data/ply00a.sht", "00496bb4"}},
    {ShotType::ReimuB, false, {"data/ply00b.sht", "00496ba4"}},
    {ShotType::MarisaA, false, {"data/ply01a.sht", "00496b94"}},
    {ShotType::MarisaB, false, {"data/ply01b.sht", "00496b84"}},
    {ShotType::SakuyaA, false, {"data/ply02a.sht", "00496b74"}},
    {ShotType::SakuyaB, false, {"data/ply02b.sht", "00496b64"}},
    {ShotType::ReimuA, true, {"data/ply00as.sht", "00496b50"}},
    {ShotType::ReimuB, true, {"data/ply00bs.sht", "00496b3c"}},
    {ShotType::MarisaA, true, {"data/ply01as.sht", "00496b28"}},
    {ShotType::MarisaB, true, {"data/ply01bs.sht", "00496b14"}},
    {ShotType::SakuyaA, true, {"data/ply02as.sht", "00496b00"}},
    {ShotType::SakuyaB, true, {"data/ply02bs.sht", "00496aec"}},
}};

const StageResourceSet* FindStageResources(int stageNumber)
{
    for (const StageResourceSet& resources : kTh07StageResources) {
        if (resources.stageNumber == stageNumber) {
            return &resources;
        }
    }
    return nullptr;
}

const ShtFileBinding* FindShtFile(ShotType shotType, bool focused)
{
    for (const ShtFileBinding& binding : kTh07ShtFiles) {
        if (binding.shotType == shotType && binding.focused == focused) {
            return &binding;
        }
    }
    return nullptr;
}

} // namespace th07
