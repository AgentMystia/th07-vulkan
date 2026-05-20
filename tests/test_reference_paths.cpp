#include "game/ReferencePaths.hpp"
#include "game/Th07Constants.hpp"

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
    assert(th07::IsPointOfCollectionAvailable(128, false));
    assert(th07::IsPointOfCollectionAvailable(0, true));
    assert(!th07::IsPointOfCollectionAvailable(127, false));

    return 0;
}
