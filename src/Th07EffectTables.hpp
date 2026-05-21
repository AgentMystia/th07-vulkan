#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace th07 {

inline constexpr std::uint32_t kTh07EffectTableAddress = 0x0049efc0;
inline constexpr std::int32_t kTh07EffectTableCount = 34;
inline constexpr std::int32_t kTh07BombCommonEffectId = 0x19;

inline constexpr std::uint32_t kTh07EffectManagerAddress = 0x012fe250;
inline constexpr std::uint32_t kTh07EffectSpawnFunctionAddress = 0x0041c1c0;
inline constexpr std::uint32_t kTh07EffectSpawnWithAuxVectorFunctionAddress = 0x0041c400;
inline constexpr std::uint32_t kTh07EffectSpawnDedicatedFunctionAddress = 0x0041c610;
inline constexpr std::uint32_t kTh07EffectUpdateFunctionAddress = 0x0041c790;
inline constexpr std::uint32_t kTh07EffectDrawMainFunctionAddress = 0x0041ca10;
inline constexpr std::uint32_t kTh07EffectDrawIndexedFunctionAddress = 0x0041cb80;
inline constexpr std::uint32_t kTh07EffectLoadStageAnmFunctionAddress = 0x0041cde0;
inline constexpr std::uint32_t kTh07EffectReleaseAnmFunctionAddress = 0x0041d050;

inline constexpr std::uint32_t kTh07EffectInitRandomSplashAddress = 0x0041a370;
inline constexpr std::uint32_t kTh07EffectUpdateLinearAddress = 0x0041a4f0;
inline constexpr std::uint32_t kTh07EffectInitRandomSplashBigAddress = 0x0041a5a0;
inline constexpr std::uint32_t kTh07EffectInitRenderMode2Address = 0x0041a730;
inline constexpr std::uint32_t kTh07EffectUpdateOrbitingRenderMode2Address = 0x0041a750;
inline constexpr std::uint32_t kTh07EffectInitRadialDirectionAddress = 0x0041aa60;
inline constexpr std::uint32_t kTh07EffectUpdateRadialInwardShortAddress = 0x0041aaf0;
inline constexpr std::uint32_t kTh07EffectUpdatePlayerPositionWhileAnmActiveAddress = 0x0041abe0;
inline constexpr std::uint32_t kTh07EffectUpdateRadialInwardSlowAddress = 0x0041ac30;
inline constexpr std::uint32_t kTh07EffectUpdateRadialOutwardAddress = 0x0041ad10;
inline constexpr std::uint32_t kTh07EffectUpdatePerspectiveMotionAddress = 0x0041aef0;
inline constexpr std::uint32_t kTh07EffectInitPerspective0041b0b0Address = 0x0041b0b0;
inline constexpr std::uint32_t kTh07EffectInitPerspective0041b4a0Address = 0x0041b4a0;
inline constexpr std::uint32_t kTh07EffectInitPerspective0041b770Address = 0x0041b770;
inline constexpr std::uint32_t kTh07EffectInitPerspective0041b9f0Address = 0x0041b9f0;
inline constexpr std::uint32_t kTh07EffectInitPerspective0041bc20Address = 0x0041bc20;
inline constexpr std::uint32_t kTh07EffectInitEasedRadialAddress = 0x0041bec0;
inline constexpr std::uint32_t kTh07EffectUpdateEasedRadialAddress = 0x0041bfd0;
inline constexpr std::uint32_t kTh07EffectUpdateCameraSnapAddress = 0x0041c100;
inline constexpr std::uint32_t kTh07GameManagerObjectAddress = 0x00626270;
inline constexpr std::uint32_t kTh07GameManagerUpdateFunctionAddress = 0x0042d8d5;
inline constexpr std::uint32_t kTh07GameManagerRegisterFunctionAddress = 0x0042f3c5;
inline constexpr std::uint32_t kTh07GameManagerCleanupFunctionAddress = 0x0042f2e4;
inline constexpr std::uint32_t kTh07GameManagerFrameCounterOffset = 0x95e8;
inline constexpr std::uint32_t kTh07GameManagerFrameCounterGlobalAddress =
    kTh07GameManagerObjectAddress + kTh07GameManagerFrameCounterOffset;
inline constexpr std::uint32_t kTh07ScreenEffectShakeFunctionAddress = 0x0044b0e0;
inline constexpr std::int32_t kTh07ScreenShakeRequiredGameFrameCount = 2;
inline constexpr std::uint32_t kTh07SetupDefaultCameraAddress = 0x00408180;
inline constexpr std::uint32_t kTh07SetupStageCameraAddress = 0x004082b0;
inline constexpr std::uint32_t kTh07AnmVmSetScriptFunctionAddress = 0x0044ea20;
inline constexpr std::uint32_t kTh07AnmVmSetActiveSpriteFunctionAddress = 0x0044e8e0;
inline constexpr std::uint32_t kTh07AnmVmResolveFloatOperandFunctionAddress = 0x00450a50;
inline constexpr std::uint32_t kTh07AnmVmResolveIntOperandFunctionAddress = 0x00450b20;
inline constexpr std::uint32_t kTh07AnmVmResolveFloatPointerFunctionAddress = 0x00450c10;
inline constexpr std::uint32_t kTh07AnmVmResolveIntPointerFunctionAddress = 0x00450ca0;
inline constexpr std::uint32_t kTh07AnmVmDrawScreenQuadFunctionAddress = 0x0044f9a0;
inline constexpr std::uint32_t kTh07AnmVmDrawScreenQuadNoRotationFunctionAddress = 0x0044f770;
inline constexpr std::uint32_t kTh07AnmVmSubmitQuadFunctionAddress = 0x0044efb0;
inline constexpr std::uint32_t kTh07RenderProjectedAnmQuadAddress = 0x0044fe00;
inline constexpr std::uint32_t kTh07AnmVmDrawCameraProjectedFunctionAddress = 0x00450130;
inline constexpr std::uint32_t kTh07AnmVmBuildTransformedQuadFunctionAddress = 0x004501a0;
inline constexpr std::uint32_t kTh07AnmVmDrawTransformedCameraProjectedFunctionAddress = 0x004504b0;
inline constexpr std::uint32_t kTh07StageObjectAddress = 0x01347b00;
inline constexpr std::uint32_t kTh07StageObjectClearDwordCount = 0x14ad;
inline constexpr std::uint32_t kTh07StageObjectClearSize = kTh07StageObjectClearDwordCount * 4;
inline constexpr std::uint32_t kTh07StageUpdateFunctionAddress = 0x00405690;
inline constexpr std::uint32_t kTh07StageSetAnmVmScriptFunctionAddress = 0x00404f30;
inline constexpr std::uint32_t kTh07StageAnmVmExecuteFunctionAddress = 0x00450d60;
inline constexpr std::uint32_t kTh07StageDrawHighPrioFunctionAddress = 0x004069d0;
inline constexpr std::uint32_t kTh07AnmVmTickTimerFunctionAddress = 0x0043958d;
inline constexpr std::uint32_t kTh07AnmVmRandomU32FunctionAddress = 0x004318d0;
inline constexpr std::uint32_t kTh07AnmVmRandomFloatZeroToOneFunctionAddress = 0x00431900;
inline constexpr std::uint32_t kTh07AnmVmNormalizeAngleFunctionAddress = 0x00431930;
inline constexpr std::uint32_t kTh07AnmVmFloatToIntFunctionAddress = 0x0048b8a0;
inline constexpr std::uint32_t kTh07AnmVmRoundFloatToByteFunctionAddress =
    kTh07AnmVmFloatToIntFunctionAddress;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0047eca0Address = 0x0047eca0;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0048b920Address = 0x0048b920;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0048ba20Address = 0x0048ba20;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0048bb0aAddress = 0x0048bb0a;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0048bb40Address = 0x0048bb40;
