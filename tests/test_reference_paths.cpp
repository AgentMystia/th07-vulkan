#include "ReferencePaths.hpp"
#include "Th07BulletTables.hpp"
#include "Th07Constants.hpp"
#include "Th07ResourceManifest.hpp"

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
    assert(th07::kTh07RuntimeAnmFiles.size() == th07::kTh07RuntimeAnmFileCount);
    assert(th07::kTh07RuntimeAnmLoadContracts.size() == th07::kTh07RuntimeAnmLoadContractCount);
    assert(th07::kTh07EclFiles[0].archivePath == "data/ecldata1.ecl");
    assert(th07::kTh07EclFiles[7].archivePath == "data/ecldata8.ecl");
    assert(th07::kTh07StdFiles[7].ghidraStringAddress == "00498994");
    assert(th07::kTh07MsgFiles[7].ghidraStringAddress == "00498140");
    assert(th07::kTh07EnemyBulletAnmFile.archivePath == "data/etama.anm");
    assert(th07::kTh07EnemyBulletAnmFile.ghidraStringAddress == "00498524");
    assert(th07::FindRuntimeAnmFile("data/title01.anm")->ghidraStringAddress == "004954a8");
    assert(th07::FindRuntimeAnmFile("data/text.anm")->ghidraStringAddress == "00497018");
    assert(th07::FindRuntimeAnmFile("data/front.anm")->ghidraStringAddress == "00498378");
    assert(th07::FindRuntimeAnmFile("data/etama.anm")->ghidraStringAddress == "00498524");
    assert(th07::FindRuntimeAnmFile("data/stg4bg5.anm")->ghidraStringAddress == "00498904");
    assert(th07::FindRuntimeAnmFile("data/title01s.anm") == nullptr);
    const th07::RuntimeAnmLoadContract *frontLoad = th07::FindRuntimeAnmLoadContract("data/front.anm");
    assert(frontLoad != nullptr);
    assert(frontLoad->evidenceFunction == "00428b19");
    assert(frontLoad->anmFileSlot == 0x15);
    assert(frontLoad->spriteScriptOffset == 0x0600);
    const th07::RuntimeAnmLoadContract *stage4Bg3Load = th07::FindRuntimeAnmLoadContract("data/stg4bg3.anm");
    assert(stage4Bg3Load != nullptr);
    assert(stage4Bg3Load->evidenceFunction == "00407000");
    assert(stage4Bg3Load->anmFileSlot == 0x07);
    assert(stage4Bg3Load->spriteScriptOffset == 0x0320);
    const th07::RuntimeAnmLoadContract *player2Load = th07::FindRuntimeAnmLoadContract("data/player02.anm");
    assert(player2Load != nullptr);
    assert(player2Load->evidenceFunction == "004423e0");
    assert(player2Load->anmFileSlot == 0x0a);
    assert(player2Load->spriteScriptOffset == 0x0400);
    const th07::RuntimeAnmLoadContract *etamaLoad = th07::FindRuntimeAnmLoadContract("data/etama.anm");
    assert(etamaLoad != nullptr);
    assert(etamaLoad->evidenceFunction == "00426f60");
    assert(etamaLoad->anmFileSlot == 0x0b);
    assert(etamaLoad->spriteScriptOffset == 0x0200);
    const th07::RuntimeAnmLoadContract *asciiLoad = th07::FindRuntimeAnmLoadContract("data/ascii.anm");
    assert(asciiLoad != nullptr);
    assert(asciiLoad->evidenceFunction == "00401d70");
    assert(asciiLoad->anmFileSlot == 0x01);
    assert(asciiLoad->spriteScriptOffset == 0x0000);
    const th07::RuntimeAnmLoadContract *titleLoad = th07::FindRuntimeAnmLoadContract("data/title01.anm");
    assert(titleLoad != nullptr);
    assert(titleLoad->evidenceFunction == "0045bf15");
    assert(titleLoad->anmFileSlot == 0x20);
    assert(titleLoad->spriteScriptOffset == 0x0900);
    const th07::RuntimeAnmLoadContract *resultLoad = th07::FindRuntimeAnmLoadContract("data/result00.anm");
    assert(resultLoad != nullptr);
    assert(resultLoad->evidenceFunction == "00449b05");
    assert(resultLoad->anmFileSlot == 0x2a);
    assert(resultLoad->spriteScriptOffset == 0x0900);
    const th07::RuntimeAnmLoadContract *musicLoad = th07::FindRuntimeAnmLoadContract("data/music00.anm");
    assert(musicLoad != nullptr);
    assert(musicLoad->evidenceFunction == "0043ae20");
    assert(musicLoad->anmFileSlot == 0x2e);
    assert(musicLoad->spriteScriptOffset == 0x0900);
    assert(th07::FindRuntimeAnmLoadContract("data/title01s.anm") == nullptr);
    assert(th07::kTh07EnemyBulletAnmResource.archivePath == th07::kTh07EnemyBulletAnmFile.archivePath);
    assert(th07::kTh07EnemyBulletAnmResource.sprites == 168);
    assert(th07::kTh07EnemyBulletAnmResource.scripts == 25);
    assert(th07::kTh07EnemyBulletAnmResource.hasTextureData);
    assert(th07::kTh07EnemyBulletLoadFunctionAddress == 0x00426f60);
    assert(th07::kTh07EnemyBulletTemplateTableAddress == 0x00495160);
    assert(th07::kTh07EnemyBulletTemplateTableFileOffset == 0x00094360);
    assert(th07::kTh07EnemyBulletTemplateCount == 11);
    assert(th07::kTh07EnemyBulletResolvedTemplateCount == 10);
    assert(th07::kTh07EnemyBulletAnmScriptOffset == 0x0200);
    assert(th07::kTh07EnemyBulletResolvedScriptLast == 0x0218);
    assert(th07::CountTh07ResolvedEnemyBulletTemplates() == th07::kTh07EnemyBulletResolvedTemplateCount);
    assert(th07::IsTh07EnemyBulletTemplateScriptResolved(0x0200));
    assert(th07::IsTh07EnemyBulletTemplateScriptResolved(0x0218));
    assert(!th07::IsTh07EnemyBulletTemplateScriptResolved(0x02a8));
    const th07::Th07EnemyBulletTemplateScripts *pellet = th07::FindTh07EnemyBulletTemplate(0);
    assert(pellet != nullptr);
    assert(pellet->bulletScript == 0x0200);
    assert(pellet->spawnFastScript == 0x0212);
    assert(pellet->spawnDonutScript == 0x020f);
    assert(pellet->activeSprite == 0);
    assert(pellet->spriteWidth == 8);
    assert(pellet->spriteHeight == 8);
    assert(pellet->grazeSize == 4.0f);
    assert(pellet->spriteOffsetCategory == 5);
    const th07::Th07EnemyBulletTemplateScripts *script208 = th07::FindTh07EnemyBulletTemplate(8);
    assert(script208 != nullptr);
    assert(script208->bulletScript == 0x0208);
    assert(script208->activeSprite == 120);
    assert(script208->grazeSize == 5.0f);
    assert(script208->spriteOffsetCategory == 1);
    const th07::Th07EnemyBulletTemplateScripts *script209 = th07::FindTh07EnemyBulletTemplate(9);
    assert(script209 != nullptr);
    assert(script209->bulletScript == 0x0209);
    assert(script209->activeSprite == 128);
    assert(script209->grazeSize == 8.0f);
    assert(script209->spriteOffsetCategory == 2);
    const th07::Th07EnemyBulletTemplateScripts *blockedBullet = th07::FindTh07EnemyBulletTemplate(10);
    assert(blockedBullet != nullptr);
    assert(blockedBullet->status == th07::Th07BulletEvidenceStatus::Blocked);
    assert(blockedBullet->bulletScript == 0x02a8);
    assert(blockedBullet->activeSprite == th07::kTh07EnemyBulletTemplateMissingSprite);
    assert(th07::kTh07ObsoleteBulletResourceCarryovers.size() == 4);
    assert(th07::Th07BulletEvidenceStatusName(th07::Th07BulletEvidenceStatus::ObsoleteTh06Carryover) ==
           "obsolete-th06-carryover");
    assert(th07::Th07BulletEvidenceStatusName(th07::Th07BulletEvidenceStatus::Blocked) == "blocked");
    assert(th07::FindStageResources(4)->backgroundAnm == "data/stg4bg.anm");
    assert(th07::FindStageResources(8)->ecl.archivePath == "data/ecldata8.ecl");
    assert(th07::FindStageResources(9) == nullptr);
    assert(th07::FindShtFile(th07::Th07ShotType::SakuyaB, false)->file.archivePath == "data/ply02b.sht");
    assert(th07::FindShtFile(th07::Th07ShotType::SakuyaB, true)->file.archivePath == "data/ply02bs.sht");
    assert(th07::IsPointOfCollectionAvailable(128, false));
    assert(th07::IsPointOfCollectionAvailable(0, true));
    assert(!th07::IsPointOfCollectionAvailable(127, false));

    return 0;
}
