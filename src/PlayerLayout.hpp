#pragma once

#include "Th07EffectTables.hpp"
#include "Th07Timer.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

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
inline constexpr std::uint32_t kPlayerOptionShotEffectCounterOffset = 0x240c;
inline constexpr std::uint32_t kPlayerModeTransitionRequestOffset = 0x240d;
inline constexpr std::uint32_t kPlayerOptionInterpolationPreviousFrameOffset = 0x2410;
inline constexpr std::uint32_t kPlayerOptionInterpolationSubframeOffset = 0x2414;
inline constexpr std::uint32_t kPlayerOptionInterpolationFrameOffset = 0x2418;
inline constexpr std::uint32_t kPlayerMovementDirectionStateOffset = 0x241c;
inline constexpr std::uint32_t kPlayerMovementHorizontalSpeedOffset = 0x2420;
inline constexpr std::uint32_t kPlayerMovementVerticalSpeedOffset = 0x2424;
inline constexpr std::uint32_t kPlayerOptionResetLeftXOffset = 0x2428;
inline constexpr std::uint32_t kPlayerOptionResetLeftYOffset = 0x242c;
inline constexpr std::uint32_t kPlayerOptionResetLeftZOffset = 0x2430;
inline constexpr std::uint32_t kPlayerOptionResetRightXOffset = 0x2434;
inline constexpr std::uint32_t kPlayerOptionResetRightYOffset = 0x2438;
inline constexpr std::uint32_t kPlayerOptionResetRightZOffset = 0x243c;
inline constexpr std::uint32_t kPlayerOptionResetAuxOffset = 0x2440;
inline constexpr std::uint32_t kPlayerBombCommonEffectXOffset = 0x16a00;
inline constexpr std::uint32_t kPlayerBombCommonEffectYOffset = 0x16a04;
inline constexpr std::uint32_t kPlayerBombCommonEffectDurationOffset = 0x16a08;
inline constexpr std::uint32_t kPlayerMode4EffectStartXOffset = 0x16a0c;
inline constexpr std::uint32_t kPlayerMode4EffectStartYOffset = 0x16a10;
inline constexpr std::uint32_t kPlayerMode4EffectStartDurationOffset = 0x16a14;
inline constexpr std::uint32_t kPlayerBombCommonEffectActivePointerOffset = 0x0b7e68;
inline constexpr std::uint32_t kPlayerModeTransitionEffectActivePointerOffset = 0x0b7e6c;
inline constexpr std::uint32_t kPlayerShotDataPointerOffset = 0x0b7e70;
inline constexpr std::uint32_t kPlayerFocusedShotDataPointerOffset = 0x0b7e74;
inline constexpr std::uint32_t kPlayerBombCommonEffectXGlobalAddress =
    kPlayerObjectAddress + kPlayerBombCommonEffectXOffset;
inline constexpr std::uint32_t kPlayerBombCommonEffectYGlobalAddress =
    kPlayerObjectAddress + kPlayerBombCommonEffectYOffset;
inline constexpr std::uint32_t kPlayerBombCommonEffectDurationGlobalAddress =
    kPlayerObjectAddress + kPlayerBombCommonEffectDurationOffset;
inline constexpr std::uint32_t kPlayerLegacyBridgeEndOffset = 0x0b310;
inline constexpr std::uint32_t kPlayerPrimaryAnmScript = 0x0400;
inline constexpr std::uint32_t kPlayerLeftOptionAnmScript = 0x0480;
inline constexpr std::uint32_t kPlayerRightOptionAnmScript = 0x0481;
inline constexpr std::uint32_t kPlayerMovementLeanLeftAnmScript = 0x0401;
inline constexpr std::uint32_t kPlayerMovementCenterFromLeftAnmScript = 0x0402;
inline constexpr std::uint32_t kPlayerMovementLeanRightAnmScript = 0x0403;
inline constexpr std::uint32_t kPlayerMovementCenterFromRightAnmScript = 0x0404;
inline constexpr std::uint32_t kPlayerMovementLeanLeftAnmTableOffset = 0x29ef4;
inline constexpr std::uint32_t kPlayerMovementCenterFromLeftAnmTableOffset = 0x29ef8;
inline constexpr std::uint32_t kPlayerMovementLeanRightAnmTableOffset = 0x29efc;
inline constexpr std::uint32_t kPlayerMovementCenterFromRightAnmTableOffset = 0x29f00;
inline constexpr std::uint8_t kPlayerOptionStateHidden = 0;
inline constexpr std::uint8_t kPlayerOptionStateUnfocused = 1;
inline constexpr std::uint8_t kPlayerOptionStateFocusing = 2;
inline constexpr std::uint8_t kPlayerOptionStateFocused = 3;
inline constexpr std::uint8_t kPlayerOptionStateUnfocusing = 4;
inline constexpr std::uint8_t kPlayerFocusNotHeld = 0;
inline constexpr std::uint8_t kPlayerFocusHeld = 1;