inline constexpr std::uint32_t kTh07AnmVmMathHelper0048bbf0Address = 0x0048bbf0;
inline constexpr std::uint32_t kTh07StageFirstAnmVmOffset = 0x0004;
inline constexpr std::uint32_t kTh07StageSecondAnmVmOffset = 0x0250;
inline constexpr std::uint32_t kTh07StageAnmVmSlotSize = 0x024c;
inline constexpr std::uint32_t kTh07AnmVmRotationXOffset = 0x0000;
inline constexpr std::uint32_t kTh07AnmVmRotationYOffset = 0x0004;
inline constexpr std::uint32_t kTh07AnmVmRotationZOffset = 0x0008;
inline constexpr std::uint32_t kTh07AnmVmRotationOffset = kTh07AnmVmRotationXOffset;
inline constexpr std::uint32_t kTh07AnmVmAngularVelocityXOffset = 0x000c;
inline constexpr std::uint32_t kTh07AnmVmAngularVelocityYOffset = 0x0010;
inline constexpr std::uint32_t kTh07AnmVmAngularVelocityZOffset = 0x0014;
inline constexpr std::uint32_t kTh07AnmVmAngularVelocityOffset = kTh07AnmVmAngularVelocityXOffset;
inline constexpr std::uint32_t kTh07AnmVmScaleXOffset = 0x0018;
inline constexpr std::uint32_t kTh07AnmVmScaleYOffset = 0x001c;
inline constexpr std::uint32_t kTh07AnmVmScaleVelocityXOffset = 0x0020;
inline constexpr std::uint32_t kTh07AnmVmScaleVelocityYOffset = 0x0024;
inline constexpr std::uint32_t kTh07AnmVmAngleField28Offset = 0x0028;
inline constexpr std::uint32_t kTh07AnmVmAngleField2cOffset = 0x002c;
inline constexpr std::uint32_t kTh07AnmVmScriptTimerPreviousOffset = 0x0030;
inline constexpr std::uint32_t kTh07AnmVmScriptTimerSubframeOffset = 0x0034;
inline constexpr std::uint32_t kTh07AnmVmScriptTimerCurrentOffset = 0x0038;
inline constexpr std::uint32_t kTh07AnmVmWaitTimerPreviousOffset = 0x003c;
inline constexpr std::uint32_t kTh07AnmVmWaitTimerSubframeOffset = 0x0040;
inline constexpr std::uint32_t kTh07AnmVmWaitTimerCurrentOffset = 0x0044;
inline constexpr std::uint32_t kTh07AnmVmPositionInterpolationTimerOffset = 0x0048;
inline constexpr std::uint32_t kTh07AnmVmColorInterpolationTimerOffset = 0x0054;
inline constexpr std::uint32_t kTh07AnmVmVisibilityInterpolationTimerOffset = 0x0060;
inline constexpr std::uint32_t kTh07AnmVmRotationInterpolationTimerOffset = 0x006c;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationTimerOffset = 0x0078;
inline constexpr std::uint32_t kTh07AnmVmPositionInterpolationDurationOffset = 0x008c;
inline constexpr std::uint32_t kTh07AnmVmColorInterpolationDurationOffset = 0x0098;
inline constexpr std::uint32_t kTh07AnmVmVisibilityInterpolationDurationOffset = 0x00a4;
inline constexpr std::uint32_t kTh07AnmVmRotationInterpolationDurationOffset = 0x00b0;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationDurationOffset = 0x00bc;
inline constexpr std::uint32_t kTh07AnmVmPositionInterpolationModeOffset = 0x00c0;
inline constexpr std::uint32_t kTh07AnmVmColorInterpolationModeOffset = 0x00c1;
inline constexpr std::uint32_t kTh07AnmVmVisibilityInterpolationModeOffset = 0x00c2;
inline constexpr std::uint32_t kTh07AnmVmRotationInterpolationModeOffset = 0x00c3;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationModeOffset = 0x00c4;
inline constexpr std::uint32_t kTh07AnmVmAngleField28VelocityOffset = 0x00f0;
inline constexpr std::uint32_t kTh07AnmVmAngleField2cVelocityOffset = 0x00f4;
inline constexpr std::uint32_t kTh07AnmVmColorOffset = 0x01b8;
inline constexpr std::uint32_t kTh07AnmVmVisibilityByteOffset = 0x01bb;
inline constexpr std::uint32_t kTh07AnmVmAlternateColorOffset = 0x01bc;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagsOffset = 0x01c0;
inline constexpr std::uint32_t kTh07AnmVmField1c4Offset = 0x01c4;
inline constexpr std::uint32_t kTh07AnmVmPendingInterruptOffset = 0x01c6;
inline constexpr std::uint32_t kTh07AnmVmDrawPositionOffset = 0x01c8;
inline constexpr std::uint32_t kTh07AnmVmActiveSpriteIndexOffset = 0x01d4;
inline constexpr std::uint32_t kTh07AnmVmBaseSpriteIndexOffset = 0x01d6;
inline constexpr std::uint32_t kTh07AnmVmScriptIndexOffset = 0x01d8;
inline constexpr std::uint32_t kTh07AnmVmBeginningOfScriptOffset = 0x01dc;
inline constexpr std::uint32_t kTh07AnmVmCurrentInstructionOffset = 0x01e0;
inline constexpr std::uint32_t kTh07AnmVmLoadedSpriteOffset = 0x01e4;
inline constexpr std::uint32_t kTh07AnmVmPositionInterpolationInitialOffset = 0x01e8;
inline constexpr std::uint32_t kTh07AnmVmPositionInterpolationTargetOffset = 0x01f4;
inline constexpr std::uint32_t kTh07AnmVmRotationInterpolationInitialOffset = 0x0200;
inline constexpr std::uint32_t kTh07AnmVmRotationInterpolationTargetOffset = 0x020c;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationInitialXOffset = 0x0218;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationInitialYOffset = 0x021c;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationFinalXOffset = 0x0220;
inline constexpr std::uint32_t kTh07AnmVmScaleInterpolationFinalYOffset = 0x0224;
inline constexpr std::uint32_t kTh07AnmVmColorInterpolationInitialOffset = 0x0228;
inline constexpr std::uint32_t kTh07AnmVmVisibilityInterpolationInitialOffset = 0x022b;
inline constexpr std::uint32_t kTh07AnmVmColorInterpolationTargetOffset = 0x022c;
inline constexpr std::uint32_t kTh07AnmVmVisibilityInterpolationTargetOffset = 0x022f;
inline constexpr std::uint32_t kTh07AnmVmAuxVectorOffset = 0x0230;
inline constexpr std::uint32_t kTh07AnmVmLastSpriteSetFrameOffset = 0x023c;
inline constexpr std::uint32_t kTh07AnmVmByte240Offset = 0x0240;
inline constexpr std::uint32_t kTh07AnmVmByte241Offset = 0x0241;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagActive = 1u << 0;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagDrawable = 1u << 1;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagRotationDirty = 1u << 2;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagScaleDirty = 1u << 3;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagBit4 = 1u << 4;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagPositionTargetsAux = 1u << 7;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagAnchorX = 1u << 10;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagAnchorY = 1u << 11;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagAnchorTopLeft = 0xc00;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagBit12 = 0x1000;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagInterruptPending = 0x2000;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagBit14 = 0x4000;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagSkipTransformRebuild = 0x8000;
inline constexpr std::uint32_t kTh07AnmVmRenderFlagUseAlternateColor = 1u << 16;
inline constexpr std::int32_t kTh07AnmVmInterruptLabelOpcode = 0x15;
inline constexpr std::int32_t kTh07AnmVmFallbackInterruptLabel = -1;
inline constexpr std::int32_t kTh07AnmVmInterpolationModeFromInstruction = -1;
inline constexpr std::size_t kTh07AnmVmInstructionOpcodeOffset = 0x00;
inline constexpr std::size_t kTh07AnmVmInstructionSizeOffset = 0x02;
inline constexpr std::size_t kTh07AnmVmInstructionTimeOffset = 0x04;
inline constexpr std::size_t kTh07AnmVmInstructionOperandFlagsOffset = 0x06;
inline constexpr std::size_t kTh07AnmVmInstructionOperandsOffset = 0x08;
inline constexpr std::size_t kTh07AnmVmInstructionHeaderSize = 0x08;
inline constexpr std::uint16_t kTh07AnmVmOperandFlagArg0IsReference = 1u << 0;
inline constexpr std::uint16_t kTh07AnmVmOperandFlagArg1IsReference = 1u << 1;
inline constexpr std::uint16_t kTh07AnmVmOperandFlagArg2IsReference = 1u << 2;
inline constexpr std::int32_t kTh07AnmVmVariableIdBase = 10000;
inline constexpr std::int32_t kTh07AnmVmIntVariable0Id = 10000;
inline constexpr std::int32_t kTh07AnmVmIntVariable1Id = 0x2711;
inline constexpr std::int32_t kTh07AnmVmIntVariable2Id = 0x2712;
inline constexpr std::int32_t kTh07AnmVmIntVariable3Id = 0x2713;
inline constexpr std::int32_t kTh07AnmVmFloatVariable0Id = 0x2714;
inline constexpr std::int32_t kTh07AnmVmFloatVariable1Id = 0x2715;
inline constexpr std::int32_t kTh07AnmVmFloatVariable2Id = 0x2716;
inline constexpr std::int32_t kTh07AnmVmFloatVariable3Id = 0x2717;
inline constexpr std::int32_t kTh07AnmVmIntVariable4Id = 0x2718;
inline constexpr std::int32_t kTh07AnmVmIntVariable5Id = 0x2719;
inline constexpr std::uint32_t kTh07AnmVmIntVariable0Offset = 0x00c8;
inline constexpr std::uint32_t kTh07AnmVmIntVariable1Offset = 0x00cc;
inline constexpr std::uint32_t kTh07AnmVmIntVariable2Offset = 0x00d0;
inline constexpr std::uint32_t kTh07AnmVmIntVariable3Offset = 0x00d4;
inline constexpr std::uint32_t kTh07AnmVmFloatVariable0Offset = 0x00d8;
inline constexpr std::uint32_t kTh07AnmVmFloatVariable1Offset = 0x00dc;
inline constexpr std::uint32_t kTh07AnmVmFloatVariable2Offset = 0x00e0;
inline constexpr std::uint32_t kTh07AnmVmFloatVariable3Offset = 0x00e4;
inline constexpr std::uint32_t kTh07AnmVmIntVariable4Offset = 0x00e8;
inline constexpr std::uint32_t kTh07AnmVmIntVariable5Offset = 0x00ec;
inline constexpr std::uint32_t kTh07AnmVmBaseTransformMatrixOffset = 0x00f8;
inline constexpr std::uint32_t kTh07AnmVmActiveTransformMatrixOffset = 0x0138;
inline constexpr std::size_t kTh07AnmVmVariableSlotCount = 10;
inline constexpr std::uint32_t kTh07StageAnmVmDrawPositionOffset = kTh07AnmVmDrawPositionOffset;
inline constexpr std::uint32_t kTh07StageAnmVmActiveSpriteIndexOffset = kTh07AnmVmActiveSpriteIndexOffset;
inline constexpr std::uint32_t kTh07StageAnmVmScriptIndexOffset = kTh07AnmVmScriptIndexOffset;
inline constexpr std::uint32_t kTh07StageAnmVmVector230Offset = kTh07AnmVmAuxVectorOffset;
inline constexpr std::uint32_t kTh07StageAnmVmByte240Offset = kTh07AnmVmByte240Offset;
inline constexpr std::uint32_t kTh07StageAnmVmByte241Offset = kTh07AnmVmByte241Offset;
inline constexpr std::int32_t kTh07StageAnmVmScriptBase = 0x0300;
inline constexpr std::uint32_t kTh07CameraPositionGlobalAddress = 0x0134ccf8;
inline constexpr std::uint32_t kTh07CameraOffsetGlobalAddress = 0x0134cd04;
inline constexpr std::uint32_t kTh07CameraForwardGlobalAddress = 0x0134cd1c;
inline constexpr std::uint32_t kTh07CameraProjectionReferencePointGlobalAddress = 0x0134cd28;
inline constexpr std::uint32_t kTh07StageCameraPositionOffset = 0x51f8;
inline constexpr std::uint32_t kTh07StageCameraOffsetOffset = 0x5204;
inline constexpr std::uint32_t kTh07StageCameraUpOffset = 0x5210;
inline constexpr std::uint32_t kTh07StageCameraForwardOffset = 0x521c;
inline constexpr std::uint32_t kTh07StageCameraSideVectorOffset = 0x5228;
inline constexpr std::uint32_t kTh07StageCameraFovOffset = 0x5234;
inline constexpr std::uint32_t kTh07StageCameraInitialPositionZBits = 0x447a0000;
inline constexpr std::uint32_t kTh07StageCameraInitialUpYBits = 0x3f800000;
inline constexpr std::uint32_t kTh07StageCameraInitialFovBits = 0x3f060a92;
inline constexpr std::uint32_t kTh07StageCameraPositionTargetOffset = 0x50f8;
inline constexpr std::uint32_t kTh07StageCameraOffsetTargetOffset = 0x5104;
inline constexpr std::uint32_t kTh07StageCameraUpTargetOffset = 0x5110;
inline constexpr std::uint32_t kTh07StageCameraFovTargetOffset = 0x5134;
inline constexpr std::uint32_t kTh07StageCameraPositionInterpStartOffset = 0x5138;
inline constexpr std::uint32_t kTh07StageCameraOffsetInterpStartOffset = 0x5144;
inline constexpr std::uint32_t kTh07StageCameraUpInterpStartOffset = 0x5150;
inline constexpr std::uint32_t kTh07StageCameraFovInterpStartOffset = 0x5174;
inline constexpr std::uint32_t kTh07StageCameraPositionCurvePoint3Offset = 0x5178;
inline constexpr std::uint32_t kTh07StageCameraOffsetCurvePoint3Offset = 0x5184;
inline constexpr std::uint32_t kTh07StageCameraUpCurvePoint3Offset = 0x5190;
inline constexpr std::uint32_t kTh07StageCameraPositionCurvePoint2Offset = 0x51b8;
inline constexpr std::uint32_t kTh07StageCameraOffsetCurvePoint2Offset = 0x51c4;
inline constexpr std::uint32_t kTh07StageCameraUpCurvePoint2Offset = 0x51d0;
inline constexpr std::uint32_t kTh07StageCameraPositionInterpDurationOffset = 0x5238;
inline constexpr std::uint32_t kTh07StageCameraOffsetInterpDurationOffset = 0x523c;
inline constexpr std::uint32_t kTh07StageCameraUpInterpDurationOffset = 0x5240;
inline constexpr std::uint32_t kTh07StageCameraFovInterpDurationOffset = 0x5244;
inline constexpr std::uint32_t kTh07StageCameraPositionInterpTimerOffset = 0x5248;
inline constexpr std::uint32_t kTh07StageCameraOffsetInterpTimerOffset = 0x5254;
inline constexpr std::uint32_t kTh07StageCameraUpInterpTimerOffset = 0x5260;
inline constexpr std::uint32_t kTh07StageCameraFovInterpTimerOffset = 0x526c;
inline constexpr std::uint32_t kTh07StageCameraPositionInterpModeOffset = 0x5278;
inline constexpr std::uint32_t kTh07StageCameraOffsetInterpModeOffset = 0x527c;
inline constexpr std::uint32_t kTh07StageCameraUpInterpModeOffset = 0x5280;
inline constexpr std::uint32_t kTh07StageCameraFovInterpModeOffset = 0x5284;
inline constexpr std::uint32_t kTh07StageStdDataOffset = 0x49c;
inline constexpr std::uint32_t kTh07StageQuadCountOffset = 0x4a0;
inline constexpr std::uint32_t kTh07StageObjectCountOffset = 0x4a4;
inline constexpr std::uint32_t kTh07StageObjectTableOffset = 0x4a8;
inline constexpr std::uint32_t kTh07StageObjectInstancesOffset = 0x4ac;
inline constexpr std::uint32_t kTh07StageScriptOffset = 0x4b0;
inline constexpr std::uint32_t kTh07StageScriptTimerOffset = 0x4b4;
inline constexpr std::uint32_t kTh07StageInstructionIndexOffset = 0x4c0;
inline constexpr std::uint32_t kTh07StageFrameCounterOffset = 0x4c4;
inline constexpr std::uint32_t kTh07StageRouteOffset = 0x4c8;
inline constexpr std::uint32_t kTh07StagePositionOffset = 0x4cc;
inline constexpr std::uint32_t kTh07StageField4d8Offset = 0x4d8;
inline constexpr std::uint32_t kTh07StageFogNearOffset = 0x4dc;
inline constexpr std::uint32_t kTh07StageFogFarOffset = 0x4e0;
inline constexpr std::uint32_t kTh07StageFogColorOffset = 0x4e4;
inline constexpr std::uint32_t kTh07StageFogInterpInitialOffset = 0x4e8;
inline constexpr std::uint32_t kTh07StageFogInterpFinalOffset = 0x4f4;
inline constexpr std::uint32_t kTh07StageFogInterpDurationOffset = 0x500;
inline constexpr std::uint32_t kTh07StageFogInterpTimerOffset = 0x504;
inline constexpr std::uint32_t kTh07StageSpellcardStateOffset = 0x514;
inline constexpr std::uint32_t kTh07StageSpellcardTimerOffset = 0x518;
inline constexpr std::uint32_t kTh07StageSpellcardBackgroundVmCountOffset = 0x520;
inline constexpr std::uint32_t kTh07StageSpellcardBackgroundVmOffset = 0x528;
inline constexpr std::uint32_t kTh07StagePositionInterpFinalOffset = 0x5288;
inline constexpr std::uint32_t kTh07StagePositionInterpEndFrameOffset = 0x5294;
inline constexpr std::uint32_t kTh07StagePositionInterpInitialOffset = 0x5298;
inline constexpr std::uint32_t kTh07StagePositionInterpStartFrameOffset = 0x52a4;
inline constexpr std::uint32_t kTh07StageCameraSetupPendingOffset = 0x52a8;
inline constexpr std::uint32_t kTh07StageAnmManagerColorWriteFunctionAddress = 0x00406930;
inline constexpr std::uint32_t kTh07StageAnmManagerColorResetGuardGlobalAddress = 0x0134cdb0;
inline constexpr std::uint32_t kTh07StageAnmManagerColorMultiplierOffset = 0x52ac;
inline constexpr std::uint32_t kTh07StageAnmManagerColorResetGuardOffset = 0x52b0;
inline constexpr std::uint32_t kTh07StageAnmManagerColorMultiplierIdle = 0x00808080;
inline constexpr std::int32_t kTh07StageCameraCurveInterpolationMode = 7;
inline constexpr std::int32_t kTh07StageCameraInitialTimerPreviousFrame = -999;
static_assert(kTh07StageObjectClearSize == 0x52b4);
static_assert(kTh07GameManagerFrameCounterGlobalAddress == 0x0062f858);
static_assert(kTh07CameraPositionGlobalAddress - kTh07StageObjectAddress == kTh07StageCameraPositionOffset);
static_assert(kTh07CameraOffsetGlobalAddress - kTh07StageObjectAddress == kTh07StageCameraOffsetOffset);
static_assert(kTh07StageAnmManagerColorResetGuardGlobalAddress - kTh07StageObjectAddress ==
              kTh07StageAnmManagerColorResetGuardOffset);
