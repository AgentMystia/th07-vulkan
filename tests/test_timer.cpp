#include "Th07Timer.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>

namespace {

bool NearlyEqual(float lhs, float rhs)
{
    return std::fabs(lhs - rhs) < 0.0001f;
}

} // namespace

int main()
{
    static_assert(th07::kTh07TimerIncrementFunctionAddress == 0x00439401);
    static_assert(th07::kTh07TimerDecrementFunctionAddress == 0x004394c7);
    static_assert(th07::kTh07TickTimerFunctionAddress == 0x0043958d);
    static_assert(th07::kTh07TimerForceStepProducerFunctionAddress == 0x00418fc0);
    static_assert(th07::kTh07SupervisorGlobalAddress == 0x00575950);
    static_assert(th07::kTh07SupervisorTimerMultiplierOffset == 0x178);
    static_assert(th07::kTh07SupervisorTimerMultiplierGlobalAddress == 0x00575ac8);
    static_assert(th07::kTh07SupervisorTimerForceStepFlagsGlobalAddress == 0x00575adc);
    static_assert(th07::kTh07SupervisorTimerForceStepFlagsOffset == 0x18c);
    static_assert(th07::kTh07TimerForceStepBit == 5);
    static_assert(th07::kTh07TimerForceStepMask == 0x20);
    static_assert(th07::kTh07TimerZeroConstantAddress == 0x00498a4c);
    static_assert(th07::kTh07TimerOneFrameConstantAddress == 0x00498a54);
    static_assert(th07::kTh07TimerSubframeThresholdAddress == 0x00498a58);
    static_assert(th07::kTh07TimerZeroBits == 0x00000000);
    static_assert(th07::kTh07TimerOneFrameBits == 0x3f800000);
    static_assert(th07::kTh07TimerSubframeThresholdBits == 0x3f7d70a4);
    static_assert(th07::kTh07TimerPreviousSentinel == -999);
    static_assert(th07::kTh07TimerPreviousSentinelBits == 0xfffffc19);
    static_assert(th07::kTh07TimerForceStepProducerMultiplierStoreAddress == 0x004190a2);
    static_assert(th07::kTh07TimerForceStepProducerFlagReadAddress == 0x004190bb);
    static_assert(th07::kTh07TimerForceStepProducerFlagOrAddress == 0x004190c1);
    static_assert(th07::kTh07TimerForceStepProducerFlagWriteAddress == 0x004190c4);
    static_assert(th07::kTh07TimerForceStepFlagAccessCount == 3);
    static_assert(th07::kTh07TimerForceStepFlagAccesses.size() ==
                  th07::kTh07TimerForceStepFlagAccessCount);

    const th07::Th07TimerForceStepFlagAccess *producer =
        th07::FindTh07TimerForceStepFlagAccess(th07::Th07TimerForceStepFlagAccessKind::ProducerSet);
    assert(producer != nullptr);
    assert(producer->functionAddress == th07::kTh07TimerForceStepProducerFunctionAddress);
    assert(producer->instructionAddress == th07::kTh07TimerForceStepProducerFlagOrAddress);
    assert(producer->bitMask == th07::kTh07TimerForceStepMask);

    const th07::Th07TimerForceStepFlagAccess *incrementRead =
        th07::FindTh07TimerForceStepFlagAccess(th07::Th07TimerForceStepFlagAccessKind::IncrementRead);
    assert(incrementRead != nullptr);
    assert(incrementRead->functionAddress == th07::kTh07TimerIncrementFunctionAddress);
    assert(incrementRead->instructionAddress == th07::kTh07TimerIncrementForceFlagReadAddress);
    assert(incrementRead->bitMask == th07::kTh07TimerForceStepMask);

    const th07::Th07TimerForceStepFlagAccess *decrementRead =
        th07::FindTh07TimerForceStepFlagAccess(th07::Th07TimerForceStepFlagAccessKind::DecrementRead);
    assert(decrementRead != nullptr);
    assert(decrementRead->functionAddress == th07::kTh07TimerDecrementFunctionAddress);
    assert(decrementRead->instructionAddress == th07::kTh07TimerDecrementForceFlagReadAddress);
    assert(decrementRead->bitMask == th07::kTh07TimerForceStepMask);

    assert(!th07::HasTh07TimerForceStep(0));
    assert(th07::HasTh07TimerForceStep(0x20));
    assert(th07::HasTh07TimerForceStep(0xffffffff));
    assert(!th07::UsesTh07TimerSubframePath(1.0f));
    assert(th07::UsesTh07TimerSubframePath(0.99f));
    assert(th07::UsesTh07TimerSubframePath(0.5f));

    th07::Th07TimerState timer{3, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, 5, 1.0f, 0);
    assert(timer.previous == 3);
    assert(NearlyEqual(timer.subframe, 0.25f));
    assert(timer.current == 15);

    timer = {3, 0.25f, 10};
    th07::DecrementTh07Timer(&timer, 5, 1.0f, 0);
    assert(timer.previous == 3);
    assert(NearlyEqual(timer.subframe, 0.25f));
    assert(timer.current == 5);

    timer = {-1, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, 2, 0.5f, 0);
    assert(timer.previous == 10);
    assert(NearlyEqual(timer.subframe, 0.25f));
    assert(timer.current == 11);

    timer = {-1, 0.25f, 10};
    th07::DecrementTh07Timer(&timer, 1, 0.5f, 0);
    assert(timer.previous == 10);
    assert(NearlyEqual(timer.subframe, 0.75f));
    assert(timer.current == 9);

    timer = {3, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, -2, 1.0f, 0);
    assert(timer.previous == 3);
    assert(NearlyEqual(timer.subframe, 0.25f));
    assert(timer.current == 8);

    timer = {3, 0.25f, 10};
    th07::DecrementTh07Timer(&timer, -2, 1.0f, 0);
    assert(timer.previous == 3);
    assert(NearlyEqual(timer.subframe, 0.25f));
    assert(timer.current == 12);

    timer = {3, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, 0, 1.0f, th07::kTh07TimerForceStepMask);
    assert(timer.previous == th07::kTh07TimerPreviousSentinel);
    assert(NearlyEqual(timer.subframe, 0.0f));
    assert(timer.current == 11);

    timer = {3, 0.25f, 10};
    th07::DecrementTh07Timer(&timer, 0, 1.0f, th07::kTh07TimerForceStepMask);
    assert(timer.previous == th07::kTh07TimerPreviousSentinel);
    assert(NearlyEqual(timer.subframe, 0.0f));
    assert(timer.current == 9);

    timer = {3, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, 1, 0.5f, th07::kTh07TimerForceStepMask);
    assert(timer.previous == 11);
    assert(NearlyEqual(timer.subframe, 0.5f));
    assert(timer.current == 11);

    timer = {3, 0.25f, 10};
    th07::DecrementTh07Timer(&timer, 1, 0.5f, th07::kTh07TimerForceStepMask);
    assert(timer.previous == 9);
    assert(NearlyEqual(timer.subframe, 0.5f));
    assert(timer.current == 8);

    timer = {3, 0.25f, 10};
    th07::IncrementTh07Timer(&timer, -1, 0.5f, th07::kTh07TimerForceStepMask);
    assert(timer.previous == 10);
    assert(NearlyEqual(timer.subframe, 0.5f));
    assert(timer.current == 9);

    std::int32_t frame = 7;
    float subframe = 0.75f;
    th07::TickTh07FrameSubtimer(&frame, &subframe, 1.0f);
    assert(frame == 8);
    assert(NearlyEqual(subframe, 0.75f));

    frame = 7;
    subframe = 0.25f;
    th07::TickTh07FrameSubtimer(&frame, &subframe, 0.99f);
    assert(frame == 8);
    assert(NearlyEqual(subframe, 0.24f));

    frame = 7;
    subframe = 0.25f;
    th07::TickTh07FrameSubtimer(&frame, &subframe, 0.5f);
    assert(frame == 7);
    assert(NearlyEqual(subframe, 0.75f));

    frame = 7;
    subframe = 0.75f;
    th07::TickTh07FrameSubtimer(&frame, &subframe, 0.5f);
    assert(frame == 8);
    assert(NearlyEqual(subframe, 0.25f));

    frame = 7;
    subframe = 0.75f;
    th07::TickTh07FrameSubtimer(&frame, &subframe, 2.4f);
    assert(frame == 8);
    assert(NearlyEqual(subframe, 0.75f));

    return 0;
}
