#pragma once

#include "AnmVm.hpp"
#include "Chain.hpp"
#include "Th07EffectTables.hpp"
#include "ZunTimer.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "zwave.hpp"
#include <cstddef>
#include <d3d8.h>
#include <d3dx8math.h>

namespace th07
{
struct RawStageHeader
{
    i16 nbObjects;
    i16 nbFaces;
    i32 facesOffset;
    i32 scriptOffset;
    i32 unk_c;
    char stageName[128];
    char songNames[4][128];
    char songPaths[4][128];
};
ZUN_ASSERT_SIZE(RawStageHeader, 0x490);

struct RawStageQuadBasic
{
    i16 type;
    i16 byteSize;
    i16 anmScript;
    i16 vmIdx;
    D3DXVECTOR3 position;
    D3DXVECTOR2 size;
};
ZUN_ASSERT_SIZE(RawStageQuadBasic, 0x1c);

struct RawStageObject
{
    i16 id;
    i8 zLevel;
    i8 flags;
    D3DXVECTOR3 position;
    D3DXVECTOR3 size;
    RawStageQuadBasic firstQuad;
};
ZUN_ASSERT_SIZE(RawStageObject, 0x38);

struct RawStageObjectInstance
{
    i16 id;
    i16 unk2;
    D3DXVECTOR3 position;
};
ZUN_ASSERT_SIZE(RawStageObjectInstance, 0x10);

struct RawStageInstr
{
    i32 frame;
    i16 opcode;
    i16 size;
    i32 args[3];
};
ZUN_ASSERT_SIZE(RawStageInstr, 0x14);

struct StageCameraSky
{
    f32 nearPlane;
    f32 farPlane;
    D3DCOLOR color;
};
ZUN_ASSERT_SIZE(StageCameraSky, 0xc);

struct StageCameraBasisBlock
{
    D3DXVECTOR3 position;
    D3DXVECTOR3 offset;
    D3DXVECTOR3 up;
    D3DXVECTOR3 forward;
    D3DXVECTOR3 sideVector;
    f32 fov;
};
ZUN_ASSERT_SIZE(StageCameraBasisBlock, 0x40);
static_assert(offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetOffset - kTh07StageCameraPositionOffset);
static_assert(offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpOffset - kTh07StageCameraPositionOffset);
static_assert(offsetof(StageCameraBasisBlock, forward) ==
              kTh07StageCameraForwardOffset - kTh07StageCameraPositionOffset);
static_assert(offsetof(StageCameraBasisBlock, sideVector) ==
              kTh07StageCameraSideVectorOffset - kTh07StageCameraPositionOffset);
static_assert(offsetof(StageCameraBasisBlock, fov) ==
              kTh07StageCameraFovOffset - kTh07StageCameraPositionOffset);

struct StageCameraInterpolationTimer
{
    i32 previousFrame;
    f32 subframe;
    i32 currentFrame;
};
ZUN_ASSERT_SIZE(StageCameraInterpolationTimer, 0xc);

struct Th07StageOriginalAnmVmSlot
{
    AnmVm vm;
    u8 th07PendingAnmVmFields_110_1b8[kTh07AnmVmColorOffset - sizeof(AnmVm)];
    D3DCOLOR originalColor;
    D3DCOLOR originalAlternateColor;
    u32 originalRenderFlags;
    u8 th07PendingAnmVmFields_1c4_1c6[kTh07AnmVmPendingInterruptOffset - 0x1c4];
    i16 originalPendingInterrupt;
    D3DXVECTOR3 originalDrawPosition;
    i16 originalActiveSpriteIndex;
    i16 originalBaseSpriteIndex;
    i16 originalScriptIndex;
    AnmRawInstr *originalBeginningOfScript;
    AnmRawInstr *originalCurrentInstruction;
    AnmLoadedSprite *originalLoadedSprite;
    u8 th07PendingAnmVmFields_1e8_230[kTh07AnmVmAuxVectorOffset - 0x1e8];
    D3DXVECTOR3 originalVector230;
    i32 originalLastSpriteSetFrame;
    u8 originalByte240;
    u8 originalByte241;
    u8 th07PendingAnmVmFields_242_24c[kTh07StageAnmVmSlotSize - 0x242];
};
ZUN_ASSERT_SIZE(Th07StageOriginalAnmVmSlot, kTh07StageAnmVmSlotSize);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalColor) == kTh07AnmVmColorOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalAlternateColor) == kTh07AnmVmAlternateColorOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalRenderFlags) == kTh07AnmVmRenderFlagsOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalPendingInterrupt) ==
              kTh07AnmVmPendingInterruptOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalDrawPosition) ==
              kTh07StageAnmVmDrawPositionOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalActiveSpriteIndex) ==
              kTh07StageAnmVmActiveSpriteIndexOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalBaseSpriteIndex) ==
              kTh07AnmVmBaseSpriteIndexOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalScriptIndex) ==
              kTh07StageAnmVmScriptIndexOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalBeginningOfScript) ==
              kTh07AnmVmBeginningOfScriptOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalCurrentInstruction) ==
              kTh07AnmVmCurrentInstructionOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalLoadedSprite) ==
              kTh07AnmVmLoadedSpriteOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalVector230) ==
              kTh07StageAnmVmVector230Offset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalLastSpriteSetFrame) ==
              kTh07AnmVmLastSpriteSetFrameOffset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalByte240) ==
              kTh07StageAnmVmByte240Offset);
