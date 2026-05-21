#pragma once

#include "PlayerLayout.hpp"
#include "Th07Constants.hpp"

#include <array>
#include <cstdint>
#include <string_view>

namespace th07 {

inline constexpr std::uint32_t kPlayerBombRoutineTableAddress = 0x0049ec50;
inline constexpr std::uint32_t kPlayerBombUpdateUnfocusedCallAddress = 0x00440b18;
inline constexpr std::uint32_t kPlayerBombUpdateFocusedCallAddress = 0x00440b26;
inline constexpr std::uint32_t kPlayerBombStartupUnfocusedCallAddress = 0x00440c59;
inline constexpr std::uint32_t kPlayerBombStartupFocusedCallAddress = 0x00440c67;
inline constexpr std::uint32_t kPlayerBombDrawUnfocusedCallAddress = 0x004420df;
inline constexpr std::uint32_t kPlayerBombDrawFocusedCallAddress = 0x004420ed;

inline constexpr std::uint32_t kPlayerBombCommonDrawBackdropAddress = 0x004083f0;
inline constexpr std::uint32_t kPlayerBombCommonStartEffectAddress = 0x004084f0;
inline constexpr std::uint32_t kPlayerBombCommonScoreDrainAddress = 0x00408610;

inline constexpr std::uint32_t kPlayerBombActiveOffset = 0x16a20;
inline constexpr std::uint32_t kPlayerBombCapturedFocusOffset = 0x16a24;
inline constexpr std::uint32_t kPlayerBombDurationOffset = 0x16a28;
inline constexpr std::uint32_t kPlayerBombScoreDrainStepOffset = 0x16a2c;
inline constexpr std::uint32_t kPlayerBombLastProcessedFrameOffset = 0x16a30;
inline constexpr std::uint32_t kPlayerBombTimerSubframeOffset = 0x16a34;
inline constexpr std::uint32_t kPlayerBombTimerFrameOffset = 0x16a38;
inline constexpr std::int32_t kPlayerBombStartupFallbackDurationFrames = 999;
inline constexpr std::int32_t kPlayerBombPowerBonusOnStart = 6;
inline constexpr std::uint32_t kPlayerBombDefaultSpeedMultiplierBits = 0x3f800000;
inline constexpr std::int32_t kPlayerBombBackdropFadeFrames = 0x3c;
inline constexpr std::int32_t kPlayerBombBackdropFadeDelta = 0x50;
inline constexpr std::uint8_t kPlayerBombBackdropAlpha = 0x80;
inline constexpr std::uint8_t kPlayerBombBackdropPlateauChannel = 0x30;
inline constexpr std::uint32_t kPlayerBombBackdropBaseColor = 0x80808080;
inline constexpr std::uint32_t kPlayerBombBackdropPlateauColor = 0x80303030;
inline constexpr std::uint32_t kPlayerStageColorWriteCommonBackdropCallAddress = 0x004084d0;
inline constexpr std::uint32_t kPlayerStageColorWriteMode4UpdateCallAddress = 0x0044162f;
inline constexpr std::uint32_t kPlayerStageColorWriteMode4DrawCallAddress = 0x0044233b;
inline constexpr std::uint32_t kPlayerModeCleanupFunctionAddress = 0x00441670;
inline constexpr std::uint32_t kPlayerModeEnterMode4FunctionAddress = 0x00441960;
inline constexpr std::uint32_t kPlayerModeEnterMode3FunctionAddress = 0x00441bd0;
inline constexpr std::uint32_t kPlayerMode4StateOffset = kPlayerModeStateOffset;
inline constexpr std::uint32_t kPlayerMode4DrawEnabledOffset = kPlayerModeDrawEnabledOffset;
inline constexpr std::uint8_t kPlayerModeStateIdle = 0;
inline constexpr std::uint8_t kPlayerModeState1 = 1;
inline constexpr std::uint8_t kPlayerModeState2 = 2;
inline constexpr std::uint8_t kPlayerModeState3 = 3;
inline constexpr std::uint8_t kPlayerModeState4 = 4;
inline constexpr std::uint8_t kPlayerModeTransitionRequestIdle = 0;
inline constexpr std::uint8_t kPlayerModeTransitionRequestStartMode4 = 1;
inline constexpr std::uint8_t kPlayerModeTransitionRequestCleanup = 2;
inline constexpr std::uint32_t kPlayerMode4StageColorFrameGlobalAddress =
    kPlayerBombCommonEffectDurationGlobalAddress;
inline constexpr std::int32_t kPlayerMode4CommonEffectDurationFrames = 0x21c;
inline constexpr std::int32_t kPlayerMode3CommonEffectDurationFrames = 0x28;
inline constexpr std::int32_t kPlayerMode3EffectInterpolationDurationFrames = 0x1e;
inline constexpr std::int32_t kPlayerMode1CleanupCommonEffectDurationFrames = 0xf0;
inline constexpr std::int32_t kPlayerInitialCommonEffectDurationFrames = 0x78;
inline constexpr std::int32_t kPlayerMode4StageColorFadeInFrames = 0x1e;
inline constexpr std::int32_t kPlayerMode4StageColorFadeOutStartFrame = 0x1fe;
inline constexpr std::int32_t kPlayerMode4StageColorTotalFrames = 0x21c;
inline constexpr std::int32_t kPlayerMode4StageColorFadeDelta = 0x50;
inline constexpr std::uint8_t kPlayerMode4StageColorPlateauChannel = 0x30;
inline constexpr std::uint32_t kPlayerMode4StageColorPlateauColor = 0x80303030;
inline constexpr std::uint32_t kPlayerBombCommonEffectSpawnFunctionAddress = 0x0041c610;
inline constexpr std::uint32_t kPlayerBombCommonEffectManagerAddress = 0x012fe250;
inline constexpr std::uint32_t kPlayerBombCommonEffectTableAddress = 0x0049efc0;
inline constexpr std::int32_t kPlayerBombCommonEffectId = 0x19;
inline constexpr std::uint32_t kPlayerBombCommonEffectAnmScript = 0x02da;
inline constexpr std::uint32_t kPlayerBombCommonEffectUpdateCallbackAddress = 0x0041c1b0;
inline constexpr std::uint32_t kPlayerBombCommonEffectInitCallbackAddress = 0;
inline constexpr std::int32_t kPlayerBombCommonEffectSpawnSlotArgument = 0;
inline constexpr std::int32_t kPlayerBombCommonEffectSpawnFlagArgument = 1;
inline constexpr std::uint32_t kPlayerBombCommonEffectInitialColor = 0xffffffff;
inline constexpr std::uint32_t kPlayerBombCommonEffectTintColor = 0xffff4040;
inline constexpr std::uint32_t kPlayerBombCommonEffectResetXBits = 0xfffffc19;
inline constexpr std::uint32_t kPlayerBombCommonEffectResetYBits = 0;
inline constexpr std::uint32_t kPlayerBombCommonEffectResetZBits = 0;
inline constexpr std::uint32_t kPlayerBombCommonEffectInterpScaleBits = 0x3d800000;
inline constexpr std::uint32_t kPlayerBombCommonEffectPostSpawnMultiplierBits = 0xbf800000;
inline constexpr std::int32_t kPlayerModeTransitionEffectId = 0x1c;
inline constexpr std::int32_t kPlayerModeTransitionEffectSpawnSlotArgument = 4;
inline constexpr std::int32_t kPlayerModeTransitionEffectSpawnFlagArgument = 1;
inline constexpr std::uint32_t kPlayerModeTransitionEffectInitialColor = 0xffffffff;
inline constexpr std::uint32_t kPlayerMode4EffectInitialScaleBits = 0x3f800000;
inline constexpr std::uint32_t kPlayerMode4EffectFinalScaleBits = 0x3e800000;
inline constexpr std::uint32_t kPlayerMode3EffectInitialScaleBits = 0x3d800000;
inline constexpr std::uint32_t kPlayerMode3EffectFinalScaleBits = 0x3fa66666;
struct PlayerBombCommonEffectConfig {
    std::int32_t effectId;
    std::int32_t spawnSlotArgument;
    std::int32_t spawnFlagArgument;
    std::uint32_t initialColor;
    std::uint32_t anmScript;
    std::uint32_t updateCallbackAddress;
    std::uint32_t initCallbackAddress;
    std::uint32_t targetXBits;
    std::uint32_t targetYBits;
    std::int32_t durationFrames;
    std::uint32_t resetXBits;
    std::uint32_t resetYBits;
    std::uint32_t resetZBits;
    std::uint32_t interpScaleBits;
    std::uint32_t postSpawnMultiplierBits;
    std::uint32_t tintColor;
};

struct PlayerModeTransitionEffectConfig {
    std::int32_t effectId;
    std::int32_t spawnSlotArgument;
    std::int32_t spawnFlagArgument;
    std::uint32_t initialColor;
    std::int32_t commonEffectDurationFrames;
    std::int32_t effectInterpolationDurationFrames;
    std::uint32_t targetXBits;
    std::uint32_t targetYBits;
    std::uint32_t initialScaleBits;
    std::uint32_t finalScaleBits;
    std::uint32_t activePointerOffset;
};

struct PlayerInitialModeOwnerState {
    std::uint8_t modeState;
    std::uint8_t optionState;
    std::uint8_t focusHeld;
    std::uint8_t transitionRequest;
    std::uint32_t commonEffectXBits;
    std::uint32_t commonEffectYBits;
    std::int32_t commonEffectDurationFrames;
};

struct PlayerMode4EntryOwnerState {
    std::uint8_t modeState;
    std::uint8_t transitionRequest;
    std::uint32_t commonEffectXBits;
    std::uint32_t commonEffectYBits;
    std::int32_t commonEffectDurationFrames;
    std::uint32_t effectStartXBits;
    std::uint32_t effectStartYBits;
    std::int32_t effectStartDurationFrames;
};

struct PlayerMode3EntryOwnerState {
    std::uint8_t modeState;
    std::uint8_t transitionRequest;
    std::uint32_t commonEffectXBits;
    std::uint32_t commonEffectYBits;
    std::int32_t commonEffectDurationFrames;
    std::int32_t lockoutFrames;
};

enum class PlayerBombRoutineSlot {
    UnfocusedCalc = 0,
    UnfocusedDraw = 1,
    FocusedCalc = 2,
    FocusedDraw = 3,
};

struct PlayerBombRoutineTarget {
    PlayerBombRoutineSlot slot;
    std::uint32_t originalAddress;
    std::string_view ghidraName;
    std::int32_t portraitAnmScript;
    std::int32_t durationFrames;
    std::int32_t commonEffectDurationFrames;
    std::uint32_t durationWriteAddress;
    std::uint32_t commonEffectDurationWriteAddress;
    std::int32_t minimumScoreDrainTotal;
    std::uint32_t minimumScoreDrainTotalLoadAddress;
    std::uint32_t scoreDrainFactorPushAddress;
    std::uint32_t scoreDrainFactorBits;
    std::uint32_t speedMultiplierBits;
    std::uint32_t verticalSpeedMultiplierWriteAddress;
    std::uint32_t horizontalSpeedMultiplierWriteAddress;

