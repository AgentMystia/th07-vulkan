#pragma once

#include "GuiLayout.hpp"
#include "Th07MsgFile.hpp"
#include "inttypes.hpp"

#include <cstdint>

namespace th07 {

inline constexpr u32 kTh07GuiLoadMsgFunctionAddress = 0x00429935;
inline constexpr u32 kTh07GuiFreeMsgFunctionAddress = 0x004299f8;
inline constexpr u32 kTh07GuiLoadMsgFreeMsgCallAddress = 0x00429940;
inline constexpr u32 kTh07GuiLoadMsgOpenPathCallAddress = 0x0042994a;
inline constexpr u32 kTh07GuiLoadMsgFileStoreAddress = 0x00429955;
inline constexpr u32 kTh07GuiLoadMsgNullCompareAddress = 0x00429961;
inline constexpr u32 kTh07GuiLoadMsgPathPushAddress = 0x0042996a;
inline constexpr u32 kTh07GuiLoadMsgErrorStringPushAddress = 0x0042996d;
inline constexpr u32 kTh07GuiLoadMsgErrorBufferPushAddress = 0x00429972;
inline constexpr u32 kTh07GuiLoadMsgErrorLogCallAddress = 0x00429977;
inline constexpr u32 kTh07GuiLoadMsgCurrentIndexInitAddress = 0x0042998a;
inline constexpr u32 kTh07GuiLoadMsgCurrentInstructionInitAddress = 0x00429997;
inline constexpr u32 kTh07GuiLoadMsgEntryCountCompareAddress = 0x004299ba;
inline constexpr u32 kTh07GuiLoadMsgEntryPointerReadAddress = 0x004299d3;
inline constexpr u32 kTh07GuiLoadMsgEntryPointerRelocateAddress = 0x004299d7;
inline constexpr u32 kTh07GuiLoadMsgEntryPointerWriteAddress = 0x004299ec;
inline constexpr u32 kTh07GuiLoadMsgSuccessReturnAddress = 0x004299f2;
inline constexpr u32 kTh07GuiFreeMsgNullCompareAddress = 0x00429a06;
inline constexpr u32 kTh07GuiFreeMsgFreeCallAddress = 0x00429a21;
inline constexpr u32 kTh07GuiFreeMsgFileClearAddress = 0x00429a2d;

inline constexpr u32 kTh07GuiLoadMsgOpenPathFunctionAddress = 0x00431330;
inline constexpr u32 kTh07GuiLoadMsgLogFunctionAddress = 0x004315f0;
inline constexpr u32 kTh07GuiFreeMsgFreeFunctionAddress = 0x0047d285;
inline constexpr u32 kTh07GuiLoadMsgLogBufferAddress = 0x00624210;
inline constexpr u32 kTh07GuiLoadMsgFailureStringAddress = 0x00498108;
inline constexpr u32 kTh07GuiLoadMsgOpenPathExternalFlag = 0;

inline constexpr u32 kTh07GuiLoadMsgOwnerPointerOffset = kTh07GuiSpellcardOwnerPointerOffset;
inline constexpr u32 kTh07GuiLoadMsgMsgFileOffset = 0x1fba4;
inline constexpr u32 kTh07GuiLoadMsgCurrentInstructionOffset = kTh07GuiMsgCurrentInstructionOffset;
inline constexpr u32 kTh07GuiLoadMsgCurrentIndexOffset = kTh07GuiMsgCurrentIndexOffset;
inline constexpr i32 kTh07GuiLoadMsgInvalidCurrentIndex = -1;
inline constexpr u32 kTh07GuiLoadMsgInitialCurrentInstruction = 0;

inline constexpr u32 kTh07GuiLoadMsgHeaderEntryCountOffset = 0x0000;
inline constexpr u32 kTh07GuiLoadMsgEntryPointerTableOffset = kTh07MsgHeaderSize;
inline constexpr u32 kTh07GuiLoadMsgEntryPointerSize = kTh07MsgEntryPointerSize;

struct Th07GuiMsgLoaderContract {
    u32 functionAddress;
    u32 freeFunctionAddress;
    u32 openPathCallAddress;
    u32 ownerPointerOffset;
    u32 msgFileOffset;
    u32 currentInstructionOffset;
    u32 currentIndexOffset;
    u32 headerEntryCountOffset;
    u32 entryPointerTableOffset;
    u32 entryPointerSize;
};

inline constexpr Th07GuiMsgLoaderContract kTh07GuiMsgLoaderContract = {
    kTh07GuiLoadMsgFunctionAddress,
    kTh07GuiFreeMsgFunctionAddress,
    kTh07GuiLoadMsgOpenPathCallAddress,
    kTh07GuiLoadMsgOwnerPointerOffset,
    kTh07GuiLoadMsgMsgFileOffset,
    kTh07GuiLoadMsgCurrentInstructionOffset,
    kTh07GuiLoadMsgCurrentIndexOffset,
    kTh07GuiLoadMsgHeaderEntryCountOffset,
    kTh07GuiLoadMsgEntryPointerTableOffset,
    kTh07GuiLoadMsgEntryPointerSize,
};

constexpr u32 Th07GuiMsgEntryPointerFileOffset(u32 entryIndex)
{
    return kTh07GuiLoadMsgEntryPointerTableOffset +
           entryIndex * kTh07GuiLoadMsgEntryPointerSize;
}

constexpr u32 Th07GuiMsgPointerTableByteSize(u32 entryPointerCount)
{
    return kTh07GuiLoadMsgEntryPointerTableOffset +
           entryPointerCount * kTh07GuiLoadMsgEntryPointerSize;
}

constexpr u32 Th07GuiMsgRelocatedEntryPointer(u32 fileBase, u32 entryOffset)
{
    return fileBase + entryOffset;
}

} // namespace th07