static_assert(offsetof(Th07StageOriginalAnmVmSlot, originalByte241) ==
              kTh07StageAnmVmByte241Offset);

enum SpellcardState
{
    NOT_RUNNING,
    RUNNING,
    RAN_FOR_60_FRAMES
};

struct StageFile
{
    char *anmFile;
    char *stdFile;
};
ZUN_ASSERT_SIZE(StageFile, 0x8);

enum StageOpcode
{
    STDOP_CAMERA_POSITION_KEY,
    STDOP_FOG,
    STDOP_CAMERA_FACING,
    STDOP_CAMERA_FACING_INTERP_LINEAR,
    STDOP_FOG_INTERP,
    STDOP_PAUSE,
};

struct Stage
{
    Stage();
    static ZunResult RegisterChain(u32 stage);
    static void CutChain();
    static ChainCallbackResult OnUpdate(Stage *stage);
    static ChainCallbackResult OnDrawHighPrio(Stage *stage);
    static ChainCallbackResult OnDrawLowPrio(Stage *stage);
    static ZunResult AddedCallback(Stage *stage);
    static ZunResult DeletedCallback(Stage *stage);

    ZunResult LoadStageData(char *anmpath, char *stdpath);
    ZunResult UpdateObjects();
    ZunResult RenderObjects(i32 zLevel);

