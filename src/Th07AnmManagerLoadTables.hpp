#pragma once

#include "Th07AnmFile.hpp"
#include "inttypes.hpp"

namespace th07 {

// Source-derived from FUN_0044df90, FUN_0044e070, and FUN_0044e4e0.
// This records the original manager slot/load ownership contract; the live
// AnmManager replacement is still pending proof of the texture/backend boundary.
inline constexpr u32 kTh07AnmManagerLoadAnmFunctionAddress = 0x0044df90;
inline constexpr u32 kTh07AnmManagerLoadAnmSegmentFunctionAddress = 0x0044e070;
inline constexpr u32 kTh07AnmManagerReleaseAnmFunctionAddress = 0x0044e4e0;
inline constexpr u32 kTh07AnmManagerLoadAnmOpenPathCallAddress = 0x0044df9e;
inline constexpr u32 kTh07AnmManagerLoadAnmFailureLogCallAddress = 0x0044dfc3;
inline constexpr u32 kTh07AnmManagerLoadAnmSegmentCallAddress = 0x0044dff3;
inline constexpr u32 kTh07AnmManagerLoadAnmFailCountStoreAddress = 0x0044e010;
inline constexpr u32 kTh07AnmManagerLoadAnmNextOffsetCompareAddress = 0x0044e028;
inline constexpr u32 kTh07AnmManagerLoadAnmSuccessCountStoreAddress = 0x0044e03d;
inline constexpr u32 kTh07AnmManagerLoadAnmNextEntryAddAddress = 0x0044e04e;

inline constexpr u32 kTh07AnmManagerOpenPathFunctionAddress = 0x00431330;
inline constexpr u32 kTh07AnmManagerLogFunctionAddress = 0x00431730;
inline constexpr u32 kTh07AnmManagerReleaseTextureFunctionAddress = 0x0044e6f0;
inline constexpr u32 kTh07AnmManagerLoadSpriteFunctionAddress = 0x0044e780;
inline constexpr u32 kTh07AnmManagerLogBufferAddress = 0x00624210;
inline constexpr u32 kTh07AnmManagerLoadFailureStringAddress = 0x00495c7c;
inline constexpr u32 kTh07AnmManagerInvalidSlotStringAddress = 0x00495c5c;
inline constexpr u32 kTh07AnmManagerInvalidVersionStringAddress = 0x00495c3c;
inline constexpr u32 kTh07AnmManagerTextureFailureStringAddress = 0x00495bf8;
inline constexpr u32 kTh07AnmManagerEmbeddedTextureFailureStringAddress = 0x00495bb8;
inline constexpr u32 kTh07AnmManagerSpriteRangeFailureStringAddress = 0x00495b80;
inline constexpr u32 kTh07AnmManagerScriptRangeFailureStringAddress = 0x00495b4c;

inline constexpr u32 kTh07AnmManagerLoadAnmInitialOwnedFlag = 1;
inline constexpr u32 kTh07AnmManagerLoadAnmChainedOwnedFlag = 0;
inline constexpr u32 kTh07AnmManagerMaxLoadedSlots = 0x32;
inline constexpr u32 kTh07AnmManagerMaxIndexedSpriteOrScript = 0x09ff;
inline constexpr u32 kTh07AnmManagerIndexedSpriteOrScriptLimit = 0x0a00;
inline constexpr u32 kTh07AnmManagerSpriteStride = 0x40;

inline constexpr u32 kTh07AnmManagerLoadedSpritesOffset = 0x000060;
inline constexpr u32 kTh07AnmManagerTexturesOffset = 0x282ac;
inline constexpr u32 kTh07AnmManagerTextureNamePointersOffset = 0x28acc;
inline constexpr u32 kTh07AnmManagerScriptsOffset = 0x28ef0;
inline constexpr u32 kTh07AnmManagerSpriteIndicesOffset = 0x2b6f0;
inline constexpr u32 kTh07AnmManagerSlotInfoOffset = 0x2def0;
inline constexpr u32 kTh07AnmManagerSlotInfoStride = 0x0c;
inline constexpr u32 kTh07AnmManagerSlotFilePointerFieldOffset = 0x00;
inline constexpr u32 kTh07AnmManagerSlotSpriteOffsetFieldOffset = 0x04;
inline constexpr u32 kTh07AnmManagerSlotLoadedCountFieldOffset = 0x08;
inline constexpr u32 kTh07AnmManagerCurrentTexturePointerOffset = 0x2e4cc;
inline constexpr u32 kTh07AnmManagerCurrentBlendModeOffset = 0x2e4d0;
inline constexpr u32 kTh07AnmManagerCurrentColorOpOffset = 0x2e4d1;
inline constexpr u32 kTh07AnmManagerCurrentVertexShaderOffset = 0x2e4d2;
inline constexpr u8 kTh07AnmManagerCurrentBlendModeResetValue = 0xff;
inline constexpr u8 kTh07AnmManagerCurrentColorOpResetValue = 0xff;
inline constexpr u8 kTh07AnmManagerCurrentVertexShaderResetValue = 0x00;

inline constexpr u32 kTh07AnmHeaderTextureSlotOffset = 0x08;
inline constexpr u32 kTh07AnmHeaderTextureWidthOffset = 0x0c;
inline constexpr u32 kTh07AnmHeaderTextureHeightOffset = 0x10;
inline constexpr u32 kTh07AnmHeaderFormatOffset = 0x14;
inline constexpr u32 kTh07AnmHeaderColorKeyOffset = 0x18;
inline constexpr u32 kTh07AnmHeaderNameOffsetOffset = 0x1c;
inline constexpr u32 kTh07AnmHeaderSpriteScriptOffsetOffset = 0x20;
inline constexpr u32 kTh07AnmHeaderMipmapNameOffsetOffset = 0x24;
inline constexpr u32 kTh07AnmHeaderVersionOffset = 0x28;
inline constexpr u32 kTh07AnmHeaderTextureDataOffsetOffset = 0x30;
inline constexpr u32 kTh07AnmHeaderEmbeddedTextureFlagOffset = 0x34;
inline constexpr u32 kTh07AnmHeaderOwnedFileFlagOffset = 0x35;
inline constexpr u32 kTh07AnmHeaderNextEntryOffsetOffset = 0x38;
inline constexpr u32 kTh07AnmHeaderSpriteOffsetTableOffset = kTh07AnmSpriteOffsetTableOffset;
inline constexpr u32 kTh07AnmHeaderExpectedVersion = 2;

struct Th07AnmManagerLoadContract {
    u32 loadAnmFunction;
    u32 loadSegmentFunction;
    u32 releaseAnmFunction;
    u32 maxLoadedSlots;
    u32 indexedSpriteScriptLimit;
    u32 slotInfoOffset;
    u32 slotInfoStride;
};

inline constexpr Th07AnmManagerLoadContract kTh07AnmManagerLoadContract = {
    kTh07AnmManagerLoadAnmFunctionAddress,
    kTh07AnmManagerLoadAnmSegmentFunctionAddress,
    kTh07AnmManagerReleaseAnmFunctionAddress,
    kTh07AnmManagerMaxLoadedSlots,
    kTh07AnmManagerIndexedSpriteOrScriptLimit,
    kTh07AnmManagerSlotInfoOffset,
    kTh07AnmManagerSlotInfoStride,
};

constexpr u32 Th07AnmManagerSlotInfoFieldOffset(u32 slot, u32 field)
{
    return kTh07AnmManagerSlotInfoOffset + slot * kTh07AnmManagerSlotInfoStride + field;
}

constexpr u32 Th07AnmManagerSlotFilePointerOffset(u32 slot)
{
    return Th07AnmManagerSlotInfoFieldOffset(slot, kTh07AnmManagerSlotFilePointerFieldOffset);
}

constexpr u32 Th07AnmManagerSlotSpriteOffsetOffset(u32 slot)
{
    return Th07AnmManagerSlotInfoFieldOffset(slot, kTh07AnmManagerSlotSpriteOffsetFieldOffset);
}

constexpr u32 Th07AnmManagerSlotLoadedCountOffset(u32 slot)
{
    return Th07AnmManagerSlotInfoFieldOffset(slot, kTh07AnmManagerSlotLoadedCountFieldOffset);
}

constexpr u32 Th07AnmManagerLoadedSpriteOffset(u32 globalSpriteId)
{
    return kTh07AnmManagerLoadedSpritesOffset + globalSpriteId * kTh07AnmManagerSpriteStride;
}

constexpr u32 Th07AnmManagerScriptPointerOffset(u32 globalScriptId)
{
    return kTh07AnmManagerScriptsOffset + globalScriptId * sizeof(u32);
}

constexpr u32 Th07AnmManagerSpriteIndexOffset(u32 globalScriptId)
{
    return kTh07AnmManagerSpriteIndicesOffset + globalScriptId * sizeof(u32);
}

constexpr u32 Th07AnmManagerTextureNamePointerOffset(u32 slot)
{
    return kTh07AnmManagerTextureNamePointersOffset + slot * sizeof(u32);
}

constexpr bool IsTh07AnmManagerSlotInRange(u32 slot)
{
    return slot < kTh07AnmManagerMaxLoadedSlots;
}

constexpr bool IsTh07AnmManagerGlobalSpriteScriptIdInRange(u32 globalId)
{
    return globalId < kTh07AnmManagerIndexedSpriteOrScriptLimit;
}

constexpr u32 Th07AnmManagerAdvanceSpriteScriptOffset(u32 spriteScriptOffset, u32 loadedIdCount)
{
    return spriteScriptOffset + loadedIdCount;
}

} // namespace th07
