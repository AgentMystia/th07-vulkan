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
    {Th07ShotType::ReimuA, false, {"data/ply00a.sht", "00496bb4"}},
    {Th07ShotType::ReimuB, false, {"data/ply00b.sht", "00496ba4"}},
    {Th07ShotType::MarisaA, false, {"data/ply01a.sht", "00496b94"}},
    {Th07ShotType::MarisaB, false, {"data/ply01b.sht", "00496b84"}},
    {Th07ShotType::SakuyaA, false, {"data/ply02a.sht", "00496b74"}},
    {Th07ShotType::SakuyaB, false, {"data/ply02b.sht", "00496b64"}},
    {Th07ShotType::ReimuA, true, {"data/ply00as.sht", "00496b50"}},
    {Th07ShotType::ReimuB, true, {"data/ply00bs.sht", "00496b3c"}},
    {Th07ShotType::MarisaA, true, {"data/ply01as.sht", "00496b28"}},
    {Th07ShotType::MarisaB, true, {"data/ply01bs.sht", "00496b14"}},
    {Th07ShotType::SakuyaA, true, {"data/ply02as.sht", "00496b00"}},
    {Th07ShotType::SakuyaB, true, {"data/ply02bs.sht", "00496aec"}},
}};

const ResourceStringAnchor kTh07EnemyBulletAnmFile = {"data/etama.anm", "00498524"};

const std::array<ResourceStringAnchor, kTh07RuntimeAnmFileCount> kTh07RuntimeAnmFiles = {{
    {"data/title01.anm", "004954a8"},
    {"data/result00.anm", "00496380"},
    {"data/player02.anm", "00496ab0"},
    {"data/player01.anm", "00496ac4"},
    {"data/player00.anm", "00496ad8"},
    {"data/music00.anm", "00496bd8"},
    {"data/text.anm", "00497018"},
    {"data/stg8enm.anm", "00497f70"},
    {"data/stg7enm.anm", "00497f84"},
    {"data/stg6enm.anm", "00497f98"},
    {"data/stg5enm.anm", "00497fac"},
    {"data/stg4enm.anm", "00497fc0"},
    {"data/stg3enm.anm", "00497fd4"},
    {"data/stg2enm.anm", "00497fe8"},
    {"data/stg1enm.anm", "00497ffc"},
    {"data/std8txt.anm", "00498150"},
    {"data/face_08_00.anm", "00498164"},
    {"data/std7txt.anm", "00498188"},
    {"data/face_07_00.anm", "0049819c"},
    {"data/std6txt.anm", "004981c0"},
    {"data/face_06_00.anm", "004981d4"},
    {"data/std5txt.anm", "004981f8"},
    {"data/face_05_00.anm", "0049820c"},
    {"data/std4txt.anm", "00498230"},
    {"data/face_04_00.anm", "00498244"},
    {"data/std3txt.anm", "00498268"},
    {"data/face_03_00.anm", "0049827c"},
    {"data/std2txt.anm", "004982a0"},
    {"data/face_02_00.anm", "004982b4"},
    {"data/std1txt.anm", "004982d8"},
    {"data/face_01_00.anm", "004982ec"},
    {"data/loading3.anm", "00498300"},
    {"data/face_sk00.anm", "00498314"},
    {"data/loading2.anm", "00498328"},
    {"data/face_mr00.anm", "0049833c"},
    {"data/loading.anm", "00498350"},
    {"data/face_rm00.anm", "00498364"},
    {"data/front.anm", "00498378"},
    {"data/etama.anm", "00498524"},
    {"data/staff01.anm", "004985c4"},
    {"data/eff08.anm", "00498624"},
    {"data/eff07.anm", "00498634"},
    {"data/eff06.anm", "00498644"},
    {"data/eff05.anm", "00498654"},
    {"data/eff04b.anm", "00498664"},
    {"data/eff04.anm", "00498674"},
    {"data/eff03.anm", "00498684"},
    {"data/eff02.anm", "00498694"},
    {"data/eff01.anm", "004986a4"},
    {"data/stg8bg.anm", "004988c4"},
    {"data/stg7bg.anm", "004988d4"},
    {"data/stg6bg.anm", "004988e4"},
    {"data/stg5bg.anm", "004988f4"},
    {"data/stg4bg5.anm", "00498904"},
    {"data/stg4bg4.anm", "00498918"},
    {"data/stg4bg3.anm", "0049892c"},
    {"data/stg4bg2.anm", "00498940"},
    {"data/stg4bg.anm", "00498954"},
    {"data/stg3bg.anm", "00498964"},
    {"data/stg2bg.anm", "00498974"},
    {"data/stg1bg.anm", "00498984"},
    {"data/capture.anm", "00498a14"},
    {"data/ascii.anm", "00498a28"},
}};

