#include "PlayerLayout.hpp"
#include "Controller.hpp"

#include <cassert>

int main()
{
    static_assert(th07::kPlayerAnmVmSlotSize == 0x024c);
    static_assert(th07::kPlayerOriginalAnmVmPrefixSize == th07::kPlayerPositionOffset);
    static_assert(th07::kPlayerFourthAnmVmOffset + th07::kPlayerAnmVmSlotSize == th07::kPlayerPositionOffset);
    static_assert(th07::kPlayerHitboxMinOffset + th07::kPlayerVectorSize == th07::kPlayerHitboxMaxOffset);
    static_assert(th07::kPlayerGrazeBoxMinOffset + th07::kPlayerVectorSize == th07::kPlayerGrazeBoxMaxOffset);
    static_assert(th07::kPlayerGrabItemBoxMinOffset + th07::kPlayerVectorSize == th07::kPlayerGrabItemBoxMaxOffset);
    static_assert(th07::kPlayerHitboxSizeOffset + th07::kPlayerVectorSize == th07::kPlayerGrazeBoxSizeOffset);
    static_assert(th07::kPlayerGrazeBoxSizeOffset + th07::kPlayerVectorSize == th07::kPlayerGrabItemSizeOffset);
    static_assert(th07::kPlayerLeftOptionPositionOffset == th07::kPlayerGrabItemSizeOffset + th07::kPlayerVectorSize);
    static_assert(th07::kPlayerRightOptionPositionOffset == th07::kPlayerLeftOptionPositionOffset +
                  th07::kPlayerVectorSize);
    static_assert(th07::kPlayerMovementDeltaYOffset == 0x09d0);
    static_assert(th07::kPlayerBombVerticalSpeedMultiplierOffset ==
                  th07::kPlayerBombHorizontalSpeedMultiplierOffset + 4);
    static_assert(th07::kPlayerModeSoundTimerOffset == 0x2400);
    static_assert(th07::kPlayerModeStateOffset == 0x2408);
    static_assert(th07::kPlayerOptionStateOffset == 0x240a);
    static_assert(th07::kPlayerModeDrawEnabledOffset == th07::kPlayerOptionStateOffset);
    static_assert(th07::kPlayerFocusHeldOffset == 0x240b);
    static_assert(th07::kPlayerModeTransitionRequestOffset == 0x240d);
    static_assert(th07::kPlayerOptionInterpolationPreviousFrameOffset == 0x2410);
    static_assert(th07::kPlayerOptionInterpolationSubframeOffset == 0x2414);
    static_assert(th07::kPlayerOptionInterpolationFrameOffset == 0x2418);
    static_assert(th07::kPlayerMovementDirectionStateOffset == 0x241c);
    static_assert(th07::kPlayerMovementHorizontalSpeedOffset == 0x2420);
    static_assert(th07::kPlayerMovementVerticalSpeedOffset == 0x2424);
    static_assert(th07::kPlayerBombCommonEffectXOffset == 0x16a00);
    static_assert(th07::kPlayerBombCommonEffectYOffset == 0x16a04);
    static_assert(th07::kPlayerBombCommonEffectDurationOffset == 0x16a08);
    static_assert(th07::kPlayerMode4EffectStartXOffset == 0x16a0c);
    static_assert(th07::kPlayerMode4EffectStartYOffset == 0x16a10);
    static_assert(th07::kPlayerMode4EffectStartDurationOffset == 0x16a14);
    static_assert(th07::kPlayerBombCommonEffectActivePointerOffset == 0x0b7e68);
    static_assert(th07::kPlayerModeTransitionEffectActivePointerOffset == 0x0b7e6c);
    static_assert(th07::kPlayerShotDataPointerOffset == 0x0b7e70);
    static_assert(th07::kPlayerFocusedShotDataPointerOffset == 0x0b7e74);
    static_assert(th07::kPlayerBombCommonEffectDurationGlobalAddress == 0x004d44e0);
    static_assert(th07::kPlayerModeTransitionEffectActivePointerOffset + 0x0c ==
                  th07::kPlayerObjectClearSize);
    static_assert(th07::kPlayerOptionStateHidden == 0);
    static_assert(th07::kPlayerOptionStateUnfocused == 1);
    static_assert(th07::kPlayerOptionStateFocusing == 2);
    static_assert(th07::kPlayerOptionStateFocused == 3);
    static_assert(th07::kPlayerOptionStateUnfocusing == 4);
    static_assert(th07::kPlayerFocusNotHeld == 0);
    static_assert(th07::kPlayerFocusHeld == 1);
    static_assert(th07::kPlayerMovementInputWordAddress == 0x004b9e50);
    static_assert(th07::kPlayerMovementAnmManagerAddress == 0x004b9e44);
    static_assert(th07::kPlayerMovementTimerMultiplierGlobalAddress == 0x00575ac8);
    static_assert(th07::kPlayerMovementPlayfieldMinXAddress == 0x0062f874);
    static_assert(th07::kPlayerMovementPlayfieldMinYAddress == 0x0062f878);
    static_assert(th07::kPlayerMovementPlayfieldSpanXAddress == 0x0062f87c);
    static_assert(th07::kPlayerMovementPlayfieldSpanYAddress == 0x0062f880);
    static_assert(th07::kPlayerShotUnfocusedOrthogonalSpeedOffset == 0x0024);
    static_assert(th07::kPlayerShotFocusedOrthogonalSpeedOffset == 0x0028);
    static_assert(th07::kPlayerShotUnfocusedDiagonalSpeedOffset == 0x002c);
    static_assert(th07::kPlayerShotFocusedDiagonalSpeedOffset == 0x0030);
    static_assert(th07::kPlayerMovementInputMaskShoot == th07::TH_BUTTON_SHOOT);
    static_assert(th07::kPlayerMovementInputMaskFocus == th07::TH_BUTTON_FOCUS);
    static_assert(th07::kPlayerMovementInputMaskUp == th07::TH_BUTTON_UP);
    static_assert(th07::kPlayerMovementInputMaskDown == th07::TH_BUTTON_DOWN);
    static_assert(th07::kPlayerMovementInputMaskLeft == th07::TH_BUTTON_LEFT);
    static_assert(th07::kPlayerMovementInputMaskRight == th07::TH_BUTTON_RIGHT);
    static_assert(th07::BuildTh07PlayerDirectionState(0) == th07::kPlayerDirectionStateNone);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_UP) ==
                  th07::kPlayerDirectionStateUp);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_DOWN) ==
                  th07::kPlayerDirectionStateDown);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_LEFT) ==
                  th07::kPlayerDirectionStateLeft);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_RIGHT) ==
                  th07::kPlayerDirectionStateRight);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_UP_LEFT) ==
                  th07::kPlayerDirectionStateUpLeft);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_UP_RIGHT) ==
                  th07::kPlayerDirectionStateUpRight);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_DOWN_LEFT) ==
                  th07::kPlayerDirectionStateDownLeft);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_DOWN_RIGHT) ==
                  th07::kPlayerDirectionStateDownRight);
    static_assert(th07::BuildTh07PlayerDirectionState(th07::TH_BUTTON_UP | th07::TH_BUTTON_DOWN |
                                                      th07::TH_BUTTON_LEFT | th07::TH_BUTTON_RIGHT) ==
                  th07::kPlayerDirectionStateUpRight);
    static_assert(th07::kPlayerMovementLeanLeftAnmScript == 0x0401);
    static_assert(th07::kPlayerMovementCenterFromLeftAnmScript == 0x0402);
    static_assert(th07::kPlayerMovementLeanRightAnmScript == 0x0403);
    static_assert(th07::kPlayerMovementCenterFromRightAnmScript == 0x0404);
    static_assert(th07::kPlayerMovementHorizontalAnimationTransitionCount == 4);
    static_assert(th07::kPlayerItemBoxCollisionFunctionAddress == 0x0043e4e0);
    static_assert(th07::kPlayerItemBoxCollisionModeStateOffset == th07::kPlayerModeStateOffset);
    static_assert(th07::kPlayerItemBoxCollisionGrabMinOffset == th07::kPlayerGrabItemBoxMinOffset);
    static_assert(th07::kPlayerItemBoxCollisionGrabMaxOffset == th07::kPlayerGrabItemBoxMaxOffset);
    static_assert(th07::kPlayerItemBoxCollisionHalfScale == 0.5f);
    static_assert(th07::kPlayerBombCollisionFunctionAddress == 0x0043e0a0);
    static_assert(th07::kPlayerBombCollisionUpdateFunctionAddress == 0x00440940);
    static_assert(th07::kPlayerBombCollisionRegisterRectFunctionAddress == 0x00441800);
    static_assert(th07::kPlayerBombCollisionRegisterCircleFunctionAddress == 0x004418b0);
    static_assert(th07::kPlayerMode1InvulnerableUpdateFunctionAddress == 0x004411c0);
    static_assert(th07::kPlayerSpawnBulletsFunctionAddress == 0x0043d160);
    static_assert(th07::kPlayerUpdatePlayerBulletsFunctionAddress == 0x0043d2f0);
    static_assert(th07::kPlayerSpawnBulletsPowerOwnerPointerAddress == 0x00626278);
    static_assert(th07::kPlayerSpawnBulletsCurrentPowerFloatOffset == 0x007c);
    static_assert(th07::kPlayerSpawnBulletsFloatToIntFunctionAddress == 0x0048b8a0);
    static_assert(th07::kPlayerSpawnBulletsDefaultCallbackFunctionAddress == 0x0043bdc0);
    static_assert(th07::kPlayerShotSlotArrayOffset == 0x2444);
    static_assert(th07::kPlayerShotSlotStride == 0x0364);
    static_assert(th07::kPlayerShotSlotCount == 0x60);
    static_assert(th07::kPlayerShotSlotAnmSpritePointerOffset == 0x01e4);
    static_assert(th07::kPlayerShotSlotPendingInterruptOffset == 0x01c6);
    static_assert(th07::kPlayerShotSlotPositionXOffset == 0x024c);
    static_assert(th07::kPlayerShotSlotPositionYOffset == 0x0250);
    static_assert(th07::kPlayerShotSlotPositionZOffset == 0x0254);
    static_assert(th07::kPlayerShotSlotSizeXOffset == 0x0318);
    static_assert(th07::kPlayerShotSlotSizeYOffset == 0x031c);
    static_assert(th07::kPlayerShotSlotVelocityXOffset == 0x0324);
    static_assert(th07::kPlayerShotSlotVelocityYOffset == 0x0328);
    static_assert(th07::kPlayerShotSlotTimerPreviousOffset == 0x033c);
    static_assert(th07::kPlayerShotSlotTimerSubframeOffset == 0x0340);
    static_assert(th07::kPlayerShotSlotTimerCurrentOffset == 0x0344);
    static_assert(th07::kPlayerShotSlotDamageOffset == 0x0348);
    static_assert(th07::kPlayerShotSlotRenderFlagsOffset == 0x01c0);
    static_assert(th07::kPlayerShotSlotRenderFlagsActiveBit == 0x1000);
    static_assert(th07::kPlayerShotSlotRenderFlagsRotationDirtyBit == 0x04);
    static_assert(th07::kPlayerShotSlotAutoRotateOffset == 0x01c4);
    static_assert(th07::kPlayerShotSlotDrawPositionXOffset == 0x01c8);
    static_assert(th07::kPlayerShotSlotDrawPositionYOffset == 0x01cc);
    static_assert(th07::kPlayerShotSlotDrawPositionZOffset == 0x01d0);
    static_assert(th07::kPlayerShotSlotStateOffset == 0x034a);
    static_assert(th07::kPlayerShotSlotTypeOffset == 0x034c);
    static_assert(th07::kPlayerShotSlotUpdateCallbackOffset == 0x0354);
    static_assert(th07::kPlayerShotSlotDrawCallbackOffset == 0x0358);
    static_assert(th07::kPlayerShotSlotCollisionCallbackOffset == 0x035c);
    static_assert(th07::kPlayerShotSlotShtRecordPointerOffset == 0x0360);
    static_assert(th07::kPlayerShotSlotAngleOffset == 0x0338);
    static_assert(th07::kPlayerShotSlotScriptIndexOffset == 0x01d8);
    static_assert(th07::kPlayerShotDrawActiveFunctionAddress == 0x0043d690);
    static_assert(th07::kPlayerShotDrawCollidedFunctionAddress == 0x0043d790);
    static_assert(th07::kPlayerShotDrawNormalizeAngleFunctionAddress == 0x00431930);
    static_assert(th07::kPlayerShotDrawAnmVmFunctionAddress == 0x0044f9a0);
    static_assert(th07::kPlayerShotDrawAnmManagerPointerAddress == 0x004b9e44);
    static_assert(th07::kPlayerShotDrawOffsetXAddress == 0x0062f864);
    static_assert(th07::kPlayerShotDrawOffsetYAddress == 0x0062f868);
    static_assert(th07::kPlayerShotDrawHalfPiBits == 0x3fc90fdb);
    static_assert(th07::kPlayerShotDrawZBits == 0x3ecccccd);
    static_assert(th07::kPlayerFireTimerFunctionAddress == 0x0043d880);
    static_assert(th07::kPlayerFireTimerStartFunctionAddress == 0x0043d990);
    static_assert(th07::kPlayerFireTimerPreviousOffset == 0x169f4);
    static_assert(th07::kPlayerFireTimerSubframeOffset == 0x169f8);
    static_assert(th07::kPlayerFireTimerCurrentOffset == 0x169fc);
    static_assert(th07::kPlayerFireTimerCurrentOffset == th07::kPlayerUpdateTrackedShotEffectMode2GateOffset);
    static_assert(th07::kPlayerFireTimerPreviousSentinelBits == 0xfffffc19);
    static_assert(th07::kPlayerFireTimerReplayGateAddress == 0x004d44f8);
    static_assert(th07::kPlayerFireTimerReplayGateByte0Address == 0x0062f645);
    static_assert(th07::kPlayerFireTimerReplayGateByte1Address == 0x0062f646);
    static_assert(th07::kPlayerDamageToEnemyFunctionAddress == 0x0043d9e0);
    static_assert(th07::kPlayerDamageHalfConstantAddress == 0x00498a50);
    static_assert(th07::kPlayerDamageZeroConstantAddress == 0x00498a4c);
    static_assert(th07::kPlayerDamageOneFrameConstantAddress == 0x00498a54);
    static_assert(th07::kPlayerDamageTwoConstantAddress == 0x00498a70);
    static_assert(th07::kPlayerDamageShotVelocityDivisorAddress == 0x00498a7c);
    static_assert(th07::kPlayerDamageBombActiveFlagOffset == 0x16a20);
    static_assert(th07::kPlayerDamageCollisionScriptDelta == 0x20);
    static_assert(th07::kPlayerDamageCollisionScriptTableOffset == 0x28ef0);
    static_assert(th07::kPlayerDamageCollisionSetScriptFunctionAddress == 0x0044ea20);
    static_assert(th07::kPlayerDamageCollisionEffectZBits == 0x3dcccccd);
    static_assert(th07::kPlayerDamageShotEffectId == 5);
    static_assert(th07::kPlayerDamageBombEffectLowId == 3);
    static_assert(th07::kPlayerDamageBombEffectHighId == 5);
    static_assert(th07::kPlayerDamageLaserTimerParityMask == 0x80000001);
    static_assert(th07::kPlayerDamageBombEffectCadenceMask == 0x03);
    static_assert(th07::kPlayerDamageBombRegionPositionOffset == 0x09dc);
    static_assert(th07::kPlayerDamageBombRegionSizeOffset == 0x09e8);
    static_assert(th07::kPlayerDamageBombRegionDamageOffset == 0x09f4);
    static_assert(th07::kPlayerDamageBombRegionAccumulatedDamageOffset == 0x09f8);
    static_assert(th07::kPlayerDamageBombRegionStride == 0x20);
    static_assert(th07::kPlayerDamageBombRegionCount == 0x70);
    static_assert(th07::kPlayerDamageBombRegionLowEffectCount == 0x60);
    static_assert(th07::kPlayerSpawnBulletsShtPowerTableOffset == 0x34);
    static_assert(th07::kPlayerSpawnBulletsShtPowerEntryStride == 0x08);
    static_assert(th07::kPlayerSpawnBulletsShtShotRecordStride == 0x34);
    static_assert(th07::kPlayerSpawnBulletsShtSpawnCallbackOffset == 0x24);
    static_assert(th07::kPlayerSpawnBulletsShtUpdateCallbackOffset == 0x28);
    static_assert(th07::kPlayerSpawnBulletsShtDrawCallbackOffset == 0x2c);
    static_assert(th07::kPlayerSpawnBulletsShtCollisionCallbackOffset == 0x30);
    static_assert(th07::kPlayerSpawnBulletsContract.shotSlotCount == th07::kPlayerShotSlotCount);
    static_assert(th07::kPlayerUpdateTrackedShotEffectTimerArrayOffset == 0x169c4);
    static_assert(th07::kPlayerUpdateTrackedShotEffectPointerArrayOffset == 0x169d0);
    static_assert(th07::kPlayerUpdateTrackedShotEffectPointerStride == 0x10);
    static_assert(th07::kPlayerUpdateTrackedShotEffectPointerCount == 3);
    static_assert(th07::kPlayerUpdateTrackedShotEffectFocusedPointerOffset == 0x169f0);
    static_assert(th07::kPlayerUpdateTrackedShotEffectMode2GateOffset == 0x169fc);
    static_assert(th07::kPlayerUpdateTrackedShotEffectPreviousSentinelBits == 0xfffffc19);
    static_assert(th07::kPlayerUpdateBoundsOwnerAddress == 0x00626270);
    static_assert(th07::kPlayerUpdateBoundsFunctionAddress == 0x0042d6d8);
    static_assert(th07::kPlayerUpdateAnmManagerPointerAddress == 0x004b9e44);
    static_assert(th07::kPlayerUpdateAnmVmStepFunctionAddress == 0x00450d60);
    static_assert(th07::kPlayerUpdateTimerHelperOwnerAddress == 0x00575950);
    static_assert(th07::kPlayerUpdateTimerHelperFunctionAddress == 0x0043958d);
    static_assert(th07::kPlayerUpdateTimerDecrementFunctionAddress == 0x004394c7);
    static_assert(th07::kPlayerUpdateLoadedSpriteWidthOffset == 0x002c);
    static_assert(th07::kPlayerUpdateLoadedSpriteHeightOffset == 0x0030);
    static_assert(th07::kPlayerUpdateBulletsContract.shotSlotStride == th07::kPlayerShotSlotStride);
    static_assert(!th07::ShouldAdvanceTh07PlayerShotPowerLevel(7, 8));
    static_assert(th07::ShouldAdvanceTh07PlayerShotPowerLevel(8, 8));
    static_assert(th07::kPlayerOptionShotEffectCallbackFunctionAddress == 0x0043d0e0);
    static_assert(th07::kPlayerOptionShotEffectCounterOffset == 0x240c);
    static_assert(th07::kPlayerOptionShotEffectSourceXOffset == 0x024c);
    static_assert(th07::kPlayerOptionShotEffectCounterMask == 0x07);
    static_assert(th07::kPlayerOptionShotEffectSpawnId == 5);
    static_assert(th07::kPlayerOptionShotEffectSpawnCount == 1);
    static_assert(th07::kPlayerOptionShotEffectSpawnColor == 0xffffffff);
    static_assert(th07::kPlayerOptionShotEffectManagerAddress == 0x012fe250);
    static_assert(th07::kPlayerOptionShotEffectSpawnFunctionAddress == 0x0041c1c0);
    static_assert(th07::kPlayerOptionFieldResetFunctionAddress == 0x00441e80);
    static_assert(th07::kPlayerOptionResetLeftXOffset == 0x2428);
    static_assert(th07::kPlayerOptionResetLeftYOffset == 0x242c);
    static_assert(th07::kPlayerOptionResetLeftZOffset == 0x2430);
    static_assert(th07::kPlayerOptionResetRightXOffset == 0x2434);
    static_assert(th07::kPlayerOptionResetRightYOffset == 0x2438);
    static_assert(th07::kPlayerOptionResetRightZOffset == 0x243c);
    static_assert(th07::kPlayerOptionResetAuxOffset == 0x2440);
    static_assert(th07::kPlayerOptionResetPendingInterruptMirrorAddress == 0x0134d476);
    static_assert(th07::kPlayerOptionResetPendingInterruptStateAddress == 0x013542ec);
    static_assert(th07::kPlayerOptionResetSentinelBits == 0xc479c000);
    static_assert(th07::kPlayerOptionResetZeroBits == 0x00000000);
    static_assert(th07::kPlayerOptionResetTopYThresholdAddress == 0x00498afc);
    static_assert(th07::kPlayerOptionResetRightXThresholdAddress == 0x00498ac0);
    static_assert(th07::kPlayerOptionResetTopYThreshold == 400.0f);
    static_assert(th07::kPlayerOptionResetRightXThreshold == 160.0f);
    static_assert(th07::kPlayerOptionResetStateEntering == 2);
    static_assert(th07::kPlayerOptionResetStateInterrupted == 3);
    static_assert(th07::kPlayerModeConfigPointerAddress == 0x00575948);
    static_assert(th07::kPlayerModeConfigCurrentPowerCapOffset == 0x0008);
    static_assert(th07::kPlayerMode1SoundTimerValue == 0x3c);
    static_assert(th07::kPlayerMode1BlendFlag == 0x10);
    static_assert(th07::kPlayerMode1WhiteRgbBits == 0x00ffffff);
    static_assert(th07::kPlayerMode1WhiteArgbBits == 0xffffffff);
    static_assert(th07::kPlayerMode1SpeedMultiplierBits == 0x3f800000);
    static_assert(th07::kPlayerMode1FadeScaleYMultiplierBits == 0x40000000);
    static_assert(th07::kPlayerMode1FadeFrames == 0x1e);
    static_assert(th07::kPlayerMode1FadeFramesConstantAddress == 0x00498b00);
    static_assert(th07::kPlayerMode1OneFrameConstantAddress == 0x00498a54);
    static_assert(th07::kPlayerMode1ScaleYMultiplierAddress == 0x00498a70);
    static_assert(th07::kPlayerMode1CompleteModeState == 3);
    static_assert(th07::kPlayerMode1CompleteCommonEffectDuration == 0xf0);
    static_assert(th07::kPlayerBombCollisionBulletClearOffset == 0x09e8);
    static_assert(th07::kPlayerBombCollisionBulletClearStride == 0x20);
    static_assert(th07::kPlayerBombCollisionBulletClearCount == 0x70);
    static_assert(th07::kPlayerBombCollisionSlotArrayOffset == 0x17dc);
    static_assert(th07::kPlayerBombCollisionSlotStride == 0x20);
    static_assert(th07::kPlayerBombCollisionSlotCount == 0x60);
    static_assert(th07::kPlayerBombCollisionSlotRectSizeXOffset == 0x08);
    static_assert(th07::kPlayerBombCollisionSlotRadiusOffset == 0x10);
    static_assert(th07::kPlayerBombCollisionSlotFramesLeftOffset == 0x18);
    static_assert(th07::kPlayerBombCollisionSlotPayloadOffset == 0x1c);
    static_assert(th07::kPlayerCollisionPayloadOffset == 0x2404);
    static_assert(th07::kPlayerCollisionDefaultPayloadBits == 0x00000006);
    static_assert(th07::kPlayerCollisionHalfScale == 0.5f);
    static_assert(th07::kPlayerGrazeCollisionMargin == 20.0f);
    static_assert(th07::kPlayerBombCollisionHalfScale == 0.5f);
    static_assert(sizeof(th07::Th07PlayerBombCollisionSlot) == th07::kPlayerBombCollisionSlotStride);
    static_assert(th07::IsTh07PlayerBombCollisionSlotFree({0.0f, 0.0f, 0.0f, 4.0f,
                                                           0.0f, 1.0f, 3, 9}));
    static_assert(!th07::IsTh07PlayerBombCollisionSlotFree({0.0f, 0.0f, 1.0f, 4.0f,
                                                            0.0f, 1.0f, 3, 9}));
    static_assert(th07::IsTh07PlayerItemCollectionMode(0));
    static_assert(!th07::IsTh07PlayerItemCollectionMode(1));
    static_assert(!th07::IsTh07PlayerItemCollectionMode(2));
    static_assert(th07::IsTh07PlayerItemCollectionMode(3));
    static_assert(th07::IsTh07PlayerItemCollectionMode(4));
    static_assert(!th07::IsTh07PlayerItemCollectionMode(5));
    static_assert(!th07::IsTh07PlayerGrazeSuppressedMode(0));
    static_assert(th07::IsTh07PlayerGrazeSuppressedMode(1));
    static_assert(th07::IsTh07PlayerGrazeSuppressedMode(2));
    static_assert(!th07::IsTh07PlayerGrazeSuppressedMode(3));
    static_assert(!th07::IsTh07PlayerGrazeSuppressedMode(4));

    assert(th07::kPlayerObjectClearSize == 0x0b7e78);
    assert(th07::kPlayerMovementInputContract.functionAddress == th07::kPlayerMovementFunctionAddress);
    assert(th07::kPlayerMovementInputContract.inputWordAddress == 0x004b9e50);
    assert(th07::kPlayerMovementInputContract.directionStateOffset == 0x241c);
    assert(th07::kPlayerMovementInputContract.focusedDiagonalSpeedOffset == 0x0030);
    assert(th07::kPlayerMovementInputContract.timerMultiplierAddress == 0x00575ac8);

    const th07::PlayerMovementSpeedInput speeds{4.0f, 1.5f, 3.0f, 1.0f};
    const th07::PlayerMovementVector focusedUpLeft =
        th07::BuildTh07PlayerMovementVector(th07::TH_BUTTON_FOCUS | th07::TH_BUTTON_UP_LEFT, speeds);
    assert(focusedUpLeft.directionState == th07::kPlayerDirectionStateUpLeft);
    assert(focusedUpLeft.focusHeld);
    assert(focusedUpLeft.horizontalSpeed == -1.0f);
    assert(focusedUpLeft.verticalSpeed == -1.0f);

    const th07::PlayerMovementVector unfocusedRight =
        th07::BuildTh07PlayerMovementVector(th07::TH_BUTTON_RIGHT, speeds);
    assert(unfocusedRight.directionState == th07::kPlayerDirectionStateRight);
    assert(!unfocusedRight.focusHeld);
    assert(unfocusedRight.horizontalSpeed == 4.0f);
    assert(unfocusedRight.verticalSpeed == 0.0f);

    const th07::PlayerHorizontalAnimationTransitionContract *leanRight =
        th07::FindPlayerHorizontalAnimationTransitionContract(
            th07::PlayerHorizontalAnimationTransitionKind::LeanRight);
    assert(leanRight != nullptr);
    assert(leanRight->scriptIndex == th07::kPlayerMovementLeanRightAnmScript);
    assert(leanRight->scriptTableOffset == th07::kPlayerMovementLeanRightAnmTableOffset);
    assert(leanRight->applyScriptCallAddress == th07::kPlayerMovementLeanRightApplyScriptCallAddress);

    const th07::PlayerItemBoxCollisionInput itemBoxOverlap{0, 20.0f, 30.0f, 4.0f, 6.0f,
                                                           10.0f, 20.0f, 30.0f, 40.0f};
    assert(th07::CheckTh07PlayerItemBoxCollision(itemBoxOverlap));
    assert(th07::CheckTh07PlayerItemBoxCollision({3, 20.0f, 30.0f, 4.0f, 6.0f,
                                                  10.0f, 20.0f, 30.0f, 40.0f}));
    assert(th07::CheckTh07PlayerItemBoxCollision({4, 20.0f, 30.0f, 4.0f, 6.0f,
                                                  10.0f, 20.0f, 30.0f, 40.0f}));
    assert(!th07::CheckTh07PlayerItemBoxCollision({1, 20.0f, 30.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f}));
    assert(!th07::CheckTh07PlayerItemBoxCollision({0, 7.0f, 30.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f}));
    assert(!th07::CheckTh07PlayerItemBoxCollision({0, 33.0f, 30.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f}));
    assert(!th07::CheckTh07PlayerItemBoxCollision({0, 20.0f, 16.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f}));
    assert(!th07::CheckTh07PlayerItemBoxCollision({0, 20.0f, 44.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f}));
    assert(th07::CheckTh07PlayerItemBoxCollision({0, 8.0f, 30.0f, 4.0f, 6.0f,
                                                  10.0f, 20.0f, 30.0f, 40.0f}));

    const th07::PlayerAabbCollisionInput killOverlap{20.0f, 30.0f, 4.0f, 6.0f,
                                                     10.0f, 20.0f, 30.0f, 40.0f, 0.0f};
    assert(th07::CheckTh07PlayerKillBoxCollision(killOverlap));
    assert(!th07::CheckTh07PlayerKillBoxCollision({7.9f, 30.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f, 0.0f}));
    assert(th07::CheckTh07PlayerKillBoxCollision({8.0f, 30.0f, 4.0f, 6.0f,
                                                  10.0f, 20.0f, 30.0f, 40.0f, 0.0f}));
    assert(th07::CheckTh07PlayerGrazeBoxCollision({-12.0f, 30.0f, 4.0f, 6.0f,
                                                   10.0f, 20.0f, 30.0f, 40.0f, 0.0f}));
    assert(!th07::CheckTh07PlayerGrazeBoxCollision({-12.1f, 30.0f, 4.0f, 6.0f,
                                                    10.0f, 20.0f, 30.0f, 40.0f, 0.0f}));

    const th07::Th07PlayerBombCollisionSlot rectSlot =
        th07::BuildTh07PlayerBombCollisionRectSlot(20.0f, 30.0f, 12.0f, 8.0f, 7);
    const th07::PlayerBombCollisionResult rectHit =
        th07::CheckTh07PlayerBombCollisionSlot(rectSlot, {24.0f, 30.0f, 4.0f, 4.0f});
    assert(rectHit.hit);
    assert(rectHit.payloadBits == 7);
    assert(!th07::CheckTh07PlayerBombCollisionSlot(rectSlot, {40.1f, 30.0f, 4.0f, 4.0f}).hit);
    assert(th07::CheckTh07PlayerBombCollisionSlot(rectSlot, {28.0f, 30.0f, 4.0f, 4.0f}).hit);

    const th07::Th07PlayerBombCollisionSlot circleSlot =
        th07::BuildTh07PlayerBombCollisionCircleSlot(10.0f, 20.0f, 5.0f, 0.5f, 2, 11);
    const th07::PlayerBombCollisionResult circleHit =
        th07::CheckTh07PlayerBombCollisionSlot(circleSlot, {13.0f, 23.0f, 20.0f, 20.0f});
    assert(circleHit.hit);
    assert(circleHit.payloadBits == 11);
    assert(!th07::CheckTh07PlayerBombCollisionSlot(circleSlot, {13.0f, 24.0f, 0.0f, 0.0f}).hit);
    assert(!th07::CheckTh07PlayerBombCollisionSlot(
        th07::BuildTh07PlayerBombCollisionCircleSlot(10.0f, 20.0f, 0.0f, 0.5f, 2, 11),
        {10.0f, 20.0f, 0.0f, 0.0f}).hit);
    const th07::Th07PlayerBombCollisionSlot grownCircle =
        th07::TickTh07PlayerBombCollisionSlot(circleSlot);
    assert(grownCircle.framesLeft == 1);
    assert(grownCircle.radius == 5.5f);
    const th07::Th07PlayerBombCollisionSlot expiredCircle =
        th07::TickTh07PlayerBombCollisionSlot(
            th07::BuildTh07PlayerBombCollisionCircleSlot(10.0f, 20.0f, 5.0f, 0.5f, 0, 11));
    assert(expiredCircle.radius == 0.0f);
    assert(expiredCircle.rectSizeX == 0.0f);

    const th07::PlayerOptionFieldResetState enteringReset =
        th07::BuildTh07PlayerOptionFieldResetState(100.0f, 500.0f, 0, 0);
    assert(enteringReset.leftXBits == th07::kPlayerOptionResetSentinelBits);
    assert(enteringReset.leftYBits == th07::kPlayerOptionResetSentinelBits);
    assert(enteringReset.leftZBits == th07::kPlayerOptionResetZeroBits);
    assert(enteringReset.rightXBits == th07::kPlayerOptionResetSentinelBits);
    assert(enteringReset.rightYBits == th07::kPlayerOptionResetSentinelBits);
    assert(enteringReset.rightZBits == th07::kPlayerOptionResetZeroBits);
    assert(enteringReset.auxBits == th07::kPlayerOptionResetZeroBits);
    assert(enteringReset.pendingInterruptState == th07::kPlayerOptionResetStateEntering);
    assert(enteringReset.pendingInterruptMirror == th07::kPlayerOptionResetStateEntering);
    assert(enteringReset.writesPendingInterrupt);

    const th07::PlayerOptionFieldResetState enteringNoWrite =
        th07::BuildTh07PlayerOptionFieldResetState(
            100.0f, 500.0f, th07::kPlayerOptionResetStateEntering,
            th07::kPlayerOptionResetStateEntering);
    assert(enteringNoWrite.pendingInterruptState == th07::kPlayerOptionResetStateEntering);
    assert(enteringNoWrite.pendingInterruptMirror == th07::kPlayerOptionResetStateEntering);
    assert(!enteringNoWrite.writesPendingInterrupt);

    const th07::PlayerOptionFieldResetState rightInterrupt =
        th07::BuildTh07PlayerOptionFieldResetState(
            200.0f, 500.0f, th07::kPlayerOptionResetStateEntering,
            th07::kPlayerOptionResetStateEntering);
    assert(rightInterrupt.pendingInterruptState == th07::kPlayerOptionResetStateInterrupted);
    assert(rightInterrupt.pendingInterruptMirror == th07::kPlayerOptionResetStateInterrupted);
    assert(rightInterrupt.writesPendingInterrupt);

    const th07::PlayerOptionFieldResetState topInterrupt =
        th07::BuildTh07PlayerOptionFieldResetState(
            100.0f, 399.0f, th07::kPlayerOptionResetStateEntering,
            th07::kPlayerOptionResetStateEntering);
    assert(topInterrupt.pendingInterruptState == th07::kPlayerOptionResetStateInterrupted);
    assert(topInterrupt.pendingInterruptMirror == th07::kPlayerOptionResetStateInterrupted);
    assert(topInterrupt.writesPendingInterrupt);

    const th07::PlayerOptionFieldResetState rightNoWrite =
        th07::BuildTh07PlayerOptionFieldResetState(200.0f, 500.0f, 0, 0);
    assert(rightNoWrite.pendingInterruptState == 0);
    assert(rightNoWrite.pendingInterruptMirror == 0);
    assert(!rightNoWrite.writesPendingInterrupt);

    const th07::PlayerMode1InvulnerableState mode1Start =
        th07::BuildTh07PlayerMode1InvulnerableUpdateState({0, 0.0f, 0, 8});
    assert(mode1Start.modeSoundTimer == th07::kPlayerMode1SoundTimerValue);
    assert(mode1Start.primaryScaleX == 0.0f);
    assert(mode1Start.primaryScaleY == 3.0f);
    assert(mode1Start.primaryColorBits == 0x00ffffff);
    assert(mode1Start.renderFlags == th07::kPlayerMode1BlendFlag);
    assert(mode1Start.horizontalSpeedMultiplierBits == th07::kPlayerMode1SpeedMultiplierBits);
    assert(mode1Start.verticalSpeedMultiplierBits == th07::kPlayerMode1SpeedMultiplierBits);
    assert(mode1Start.currentPower == 0);
    assert(!mode1Start.completed);

    const th07::PlayerMode1InvulnerableState mode1Mid =
        th07::BuildTh07PlayerMode1InvulnerableUpdateState({15, 0.0f, 0x20, 8});
    assert(mode1Mid.primaryScaleX == 0.5f);
    assert(mode1Mid.primaryScaleY == 2.0f);
    assert(mode1Mid.primaryColorBits == 0x7fffffff);
    assert(mode1Mid.renderFlags == 0x30);
    assert(!mode1Mid.completed);

    const th07::PlayerMode1InvulnerableState mode1Done =
        th07::BuildTh07PlayerMode1InvulnerableUpdateState({30, 0.0f, 0x10, 8});
    assert(mode1Done.completed);
    assert(mode1Done.modeState == th07::kPlayerMode1CompleteModeState);
    assert(mode1Done.primaryScaleX == 1.0f);
    assert(mode1Done.primaryScaleY == 1.0f);
    assert(mode1Done.primaryColorBits == th07::kPlayerMode1WhiteArgbBits);
    assert((mode1Done.renderFlags & th07::kPlayerMode1BlendFlag) == 0);
    assert(mode1Done.commonEffectPrevious == th07::kTh07TimerPreviousSentinel);
    assert(mode1Done.commonEffectSubframe == 0.0f);
    assert(mode1Done.commonEffectDuration == th07::kPlayerMode1CompleteCommonEffectDuration);
    assert(mode1Done.currentPower == 8);

    const th07::PlayerOptionShotEffectState effectNoSpawn =
        th07::BuildTh07PlayerOptionShotEffectState({6, 11.0f, 22.0f, 33.0f});
    assert(effectNoSpawn.counter == 7);
    assert(!effectNoSpawn.shouldSpawn);
    assert(effectNoSpawn.returnValue == 0);

    const th07::PlayerOptionShotEffectState effectSpawn =
        th07::BuildTh07PlayerOptionShotEffectState({7, 11.0f, 22.0f, 33.0f});
    assert(effectSpawn.counter == 8);
    assert(effectSpawn.shouldSpawn);
    assert(effectSpawn.effectId == th07::kPlayerOptionShotEffectSpawnId);
    assert(effectSpawn.spawnCount == th07::kPlayerOptionShotEffectSpawnCount);
    assert(effectSpawn.color == th07::kPlayerOptionShotEffectSpawnColor);
    assert(effectSpawn.spawnX == 11.0f);
    assert(effectSpawn.spawnY == 22.0f);
    assert(effectSpawn.spawnZ == 33.0f);

    const th07::PlayerOptionShotEffectState effectWrapped =
        th07::BuildTh07PlayerOptionShotEffectState({255, 1.0f, 2.0f, 3.0f});
    assert(effectWrapped.counter == 0);
    assert(effectWrapped.shouldSpawn);
    return 0;
}