    float scoreDrainFactor() const;
    float speedMultiplier() const;
};

struct PlayerBombRoutineSet {
    Th07ShotType shotType;
    std::array<PlayerBombRoutineTarget, 4> routines;
};

enum class PlayerStageColorWriteKind {
    CommonBombBackdrop,
    Mode4Update,
    Mode4Draw,
};

struct PlayerStageColorWriteProducer {
    PlayerStageColorWriteKind kind;
    std::uint32_t originalFunctionAddress;
    std::uint32_t callInstructionAddress;
    std::string_view ghidraName;
};

extern const std::array<PlayerBombRoutineSet, 6> kPlayerBombRoutineSets;
extern const std::array<PlayerStageColorWriteProducer, 3> kPlayerStageColorWriteProducers;

const PlayerBombRoutineSet *FindPlayerBombRoutineSet(Th07ShotType shotType);
const PlayerBombRoutineTarget *FindPlayerBombRoutine(Th07ShotType shotType, PlayerBombRoutineSlot slot);
const PlayerBombRoutineTarget *FindPlayerBombRoutine(Th07ShotType shotType, bool focused, bool draw);
const PlayerStageColorWriteProducer *FindPlayerStageColorWriteProducer(PlayerStageColorWriteKind kind);
PlayerBombCommonEffectConfig BuildPlayerBombCommonEffectConfig(std::uint32_t targetXBits,
                                                               std::uint32_t targetYBits,
                                                               std::int32_t durationFrames);
PlayerModeTransitionEffectConfig BuildPlayerMode4TransitionEffectConfig();
PlayerModeTransitionEffectConfig BuildPlayerMode3TransitionEffectConfig();
PlayerInitialModeOwnerState BuildPlayerInitialModeOwnerState();
PlayerMode4EntryOwnerState BuildPlayerMode4EntryOwnerState();
PlayerMode3EntryOwnerState BuildPlayerMode3EntryOwnerState();
std::uint32_t ComputePlayerBombBackdropColor(std::int32_t timerFrame, std::int32_t durationFrames);
std::uint32_t ComputePlayerMode4StageColor(std::int32_t mode4Frame);
std::int32_t ApplyPlayerBombScoreDrainDifficultyScale(std::int32_t scoreDrain, Th07Difficulty difficulty);
std::int32_t ComputePlayerBombScoreDrainStep(std::int32_t scoreDelta, Th07Difficulty difficulty,
                                             std::int32_t durationFrames, float scoreDrainFactor,
                                             std::int32_t minimumScoreDrainTotal);
std::int32_t ComputePlayerBombScoreDrainStep(std::int32_t scoreDelta, Th07Difficulty difficulty,
                                             const PlayerBombRoutineTarget &routine);
std::string_view PlayerBombRoutineSlotName(PlayerBombRoutineSlot slot);

} // namespace th07
