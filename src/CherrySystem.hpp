#pragma once

#include "inttypes.hpp"

#include <array>

namespace th07
{
inline constexpr u32 kTh07CherryPointMaxStringAddress = 0x004983b4;
inline constexpr u32 kTh07CherryValueStringAddress = 0x004984cc;
inline constexpr std::size_t kTh07CherryHudStringAnchorCount = 2;
inline constexpr u32 kTh07CherryManagerAddress = 0x00626270;
inline constexpr u32 kTh07CherryPrimaryGainFunctionAddress = 0x0042f5a2;
inline constexpr u32 kTh07CherrySecondaryGainFunctionAddress = 0x0042f69f;
inline constexpr u32 kTh07CherryEclOpcode160CallAddress = 0x00416722;
inline constexpr u32 kTh07CherryEclOpcode160ThisPointerAddress = 0x00626270;
inline constexpr u16 kTh07CherryEclGainOpcode = 160;
inline constexpr u32 kTh07CherryNoInstructionAddress = 0;
inline constexpr u32 kTh07CherryCapOffset = 0x9618;
inline constexpr u32 kTh07CherryCurrentOffset = 0x961c;
inline constexpr u32 kTh07CherryPlusOffset = 0x9620;
inline constexpr u32 kTh07CherryPlayerPointerOffset = 0x0008;
inline constexpr u32 kTh07CherryPlayerThresholdOffset = 0x0088;
inline constexpr u32 kTh07CherryDisableFlagAddress = 0x004bfee5;
inline constexpr u32 kTh07CherryBorderStartFunctionAddress = 0x00441960;
inline constexpr u32 kTh07CherryBorderStartThisPointerAddress = 0x004bdad8;
inline constexpr u32 kTh07CherryToastRegisterFunctionAddress = 0x00427c81;
inline constexpr u32 kTh07CherryToastRegisterThisPointerAddress = 0x0049fbf0;
inline constexpr u8 kTh07CherryPointMaxToastKind = 3;
inline constexpr u32 kTh07CherryPrimaryCurrentWriteAddress = 0x0042f5c5;
inline constexpr u32 kTh07CherryPrimaryCapCompareAddress = 0x0042f5d7;
inline constexpr u32 kTh07CherryPrimaryCapClampWriteAddress = 0x0042f5eb;
inline constexpr u32 kTh07CherryPrimaryPlusWriteAddress = 0x0042f611;
inline constexpr u32 kTh07CherryPrimaryPlusClampWriteAddress = 0x0042f647;
inline constexpr u32 kTh07CherryPrimaryBorderStartCallAddress = 0x0042f652;
inline constexpr u32 kTh07CherrySecondaryCurrentWriteAddress = 0x0042f6c2;
inline constexpr u32 kTh07CherrySecondaryCapCompareAddress = 0x0042f6d4;
inline constexpr u32 kTh07CherrySecondaryCapClampWriteAddress = 0x0042f6e8;
inline constexpr std::size_t kTh07CherryGainHelperCount = 2;

struct CherryHudStringAnchor
{
    const char *text;
    u32 ghidraStringAddress;
};

enum class CherryGainHelperKind
{
    Primary,
    Secondary,
};

struct CherryGainHelperContract
{
    CherryGainHelperKind kind;
    const char *sourceFunction;
    u32 functionAddress;
    u32 managerAddress;
    bool updatesCherryPlus;
    u32 currentOffset;
    u32 capOffset;
    u32 cherryPlusOffset;
    u32 playerPointerOffset;
    u32 playerThresholdOffset;
    u32 thresholdDelta;
    u32 disableFlagAddress;
    u32 currentWriteAddress;
    u32 capCompareAddress;
    u32 capClampWriteAddress;
    u32 cherryPlusWriteAddress;
    u32 cherryPlusClampWriteAddress;
    u32 borderStartCallAddress;
    u32 borderStartFunctionAddress;
    u32 borderStartThisPointerAddress;
    u32 toastKindPushAddress;
    u32 toastScorePushAddress;
    u32 toastCallAddress;
    u32 toastRegisterFunctionAddress;
    u32 toastRegisterThisPointerAddress;
    u8 toastKind;
    const char *evidenceLines;
};

constexpr int kCherryMax = 500000;
constexpr int kCherryBorderStart = 50000;
constexpr int kCherryDisplayScale = 10;

extern const std::array<CherryHudStringAnchor, kTh07CherryHudStringAnchorCount> kCherryHudStringAnchors;
extern const std::array<CherryGainHelperContract, kTh07CherryGainHelperCount> kTh07CherryGainHelperContracts;

const CherryGainHelperContract *FindTh07CherryGainHelperContract(CherryGainHelperKind kind);
} // namespace th07