inline constexpr std::uint32_t kPlayerInitFunctionAddress = 0x004423e0;
inline constexpr std::uint32_t kPlayerMovementFunctionAddress = 0x0043ee50;
inline constexpr std::uint32_t kPlayerMovementInputWordAddress = 0x004b9e50;
inline constexpr std::uint32_t kPlayerMovementAnmManagerAddress = 0x004b9e44;
inline constexpr std::uint32_t kPlayerMovementZeroFloatAddress = kTh07TimerZeroConstantAddress;
inline constexpr std::uint32_t kPlayerMovementTimerMultiplierGlobalAddress = 0x00575ac8;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMinXAddress = 0x0062f874;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMinYAddress = 0x0062f878;
inline constexpr std::uint32_t kPlayerMovementPlayfieldSpanXAddress = 0x0062f87c;
inline constexpr std::uint32_t kPlayerMovementPlayfieldSpanYAddress = 0x0062f880;
inline constexpr std::uint32_t kPlayerShotUnfocusedOrthogonalSpeedOffset = 0x0024;
inline constexpr std::uint32_t kPlayerShotFocusedOrthogonalSpeedOffset = 0x0028;
inline constexpr std::uint32_t kPlayerShotUnfocusedDiagonalSpeedOffset = 0x002c;
inline constexpr std::uint32_t kPlayerShotFocusedDiagonalSpeedOffset = 0x0030;
inline constexpr std::uint16_t kPlayerMovementInputMaskShoot = 0x0001;
inline constexpr std::uint16_t kPlayerMovementInputMaskFocus = 0x0004;
inline constexpr std::uint16_t kPlayerMovementInputMaskUp = 0x0010;
inline constexpr std::uint16_t kPlayerMovementInputMaskDown = 0x0020;
inline constexpr std::uint16_t kPlayerMovementInputMaskLeft = 0x0040;
inline constexpr std::uint16_t kPlayerMovementInputMaskRight = 0x0080;
inline constexpr std::uint32_t kPlayerDirectionStateNone = 0;
inline constexpr std::uint32_t kPlayerDirectionStateUp = 1;
inline constexpr std::uint32_t kPlayerDirectionStateDown = 2;
inline constexpr std::uint32_t kPlayerDirectionStateLeft = 3;
inline constexpr std::uint32_t kPlayerDirectionStateRight = 4;
inline constexpr std::uint32_t kPlayerDirectionStateUpLeft = 5;
inline constexpr std::uint32_t kPlayerDirectionStateUpRight = 6;
inline constexpr std::uint32_t kPlayerDirectionStateDownLeft = 7;
inline constexpr std::uint32_t kPlayerDirectionStateDownRight = 8;
inline constexpr std::uint32_t kPlayerMovementDirectionStateClearAddress = 0x0043ee82;
inline constexpr std::uint32_t kPlayerMovementInputUpReadAddress = 0x0043ee8c;
inline constexpr std::uint32_t kPlayerMovementInputDownReadAddress = 0x0043eeee;
inline constexpr std::uint32_t kPlayerMovementInputLeftReadAddress = 0x0043ef4d;
inline constexpr std::uint32_t kPlayerMovementInputRightReadAddress = 0x0043ef6b;
inline constexpr std::uint32_t kPlayerMovementInputFocusReadAddress = 0x0043ef8c;
inline constexpr std::uint32_t kPlayerMovementFocusHeldWriteAddress = 0x0043efa4;
inline constexpr std::uint32_t kPlayerMovementFocusNotHeldWriteAddress = 0x0043f0ba;
inline constexpr std::uint32_t kPlayerMovementFocusedOrthogonalSpeedFirstLoadAddress = 0x0043eff2;
inline constexpr std::uint32_t kPlayerMovementFocusedDiagonalSpeedFirstLoadAddress = 0x0043f04f;
inline constexpr std::uint32_t kPlayerMovementUnfocusedOrthogonalSpeedFirstLoadAddress = 0x0043f108;
inline constexpr std::uint32_t kPlayerMovementUnfocusedDiagonalSpeedFirstLoadAddress = 0x0043f165;
inline constexpr std::uint32_t kPlayerMovementLeanLeftScriptWriteAddress = 0x0043f1fd;
inline constexpr std::uint32_t kPlayerMovementCenterFromLeftScriptWriteAddress = 0x0043f261;
inline constexpr std::uint32_t kPlayerMovementLeanRightScriptWriteAddress = 0x0043f2c4;
inline constexpr std::uint32_t kPlayerMovementCenterFromRightScriptWriteAddress = 0x0043f329;
inline constexpr std::uint32_t kPlayerMovementLeanLeftApplyScriptCallAddress = 0x0043f223;
inline constexpr std::uint32_t kPlayerMovementCenterFromLeftApplyScriptCallAddress = 0x0043f287;
inline constexpr std::uint32_t kPlayerMovementLeanRightApplyScriptCallAddress = 0x0043f2ea;
inline constexpr std::uint32_t kPlayerMovementCenterFromRightApplyScriptCallAddress = 0x0043f34f;
inline constexpr std::uint32_t kPlayerMovementHorizontalSpeedStoreAddress = 0x0043f35d;
inline constexpr std::uint32_t kPlayerMovementVerticalSpeedStoreAddress = 0x0043f36c;
inline constexpr std::uint32_t kPlayerMovementHorizontalMultiplierReadAddress = 0x0043f37b;
inline constexpr std::uint32_t kPlayerMovementVerticalMultiplierReadAddress = 0x0043f39c;
inline constexpr std::uint32_t kPlayerMovementHorizontalTimerMultiplierReadAddress = 0x0043f381;
inline constexpr std::uint32_t kPlayerMovementVerticalTimerMultiplierReadAddress = 0x0043f3a2;
inline constexpr std::uint32_t kPlayerMovementHorizontalDeltaStoreAddress = 0x0043f38d;
inline constexpr std::uint32_t kPlayerMovementVerticalDeltaStoreAddress = 0x0043f3ae;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMinXCompareAddress = 0x0043f41c;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMaxXStoreAddress = 0x0043f46e;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMinYCompareAddress = 0x0043f480;
inline constexpr std::uint32_t kPlayerMovementPlayfieldMaxYStoreAddress = 0x0043f4d2;
inline constexpr std::uint32_t kPlayerSpawnBulletsFunctionAddress = 0x0043d160;
inline constexpr std::uint32_t kPlayerUpdatePlayerBulletsFunctionAddress = 0x0043d2f0;
inline constexpr std::uint32_t kPlayerSpawnBulletsPowerOwnerPointerAddress = 0x00626278;
inline constexpr std::uint32_t kPlayerSpawnBulletsCurrentPowerFloatOffset = 0x007c;
inline constexpr std::uint32_t kPlayerSpawnBulletsFloatToIntFunctionAddress = 0x0048b8a0;
inline constexpr std::uint32_t kPlayerSpawnBulletsDefaultCallbackFunctionAddress = 0x0043bdc0;
inline constexpr std::uint32_t kPlayerShotSlotArrayOffset = 0x2444;
inline constexpr std::uint32_t kPlayerShotSlotStride = 0x0364;
inline constexpr std::uint32_t kPlayerShotSlotCount = 0x60;
inline constexpr std::uint32_t kPlayerShotSlotAnmSpritePointerOffset = 0x01e4;
inline constexpr std::uint32_t kPlayerShotSlotPendingInterruptOffset = 0x01c6;
inline constexpr std::uint32_t kPlayerShotSlotPositionXOffset = 0x024c;
inline constexpr std::uint32_t kPlayerShotSlotPositionYOffset = 0x0250;
inline constexpr std::uint32_t kPlayerShotSlotPositionZOffset = 0x0254;
inline constexpr std::uint32_t kPlayerShotSlotSizeXOffset = 0x0318;
inline constexpr std::uint32_t kPlayerShotSlotSizeYOffset = 0x031c;
inline constexpr std::uint32_t kPlayerShotSlotVelocityXOffset = 0x0324;
inline constexpr std::uint32_t kPlayerShotSlotVelocityYOffset = 0x0328;
inline constexpr std::uint32_t kPlayerShotSlotTimerPreviousOffset = 0x033c;
inline constexpr std::uint32_t kPlayerShotSlotTimerSubframeOffset = 0x0340;
inline constexpr std::uint32_t kPlayerShotSlotTimerCurrentOffset = 0x0344;
inline constexpr std::uint32_t kPlayerShotSlotDamageOffset = 0x0348;
inline constexpr std::uint32_t kPlayerShotSlotRenderFlagsOffset = 0x01c0;
inline constexpr std::uint32_t kPlayerShotSlotRenderFlagsActiveBit = 0x00001000;
inline constexpr std::uint32_t kPlayerShotSlotRenderFlagsRotationDirtyBit = 0x00000004;
inline constexpr std::uint32_t kPlayerShotSlotAutoRotateOffset = 0x01c4;
inline constexpr std::uint32_t kPlayerShotSlotDrawPositionXOffset = 0x01c8;
inline constexpr std::uint32_t kPlayerShotSlotDrawPositionYOffset = 0x01cc;
inline constexpr std::uint32_t kPlayerShotSlotDrawPositionZOffset = 0x01d0;
inline constexpr std::uint32_t kPlayerShotSlotStateOffset = 0x034a;
inline constexpr std::uint32_t kPlayerShotSlotTypeOffset = 0x034c;
inline constexpr std::uint32_t kPlayerShotSlotUpdateCallbackOffset = 0x0354;
inline constexpr std::uint32_t kPlayerShotSlotDrawCallbackOffset = 0x0358;
inline constexpr std::uint32_t kPlayerShotSlotCollisionCallbackOffset = 0x035c;
inline constexpr std::uint32_t kPlayerShotSlotShtRecordPointerOffset = 0x0360;
inline constexpr std::uint32_t kPlayerShotSlotAngleOffset = 0x0338;
inline constexpr std::uint32_t kPlayerShotSlotScriptIndexOffset = 0x01d8;
inline constexpr std::uint32_t kPlayerShotSlotActiveState = 0x0001;
inline constexpr std::uint32_t kPlayerShotSlotCollidedState = 0x0002;
inline constexpr std::uint32_t kPlayerShotDrawActiveFunctionAddress = 0x0043d690;
inline constexpr std::uint32_t kPlayerShotDrawCollidedFunctionAddress = 0x0043d790;
inline constexpr std::uint32_t kPlayerShotDrawNormalizeAngleFunctionAddress = kTh07AnmVmNormalizeAngleFunctionAddress;
inline constexpr std::uint32_t kPlayerShotDrawAnmVmFunctionAddress = kTh07AnmVmDrawScreenQuadFunctionAddress;
inline constexpr std::uint32_t kPlayerShotDrawAnmManagerPointerAddress = kPlayerMovementAnmManagerAddress;
inline constexpr std::uint32_t kPlayerShotDrawOffsetXAddress = 0x0062f864;
inline constexpr std::uint32_t kPlayerShotDrawOffsetYAddress = 0x0062f868;
inline constexpr std::uint32_t kPlayerShotDrawHalfPiBits = 0x3fc90fdb;
inline constexpr std::uint32_t kPlayerShotDrawZBits = 0x3ecccccd;
inline constexpr float kPlayerShotDrawZ = 0.4f;
inline constexpr std::uint32_t kPlayerFireTimerFunctionAddress = 0x0043d880;
inline constexpr std::uint32_t kPlayerFireTimerStartFunctionAddress = 0x0043d990;
inline constexpr std::uint32_t kPlayerFireTimerPreviousOffset = 0x169f4;
inline constexpr std::uint32_t kPlayerFireTimerSubframeOffset = 0x169f8;
inline constexpr std::uint32_t kPlayerFireTimerCurrentOffset = 0x169fc;
inline constexpr std::int32_t kPlayerFireTimerStopValue = -1;
inline constexpr std::int32_t kPlayerFireTimerStartValue = 0;
inline constexpr std::int32_t kPlayerFireTimerStopFrame = 30;
inline constexpr std::uint32_t kPlayerFireTimerPreviousSentinelBits = kTh07TimerPreviousSentinelBits;
inline constexpr std::uint32_t kPlayerFireTimerSubframeZeroBits = kTh07TimerZeroBits;
inline constexpr std::uint32_t kPlayerFireTimerReplayGateAddress = 0x004d44f8;
inline constexpr std::uint32_t kPlayerFireTimerReplayGateByte0Address = 0x0062f645;
inline constexpr std::uint32_t kPlayerFireTimerReplayGateByte1Address = 0x0062f646;
inline constexpr std::uint32_t kPlayerDamageToEnemyFunctionAddress = 0x0043d9e0;
inline constexpr std::uint32_t kPlayerDamageHalfConstantAddress = 0x00498a50;
inline constexpr std::uint32_t kPlayerDamageZeroConstantAddress = kTh07TimerZeroConstantAddress;
inline constexpr std::uint32_t kPlayerDamageOneFrameConstantAddress = kTh07TimerOneFrameConstantAddress;
inline constexpr std::uint32_t kPlayerDamageTwoConstantAddress = 0x00498a70;
inline constexpr std::uint32_t kPlayerDamageShotVelocityDivisorAddress = 0x00498a7c;
inline constexpr std::uint32_t kPlayerDamageBombActiveFlagOffset = 0x16a20;
inline constexpr std::uint32_t kPlayerDamageCollisionScriptDelta = 0x20;
inline constexpr std::uint32_t kPlayerDamageCollisionScriptTableOffset = 0x28ef0;
inline constexpr std::uint32_t kPlayerDamageCollisionSetScriptFunctionAddress =
    kTh07AnmVmSetScriptFunctionAddress;