static_assert(kTh07CameraForwardGlobalAddress - kTh07StageObjectAddress == kTh07StageCameraForwardOffset);
static_assert(kTh07CameraProjectionReferencePointGlobalAddress - kTh07StageObjectAddress ==
              kTh07StageCameraSideVectorOffset);
static_assert(kTh07AnmVmRenderFlagAnchorTopLeft ==
              (kTh07AnmVmRenderFlagAnchorX | kTh07AnmVmRenderFlagAnchorY));

inline constexpr std::uint32_t kTh07EffectSlotSize = 0x2d8;
inline constexpr std::uint32_t kTh07EffectArrayOffset = 0x1c;
inline constexpr std::int32_t kTh07EffectPrimarySlotCount = 400;
inline constexpr std::int32_t kTh07EffectDedicatedSlotBaseIndex = 400;
inline constexpr std::int32_t kTh07EffectDedicatedSlotCount = 8;
inline constexpr std::int32_t kTh07EffectUpdateSlotCount = 0x198;
inline constexpr std::uint32_t kTh07EffectFallbackSlotOffset = 0x4885c;

inline constexpr std::uint32_t kTh07EffectNextIndexOffset = 0x0;
inline constexpr std::uint32_t kTh07EffectActiveCountOffset = 0x8;
inline constexpr std::uint32_t kTh07EffectFrameCounterOffset = 0x496a4;