const std::array<RuntimeAnmLoadContract, kTh07RuntimeAnmLoadContractCount> kTh07RuntimeAnmLoadContracts = {{
    {{"data/ascii.anm", "00498a28"}, "00401d70", 0x01, 0x0000},
    {{"data/capture.anm", "00498a14"}, "00401d70", 0x04, 0x0724},
    {{"data/front.anm", "00498378"}, "00428b19", 0x15, 0x0600},
    {{"data/face_rm00.anm", "00498364"}, "00428b19", 0x19, 0x04a0},
    {{"data/loading.anm", "00498350"}, "00428b19", 0x17, 0x061e},
    {{"data/face_mr00.anm", "0049833c"}, "00428b19", 0x19, 0x04a0},
    {{"data/loading2.anm", "00498328"}, "00428b19", 0x17, 0x061e},
    {{"data/face_sk00.anm", "00498314"}, "00428b19", 0x19, 0x04a0},
    {{"data/loading3.anm", "00498300"}, "00428b19", 0x17, 0x061e},
    {{"data/face_01_00.anm", "004982ec"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std1txt.anm", "004982d8"}, "00428b19", 0x18, 0x0800},
    {{"data/face_02_00.anm", "004982b4"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std2txt.anm", "004982a0"}, "00428b19", 0x18, 0x0800},
    {{"data/face_03_00.anm", "0049827c"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std3txt.anm", "00498268"}, "00428b19", 0x18, 0x0800},
    {{"data/face_04_00.anm", "00498244"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std4txt.anm", "00498230"}, "00428b19", 0x18, 0x0800},
    {{"data/face_05_00.anm", "0049820c"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std5txt.anm", "004981f8"}, "00428b19", 0x18, 0x0800},
    {{"data/face_06_00.anm", "004981d4"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std6txt.anm", "004981c0"}, "00428b19", 0x18, 0x0800},
    {{"data/face_07_00.anm", "0049819c"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std7txt.anm", "00498188"}, "00428b19", 0x18, 0x0800},
    {{"data/face_08_00.anm", "00498164"}, "00428b19", 0x1c, 0x04ad},
    {{"data/std8txt.anm", "00498150"}, "00428b19", 0x18, 0x0800},
    {{"data/stg1bg.anm", "00498984"}, "00407000", 0x05, 0x0300},
    {{"data/stg2bg.anm", "00498974"}, "00407000", 0x05, 0x0300},
    {{"data/stg3bg.anm", "00498964"}, "00407000", 0x05, 0x0300},
    {{"data/stg4bg.anm", "00498954"}, "00407000", 0x05, 0x0300},
    {{"data/stg4bg2.anm", "00498940"}, "00407000", 0x06, 0x0310},
    {{"data/stg4bg3.anm", "0049892c"}, "00407000", 0x07, 0x0320},
    {{"data/stg4bg4.anm", "00498918"}, "00407000", 0x08, 0x0330},
    {{"data/stg4bg5.anm", "00498904"}, "00407000", 0x09, 0x0340},
    {{"data/stg5bg.anm", "004988f4"}, "00407000", 0x05, 0x0300},
    {{"data/stg6bg.anm", "004988e4"}, "00407000", 0x05, 0x0300},
    {{"data/stg7bg.anm", "004988d4"}, "00407000", 0x05, 0x0300},
    {{"data/stg8bg.anm", "004988c4"}, "00407000", 0x05, 0x0300},
    {{"data/etama.anm", "00498524"}, "00426f60", 0x0b, 0x0200},
    {{"data/player00.anm", "00496ad8"}, "004423e0", 0x0a, 0x0400},
    {{"data/player01.anm", "00496ac4"}, "004423e0", 0x0a, 0x0400},
    {{"data/player02.anm", "00496ab0"}, "004423e0", 0x0a, 0x0400},
    {{"data/staff01.anm", "004985c4"}, "0041e590", 0x31, 0x0600},
    {{"data/text.anm", "00497018"}, "00438986", 0x00, 0x0700},
    {{"data/music00.anm", "00496bd8"}, "0043ae20", 0x2e, 0x0900},
    {{"data/result00.anm", "00496380"}, "00449b05", 0x2a, 0x0900},
    {{"data/title01.anm", "004954a8"}, "0045bf15", 0x20, 0x0900},
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

const ShtFileBinding* FindShtFile(Th07ShotType shotType, bool focused)
{
    for (const ShtFileBinding& binding : kTh07ShtFiles) {
        if (binding.shotType == shotType && binding.focused == focused) {
            return &binding;
        }
    }
    return nullptr;
}

const ResourceStringAnchor* FindRuntimeAnmFile(std::string_view archivePath)
{
    for (const ResourceStringAnchor& file : kTh07RuntimeAnmFiles) {
        if (file.archivePath == archivePath) {
            return &file;
        }
    }
    return nullptr;
}

const RuntimeAnmLoadContract* FindRuntimeAnmLoadContract(std::string_view archivePath)
{
    for (const RuntimeAnmLoadContract& contract : kTh07RuntimeAnmLoadContracts) {
        if (contract.file.archivePath == archivePath) {
            return &contract;
        }
    }
    return nullptr;
}

} // namespace th07