inline constexpr std::uint32_t kPlayerDamageCollisionEffectZBits = 0x3dcccccd;
inline constexpr std::int32_t kPlayerDamageShotEffectId = 5;
inline constexpr std::int32_t kPlayerDamageBombEffectLowId = 3;
inline constexpr std::int32_t kPlayerDamageBombEffectHighId = 5;
inline constexpr std::uint32_t kPlayerDamageEffectSpawnCount = 1;
inline constexpr std::uint32_t kPlayerDamageEffectColor = 0xffffffff;
inline constexpr std::uint32_t kPlayerDamageLaserTimerParityMask = 0x80000001;
inline constexpr std::uint32_t kPlayerDamageBombEffectCadenceMask = 0x03;
inline constexpr std::uint32_t kPlayerDamageBombRegionPositionOffset = 0x09dc;
inline constexpr std::uint32_t kPlayerDamageBombRegionSizeOffset = 0x09e8;
inline constexpr std::uint32_t kPlayerDamageBombRegionDamageOffset = 0x09f4;
inline constexpr std::uint32_t kPlayerDamageBombRegionAccumulatedDamageOffset = 0x09f8;
inline constexpr std::uint32_t kPlayerDamageBombRegionStride = 0x20;
inline constexpr std::uint32_t kPlayerDamageBombRegionCount = 0x70;
inline constexpr std::uint32_t kPlayerDamageBombRegionLowEffectCount = 0x60;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtPowerTableOffset = 0x0034;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtPowerEntryStride = 0x0008;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtPowerRecordPointerOffset = 0x0000;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtPowerThresholdOffset = 0x0004;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtShotRecordStride = 0x0034;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtShotRecordTerminatorOffset = 0x0000;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtSpawnCallbackOffset = 0x0024;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtUpdateCallbackOffset = 0x0028;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtDrawCallbackOffset = 0x002c;
inline constexpr std::uint32_t kPlayerSpawnBulletsShtCollisionCallbackOffset = 0x0030;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectTimerArrayOffset = 0x169c4;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectCurrentOffset = 0x0008;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectPointerArrayOffset = 0x169d0;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectPointerStride = 0x0010;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectPointerCount = 0x0003;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectFocusedPointerOffset = 0x169f0;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectMode2GateOffset = 0x169fc;
inline constexpr std::int32_t kPlayerUpdateTrackedShotEffectMaxDuration = 999;
inline constexpr std::int32_t kPlayerUpdateTrackedShotEffectClampDuration = 50;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectPreviousSentinelBits = 0xfffffc19;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectZeroBits = 0x00000000;
inline constexpr std::uint32_t kPlayerUpdateTrackedShotEffectPendingInterrupt = 1;
inline constexpr std::int16_t kPlayerUpdateLaserTypeFirst = 4;
inline constexpr std::int16_t kPlayerUpdateLaserTypeSecond = 5;
inline constexpr std::uint32_t kPlayerUpdateBoundsOwnerAddress = 0x00626270;
inline constexpr std::uint32_t kPlayerUpdateBoundsFunctionAddress = 0x0042d6d8;
inline constexpr std::uint32_t kPlayerUpdateAnmManagerPointerAddress = kPlayerMovementAnmManagerAddress;
inline constexpr std::uint32_t kPlayerUpdateAnmVmStepFunctionAddress = 0x00450d60;
inline constexpr std::uint32_t kPlayerUpdateTimerHelperOwnerAddress = 0x00575950;
inline constexpr std::uint32_t kPlayerUpdateTimerHelperFunctionAddress = 0x0043958d;
inline constexpr std::uint32_t kPlayerUpdateTimerDecrementFunctionAddress = 0x004394c7;
inline constexpr std::uint32_t kPlayerUpdateLoadedSpriteWidthOffset = 0x002c;
inline constexpr std::uint32_t kPlayerUpdateLoadedSpriteHeightOffset = 0x0030;
inline constexpr std::uint32_t kPlayerKillBoxCollisionFunctionAddress = 0x0043e260;
inline constexpr std::uint32_t kPlayerGrazeCollisionFunctionAddress = 0x0043e3b0;
inline constexpr std::uint32_t kPlayerItemBoxCollisionFunctionAddress = 0x0043e4e0;
inline constexpr std::uint32_t kPlayerLaserHitboxFunctionAddress = 0x0043e6b0;
inline constexpr std::uint32_t kPlayerOptionShotEffectCallbackFunctionAddress = 0x0043d0e0;
inline constexpr std::uint32_t kPlayerBombCollisionFunctionAddress = 0x0043e0a0;
inline constexpr std::uint32_t kPlayerBombCollisionUpdateFunctionAddress = 0x00440940;
inline constexpr std::uint32_t kPlayerBombCollisionRegisterRectFunctionAddress = 0x00441800;
inline constexpr std::uint32_t kPlayerBombCollisionRegisterCircleFunctionAddress = 0x004418b0;
inline constexpr std::uint32_t kPlayerMode1InvulnerableUpdateFunctionAddress = 0x004411c0;
inline constexpr std::uint32_t kPlayerOptionFieldResetFunctionAddress = 0x00441e80;
inline constexpr std::uint32_t kPlayerOptionResetPendingInterruptMirrorAddress = 0x0134d476;
inline constexpr std::uint32_t kPlayerOptionResetPendingInterruptStateAddress = 0x013542ec;
inline constexpr std::uint32_t kPlayerOptionResetSentinelBits = 0xc479c000;
inline constexpr std::uint32_t kPlayerOptionResetZeroBits = 0x00000000;
inline constexpr std::uint32_t kPlayerOptionResetTopYThresholdAddress = 0x00498afc;
inline constexpr std::uint32_t kPlayerOptionResetRightXThresholdAddress = 0x00498ac0;
inline constexpr float kPlayerOptionResetTopYThreshold = 400.0f;
inline constexpr float kPlayerOptionResetRightXThreshold = 160.0f;
inline constexpr std::uint32_t kPlayerOptionResetStateEntering = 2;
inline constexpr std::uint32_t kPlayerOptionResetStateInterrupted = 3;
inline constexpr std::uint32_t kPlayerModeConfigPointerAddress = 0x00575948;
inline constexpr std::uint32_t kPlayerModeConfigCurrentPowerCapOffset = 0x0008;
inline constexpr std::uint32_t kPlayerMode1SoundTimerValue = 0x0000003c;
inline constexpr std::uint32_t kPlayerMode1BlendFlag = 0x00000010;
inline constexpr std::uint32_t kPlayerMode1BlendFlagClearMask = 0xffffffef;
inline constexpr std::uint32_t kPlayerMode1WhiteRgbBits = 0x00ffffff;
inline constexpr std::uint32_t kPlayerMode1WhiteArgbBits = 0xffffffff;
inline constexpr std::uint32_t kPlayerMode1SpeedMultiplierBits = 0x3f800000;
inline constexpr std::uint32_t kPlayerMode1FadeScaleYMultiplierBits = 0x40000000;
inline constexpr float kPlayerMode1FadeScaleYMultiplier = 2.0f;
inline constexpr std::int32_t kPlayerMode1FadeFrames = 0x1e;
inline constexpr float kPlayerMode1FadeFramesFloat = 30.0f;
inline constexpr std::uint32_t kPlayerMode1FadeFramesConstantAddress = 0x00498b00;
inline constexpr std::uint32_t kPlayerMode1OneFrameConstantAddress = kTh07TimerOneFrameConstantAddress;
inline constexpr std::uint32_t kPlayerMode1ScaleYMultiplierAddress = 0x00498a70;
inline constexpr std::uint8_t kPlayerMode1CompleteModeState = 3;
inline constexpr std::int32_t kPlayerMode1CompleteCommonEffectDuration = 0x000000f0;
inline constexpr std::uint32_t kPlayerOptionShotEffectSourceXOffset = 0x024c;
inline constexpr std::uint8_t kPlayerOptionShotEffectCounterMask = 0x07;
inline constexpr std::int32_t kPlayerOptionShotEffectSpawnId = 5;
inline constexpr std::int32_t kPlayerOptionShotEffectSpawnCount = 1;
inline constexpr std::uint32_t kPlayerOptionShotEffectSpawnColor = 0xffffffff;
inline constexpr std::uint32_t kPlayerOptionShotEffectManagerAddress = kTh07EffectManagerAddress;
inline constexpr std::uint32_t kPlayerOptionShotEffectSpawnFunctionAddress = kTh07EffectSpawnFunctionAddress;
inline constexpr std::uint32_t kPlayerBombCollisionBulletClearOffset = 0x09e8;
inline constexpr std::uint32_t kPlayerBombCollisionBulletClearStride = 0x20;
inline constexpr std::uint32_t kPlayerBombCollisionBulletClearCount = 0x70;
inline constexpr std::uint32_t kPlayerBombCollisionSlotArrayOffset = 0x17dc;
inline constexpr std::uint32_t kPlayerBombCollisionSlotStride = 0x20;
inline constexpr std::uint32_t kPlayerBombCollisionSlotCount = 0x60;
inline constexpr std::uint32_t kPlayerBombCollisionSlotXOffset = 0x00;
inline constexpr std::uint32_t kPlayerBombCollisionSlotYOffset = 0x04;
inline constexpr std::uint32_t kPlayerBombCollisionSlotRectSizeXOffset = 0x08;
inline constexpr std::uint32_t kPlayerBombCollisionSlotRectSizeYOffset = 0x0c;
inline constexpr std::uint32_t kPlayerBombCollisionSlotRadiusOffset = 0x10;
inline constexpr std::uint32_t kPlayerBombCollisionSlotRadiusVelocityOffset = 0x14;
inline constexpr std::uint32_t kPlayerBombCollisionSlotFramesLeftOffset = 0x18;
inline constexpr std::uint32_t kPlayerBombCollisionSlotPayloadOffset = 0x1c;
inline constexpr std::uint32_t kPlayerCollisionPayloadOffset = 0x2404;
inline constexpr std::uint32_t kPlayerCollisionDefaultPayloadBits = 0x00000006;
inline constexpr float kPlayerCollisionHalfScale = 1.0f / 2.0f;
inline constexpr float kPlayerGrazeCollisionMargin = 20.0f;
inline constexpr float kPlayerBombCollisionHalfScale = 1.0f / 2.0f;
inline constexpr std::uint32_t kPlayerItemBoxCollisionModeStateOffset = kPlayerModeStateOffset;
inline constexpr std::uint32_t kPlayerItemBoxCollisionGrabMinOffset = kPlayerGrabItemBoxMinOffset;
inline constexpr std::uint32_t kPlayerItemBoxCollisionGrabMaxOffset = kPlayerGrabItemBoxMaxOffset;
inline constexpr float kPlayerItemBoxCollisionHalfScale = 1.0f / 2.0f;
inline constexpr std::uint32_t kPlayerInitialZBits = 0x3efae148;
inline constexpr std::uint32_t kPlayerHitboxDepthBits = 0x40a00000;
inline constexpr std::uint32_t kPlayerGrazeBoxDepthBits = 0x40a00000;
inline constexpr std::uint32_t kPlayerGrabItemSizeXBits = 0x41400000;
inline constexpr std::uint32_t kPlayerGrabItemSizeYBits = 0x41400000;
inline constexpr std::uint32_t kPlayerGrabItemSizeZBits = 0x40a00000;
inline constexpr std::size_t kPlayerMovementHorizontalAnimationTransitionCount = 4;