    AnmVm *quadVms;
    Th07StageOriginalAnmVmSlot th07StageAnmVms[2];
    RawStageHeader *th07StdData;
    i32 th07QuadCount;
    i32 th07ObjectsCount;
    RawStageObject **th07Objects;
    RawStageObjectInstance *th07ObjectInstances;
    RawStageInstr *th07BeginningOfScript;
    StageCameraInterpolationTimer th07ScriptTime;
    i32 th07InstructionIndex;
    i32 th07FrameCounter;
    u32 th07StageRoute;
    D3DXVECTOR3 th07StagePosition;
    i32 th07StageField4d8;
    StageCameraSky th07Fog;
    StageCameraSky th07FogInterpInitial;
    StageCameraSky th07FogInterpFinal;
    i32 th07FogInterpDuration;
    StageCameraInterpolationTimer th07FogInterpTimer;
    u8 th07PendingStageFields_510_514[0x514 - 0x510];
    SpellcardState spellcardState;
    i32 ticksSinceSpellcardStarted;
    i32 th07SpellcardScreenFadePending;
    i32 th07SpellcardBackgroundVmCount;
    u8 th07PendingStageFields_524_528[0x528 - 0x524];
    AnmVm spellcardBackground;
    i8 skyFogNeedsSetup;
    u8 th07PendingStageFields_639_50f4[0x50f4 - 0x639];
    i32 th07CameraResumeInstruction;
    StageCameraBasisBlock th07CameraTargetBasis;
    StageCameraBasisBlock th07CameraInterpStartBasis;
    StageCameraBasisBlock th07CameraCurvePoint3Basis;
    StageCameraBasisBlock th07CameraCurvePoint2Basis;
    StageCameraBasisBlock th07CameraCurrentBasis;
    i32 th07CameraInterpDurations[4];
    StageCameraInterpolationTimer th07CameraInterpTimers[4];
    i32 th07CameraInterpModes[4];
    D3DXVECTOR3 th07StagePositionInterpFinal;
    i32 th07StagePositionInterpEndFrame;
    D3DXVECTOR3 th07StagePositionInterpInitial;
    i32 th07StagePositionInterpStartFrame;
    u8 th07CameraSetupPendingAfterJump;
    u8 th07PendingStageFields_52a9_52ac[0x52ac - 0x52a9];
    D3DCOLOR th07AnmManagerColorMultiplier;
    i32 th07AnmManagerColorResetGuard;
};
ZUN_ASSERT_SIZE(Stage, 0x52b4);
static_assert(offsetof(Stage, th07StageAnmVms[0]) == kTh07StageFirstAnmVmOffset);
static_assert(offsetof(Stage, th07StageAnmVms[1]) == kTh07StageSecondAnmVmOffset);
static_assert(offsetof(Stage, th07StdData) == kTh07StageStdDataOffset);
static_assert(offsetof(Stage, th07QuadCount) == kTh07StageQuadCountOffset);
static_assert(offsetof(Stage, th07ObjectsCount) == kTh07StageObjectCountOffset);
static_assert(offsetof(Stage, th07Objects) == kTh07StageObjectTableOffset);
static_assert(offsetof(Stage, th07ObjectInstances) == kTh07StageObjectInstancesOffset);
static_assert(offsetof(Stage, th07BeginningOfScript) == kTh07StageScriptOffset);
static_assert(offsetof(Stage, th07ScriptTime) == kTh07StageScriptTimerOffset);
static_assert(offsetof(Stage, th07InstructionIndex) == kTh07StageInstructionIndexOffset);
static_assert(offsetof(Stage, th07FrameCounter) == kTh07StageFrameCounterOffset);
static_assert(offsetof(Stage, th07StageRoute) == kTh07StageRouteOffset);
static_assert(offsetof(Stage, th07StagePosition) == kTh07StagePositionOffset);
static_assert(offsetof(Stage, th07StageField4d8) == kTh07StageField4d8Offset);
static_assert(offsetof(Stage, th07Fog) == kTh07StageFogNearOffset);
static_assert(offsetof(Stage, th07Fog) + offsetof(StageCameraSky, farPlane) == kTh07StageFogFarOffset);
static_assert(offsetof(Stage, th07Fog) + offsetof(StageCameraSky, color) == kTh07StageFogColorOffset);
static_assert(offsetof(Stage, th07FogInterpInitial) == kTh07StageFogInterpInitialOffset);
static_assert(offsetof(Stage, th07FogInterpFinal) == kTh07StageFogInterpFinalOffset);
static_assert(offsetof(Stage, th07FogInterpDuration) == kTh07StageFogInterpDurationOffset);
static_assert(offsetof(Stage, th07FogInterpTimer) == kTh07StageFogInterpTimerOffset);
static_assert(offsetof(Stage, spellcardState) == kTh07StageSpellcardStateOffset);
static_assert(offsetof(Stage, ticksSinceSpellcardStarted) == kTh07StageSpellcardTimerOffset);
static_assert(offsetof(Stage, th07SpellcardBackgroundVmCount) == kTh07StageSpellcardBackgroundVmCountOffset);
static_assert(offsetof(Stage, spellcardBackground) == kTh07StageSpellcardBackgroundVmOffset);
static_assert(offsetof(Stage, th07CameraResumeInstruction) == 0x50f4);
static_assert(offsetof(Stage, th07CameraTargetBasis) == kTh07StageCameraPositionTargetOffset);
static_assert(offsetof(Stage, th07CameraTargetBasis) + offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetTargetOffset);
static_assert(offsetof(Stage, th07CameraTargetBasis) + offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpTargetOffset);
static_assert(offsetof(Stage, th07CameraTargetBasis) + offsetof(StageCameraBasisBlock, fov) ==
              kTh07StageCameraFovTargetOffset);
