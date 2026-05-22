#include "PlayerBombTables.hpp"

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>

namespace th07 {

namespace {

std::int32_t RoundDownToMultipleOfTen(std::int32_t value)
{
    return value - value % 10;
}

std::int32_t TruncateX87PositiveScoreProduct(std::int32_t scoreDelta, float scoreDrainFactor)
{
    const long double product =
        static_cast<long double>(scoreDelta) * static_cast<long double>(scoreDrainFactor);
    return static_cast<std::int32_t>(product);
}

std::uint32_t PlayerBombBackdropColorFromChannel(std::uint8_t channel)
{
    return (static_cast<std::uint32_t>(kPlayerBombBackdropAlpha) << 24) |
           (static_cast<std::uint32_t>(channel) << 16) |
           (static_cast<std::uint32_t>(channel) << 8) |
           static_cast<std::uint32_t>(channel);
}

} // namespace

const std::array<PlayerBombRoutineSet, 6> kPlayerBombRoutineSets = {{
    {Th07ShotType::ReimuA,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x00408710, "FUN_00408710", 0x4a1, 140, 200, 0x004087a7, 0x004087bd, 4000, 0x00408879, 0x00408874, 0x3e4ccccd, 0x3f800000, 0, 0},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x00408e10, "FUN_00408e10", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x004091b0, "FUN_004091b0", 0x4a1, 300, 360, 0x00409266, 0x0040927f, 5000, 0x00409326, 0x00409321, 0x3e6147ae, 0x3f19999a, 0x0040933c, 0x0040934c},
         {PlayerBombRoutineSlot::FocusedDraw, 0x00409990, "FUN_00409990", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     }}},
    {Th07ShotType::ReimuB,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x00409dd0, "FUN_00409dd0", 0x4a1, 140, 200, 0x00409e6d, 0x00409e86, 3000, 0x00409fc8, 0x00409fc3, 0x3e2e147b, 0x3f800000, 0, 0},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x0040a280, "FUN_0040a280", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x0040a3a0, "FUN_0040a3a0", 0x4a1, 190, 250, 0x0040a456, 0x0040a46e, 3000, 0x0040a523, 0x0040a51e, 0x3e2e147b, 0x3ecccccd, 0x0040a533, 0x0040a540},
         {PlayerBombRoutineSlot::FocusedDraw, 0x0040a6b0, "FUN_0040a6b0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     }}},
    {Th07ShotType::MarisaA,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x0040a7c0, "FUN_0040a7c0", 0x4a3, 200, 250, 0x0040a85c, 0x0040a875, 8000, 0x0040a997, 0x0040a992, 0x3e99999a, 0x3f800000, 0, 0},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x0040aba0, "FUN_0040aba0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x0040af10, "FUN_0040af10", 0x4a2, 260, 310, 0x0040afe4, 0x0040afff, 9000, 0x0040b05d, 0x0040b058, 0x3ea8f5c3, 0x3ecccccd, 0x0040b073, 0x0040b083},
         {PlayerBombRoutineSlot::FocusedDraw, 0x0040b5d0, "FUN_0040b5d0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     }}},
    {Th07ShotType::MarisaB,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x0040b7d0, "FUN_0040b7d0", 0x4a1, 300, 300, 0x0040b8a7, 0x0040b8c0, 8000, 0x0040b999, 0x0040b994, 0x3eb33333, 0x3ecccccd, 0x0040b97d, 0x0040b98a},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x0040bca0, "FUN_0040bca0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x0040be20, "FUN_0040be20", 0x4a2, 340, 390, 0x0040bed6, 0x0040beee, 10000, 0x0040bfb2, 0x0040bfad, 0x3ed1eb85, 0x3e4ccccd, 0x0040bf96, 0x0040bfa3},
         {PlayerBombRoutineSlot::FocusedDraw, 0x0040c160, "FUN_0040c160", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     }}},
    {Th07ShotType::SakuyaA,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x0040c2e0, "FUN_0040c2e0", 0x4a1, 160, 210, 0x0040c39b, 0x0040c3b7, 6000, 0x0040c445, 0x0040c440, 0x3e8f5c29, 0x3f800000, 0, 0},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x0040c970, "FUN_0040c970", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x0040ca50, "FUN_0040ca50", 0x4a1, 250, 290, 0x0040cb2b, 0x0040cb47, 6500, 0x0040cbaf, 0x0040cbaa, 0x3e947ae1, 0x3e99999a, 0x0040cbc5, 0x0040cbd5},
         {PlayerBombRoutineSlot::FocusedDraw, 0x0040d3b0, "FUN_0040d3b0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     }}},
    {Th07ShotType::SakuyaB,
     {{
         {PlayerBombRoutineSlot::UnfocusedCalc, 0x0040d4c0, "FUN_0040d4c0", 0x4a3, 160, 260, 0x0040d594, 0x0040d5ad, 5500, 0x0040d61c, 0x0040d617, 0x3e851eb8, 0x40000000, 0x0040d62c, 0x0040d639},
         {PlayerBombRoutineSlot::UnfocusedDraw, 0x0040d9a0, "FUN_0040d9a0", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {PlayerBombRoutineSlot::FocusedCalc, 0x0040da80, "FUN_0040da80", 0x4a3, 300, 420, 0x0040dbc1, 0x0040dbdc, 6000, 0x0040dd59, 0x0040dd54, 0x3e947ae1, 0x3fc00000, 0x0040dd2c, 0x0040dd3c},
         {PlayerBombRoutineSlot::FocusedDraw, 0x0040e280, "FUN_0040e280", -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }}},
}};

const std::array<PlayerStageColorWriteProducer, 3> kPlayerStageColorWriteProducers = {{
    {PlayerStageColorWriteKind::CommonBombBackdrop, kPlayerBombCommonDrawBackdropAddress,
     kPlayerStageColorWriteCommonBackdropCallAddress, "FUN_004083f0"},
    {PlayerStageColorWriteKind::Mode4Update, 0x00441330, kPlayerStageColorWriteMode4UpdateCallAddress,
     "FUN_00441330"},
    {PlayerStageColorWriteKind::Mode4Draw, 0x004420b0, kPlayerStageColorWriteMode4DrawCallAddress,
     "FUN_004420b0"},
}};

const std::array<PlayerModeEffectLifecycleContract, kPlayerModeEffectLifecycleContractCount>
    kPlayerModeEffectLifecycleContracts = {{
        {PlayerModeEffectLifecycleKind::ReplaceTransitionEffect,
         PlayerModeEffectPointerSlot::TransitionEffect,
         kPlayerModeEnterMode4FunctionAddress,
         kPlayerModeTransitionEffectActivePointerOffset,
         0x00441a5e,
         kPlayerModeEffectLifecycleNoInstruction,
         0x00441aa4,
         0x00441b74,
         kPlayerModeTransitionEffectId,
         kPlayerModeTransitionEffectSpawnSlotArgument,
         kPlayerModeTransitionEffectSpawnFlagArgument,
         "FUN_00441960: kills previous transition active flag, spawns 0x1c, stores +0xb7e6c"},
        {PlayerModeEffectLifecycleKind::ClearCommonEffect,
         PlayerModeEffectPointerSlot::CommonEffect,
         kPlayerModeEnterMode4FunctionAddress,
         kPlayerBombCommonEffectActivePointerOffset,
         0x00441a7a,
         0x00441a84,
         kPlayerModeEffectLifecycleNoInstruction,
         kPlayerModeEffectLifecycleNoInstruction,
         kPlayerModeEffectLifecycleNoEffectId,
         kPlayerModeEffectLifecycleNoEffectId,
         kPlayerModeEffectLifecycleNoEffectId,
         "FUN_00441960: kills common effect active flag and clears +0xb7e68"},
        {PlayerModeEffectLifecycleKind::ReplaceTransitionEffect,
         PlayerModeEffectPointerSlot::TransitionEffect,
         kPlayerModeEnterMode3FunctionAddress,
         kPlayerModeTransitionEffectActivePointerOffset,
         0x00441bee,
         0x00441bf8,
         0x00441c19,
         0x00441d35,
         kPlayerModeTransitionEffectId,
         kPlayerModeTransitionEffectSpawnSlotArgument,
         kPlayerModeTransitionEffectSpawnFlagArgument,
         "FUN_00441bd0: clears transition pointer, spawns 0x1c, stores +0xb7e6c"},
        {PlayerModeEffectLifecycleKind::ClearTransitionEffect,
         PlayerModeEffectPointerSlot::TransitionEffect,
         kPlayerModeCleanupFunctionAddress,
         kPlayerModeTransitionEffectActivePointerOffset,
         0x004417e1,
         0x004417eb,
         kPlayerModeEffectLifecycleNoInstruction,
         kPlayerModeEffectLifecycleNoInstruction,
         kPlayerModeEffectLifecycleNoEffectId,
         kPlayerModeEffectLifecycleNoEffectId,
         kPlayerModeEffectLifecycleNoEffectId,
         "FUN_00441670: kills transition active flag and clears +0xb7e6c"},
    }};

const std::array<PlayerModeRuntimeBranchContract, kPlayerModeRuntimeBranchContractCount>
    kPlayerModeRuntimeBranchContracts = {{
        {PlayerModeRuntimeBranchKind::UpdateMode3,
         kPlayerModeUpdateFunctionAddress,
         kPlayerModeState3,
         0x00441370,
         kPlayerBombCommonEffectXOffset,
         0x004413b7,
         kTh07TimerDecrementFunctionAddress,
         0x004413c8,
         kPlayerBombCommonEffectActivePointerOffset,
         0x0044137c,
         0x0044145c,
         0x0044146b,
         0x004413fa,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         "FUN_00441330 mode 3: follows common effect, decrements +0x16a00 timer, expires to idle"},
        {PlayerModeRuntimeBranchKind::UpdateMode4,
         kPlayerModeUpdateFunctionAddress,
         kPlayerModeState4,
         0x00441484,
         kPlayerBombCommonEffectXOffset,
         0x0044151f,
         kTh07TimerDecrementFunctionAddress,
         0x00441530,
         kPlayerModeTransitionEffectActivePointerOffset,
         0x00441490,
         0x00441574,
         0x00441583,
         0x00441546,
         kPlayerModeCleanupFunctionAddress,
         kPlayerStageColorWriteMode4UpdateCallAddress,
         "FUN_00441330 mode 4: follows transition effect, updates score gauge, expires through cleanup"},
        {PlayerModeRuntimeBranchKind::UpdateDefault,
         kPlayerModeUpdateFunctionAddress,
         kPlayerModeRuntimeDefaultBranchState,
         0x00441636,
         kPlayerBombCommonEffectXOffset,
         0x00441660,
         kTh07TickTimerFunctionAddress,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoOwnerOffset,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         0x0044164b,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         "FUN_00441330 default: mirrors +0x16a08 into timer previous then ticks +0x16a08/+0x16a04"},
        {PlayerModeRuntimeBranchKind::DrawMode4,
         kPlayerModeDrawFunctionAddress,
         kPlayerModeState4,
         0x0044223d,
         kPlayerModeRuntimeNoOwnerOffset,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         0x00442252,
         kPlayerModeRuntimeNoOwnerOffset,
         kPlayerModeRuntimeNoInstruction,
         0x00442280,
         0x0044228f,
         kPlayerModeRuntimeNoInstruction,
         kPlayerModeRuntimeNoInstruction,
         kPlayerStageColorWriteMode4DrawCallAddress,
         "FUN_004420b0 draw mode 4: duration-gated blink color and stage-color producer"},
    }};

float PlayerBombRoutineTarget::scoreDrainFactor() const
{
    return std::bit_cast<float>(scoreDrainFactorBits);
}

float PlayerBombRoutineTarget::speedMultiplier() const
{
    return std::bit_cast<float>(speedMultiplierBits);
}

const PlayerBombRoutineSet *FindPlayerBombRoutineSet(Th07ShotType shotType)
{
    for (const PlayerBombRoutineSet &set : kPlayerBombRoutineSets) {
        if (set.shotType == shotType) {
            return &set;
        }
    }
    return nullptr;
}

const PlayerBombRoutineTarget *FindPlayerBombRoutine(Th07ShotType shotType, PlayerBombRoutineSlot slot)
{
    const PlayerBombRoutineSet *set = FindPlayerBombRoutineSet(shotType);
    if (set == nullptr) {
        return nullptr;
    }

    const std::size_t slotIndex = static_cast<std::size_t>(slot);
    return slotIndex < set->routines.size() ? &set->routines[slotIndex] : nullptr;
}

const PlayerBombRoutineTarget *FindPlayerBombRoutine(Th07ShotType shotType, bool focused, bool draw)
{
    if (draw) {
        return FindPlayerBombRoutine(
            shotType, focused ? PlayerBombRoutineSlot::FocusedDraw : PlayerBombRoutineSlot::UnfocusedDraw);
    }

    return FindPlayerBombRoutine(
        shotType, focused ? PlayerBombRoutineSlot::FocusedCalc : PlayerBombRoutineSlot::UnfocusedCalc);
}

const PlayerStageColorWriteProducer *FindPlayerStageColorWriteProducer(PlayerStageColorWriteKind kind)
{
    for (const PlayerStageColorWriteProducer &producer : kPlayerStageColorWriteProducers) {
        if (producer.kind == kind) {
            return &producer;
        }
    }
    return nullptr;
}

const PlayerModeEffectLifecycleContract *FindPlayerModeEffectLifecycleContract(
    PlayerModeEffectLifecycleKind kind, std::uint32_t functionAddress,
    PlayerModeEffectPointerSlot pointerSlot)
{
    for (const PlayerModeEffectLifecycleContract &contract : kPlayerModeEffectLifecycleContracts) {
        if (contract.kind == kind &&
            contract.functionAddress == functionAddress &&
            contract.pointerSlot == pointerSlot) {
            return &contract;
        }
    }
    return nullptr;
}

const PlayerModeRuntimeBranchContract *FindPlayerModeRuntimeBranchContract(
    PlayerModeRuntimeBranchKind kind)
{
    for (const PlayerModeRuntimeBranchContract &contract : kPlayerModeRuntimeBranchContracts) {
        if (contract.kind == kind) {
            return &contract;
        }
    }
    return nullptr;
}

PlayerBombCommonEffectConfig BuildPlayerBombCommonEffectConfig(std::uint32_t targetXBits,
                                                               std::uint32_t targetYBits,
                                                               std::int32_t durationFrames)
{
    return {
        kPlayerBombCommonEffectId,
        kPlayerBombCommonEffectSpawnSlotArgument,
        kPlayerBombCommonEffectSpawnFlagArgument,
        kPlayerBombCommonEffectInitialColor,
        kPlayerBombCommonEffectAnmScript,
        kPlayerBombCommonEffectUpdateCallbackAddress,
        kPlayerBombCommonEffectInitCallbackAddress,
        targetXBits,
        targetYBits,
        durationFrames,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerBombCommonEffectResetZBits,
        kPlayerBombCommonEffectInterpScaleBits,
        kPlayerBombCommonEffectPostSpawnMultiplierBits,
        kPlayerBombCommonEffectTintColor,
    };
}

PlayerModeTransitionEffectConfig BuildPlayerMode4TransitionEffectConfig()
{
    return {
        kPlayerModeTransitionEffectId,
        kPlayerModeTransitionEffectSpawnSlotArgument,
        kPlayerModeTransitionEffectSpawnFlagArgument,
        kPlayerModeTransitionEffectInitialColor,
        kPlayerMode4CommonEffectDurationFrames,
        kPlayerMode4CommonEffectDurationFrames,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerMode4EffectInitialScaleBits,
        kPlayerMode4EffectFinalScaleBits,
        kPlayerModeTransitionEffectActivePointerOffset,
    };
}

PlayerModeTransitionEffectConfig BuildPlayerMode3TransitionEffectConfig()
{
    return {
        kPlayerModeTransitionEffectId,
        kPlayerModeTransitionEffectSpawnSlotArgument,
        kPlayerModeTransitionEffectSpawnFlagArgument,
        kPlayerModeTransitionEffectInitialColor,
        kPlayerMode3CommonEffectDurationFrames,
        kPlayerMode3EffectInterpolationDurationFrames,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerMode3EffectInitialScaleBits,
        kPlayerMode3EffectFinalScaleBits,
        kPlayerModeTransitionEffectActivePointerOffset,
    };
}

PlayerInitialModeOwnerState BuildPlayerInitialModeOwnerState()
{
    return {
        kPlayerModeState1,
        kPlayerOptionStateUnfocused,
        0,
        kPlayerModeTransitionRequestIdle,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerInitialCommonEffectDurationFrames,
    };
}

PlayerMode4EntryOwnerState BuildPlayerMode4EntryOwnerState()
{
    return {
        kPlayerModeState4,
        kPlayerModeTransitionRequestStartMode4,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerMode4CommonEffectDurationFrames,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerMode4CommonEffectDurationFrames,
    };
}

PlayerMode3EntryOwnerState BuildPlayerMode3EntryOwnerState()
{
    return {
        kPlayerModeState3,
        kPlayerModeTransitionRequestIdle,
        kPlayerBombCommonEffectResetXBits,
        kPlayerBombCommonEffectResetYBits,
        kPlayerMode3CommonEffectDurationFrames,
        kPlayerMode3CommonEffectDurationFrames,
    };
}

std::uint32_t ComputePlayerBombBackdropColor(std::int32_t timerFrame, std::int32_t durationFrames)
{
    std::int32_t channel = kPlayerBombBackdropPlateauChannel;
    if (timerFrame < kPlayerBombBackdropFadeFrames) {
        channel = kPlayerBombBackdropAlpha -
                  (timerFrame * kPlayerBombBackdropFadeDelta) / kPlayerBombBackdropFadeFrames;
    }
    else if (timerFrame >= durationFrames - kPlayerBombBackdropFadeFrames) {
        channel = kPlayerBombBackdropAlpha -
                  ((durationFrames - timerFrame) * kPlayerBombBackdropFadeDelta) /
                      kPlayerBombBackdropFadeFrames;
    }

    return PlayerBombBackdropColorFromChannel(static_cast<std::uint8_t>(channel));
}

std::uint32_t ComputePlayerMode4StageColor(std::int32_t mode4Frame)
{
    std::int32_t channel = kPlayerMode4StageColorPlateauChannel;
    if (mode4Frame < kPlayerMode4StageColorFadeInFrames) {
        channel = kPlayerBombBackdropAlpha -
                  (mode4Frame * kPlayerMode4StageColorFadeDelta) /
                      kPlayerMode4StageColorFadeInFrames;
    }
    else if (mode4Frame >= kPlayerMode4StageColorFadeOutStartFrame) {
        channel = kPlayerBombBackdropAlpha -
                  ((kPlayerMode4StageColorTotalFrames - mode4Frame) *
                   kPlayerMode4StageColorFadeDelta) /
                      kPlayerMode4StageColorFadeInFrames;
    }

    return PlayerBombBackdropColorFromChannel(static_cast<std::uint8_t>(channel));
}

std::int32_t ApplyPlayerBombScoreDrainDifficultyScale(std::int32_t scoreDrain, Th07Difficulty difficulty)
{
    switch (difficulty) {
    case Th07Difficulty::Hard:
        return scoreDrain / 2;
    case Th07Difficulty::Lunatic:
        return scoreDrain / 4;
    case Th07Difficulty::Extra:
    case Th07Difficulty::Phantasm:
        return scoreDrain / 3;
    case Th07Difficulty::Easy:
    case Th07Difficulty::Normal:
        return scoreDrain;
    }

    return scoreDrain;
}

std::int32_t ComputePlayerBombScoreDrainStep(std::int32_t scoreDelta, Th07Difficulty difficulty,
                                             std::int32_t durationFrames, float scoreDrainFactor,
                                             std::int32_t minimumScoreDrainTotal)
{
    assert(durationFrames > 0);
    const std::int32_t scaledScoreDrain = ApplyPlayerBombScoreDrainDifficultyScale(
        TruncateX87PositiveScoreProduct(scoreDelta, scoreDrainFactor), difficulty);
    const std::int32_t scoreStep = RoundDownToMultipleOfTen(scaledScoreDrain / durationFrames);
    const std::int32_t minimumStep = RoundDownToMultipleOfTen(minimumScoreDrainTotal / durationFrames);
    return std::max(scoreStep, minimumStep);
}

std::int32_t ComputePlayerBombScoreDrainStep(std::int32_t scoreDelta, Th07Difficulty difficulty,
                                             const PlayerBombRoutineTarget &routine)
{
    return ComputePlayerBombScoreDrainStep(scoreDelta, difficulty, routine.durationFrames,
                                           routine.scoreDrainFactor(), routine.minimumScoreDrainTotal);
}

std::string_view PlayerBombRoutineSlotName(PlayerBombRoutineSlot slot)
{
    switch (slot) {
    case PlayerBombRoutineSlot::UnfocusedCalc:
        return "unfocused calc";
    case PlayerBombRoutineSlot::UnfocusedDraw:
        return "unfocused draw";
    case PlayerBombRoutineSlot::FocusedCalc:
        return "focused calc";
    case PlayerBombRoutineSlot::FocusedDraw:
        return "focused draw";
    }

    return "unknown";
}

} // namespace th07