struct PlayerMovementInputContract {
    std::uint32_t functionAddress;
    std::uint32_t inputWordAddress;
    std::uint32_t directionStateOffset;
    std::uint32_t focusHeldOffset;
    std::uint32_t shotDataPointerOffset;
    std::uint32_t unfocusedOrthogonalSpeedOffset;
    std::uint32_t focusedOrthogonalSpeedOffset;
    std::uint32_t unfocusedDiagonalSpeedOffset;
    std::uint32_t focusedDiagonalSpeedOffset;
    std::uint32_t horizontalSpeedOffset;
    std::uint32_t verticalSpeedOffset;
    std::uint32_t horizontalDeltaOffset;
    std::uint32_t verticalDeltaOffset;
    std::uint32_t horizontalSpeedMultiplierOffset;
    std::uint32_t verticalSpeedMultiplierOffset;
    std::uint32_t timerMultiplierAddress;
    std::uint32_t playfieldMinXAddress;
    std::uint32_t playfieldMinYAddress;
    std::uint32_t playfieldSpanXAddress;
    std::uint32_t playfieldSpanYAddress;
    std::string_view evidence;
};

struct PlayerItemBoxCollisionInput {
    std::uint8_t modeState;
    float itemCenterX;
    float itemCenterY;
    float itemSizeX;
    float itemSizeY;
    float grabMinX;
    float grabMinY;
    float grabMaxX;
    float grabMaxY;
};

struct PlayerAabbCollisionInput {
    float centerX;
    float centerY;
    float sizeX;
    float sizeY;
    float boxMinX;
    float boxMinY;
    float boxMaxX;
    float boxMaxY;
    float margin;
};

struct Th07PlayerBombCollisionSlot {
    float x;
    float y;
    float rectSizeX;
    float rectSizeY;
    float radius;
    float radiusVelocity;
    std::int32_t framesLeft;
    std::uint32_t payloadBits;
};

struct PlayerBombCollisionInput {
    float centerX;
    float centerY;
    float sizeX;
    float sizeY;
};

struct PlayerBombCollisionResult {
    bool hit;
    std::uint32_t payloadBits;
};

struct PlayerOptionFieldResetState {
    std::uint32_t leftXBits;
    std::uint32_t leftYBits;
    std::uint32_t leftZBits;
    std::uint32_t rightXBits;
    std::uint32_t rightYBits;
    std::uint32_t rightZBits;
    std::uint32_t auxBits;
    std::uint32_t pendingInterruptState;
    std::uint32_t pendingInterruptMirror;
    bool writesPendingInterrupt;
};

struct PlayerMode1InvulnerableInput {
    std::int32_t commonEffectDuration;
    float commonEffectSubframe;
    std::uint32_t renderFlags;
    std::int32_t currentPowerCap;
};

struct PlayerMode1InvulnerableState {
    std::uint32_t modeSoundTimer;
    float primaryScaleX;
    float primaryScaleY;
    std::uint32_t primaryColorBits;
    std::uint32_t renderFlags;
    std::uint32_t horizontalSpeedMultiplierBits;
    std::uint32_t verticalSpeedMultiplierBits;
    std::int32_t currentPower;
    std::uint8_t modeState;
    std::int32_t commonEffectPrevious;
    float commonEffectSubframe;
    std::int32_t commonEffectDuration;
    bool completed;
};

struct PlayerOptionShotEffectInput {
    std::uint8_t counter;
    float sourceX;
    float impactY;
    float impactZ;
};

struct PlayerOptionShotEffectState {
    std::uint8_t counter;
    bool shouldSpawn;
    std::int32_t effectId;
    std::int32_t spawnCount;
    std::uint32_t color;
    float spawnX;
    float spawnY;
    float spawnZ;
    std::int32_t returnValue;
};

struct PlayerSpawnBulletsContract {
    std::uint32_t functionAddress;
    std::uint32_t focusHeldOffset;
    std::uint32_t unfocusedShotDataPointerOffset;
    std::uint32_t focusedShotDataPointerOffset;
    std::uint32_t shotSlotArrayOffset;
    std::uint32_t shotSlotStride;
    std::uint32_t shotSlotCount;
    std::uint32_t slotStateOffset;
    std::uint32_t slotRenderFlagsOffset;
    std::uint32_t slotRecordPointerOffset;
    std::uint32_t shtPowerTableOffset;
    std::uint32_t shtPowerEntryStride;
    std::uint32_t shtShotRecordStride;
    std::uint32_t defaultSpawnCallbackAddress;
    std::string_view evidence;
};

struct PlayerUpdateBulletsContract {
    std::uint32_t functionAddress;
    std::uint32_t modeStateOffset;
    std::uint32_t optionStateOffset;
    std::uint32_t shotSlotArrayOffset;
    std::uint32_t shotSlotStride;
    std::uint32_t shotSlotCount;
    std::uint32_t trackedEffectTimerArrayOffset;
    std::uint32_t trackedEffectPointerArrayOffset;
    std::uint32_t trackedEffectPointerStride;
    std::uint32_t trackedEffectPointerCount;
    std::uint32_t updateCallbackOffset;
    std::uint32_t boundsFunctionAddress;
    std::uint32_t anmVmStepFunctionAddress;
    std::uint32_t timerHelperFunctionAddress;
    std::string_view evidence;
};

enum class PlayerHorizontalAnimationTransitionKind {
    LeanLeft,
    CenterFromLeft,
    LeanRight,
    CenterFromRight,
};

struct PlayerHorizontalAnimationTransitionContract {
    PlayerHorizontalAnimationTransitionKind kind;
    std::uint32_t scriptIndex;
    std::uint32_t scriptTableOffset;
    std::uint32_t scriptWriteAddress;
    std::uint32_t applyScriptCallAddress;
    std::string_view evidence;
};

inline constexpr PlayerMovementInputContract kPlayerMovementInputContract{
    kPlayerMovementFunctionAddress,
    kPlayerMovementInputWordAddress,
    kPlayerMovementDirectionStateOffset,
    kPlayerFocusHeldOffset,
    kPlayerShotDataPointerOffset,
    kPlayerShotUnfocusedOrthogonalSpeedOffset,
    kPlayerShotFocusedOrthogonalSpeedOffset,
    kPlayerShotUnfocusedDiagonalSpeedOffset,
    kPlayerShotFocusedDiagonalSpeedOffset,
    kPlayerMovementHorizontalSpeedOffset,
    kPlayerMovementVerticalSpeedOffset,
    kPlayerMovementDeltaXOffset,
    kPlayerMovementDeltaYOffset,
    kPlayerBombHorizontalSpeedMultiplierOffset,
    kPlayerBombVerticalSpeedMultiplierOffset,
    kPlayerMovementTimerMultiplierGlobalAddress,
    kPlayerMovementPlayfieldMinXAddress,
    kPlayerMovementPlayfieldMinYAddress,
    kPlayerMovementPlayfieldSpanXAddress,
    kPlayerMovementPlayfieldSpanYAddress,
    "FUN_0043ee50:23-163",
};