inline constexpr std::uint32_t kTh07EffectDrawListScrollHeadOffset = 0x48e08;
inline constexpr std::uint32_t kTh07EffectDrawListIndexedHeadOffset = 0x490e0;
inline constexpr std::uint32_t kTh07EffectDrawListNoScrollHeadOffset = 0x493b8;
inline constexpr std::uint32_t kTh07EffectDrawListScrollFlaggedHeadOffset = 0x49690;
inline constexpr std::uint32_t kTh07EffectDrawListScrollTailOffset = 0x49694;
inline constexpr std::uint32_t kTh07EffectDrawListIndexedTailOffset = 0x49698;
inline constexpr std::uint32_t kTh07EffectDrawListNoScrollTailOffset = 0x4969c;
inline constexpr std::uint32_t kTh07EffectDrawListScrollFlaggedTailOffset = 0x496a0;
inline constexpr std::uint32_t kTh07EffectDrawListScrollSentinelOffset = 0x48b34;
inline constexpr std::uint32_t kTh07EffectDrawListIndexedSentinelOffset = 0x48e0c;
inline constexpr std::uint32_t kTh07EffectDrawListNoScrollSentinelOffset = 0x490e4;
inline constexpr std::uint32_t kTh07EffectDrawListScrollFlaggedSentinelOffset = 0x493bc;