static_assert(offsetof(Stage, th07CameraInterpStartBasis) == kTh07StageCameraPositionInterpStartOffset);
static_assert(offsetof(Stage, th07CameraInterpStartBasis) + offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetInterpStartOffset);
static_assert(offsetof(Stage, th07CameraInterpStartBasis) + offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpInterpStartOffset);
static_assert(offsetof(Stage, th07CameraInterpStartBasis) + offsetof(StageCameraBasisBlock, fov) ==
              kTh07StageCameraFovInterpStartOffset);
static_assert(offsetof(Stage, th07CameraCurvePoint3Basis) == kTh07StageCameraPositionCurvePoint3Offset);
static_assert(offsetof(Stage, th07CameraCurvePoint3Basis) + offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetCurvePoint3Offset);
static_assert(offsetof(Stage, th07CameraCurvePoint3Basis) + offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpCurvePoint3Offset);
static_assert(offsetof(Stage, th07CameraCurvePoint2Basis) == kTh07StageCameraPositionCurvePoint2Offset);
static_assert(offsetof(Stage, th07CameraCurvePoint2Basis) + offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetCurvePoint2Offset);
static_assert(offsetof(Stage, th07CameraCurvePoint2Basis) + offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpCurvePoint2Offset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) == kTh07StageCameraPositionOffset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) + offsetof(StageCameraBasisBlock, offset) ==
              kTh07StageCameraOffsetOffset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) + offsetof(StageCameraBasisBlock, up) ==
              kTh07StageCameraUpOffset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) + offsetof(StageCameraBasisBlock, forward) ==
              kTh07StageCameraForwardOffset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) + offsetof(StageCameraBasisBlock, sideVector) ==
              kTh07StageCameraSideVectorOffset);
static_assert(offsetof(Stage, th07CameraCurrentBasis) + offsetof(StageCameraBasisBlock, fov) ==
              kTh07StageCameraFovOffset);
static_assert(offsetof(Stage, th07CameraInterpDurations) == kTh07StageCameraPositionInterpDurationOffset);
static_assert(offsetof(Stage, th07CameraInterpTimers) == kTh07StageCameraPositionInterpTimerOffset);
static_assert(offsetof(Stage, th07CameraInterpModes) == kTh07StageCameraPositionInterpModeOffset);
static_assert(offsetof(Stage, th07StagePositionInterpFinal) == kTh07StagePositionInterpFinalOffset);
static_assert(offsetof(Stage, th07StagePositionInterpEndFrame) == kTh07StagePositionInterpEndFrameOffset);
static_assert(offsetof(Stage, th07StagePositionInterpInitial) == kTh07StagePositionInterpInitialOffset);
static_assert(offsetof(Stage, th07StagePositionInterpStartFrame) == kTh07StagePositionInterpStartFrameOffset);
static_assert(offsetof(Stage, th07CameraSetupPendingAfterJump) == kTh07StageCameraSetupPendingOffset);
static_assert(offsetof(Stage, th07AnmManagerColorMultiplier) == kTh07StageAnmManagerColorMultiplierOffset);
static_assert(offsetof(Stage, th07AnmManagerColorResetGuard) == kTh07StageAnmManagerColorResetGuardOffset);

void ApplyTh07StageAnmManagerColorWrite(Stage *stage, D3DCOLOR color);
void GuardTh07StageAnmManagerColorReset(Stage *stage);
DIFFABLE_EXTERN(Stage, g_Stage)
}; // namespace th07