inline constexpr PlayerSpawnBulletsContract kPlayerSpawnBulletsContract{
    kPlayerSpawnBulletsFunctionAddress,
    kPlayerFocusHeldOffset,
    kPlayerShotDataPointerOffset,
    kPlayerFocusedShotDataPointerOffset,
    kPlayerShotSlotArrayOffset,
    kPlayerShotSlotStride,
    kPlayerShotSlotCount,
    kPlayerShotSlotStateOffset,
    kPlayerShotSlotRenderFlagsOffset,
    kPlayerShotSlotShtRecordPointerOffset,
    kPlayerSpawnBulletsShtPowerTableOffset,
    kPlayerSpawnBulletsShtPowerEntryStride,
    kPlayerSpawnBulletsShtShotRecordStride,
    kPlayerSpawnBulletsDefaultCallbackFunctionAddress,
    "FUN_0043d160: chooses SHT table by focus byte, selects power row, scans raw shot slots",
};

inline constexpr PlayerUpdateBulletsContract kPlayerUpdateBulletsContract{
    kPlayerUpdatePlayerBulletsFunctionAddress,
    kPlayerModeStateOffset,
    kPlayerOptionStateOffset,
    kPlayerShotSlotArrayOffset,
    kPlayerShotSlotStride,
    kPlayerShotSlotCount,
    kPlayerUpdateTrackedShotEffectTimerArrayOffset,
    kPlayerUpdateTrackedShotEffectPointerArrayOffset,
    kPlayerUpdateTrackedShotEffectPointerStride,
    kPlayerUpdateTrackedShotEffectPointerCount,
    kPlayerShotSlotUpdateCallbackOffset,
    kPlayerUpdateBoundsFunctionAddress,
    kPlayerUpdateAnmVmStepFunctionAddress,
    kPlayerUpdateTimerHelperFunctionAddress,
    "FUN_0043d2f0: option tracked-shot cleanup, raw shot slot update callback, movement, bounds, VM step",
};

inline constexpr bool ShouldAdvanceTh07PlayerShotPowerLevel(std::int32_t currentPower,
                                                            std::int32_t threshold)
{
    return threshold <= currentPower;
}

inline constexpr bool IsTh07PlayerItemCollectionMode(std::uint8_t modeState)
{
    return modeState == 0 || modeState == 3 || modeState == 4;
}

inline constexpr bool CheckTh07PlayerItemBoxCollision(const PlayerItemBoxCollisionInput &input)
{
    if (!IsTh07PlayerItemCollectionMode(input.modeState)) {
        return false;
    }

    const float halfItemWidth = input.itemSizeX * kPlayerItemBoxCollisionHalfScale;
    const float halfItemHeight = input.itemSizeY * kPlayerItemBoxCollisionHalfScale;
    return !(input.itemCenterX + halfItemWidth < input.grabMinX ||
             input.grabMaxX < input.itemCenterX - halfItemWidth ||
             input.itemCenterY + halfItemHeight < input.grabMinY ||
             input.grabMaxY < input.itemCenterY - halfItemHeight);
}

inline constexpr bool CheckTh07PlayerAabbCollision(const PlayerAabbCollisionInput &input)
{
    const float halfWidth = input.sizeX * kPlayerCollisionHalfScale;
    const float halfHeight = input.sizeY * kPlayerCollisionHalfScale;
    return !(input.centerX + halfWidth + input.margin < input.boxMinX ||
             input.boxMaxX < input.centerX - halfWidth - input.margin ||
             input.centerY + halfHeight + input.margin < input.boxMinY ||
             input.boxMaxY < input.centerY - halfHeight - input.margin);
}

inline constexpr bool CheckTh07PlayerKillBoxCollision(const PlayerAabbCollisionInput &input)
{
    PlayerAabbCollisionInput exactInput = input;
    exactInput.margin = 0.0f;
    return CheckTh07PlayerAabbCollision(exactInput);
}

inline constexpr bool CheckTh07PlayerGrazeBoxCollision(const PlayerAabbCollisionInput &input)
{
    PlayerAabbCollisionInput grazeInput = input;
    grazeInput.margin = kPlayerGrazeCollisionMargin;
    return CheckTh07PlayerAabbCollision(grazeInput);
}

inline constexpr bool IsTh07PlayerGrazeSuppressedMode(std::uint8_t modeState)
{
    return modeState == 1 || modeState == 2;
}

inline constexpr bool IsTh07PlayerBombCollisionSlotFree(const Th07PlayerBombCollisionSlot &slot)
{
    return slot.rectSizeX == 0.0f && slot.radius == 0.0f;
}

inline constexpr Th07PlayerBombCollisionSlot BuildTh07PlayerBombCollisionRectSlot(
    float x, float y, float sizeX, float sizeY, std::uint32_t payloadBits)
{
    return {x, y, sizeX, sizeY, 0.0f, 0.0f, 0, payloadBits};
}

inline constexpr Th07PlayerBombCollisionSlot BuildTh07PlayerBombCollisionCircleSlot(
    float x, float y, float radius, float radiusVelocity, std::int32_t framesLeft,
    std::uint32_t payloadBits)
{
    return {x, y, 0.0f, 0.0f, radius, radiusVelocity, framesLeft, payloadBits};
}

inline constexpr PlayerBombCollisionResult CheckTh07PlayerBombCollisionSlot(
    const Th07PlayerBombCollisionSlot &slot, const PlayerBombCollisionInput &input)
{
    if (slot.rectSizeX == 0.0f) {
        const float deltaX = input.centerX - slot.x;
        const float deltaY = input.centerY - slot.y;
        const float radiusSquared = slot.radius * slot.radius;
        if (slot.radius != 0.0f && deltaY * deltaY + deltaX * deltaX < radiusSquared) {
            return {true, slot.payloadBits};
        }
        return {false, 0};
    }

    const float halfSlotWidth = slot.rectSizeX * kPlayerBombCollisionHalfScale;
    const float halfSlotHeight = slot.rectSizeY * kPlayerBombCollisionHalfScale;
    const float halfInputWidth = input.sizeX * kPlayerBombCollisionHalfScale;
    const float halfInputHeight = input.sizeY * kPlayerBombCollisionHalfScale;
    const bool overlaps = slot.x - halfSlotWidth <= input.centerX + halfInputWidth &&
                          input.centerX - halfInputWidth <= halfSlotWidth + slot.x &&
                          slot.y - halfSlotHeight <= input.centerY + halfInputHeight &&
                          input.centerY - halfInputHeight <= halfSlotHeight + slot.y;
    return {overlaps, overlaps ? slot.payloadBits : 0};
}

inline constexpr Th07PlayerBombCollisionSlot
TickTh07PlayerBombCollisionSlot(const Th07PlayerBombCollisionSlot &slot)
{
    Th07PlayerBombCollisionSlot next = slot;
    if (next.framesLeft < 1) {
        next.radius = 0.0f;
        next.rectSizeX = 0.0f;
    }
    else {
        --next.framesLeft;
        next.radius += next.radiusVelocity;
    }
    return next;
}

inline constexpr PlayerOptionFieldResetState
BuildTh07PlayerOptionFieldResetState(float playerX, float playerY,
                                     std::uint32_t pendingInterruptState,
                                     std::uint32_t pendingInterruptMirror)
{
    PlayerOptionFieldResetState state{
        kPlayerOptionResetSentinelBits,
        kPlayerOptionResetSentinelBits,
        kPlayerOptionResetZeroBits,
        kPlayerOptionResetSentinelBits,
        kPlayerOptionResetSentinelBits,
        kPlayerOptionResetZeroBits,
        kPlayerOptionResetZeroBits,
        pendingInterruptState,
        pendingInterruptMirror,
        false,
    };

    if (playerY < kPlayerOptionResetTopYThreshold) {
        if (state.pendingInterruptState == kPlayerOptionResetStateEntering) {
            state.pendingInterruptMirror = kPlayerOptionResetStateInterrupted;
            state.pendingInterruptState = kPlayerOptionResetStateInterrupted;
            state.writesPendingInterrupt = true;
        }
    }
    else if (state.pendingInterruptState == kPlayerOptionResetStateEntering ||
             kPlayerOptionResetRightXThreshold <= playerX) {
        if (state.pendingInterruptState == kPlayerOptionResetStateEntering &&
            kPlayerOptionResetRightXThreshold < playerX) {
            state.pendingInterruptMirror = kPlayerOptionResetStateInterrupted;
            state.pendingInterruptState = kPlayerOptionResetStateInterrupted;
            state.writesPendingInterrupt = true;
        }
    }
    else {
        state.pendingInterruptMirror = kPlayerOptionResetStateEntering;
        state.pendingInterruptState = kPlayerOptionResetStateEntering;
        state.writesPendingInterrupt = true;
    }

    return state;
}

