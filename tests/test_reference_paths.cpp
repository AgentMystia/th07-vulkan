#include "game/ReferencePaths.hpp"
#include "game/Th07Constants.hpp"
#include "game/Th07ResourceManifest.hpp"

#include <cassert>
#include <filesystem>
#include <string>

int main()
{
    const auto required = th07::RequiredOriginalFiles();
    assert(required.size() == 3);
    assert(required[0].relativePath == "th07.exe");
    assert(required[1].relativePath == "th07.dat");
    assert(required[2].relativePath == "thbgm.dat");

    const auto projectRoot = th07::FindProjectRoot();
    assert(std::filesystem::exists(projectRoot / "AGENTS.md"));
    assert(th07::ReferenceRoot(projectRoot).filename() == "reference");
    assert(th07::OriginalReferenceRoot(projectRoot).filename() == "original");
    assert(th07::ExtractedReferenceRoot(projectRoot).filename() == "extracted");

    assert(th07::kStageEclFiles.size() == 8);
    assert(th07::kStageStdFiles.size() == 8);
    assert(th07::kShotFiles.size() == 12);
    assert(th07::kTh07StageResources.size() == 8);
    assert(th07::kTh07ShtFiles.size() == 12);
    assert(th07::kTh07EclFiles[0].archivePath == "data/ecldata1.ecl");
    assert(th07::kTh07EclFiles[7].archivePath == "data/ecldata8.ecl");
    assert(th07::kTh07StdFiles[7].ghidraStringAddress == "00498994");
    assert(th07::kTh07MsgFiles[7].ghidraStringAddress == "00498140");
    assert(th07::FindStageResources(4)->backgroundAnm == "data/stg4bg.anm");
    assert(th07::FindStageResources(8)->ecl.archivePath == "data/ecldata8.ecl");
    assert(th07::FindStageResources(9) == nullptr);
    assert(th07::FindShtFile(th07::ShotType::SakuyaB, false)->file.archivePath == "data/ply02b.sht");
    assert(th07::FindShtFile(th07::ShotType::SakuyaB, true)->file.archivePath == "data/ply02bs.sht");
    assert(th07::IsPointOfCollectionAvailable(128, false));
    assert(th07::IsPointOfCollectionAvailable(0, true));
    assert(!th07::IsPointOfCollectionAvailable(127, false));

    return 0;
}