inline constexpr std::uint32_t kTh07EffectSlotColorOffset = 0x1b8;
inline constexpr std::uint32_t kTh07EffectSlotRenderFlagsOffset = 0x1c0;
inline constexpr std::uint32_t kTh07EffectSlotDrawPositionOffset = 0x1c8;
inline constexpr std::uint32_t kTh07EffectSlotAnmScriptOffset = 0x1d8;
inline constexpr std::uint32_t kTh07EffectSlotAnmInstructionPointerOffset = 0x1e0;
inline constexpr std::uint32_t kTh07EffectSlotPositionOffset = 0x24c;
inline constexpr std::uint32_t kTh07EffectSlotAuxVectorOffset = 0x258;
inline constexpr std::uint32_t kTh07EffectSlotVelocityOffset = 0x264;
inline constexpr std::uint32_t kTh07EffectSlotAccelerationOffset = 0x270;
inline constexpr std::uint32_t kTh07EffectSlotOriginOffset = 0x288;
inline constexpr std::uint32_t kTh07EffectSlotDirectionOffset = 0x294;
inline constexpr std::uint32_t kTh07EffectSlotTimerPreviousFrameOffset = 0x2b8;
inline constexpr std::uint32_t kTh07EffectSlotTimerSubframeOffset = 0x2bc;
inline constexpr std::uint32_t kTh07EffectSlotTimerFrameOffset = 0x2c0;
inline constexpr std::uint32_t kTh07EffectSlotUpdateCallbackOffset = 0x2c8;
inline constexpr std::uint32_t kTh07EffectSlotActiveFlagOffset = 0x2cc;
inline constexpr std::uint32_t kTh07EffectSlotEffectIdOffset = 0x2cd;
inline constexpr std::uint32_t kTh07EffectSlotFadeFlagOffset = 0x2ce;
inline constexpr std::uint32_t kTh07EffectSlotFadeFrameOffset = 0x2cf;
inline constexpr std::uint32_t kTh07EffectSlotRenderModeOffset = 0x2d0;
inline constexpr std::uint32_t kTh07EffectSlotNextDrawOffset = 0x2d4;
inline constexpr std::uint32_t kTh07EffectSlotInitialRenderFlagBits = 0x1000;
inline constexpr std::uint32_t kTh07EffectSlotInitialTimerPreviousFrame = 0xfffffc19;

inline constexpr float kTh07EffectRandomSplashSpan = 256.0f;
inline constexpr float kTh07EffectRandomSplashCenter = 128.0f;
inline constexpr float kTh07EffectRandomSplashVelocityDivisor = 12.0f;
inline constexpr float kTh07EffectRandomSplashAccelerationFrames = 19.0f;
inline constexpr float kTh07EffectRandomSplashBigVelocityMultiplier = 4.0f;
inline constexpr float kTh07EffectRandomSplashBigVelocityDivisor = 33.0f;
inline constexpr float kTh07EffectRandomSplashBigAccelerationFrames = 20.0f;
inline constexpr float kTh07EffectRadialRandomAngleScale = 6.2831855f;
inline constexpr float kTh07EffectRadialRandomAngleBias = 3.1415927f;
inline constexpr float kTh07EffectRadialDistance = 256.0f;
inline constexpr float kTh07EffectRadialInwardShortFrames = 60.0f;
inline constexpr float kTh07EffectRadialInwardSlowFrames = 240.0f;
inline constexpr float kTh07EffectRadialOutwardFrames = 30.0f;
inline constexpr float kTh07EffectOrbitSideVectorLengthEpsilon = 0.00001f;
inline constexpr float kTh07EffectOrbitZScale = 6.0f;
inline constexpr float kTh07EffectOrbitFadeFrames = 16.0f;
inline constexpr float kTh07EffectOrbitFadeAlphaScale = 255.0f;
inline constexpr float kTh07EffectOrbitFadeScaleBase = 2.0f;
inline constexpr float kTh07EffectPerspectiveForwardDotThreshold = 0.94f;
inline constexpr float kTh07EffectPerspectiveZAliveThreshold = 0.0f;
inline constexpr std::uint32_t kTh07EffectPerspectiveRenderFlagBits = 0x4;
inline constexpr std::int32_t kTh07EffectPerspectiveRenderMode = 1;
inline constexpr std::int32_t kTh07EffectCameraSnapRenderMode = 3;
inline constexpr std::int32_t kTh07EffectPerspectiveWhiteAnmScript = 0x02d8;
inline constexpr std::int32_t kTh07EffectPerspectiveRgbWhiteGateScale = 100;
inline constexpr float kTh07EffectPerspectiveCameraOffsetHalf = 0.5f;
inline constexpr float kTh07EffectPerspectiveTargetXYSpan = 160.0f;
inline constexpr float kTh07EffectPerspectiveTargetXYCenter = 80.0f;
inline constexpr float kTh07EffectPerspectiveWideTargetXSpan = 120.0f;
inline constexpr float kTh07EffectPerspectiveWideTargetXCenter = 60.0f;
inline constexpr float kTh07EffectPerspectiveWideTargetYSpan = 200.0f;
inline constexpr float kTh07EffectPerspectiveWideTargetYCenter = 100.0f;
inline constexpr float kTh07EffectPerspectiveWideTargetZCenter = 100.0f;
inline constexpr float kTh07EffectPerspectiveTargetZSpan = 100.0f;
inline constexpr float kTh07EffectPerspectiveTargetZCenter = 50.0f;
inline constexpr float kTh07EffectPerspectiveForwardTargetZSpan = 200.0f;
inline constexpr float kTh07EffectPerspectiveVelocityXYSpan = 0.06f;
inline constexpr float kTh07EffectPerspectiveVelocityXYCenter = 0.03f;
inline constexpr float kTh07EffectPerspectiveVelocityZSpan = 0.1f;
inline constexpr float kTh07EffectPerspectiveVelocityZBase = 0.03f;
inline constexpr float kTh07EffectPerspectiveHighVelocityZBase = 0.09f;
inline constexpr float kTh07EffectPerspectiveNegativeVelocityZSpan = 0.2f;
inline constexpr float kTh07EffectPerspectiveNegativeVelocityZBase = 0.06f;
inline constexpr float kTh07EffectPerspectiveLowVelocityZSpan = 0.02f;
inline constexpr float kTh07EffectPerspectiveLowVelocityZBase = 0.01f;
inline constexpr float kTh07EffectPerspectiveAccelerationXYSpan = 0.0002f;
inline constexpr float kTh07EffectPerspectiveAccelerationXYCenter = 0.0001f;
inline constexpr float kTh07EffectPerspectiveFallAccelerationZ = -0.015f;
inline constexpr float kTh07EffectPerspectiveSmallRotationXSpan = 0.031415928f;
inline constexpr float kTh07EffectPerspectiveSmallRotationXCenter = 0.015707964f;
inline constexpr float kTh07EffectPerspectiveRotationXSpan = 0.062831856f;
inline constexpr float kTh07EffectPerspectiveRotationXCenter = 0.031415928f;
inline constexpr float kTh07EffectEasedRadialAngleSentinel = -990.0f;
inline constexpr float kTh07EffectEasedRadialRadiusScale = 1.5f;
inline constexpr float kTh07EffectEasedRadialRadiusBase = 1.0f;
inline constexpr float kTh07EffectEasedRadialDistance = 128.0f;
inline constexpr float kTh07EffectEasedRadialFrames = 90.0f;

struct Th07EffectTableEntry {
    std::int32_t anmScript;
    std::uint32_t updateCallbackAddress;
    std::uint32_t initCallbackAddress;
};

struct Th07EffectDrawListLayout {
    std::string_view name;
    std::uint32_t headOffset;
    std::uint32_t tailOffset;
    std::uint32_t sentinelOffset;
};

struct Th07EffectSpawnContract {
    bool valid;
    std::int32_t effectId;
    std::int32_t slotIndex;
    std::uint32_t slotOffset;
    std::uint32_t color;
    std::int32_t anmScript;
    std::uint32_t updateCallbackAddress;
    std::uint32_t initCallbackAddress;
    std::uint32_t renderFlagBitsSet;
    std::uint32_t timerPreviousFrame;
    std::uint32_t timerSubframe;
    std::uint32_t timerFrame;
};

struct Th07EffectVector3 {
    float x;
    float y;
    float z;
};