inline constexpr PlayerMode1InvulnerableState
BuildTh07PlayerMode1InvulnerableUpdateState(const PlayerMode1InvulnerableInput &input)
{
    const float fade =
        kTh07TimerOneFrame -
        (static_cast<float>(input.commonEffectDuration) + input.commonEffectSubframe) /
            kPlayerMode1FadeFramesFloat;
    const std::int32_t alpha =
        (input.commonEffectDuration * static_cast<std::int32_t>(0xff)) / kPlayerMode1FadeFrames;

    PlayerMode1InvulnerableState state{
        kPlayerMode1SoundTimerValue,
        kTh07TimerOneFrame - kTh07TimerOneFrame * fade,
        kPlayerMode1FadeScaleYMultiplier * fade + kTh07TimerOneFrame,
        (static_cast<std::uint32_t>(alpha) << 24) | kPlayerMode1WhiteRgbBits,
        input.renderFlags | kPlayerMode1BlendFlag,
        kPlayerMode1SpeedMultiplierBits,
        kPlayerMode1SpeedMultiplierBits,
        0,
        0,
        0,
        input.commonEffectSubframe,
        input.commonEffectDuration,
        false,
    };

    if (kPlayerMode1FadeFrames <= input.commonEffectDuration) {
        state.modeState = kPlayerMode1CompleteModeState;
        state.primaryScaleX = kTh07TimerOneFrame;
        state.primaryScaleY = kTh07TimerOneFrame;
        state.primaryColorBits = kPlayerMode1WhiteArgbBits;
        state.renderFlags &= kPlayerMode1BlendFlagClearMask;
        state.commonEffectDuration = kPlayerMode1CompleteCommonEffectDuration;
        state.commonEffectSubframe = kTh07TimerZeroFrame;
        state.commonEffectPrevious = kTh07TimerPreviousSentinel;
        state.currentPower = input.currentPowerCap;
        state.completed = true;
    }

    return state;
}

inline constexpr PlayerOptionShotEffectState
BuildTh07PlayerOptionShotEffectState(const PlayerOptionShotEffectInput &input)
{
    const std::uint8_t nextCounter = static_cast<std::uint8_t>(input.counter + 1);
    return {
        nextCounter,
        (nextCounter & kPlayerOptionShotEffectCounterMask) == 0,
        kPlayerOptionShotEffectSpawnId,
        kPlayerOptionShotEffectSpawnCount,
        kPlayerOptionShotEffectSpawnColor,
        input.sourceX,
        input.impactY,
        input.impactZ,
        0,
    };
}

inline constexpr std::array<PlayerHorizontalAnimationTransitionContract,
                            kPlayerMovementHorizontalAnimationTransitionCount>
    kPlayerHorizontalAnimationTransitionContracts = {{
        {PlayerHorizontalAnimationTransitionKind::LeanLeft,
         kPlayerMovementLeanLeftAnmScript,
         kPlayerMovementLeanLeftAnmTableOffset,
         kPlayerMovementLeanLeftScriptWriteAddress,
         kPlayerMovementLeanLeftApplyScriptCallAddress,
         "FUN_0043ee50:118-126; th07.exe@0043f1fd"},
        {PlayerHorizontalAnimationTransitionKind::CenterFromLeft,
         kPlayerMovementCenterFromLeftAnmScript,
         kPlayerMovementCenterFromLeftAnmTableOffset,
         kPlayerMovementCenterFromLeftScriptWriteAddress,
         kPlayerMovementCenterFromLeftApplyScriptCallAddress,
         "FUN_0043ee50:120-128; th07.exe@0043f261"},
        {PlayerHorizontalAnimationTransitionKind::LeanRight,
         kPlayerMovementLeanRightAnmScript,
         kPlayerMovementLeanRightAnmTableOffset,
         kPlayerMovementLeanRightScriptWriteAddress,
         kPlayerMovementLeanRightApplyScriptCallAddress,
         "FUN_0043ee50:130-138; th07.exe@0043f2c4"},
        {PlayerHorizontalAnimationTransitionKind::CenterFromRight,
         kPlayerMovementCenterFromRightAnmScript,
         kPlayerMovementCenterFromRightAnmTableOffset,
         kPlayerMovementCenterFromRightScriptWriteAddress,
         kPlayerMovementCenterFromRightApplyScriptCallAddress,
         "FUN_0043ee50:132-140; th07.exe@0043f329"},
    }};

struct PlayerMovementSpeedInput {
    float unfocusedOrthogonal;
    float focusedOrthogonal;
    float unfocusedDiagonal;
    float focusedDiagonal;
};

struct PlayerMovementVector {
    std::uint32_t directionState;
    bool focusHeld;
    float horizontalSpeed;
    float verticalSpeed;
};

inline const PlayerHorizontalAnimationTransitionContract *
FindPlayerHorizontalAnimationTransitionContract(PlayerHorizontalAnimationTransitionKind kind)
{
    for (const PlayerHorizontalAnimationTransitionContract &contract :
         kPlayerHorizontalAnimationTransitionContracts) {
        if (contract.kind == kind) {
            return &contract;
        }
    }
    return nullptr;
}

inline constexpr bool IsTh07PlayerFocusHeld(std::uint16_t inputMask)
{
    return (inputMask & kPlayerMovementInputMaskFocus) != 0;
}

inline constexpr std::uint32_t BuildTh07PlayerDirectionState(std::uint16_t inputMask)
{
    if ((inputMask & kPlayerMovementInputMaskUp) != 0) {
        std::uint32_t state = kPlayerDirectionStateUp;
        if ((inputMask & kPlayerMovementInputMaskLeft) != 0) {
            state = kPlayerDirectionStateUpLeft;
        }
        if ((inputMask & kPlayerMovementInputMaskRight) != 0) {
            state = kPlayerDirectionStateUpRight;
        }
        return state;
    }

    if ((inputMask & kPlayerMovementInputMaskDown) != 0) {
        std::uint32_t state = kPlayerDirectionStateDown;
        if ((inputMask & kPlayerMovementInputMaskLeft) != 0) {
            state = kPlayerDirectionStateDownLeft;
        }
        if ((inputMask & kPlayerMovementInputMaskRight) != 0) {
            state = kPlayerDirectionStateDownRight;
        }
        return state;
    }

    std::uint32_t state = kPlayerDirectionStateNone;
    if ((inputMask & kPlayerMovementInputMaskLeft) != 0) {
        state = kPlayerDirectionStateLeft;
    }
    if ((inputMask & kPlayerMovementInputMaskRight) != 0) {
        state = kPlayerDirectionStateRight;
    }
    return state;
}

inline constexpr PlayerMovementVector
BuildTh07PlayerMovementVector(std::uint16_t inputMask, const PlayerMovementSpeedInput &speeds)
{
    const std::uint32_t state = BuildTh07PlayerDirectionState(inputMask);
    const bool focusHeld = IsTh07PlayerFocusHeld(inputMask);
    const float orthogonal = focusHeld ? speeds.focusedOrthogonal : speeds.unfocusedOrthogonal;
    const float diagonal = focusHeld ? speeds.focusedDiagonal : speeds.unfocusedDiagonal;
    float horizontal = 0.0f;
    float vertical = 0.0f;

    switch (state) {
    case kPlayerDirectionStateUp:
        vertical = -orthogonal;
        break;
    case kPlayerDirectionStateDown:
        vertical = orthogonal;
        break;
    case kPlayerDirectionStateLeft:
        horizontal = -orthogonal;
        break;
    case kPlayerDirectionStateRight:
        horizontal = orthogonal;
        break;
    case kPlayerDirectionStateUpLeft:
        horizontal = -diagonal;
        vertical = -diagonal;
        break;
    case kPlayerDirectionStateUpRight:
        horizontal = diagonal;
        vertical = -diagonal;
        break;
    case kPlayerDirectionStateDownLeft:
        horizontal = -diagonal;
        vertical = diagonal;
        break;
    case kPlayerDirectionStateDownRight:
        horizontal = diagonal;
        vertical = diagonal;
        break;
    default:
        break;
    }

    return {state, focusHeld, horizontal, vertical};
}

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
static_assert(kPlayerBombHorizontalSpeedMultiplierOffset == 0x23f0);
static_assert(kPlayerBombVerticalSpeedMultiplierOffset == 0x23f4);
static_assert(kPlayerOptionStateOffset == kPlayerModeDrawEnabledOffset);
static_assert(kPlayerFocusHeldOffset == kPlayerOptionStateOffset + 1);
static_assert(kPlayerOptionShotEffectCounterOffset == kPlayerFocusHeldOffset + 1);
static_assert(kPlayerModeTransitionRequestOffset == kPlayerOptionShotEffectCounterOffset + 1);
static_assert(kPlayerOptionInterpolationSubframeOffset ==
              kPlayerOptionInterpolationPreviousFrameOffset + 4);
