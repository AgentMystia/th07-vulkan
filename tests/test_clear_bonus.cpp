#include "ClearBonusSystem.hpp"

#include <cassert>
#include <cmath>
#include <cstring>

namespace
{
bool NearlyEqual(float lhs, float rhs)
{
    return std::fabs(lhs - rhs) < 0.00001f;
}
} // namespace

int main()
{
    static_assert(th07::kTh07ClearBonusFunctionAddress == 0x00427f22);
    static_assert(th07::kTh07StageClearBonusInstructionAddress == 0x00427fc2);
    static_assert(th07::kTh07PointItemClearBonusInstructionAddress == 0x00428001);
    static_assert(th07::kTh07GrazeClearBonusInstructionAddress == 0x00428040);
    static_assert(th07::kTh07CherryClearBonusInstructionAddress == 0x0042807f);
    static_assert(th07::kTh07LifeClearBonusInstructionAddress == 0x00428103);
    static_assert(th07::kTh07BombClearBonusInstructionAddress == 0x00428143);
    static_assert(th07::kTh07ClearBonusSummaryVisibleCheckAddress == 0x00427f45);
    static_assert(th07::kTh07ClearBonusPointItemsLoadAddress == 0x00427ffb);
    static_assert(th07::kTh07ClearBonusGrazeLoadAddress == 0x0042803a);
    static_assert(th07::kTh07ClearBonusCherryLoadAddress == 0x00428079);
    static_assert(th07::kTh07ClearBonusTotalScorePushAddress == 0x0042831a);
    static_assert(th07::kTh07BonusPopupVisibleCheckAddress == 0x00428368);
    static_assert(th07::kTh07BonusPopupScorePushAddress == 0x00428381);
    static_assert(th07::kTh07BonusPopupPositionAddAddress == 0x00428392);
    static_assert(th07::kTh07BonusToastKindLoadAddress == 0x004283b2);
    static_assert(th07::kTh07BonusToastRegisterFunctionAddress == 0x00427c81);
    static_assert(th07::kTh07BonusToastRegisterPositionXLoadAddress == 0x00427c8c);
    static_assert(th07::kTh07BonusToastRegisterPositionYLoadAddress == 0x00427c95);
    static_assert(th07::kTh07BonusToastRegisterPositionAddAddress == 0x00427ca9);
    static_assert(th07::kTh07BonusToastRegisterKindWriteAddress == 0x00427cbe);
    static_assert(th07::kTh07BonusToastRegisterTimerAddAddress == 0x00427cca);
    static_assert(th07::kTh07BonusToastRegisterTimerCurrentClearAddress == 0x00427cd5);
    static_assert(th07::kTh07BonusToastRegisterTimerSubframeClearAddress == 0x00427cdc);
    static_assert(th07::kTh07BonusToastRegisterTimerPreviousWriteAddress == 0x00427ce4);
    static_assert(th07::kTh07BonusToastRegisterScoreWriteAddress == 0x00427cf3);
    static_assert(th07::kTh07BonusToastRegisterGlobalStateWriteAddress == 0x00427cf9);
    static_assert(th07::kTh07BonusToastCherryPointMaxPrimaryProducerFunctionAddress == 0x0042f5a2);
    static_assert(th07::kTh07BonusToastCherryPointMaxPrimaryKindPushAddress == 0x0042f679);
    static_assert(th07::kTh07BonusToastCherryPointMaxPrimaryScorePushAddress == 0x0042f690);
    static_assert(th07::kTh07BonusToastCherryPointMaxPrimaryCallAddress == 0x0042f696);
    static_assert(th07::kTh07BonusToastCherryPointMaxSecondaryProducerFunctionAddress == 0x0042f69f);
    static_assert(th07::kTh07BonusToastCherryPointMaxSecondaryKindPushAddress == 0x0042f710);
    static_assert(th07::kTh07BonusToastCherryPointMaxSecondaryScorePushAddress == 0x0042f727);
    static_assert(th07::kTh07BonusToastCherryPointMaxSecondaryCallAddress == 0x0042f72d);
    static_assert(th07::kTh07BonusToastFullPowerItemProducerFunctionAddress == 0x00432990);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0KindPushAddress == 0x00433006);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0ScorePushAddress == 0x00433008);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0CallAddress == 0x0043300f);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0AltKindPushAddress == 0x004334b2);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0AltScorePushAddress == 0x004334b4);
    static_assert(th07::kTh07BonusToastFullPowerItemCase0AltCallAddress == 0x004334bb);
    static_assert(th07::kTh07BonusToastFullPowerItemCase4KindPushAddress == 0x004335e5);
    static_assert(th07::kTh07BonusToastFullPowerItemCase4ScorePushAddress == 0x004335e7);
    static_assert(th07::kTh07BonusToastFullPowerItemCase4CallAddress == 0x004335ee);
    static_assert(th07::kTh07BonusToastBorderBonusProducerFunctionAddress == 0x00441670);
    static_assert(th07::kTh07BonusToastBorderBonusKindPushAddress == 0x004416d2);
    static_assert(th07::kTh07BonusToastBorderBonusScorePushAddress == 0x004416d7);
    static_assert(th07::kTh07BonusToastBorderBonusCallAddress == 0x004416dd);
    static_assert(th07::kTh07BonusToastSupernaturalBorderProducerFunctionAddress == 0x00441960);
    static_assert(th07::kTh07BonusToastSupernaturalBorderKindPushAddress == 0x00441b7a);
    static_assert(th07::kTh07BonusToastSupernaturalBorderScorePushAddress == 0x00441b7c);
    static_assert(th07::kTh07BonusToastSupernaturalBorderCallAddress == 0x00441b83);
    static_assert(th07::kTh07BonusToastUpdateFunctionAddress == 0x0042adab);
    static_assert(th07::kTh07BonusToastUpdateVisibleCheckAddress == 0x0042b255);
    static_assert(th07::kTh07BonusToastUpdateTimerCurrentCompareAddress == 0x0042b26a);
    static_assert(th07::kTh07BonusToastUpdateTimerAddAddress == 0x0042b27e);
    static_assert(th07::kTh07BonusToastUpdatePositionXWriteAddress == 0x0042b2aa);
    static_assert(th07::kTh07BonusToastUpdateFinalPositionXLoadAddress == 0x0042b2b8);
    static_assert(th07::kTh07BonusToastUpdateFinalPositionXWriteAddress == 0x0042b2be);
    static_assert(th07::kTh07BonusToastUpdateExpireCompareAddress == 0x0042b2cc);
    static_assert(th07::kTh07BonusToastUpdateKindClearAddress == 0x0042b2e3);
    static_assert(th07::kTh07BonusToastUpdateTimerCallAddress == 0x0042b2fb);
    static_assert(th07::kTh07BonusToastUpdateTimerCallTargetAddress == 0x004010b0);
    static_assert(th07::kTh07BonusToastInitKindClearAddress == 0x0042986f);
    static_assert(th07::kTh07BonusToastPositionAddAddress == 0x004283f5);
    static_assert(th07::kTh07BonusToastFullPowerCompareAddress == 0x004283bb);
    static_assert(th07::kTh07BonusToastSupernaturalBorderCompareAddress == 0x004283c1);
    static_assert(th07::kTh07BonusToastCherryPointMaxCompareAddress == 0x004283c7);
    static_assert(th07::kTh07BonusToastBorderBonusCompareAddress == 0x004283d1);
    static_assert(th07::kTh07BonusToastFullPowerColorWriteAddress == 0x004283e0);
    static_assert(th07::kTh07BonusToastFullPowerStringPushAddress == 0x004283ea);
    static_assert(th07::kTh07BonusToastFullPowerPositionAddAddress == 0x004283f5);
    static_assert(th07::kTh07BonusToastSupernaturalScaleLoadAddress == 0x00428414);
    static_assert(th07::kTh07BonusToastSupernaturalRenderModeWriteAddress == 0x00428428);
    static_assert(th07::kTh07BonusToastSupernaturalColorWriteAddress == 0x00428432);
    static_assert(th07::kTh07BonusToastSupernaturalStringPushAddress == 0x0042843c);
    static_assert(th07::kTh07BonusToastSupernaturalPositionAddAddress == 0x00428447);
    static_assert(th07::kTh07BonusToastSupernaturalRenderModeResetAddress == 0x00428471);
    static_assert(th07::kTh07BonusToastCherryPointMaxColorWriteAddress == 0x00428480);
    static_assert(th07::kTh07BonusToastCherryPointMaxStringPushAddress == 0x0042848a);
    static_assert(th07::kTh07BonusToastCherryPointMaxPositionAddAddress == 0x00428495);
    static_assert(th07::kTh07BonusToastBorderBonusScaleLoadAddress == 0x004284b1);
    static_assert(th07::kTh07BonusToastBorderBonusRenderModeWriteAddress == 0x004284c5);
    static_assert(th07::kTh07BonusToastBorderBonusColorWriteAddress == 0x004284cf);
    static_assert(th07::kTh07BorderBonusScorePushAddress == 0x004284df);
    static_assert(th07::kTh07BonusToastBorderBonusStringPushAddress == 0x004284e5);
    static_assert(th07::kTh07BonusToastBorderBonusPositionAddAddress == 0x004284f0);
    static_assert(th07::kTh07BonusToastBorderBonusRenderModeResetAddress == 0x0042851a);
    static_assert(th07::kTh07SpellCardBonusVisibleCheckAddress == 0x0042852a);
    static_assert(th07::kTh07SpellCardBonusPositionWriteAddress == 0x00428565);
    static_assert(th07::kTh07SpellCardBonusLineYWriteAddress == 0x00428577);
    static_assert(th07::kTh07SpellCardBonusCaptureIdPushAddress == 0x004285bf);
    static_assert(th07::kTh07FullPowerModeStringAddress == 0x004983e0);
    static_assert(th07::kTh07BonusToastPositionXStartConstantAddress == 0x00498a74);
    static_assert(th07::kTh07BonusToastPositionYStartConstantAddress == 0x00498b30);
    static_assert(th07::kTh07BonusToastSlideDeltaConstantAddress == 0x00498cb8);
    static_assert(th07::kTh07BonusToastSlideDivisorConstantAddress == 0x00498b00);
    static_assert(th07::kTh07BonusToastPositionXFinalConstantAddress == 0x00498bb8);
    static_assert(th07::kTh07RankBonusRuleCount == 6);
    static_assert(th07::kTh07BonusToastRuleCount == 4);
    static_assert(th07::kTh07BonusToastProducerCallCount == 7);
    static_assert(th07::kTh07ClearBonusOwnerFieldCount == 15);
    static_assert(th07::kTh07ClearBonusSummaryVisibleOffset == 0x209b4);
    static_assert(th07::kTh07ClearBonusTotalScoreOffset == 0x209b8);
    static_assert(th07::kTh07BonusPopupPositionOffset == 0x209c0);
    static_assert(th07::kTh07BonusPopupScoreOffset == 0x209cc);
    static_assert(th07::kTh07BonusPopupVisibleOffset == 0x209d0);
    static_assert(th07::kTh07BonusToastPositionOffset == 0x209e0);
    static_assert(th07::kTh07BonusToastPositionYOffset == 0x209e4);
    static_assert(th07::kTh07BonusToastPositionZOffset == 0x209e8);
    static_assert(th07::kTh07BorderBonusScoreOffset == 0x209ec);
    static_assert(th07::kTh07BonusToastKindOffset == 0x209f0);
    static_assert(th07::kTh07BonusToastTimerOffset == 0x209f4);
    static_assert(th07::kTh07BonusToastTimerPreviousOffset == 0x209f4);
    static_assert(th07::kTh07BonusToastTimerSubframeOffset == 0x209f8);
    static_assert(th07::kTh07BonusToastTimerCurrentOffset == 0x209fc);
    static_assert(th07::kTh07SpellCardBonusPositionOffset == 0x20a00);
    static_assert(th07::kTh07SpellCardBonusLineYOffset == 0x20a04);
    static_assert(th07::kTh07SpellCardBonusCaptureIdOffset == 0x20a0c);
    static_assert(th07::kTh07SpellCardBonusVisibleOffset == 0x20a10);
    static_assert(th07::kTh07ClearBonusPointItemsOffset == 0x20a24);
    static_assert(th07::kTh07ClearBonusCherryOffset == 0x20a28);
    static_assert(th07::kTh07ClearBonusGrazeOffset == 0x20a2c);
    static_assert(th07::kStageClearBonusPerStage == 1000000);
    static_assert(th07::kPointItemClearBonus == 50000);
    static_assert(th07::kGrazeClearBonus == 500);
    static_assert(th07::kCherryClearBonus == 10);
    static_assert(th07::kLifeClearBonus == 20000000);
    static_assert(th07::kBombClearBonus == 4000000);
    static_assert(th07::kTh07BonusToastColorWarmBits == 0xffc0b0ff);
    static_assert(th07::kTh07BonusToastColorBorderBits == 0xffe0b0ff);
    static_assert(th07::kTh07BonusToastScaledRenderMode == 0x0b);
    static_assert(th07::kTh07BonusToastDefaultRenderMode == 0x0e);
    static_assert(th07::kTh07BonusToastTimerInitialPrevious == -999);
    static_assert(th07::kTh07BonusToastSlideFrames == 30);
    static_assert(th07::kTh07BonusToastLifetimeFrames == 180);
    static_assert(th07::kTh07BonusToastRegisterGlobalState == 2);

    assert(th07::StageClearBonus(6) == 6000000);
    assert(th07::PointItemClearBonus(37) == 1850000);
    assert(th07::GrazeClearBonus(1234) == 617000);
    assert(th07::CherryClearBonus(500000) == 5000000);
    assert(th07::LifeClearBonus(4) == 80000000);
    assert(th07::BombClearBonus(3) == 12000000);

    assert(th07::kRankBonusRules.size() == th07::kTh07RankBonusRuleCount);
    assert(th07::kRankBonusRules[0].difficulty == th07::Th07Difficulty::Easy);
    assert(std::strcmp(th07::kRankBonusRules[0].label, "Easy Rank    *0.5") == 0);
    assert(NearlyEqual(th07::kRankBonusRules[0].multiplier, 0.5f));
    assert(th07::kRankBonusRules[0].ghidraStringAddress == 0x0049849c);
    assert(th07::kRankBonusRules[3].difficulty == th07::Th07Difficulty::Lunatic);
    assert(NearlyEqual(th07::kRankBonusRules[3].multiplier, 1.5f));
    assert(th07::kRankBonusRules[5].difficulty == th07::Th07Difficulty::Phantasm);
    assert(std::strcmp(th07::kRankBonusRules[5].label, "Phantasm Rank*2.0") == 0);
    assert(NearlyEqual(th07::kRankBonusRules[5].multiplier, 2.0f));
    assert(th07::kRankBonusRules[5].ghidraStringAddress == 0x00498438);

    assert(th07::kTh07BonusToastRules.size() == th07::kTh07BonusToastRuleCount);
    assert(th07::kTh07BonusToastRules[0].kind == th07::BonusToastKind::FullPowerMode);
    assert(std::strcmp(th07::kTh07BonusToastRules[0].label, "Full Power Mode!") == 0);
    assert(th07::kTh07BonusToastRules[0].ghidraStringAddress == th07::kTh07FullPowerModeStringAddress);
    assert(th07::kTh07BonusToastRules[0].colorBits == th07::kTh07BonusToastColorWarmBits);
    assert(!th07::kTh07BonusToastRules[0].usesBorderBonusScore);
    assert(!th07::kTh07BonusToastRules[0].usesScaledRenderState);
    assert(th07::kTh07BonusToastRules[1].kind == th07::BonusToastKind::SupernaturalBorder);
    assert(std::strcmp(th07::kTh07BonusToastRules[1].label, "Supernatural Border!!") == 0);
    assert(th07::kTh07BonusToastRules[1].ghidraStringAddress ==
           th07::kTh07SupernaturalBorderStringAddress);
    assert(th07::kTh07BonusToastRules[1].positionAddAddress ==
           th07::kTh07BonusToastSupernaturalPositionAddAddress);
    assert(th07::kTh07BonusToastRules[1].usesScaledRenderState);
    assert(th07::kTh07BonusToastRules[2].kind == th07::BonusToastKind::CherryPointMax);
    assert(th07::kTh07BonusToastRules[2].ghidraStringAddress == th07::kTh07CherryPointMaxStringAddress);
    assert(th07::kTh07BonusToastRules[3].kind == th07::BonusToastKind::BorderBonus);
    assert(std::strcmp(th07::kTh07BonusToastRules[3].label, "Border Bonus %7d") == 0);
    assert(th07::kTh07BonusToastRules[3].ghidraStringAddress == th07::kTh07BorderBonusStringAddress);
    assert(th07::kTh07BonusToastRules[3].stringPushAddress ==
           th07::kTh07BonusToastBorderBonusStringPushAddress);
    assert(th07::kTh07BonusToastRules[3].colorBits == th07::kTh07BonusToastColorBorderBits);
    assert(th07::kTh07BonusToastRules[3].usesBorderBonusScore);
    assert(th07::kTh07BonusToastRules[3].usesScaledRenderState);
    assert(th07::FindTh07BonusToastRule(th07::BonusToastKind::SupernaturalBorder)->positionAddAddress ==
           th07::kTh07BonusToastSupernaturalPositionAddAddress);
    assert(th07::FindTh07BonusToastRule(th07::BonusToastKind::None) == nullptr);
    assert(th07::kTh07BonusToastLifecycleContract.registerFunctionAddress ==
           th07::kTh07BonusToastRegisterFunctionAddress);
    assert(th07::kTh07BonusToastLifecycleContract.updateFunctionAddress ==
           th07::kTh07BonusToastUpdateFunctionAddress);
    assert(th07::kTh07BonusToastLifecycleContract.initKindClearAddress ==
           th07::kTh07BonusToastInitKindClearAddress);
    assert(th07::kTh07BonusToastLifecycleContract.positionOffset == th07::kTh07BonusToastPositionOffset);
    assert(th07::kTh07BonusToastLifecycleContract.scoreOffset == th07::kTh07BorderBonusScoreOffset);
    assert(th07::kTh07BonusToastLifecycleContract.kindOffset == th07::kTh07BonusToastKindOffset);
    assert(th07::kTh07BonusToastLifecycleContract.timerOffset == th07::kTh07BonusToastTimerOffset);
    assert(th07::kTh07BonusToastLifecycleContract.timerPreviousInitial ==
           th07::kTh07BonusToastTimerInitialPrevious);
    assert(th07::kTh07BonusToastLifecycleContract.slideFrames == th07::kTh07BonusToastSlideFrames);
    assert(th07::kTh07BonusToastLifecycleContract.lifetimeFrames == th07::kTh07BonusToastLifetimeFrames);
    assert(th07::UsesTh07BonusToastSlidePath(0));
    assert(th07::UsesTh07BonusToastSlidePath(29));
    assert(!th07::UsesTh07BonusToastSlidePath(30));
    assert(!th07::HasTh07BonusToastExpired(179));
    assert(th07::HasTh07BonusToastExpired(180));
    assert(th07::kTh07BonusToastProducerCalls.size() == th07::kTh07BonusToastProducerCallCount);
    assert(th07::kTh07BonusToastProducerCalls[0].kind == th07::BonusToastKind::CherryPointMax);
    assert(std::strcmp(th07::kTh07BonusToastProducerCalls[0].sourceFunction, "FUN_0042f5a2") == 0);
    assert(th07::kTh07BonusToastProducerCalls[0].callAddress ==
           th07::kTh07BonusToastCherryPointMaxPrimaryCallAddress);
    assert(!th07::kTh07BonusToastProducerCalls[0].scoreIsImmediate);
    assert(th07::kTh07BonusToastProducerCalls[2].kind == th07::BonusToastKind::FullPowerMode);
    assert(th07::kTh07BonusToastProducerCalls[2].scoreIsImmediate);
    assert(th07::kTh07BonusToastProducerCalls[2].scoreImmediate == 0);
    assert(th07::kTh07BonusToastProducerCalls[5].kind == th07::BonusToastKind::BorderBonus);
    assert(th07::kTh07BonusToastProducerCalls[5].callAddress ==
           th07::kTh07BonusToastBorderBonusCallAddress);
    assert(!th07::kTh07BonusToastProducerCalls[5].scoreIsImmediate);
    assert(th07::kTh07BonusToastProducerCalls[6].kind == th07::BonusToastKind::SupernaturalBorder);
    assert(th07::CountTh07BonusToastProducerCalls(th07::BonusToastKind::FullPowerMode) == 3);
    assert(th07::CountTh07BonusToastProducerCalls(th07::BonusToastKind::CherryPointMax) == 2);
    assert(th07::CountTh07BonusToastProducerCalls(th07::BonusToastKind::BorderBonus) == 1);
    assert(th07::CountTh07BonusToastProducerCalls(th07::BonusToastKind::SupernaturalBorder) == 1);
    assert(th07::CountTh07BonusToastProducerCalls(th07::BonusToastKind::None) == 0);

    assert(th07::kTh07ClearBonusOwnerFields.size() == th07::kTh07ClearBonusOwnerFieldCount);
    assert(th07::kTh07ClearBonusOwnerFields[0].kind == th07::ClearBonusOwnerFieldKind::ClearSummaryVisible);
    assert(th07::kTh07ClearBonusOwnerFields[0].offset == th07::kTh07ClearBonusSummaryVisibleOffset);
    assert(th07::kTh07ClearBonusOwnerFields[0].firstUseAddress ==
           th07::kTh07ClearBonusSummaryVisibleCheckAddress);
    assert(std::strcmp(th07::kTh07ClearBonusOwnerFields[0].name, "clear summary visible") == 0);
    assert(th07::kTh07ClearBonusOwnerFields[7].kind == th07::ClearBonusOwnerFieldKind::BonusToastKind);
    assert(th07::kTh07ClearBonusOwnerFields[7].offset == th07::kTh07BonusToastKindOffset);
    assert(th07::kTh07ClearBonusOwnerFields[7].firstUseAddress == th07::kTh07BonusToastKindLoadAddress);
    assert(th07::kTh07ClearBonusOwnerFields[12].kind == th07::ClearBonusOwnerFieldKind::PointItemsInStage);
    assert(th07::kTh07ClearBonusOwnerFields[14].kind == th07::ClearBonusOwnerFieldKind::GrazeInStage);
    assert(th07::kTh07ClearBonusOwnerFields[14].offset == th07::kTh07ClearBonusGrazeOffset);
    assert(th07::FindTh07ClearBonusOwnerField(th07::ClearBonusOwnerFieldKind::BonusPopupScore)->offset ==
           th07::kTh07BonusPopupScoreOffset);
    assert(th07::FindTh07ClearBonusOwnerField(th07::ClearBonusOwnerFieldKind::SpellCardBonusCaptureId)
               ->firstUseAddress == th07::kTh07SpellCardBonusCaptureIdPushAddress);

    return 0;
}