struct Th07EffectPerspectiveCameraBasis {
    Th07EffectVector3 position;
    Th07EffectVector3 offset;
};

struct Th07StageCameraState {
    Th07EffectVector3 position;
    Th07EffectVector3 offset;
    Th07EffectVector3 up;
    Th07EffectVector3 forward;
    Th07EffectVector3 sideVector;
    float fov;
};

struct Th07StageCameraSetupState {
    Th07StageCameraState camera;
    Th07EffectVector3 target;
    Th07EffectPerspectiveCameraBasis effectCameraBasis;
};

struct Th07StageCameraInterpolationTimerState {
    std::int32_t previousFrame;
    float subframe;
    std::int32_t currentFrame;
};

struct Th07StageCameraRuntimeState {
    Th07StageCameraState target;
    Th07StageCameraState interpolationStart;
    Th07StageCameraState curvePoint3;
    Th07StageCameraState curvePoint2;
    Th07StageCameraState current;
    std::array<std::int32_t, 4> interpolationDurations;
    std::array<Th07StageCameraInterpolationTimerState, 4> interpolationTimers;
    std::array<std::int32_t, 4> interpolationModes;
};

struct Th07StageCameraInstruction {
    std::int32_t opcode;
    std::array<std::int32_t, 3> args;
};

struct Th07StageScriptInstruction {
    std::int32_t frame;
    std::int32_t opcode;
    std::array<std::int32_t, 3> args;
};

struct Th07StageFogState {
    float nearPlane;
    float farPlane;
    std::uint32_t color;
};

struct Th07StageScriptRuntimeState {
    std::int32_t instructionIndex;
    Th07StageCameraInterpolationTimerState scriptTimer;
    std::int32_t frameCounter;
    Th07EffectVector3 position;
    std::int32_t field4d8;
    Th07StageFogState fog;
    Th07StageFogState fogInterpolationInitial;
    Th07StageFogState fogInterpolationFinal;
    std::int32_t fogInterpolationDuration;
    Th07StageCameraInterpolationTimerState fogInterpolationTimer;
    std::int32_t cameraResumeInstruction;
    Th07EffectVector3 positionInterpolationFinal;
    std::int32_t positionInterpolationEndFrame;
    Th07EffectVector3 positionInterpolationInitial;
    std::int32_t positionInterpolationStartFrame;
    bool cameraSetupPending;
};

enum class Th07StageAnmVmSlot {
    Primary,
    Secondary,
};

enum class Th07AnmVmRenderHelperKind {
    ScreenQuad,
    CameraProjected,
    TransformedCameraProjected,
};

struct Th07AnmVmRenderHelperContract {
    std::uint32_t functionAddress;
    Th07AnmVmRenderHelperKind kind;
    std::uint32_t quadBuilderAddress;
    bool checksActiveFlag;
    bool checksDrawableFlag;
    bool checksVisibilityByte;
    bool usesDrawPosition;
    bool usesLoadedSprite;
    bool usesCameraProjection;
    bool submitsQuad;
};

struct Th07AnmVmInstructionHeader {
    std::int16_t opcode;
    std::uint16_t size;
    std::int16_t time;
    std::uint16_t operandFlags;
};

enum class Th07AnmVmExecuteOpcodeSemantic {
    EndScript,
    SetActiveSprite,
    Jump,
    Loop,
    SetDrawPositionOrAux,
    SetScale,
    SetVisibility,
    SetRgb,
    ToggleHorizontalScale,
    ToggleVerticalScale,
    SetRotation,
    SetAngularVelocity,
};

struct Th07AnmVmExecuteOpcodeContract {
    std::int32_t opcode;
    Th07AnmVmExecuteOpcodeSemantic semantic;
    bool clearsActiveFlag;
    bool clearsCurrentInstruction;
    bool setsActiveFlag;
    bool jumpsToScriptOffset;
    bool writesActiveSprite;
    bool writesDrawPosition;
    bool writesAuxVector;
    bool writesScale;
    bool writesVisibilityByte;
    bool writesRgb;
    bool writesRotation;
    bool writesAngularVelocity;
    std::uint32_t setRenderFlags;
};

enum class Th07AnmVmInterpolationLane {
    None,
    Position,
    Color,
    Visibility,
    Rotation,
    Scale,
};

enum class Th07AnmVmFollowupOpcodeSemantic {
    SetScaleVelocity,
    StartVisibilityInterpolation,
    SetRenderFlagBit4,
    StartPositionInterpolation,
    StopUntilInterrupt,
    SetAnchorTopLeft,
    StopAndClearActiveUntilInterrupt,
    SetPositionTargetsAux,
    SetField1c4,
    AddAngleField28,
    AddAngleField2c,
    SetActiveRenderFlag,
    StartScaleInterpolation,
    SetRenderFlagBit12,
    SetRenderFlagBit14,
    StartColorInterpolation,
    StartRotationInterpolation,
};

struct Th07AnmVmFollowupOpcodeContract {
    std::int32_t opcode;
    Th07AnmVmFollowupOpcodeSemantic semantic;
    Th07AnmVmInterpolationLane interpolationLane;
    std::int32_t interpolationMode;
    std::uint32_t modeOffset;
    std::uint32_t timerOffset;
    std::uint32_t durationOffset;
    std::uint32_t primaryFieldOffset;
    std::uint32_t secondaryFieldOffset;
    std::uint32_t targetFieldOffset;
    std::uint32_t setRenderFlags;
    std::uint32_t controlledRenderFlag;
    bool clearsActiveFlag;
    bool stopsUntilInterrupt;
};

enum class Th07AnmVmVariableValueKind {
    Int,
    Float,
};

struct Th07AnmVmVariableSlot {
    std::int32_t variableId;
    std::uint32_t fieldOffset;
    Th07AnmVmVariableValueKind valueKind;
};

enum class Th07AnmVmVariableOpcodeSemantic {
    Assign,
    AddAssign,
    SubtractAssign,
    MultiplyAssign,
    DivideAssign,
    ModuloAssign,
    MathHelperAssign,
    AddOperands,
    SubtractOperands,
    MultiplyOperands,
    DivideOperands,
    ModuloOperands,
    RandomIntModulo,
    RandomFloatScaled,
    NormalizeAngle,
};

struct Th07AnmVmVariableOpcodeContract {
    std::int32_t opcode;
    Th07AnmVmVariableOpcodeSemantic semantic;
    Th07AnmVmVariableValueKind valueKind;
    std::int32_t sourceOperandCount;
    bool writesViaIntResolver;
    bool writesViaFloatResolver;
    bool readsCurrentDestination;
    bool usesIntSourceResolver;
    bool usesFloatSourceResolver;
    bool guardsZeroDivisor;
    std::uint32_t helperFunctionAddress;
};

enum class Th07AnmVmConditionalJumpComparison {
    Equal,
    NotEqual,
    LessThan,
    LessOrEqual,
    GreaterThan,
    GreaterOrEqual,
};

struct Th07AnmVmConditionalJumpOpcodeContract {
    std::int32_t opcode;
    Th07AnmVmConditionalJumpComparison comparison;
    Th07AnmVmVariableValueKind valueKind;
    std::int32_t comparedOperandCount;
    bool usesIntSourceResolver;
    bool usesFloatSourceResolver;
    std::int32_t jumpOffsetOperandIndex;
    std::int32_t jumpFrameOperandIndex;
    bool jumpsToScriptRelativeOffset;
    bool resetsScriptTimer;
};

enum class Th07AnmVmControlOpcodeSemantic {
    WaitTimer,
    SetAngleField28Velocity,
    SetAngleField2cVelocity,
};