static_assert(kPlayerOptionInterpolationFrameOffset == kPlayerOptionInterpolationSubframeOffset + 4);
static_assert(kPlayerMovementDirectionStateOffset == kPlayerOptionInterpolationFrameOffset + 4);
static_assert(kPlayerMovementHorizontalSpeedOffset == kPlayerMovementDirectionStateOffset + 4);
static_assert(kPlayerMovementVerticalSpeedOffset == kPlayerMovementHorizontalSpeedOffset + 4);
static_assert(kPlayerBombCommonEffectYOffset == kPlayerBombCommonEffectXOffset + 4);
static_assert(kPlayerBombCommonEffectDurationOffset == kPlayerBombCommonEffectYOffset + 4);
static_assert(kPlayerMode4EffectStartXOffset == kPlayerBombCommonEffectDurationOffset + 4);
static_assert(kPlayerMode4EffectStartYOffset == kPlayerMode4EffectStartXOffset + 4);
static_assert(kPlayerMode4EffectStartDurationOffset == kPlayerMode4EffectStartYOffset + 4);
static_assert(kPlayerBombCommonEffectActivePointerOffset + 4 ==
              kPlayerModeTransitionEffectActivePointerOffset);
static_assert(kPlayerModeTransitionEffectActivePointerOffset + 4 == kPlayerShotDataPointerOffset);
static_assert(kPlayerShotDataPointerOffset + 4 == kPlayerFocusedShotDataPointerOffset);
static_assert(kPlayerFocusedShotDataPointerOffset + 4 == kPlayerObjectClearSize);
static_assert(kPlayerModeTransitionEffectActivePointerOffset + 0x0c == kPlayerObjectClearSize);
static_assert(kPlayerBombCommonEffectXGlobalAddress == 0x004d44d8);
static_assert(kPlayerBombCommonEffectYGlobalAddress == 0x004d44dc);
static_assert(kPlayerBombCommonEffectDurationGlobalAddress == 0x004d44e0);
static_assert(kPlayerMovementTimerMultiplierGlobalAddress == kTh07SupervisorTimerMultiplierGlobalAddress);
static_assert(kPlayerMovementZeroFloatAddress == kTh07TimerZeroConstantAddress);
static_assert(kPlayerBombCollisionBulletClearOffset == 0x09e8);
static_assert(kPlayerBombCollisionSlotArrayOffset == 0x17dc);
static_assert(kPlayerBombCollisionSlotArrayOffset +
                  kPlayerBombCollisionSlotStride * kPlayerBombCollisionSlotCount ==
              0x23dc);
static_assert(kPlayerBombCollisionSlotPayloadOffset + 4 == kPlayerBombCollisionSlotStride);
static_assert(sizeof(Th07PlayerBombCollisionSlot) == kPlayerBombCollisionSlotStride);
static_assert(kPlayerOptionShotEffectCallbackFunctionAddress == 0x0043d0e0);
static_assert(kPlayerSpawnBulletsFunctionAddress == 0x0043d160);
static_assert(kPlayerUpdatePlayerBulletsFunctionAddress == 0x0043d2f0);
static_assert(kPlayerSpawnBulletsPowerOwnerPointerAddress == 0x00626278);
static_assert(kPlayerSpawnBulletsCurrentPowerFloatOffset == 0x007c);
static_assert(kPlayerSpawnBulletsFloatToIntFunctionAddress == 0x0048b8a0);
static_assert(kPlayerSpawnBulletsDefaultCallbackFunctionAddress == 0x0043bdc0);
static_assert(kPlayerShotSlotArrayOffset == kPlayerOptionResetAuxOffset + 4);
static_assert(kPlayerShotSlotStride == 0x0364);
static_assert(kPlayerShotSlotCount == 0x60);
static_assert(kPlayerShotSlotAnmSpritePointerOffset == 0x01e4);
static_assert(kPlayerShotSlotPendingInterruptOffset == 0x01c6);
static_assert(kPlayerShotSlotPositionXOffset == 0x024c);
static_assert(kPlayerShotSlotPositionYOffset == 0x0250);
static_assert(kPlayerShotSlotPositionZOffset == 0x0254);
static_assert(kPlayerShotSlotSizeXOffset == 0x0318);
static_assert(kPlayerShotSlotSizeYOffset == 0x031c);
static_assert(kPlayerShotSlotVelocityXOffset == 0x0324);
static_assert(kPlayerShotSlotVelocityYOffset == 0x0328);
static_assert(kPlayerShotSlotTimerPreviousOffset == 0x033c);
static_assert(kPlayerShotSlotTimerSubframeOffset == 0x0340);
static_assert(kPlayerShotSlotTimerCurrentOffset == 0x0344);
static_assert(kPlayerShotSlotDamageOffset == 0x0348);
static_assert(kPlayerShotSlotAutoRotateOffset == 0x01c4);
static_assert(kPlayerShotSlotDrawPositionXOffset == kTh07AnmVmDrawPositionOffset);
static_assert(kPlayerShotSlotDrawPositionYOffset == kPlayerShotSlotDrawPositionXOffset + 4);
static_assert(kPlayerShotSlotDrawPositionZOffset == kPlayerShotSlotDrawPositionYOffset + 4);
static_assert(kPlayerShotSlotAngleOffset == 0x0338);
static_assert(kPlayerShotSlotScriptIndexOffset == 0x01d8);
static_assert(kPlayerShotSlotStateOffset == 0x034a);
static_assert(kPlayerShotSlotTypeOffset == 0x034c);
static_assert(kPlayerShotSlotShtRecordPointerOffset == 0x0360);
static_assert(kPlayerShotDrawActiveFunctionAddress == 0x0043d690);
static_assert(kPlayerShotDrawCollidedFunctionAddress == 0x0043d790);
static_assert(kPlayerShotDrawNormalizeAngleFunctionAddress == 0x00431930);
static_assert(kPlayerShotDrawAnmVmFunctionAddress == 0x0044f9a0);
static_assert(kPlayerShotDrawAnmManagerPointerAddress == 0x004b9e44);
static_assert(kPlayerShotDrawOffsetXAddress == 0x0062f864);
static_assert(kPlayerShotDrawOffsetYAddress == 0x0062f868);
static_assert(kPlayerShotDrawHalfPiBits == 0x3fc90fdb);
static_assert(kPlayerShotDrawZBits == 0x3ecccccd);
static_assert(kPlayerFireTimerFunctionAddress == 0x0043d880);
static_assert(kPlayerFireTimerStartFunctionAddress == 0x0043d990);
static_assert(kPlayerFireTimerPreviousOffset == 0x169f4);
static_assert(kPlayerFireTimerSubframeOffset == 0x169f8);
static_assert(kPlayerFireTimerCurrentOffset == 0x169fc);
static_assert(kPlayerFireTimerCurrentOffset == kPlayerUpdateTrackedShotEffectMode2GateOffset);
static_assert(kPlayerFireTimerPreviousSentinelBits == 0xfffffc19);
static_assert(kPlayerFireTimerReplayGateAddress == 0x004d44f8);
static_assert(kPlayerFireTimerReplayGateByte0Address == 0x0062f645);
static_assert(kPlayerFireTimerReplayGateByte1Address == 0x0062f646);
static_assert(kPlayerDamageToEnemyFunctionAddress == 0x0043d9e0);
static_assert(kPlayerDamageHalfConstantAddress == 0x00498a50);
static_assert(kPlayerDamageZeroConstantAddress == 0x00498a4c);
static_assert(kPlayerDamageOneFrameConstantAddress == 0x00498a54);
static_assert(kPlayerDamageTwoConstantAddress == 0x00498a70);
static_assert(kPlayerDamageShotVelocityDivisorAddress == 0x00498a7c);
static_assert(kPlayerDamageBombActiveFlagOffset == 0x16a20);
static_assert(kPlayerDamageCollisionScriptDelta == 0x20);
static_assert(kPlayerDamageCollisionScriptTableOffset == 0x28ef0);
static_assert(kPlayerDamageCollisionSetScriptFunctionAddress == 0x0044ea20);
static_assert(kPlayerDamageCollisionEffectZBits == 0x3dcccccd);
static_assert(kPlayerDamageShotEffectId == 5);
static_assert(kPlayerDamageBombEffectLowId == 3);
static_assert(kPlayerDamageBombEffectHighId == 5);
static_assert(kPlayerDamageLaserTimerParityMask == 0x80000001);
static_assert(kPlayerDamageBombEffectCadenceMask == 0x03);
static_assert(kPlayerDamageBombRegionPositionOffset == 0x09dc);
static_assert(kPlayerDamageBombRegionSizeOffset == kPlayerBombCollisionBulletClearOffset);
static_assert(kPlayerDamageBombRegionDamageOffset == 0x09f4);
static_assert(kPlayerDamageBombRegionAccumulatedDamageOffset == 0x09f8);
static_assert(kPlayerDamageBombRegionStride == kPlayerBombCollisionBulletClearStride);
static_assert(kPlayerDamageBombRegionCount == kPlayerBombCollisionBulletClearCount);
static_assert(kPlayerDamageBombRegionLowEffectCount == kPlayerShotSlotCount);
static_assert(kPlayerUpdateTrackedShotEffectTimerArrayOffset == 0x169c4);
static_assert(kPlayerUpdateTrackedShotEffectPointerArrayOffset == 0x169d0);
static_assert(kPlayerUpdateTrackedShotEffectPointerStride == 0x10);
static_assert(kPlayerUpdateTrackedShotEffectPointerCount == 3);
static_assert(kPlayerUpdateTrackedShotEffectFocusedPointerOffset == 0x169f0);
static_assert(kPlayerUpdateTrackedShotEffectMode2GateOffset == 0x169fc);
static_assert(kPlayerUpdateTrackedShotEffectPreviousSentinelBits == 0xfffffc19);
static_assert(kPlayerUpdateBoundsOwnerAddress == 0x00626270);
static_assert(kPlayerUpdateBoundsFunctionAddress == 0x0042d6d8);
static_assert(kPlayerUpdateAnmManagerPointerAddress == 0x004b9e44);
static_assert(kPlayerUpdateAnmVmStepFunctionAddress == 0x00450d60);
static_assert(kPlayerUpdateTimerHelperOwnerAddress == 0x00575950);
static_assert(kPlayerUpdateTimerHelperFunctionAddress == 0x0043958d);
static_assert(kPlayerUpdateTimerDecrementFunctionAddress == 0x004394c7);
static_assert(kPlayerSpawnBulletsShtPowerTableOffset == 0x34);
static_assert(kPlayerSpawnBulletsShtPowerEntryStride == 0x08);
static_assert(kPlayerSpawnBulletsShtShotRecordStride == 0x34);
static_assert(kPlayerSpawnBulletsShtSpawnCallbackOffset == 0x24);
static_assert(kPlayerSpawnBulletsShtUpdateCallbackOffset == 0x28);
static_assert(kPlayerSpawnBulletsShtDrawCallbackOffset == 0x2c);
static_assert(kPlayerSpawnBulletsShtCollisionCallbackOffset == 0x30);
static_assert(kPlayerSpawnBulletsContract.shotSlotStride == kPlayerShotSlotStride);
static_assert(kPlayerUpdateBulletsContract.shotSlotStride == kPlayerShotSlotStride);
static_assert(!ShouldAdvanceTh07PlayerShotPowerLevel(7, 8));
static_assert(ShouldAdvanceTh07PlayerShotPowerLevel(8, 8));
static_assert(kPlayerOptionShotEffectSourceXOffset == 0x024c);
static_assert(kPlayerOptionShotEffectManagerAddress == kTh07EffectManagerAddress);
static_assert(kPlayerOptionShotEffectSpawnFunctionAddress == kTh07EffectSpawnFunctionAddress);
static_assert(kPlayerOptionShotEffectSpawnId == 5);
static_assert(kPlayerOptionShotEffectSpawnCount == 1);
static_assert(kPlayerOptionShotEffectSpawnColor == 0xffffffff);
static_assert(!BuildTh07PlayerOptionShotEffectState({6, 10.0f, 20.0f, 30.0f}).shouldSpawn);
static_assert(BuildTh07PlayerOptionShotEffectState({7, 10.0f, 20.0f, 30.0f}).shouldSpawn);
static_assert(BuildTh07PlayerOptionShotEffectState({255, 10.0f, 20.0f, 30.0f}).counter == 0);
static_assert(BuildTh07PlayerOptionShotEffectState({7, 10.0f, 20.0f, 30.0f}).spawnX == 10.0f);
static_assert(BuildTh07PlayerOptionShotEffectState({7, 10.0f, 20.0f, 30.0f}).spawnY == 20.0f);
static_assert(BuildTh07PlayerOptionShotEffectState({7, 10.0f, 20.0f, 30.0f}).returnValue == 0);
static_assert(kPlayerOptionFieldResetFunctionAddress == 0x00441e80);
static_assert(kPlayerMode1InvulnerableUpdateFunctionAddress == 0x004411c0);
static_assert(kPlayerModeConfigPointerAddress == 0x00575948);
static_assert(kPlayerModeConfigCurrentPowerCapOffset == 8);
static_assert(kPlayerMode1FadeFramesConstantAddress == 0x00498b00);
static_assert(kPlayerMode1OneFrameConstantAddress == kTh07TimerOneFrameConstantAddress);
static_assert(kPlayerMode1ScaleYMultiplierAddress == 0x00498a70);
static_assert(kPlayerMode1SpeedMultiplierBits == kTh07TimerOneFrameBits);
static_assert(kPlayerMode1FadeScaleYMultiplierBits == 0x40000000);
static_assert(kPlayerMode1FadeFrames == 30);
static_assert(kPlayerMode1CompleteCommonEffectDuration == 240);
static_assert(BuildTh07PlayerMode1InvulnerableUpdateState({0, 0.0f, 0, 8}).primaryScaleX == 0.0f);
static_assert(BuildTh07PlayerMode1InvulnerableUpdateState({0, 0.0f, 0, 8}).primaryScaleY == 3.0f);
static_assert(BuildTh07PlayerMode1InvulnerableUpdateState({15, 0.0f, 0, 8}).primaryColorBits ==
              0x7fffffff);
