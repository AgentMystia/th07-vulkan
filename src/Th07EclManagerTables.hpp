#pragma once

#include "Th07EclFile.hpp"
#include "inttypes.hpp"

#include <cstdint>

namespace th07 {

inline constexpr u32 kTh07EclManagerLoadFunctionAddress = 0x0040e420;
inline constexpr u32 kTh07EclManagerUnloadFunctionAddress = 0x0040e4f0;
inline constexpr u32 kTh07EclManagerCallSubFunctionAddress = 0x0040e530;
inline constexpr u32 kTh07EclManagerLoadOpenPathCallAddress = 0x0040e42e;
inline constexpr u32 kTh07EclManagerLoadErrorStringPushAddress = 0x0040e440;
inline constexpr u32 kTh07EclManagerLoadErrorBufferPushAddress = 0x0040e445;
inline constexpr u32 kTh07EclManagerLoadErrorLogCallAddress = 0x0040e44a;
inline constexpr u32 kTh07EclManagerUnloadFreeCallAddress = 0x0040e50d;
inline constexpr u32 kTh07EclManagerLogBufferAddress = 0x00624210;
inline constexpr u32 kTh07EclManagerLoadFailureStringAddress = 0x00498700;
inline constexpr u32 kTh07EclManagerLogFunctionAddress = 0x004315f0;
inline constexpr u32 kTh07EclManagerOpenPathFunctionAddress = 0x00431330;
inline constexpr u32 kTh07EclManagerFreeFunctionAddress = 0x0047d285;

inline constexpr u32 kTh07EclManagerOwnerFileOffset = 0x0000;
inline constexpr u32 kTh07EclManagerOwnerSubOffsetTableOffset = 0x0004;
inline constexpr u32 kTh07EclManagerOwnerKnownPointerSpan = 0x0008;

inline constexpr u32 kTh07EclManagerSubCountOffset = 0x0000;
inline constexpr u32 kTh07EclManagerLoadHeaderRelocationBaseOffset = 0x0004;
inline constexpr u32 kTh07EclManagerLoadHeaderRelocationEntryCount = 0x0010;
inline constexpr u32 kTh07EclManagerLoadHeaderRelocationLastOffset =
    kTh07EclManagerLoadHeaderRelocationBaseOffset +
    (kTh07EclManagerLoadHeaderRelocationEntryCount - 1) * sizeof(u32);
inline constexpr u32 kTh07EclManagerSubOffsetSentinelFileOffset = kTh07EclSubOffsetTableOffset;
inline constexpr u32 kTh07EclManagerRuntimeSubOffsetTableFileOffset = 0x0044;
inline constexpr bool kTh07EclManagerRuntimeSubOffsetTableSkipsSentinel = true;

inline constexpr u32 kTh07EclManagerContextCurrentInstrOffset = 0x0000;
inline constexpr u32 kTh07EclManagerContextPrimaryTimerPreviousOffset = 0x0004;
inline constexpr u32 kTh07EclManagerContextPrimaryTimerSubframeOffset = 0x0008;
inline constexpr u32 kTh07EclManagerContextPrimaryTimerCurrentOffset = 0x000c;
inline constexpr u32 kTh07EclManagerContextInterruptTimerPreviousOffset = 0x0080;
inline constexpr u32 kTh07EclManagerContextInterruptTimerSubframeOffset = 0x0084;
inline constexpr u32 kTh07EclManagerContextInterruptTimerCurrentOffset = 0x0088;
inline constexpr u32 kTh07EclManagerContextSubIdOffset = 0x0214;
inline constexpr u32 kTh07EclManagerContextMinimumKnownSpan = 0x0216;
inline constexpr i32 kTh07EclManagerTimerPreviousSentinel = -999;

struct Th07EclManagerLoadContract {
    u32 functionAddress;
    u32 openPathCallAddress;
    u32 ownerFileOffset;
    u32 ownerSubOffsetTableOffset;
    u32 headerRelocationBaseOffset;
    u32 headerRelocationEntryCount;
    u32 runtimeSubOffsetTableFileOffset;
    bool runtimeSubOffsetTableSkipsSentinel;
};

struct Th07EclManagerCallSubContract {
    u32 functionAddress;
    u32 ownerSubOffsetTableOffset;
    u32 contextCurrentInstrOffset;
    u32 primaryTimerPreviousOffset;
    u32 primaryTimerSubframeOffset;
    u32 primaryTimerCurrentOffset;
    u32 interruptTimerPreviousOffset;
    u32 interruptTimerSubframeOffset;
    u32 interruptTimerCurrentOffset;
    u32 contextSubIdOffset;
    i32 timerPreviousSentinel;
};

inline constexpr Th07EclManagerLoadContract kTh07EclManagerLoadContract = {
    kTh07EclManagerLoadFunctionAddress,
    kTh07EclManagerLoadOpenPathCallAddress,
    kTh07EclManagerOwnerFileOffset,
    kTh07EclManagerOwnerSubOffsetTableOffset,
    kTh07EclManagerLoadHeaderRelocationBaseOffset,
    kTh07EclManagerLoadHeaderRelocationEntryCount,
    kTh07EclManagerRuntimeSubOffsetTableFileOffset,
    kTh07EclManagerRuntimeSubOffsetTableSkipsSentinel,
};

inline constexpr Th07EclManagerCallSubContract kTh07EclManagerCallSubContract = {
    kTh07EclManagerCallSubFunctionAddress,
    kTh07EclManagerOwnerSubOffsetTableOffset,
    kTh07EclManagerContextCurrentInstrOffset,
    kTh07EclManagerContextPrimaryTimerPreviousOffset,
    kTh07EclManagerContextPrimaryTimerSubframeOffset,
    kTh07EclManagerContextPrimaryTimerCurrentOffset,
    kTh07EclManagerContextInterruptTimerPreviousOffset,
    kTh07EclManagerContextInterruptTimerSubframeOffset,
    kTh07EclManagerContextInterruptTimerCurrentOffset,
    kTh07EclManagerContextSubIdOffset,
    kTh07EclManagerTimerPreviousSentinel,
};

constexpr u32 Th07EclManagerHeaderRelocationEntryFileOffset(u32 index)
{
    return kTh07EclManagerLoadHeaderRelocationBaseOffset + index * sizeof(u32);
}

constexpr u32 Th07EclManagerRuntimeSubOffsetEntryFileOffset(u32 subIndex)
{
    return kTh07EclManagerRuntimeSubOffsetTableFileOffset + subIndex * sizeof(u32);
}

} // namespace th07
