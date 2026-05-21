#pragma once

#include "Th07EffectTables.hpp"

#include <cstdint>

namespace th07 {

inline constexpr std::uint32_t kPlayerObjectAddress = 0x004bdad8;
inline constexpr std::uint32_t kPlayerObjectClearDwordCount = 0x2df9e;
inline constexpr std::uint32_t kPlayerObjectClearSize = kPlayerObjectClearDwordCount * 4;

inline constexpr std::uint32_t kPlayerPositionOffset = 0x0930;
inline constexpr std::uint32_t kPlayerVectorSize = 0x000c;
inline constexpr std::uint32_t kPlayerPositionXOffset = kPlayerPositionOffset;
inline constexpr std::uint32_t kPlayerPositionYOffset = kPlayerPositionOffset + 4;
inline constexpr std::uint32_t kPlayerPositionZOffset = kPlayerPositionOffset + 8;
inline constexpr std::uint32_t kPlayerPendingVector93cOffset = 0x093c;
inline constexpr std::uint32_t kPlayerHitboxMinOffset = 0x0948;
inline constexpr std::uint32_t kPlayerHitboxMaxOffset = 0x0954;
inline constexpr std::uint32_t kPlayerGrazeBoxMinOffset = 0x0960;
inline constexpr std::uint32_t kPlayerGrazeBoxMaxOffset = 0x096c;
inline constexpr std::uint32_t kPlayerGrabItemBoxMinOffset = 0x0978;
inline constexpr std::uint32_t kPlayerGrabItemBoxMaxOffset = 0x0984;
inline constexpr std::uint32_t kPlayerHitboxSizeOffset = 0x0990;
inline constexpr std::uint32_t kPlayerGrazeBoxSizeOffset = 0x099c;
inline constexpr std::uint32_t kPlayerGrabItemSizeOffset = 0x09a8;
inline constexpr std::uint32_t kPlayerAnmVmSlotSize = kTh07StageAnmVmSlotSize;
inline constexpr std::uint32_t kPlayerPrimaryAnmVmOffset = 0;
inline constexpr std::uint32_t kPlayerLeftOptionAnmVmOffset = kPlayerAnmVmSlotSize;
inline constexpr std::uint32_t kPlayerRightOptionAnmVmOffset = kPlayerAnmVmSlotSize * 2;
inline constexpr std::uint32_t kPlayerFourthAnmVmOffset = kPlayerAnmVmSlotSize * 3;
inline constexpr std::uint32_t kPlayerOriginalAnmVmPrefixSize = kPlayerAnmVmSlotSize * 4;
inline constexpr std::uint32_t kPlayerPrimaryAnmVmScriptIndexOffset =
    kPlayerPrimaryAnmVmOffset + kTh07StageAnmVmScriptIndexOffset;
inline constexpr std::uint32_t kPlayerLeftOptionAnmVmScriptIndexOffset =
    kPlayerLeftOptionAnmVmOffset + kTh07StageAnmVmScriptIndexOffset;
inline constexpr std::uint32_t kPlayerRightOptionAnmVmScriptIndexOffset =
    kPlayerRightOptionAnmVmOffset + kTh07StageAnmVmScriptIndexOffset;
inline constexpr std::uint32_t kPlayerPrimaryAnmVmDrawPositionOffset =
    kPlayerPrimaryAnmVmOffset + kTh07StageAnmVmDrawPositionOffset;
inline constexpr std::uint32_t kPlayerLeftOptionAnmVmDrawPositionOffset =
    kPlayerLeftOptionAnmVmOffset + kTh07StageAnmVmDrawPositionOffset;
inline constexpr std::uint32_t kPlayerRightOptionAnmVmDrawPositionOffset =
    kPlayerRightOptionAnmVmOffset + kTh07StageAnmVmDrawPositionOffset;
inline constexpr std::uint32_t kPlayerLeftOptionPositionOffset = 0x09b4;
inline constexpr std::uint32_t kPlayerRightOptionPositionOffset = 0x09c0;
inline constexpr std::uint32_t kPlayerMovementDeltaOffset = 0x09cc;
inline constexpr std::uint32_t kPlayerMovementDeltaXOffset = kPlayerMovementDeltaOffset;
inline constexpr std::uint32_t kPlayerMovementDeltaYOffset = kPlayerMovementDeltaOffset + 4;
inline constexpr std::uint32_t kPlayerBombHorizontalSpeedMultiplierOffset = 0x23f0;
inline constexpr std::uint32_t kPlayerBombVerticalSpeedMultiplierOffset = 0x23f4;
inline constexpr std::uint32_t kPlayerCurrentPowerOffset = 0x23f8;
inline constexpr std::uint32_t kPlayerMode3LockoutTimerOffset = 0x23fc;
inline constexpr std::uint32_t kPlayerModeSoundTimerOffset = 0x2400;
inline constexpr std::uint32_t kPlayerModeStateOffset = 0x2408;
inline constexpr std::uint32_t kPlayerOptionStateOffset = 0x240a;
inline constexpr std::uint32_t kPlayerModeDrawEnabledOffset = kPlayerOptionStateOffset;
inline constexpr std::uint32_t kPlayerFocusHeldOffset = 0x240b;
inline constexpr std::uint32_t kPlayerModeTransitionRequestOffset = 0x240d;
inline constexpr std::uint32_t kPlayerOptionInterpolationPreviousFrameOffset = 0x2410;
inline constexpr std::uint32_t kPlayerOptionInterpolationSubframeOffset = 0x2414;
inline constexpr std::uint32_t kPlayerOptionInterpolationFrameOffset = 0x2418;
inline constexpr std::uint32_t kPlayerModeTransitionEffectActivePointerOffset = 0x0b7e6c;
inline constexpr std::uint32_t kPlayerPrimaryAnmScript = 0x0400;
inline constexpr std::uint32_t kPlayerLeftOptionAnmScript = 0x0480;
inline constexpr std::uint32_t kPlayerRightOptionAnmScript = 0x0481;
inline constexpr std::uint8_t kPlayerOptionStateHidden = 0;
inline constexpr std::uint8_t kPlayerOptionStateUnfocused = 1;
inline constexpr std::uint8_t kPlayerOptionStateFocusing = 2;
inline constexpr std::uint8_t kPlayerOptionStateFocused = 3;
inline constexpr std::uint8_t kPlayerOptionStateUnfocusing = 4;

inline constexpr std::uint32_t kPlayerInitFunctionAddress = 0x004423e0;
inline constexpr std::uint32_t kPlayerMovementFunctionAddress = 0x0043ee50;
inline constexpr std::uint32_t kPlayerKillBoxCollisionFunctionAddress = 0x0043e260;
inline constexpr std::uint32_t kPlayerGrazeCollisionFunctionAddress = 0x0043e3b0;
inline constexpr std::uint32_t kPlayerItemBoxCollisionFunctionAddress = 0x0043e4e0;
inline constexpr std::uint32_t kPlayerLaserHitboxFunctionAddress = 0x0043e6b0;
inline constexpr std::uint32_t kPlayerInitialZBits = 0x3efae148;
inline constexpr std::uint32_t kPlayerHitboxDepthBits = 0x40a00000;
inline constexpr std::uint32_t kPlayerGrazeBoxDepthBits = 0x40a00000;
inline constexpr std::uint32_t kPlayerGrabItemSizeXBits = 0x41400000;
inline constexpr std::uint32_t kPlayerGrabItemSizeYBits = 0x41400000;
inline constexpr std::uint32_t kPlayerGrabItemSizeZBits = 0x40a00000;

static_assert(kPlayerObjectClearSize == 0x0b7e78);
static_assert(kPlayerOriginalAnmVmPrefixSize == 0x0930);
static_assert(kPlayerOriginalAnmVmPrefixSize == kPlayerPositionOffset);
static_assert(kPlayerPositionYOffset == 0x0934);
static_assert(kPlayerPositionZOffset == 0x0938);
static_assert(kPlayerHitboxMinOffset + kPlayerVectorSize == kPlayerHitboxMaxOffset);
static_assert(kPlayerGrazeBoxMinOffset + kPlayerVectorSize == kPlayerGrazeBoxMaxOffset);
static_assert(kPlayerGrabItemBoxMinOffset + kPlayerVectorSize == kPlayerGrabItemBoxMaxOffset);
static_assert(kPlayerHitboxSizeOffset + kPlayerVectorSize == kPlayerGrazeBoxSizeOffset);
static_assert(kPlayerGrazeBoxSizeOffset + kPlayerVectorSize == kPlayerGrabItemSizeOffset);
static_assert(kPlayerLeftOptionAnmVmOffset == 0x024c);
static_assert(kPlayerRightOptionAnmVmOffset == 0x0498);
static_assert(kPlayerFourthAnmVmOffset == 0x06e4);
static_assert(kPlayerPrimaryAnmVmScriptIndexOffset == 0x01d8);
static_assert(kPlayerLeftOptionAnmVmScriptIndexOffset == 0x0424);
static_assert(kPlayerRightOptionAnmVmScriptIndexOffset == 0x0670);
static_assert(kPlayerPrimaryAnmVmDrawPositionOffset == 0x01c8);
static_assert(kPlayerLeftOptionAnmVmDrawPositionOffset == 0x0414);
static_assert(kPlayerRightOptionAnmVmDrawPositionOffset == 0x0660);
static_assert(kPlayerLeftOptionPositionOffset == kPlayerGrabItemSizeOffset + kPlayerVectorSize);
static_assert(kPlayerRightOptionPositionOffset == kPlayerLeftOptionPositionOffset + kPlayerVectorSize);
static_assert(kPlayerMovementDeltaYOffset == 0x09d0);
static_assert(kPlayerOptionStateOffset == kPlayerModeDrawEnabledOffset);
static_assert(kPlayerFocusHeldOffset == kPlayerOptionStateOffset + 1);
static_assert(kPlayerOptionInterpolationSubframeOffset ==
              kPlayerOptionInterpolationPreviousFrameOffset + 4);
static_assert(kPlayerOptionInterpolationFrameOffset == kPlayerOptionInterpolationSubframeOffset + 4);
static_assert(kPlayerOptionStateFocused == 3);

} // namespace th07
