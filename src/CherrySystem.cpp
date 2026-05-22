#include "CherrySystem.hpp"

#include "ClearBonusSystem.hpp"

namespace th07
{
const std::array<CherryHudStringAnchor, kTh07CherryHudStringAnchorCount> kCherryHudStringAnchors = {
    CherryHudStringAnchor{"CherryPoint Max!", kTh07CherryPointMaxStringAddress},
    CherryHudStringAnchor{"Cherry = %8d", kTh07CherryValueStringAddress},
};

const std::array<CherryGainHelperContract, kTh07CherryGainHelperCount> kTh07CherryGainHelperContracts = {{
    {CherryGainHelperKind::Primary,
     "FUN_0042f5a2",
     kTh07CherryPrimaryGainFunctionAddress,
     kTh07CherryManagerAddress,
     true,
     kTh07CherryCurrentOffset,
     kTh07CherryCapOffset,
     kTh07CherryPlusOffset,
     kTh07CherryPlayerPointerOffset,
     kTh07CherryPlayerThresholdOffset,
     kCherryBorderStart,
     kTh07CherryDisableFlagAddress,
     kTh07CherryPrimaryCurrentWriteAddress,
     kTh07CherryPrimaryCapCompareAddress,
     kTh07CherryPrimaryCapClampWriteAddress,
     kTh07CherryPrimaryPlusWriteAddress,
     kTh07CherryPrimaryPlusClampWriteAddress,
     kTh07CherryPrimaryBorderStartCallAddress,
     kTh07CherryBorderStartFunctionAddress,
     kTh07CherryBorderStartThisPointerAddress,
     kTh07BonusToastCherryPointMaxPrimaryKindPushAddress,
     kTh07BonusToastCherryPointMaxPrimaryScorePushAddress,
     kTh07BonusToastCherryPointMaxPrimaryCallAddress,
     kTh07CherryToastRegisterFunctionAddress,
     kTh07CherryToastRegisterThisPointerAddress,
     kTh07CherryPointMaxToastKind,
     "0042f5a2:8-23"},
    {CherryGainHelperKind::Secondary,
     "FUN_0042f69f",
     kTh07CherrySecondaryGainFunctionAddress,
     kTh07CherryManagerAddress,
     false,
     kTh07CherryCurrentOffset,
     kTh07CherryCapOffset,
     kTh07CherryPlusOffset,
     kTh07CherryPlayerPointerOffset,
     kTh07CherryPlayerThresholdOffset,
     kCherryBorderStart,
     kTh07CherryDisableFlagAddress,
     kTh07CherrySecondaryCurrentWriteAddress,
     kTh07CherrySecondaryCapCompareAddress,
     kTh07CherrySecondaryCapClampWriteAddress,
     kTh07CherryNoInstructionAddress,
     kTh07CherryNoInstructionAddress,
     kTh07CherryNoInstructionAddress,
     kTh07CherryNoInstructionAddress,
     kTh07CherryNoInstructionAddress,
     kTh07BonusToastCherryPointMaxSecondaryKindPushAddress,
     kTh07BonusToastCherryPointMaxSecondaryScorePushAddress,
     kTh07BonusToastCherryPointMaxSecondaryCallAddress,
     kTh07CherryToastRegisterFunctionAddress,
     kTh07CherryToastRegisterThisPointerAddress,
     kTh07CherryPointMaxToastKind,
     "0042f69f:8-17"},
}};

const CherryGainHelperContract *FindTh07CherryGainHelperContract(CherryGainHelperKind kind)
{
    for (const CherryGainHelperContract &contract : kTh07CherryGainHelperContracts) {
        if (contract.kind == kind) {
            return &contract;
        }
    }
    return nullptr;
}
} // namespace th07
