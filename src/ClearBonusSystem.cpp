#include "ClearBonusSystem.hpp"

namespace th07
{
const std::array<RankBonusRule, kTh07RankBonusRuleCount> kRankBonusRules = {{
    {Th07Difficulty::Easy, "Easy Rank    *0.5", 0.5f, 0x0049849c},
    {Th07Difficulty::Normal, "Normal Rank  *1.0", 1.0f, 0x00498488},
    {Th07Difficulty::Hard, "Hard Rank    *1.2", 1.2f, 0x00498474},
    {Th07Difficulty::Lunatic, "Lunatic Rank *1.5", 1.5f, 0x00498460},
    {Th07Difficulty::Extra, "Extra Rank   *2.0", 2.0f, 0x0049844c},
    {Th07Difficulty::Phantasm, "Phantasm Rank*2.0", 2.0f, 0x00498438},
}};

const std::array<BonusToastRule, kTh07BonusToastRuleCount> kTh07BonusToastRules = {{
    {BonusToastKind::FullPowerMode, "Full Power Mode!", kTh07FullPowerModeStringAddress,
     kTh07BonusToastFullPowerStringPushAddress, kTh07BonusToastFullPowerPositionAddAddress,
     kTh07BonusToastColorWarmBits, false, false},
    {BonusToastKind::SupernaturalBorder, "Supernatural Border!!", kTh07SupernaturalBorderStringAddress,
     kTh07BonusToastSupernaturalStringPushAddress, kTh07BonusToastSupernaturalPositionAddAddress,
     kTh07BonusToastColorBorderBits, false, true},
    {BonusToastKind::CherryPointMax, "CherryPoint Max!", kTh07CherryPointMaxStringAddress,
     kTh07BonusToastCherryPointMaxStringPushAddress, kTh07BonusToastCherryPointMaxPositionAddAddress,
     kTh07BonusToastColorWarmBits, false, false},
    {BonusToastKind::BorderBonus, "Border Bonus %7d", kTh07BorderBonusStringAddress,
     kTh07BonusToastBorderBonusStringPushAddress, kTh07BonusToastBorderBonusPositionAddAddress,
     kTh07BonusToastColorBorderBits, true, true},
}};

const BonusToastLifecycleContract kTh07BonusToastLifecycleContract = {
    kTh07BonusToastRegisterFunctionAddress,
    kTh07BonusToastUpdateFunctionAddress,
    kTh07BonusToastInitKindClearAddress,
    kTh07BonusToastPositionOffset,
    kTh07BorderBonusScoreOffset,
    kTh07BonusToastKindOffset,
    kTh07BonusToastTimerOffset,
    kTh07BonusToastTimerInitialPrevious,
    kTh07BonusToastSlideFrames,
    kTh07BonusToastLifetimeFrames,
};

const std::array<BonusToastProducerCall, kTh07BonusToastProducerCallCount> kTh07BonusToastProducerCalls = {{
    {BonusToastKind::CherryPointMax, "FUN_0042f5a2",
     kTh07BonusToastCherryPointMaxPrimaryProducerFunctionAddress,
     kTh07BonusToastCherryPointMaxPrimaryKindPushAddress,
     kTh07BonusToastCherryPointMaxPrimaryScorePushAddress,
     kTh07BonusToastCherryPointMaxPrimaryCallAddress, false, 0,
     "cherry cap delta: owner +0x961c minus player +0x88"},
    {BonusToastKind::CherryPointMax, "FUN_0042f69f",
     kTh07BonusToastCherryPointMaxSecondaryProducerFunctionAddress,
     kTh07BonusToastCherryPointMaxSecondaryKindPushAddress,
     kTh07BonusToastCherryPointMaxSecondaryScorePushAddress,
     kTh07BonusToastCherryPointMaxSecondaryCallAddress, false, 0,
     "cherry cap delta: owner +0x961c minus player +0x88"},
    {BonusToastKind::FullPowerMode, "FUN_00432990",
     kTh07BonusToastFullPowerItemProducerFunctionAddress,
     kTh07BonusToastFullPowerItemCase0KindPushAddress,
     kTh07BonusToastFullPowerItemCase0ScorePushAddress,
     kTh07BonusToastFullPowerItemCase0CallAddress, true, 0,
     "item collection full-power branch"},
    {BonusToastKind::FullPowerMode, "FUN_00432990",
     kTh07BonusToastFullPowerItemProducerFunctionAddress,
     kTh07BonusToastFullPowerItemCase0AltKindPushAddress,
     kTh07BonusToastFullPowerItemCase0AltScorePushAddress,
     kTh07BonusToastFullPowerItemCase0AltCallAddress, true, 0,
     "item collection full-power branch"},
    {BonusToastKind::FullPowerMode, "FUN_00432990",
     kTh07BonusToastFullPowerItemProducerFunctionAddress,
     kTh07BonusToastFullPowerItemCase4KindPushAddress,
     kTh07BonusToastFullPowerItemCase4ScorePushAddress,
     kTh07BonusToastFullPowerItemCase4CallAddress, true, 0,
     "item collection full-power item case"},
    {BonusToastKind::BorderBonus, "FUN_00441670",
     kTh07BonusToastBorderBonusProducerFunctionAddress,
     kTh07BonusToastBorderBonusKindPushAddress,
     kTh07BonusToastBorderBonusScorePushAddress,
     kTh07BonusToastBorderBonusCallAddress, false, 0,
     "border bonus score: (DAT_0062f88c - player +0x88) * 10"},
    {BonusToastKind::SupernaturalBorder, "FUN_00441960",
     kTh07BonusToastSupernaturalBorderProducerFunctionAddress,
     kTh07BonusToastSupernaturalBorderKindPushAddress,
     kTh07BonusToastSupernaturalBorderScorePushAddress,
     kTh07BonusToastSupernaturalBorderCallAddress, true, 0,
     "supernatural border start"},
}};

const std::array<ClearBonusOwnerField, kTh07ClearBonusOwnerFieldCount> kTh07ClearBonusOwnerFields = {{
    {ClearBonusOwnerFieldKind::ClearSummaryVisible, kTh07ClearBonusSummaryVisibleOffset,
     kTh07ClearBonusSummaryVisibleCheckAddress, "clear summary visible"},
    {ClearBonusOwnerFieldKind::ClearSummaryTotalScore, kTh07ClearBonusTotalScoreOffset,
     kTh07ClearBonusTotalScorePushAddress, "clear summary total score"},
    {ClearBonusOwnerFieldKind::BonusPopupPosition, kTh07BonusPopupPositionOffset,
     kTh07BonusPopupPositionAddAddress, "bonus popup text position"},
    {ClearBonusOwnerFieldKind::BonusPopupScore, kTh07BonusPopupScoreOffset,
     kTh07BonusPopupScorePushAddress, "bonus popup score"},
    {ClearBonusOwnerFieldKind::BonusPopupVisible, kTh07BonusPopupVisibleOffset,
     kTh07BonusPopupVisibleCheckAddress, "bonus popup visible"},
    {ClearBonusOwnerFieldKind::BonusToastPosition, kTh07BonusToastPositionOffset,
     kTh07BonusToastPositionAddAddress, "bonus toast text position"},
    {ClearBonusOwnerFieldKind::BorderBonusScore, kTh07BorderBonusScoreOffset,
     kTh07BorderBonusScorePushAddress, "border bonus score"},
    {ClearBonusOwnerFieldKind::BonusToastKind, kTh07BonusToastKindOffset,
     kTh07BonusToastKindLoadAddress, "bonus toast kind"},
    {ClearBonusOwnerFieldKind::SpellCardBonusPosition, kTh07SpellCardBonusPositionOffset,
     kTh07SpellCardBonusPositionWriteAddress, "spell-card bonus text position"},
    {ClearBonusOwnerFieldKind::SpellCardBonusLineY, kTh07SpellCardBonusLineYOffset,
     kTh07SpellCardBonusLineYWriteAddress, "spell-card bonus line y"},
    {ClearBonusOwnerFieldKind::SpellCardBonusCaptureId, kTh07SpellCardBonusCaptureIdOffset,
     kTh07SpellCardBonusCaptureIdPushAddress, "spell-card bonus capture id"},
    {ClearBonusOwnerFieldKind::SpellCardBonusVisible, kTh07SpellCardBonusVisibleOffset,
     kTh07SpellCardBonusVisibleCheckAddress, "spell-card bonus visible"},
    {ClearBonusOwnerFieldKind::PointItemsInStage, kTh07ClearBonusPointItemsOffset,
     kTh07ClearBonusPointItemsLoadAddress, "point items in stage"},
    {ClearBonusOwnerFieldKind::Cherry, kTh07ClearBonusCherryOffset,
     kTh07ClearBonusCherryLoadAddress, "cherry"},
    {ClearBonusOwnerFieldKind::GrazeInStage, kTh07ClearBonusGrazeOffset,
     kTh07ClearBonusGrazeLoadAddress, "graze in stage"},
}};

int StageClearBonus(int currentStage)
{
    return currentStage * kStageClearBonusPerStage;
}

int PointItemClearBonus(int pointItemsInStage)
{
    return pointItemsInStage * kPointItemClearBonus;
}

int GrazeClearBonus(int grazeInStage)
{
    return grazeInStage * kGrazeClearBonus;
}

int CherryClearBonus(int cherry)
{
    return cherry * kCherryClearBonus;
}

int LifeClearBonus(int livesRemaining)
{
    return livesRemaining * kLifeClearBonus;
}

int BombClearBonus(int bombsRemaining)
{
    return bombsRemaining * kBombClearBonus;
}

bool UsesTh07BonusToastSlidePath(int currentFrame)
{
    return currentFrame < kTh07BonusToastSlideFrames;
}

bool HasTh07BonusToastExpired(int currentFrame)
{
    return currentFrame >= kTh07BonusToastLifetimeFrames;
}

std::size_t CountTh07BonusToastProducerCalls(BonusToastKind kind)
{
    std::size_t count = 0;
    for (const BonusToastProducerCall &producer : kTh07BonusToastProducerCalls) {
        if (producer.kind == kind) {
            ++count;
        }
    }

    return count;
}

const BonusToastRule *FindTh07BonusToastRule(BonusToastKind kind)
{
    for (const BonusToastRule &rule : kTh07BonusToastRules) {
        if (rule.kind == kind) {
            return &rule;
        }
    }

    return nullptr;
}

const ClearBonusOwnerField *FindTh07ClearBonusOwnerField(ClearBonusOwnerFieldKind kind)
{
    for (const ClearBonusOwnerField &field : kTh07ClearBonusOwnerFields) {
        if (field.kind == kind) {
            return &field;
        }
    }

    return nullptr;
}
} // namespace th07
