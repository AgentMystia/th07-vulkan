#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace th07 {

inline constexpr std::uint32_t kTh07TimerIncrementFunctionAddress = 0x00439401;
inline constexpr std::uint32_t kTh07TimerDecrementFunctionAddress = 0x004394c7;
inline constexpr std::uint32_t kTh07TickTimerFunctionAddress = 0x0043958d;
inline constexpr std::uint32_t kTh07TimerForceStepProducerFunctionAddress = 0x00418fc0;
inline constexpr std::uint32_t kTh07SupervisorGlobalAddress = 0x00575950;
inline constexpr std::uint32_t kTh07SupervisorTimerMultiplierOffset = 0x178;
inline constexpr std::uint32_t kTh07SupervisorTimerMultiplierGlobalAddress = 0x00575ac8;
inline constexpr std::uint32_t kTh07SupervisorTimerForceStepFlagsGlobalAddress = 0x00575adc;
inline constexpr std::uint32_t kTh07SupervisorTimerForceStepFlagsOffset = 0x18c;
inline constexpr std::uint32_t kTh07TimerForceStepBit = 5;
inline constexpr std::uint32_t kTh07TimerForceStepMask = 1u << kTh07TimerForceStepBit;
inline constexpr std::uint32_t kTh07TimerZeroConstantAddress = 0x00498a4c;
inline constexpr std::uint32_t kTh07TimerOneFrameConstantAddress = 0x00498a54;
inline constexpr std::uint32_t kTh07TimerSubframeThresholdAddress = 0x00498a58;
inline constexpr std::uint32_t kTh07TimerZeroBits = 0x00000000;
inline constexpr std::uint32_t kTh07TimerOneFrameBits = 0x3f800000;
inline constexpr std::uint32_t kTh07TimerSubframeThresholdBits = 0x3f7d70a4;
inline constexpr std::int32_t kTh07TimerPreviousSentinel = -999;
inline constexpr std::uint32_t kTh07TimerPreviousSentinelBits = 0xfffffc19;
inline constexpr float kTh07TimerOneFrame = 1.0f;
inline constexpr float kTh07TimerZeroFrame = 0.0f;
inline constexpr float kTh07TimerSubframeThreshold = 0.99f;
inline constexpr std::uint32_t kTh07TimerIncrementForceFlagReadAddress = 0x00439408;
inline constexpr std::uint32_t kTh07TimerIncrementPreviousSentinelWriteAddress = 0x0043942c;
inline constexpr std::uint32_t kTh07TimerIncrementMultiplierLoadAddress = 0x00439435;
inline constexpr std::uint32_t kTh07TimerIncrementWholeFrameAddAddress = 0x00439448;
inline constexpr std::uint32_t kTh07TimerIncrementNegativeCallAddress = 0x00439468;
inline constexpr std::uint32_t kTh07TimerIncrementSubframeMultiplyAddress = 0x0043947a;
inline constexpr std::uint32_t kTh07TimerIncrementOneFrameCompareAddress = 0x00439492;
inline constexpr std::uint32_t kTh07TimerDecrementForceFlagReadAddress = 0x004394ce;
inline constexpr std::uint32_t kTh07TimerDecrementPreviousSentinelWriteAddress = 0x004394f2;
inline constexpr std::uint32_t kTh07TimerDecrementMultiplierLoadAddress = 0x004394fb;
inline constexpr std::uint32_t kTh07TimerDecrementWholeFrameSubtractAddress = 0x0043950e;
inline constexpr std::uint32_t kTh07TimerDecrementNegativeCallAddress = 0x0043952e;
inline constexpr std::uint32_t kTh07TimerDecrementSubframeMultiplyAddress = 0x00439540;
inline constexpr std::uint32_t kTh07TimerDecrementZeroCompareAddress = 0x00439558;
inline constexpr std::uint32_t kTh07TimerForceStepProducerMultiplierStoreAddress = 0x004190a2;
inline constexpr std::uint32_t kTh07TimerForceStepProducerFlagReadAddress = 0x004190bb;
inline constexpr std::uint32_t kTh07TimerForceStepProducerFlagOrAddress = 0x004190c1;
inline constexpr std::uint32_t kTh07TimerForceStepProducerFlagWriteAddress = 0x004190c4;
inline constexpr std::size_t kTh07TimerForceStepFlagAccessCount = 3;

enum class Th07TimerForceStepFlagAccessKind {
    ProducerSet,
    IncrementRead,
    DecrementRead,
};

