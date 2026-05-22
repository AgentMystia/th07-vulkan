#include "BorderSystem.hpp"
#include "CherrySystem.hpp"

#include <cassert>
#include <cstring>

int main()
{
    static_assert(th07::kTh07CherryPointMaxStringAddress == 0x004983b4);
    static_assert(th07::kTh07CherryValueStringAddress == 0x004984cc);
    static_assert(th07::kTh07CherryHudStringAnchorCount == 2);
    static_assert(th07::kTh07CherryManagerAddress == 0x00626270);
    static_assert(th07::kTh07CherryPrimaryGainFunctionAddress == 0x0042f5a2);
    static_assert(th07::kTh07CherrySecondaryGainFunctionAddress == 0x0042f69f);
    static_assert(th07::kTh07CherryEclOpcode160CallAddress == 0x00416722);
    static_assert(th07::kTh07CherryEclOpcode160ThisPointerAddress == 0x00626270);
    static_assert(th07::kTh07CherryEclGainOpcode == 160);
    static_assert(th07::kTh07CherryCapOffset == 0x9618);
    static_assert(th07::kTh07CherryCurrentOffset == 0x961c);
    static_assert(th07::kTh07CherryPlusOffset == 0x9620);
    static_assert(th07::kTh07CherryPlayerPointerOffset == 0x0008);
    static_assert(th07::kTh07CherryPlayerThresholdOffset == 0x0088);
    static_assert(th07::kTh07CherryDisableFlagAddress == 0x004bfee5);
    static_assert(th07::kTh07CherryBorderStartFunctionAddress == 0x00441960);
    static_assert(th07::kTh07CherryBorderStartThisPointerAddress == 0x004bdad8);
    static_assert(th07::kTh07CherryToastRegisterFunctionAddress == 0x00427c81);
    static_assert(th07::kTh07CherryToastRegisterThisPointerAddress == 0x0049fbf0);
    static_assert(th07::kTh07CherryPointMaxToastKind == 3);
    static_assert(th07::kTh07CherryGainHelperCount == 2);
    static_assert(th07::kTh07BorderBonusStringAddress == 0x004983a0);
    static_assert(th07::kTh07SupernaturalBorderStringAddress == 0x004983c8);
    static_assert(th07::kTh07PlayerPenaltyLowStringAddress == 0x00498410);
    static_assert(th07::kTh07PlayerPenaltyHalfStringAddress == 0x00498424);
    static_assert(th07::kTh07PhantasmRankStringAddress == 0x00498438);
    static_assert(th07::kTh07BorderHudStringAnchorCount == 5);

    assert(th07::kCherryHudStringAnchors.size() == th07::kTh07CherryHudStringAnchorCount);
    assert(std::strcmp(th07::kCherryHudStringAnchors[0].text, "CherryPoint Max!") == 0);
    assert(th07::kCherryHudStringAnchors[0].ghidraStringAddress == th07::kTh07CherryPointMaxStringAddress);
    assert(std::strcmp(th07::kCherryHudStringAnchors[1].text, "Cherry = %8d") == 0);
    assert(th07::kCherryHudStringAnchors[1].ghidraStringAddress == th07::kTh07CherryValueStringAddress);
    assert(th07::kTh07CherryGainHelperContracts.size() == th07::kTh07CherryGainHelperCount);
    const th07::CherryGainHelperContract *primary =
        th07::FindTh07CherryGainHelperContract(th07::CherryGainHelperKind::Primary);
    const th07::CherryGainHelperContract *secondary =
        th07::FindTh07CherryGainHelperContract(th07::CherryGainHelperKind::Secondary);
    assert(primary != nullptr);
    assert(secondary != nullptr);
    assert(std::strcmp(primary->sourceFunction, "FUN_0042f5a2") == 0);
    assert(primary->functionAddress == th07::kTh07CherryPrimaryGainFunctionAddress);
    assert(primary->managerAddress == th07::kTh07CherryManagerAddress);
    assert(primary->updatesCherryPlus);
    assert(primary->currentOffset == 0x961c);
    assert(primary->capOffset == 0x9618);
    assert(primary->cherryPlusOffset == 0x9620);
    assert(primary->playerPointerOffset == 0x0008);
    assert(primary->playerThresholdOffset == 0x0088);
    assert(primary->thresholdDelta == th07::kCherryBorderStart);
    assert(primary->disableFlagAddress == 0x004bfee5);
    assert(primary->borderStartCallAddress == 0x0042f652);
    assert(primary->borderStartFunctionAddress == 0x00441960);
    assert(primary->borderStartThisPointerAddress == 0x004bdad8);
    assert(primary->toastCallAddress == 0x0042f696);
    assert(primary->toastRegisterFunctionAddress == 0x00427c81);
    assert(primary->toastRegisterThisPointerAddress == 0x0049fbf0);
    assert(primary->toastKind == th07::kTh07CherryPointMaxToastKind);
    assert(std::strcmp(primary->evidenceLines, "0042f5a2:8-23") == 0);
    assert(std::strcmp(secondary->sourceFunction, "FUN_0042f69f") == 0);
    assert(secondary->functionAddress == th07::kTh07CherrySecondaryGainFunctionAddress);
    assert(!secondary->updatesCherryPlus);
    assert(secondary->cherryPlusWriteAddress == th07::kTh07CherryNoInstructionAddress);
    assert(secondary->borderStartCallAddress == th07::kTh07CherryNoInstructionAddress);
    assert(secondary->toastCallAddress == 0x0042f72d);
    assert(std::strcmp(secondary->evidenceLines, "0042f69f:8-17") == 0);

    assert(th07::kBorderHudStringAnchors.size() == th07::kTh07BorderHudStringAnchorCount);
    assert(std::strcmp(th07::kBorderHudStringAnchors[0].text, "Border Bonus %7d") == 0);
    assert(th07::kBorderHudStringAnchors[0].ghidraStringAddress == th07::kTh07BorderBonusStringAddress);
    assert(std::strcmp(th07::kBorderHudStringAnchors[1].text, "Supernatural Border!!") == 0);
    assert(th07::kBorderHudStringAnchors[1].ghidraStringAddress == th07::kTh07SupernaturalBorderStringAddress);
    assert(std::strcmp(th07::kBorderHudStringAnchors[2].text, "Player Penalty*0.2") == 0);
    assert(th07::kBorderHudStringAnchors[2].ghidraStringAddress == th07::kTh07PlayerPenaltyLowStringAddress);
    assert(std::strcmp(th07::kBorderHudStringAnchors[3].text, "Player Penalty*0.5") == 0);
    assert(th07::kBorderHudStringAnchors[3].ghidraStringAddress == th07::kTh07PlayerPenaltyHalfStringAddress);
    assert(std::strcmp(th07::kBorderHudStringAnchors[4].text, "Phantasm Rank*2.0") == 0);
    assert(th07::kBorderHudStringAnchors[4].ghidraStringAddress == th07::kTh07PhantasmRankStringAddress);

    return 0;
}