static_assert(!BuildTh07PlayerMode1InvulnerableUpdateState({29, 0.0f, 0x10, 8}).completed);
static_assert(BuildTh07PlayerMode1InvulnerableUpdateState({30, 0.0f, 0x10, 8}).completed);
static_assert(BuildTh07PlayerMode1InvulnerableUpdateState({30, 0.0f, 0x10, 8}).currentPower == 8);
static_assert(kPlayerOptionResetLeftXOffset == 0x2428);
static_assert(kPlayerOptionResetAuxOffset == kPlayerOptionResetLeftXOffset + 0x18);
static_assert(kPlayerOptionResetPendingInterruptMirrorAddress == 0x0134d476);
static_assert(kPlayerOptionResetPendingInterruptStateAddress == 0x013542ec);
static_assert(kPlayerOptionResetSentinelBits == 0xc479c000);
static_assert(kPlayerOptionResetTopYThreshold == 400.0f);
static_assert(kPlayerOptionResetRightXThreshold == 160.0f);
static_assert(BuildTh07PlayerOptionFieldResetState(100.0f, 500.0f, 0, 0).pendingInterruptState ==
              kPlayerOptionResetStateEntering);
static_assert(!BuildTh07PlayerOptionFieldResetState(100.0f, 500.0f,
                                                    kPlayerOptionResetStateEntering,
                                                    kPlayerOptionResetStateEntering)
                   .writesPendingInterrupt);
static_assert(BuildTh07PlayerOptionFieldResetState(200.0f, 500.0f,
                                                   kPlayerOptionResetStateEntering,
                                                   kPlayerOptionResetStateEntering)
                  .pendingInterruptState == kPlayerOptionResetStateInterrupted);
static_assert(BuildTh07PlayerOptionFieldResetState(100.0f, 399.0f,
                                                   kPlayerOptionResetStateEntering,
                                                   kPlayerOptionResetStateEntering)
                  .pendingInterruptState == kPlayerOptionResetStateInterrupted);
static_assert(kPlayerShotFocusedOrthogonalSpeedOffset == kPlayerShotUnfocusedOrthogonalSpeedOffset + 4);
static_assert(kPlayerShotUnfocusedDiagonalSpeedOffset == kPlayerShotFocusedOrthogonalSpeedOffset + 4);
static_assert(kPlayerShotFocusedDiagonalSpeedOffset == kPlayerShotUnfocusedDiagonalSpeedOffset + 4);
static_assert(kPlayerMovementInputMaskShoot == 0x0001);
static_assert(kPlayerMovementInputMaskFocus == 0x0004);
static_assert(kPlayerMovementInputMaskUp == 0x0010);
static_assert(kPlayerMovementInputMaskDown == 0x0020);
static_assert(kPlayerMovementInputMaskLeft == 0x0040);
static_assert(kPlayerMovementInputMaskRight == 0x0080);
static_assert(kPlayerDirectionStateDownRight == 8);
static_assert(kPlayerMovementCenterFromRightAnmTableOffset ==
              kPlayerMovementLeanLeftAnmTableOffset + 0x0c);
static_assert(kPlayerMovementInputContract.functionAddress == kPlayerMovementFunctionAddress);
static_assert(kPlayerMovementInputContract.shotDataPointerOffset == kPlayerShotDataPointerOffset);
static_assert(kPlayerMovementInputContract.timerMultiplierAddress ==
              kTh07SupervisorTimerMultiplierGlobalAddress);
static_assert(kPlayerHorizontalAnimationTransitionContracts.size() ==
              kPlayerMovementHorizontalAnimationTransitionCount);
static_assert(BuildTh07PlayerDirectionState(kPlayerMovementInputMaskUp |
                                            kPlayerMovementInputMaskDown) ==
              kPlayerDirectionStateUp);
static_assert(BuildTh07PlayerDirectionState(kPlayerMovementInputMaskLeft |
                                            kPlayerMovementInputMaskRight) ==
              kPlayerDirectionStateRight);
static_assert(BuildTh07PlayerDirectionState(kPlayerMovementInputMaskUp |
                                            kPlayerMovementInputMaskLeft |
                                            kPlayerMovementInputMaskRight) ==
              kPlayerDirectionStateUpRight);
static_assert(kPlayerOptionStateFocused == 3);

} // namespace th07