struct Th07AnmVmControlOpcodeContract {
    std::int32_t opcode;
    Th07AnmVmControlOpcodeSemantic semantic;
    Th07AnmVmVariableValueKind valueKind;
    std::int32_t sourceOperandCount;
    std::uint32_t writesFieldOffset;
    std::uint32_t pairedAngleFieldOffset;
    std::uint32_t timerPreviousOffset;
    std::uint32_t timerSubframeOffset;
    std::uint32_t timerCurrentOffset;
    bool usesIntSourceResolver;
    bool usesFloatSourceResolver;
    bool pausesInstructionWhilePositive;
    bool clearsTimerWhenNonPositive;
    bool postStepAddsToAngleField;
};

enum class Th07AnmVmPostStepActionSemantic {
    ApplyAngularVelocity,
    AdvanceInterpolationLane,
    ApplyScaleVelocity,
    ApplyAngleFieldVelocity,
    AdvanceScriptTimer,
};

struct Th07AnmVmPostStepActionContract {
    Th07AnmVmPostStepActionSemantic semantic;
    Th07AnmVmInterpolationLane interpolationLane;
    std::uint32_t primaryFieldOffset;
    std::uint32_t secondaryFieldOffset;
    std::uint32_t targetFieldOffset;
    std::uint32_t velocityFieldOffset;
    std::uint32_t timerOffset;
    std::uint32_t durationOffset;
    std::uint32_t modeOffset;
    std::uint32_t setRenderFlags;
    std::uint32_t timerHelperFunctionAddress;
    std::uint32_t valueHelperFunctionAddress;
    bool usesFramerateMultiplier;
    bool wrapsAngle;
    bool selectsAuxPositionByRenderFlag;
    bool clearsDurationWhenComplete;
    bool incrementsManagerStepCounter;
};

struct Th07StageAnmVmOpcodeEffect {
    bool handled;
    Th07StageAnmVmSlot slot;
    bool loadScript;
    bool clearPrimaryActiveSprite;
    std::int32_t scriptIndex;
};

enum class Th07StageScriptStepResult {
    Unsupported,
    Applied,
    Jumped,
    Paused,
};

enum class Th07StageCameraChannel {
    Position,
    Offset,
    Up,
    Fov,
};

enum class Th07StageCameraOpcodeSemantic {
    SetLinearTarget,
    StartLinearInterpolation,
    SetCurvePoint0,
    SetCurvePoint1,
    SetCurvePoint2,
    SetCurvePoint3,
    StartCurveInterpolation,
};

struct Th07StageCameraVectorChannelLayout {
    Th07StageCameraChannel channel;
    std::uint32_t currentOffset;
    std::uint32_t linearTargetOffset;
    std::uint32_t interpStartOffset;
    std::uint32_t curvePoint2Offset;
    std::uint32_t curvePoint3Offset;
    std::uint32_t interpDurationOffset;
    std::uint32_t interpTimerOffset;
    std::uint32_t interpModeOffset;
};

struct Th07StageCameraScalarChannelLayout {
    Th07StageCameraChannel channel;
    std::uint32_t currentOffset;
    std::uint32_t linearTargetOffset;
    std::uint32_t interpStartOffset;
    std::uint32_t interpDurationOffset;
    std::uint32_t interpTimerOffset;
    std::uint32_t interpModeOffset;
};

struct Th07StageCameraOpcodeLayout {
    std::int32_t opcode;
    Th07StageCameraChannel channel;
    Th07StageCameraOpcodeSemantic semantic;
    std::uint32_t primaryFieldOffset;
};

enum class Th07EffectPerspectiveInitKind {
    Init0041b0b0,
    Init0041b4a0,
    Init0041b770,
    Init0041b9f0,
    Init0041bc20,
};

struct Th07EffectPerspectiveInitRandoms {
    float targetX;
    float targetY;
    float targetZ;
    float velocityX;
    float velocityY;
    float velocityZ;
    float accelerationX;
    float accelerationY;
    float rotationZ;
    float rotationX;
};

struct Th07EffectQuaternion {
    float x;
    float y;
    float z;
    float w;
};

struct Th07EffectLinearMotion {
    Th07EffectVector3 velocity;
    Th07EffectVector3 acceleration;
};

struct Th07EffectRadialState {
    Th07EffectVector3 origin;
    Th07EffectVector3 direction;
};

struct Th07EffectAnchoredPosition {
    bool active;
    Th07EffectVector3 position;
};

struct Th07EffectOrbitState {
    Th07EffectVector3 position;
    Th07EffectQuaternion quaternion;
};

struct Th07EffectOrbitFadeStep {
    bool active;
    std::int32_t nextFrame;
    std::uint32_t color;
    float scale;
};

struct Th07EffectPerspectiveMotionState {
    Th07EffectVector3 velocity;
    Th07EffectVector3 targetPosition;
    Th07EffectVector3 position;
    float rotationZ;
    std::uint32_t renderFlags;
    bool inForwardCone;
    bool active;
};

struct Th07EffectPerspectiveInitState {
    Th07EffectVector3 targetPosition;
    Th07EffectVector3 velocity;
    Th07EffectVector3 acceleration;
    float rotationX;
    float rotationZ;
    std::int32_t renderMode;
    bool forceRgbWhite;
    bool doubleAngleVelocityZ;
};

struct Th07EffectCameraSnapState {
    Th07EffectVector3 targetPosition;
    Th07EffectVector3 position;
    std::int32_t renderMode;
    bool active;
};

struct Th07EffectAnmLoad {
    std::int32_t anmFileSlot;
    std::string_view archivePath;
    std::uint32_t archivePathAddress;
    std::int32_t scriptBase;
    std::uint32_t instructionAddress;
};

struct Th07EffectStageAnmLoad {
    std::int32_t currentStageValue;
    std::int32_t anmBankCount;
    std::uint32_t anmBankCountWriteAddress;
    std::array<Th07EffectAnmLoad, 2> loads;
};

extern const std::array<Th07EffectTableEntry, kTh07EffectTableCount> kTh07EffectTable;
extern const std::array<Th07EffectDrawListLayout, 4> kTh07EffectDrawLists;
extern const std::array<Th07EffectStageAnmLoad, 9> kTh07EffectStageAnmLoads;
extern const std::array<Th07StageCameraVectorChannelLayout, 3> kTh07StageCameraVectorChannels;
extern const Th07StageCameraScalarChannelLayout kTh07StageCameraFovChannel;
extern const std::array<Th07StageCameraOpcodeLayout, 23> kTh07StageCameraOpcodes;
extern const std::array<Th07AnmVmFollowupOpcodeContract, 22> kTh07AnmVmFollowupExecuteOpcodes;
extern const std::array<Th07AnmVmVariableSlot, kTh07AnmVmVariableSlotCount> kTh07AnmVmVariableSlots;
extern const std::array<Th07AnmVmVariableOpcodeContract, 30> kTh07AnmVmVariableExecuteOpcodes;
extern const std::array<Th07AnmVmConditionalJumpOpcodeContract, 12> kTh07AnmVmConditionalJumpExecuteOpcodes;
extern const std::array<Th07AnmVmControlOpcodeContract, 3> kTh07AnmVmControlExecuteOpcodes;
extern const std::array<Th07AnmVmPostStepActionContract, 13> kTh07AnmVmPostStepActions;

const Th07EffectTableEntry *FindTh07EffectTableEntry(std::int32_t effectId);
const Th07EffectStageAnmLoad *FindTh07EffectStageAnmLoad(std::int32_t currentStageValue);
const Th07StageCameraVectorChannelLayout *FindTh07StageCameraVectorChannel(Th07StageCameraChannel channel);
const Th07StageCameraOpcodeLayout *FindTh07StageCameraOpcode(std::int32_t opcode);
bool ReadTh07AnmVmInstructionHeader(std::span<const std::uint8_t> bytes,
                                    Th07AnmVmInstructionHeader *outHeader);
