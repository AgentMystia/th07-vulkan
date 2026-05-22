#pragma once

#include "Th07EffectTables.hpp"
#include "Th07StdFile.hpp"
#include "inttypes.hpp"

#include <cstdint>

namespace th07 {

inline constexpr u32 kTh07StageLoadStdFunctionAddress = 0x00407610;
inline constexpr u32 kTh07StageLoadStdOpenPathCallAddress = 0x0040761e;
inline constexpr u32 kTh07StageLoadStdErrorStringPushAddress = 0x00407638;
inline constexpr u32 kTh07StageLoadStdErrorBufferPushAddress = 0x0040763d;
inline constexpr u32 kTh07StageLoadStdErrorLogCallAddress = 0x00407642;
inline constexpr u32 kTh07StageLoadStdMallocCallAddress = 0x00407730;
inline constexpr u32 kTh07StageLoadStdSetScriptCallAddress = 0x004077b3;
inline constexpr u32 kTh07StageLoadStdOpenPathFunctionAddress = 0x00431330;
inline constexpr u32 kTh07StageLoadStdLogFunctionAddress = 0x004315f0;
inline constexpr u32 kTh07StageLoadStdMallocFunctionAddress = 0x0047d39d;
inline constexpr u32 kTh07StageLoadStdSetScriptFunctionAddress = kTh07StageSetAnmVmScriptFunctionAddress;
inline constexpr u32 kTh07StageLoadStdAnmManagerPointerAddress = 0x004b9e44;
inline constexpr u32 kTh07StageLoadStdLogBufferAddress = 0x00624210;
inline constexpr u32 kTh07StageLoadStdFailureStringAddress = 0x0049888c;

inline constexpr u32 kTh07StageLoadStdOwnerVmArrayOffset = 0x0000;
inline constexpr u32 kTh07StageLoadStdOwnerStdDataOffset = kTh07StageStdDataOffset;
inline constexpr u32 kTh07StageLoadStdOwnerQuadCountOffset = kTh07StageQuadCountOffset;
inline constexpr u32 kTh07StageLoadStdOwnerObjectCountOffset = kTh07StageObjectCountOffset;
inline constexpr u32 kTh07StageLoadStdOwnerObjectTableOffset = kTh07StageObjectTableOffset;
inline constexpr u32 kTh07StageLoadStdOwnerObjectInstancesOffset = kTh07StageObjectInstancesOffset;
inline constexpr u32 kTh07StageLoadStdOwnerScriptOffset = kTh07StageScriptOffset;

inline constexpr u32 kTh07StageLoadStdHeaderObjectCountOffset = 0x0000;
inline constexpr u32 kTh07StageLoadStdHeaderQuadCountOffset = 0x0002;
inline constexpr u32 kTh07StageLoadStdHeaderInstancesOffsetOffset = 0x0004;
inline constexpr u32 kTh07StageLoadStdHeaderScriptOffsetOffset = 0x0008;
inline constexpr u32 kTh07StageLoadStdObjectOffsetTableFileOffset = kTh07StdObjectOffsetTableOffset;
inline constexpr u32 kTh07StageLoadStdObjectPointerSize = sizeof(u32);
inline constexpr u32 kTh07StageLoadStdObjectFlagsOffset = 0x0003;
inline constexpr u8 kTh07StageLoadStdObjectActiveFlag = 1;
inline constexpr u32 kTh07StageLoadStdObjectFirstQuadOffset = kTh07StdObjectFirstQuadOffset;
inline constexpr u32 kTh07StageLoadStdQuadTypeOffset = 0x0000;
inline constexpr u32 kTh07StageLoadStdQuadByteSizeOffset = 0x0002;
inline constexpr u32 kTh07StageLoadStdQuadAnmScriptOffset = 0x0004;
inline constexpr u32 kTh07StageLoadStdQuadVmIndexOffset = 0x0006;
inline constexpr u32 kTh07StageLoadStdQuadVmSlotSize = kTh07StageAnmVmSlotSize;
inline constexpr i32 kTh07StageLoadStdAnmScriptBase = kTh07StageAnmVmScriptBase;

struct Th07StageStdLoaderContract {
    u32 functionAddress;
    u32 openPathCallAddress;
    u32 ownerStdDataOffset;
    u32 ownerQuadCountOffset;
    u32 ownerObjectCountOffset;
    u32 ownerObjectTableOffset;
    u32 ownerObjectInstancesOffset;
    u32 ownerScriptOffset;
    u32 objectOffsetTableFileOffset;
    u32 quadVmSlotSize;
    i32 anmScriptBase;
};

inline constexpr Th07StageStdLoaderContract kTh07StageStdLoaderContract = {
    kTh07StageLoadStdFunctionAddress,
    kTh07StageLoadStdOpenPathCallAddress,
    kTh07StageLoadStdOwnerStdDataOffset,
    kTh07StageLoadStdOwnerQuadCountOffset,
    kTh07StageLoadStdOwnerObjectCountOffset,
    kTh07StageLoadStdOwnerObjectTableOffset,
    kTh07StageLoadStdOwnerObjectInstancesOffset,
    kTh07StageLoadStdOwnerScriptOffset,
    kTh07StageLoadStdObjectOffsetTableFileOffset,
    kTh07StageLoadStdQuadVmSlotSize,
    kTh07StageLoadStdAnmScriptBase,
};

constexpr u32 Th07StageStdObjectOffsetTableEntryFileOffset(u32 objectIndex)
{
    return kTh07StageLoadStdObjectOffsetTableFileOffset +
           objectIndex * kTh07StageLoadStdObjectPointerSize;
}

constexpr u32 Th07StageStdVmAllocationSize(u32 quadCount)
{
    return quadCount * kTh07StageLoadStdQuadVmSlotSize;
}

} // namespace th07
