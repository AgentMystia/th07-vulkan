#pragma once

#include "Th07Constants.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
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

struct RuntimeAnmLoadContract {
    ResourceStringAnchor file;
    std::string_view evidenceFunction;
    std::uint8_t anmFileSlot;
    std::uint16_t spriteScriptOffset;
};

constexpr std::size_t kTh07RuntimeAnmFileCount = 63;
constexpr std::size_t kTh07RuntimeAnmLoadContractCount = 46;

extern const std::array<ResourceStringAnchor, 8> kTh07EclFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07StdFiles;
extern const std::array<ResourceStringAnchor, 8> kTh07MsgFiles;
extern const std::array<StageResourceSet, 8> kTh07StageResources;
extern const std::array<ShtFileBinding, 12> kTh07ShtFiles;
extern const ResourceStringAnchor kTh07EnemyBulletAnmFile;
extern const std::array<ResourceStringAnchor, kTh07RuntimeAnmFileCount> kTh07RuntimeAnmFiles;
extern const std::array<RuntimeAnmLoadContract, kTh07RuntimeAnmLoadContractCount> kTh07RuntimeAnmLoadContracts;

constexpr std::string_view kTh07VersionString = "` Perfect Cherry Blossom. ver 1.00b";
constexpr std::string_view kTh07VersionStringAddress = "00497ba9";
constexpr std::string_view kTh07BgmFmtPath = "bgm/thbgm.fmt";
constexpr std::string_view kTh07BgmDatPath = "./thbgm.dat";

const StageResourceSet* FindStageResources(int stageNumber);
const ShtFileBinding* FindShtFile(Th07ShotType shotType, bool focused);
const ResourceStringAnchor* FindRuntimeAnmFile(std::string_view archivePath);
const RuntimeAnmLoadContract* FindRuntimeAnmLoadContract(std::string_view archivePath);

} // namespace th07