std::size_t Th07AnmVmInstructionNextOffset(Th07AnmVmInstructionHeader header);
std::size_t Th07AnmVmInstructionOperandOffset(std::size_t operandIndex);
std::int32_t TruncateTh07AnmVmFloatToI32(float value);
const Th07AnmVmVariableSlot *FindTh07AnmVmVariableSlot(std::int32_t variableId);
bool ReadTh07AnmVmInstructionI32Operand(std::span<const std::uint8_t> bytes,
                                        std::size_t operandIndex,
                                        std::int32_t *outValue);
bool ReadTh07AnmVmInstructionF32Operand(std::span<const std::uint8_t> bytes,
                                        std::size_t operandIndex,
                                        float *outValue);
std::uint32_t Th07EffectSlotOffset(std::int32_t slotIndex);
std::uint32_t Th07EffectDedicatedSlotOffset(std::int32_t dedicatedSlotArgument);
Th07EffectSpawnContract BuildTh07DedicatedEffectSpawnContract(std::int32_t effectId,
                                                              std::int32_t dedicatedSlotArgument,
                                                              std::uint32_t color);
Th07EffectLinearMotion BuildTh07EffectRandomSplashMotion(float randomXZeroToOne,
                                                         float randomYZeroToOne,
                                                         float framerateMultiplier);
Th07EffectLinearMotion BuildTh07EffectRandomSplashBigMotion(float randomXZeroToOne,
                                                            float randomYZeroToOne,
                                                            float framerateMultiplier);
Th07EffectRadialState BuildTh07EffectRadialState(Th07EffectVector3 position,
                                                 float randomZeroToOne);
Th07EffectVector3 BuildTh07EffectRadialInwardPosition(Th07EffectVector3 origin,
                                                      Th07EffectVector3 direction,
                                                      float timerFrames,
                                                      float durationFrames,
                                                      bool forceZToZero);
Th07EffectVector3 BuildTh07EffectRadialOutwardPosition(Th07EffectVector3 origin,
                                                       Th07EffectVector3 direction,
                                                       float timerFrames);
Th07EffectAnchoredPosition BuildTh07EffectPlayerAnchoredPosition(bool hasActiveAnmInstruction,
                                                                 Th07EffectVector3 playerPosition);
Th07StageCameraState BuildTh07InitialStageCameraState();
Th07StageCameraRuntimeState BuildTh07InitialStageCameraRuntimeState();
Th07StageCameraSetupState BuildTh07StageCameraSetupState(Th07EffectVector3 position,
                                                         Th07EffectVector3 offset,
                                                         Th07EffectVector3 up,
                                                         float fov);
Th07EffectPerspectiveCameraBasis BuildTh07EffectCameraBasis(Th07StageCameraState camera);
bool ApplyTh07StageCameraOpcode(Th07StageCameraRuntimeState *state,
                                Th07StageCameraInstruction instruction);
bool IsTh07StageNonCameraOpcode(std::int32_t opcode);
bool IsTh07StageScriptInstructionDue(Th07StageScriptRuntimeState state,
                                     Th07StageScriptInstruction instruction);
Th07StageAnmVmOpcodeEffect BuildTh07StageAnmVmOpcodeEffect(Th07StageScriptInstruction instruction);
const Th07AnmVmRenderHelperContract *FindTh07AnmVmRenderHelper(std::uint32_t functionAddress);
const Th07AnmVmExecuteOpcodeContract *FindTh07AnmVmExecuteOpcode(std::int32_t opcode);
const Th07AnmVmFollowupOpcodeContract *FindTh07AnmVmFollowupExecuteOpcode(std::int32_t opcode);
const Th07AnmVmVariableOpcodeContract *FindTh07AnmVmVariableExecuteOpcode(std::int32_t opcode);
const Th07AnmVmConditionalJumpOpcodeContract *FindTh07AnmVmConditionalJumpExecuteOpcode(std::int32_t opcode);
const Th07AnmVmControlOpcodeContract *FindTh07AnmVmControlExecuteOpcode(std::int32_t opcode);
const Th07AnmVmPostStepActionContract *FindTh07AnmVmPostStepAction(Th07AnmVmPostStepActionSemantic semantic,
                                                                   std::uint32_t primaryFieldOffset);
Th07StageScriptStepResult ApplyTh07StageScriptOpcode(Th07StageScriptRuntimeState *state,
                                                     Th07StageScriptInstruction instruction,
                                                     Th07StageScriptInstruction nextPositionKey);
void AdvanceTh07StageFogInterpolation(Th07StageScriptRuntimeState *state,
                                      float framerateMultiplier,
                                      float effectiveFramerateMultiplier);
void AdvanceTh07StageCameraRuntimeState(Th07StageCameraRuntimeState *state,
                                        float framerateMultiplier,
                                        float effectiveFramerateMultiplier);
float ApplyTh07StageCameraInterpolationCurve(float ratio, std::int32_t interpolationMode);
float BuildTh07StageCameraCurveComponent(float point0,
                                         float point1,
                                         float point2,
                                         float point3,
                                         float ratio);
Th07EffectVector3 BuildTh07StageCameraInterpolatedVector(Th07EffectVector3 point0,
                                                         Th07EffectVector3 point1,
                                                         Th07EffectVector3 point2,
                                                         Th07EffectVector3 point3,
                                                         float ratio,
                                                         std::int32_t interpolationMode);
float BuildTh07StageCameraInterpolatedScalar(float point0,
                                             float point1,
                                             float ratio,
                                             std::int32_t interpolationMode);
Th07EffectOrbitState BuildTh07EffectOrbitingRenderMode2State(Th07EffectVector3 origin,
                                                             Th07EffectVector3 direction,
                                                             float radius,
                                                             float angle);
Th07EffectOrbitFadeStep BuildTh07EffectOrbitFadeStep(std::int32_t currentFadeFrame,
                                                     std::uint32_t color);
Th07EffectPerspectiveMotionState BuildTh07EffectPerspectiveMotionState(Th07EffectVector3 velocity,
                                                                       Th07EffectVector3 acceleration,
                                                                       Th07EffectVector3 targetPosition,
                                                                       float rotationX,
                                                                       float rotationZ,
                                                                       std::uint32_t renderFlags,
                                                                       Th07EffectVector3 cameraPosition,
                                                                       Th07EffectVector3 cameraForward);
bool BuildTh07EffectPerspectiveRgbWhiteGate(std::int32_t maxValue,
                                            std::int32_t currentValue,
                                            std::int32_t denominator,
                                            std::uint32_t randomValue);
Th07EffectPerspectiveInitState BuildTh07EffectPerspectiveInitState(Th07EffectPerspectiveInitKind kind,
                                                                   Th07EffectVector3 auxVector,
                                                                   Th07EffectPerspectiveCameraBasis cameraBasis,
                                                                   Th07EffectPerspectiveInitRandoms randoms,
                                                                   float framerateMultiplier,
                                                                   bool rgbWhiteGatePassed);
Th07EffectCameraSnapState BuildTh07EffectCameraSnapState(Th07EffectPerspectiveCameraBasis cameraBasis);
float NormalizeTh07EffectAngle(float angle);
Th07EffectRadialState BuildTh07EffectEasedRadialStateFromAngle(Th07EffectVector3 position,
                                                               float angle,
                                                               float randomRadiusZeroToOne);
Th07EffectRadialState BuildTh07EffectEasedRadialStateFromRandomAngle(Th07EffectVector3 position,
                                                                     float randomAngleZeroToOne,
                                                                     float randomRadiusZeroToOne);
Th07EffectVector3 BuildTh07EffectEasedRadialPosition(Th07EffectVector3 origin,
                                                     Th07EffectVector3 scaledDirection,
                                                     float timerFrames);

} // namespace th07