struct Th07TimerForceStepFlagAccess {
    Th07TimerForceStepFlagAccessKind kind;
    std::uint32_t functionAddress;
    std::uint32_t instructionAddress;
    std::uint32_t bitMask;
    std::string_view evidence;
};

inline constexpr std::array<Th07TimerForceStepFlagAccess, kTh07TimerForceStepFlagAccessCount>
    kTh07TimerForceStepFlagAccesses = {{
        {Th07TimerForceStepFlagAccessKind::ProducerSet,
         kTh07TimerForceStepProducerFunctionAddress,
         kTh07TimerForceStepProducerFlagOrAddress,
         kTh07TimerForceStepMask,
         "FUN_00418fc0 sets DAT_00575adc bit 5 after deriving DAT_00575ac8"},
        {Th07TimerForceStepFlagAccessKind::IncrementRead,
         kTh07TimerIncrementFunctionAddress,
         kTh07TimerIncrementForceFlagReadAddress,
         kTh07TimerForceStepMask,
         "FUN_00439401 reads DAT_00575adc bit 5 before incrementing"},
        {Th07TimerForceStepFlagAccessKind::DecrementRead,
         kTh07TimerDecrementFunctionAddress,
         kTh07TimerDecrementForceFlagReadAddress,
         kTh07TimerForceStepMask,
         "FUN_004394c7 reads DAT_00575adc bit 5 before decrementing"},
    }};

struct Th07TimerState {
    std::int32_t previous;
    float subframe;
    std::int32_t current;
};

inline const Th07TimerForceStepFlagAccess *
FindTh07TimerForceStepFlagAccess(Th07TimerForceStepFlagAccessKind kind)
{
    for (const Th07TimerForceStepFlagAccess &access : kTh07TimerForceStepFlagAccesses) {
        if (access.kind == kind) {
            return &access;
        }
    }
    return nullptr;
}

inline bool HasTh07TimerForceStep(std::uint32_t flags)
{
    return (flags & kTh07TimerForceStepMask) != 0;
}

inline bool UsesTh07TimerSubframePath(float timerMultiplier)
{
    return timerMultiplier <= kTh07TimerSubframeThreshold;
}

inline void DecrementTh07Timer(Th07TimerState *timer,
                               std::int32_t value,
                               float timerMultiplier,
                               std::uint32_t forceStepFlags);

inline void IncrementTh07Timer(Th07TimerState *timer,
                               std::int32_t value,
                               float timerMultiplier,
                               std::uint32_t forceStepFlags)
{
    if (HasTh07TimerForceStep(forceStepFlags)) {
        timer->current += 1;
        timer->subframe = kTh07TimerZeroFrame;
        timer->previous = kTh07TimerPreviousSentinel;
    }
    if (!UsesTh07TimerSubframePath(timerMultiplier)) {
        timer->current += value;
        return;
    }
    if (value < 0) {
        DecrementTh07Timer(timer, -value, timerMultiplier, forceStepFlags);
        return;
    }

    timer->previous = timer->current;
    timer->subframe += static_cast<float>(value) * timerMultiplier;
    while (timer->subframe >= kTh07TimerOneFrame) {
        timer->current += 1;
        timer->subframe -= kTh07TimerOneFrame;
    }
}

inline void DecrementTh07Timer(Th07TimerState *timer,
                               std::int32_t value,
                               float timerMultiplier,
                               std::uint32_t forceStepFlags)
{
    if (HasTh07TimerForceStep(forceStepFlags)) {
        timer->current -= 1;
        timer->subframe = kTh07TimerZeroFrame;
        timer->previous = kTh07TimerPreviousSentinel;
    }
    if (!UsesTh07TimerSubframePath(timerMultiplier)) {
        timer->current -= value;
        return;
    }
    if (value < 0) {
        IncrementTh07Timer(timer, -value, timerMultiplier, forceStepFlags);
        return;
    }

    timer->previous = timer->current;
    timer->subframe -= static_cast<float>(value) * timerMultiplier;
    while (timer->subframe < kTh07TimerZeroFrame) {
        timer->current -= 1;
        timer->subframe += kTh07TimerOneFrame;
    }
}

inline void TickTh07FrameSubtimer(std::int32_t *currentFrame,
                                  float *subframe,
                                  float timerMultiplier)
{
    // FUN_0043958d only carries one pending subframe into one whole-frame tick.
    if (!UsesTh07TimerSubframePath(timerMultiplier)) {
        *currentFrame += 1;
        return;
    }

    *subframe += timerMultiplier;
    if (*subframe >= kTh07TimerOneFrame) {
        *currentFrame += 1;
        *subframe -= kTh07TimerOneFrame;
    }
}

} // namespace th07
