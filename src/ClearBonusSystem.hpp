#pragma once

#include "BorderSystem.hpp"
#include "CherrySystem.hpp"
#include "Th07Constants.hpp"
#include "inttypes.hpp"

#include <array>
#include <cstddef>

namespace th07
{
inline constexpr u32 kTh07ClearBonusFunctionAddress = 0x00427f22;
inline constexpr u32 kTh07StageClearBonusInstructionAddress = 0x00427fc2;
inline constexpr u32 kTh07PointItemClearBonusInstructionAddress = 0x00428001;
inline constexpr u32 kTh07GrazeClearBonusInstructionAddress = 0x00428040;
inline constexpr u32 kTh07CherryClearBonusInstructionAddress = 0x0042807f;
inline constexpr u32 kTh07LifeClearBonusInstructionAddress = 0x00428103;
inline constexpr u32 kTh07BombClearBonusInstructionAddress = 0x00428143;

inline constexpr u32 kTh07ClearBonusSummaryVisibleCheckAddress = 0x00427f45;
inline constexpr u32 kTh07ClearBonusPointItemsLoadAddress = 0x00427ffb;
inline constexpr u32 kTh07ClearBonusGrazeLoadAddress = 0x0042803a;
inline constexpr u32 kTh07ClearBonusCherryLoadAddress = 0x00428079;
inline constexpr u32 kTh07ClearBonusTotalScorePushAddress = 0x0042831a;
inline constexpr u32 kTh07BonusPopupVisibleCheckAddress = 0x00428368;
inline constexpr u32 kTh07BonusPopupScorePushAddress = 0x00428381;
inline constexpr u32 kTh07BonusPopupPositionAddAddress = 0x00428392;
inline constexpr u32 kTh07BonusToastKindLoadAddress = 0x004283b2;
inline constexpr u32 kTh07BonusToastRegisterFunctionAddress = 0x00427c81;
inline constexpr u32 kTh07BonusToastRegisterPositionXLoadAddress = 0x00427c8c;
inline constexpr u32 kTh07BonusToastRegisterPositionYLoadAddress = 0x00427c95;
inline constexpr u32 kTh07BonusToastRegisterPositionAddAddress = 0x00427ca9;
inline constexpr u32 kTh07BonusToastRegisterKindWriteAddress = 0x00427cbe;
inline constexpr u32 kTh07BonusToastRegisterTimerAddAddress = 0x00427cca;
inline constexpr u32 kTh07BonusToastRegisterTimerCurrentClearAddress = 0x00427cd5;
inline constexpr u32 kTh07BonusToastRegisterTimerSubframeClearAddress = 0x00427cdc;
inline constexpr u32 kTh07BonusToastRegisterTimerPreviousWriteAddress = 0x00427ce4;
inline constexpr u32 kTh07BonusToastRegisterScoreWriteAddress = 0x00427cf3;
inline constexpr u32 kTh07BonusToastRegisterGlobalStateWriteAddress = 0x00427cf9;
inline constexpr u32 kTh07BonusToastCherryPointMaxPrimaryProducerFunctionAddress = 0x0042f5a2;
inline constexpr u32 kTh07BonusToastCherryPointMaxPrimaryKindPushAddress = 0x0042f679;
inline constexpr u32 kTh07BonusToastCherryPointMaxPrimaryScorePushAddress = 0x0042f690;
inline constexpr u32 kTh07BonusToastCherryPointMaxPrimaryCallAddress = 0x0042f696;
inline constexpr u32 kTh07BonusToastCherryPointMaxSecondaryProducerFunctionAddress = 0x0042f69f;
inline constexpr u32 kTh07BonusToastCherryPointMaxSecondaryKindPushAddress = 0x0042f710;
inline constexpr u32 kTh07BonusToastCherryPointMaxSecondaryScorePushAddress = 0x0042f727;
inline constexpr u32 kTh07BonusToastCherryPointMaxSecondaryCallAddress = 0x0042f72d;
inline constexpr u32 kTh07BonusToastFullPowerItemProducerFunctionAddress = 0x00432990;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0KindPushAddress = 0x00433006;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0ScorePushAddress = 0x00433008;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0CallAddress = 0x0043300f;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0AltKindPushAddress = 0x004334b2;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0AltScorePushAddress = 0x004334b4;
inline constexpr u32 kTh07BonusToastFullPowerItemCase0AltCallAddress = 0x004334bb;
inline constexpr u32 kTh07BonusToastFullPowerItemCase4KindPushAddress = 0x004335e5;
inline constexpr u32 kTh07BonusToastFullPowerItemCase4ScorePushAddress = 0x004335e7;
inline constexpr u32 kTh07BonusToastFullPowerItemCase4CallAddress = 0x004335ee;
inline constexpr u32 kTh07BonusToastBorderBonusProducerFunctionAddress = 0x00441670;
inline constexpr u32 kTh07BonusToastBorderBonusKindPushAddress = 0x004416d2;
inline constexpr u32 kTh07BonusToastBorderBonusScorePushAddress = 0x004416d7;
inline constexpr u32 kTh07BonusToastBorderBonusCallAddress = 0x004416dd;
inline constexpr u32 kTh07BonusToastSupernaturalBorderProducerFunctionAddress = 0x00441960;
inline constexpr u32 kTh07BonusToastSupernaturalBorderKindPushAddress = 0x00441b7a;
inline constexpr u32 kTh07BonusToastSupernaturalBorderScorePushAddress = 0x00441b7c;
inline constexpr u32 kTh07BonusToastSupernaturalBorderCallAddress = 0x00441b83;
inline constexpr u32 kTh07BonusToastUpdateFunctionAddress = 0x0042adab;
inline constexpr u32 kTh07BonusToastUpdateVisibleCheckAddress = 0x0042b255;
inline constexpr u32 kTh07BonusToastUpdateTimerCurrentCompareAddress = 0x0042b26a;
inline constexpr u32 kTh07BonusToastUpdateTimerAddAddress = 0x0042b27e;
inline constexpr u32 kTh07BonusToastUpdatePositionXWriteAddress = 0x0042b2aa;
inline constexpr u32 kTh07BonusToastUpdateFinalPositionXLoadAddress = 0x0042b2b8;
inline constexpr u32 kTh07BonusToastUpdateFinalPositionXWriteAddress = 0x0042b2be;
inline constexpr u32 kTh07BonusToastUpdateExpireCompareAddress = 0x0042b2cc;
inline constexpr u32 kTh07BonusToastUpdateKindClearAddress = 0x0042b2e3;
inline constexpr u32 kTh07BonusToastUpdateTimerCallAddress = 0x0042b2fb;
inline constexpr u32 kTh07BonusToastUpdateTimerCallTargetAddress = 0x004010b0;
inline constexpr u32 kTh07BonusToastInitKindClearAddress = 0x0042986f;
inline constexpr u32 kTh07BonusToastPositionAddAddress = 0x004283f5;
inline constexpr u32 kTh07BonusToastFullPowerCompareAddress = 0x004283bb;
inline constexpr u32 kTh07BonusToastSupernaturalBorderCompareAddress = 0x004283c1;
inline constexpr u32 kTh07BonusToastCherryPointMaxCompareAddress = 0x004283c7;
inline constexpr u32 kTh07BonusToastBorderBonusCompareAddress = 0x004283d1;
inline constexpr u32 kTh07BonusToastFullPowerColorWriteAddress = 0x004283e0;
inline constexpr u32 kTh07BonusToastFullPowerStringPushAddress = 0x004283ea;
inline constexpr u32 kTh07BonusToastFullPowerPositionAddAddress = 0x004283f5;
inline constexpr u32 kTh07BonusToastSupernaturalScaleLoadAddress = 0x00428414;
inline constexpr u32 kTh07BonusToastSupernaturalRenderModeWriteAddress = 0x00428428;
inline constexpr u32 kTh07BonusToastSupernaturalColorWriteAddress = 0x00428432;
inline constexpr u32 kTh07BonusToastSupernaturalStringPushAddress = 0x0042843c;
inline constexpr u32 kTh07BonusToastSupernaturalPositionAddAddress = 0x00428447;
inline constexpr u32 kTh07BonusToastSupernaturalRenderModeResetAddress = 0x00428471;
inline constexpr u32 kTh07BonusToastCherryPointMaxColorWriteAddress = 0x00428480;
inline constexpr u32 kTh07BonusToastCherryPointMaxStringPushAddress = 0x0042848a;
inline constexpr u32 kTh07BonusToastCherryPointMaxPositionAddAddress = 0x00428495;
inline constexpr u32 kTh07BonusToastBorderBonusScaleLoadAddress = 0x004284b1;
inline constexpr u32 kTh07BonusToastBorderBonusRenderModeWriteAddress = 0x004284c5;
inline constexpr u32 kTh07BonusToastBorderBonusColorWriteAddress = 0x004284cf;
inline constexpr u32 kTh07BorderBonusScorePushAddress = 0x004284df;
inline constexpr u32 kTh07BonusToastBorderBonusStringPushAddress = 0x004284e5;
inline constexpr u32 kTh07BonusToastBorderBonusPositionAddAddress = 0x004284f0;
inline constexpr u32 kTh07BonusToastBorderBonusRenderModeResetAddress = 0x0042851a;
inline constexpr u32 kTh07SpellCardBonusVisibleCheckAddress = 0x0042852a;
inline constexpr u32 kTh07SpellCardBonusPositionWriteAddress = 0x00428565;
inline constexpr u32 kTh07SpellCardBonusLineYWriteAddress = 0x00428577;
inline constexpr u32 kTh07SpellCardBonusCaptureIdPushAddress = 0x004285bf;

inline constexpr u32 kTh07StageClearStringAddress = 0x00498518;
inline constexpr u32 kTh07AllClearStringAddress = 0x0049850c;
inline constexpr u32 kTh07ClearBonusClearStringAddress = 0x004984fc;
inline constexpr u32 kTh07ClearBonusPointStringAddress = 0x004984ec;
inline constexpr u32 kTh07ClearBonusGrazeStringAddress = 0x004984dc;
inline constexpr u32 kTh07ClearBonusCherryStringAddress = 0x004984cc;
inline constexpr u32 kTh07ClearBonusPlayerStringAddress = 0x004984c0;
inline constexpr u32 kTh07ClearBonusBombStringAddress = 0x004984b0;
inline constexpr u32 kTh07ClearBonusTotalStringAddress = 0x00498400;
inline constexpr u32 kTh07ClearBonusSpellCardStringAddress = 0x0049838c;
inline constexpr u32 kTh07FullPowerModeStringAddress = 0x004983e0;
inline constexpr u32 kTh07BonusToastPositionXStartConstantAddress = 0x00498a74;
inline constexpr u32 kTh07BonusToastPositionYStartConstantAddress = 0x00498b30;
inline constexpr u32 kTh07BonusToastSlideDeltaConstantAddress = 0x00498cb8;
inline constexpr u32 kTh07BonusToastSlideDivisorConstantAddress = 0x00498b00;
inline constexpr u32 kTh07BonusToastPositionXFinalConstantAddress = 0x00498bb8;

enum class BonusToastKind
{
    None = 0,
    FullPowerMode = 1,
    SupernaturalBorder = 2,
    CherryPointMax = 3,
    BorderBonus = 4,
};

enum class ClearBonusOwnerFieldKind
{
    ClearSummaryVisible,
    ClearSummaryTotalScore,
    BonusPopupPosition,
    BonusPopupScore,
    BonusPopupVisible,
    BonusToastPosition,
    BorderBonusScore,
    BonusToastKind,
    SpellCardBonusPosition,
    SpellCardBonusLineY,
    SpellCardBonusCaptureId,
    SpellCardBonusVisible,
    PointItemsInStage,
    Cherry,
    GrazeInStage,
};

struct RankBonusRule
{
    Th07Difficulty difficulty;
    const char *label;
    float multiplier;
    u32 ghidraStringAddress;
};

struct BonusToastRule
{
    BonusToastKind kind;
    const char *label;
    u32 ghidraStringAddress;
    u32 stringPushAddress;
    u32 positionAddAddress;
    u32 colorBits;
    bool usesBorderBonusScore;
    bool usesScaledRenderState;
};

struct BonusToastLifecycleContract
{
    u32 registerFunctionAddress;
    u32 updateFunctionAddress;
    u32 initKindClearAddress;
    u32 positionOffset;
    u32 scoreOffset;
    u32 kindOffset;
    u32 timerOffset;
    int timerPreviousInitial;
    int slideFrames;
    int lifetimeFrames;
};

struct BonusToastProducerCall
{
    BonusToastKind kind;
    const char *sourceFunction;
    u32 sourceFunctionAddress;
    u32 kindPushAddress;
    u32 scorePushAddress;
    u32 callAddress;
    bool scoreIsImmediate;
    int scoreImmediate;
    const char *scoreSource;
};

struct ClearBonusOwnerField
{
    ClearBonusOwnerFieldKind kind;
    u32 offset;
    u32 firstUseAddress;
    const char *name;
};

inline constexpr std::size_t kTh07RankBonusRuleCount = 6;
inline constexpr std::size_t kTh07BonusToastRuleCount = 4;
inline constexpr std::size_t kTh07BonusToastProducerCallCount = 7;
inline constexpr std::size_t kTh07ClearBonusOwnerFieldCount = 15;
inline constexpr u32 kTh07ClearBonusSummaryVisibleOffset = 0x209b4;
inline constexpr u32 kTh07ClearBonusTotalScoreOffset = 0x209b8;
inline constexpr u32 kTh07BonusPopupPositionOffset = 0x209c0;
inline constexpr u32 kTh07BonusPopupScoreOffset = 0x209cc;
inline constexpr u32 kTh07BonusPopupVisibleOffset = 0x209d0;
inline constexpr u32 kTh07BonusToastPositionOffset = 0x209e0;
inline constexpr u32 kTh07BonusToastPositionYOffset = 0x209e4;
inline constexpr u32 kTh07BonusToastPositionZOffset = 0x209e8;
inline constexpr u32 kTh07BorderBonusScoreOffset = 0x209ec;
inline constexpr u32 kTh07BonusToastKindOffset = 0x209f0;
inline constexpr u32 kTh07BonusToastTimerOffset = 0x209f4;
inline constexpr u32 kTh07BonusToastTimerPreviousOffset = 0x209f4;
inline constexpr u32 kTh07BonusToastTimerSubframeOffset = 0x209f8;
inline constexpr u32 kTh07BonusToastTimerCurrentOffset = 0x209fc;
inline constexpr u32 kTh07SpellCardBonusPositionOffset = 0x20a00;
inline constexpr u32 kTh07SpellCardBonusLineYOffset = 0x20a04;
inline constexpr u32 kTh07SpellCardBonusCaptureIdOffset = 0x20a0c;
inline constexpr u32 kTh07SpellCardBonusVisibleOffset = 0x20a10;
inline constexpr u32 kTh07ClearBonusPointItemsOffset = 0x20a24;
inline constexpr u32 kTh07ClearBonusCherryOffset = 0x20a28;
inline constexpr u32 kTh07ClearBonusGrazeOffset = 0x20a2c;
constexpr int kStageClearBonusPerStage = 1000000;
constexpr int kPointItemClearBonus = 50000;
constexpr int kGrazeClearBonus = 500;
constexpr int kCherryClearBonus = 10;
constexpr int kLifeClearBonus = 20000000;
constexpr int kBombClearBonus = 4000000;
constexpr float kPlayerPenaltyHalf = 0.5f;
constexpr float kPlayerPenaltyLow = 0.2f;
constexpr u32 kTh07BonusToastColorWarmBits = 0xffc0b0ff;
constexpr u32 kTh07BonusToastColorBorderBits = 0xffe0b0ff;
constexpr int kTh07BonusToastScaledRenderMode = 0x0b;
constexpr int kTh07BonusToastDefaultRenderMode = 0x0e;
constexpr int kTh07BonusToastTimerInitialPrevious = -999;
constexpr int kTh07BonusToastSlideFrames = 30;
constexpr int kTh07BonusToastLifetimeFrames = 180;
constexpr int kTh07BonusToastRegisterGlobalState = 2;

extern const std::array<RankBonusRule, kTh07RankBonusRuleCount> kRankBonusRules;
extern const std::array<BonusToastRule, kTh07BonusToastRuleCount> kTh07BonusToastRules;
extern const BonusToastLifecycleContract kTh07BonusToastLifecycleContract;
extern const std::array<BonusToastProducerCall, kTh07BonusToastProducerCallCount>
    kTh07BonusToastProducerCalls;
extern const std::array<ClearBonusOwnerField, kTh07ClearBonusOwnerFieldCount> kTh07ClearBonusOwnerFields;

int StageClearBonus(int currentStage);
int PointItemClearBonus(int pointItemsInStage);
int GrazeClearBonus(int grazeInStage);
int CherryClearBonus(int cherry);
int LifeClearBonus(int livesRemaining);
int BombClearBonus(int bombsRemaining);
bool UsesTh07BonusToastSlidePath(int currentFrame);
bool HasTh07BonusToastExpired(int currentFrame);
std::size_t CountTh07BonusToastProducerCalls(BonusToastKind kind);
const BonusToastRule *FindTh07BonusToastRule(BonusToastKind kind);
const ClearBonusOwnerField *FindTh07ClearBonusOwnerField(ClearBonusOwnerFieldKind kind);
} // namespace th07
