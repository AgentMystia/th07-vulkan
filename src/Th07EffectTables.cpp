#include "Th07EffectTables.hpp"

#include <bit>
#include <cstddef>
#include <cmath>
#include <cstring>

namespace th07 {
namespace {

std::uint16_t ReadInstructionU16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return static_cast<std::uint16_t>(bytes[offset]) |
           (static_cast<std::uint16_t>(bytes[offset + 1]) << 8);
}

std::int16_t ReadInstructionI16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    return std::bit_cast<std::int16_t>(ReadInstructionU16(bytes, offset));
}

} // namespace

const std::array<Th07EffectTableEntry, kTh07EffectTableCount> kTh07EffectTable = {{
    {0x02ab, 0, 0},
    {0x02ac, 0, 0},
    {0x02ad, 0, 0},
    {0x02ae, 0x0041a4f0, 0x0041a5a0},
    {0x02b3, 0x0041a4f0, 0x0041a370},
    {0x02b4, 0x0041a4f0, 0x0041a370},
    {0x02b5, 0x0041a4f0, 0x0041a370},
    {0x02b6, 0x0041a4f0, 0x0041a370},
    {0x02b7, 0x0041a4f0, 0x0041a370},
    {0x02b8, 0x0041a4f0, 0x0041a370},
    {0x02b9, 0x0041a4f0, 0x0041a370},
    {0x02ba, 0x0041a4f0, 0x0041a370},
    {0x02bb, 0, 0},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02dc, 0, 0},
    {0x02af, 0x0041aaf0, 0x0041aa60},
    {0x02b0, 0x0041ac30, 0x0041aa60},
    {0x02bd, 0x0041c1b0, 0},
    {0x02bf, 0x0041aef0, 0x0041b0b0},
    {0x02c3, 0, 0},
    {0x02c0, 0x0041bfd0, 0x0041bec0},
    {0x0304, 0x0041c100, 0},
    {0x02c2, 0x0041abe0, 0},
    {0x02da, 0x0041c1b0, 0},
    {0x02bf, 0x0041aef0, 0x0041b4a0},
    {0x02bf, 0x0041aef0, 0x0041b770},
    {0x02db, 0x0041c1b0, 0},
    {0x02b2, 0x0041ad10, 0x0041aa60},
    {0x02bf, 0x0041aef0, 0x0041b9f0},
    {0x02bf, 0x0041aef0, 0x0041bc20},
    {0x02c1, 0x0041bfd0, 0x0041bec0},
    {0x02b1, 0x0041aaf0, 0x0041aa60},
}};

const std::array<Th07EffectDrawListLayout, 4> kTh07EffectDrawLists = {{
    {"scroll", kTh07EffectDrawListScrollHeadOffset, kTh07EffectDrawListScrollTailOffset,
     kTh07EffectDrawListScrollSentinelOffset},
    {"indexed", kTh07EffectDrawListIndexedHeadOffset, kTh07EffectDrawListIndexedTailOffset,
     kTh07EffectDrawListIndexedSentinelOffset},
    {"no scroll", kTh07EffectDrawListNoScrollHeadOffset, kTh07EffectDrawListNoScrollTailOffset,
     kTh07EffectDrawListNoScrollSentinelOffset},
    {"scroll flagged", kTh07EffectDrawListScrollFlaggedHeadOffset, kTh07EffectDrawListScrollFlaggedTailOffset,
     kTh07EffectDrawListScrollFlaggedSentinelOffset},
}};

const std::array<Th07EffectStageAnmLoad, 9> kTh07EffectStageAnmLoads = {{
    {0, 1, 0x0041ce17, {{{0x11, "data/eff01.anm", 0x004986a4, 0x02dc, 0x0041ce21}, {0, {}, 0, 0, 0}}}},
    {1, 1, 0x0041ce17, {{{0x11, "data/eff01.anm", 0x004986a4, 0x02dc, 0x0041ce21}, {0, {}, 0, 0, 0}}}},
    {2, 1, 0x0041ce49, {{{0x11, "data/eff02.anm", 0x00498694, 0x02dc, 0x0041ce53}, {0, {}, 0, 0, 0}}}},
    {3, 1, 0x0041ce7b, {{{0x11, "data/eff03.anm", 0x00498684, 0x02dc, 0x0041ce85}, {0, {}, 0, 0, 0}}}},
    {4, 2, 0x0041cead, {{{0x11, "data/eff04.anm", 0x00498674, 0x02dc, 0x0041ceb7},
                         {0x12, "data/eff04b.anm", 0x00498664, 0x02dd, 0x0041ceda}}}},
    {5, 2, 0x0041cf02, {{{0x11, "data/eff05.anm", 0x00498654, 0x02dc, 0x0041cf0c}, {0, {}, 0, 0, 0}}}},
    {6, 2, 0x0041cf34, {{{0x11, "data/eff05.anm", 0x00498654, 0x02dc, 0x0041cf3e},
                         {0x13, "data/eff06.anm", 0x00498644, 0x02de, 0x0041cf61}}}},
    {7, 1, 0x0041cf89, {{{0x11, "data/eff02.anm", 0x00498694, 0x02dc, 0x0041cf93},
                         {0x12, "data/eff07.anm", 0x00498634, 0x02dd, 0x0041cfb3}}}},
    {8, 2, 0x0041cfd5, {{{0x11, "data/eff07.anm", 0x00498634, 0x02dc, 0x0041cfdf},
                         {0x13, "data/eff08.anm", 0x00498624, 0x02de, 0x0041cfff}}}},
}};

const std::array<Th07AnmVmRenderHelperContract, 3> kTh07AnmVmRenderHelpers = {{
    {kTh07AnmVmDrawScreenQuadFunctionAddress,
     Th07AnmVmRenderHelperKind::ScreenQuad,
     kTh07AnmVmDrawScreenQuadNoRotationFunctionAddress,
     true,
     true,
     true,
     true,
     true,
     false,
     true},
    {kTh07AnmVmDrawCameraProjectedFunctionAddress,
     Th07AnmVmRenderHelperKind::CameraProjected,
     kTh07RenderProjectedAnmQuadAddress,
     true,
     true,
     true,
     true,
     true,
     true,
     true},
    {kTh07AnmVmDrawTransformedCameraProjectedFunctionAddress,
     Th07AnmVmRenderHelperKind::TransformedCameraProjected,
     kTh07AnmVmBuildTransformedQuadFunctionAddress,
     true,
     true,
     true,
     true,
     true,
     true,
     true},
}};

const std::array<Th07AnmVmExecuteOpcodeContract, 14> kTh07AnmVmExecuteOpcodes = {{
    {-1,
     Th07AnmVmExecuteOpcodeSemantic::EndScript,
     true,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     0},
    {1,
     Th07AnmVmExecuteOpcodeSemantic::EndScript,
     true,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     0},
    {2,
     Th07AnmVmExecuteOpcodeSemantic::EndScript,
     false,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     0},
    {3,
     Th07AnmVmExecuteOpcodeSemantic::SetActiveSprite,
     false,
     false,
     true,
     false,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     kTh07AnmVmRenderFlagActive},
    {4,
     Th07AnmVmExecuteOpcodeSemantic::Jump,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     0},
    {5,
     Th07AnmVmExecuteOpcodeSemantic::Loop,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     0},
    {6,
     Th07AnmVmExecuteOpcodeSemantic::SetDrawPositionOrAux,
     false,
     false,
     false,
     false,
     false,
     true,
     true,
     false,
     false,
     false,
     false,
     false,
     0},
    {7,
     Th07AnmVmExecuteOpcodeSemantic::SetScale,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     false,
     kTh07AnmVmRenderFlagScaleDirty},
    {8,
     Th07AnmVmExecuteOpcodeSemantic::SetVisibility,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     0},
    {9,
     Th07AnmVmExecuteOpcodeSemantic::SetRgb,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     false,
     0},
    {0x0a,
     Th07AnmVmExecuteOpcodeSemantic::ToggleHorizontalScale,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     false,
     kTh07AnmVmRenderFlagScaleDirty},
    {0x0b,
     Th07AnmVmExecuteOpcodeSemantic::ToggleVerticalScale,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     false,
     false,
     false,
     kTh07AnmVmRenderFlagScaleDirty},
    {0x0c,
     Th07AnmVmExecuteOpcodeSemantic::SetRotation,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     false,
     kTh07AnmVmRenderFlagRotationDirty},
    {0x0d,
     Th07AnmVmExecuteOpcodeSemantic::SetAngularVelocity,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     false,
     true,
     kTh07AnmVmRenderFlagRotationDirty},
}};

const std::array<Th07AnmVmFollowupOpcodeContract, 22> kTh07AnmVmFollowupExecuteOpcodes = {{
    {.opcode = 0x0e,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetScaleVelocity,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmScaleVelocityXOffset,
     .secondaryFieldOffset = kTh07AnmVmScaleVelocityYOffset,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x0f,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartVisibilityInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Visibility,
     .interpolationMode = 0,
     .modeOffset = kTh07AnmVmVisibilityInterpolationModeOffset,
     .timerOffset = kTh07AnmVmVisibilityInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmVisibilityInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmVisibilityByteOffset,
     .secondaryFieldOffset = kTh07AnmVmVisibilityInterpolationInitialOffset,
     .targetFieldOffset = kTh07AnmVmVisibilityInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x10,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit4,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagBit4,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x11,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Position,
     .interpolationMode = 0,
     .modeOffset = kTh07AnmVmPositionInterpolationModeOffset,
     .timerOffset = kTh07AnmVmPositionInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmPositionInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmDrawPositionOffset,
     .secondaryFieldOffset = kTh07AnmVmAuxVectorOffset,
     .targetFieldOffset = kTh07AnmVmPositionInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x12,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Position,
     .interpolationMode = 4,
     .modeOffset = kTh07AnmVmPositionInterpolationModeOffset,
     .timerOffset = kTh07AnmVmPositionInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmPositionInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmDrawPositionOffset,
     .secondaryFieldOffset = kTh07AnmVmAuxVectorOffset,
     .targetFieldOffset = kTh07AnmVmPositionInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x13,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Position,
     .interpolationMode = 6,
     .modeOffset = kTh07AnmVmPositionInterpolationModeOffset,
     .timerOffset = kTh07AnmVmPositionInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmPositionInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmDrawPositionOffset,
     .secondaryFieldOffset = kTh07AnmVmAuxVectorOffset,
     .targetFieldOffset = kTh07AnmVmPositionInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x14,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StopUntilInterrupt,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmPendingInterruptOffset,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = kTh07AnmVmRenderFlagInterruptPending,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = true},
    {.opcode = 0x16,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetAnchorTopLeft,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = kTh07AnmVmRenderFlagAnchorTopLeft,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x17,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StopAndClearActiveUntilInterrupt,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmPendingInterruptOffset,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = kTh07AnmVmRenderFlagInterruptPending,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = true,
     .stopsUntilInterrupt = true},
    {.opcode = 0x18,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetPositionTargetsAux,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagPositionTargetsAux,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x19,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetField1c4,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmField1c4Offset,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1a,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::AddAngleField28,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmAngleField28Offset,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1b,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::AddAngleField2c,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = kTh07AnmVmAngleField2cOffset,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1c,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetActiveRenderFlag,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagActive,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1d,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartScaleInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Scale,
     .interpolationMode = 0,
     .modeOffset = kTh07AnmVmScaleInterpolationModeOffset,
     .timerOffset = kTh07AnmVmScaleInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmScaleInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmScaleXOffset,
     .secondaryFieldOffset = kTh07AnmVmScaleInterpolationInitialXOffset,
     .targetFieldOffset = kTh07AnmVmScaleInterpolationFinalXOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1e,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit12,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagBit12,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x1f,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit14,
     .interpolationLane = Th07AnmVmInterpolationLane::None,
     .interpolationMode = 0,
     .modeOffset = 0,
     .timerOffset = 0,
     .durationOffset = 0,
     .primaryFieldOffset = 0,
     .secondaryFieldOffset = 0,
     .targetFieldOffset = 0,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagBit14,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x20,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Position,
     .interpolationMode = kTh07AnmVmInterpolationModeFromInstruction,
     .modeOffset = kTh07AnmVmPositionInterpolationModeOffset,
     .timerOffset = kTh07AnmVmPositionInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmPositionInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmDrawPositionOffset,
     .secondaryFieldOffset = kTh07AnmVmPositionInterpolationInitialOffset,
     .targetFieldOffset = kTh07AnmVmPositionInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = kTh07AnmVmRenderFlagPositionTargetsAux,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x21,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartColorInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Color,
     .interpolationMode = kTh07AnmVmInterpolationModeFromInstruction,
     .modeOffset = kTh07AnmVmColorInterpolationModeOffset,
     .timerOffset = kTh07AnmVmColorInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmColorInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmColorOffset,
     .secondaryFieldOffset = kTh07AnmVmColorInterpolationInitialOffset,
     .targetFieldOffset = kTh07AnmVmColorInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x22,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartVisibilityInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Visibility,
     .interpolationMode = kTh07AnmVmInterpolationModeFromInstruction,
     .modeOffset = kTh07AnmVmVisibilityInterpolationModeOffset,
     .timerOffset = kTh07AnmVmVisibilityInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmVisibilityInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmVisibilityByteOffset,
     .secondaryFieldOffset = kTh07AnmVmVisibilityInterpolationInitialOffset,
     .targetFieldOffset = kTh07AnmVmVisibilityInterpolationTargetOffset,
     .setRenderFlags = 0,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x23,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartRotationInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Rotation,
     .interpolationMode = kTh07AnmVmInterpolationModeFromInstruction,
     .modeOffset = kTh07AnmVmRotationInterpolationModeOffset,
     .timerOffset = kTh07AnmVmRotationInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmRotationInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmRotationOffset,
     .secondaryFieldOffset = kTh07AnmVmRotationInterpolationInitialOffset,
     .targetFieldOffset = kTh07AnmVmRotationInterpolationTargetOffset,
     .setRenderFlags = kTh07AnmVmRenderFlagRotationDirty,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
    {.opcode = 0x24,
     .semantic = Th07AnmVmFollowupOpcodeSemantic::StartScaleInterpolation,
     .interpolationLane = Th07AnmVmInterpolationLane::Scale,
     .interpolationMode = kTh07AnmVmInterpolationModeFromInstruction,
     .modeOffset = kTh07AnmVmScaleInterpolationModeOffset,
     .timerOffset = kTh07AnmVmScaleInterpolationTimerOffset,
     .durationOffset = kTh07AnmVmScaleInterpolationDurationOffset,
     .primaryFieldOffset = kTh07AnmVmScaleXOffset,
     .secondaryFieldOffset = kTh07AnmVmScaleInterpolationInitialXOffset,
     .targetFieldOffset = kTh07AnmVmScaleInterpolationFinalXOffset,
     .setRenderFlags = kTh07AnmVmRenderFlagScaleDirty,
     .controlledRenderFlag = 0,
     .clearsActiveFlag = false,
     .stopsUntilInterrupt = false},
}};

const std::array<Th07AnmVmVariableSlot, kTh07AnmVmVariableSlotCount> kTh07AnmVmVariableSlots = {{
    {kTh07AnmVmIntVariable0Id, kTh07AnmVmIntVariable0Offset, Th07AnmVmVariableValueKind::Int},
    {kTh07AnmVmIntVariable1Id, kTh07AnmVmIntVariable1Offset, Th07AnmVmVariableValueKind::Int},
    {kTh07AnmVmIntVariable2Id, kTh07AnmVmIntVariable2Offset, Th07AnmVmVariableValueKind::Int},
    {kTh07AnmVmIntVariable3Id, kTh07AnmVmIntVariable3Offset, Th07AnmVmVariableValueKind::Int},
    {kTh07AnmVmFloatVariable0Id, kTh07AnmVmFloatVariable0Offset, Th07AnmVmVariableValueKind::Float},
    {kTh07AnmVmFloatVariable1Id, kTh07AnmVmFloatVariable1Offset, Th07AnmVmVariableValueKind::Float},
    {kTh07AnmVmFloatVariable2Id, kTh07AnmVmFloatVariable2Offset, Th07AnmVmVariableValueKind::Float},
    {kTh07AnmVmFloatVariable3Id, kTh07AnmVmFloatVariable3Offset, Th07AnmVmVariableValueKind::Float},
    {kTh07AnmVmIntVariable4Id, kTh07AnmVmIntVariable4Offset, Th07AnmVmVariableValueKind::Int},
    {kTh07AnmVmIntVariable5Id, kTh07AnmVmIntVariable5Offset, Th07AnmVmVariableValueKind::Int},
}};

const std::array<Th07AnmVmVariableOpcodeContract, 30> kTh07AnmVmVariableExecuteOpcodes = {{
    {0x25, Th07AnmVmVariableOpcodeSemantic::Assign, Th07AnmVmVariableValueKind::Int, 1, true, false,
     false, true, false, false, 0},
    {0x26, Th07AnmVmVariableOpcodeSemantic::Assign, Th07AnmVmVariableValueKind::Float, 1, false, true,
     false, false, true, false, 0},
    {0x27, Th07AnmVmVariableOpcodeSemantic::AddAssign, Th07AnmVmVariableValueKind::Int, 1, true, false,
     true, true, false, false, 0},
    {0x28, Th07AnmVmVariableOpcodeSemantic::AddAssign, Th07AnmVmVariableValueKind::Float, 1, false, true,
     true, false, true, false, 0},
    {0x29, Th07AnmVmVariableOpcodeSemantic::SubtractAssign, Th07AnmVmVariableValueKind::Int, 1, true,
     false, true, true, false, false, 0},
    {0x2a, Th07AnmVmVariableOpcodeSemantic::SubtractAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, true, false, true, false, 0},
    {0x2b, Th07AnmVmVariableOpcodeSemantic::MultiplyAssign, Th07AnmVmVariableValueKind::Int, 1, true,
     false, true, true, false, false, 0},
    {0x2c, Th07AnmVmVariableOpcodeSemantic::MultiplyAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, true, false, true, false, 0},
    {0x2d, Th07AnmVmVariableOpcodeSemantic::DivideAssign, Th07AnmVmVariableValueKind::Int, 1, true, false,
     true, true, false, false, 0},
    {0x2e, Th07AnmVmVariableOpcodeSemantic::DivideAssign, Th07AnmVmVariableValueKind::Float, 1, false, true,
     true, false, true, false, 0},
    {0x2f, Th07AnmVmVariableOpcodeSemantic::ModuloAssign, Th07AnmVmVariableValueKind::Int, 1, true, false,
     true, true, false, false, 0},
    {0x30, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048bb0aAddress},
    {0x31, Th07AnmVmVariableOpcodeSemantic::AddOperands, Th07AnmVmVariableValueKind::Int, 2, true, false,
     false, true, false, false, 0},
    {0x32, Th07AnmVmVariableOpcodeSemantic::AddOperands, Th07AnmVmVariableValueKind::Float, 2, false, true,
     false, false, true, false, 0},
    {0x33, Th07AnmVmVariableOpcodeSemantic::SubtractOperands, Th07AnmVmVariableValueKind::Int, 2, true,
     false, false, true, false, false, 0},
    {0x34, Th07AnmVmVariableOpcodeSemantic::SubtractOperands, Th07AnmVmVariableValueKind::Float, 2, false,
     true, false, false, true, false, 0},
    {0x35, Th07AnmVmVariableOpcodeSemantic::MultiplyOperands, Th07AnmVmVariableValueKind::Int, 2, true,
     false, false, true, false, false, 0},
    {0x36, Th07AnmVmVariableOpcodeSemantic::MultiplyOperands, Th07AnmVmVariableValueKind::Float, 2, false,
     true, false, false, true, false, 0},
    {0x37, Th07AnmVmVariableOpcodeSemantic::DivideOperands, Th07AnmVmVariableValueKind::Int, 2, true, false,
     false, true, false, false, 0},
    {0x38, Th07AnmVmVariableOpcodeSemantic::DivideOperands, Th07AnmVmVariableValueKind::Float, 2, false,
     true, false, false, true, false, 0},
    {0x39, Th07AnmVmVariableOpcodeSemantic::ModuloOperands, Th07AnmVmVariableValueKind::Int, 2, true, false,
     false, true, false, false, 0},
    {0x3a, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 2, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048bb0aAddress},
    {0x3b, Th07AnmVmVariableOpcodeSemantic::RandomIntModulo, Th07AnmVmVariableValueKind::Int, 1, true, false,
     false, true, false, true, kTh07AnmVmRandomU32FunctionAddress},
    {0x3c, Th07AnmVmVariableOpcodeSemantic::RandomFloatScaled, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmRandomFloatZeroToOneFunctionAddress},
    {0x3d, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048bb40Address},
    {0x3e, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048bbf0Address},
    {0x3f, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048b920Address},
    {0x40, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0047eca0Address},
    {0x41, Th07AnmVmVariableOpcodeSemantic::MathHelperAssign, Th07AnmVmVariableValueKind::Float, 1, false,
     true, false, false, true, false, kTh07AnmVmMathHelper0048ba20Address},
    {0x42, Th07AnmVmVariableOpcodeSemantic::NormalizeAngle, Th07AnmVmVariableValueKind::Float, 1, false, true,
     false, false, true, false, kTh07AnmVmNormalizeAngleFunctionAddress},
}};

const std::array<Th07AnmVmConditionalJumpOpcodeContract, 12> kTh07AnmVmConditionalJumpExecuteOpcodes = {{
    {0x43, Th07AnmVmConditionalJumpComparison::Equal, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x44, Th07AnmVmConditionalJumpComparison::Equal, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
    {0x45, Th07AnmVmConditionalJumpComparison::NotEqual, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x46, Th07AnmVmConditionalJumpComparison::NotEqual, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
    {0x47, Th07AnmVmConditionalJumpComparison::LessThan, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x48, Th07AnmVmConditionalJumpComparison::LessThan, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
    {0x49, Th07AnmVmConditionalJumpComparison::LessOrEqual, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x4a, Th07AnmVmConditionalJumpComparison::LessOrEqual, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
    {0x4b, Th07AnmVmConditionalJumpComparison::GreaterThan, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x4c, Th07AnmVmConditionalJumpComparison::GreaterThan, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
    {0x4d, Th07AnmVmConditionalJumpComparison::GreaterOrEqual, Th07AnmVmVariableValueKind::Int, 2,
     true, false, 2, 3, true, true},
    {0x4e, Th07AnmVmConditionalJumpComparison::GreaterOrEqual, Th07AnmVmVariableValueKind::Float, 2,
     false, true, 2, 3, true, true},
}};

const std::array<Th07AnmVmControlOpcodeContract, 3> kTh07AnmVmControlExecuteOpcodes = {{
    {0x4f,
     Th07AnmVmControlOpcodeSemantic::WaitTimer,
     Th07AnmVmVariableValueKind::Int,
     1,
     kTh07AnmVmWaitTimerCurrentOffset,
     0,
     kTh07AnmVmWaitTimerPreviousOffset,
     kTh07AnmVmWaitTimerSubframeOffset,
     kTh07AnmVmWaitTimerCurrentOffset,
     true,
     false,
     true,
     true,
     false},
    {0x50,
     Th07AnmVmControlOpcodeSemantic::SetAngleField28Velocity,
     Th07AnmVmVariableValueKind::Float,
     1,
     kTh07AnmVmAngleField28VelocityOffset,
     kTh07AnmVmAngleField28Offset,
     0,
     0,
     0,
     false,
     true,
     false,
     false,
     true},
    {0x51,
     Th07AnmVmControlOpcodeSemantic::SetAngleField2cVelocity,
     Th07AnmVmVariableValueKind::Float,
     1,
     kTh07AnmVmAngleField2cVelocityOffset,
     kTh07AnmVmAngleField2cOffset,
     0,
     0,
     0,
     false,
     true,
     false,
     false,
     true},
}};

const std::array<Th07AnmVmPostStepActionContract, 13> kTh07AnmVmPostStepActions = {{
    {Th07AnmVmPostStepActionSemantic::ApplyAngularVelocity,
     Th07AnmVmInterpolationLane::Rotation,
     kTh07AnmVmRotationXOffset,
     0,
     0,
     kTh07AnmVmAngularVelocityXOffset,
     0,
     0,
     0,
     kTh07AnmVmRenderFlagRotationDirty,
     0,
     kTh07AnmVmNormalizeAngleFunctionAddress,
     true,
     true,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyAngularVelocity,
     Th07AnmVmInterpolationLane::Rotation,
     kTh07AnmVmRotationYOffset,
     0,
     0,
     kTh07AnmVmAngularVelocityYOffset,
     0,
     0,
     0,
     kTh07AnmVmRenderFlagRotationDirty,
     0,
     kTh07AnmVmNormalizeAngleFunctionAddress,
     true,
     true,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyAngularVelocity,
     Th07AnmVmInterpolationLane::Rotation,
     kTh07AnmVmRotationZOffset,
     0,
     0,
     kTh07AnmVmAngularVelocityZOffset,
     0,
     0,
     0,
     kTh07AnmVmRenderFlagRotationDirty,
     0,
     kTh07AnmVmNormalizeAngleFunctionAddress,
     true,
     true,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
     Th07AnmVmInterpolationLane::Position,
     kTh07AnmVmDrawPositionOffset,
     kTh07AnmVmPositionInterpolationInitialOffset,
     kTh07AnmVmPositionInterpolationTargetOffset,
     0,
     kTh07AnmVmPositionInterpolationTimerOffset,
     kTh07AnmVmPositionInterpolationDurationOffset,
     kTh07AnmVmPositionInterpolationModeOffset,
     0,
     kTh07AnmVmTickTimerFunctionAddress,
     0,
     false,
     false,
     true,
     true,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
     Th07AnmVmInterpolationLane::Color,
     kTh07AnmVmColorOffset,
     kTh07AnmVmColorInterpolationInitialOffset,
     kTh07AnmVmColorInterpolationTargetOffset,
     0,
     kTh07AnmVmColorInterpolationTimerOffset,
     kTh07AnmVmColorInterpolationDurationOffset,
     kTh07AnmVmColorInterpolationModeOffset,
     0,
     kTh07AnmVmTickTimerFunctionAddress,
     kTh07AnmVmRoundFloatToByteFunctionAddress,
     false,
     false,
     false,
     true,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
     Th07AnmVmInterpolationLane::Visibility,
     kTh07AnmVmVisibilityByteOffset,
     kTh07AnmVmVisibilityInterpolationInitialOffset,
     kTh07AnmVmVisibilityInterpolationTargetOffset,
     0,
     kTh07AnmVmVisibilityInterpolationTimerOffset,
     kTh07AnmVmVisibilityInterpolationDurationOffset,
     kTh07AnmVmVisibilityInterpolationModeOffset,
     0,
     kTh07AnmVmTickTimerFunctionAddress,
     kTh07AnmVmRoundFloatToByteFunctionAddress,
     false,
     false,
     false,
     true,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
     Th07AnmVmInterpolationLane::Rotation,
     kTh07AnmVmRotationOffset,
     kTh07AnmVmRotationInterpolationInitialOffset,
     kTh07AnmVmRotationInterpolationTargetOffset,
     0,
     kTh07AnmVmRotationInterpolationTimerOffset,
     kTh07AnmVmRotationInterpolationDurationOffset,
     kTh07AnmVmRotationInterpolationModeOffset,
     kTh07AnmVmRenderFlagRotationDirty,
     kTh07AnmVmTickTimerFunctionAddress,
     kTh07AnmVmNormalizeAngleFunctionAddress,
     false,
     true,
     false,
     true,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane,
     Th07AnmVmInterpolationLane::Scale,
     kTh07AnmVmScaleXOffset,
     kTh07AnmVmScaleInterpolationInitialXOffset,
     kTh07AnmVmScaleInterpolationFinalXOffset,
     0,
     kTh07AnmVmScaleInterpolationTimerOffset,
     kTh07AnmVmScaleInterpolationDurationOffset,
     kTh07AnmVmScaleInterpolationModeOffset,
     kTh07AnmVmRenderFlagScaleDirty,
     kTh07AnmVmTickTimerFunctionAddress,
     0,
     false,
     false,
     false,
     true,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyScaleVelocity,
     Th07AnmVmInterpolationLane::Scale,
     kTh07AnmVmScaleYOffset,
     0,
     0,
     kTh07AnmVmScaleVelocityYOffset,
     0,
     0,
     0,
     kTh07AnmVmRenderFlagScaleDirty,
     0,
     0,
     true,
     false,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyScaleVelocity,
     Th07AnmVmInterpolationLane::Scale,
     kTh07AnmVmScaleXOffset,
     0,
     0,
     kTh07AnmVmScaleVelocityXOffset,
     0,
     0,
     0,
     kTh07AnmVmRenderFlagScaleDirty | kTh07AnmVmRenderFlagRotationDirty,
     0,
     0,
     true,
     false,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyAngleFieldVelocity,
     Th07AnmVmInterpolationLane::None,
     kTh07AnmVmAngleField28Offset,
     0,
     0,
     kTh07AnmVmAngleField28VelocityOffset,
     0,
     0,
     0,
     0,
     0,
     0,
     false,
     true,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::ApplyAngleFieldVelocity,
     Th07AnmVmInterpolationLane::None,
     kTh07AnmVmAngleField2cOffset,
     0,
     0,
     kTh07AnmVmAngleField2cVelocityOffset,
     0,
     0,
     0,
     0,
     0,
     0,
     false,
     true,
     false,
     false,
     false},
    {Th07AnmVmPostStepActionSemantic::AdvanceScriptTimer,
     Th07AnmVmInterpolationLane::None,
     kTh07AnmVmScriptTimerCurrentOffset,
     0,
     0,
     0,
     kTh07AnmVmScriptTimerPreviousOffset,
     0,
     0,
     0,
     kTh07AnmVmTickTimerFunctionAddress,
     0,
     false,
     false,
     false,
     false,
     true},
}};

const std::array<Th07StageCameraVectorChannelLayout, 3> kTh07StageCameraVectorChannels = {{
    {Th07StageCameraChannel::Position,
     kTh07StageCameraPositionOffset,
     kTh07StageCameraPositionTargetOffset,
     kTh07StageCameraPositionInterpStartOffset,
     kTh07StageCameraPositionCurvePoint2Offset,
     kTh07StageCameraPositionCurvePoint3Offset,
     kTh07StageCameraPositionInterpDurationOffset,
     kTh07StageCameraPositionInterpTimerOffset,
     kTh07StageCameraPositionInterpModeOffset},
    {Th07StageCameraChannel::Offset,
     kTh07StageCameraOffsetOffset,
     kTh07StageCameraOffsetTargetOffset,
     kTh07StageCameraOffsetInterpStartOffset,
     kTh07StageCameraOffsetCurvePoint2Offset,
     kTh07StageCameraOffsetCurvePoint3Offset,
     kTh07StageCameraOffsetInterpDurationOffset,
     kTh07StageCameraOffsetInterpTimerOffset,
     kTh07StageCameraOffsetInterpModeOffset},
    {Th07StageCameraChannel::Up,
     kTh07StageCameraUpOffset,
     kTh07StageCameraUpTargetOffset,
     kTh07StageCameraUpInterpStartOffset,
     kTh07StageCameraUpCurvePoint2Offset,
     kTh07StageCameraUpCurvePoint3Offset,
     kTh07StageCameraUpInterpDurationOffset,
     kTh07StageCameraUpInterpTimerOffset,
     kTh07StageCameraUpInterpModeOffset},
}};

const Th07StageCameraScalarChannelLayout kTh07StageCameraFovChannel = {
    Th07StageCameraChannel::Fov,
    kTh07StageCameraFovOffset,
    kTh07StageCameraFovTargetOffset,
    kTh07StageCameraFovInterpStartOffset,
    kTh07StageCameraFovInterpDurationOffset,
    kTh07StageCameraFovInterpTimerOffset,
    kTh07StageCameraFovInterpModeOffset,
};

const std::array<Th07StageCameraOpcodeLayout, 23> kTh07StageCameraOpcodes = {{
    {0x05, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::SetLinearTarget,
     kTh07StageCameraPositionTargetOffset},
    {0x06, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::StartLinearInterpolation,
     kTh07StageCameraPositionInterpDurationOffset},
    {0x07, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::SetLinearTarget,
     kTh07StageCameraOffsetTargetOffset},
    {0x08, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::StartLinearInterpolation,
     kTh07StageCameraOffsetInterpDurationOffset},
    {0x09, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::SetLinearTarget,
     kTh07StageCameraUpTargetOffset},
    {0x0a, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::StartLinearInterpolation,
     kTh07StageCameraUpInterpDurationOffset},
    {0x0b, Th07StageCameraChannel::Fov, Th07StageCameraOpcodeSemantic::SetLinearTarget,
     kTh07StageCameraFovTargetOffset},
    {0x0c, Th07StageCameraChannel::Fov, Th07StageCameraOpcodeSemantic::StartLinearInterpolation,
     kTh07StageCameraFovInterpDurationOffset},
    {0x0e, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::SetCurvePoint0,
     kTh07StageCameraPositionInterpStartOffset},
    {0x0f, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::SetCurvePoint1,
     kTh07StageCameraPositionTargetOffset},
    {0x10, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::SetCurvePoint2,
     kTh07StageCameraPositionCurvePoint2Offset},
    {0x11, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::SetCurvePoint3,
     kTh07StageCameraPositionCurvePoint3Offset},
    {0x12, Th07StageCameraChannel::Position, Th07StageCameraOpcodeSemantic::StartCurveInterpolation,
     kTh07StageCameraPositionInterpDurationOffset},
    {0x13, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::SetCurvePoint0,
     kTh07StageCameraOffsetInterpStartOffset},
    {0x14, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::SetCurvePoint1,
     kTh07StageCameraOffsetTargetOffset},
    {0x15, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::SetCurvePoint2,
     kTh07StageCameraOffsetCurvePoint2Offset},
    {0x16, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::SetCurvePoint3,
     kTh07StageCameraOffsetCurvePoint3Offset},
    {0x17, Th07StageCameraChannel::Offset, Th07StageCameraOpcodeSemantic::StartCurveInterpolation,
     kTh07StageCameraOffsetInterpDurationOffset},
    {0x18, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::SetCurvePoint0,
     kTh07StageCameraUpInterpStartOffset},
    {0x19, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::SetCurvePoint1,
     kTh07StageCameraUpTargetOffset},
    {0x1a, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::SetCurvePoint2,
     kTh07StageCameraUpCurvePoint2Offset},
    {0x1b, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::SetCurvePoint3,
     kTh07StageCameraUpCurvePoint3Offset},
    {0x1c, Th07StageCameraChannel::Up, Th07StageCameraOpcodeSemantic::StartCurveInterpolation,
     kTh07StageCameraUpInterpDurationOffset},
}};

const Th07EffectTableEntry *FindTh07EffectTableEntry(std::int32_t effectId)
{
    if (effectId < 0 || effectId >= static_cast<std::int32_t>(kTh07EffectTable.size())) {
        return nullptr;
    }

    return &kTh07EffectTable[static_cast<std::size_t>(effectId)];
}

const Th07EffectStageAnmLoad *FindTh07EffectStageAnmLoad(std::int32_t currentStageValue)
{
    for (const Th07EffectStageAnmLoad &load : kTh07EffectStageAnmLoads) {
        if (load.currentStageValue == currentStageValue) {
            return &load;
        }
    }
    return nullptr;
}

const Th07StageCameraVectorChannelLayout *FindTh07StageCameraVectorChannel(Th07StageCameraChannel channel)
{
    for (const Th07StageCameraVectorChannelLayout &layout : kTh07StageCameraVectorChannels) {
        if (layout.channel == channel) {
            return &layout;
        }
    }
    return nullptr;
}

const Th07StageCameraOpcodeLayout *FindTh07StageCameraOpcode(std::int32_t opcode)
{
    for (const Th07StageCameraOpcodeLayout &layout : kTh07StageCameraOpcodes) {
        if (layout.opcode == opcode) {
            return &layout;
        }
    }
    return nullptr;
}

bool ReadTh07AnmVmInstructionHeader(std::span<const std::uint8_t> bytes,
                                    Th07AnmVmInstructionHeader *outHeader)
{
    if (outHeader == nullptr || bytes.size() < kTh07AnmVmInstructionHeaderSize) {
        return false;
    }

    *outHeader = {
        ReadInstructionI16(bytes, kTh07AnmVmInstructionOpcodeOffset),
        ReadInstructionU16(bytes, kTh07AnmVmInstructionSizeOffset),
        ReadInstructionI16(bytes, kTh07AnmVmInstructionTimeOffset),
        ReadInstructionU16(bytes, kTh07AnmVmInstructionOperandFlagsOffset),
    };
    return outHeader->size >= kTh07AnmVmInstructionHeaderSize;
}

std::size_t Th07AnmVmInstructionNextOffset(Th07AnmVmInstructionHeader header)
{
    return header.size;
}

std::size_t Th07AnmVmInstructionOperandOffset(std::size_t operandIndex)
{
    return kTh07AnmVmInstructionOperandsOffset + operandIndex * sizeof(std::uint32_t);
}

std::int32_t TruncateTh07AnmVmFloatToI32(float value)
{
    return value < 0.0f ? static_cast<std::int32_t>(std::ceil(value)) :
                          static_cast<std::int32_t>(std::floor(value));
}

const Th07AnmVmVariableSlot *FindTh07AnmVmVariableSlot(std::int32_t variableId)
{
    for (const Th07AnmVmVariableSlot &slot : kTh07AnmVmVariableSlots) {
        if (slot.variableId == variableId) {
            return &slot;
        }
    }

    return nullptr;
}

bool ReadTh07AnmVmInstructionI32Operand(std::span<const std::uint8_t> bytes,
                                        std::size_t operandIndex,
                                        std::int32_t *outValue)
{
    const std::size_t offset = Th07AnmVmInstructionOperandOffset(operandIndex);
    if (outValue == nullptr || offset > bytes.size() || sizeof(std::uint32_t) > bytes.size() - offset) {
        return false;
    }

    const std::uint32_t raw = static_cast<std::uint32_t>(bytes[offset]) |
                              (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
                              (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
                              (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
    *outValue = std::bit_cast<std::int32_t>(raw);
    return true;
}

bool ReadTh07AnmVmInstructionF32Operand(std::span<const std::uint8_t> bytes,
                                        std::size_t operandIndex,
                                        float *outValue)
{
    std::int32_t raw = 0;
    if (outValue == nullptr || !ReadTh07AnmVmInstructionI32Operand(bytes, operandIndex, &raw)) {
        return false;
    }

    *outValue = std::bit_cast<float>(raw);
    return true;
}

std::uint32_t Th07EffectSlotOffset(std::int32_t slotIndex)
{
    if (slotIndex < 0) {
        return kTh07EffectFallbackSlotOffset;
    }
    return kTh07EffectArrayOffset + static_cast<std::uint32_t>(slotIndex) * kTh07EffectSlotSize;
}

std::uint32_t Th07EffectDedicatedSlotOffset(std::int32_t dedicatedSlotArgument)
{
    return Th07EffectSlotOffset(kTh07EffectDedicatedSlotBaseIndex + dedicatedSlotArgument);
}

Th07EffectSpawnContract BuildTh07DedicatedEffectSpawnContract(std::int32_t effectId,
                                                              std::int32_t dedicatedSlotArgument,
                                                              std::uint32_t color)
{
    const Th07EffectTableEntry *entry = FindTh07EffectTableEntry(effectId);
    if (entry == nullptr || dedicatedSlotArgument < 0 ||
        dedicatedSlotArgument >= kTh07EffectDedicatedSlotCount) {
        return {};
    }

    const std::int32_t slotIndex = kTh07EffectDedicatedSlotBaseIndex + dedicatedSlotArgument;
    return {
        true,
        effectId,
        slotIndex,
        Th07EffectSlotOffset(slotIndex),
        color,
        entry->anmScript,
        entry->updateCallbackAddress,
        entry->initCallbackAddress,
        kTh07EffectSlotInitialRenderFlagBits,
        kTh07EffectSlotInitialTimerPreviousFrame,
        0,
        0,
    };
}

namespace {

float LengthSquared(Th07EffectVector3 value)
{
    return value.x * value.x + value.y * value.y + value.z * value.z;
}

Th07EffectVector3 NormalizeOrZero(Th07EffectVector3 value)
{
    const float lengthSquared = LengthSquared(value);
    if (lengthSquared <= 0.0f) {
        return {};
    }
    const float inverseLength = 1.0f / std::sqrt(lengthSquared);
    return {
        value.x * inverseLength,
        value.y * inverseLength,
        value.z * inverseLength,
    };
}

Th07EffectVector3 Add(Th07EffectVector3 lhs, Th07EffectVector3 rhs)
{
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z,
    };
}

Th07EffectVector3 Scale(Th07EffectVector3 value, float scalar)
{
    return {
        value.x * scalar,
        value.y * scalar,
        value.z * scalar,
    };
}

Th07EffectVector3 Cross(Th07EffectVector3 lhs, Th07EffectVector3 rhs)
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

float FloatFromBits(std::uint32_t bits)
{
    float value = 0.0f;
    static_assert(sizeof(value) == sizeof(bits));
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

float FloatFromSignedBits(std::int32_t bits)
{
    return FloatFromBits(static_cast<std::uint32_t>(bits));
}

Th07EffectVector3 CameraOffsetPosition(Th07EffectPerspectiveCameraBasis cameraBasis,
                                       float offsetScale)
{
    return Add(cameraBasis.position, Scale(cameraBasis.offset, offsetScale));
}

float RandomSpan(float randomZeroToOne, float span, float center)
{
    return randomZeroToOne * span - center;
}

Th07EffectVector3 StageScriptVectorArgs(Th07StageScriptInstruction instruction)
{
    return {
        FloatFromSignedBits(instruction.args[0]),
        FloatFromSignedBits(instruction.args[1]),
        FloatFromSignedBits(instruction.args[2]),
    };
}

std::uint32_t InterpolateColorByte(std::uint32_t initial,
                                   std::uint32_t final,
                                   std::uint32_t shift,
                                   float ratio)
{
    const std::uint32_t initialComponent = (initial >> shift) & 0xff;
    const std::uint32_t finalComponent = (final >> shift) & 0xff;
    const std::uint32_t interpolated =
        static_cast<std::uint32_t>((static_cast<float>(finalComponent) -
                                    static_cast<float>(initialComponent)) *
                                       ratio +
                                   static_cast<float>(initialComponent));
    return (interpolated & 0xff) << shift;
}

std::uint32_t BuildInterpolatedColor(std::uint32_t initial,
                                     std::uint32_t final,
                                     float ratio)
{
    return InterpolateColorByte(initial, final, 0, ratio) |
           InterpolateColorByte(initial, final, 8, ratio) |
           InterpolateColorByte(initial, final, 16, ratio) |
           InterpolateColorByte(initial, final, 24, ratio);
}

Th07EffectLinearMotion BuildRandomSplashMotion(float randomXZeroToOne,
                                               float randomYZeroToOne,
                                               float framerateMultiplier,
                                               float velocityScale,
                                               float accelerationFrames)
{
    const float velocityX =
        (randomXZeroToOne * kTh07EffectRandomSplashSpan - kTh07EffectRandomSplashCenter) * velocityScale;
    const float velocityY =
        (randomYZeroToOne * kTh07EffectRandomSplashSpan - kTh07EffectRandomSplashCenter) * velocityScale;

    return {
        {velocityX * framerateMultiplier, velocityY * framerateMultiplier, 0.0f},
        {-velocityX / accelerationFrames * framerateMultiplier,
         -velocityY / accelerationFrames * framerateMultiplier,
         0.0f},
    };
}

std::size_t StageCameraChannelIndex(Th07StageCameraChannel channel)
{
    switch (channel) {
    case Th07StageCameraChannel::Position:
        return 0;
    case Th07StageCameraChannel::Offset:
        return 1;
    case Th07StageCameraChannel::Up:
        return 2;
    case Th07StageCameraChannel::Fov:
        return 3;
    }
    return 0;
}

Th07EffectVector3 *StageCameraVectorForChannel(Th07StageCameraState *state,
                                               Th07StageCameraChannel channel)
{
    switch (channel) {
    case Th07StageCameraChannel::Position:
        return &state->position;
    case Th07StageCameraChannel::Offset:
        return &state->offset;
    case Th07StageCameraChannel::Up:
        return &state->up;
    case Th07StageCameraChannel::Fov:
        return nullptr;
    }
    return nullptr;
}

float *StageCameraScalarForChannel(Th07StageCameraState *state,
                                   Th07StageCameraChannel channel)
{
    if (channel == Th07StageCameraChannel::Fov) {
        return &state->fov;
    }
    return nullptr;
}

void ResetStageCameraInterpolationTimer(Th07StageCameraRuntimeState *state,
                                        Th07StageCameraChannel channel,
                                        std::int32_t interpolationMode)
{
    const std::size_t index = StageCameraChannelIndex(channel);
    state->interpolationTimers[index] = {kTh07StageCameraInitialTimerPreviousFrame, 0.0f, 0};
    state->interpolationModes[index] = interpolationMode;
}

void TickStageCameraInterpolationTimer(Th07StageCameraInterpolationTimerState *timer,
                                       float framerateMultiplier,
                                       float effectiveFramerateMultiplier)
{
    timer->previousFrame = timer->currentFrame;
    if (framerateMultiplier <= 0.99f) {
        timer->subframe += effectiveFramerateMultiplier;
        if (timer->subframe >= 1.0f) {
            timer->currentFrame += 1;
            timer->subframe -= 1.0f;
        }
    } else {
        timer->currentFrame += 1;
    }
}

float AdvanceStageCameraInterpolationTimer(Th07StageCameraRuntimeState *state,
                                           Th07StageCameraChannel channel,
                                           float framerateMultiplier,
                                           float effectiveFramerateMultiplier)
{
    const std::size_t index = StageCameraChannelIndex(channel);
    std::int32_t &duration = state->interpolationDurations[index];
    Th07StageCameraInterpolationTimerState &timer = state->interpolationTimers[index];
    if (duration <= 0) {
        if (duration < 0) {
            timer.currentFrame = duration;
            timer.subframe = 0.0f;
            timer.previousFrame = kTh07StageCameraInitialTimerPreviousFrame;
            duration = 0;
        }
        return 1.0f;
    }

    if (timer.currentFrame < duration) {
        TickStageCameraInterpolationTimer(&timer, framerateMultiplier, effectiveFramerateMultiplier);
        return (static_cast<float>(timer.currentFrame) + timer.subframe) / static_cast<float>(duration);
    }

    timer.currentFrame = duration;
    timer.subframe = 0.0f;
    timer.previousFrame = kTh07StageCameraInitialTimerPreviousFrame;
    duration = 0;
    return 1.0f;
}

void ApplyLinearStageCameraTarget(Th07StageCameraRuntimeState *state,
                                  Th07StageCameraChannel channel,
                                  Th07EffectVector3 value)
{
    Th07EffectVector3 *start = StageCameraVectorForChannel(&state->interpolationStart, channel);
    Th07EffectVector3 *target = StageCameraVectorForChannel(&state->target, channel);
    Th07EffectVector3 *current = StageCameraVectorForChannel(&state->current, channel);
    if (start == nullptr || target == nullptr || current == nullptr) {
        return;
    }

    *start = *target;
    *target = value;
    if (state->interpolationDurations[StageCameraChannelIndex(channel)] == 0) {
        *current = value;
    }
}

void ApplyLinearStageCameraScalarTarget(Th07StageCameraRuntimeState *state,
                                        Th07StageCameraChannel channel,
                                        float value)
{
    float *start = StageCameraScalarForChannel(&state->interpolationStart, channel);
    float *target = StageCameraScalarForChannel(&state->target, channel);
    float *current = StageCameraScalarForChannel(&state->current, channel);
    if (start == nullptr || target == nullptr || current == nullptr) {
        return;
    }

    *start = *target;
    *target = value;
    if (state->interpolationDurations[StageCameraChannelIndex(channel)] == 0) {
        *current = value;
    }
}

void ApplyCurveStageCameraPoint(Th07StageCameraRuntimeState *state,
                                Th07StageCameraChannel channel,
                                Th07StageCameraOpcodeSemantic semantic,
                                Th07EffectVector3 value)
{
    Th07StageCameraState *basis = nullptr;
    switch (semantic) {
    case Th07StageCameraOpcodeSemantic::SetCurvePoint0:
        basis = &state->interpolationStart;
        break;
    case Th07StageCameraOpcodeSemantic::SetCurvePoint1:
        basis = &state->target;
        break;
    case Th07StageCameraOpcodeSemantic::SetCurvePoint2:
        basis = &state->curvePoint2;
        break;
    case Th07StageCameraOpcodeSemantic::SetCurvePoint3:
        basis = &state->curvePoint3;
        break;
    default:
        return;
    }

    Th07EffectVector3 *point = StageCameraVectorForChannel(basis, channel);
    if (point != nullptr) {
        *point = value;
    }
}

Th07EffectPerspectiveInitState BuildPerspectiveInit0041b0b0(Th07EffectVector3 auxVector,
                                                            Th07EffectPerspectiveCameraBasis cameraBasis,
                                                            Th07EffectPerspectiveInitRandoms randoms,
                                                            float framerateMultiplier,
                                                            bool rgbWhiteGatePassed)
{
    Th07EffectVector3 targetPosition = CameraOffsetPosition(cameraBasis, 1.0f);
    targetPosition.x += RandomSpan(randoms.targetX,
                                   kTh07EffectPerspectiveWideTargetXSpan,
                                   kTh07EffectPerspectiveWideTargetXCenter) -
                        cameraBasis.offset.x / 2.0f;
    targetPosition.y += RandomSpan(randoms.targetY,
                                   kTh07EffectPerspectiveWideTargetYSpan,
                                   kTh07EffectPerspectiveWideTargetYCenter) -
                        cameraBasis.offset.y / 2.0f;
    targetPosition.z += RandomSpan(randoms.targetZ,
                                   kTh07EffectPerspectiveTargetZSpan,
                                   kTh07EffectPerspectiveWideTargetZCenter) -
                        cameraBasis.offset.z / 2.0f;

    const Th07EffectVector3 velocity = Scale({
        auxVector.x + RandomSpan(randoms.velocityX,
                                 kTh07EffectPerspectiveVelocityXYSpan,
                                 kTh07EffectPerspectiveVelocityXYCenter),
        auxVector.y + RandomSpan(randoms.velocityY,
                                 kTh07EffectPerspectiveVelocityXYSpan,
                                 kTh07EffectPerspectiveVelocityXYCenter),
        auxVector.z + randoms.velocityZ * kTh07EffectPerspectiveVelocityZSpan +
            kTh07EffectPerspectiveVelocityZBase,
    }, framerateMultiplier);
    const Th07EffectVector3 acceleration = Scale({
        RandomSpan(randoms.accelerationX,
                   kTh07EffectPerspectiveAccelerationXYSpan,
                   kTh07EffectPerspectiveAccelerationXYCenter),
        RandomSpan(randoms.accelerationY,
                   kTh07EffectPerspectiveAccelerationXYSpan,
                   kTh07EffectPerspectiveAccelerationXYCenter),
        0.0f,
    }, framerateMultiplier);

    return {
        targetPosition,
        velocity,
        acceleration,
        RandomSpan(randoms.rotationX,
                   kTh07EffectPerspectiveSmallRotationXSpan,
                   kTh07EffectPerspectiveSmallRotationXCenter),
        RandomSpan(randoms.rotationZ,
                   kTh07EffectRadialRandomAngleScale,
                   kTh07EffectRadialRandomAngleBias),
        kTh07EffectPerspectiveRenderMode,
        rgbWhiteGatePassed,
        false,
    };
}

Th07EffectPerspectiveInitState BuildPerspectiveInitOrbiting(Th07EffectPerspectiveInitKind kind,
                                                            Th07EffectVector3 auxVector,
                                                            Th07EffectPerspectiveCameraBasis cameraBasis,
                                                            Th07EffectPerspectiveInitRandoms randoms,
                                                            float framerateMultiplier,
                                                            bool rgbWhiteGatePassed)
{
    const Th07EffectVector3 localTarget = {
        RandomSpan(randoms.targetX,
                   kTh07EffectPerspectiveTargetXYSpan,
                   kTh07EffectPerspectiveTargetXYCenter),
        RandomSpan(randoms.targetY,
                   kTh07EffectPerspectiveTargetXYSpan,
                   kTh07EffectPerspectiveTargetXYCenter),
        RandomSpan(randoms.targetZ,
                   kTh07EffectPerspectiveTargetZSpan,
                   kTh07EffectPerspectiveTargetZCenter),
    };
    const float velocityZ =
        kind == Th07EffectPerspectiveInitKind::Init0041b770
            ? -(randoms.velocityZ * kTh07EffectPerspectiveNegativeVelocityZSpan) -
                  kTh07EffectPerspectiveNegativeVelocityZBase
            : randoms.velocityZ * kTh07EffectPerspectiveVelocityZSpan +
                  kTh07EffectPerspectiveHighVelocityZBase;

    return {
        Add(CameraOffsetPosition(cameraBasis, kTh07EffectPerspectiveCameraOffsetHalf), localTarget),
        Scale({
            -localTarget.y / auxVector.x,
            localTarget.x / auxVector.x,
            velocityZ,
        }, framerateMultiplier),
        {},
        RandomSpan(randoms.rotationX,
                   kTh07EffectPerspectiveRotationXSpan,
                   kTh07EffectPerspectiveRotationXCenter),
        RandomSpan(randoms.rotationZ,
                   kTh07EffectRadialRandomAngleScale,
                   kTh07EffectRadialRandomAngleBias),
        kTh07EffectPerspectiveRenderMode,
        kind == Th07EffectPerspectiveInitKind::Init0041b770 || rgbWhiteGatePassed,
        false,
    };
}

Th07EffectPerspectiveInitState BuildPerspectiveInitDrift(Th07EffectPerspectiveInitKind kind,
                                                         Th07EffectVector3 auxVector,
                                                         Th07EffectPerspectiveCameraBasis cameraBasis,
                                                         Th07EffectPerspectiveInitRandoms randoms,
                                                         float framerateMultiplier)
{
    const bool fallingForward = kind == Th07EffectPerspectiveInitKind::Init0041bc20;
    const Th07EffectVector3 localTarget = {
        RandomSpan(randoms.targetX,
                   kTh07EffectPerspectiveTargetXYSpan,
                   kTh07EffectPerspectiveTargetXYCenter),
        RandomSpan(randoms.targetY,
                   kTh07EffectPerspectiveTargetXYSpan,
                   kTh07EffectPerspectiveTargetXYCenter),
        fallingForward
            ? randoms.targetZ * kTh07EffectPerspectiveForwardTargetZSpan
            : RandomSpan(randoms.targetZ,
                         kTh07EffectPerspectiveTargetZSpan,
                         kTh07EffectPerspectiveWideTargetZCenter),
    };
    const Th07EffectVector3 rawVelocity = {
        auxVector.x + RandomSpan(randoms.velocityX,
                                 kTh07EffectPerspectiveVelocityXYSpan,
                                 kTh07EffectPerspectiveVelocityXYCenter),
        auxVector.y + RandomSpan(randoms.velocityY,
                                 kTh07EffectPerspectiveVelocityXYSpan,
                                 kTh07EffectPerspectiveVelocityXYCenter),
        fallingForward
            ? auxVector.z - randoms.velocityZ * kTh07EffectPerspectiveVelocityZSpan
            : auxVector.z + randoms.velocityZ * kTh07EffectPerspectiveLowVelocityZSpan +
                  kTh07EffectPerspectiveLowVelocityZBase,
    };

    return {
        Add(CameraOffsetPosition(cameraBasis, kTh07EffectPerspectiveCameraOffsetHalf), localTarget),
        fallingForward ? Scale(rawVelocity, framerateMultiplier) : rawVelocity,
        {0.0f, 0.0f, fallingForward ? kTh07EffectPerspectiveFallAccelerationZ : 0.0f},
        RandomSpan(randoms.rotationX,
                   kTh07EffectPerspectiveRotationXSpan,
                   kTh07EffectPerspectiveRotationXCenter),
        RandomSpan(randoms.rotationZ,
                   kTh07EffectRadialRandomAngleScale,
                   kTh07EffectRadialRandomAngleBias),
        kTh07EffectPerspectiveRenderMode,
        true,
        fallingForward,
    };
}

} // namespace

Th07EffectLinearMotion BuildTh07EffectRandomSplashMotion(float randomXZeroToOne,
                                                         float randomYZeroToOne,
                                                         float framerateMultiplier)
{
    return BuildRandomSplashMotion(randomXZeroToOne,
                                   randomYZeroToOne,
                                   framerateMultiplier,
                                   1.0f / kTh07EffectRandomSplashVelocityDivisor,
                                   kTh07EffectRandomSplashAccelerationFrames);
}

Th07EffectLinearMotion BuildTh07EffectRandomSplashBigMotion(float randomXZeroToOne,
                                                            float randomYZeroToOne,
                                                            float framerateMultiplier)
{
    return BuildRandomSplashMotion(randomXZeroToOne,
                                   randomYZeroToOne,
                                   framerateMultiplier,
                                   kTh07EffectRandomSplashBigVelocityMultiplier /
                                       kTh07EffectRandomSplashBigVelocityDivisor,
                                   kTh07EffectRandomSplashBigAccelerationFrames);
}

Th07EffectRadialState BuildTh07EffectRadialState(Th07EffectVector3 position,
                                                 float randomZeroToOne)
{
    const float angle = randomZeroToOne * kTh07EffectRadialRandomAngleScale -
                        kTh07EffectRadialRandomAngleBias;
    return {
        {position.x, position.y, 0.0f},
        {static_cast<float>(std::sin(angle)), static_cast<float>(std::cos(angle)), 0.0f},
    };
}

Th07EffectVector3 BuildTh07EffectRadialInwardPosition(Th07EffectVector3 origin,
                                                      Th07EffectVector3 direction,
                                                      float timerFrames,
                                                      float durationFrames,
                                                      bool forceZToZero)
{
    const float distance = kTh07EffectRadialDistance -
                           timerFrames * kTh07EffectRadialDistance / durationFrames;
    Th07EffectVector3 position = {
        distance * direction.x + origin.x,
        distance * direction.y + origin.y,
        distance * direction.z + origin.z,
    };
    if (forceZToZero) {
        position.z = 0.0f;
    }
    return position;
}

Th07EffectVector3 BuildTh07EffectRadialOutwardPosition(Th07EffectVector3 origin,
                                                       Th07EffectVector3 direction,
                                                       float timerFrames)
{
    const float distance = timerFrames * kTh07EffectRadialDistance /
                           kTh07EffectRadialOutwardFrames;
    return {
        distance * direction.x + origin.x,
        distance * direction.y + origin.y,
        distance * direction.z + origin.z,
    };
}

Th07EffectAnchoredPosition BuildTh07EffectPlayerAnchoredPosition(bool hasActiveAnmInstruction,
                                                                 Th07EffectVector3 playerPosition)
{
    if (!hasActiveAnmInstruction) {
        return {false, {}};
    }
    return {true, playerPosition};
}

Th07StageCameraState BuildTh07InitialStageCameraState()
{
    return BuildTh07StageCameraSetupState(
        {0.0f, 0.0f, FloatFromBits(kTh07StageCameraInitialPositionZBits)},
        {},
        {0.0f, FloatFromBits(kTh07StageCameraInitialUpYBits), 0.0f},
        FloatFromBits(kTh07StageCameraInitialFovBits)).camera;
}

Th07StageCameraRuntimeState BuildTh07InitialStageCameraRuntimeState()
{
    Th07StageCameraRuntimeState state = {};
    state.current = BuildTh07InitialStageCameraState();
    state.target = state.current;
    state.interpolationStart = state.current;
    for (Th07StageCameraInterpolationTimerState &timer : state.interpolationTimers) {
        timer = {kTh07StageCameraInitialTimerPreviousFrame, 0.0f, 0};
    }
    return state;
}

Th07StageCameraSetupState BuildTh07StageCameraSetupState(Th07EffectVector3 position,
                                                         Th07EffectVector3 offset,
                                                         Th07EffectVector3 up,
                                                         float fov)
{
    const Th07StageCameraState camera = {
        position,
        offset,
        up,
        NormalizeOrZero(offset),
        NormalizeOrZero(Cross(offset, up)),
        fov,
    };
    return {
        camera,
        Add(position, offset),
        BuildTh07EffectCameraBasis(camera),
    };
}

Th07EffectPerspectiveCameraBasis BuildTh07EffectCameraBasis(Th07StageCameraState camera)
{
    return {camera.position, camera.offset};
}

bool ApplyTh07StageCameraOpcode(Th07StageCameraRuntimeState *state,
                                Th07StageCameraInstruction instruction)
{
    if (state == nullptr) {
        return false;
    }

    const Th07StageCameraOpcodeLayout *layout = FindTh07StageCameraOpcode(instruction.opcode);
    if (layout == nullptr) {
        return false;
    }

    const Th07EffectVector3 vectorArgs = {
        FloatFromSignedBits(instruction.args[0]),
        FloatFromSignedBits(instruction.args[1]),
        FloatFromSignedBits(instruction.args[2]),
    };

    switch (layout->semantic) {
    case Th07StageCameraOpcodeSemantic::SetLinearTarget:
        if (layout->channel == Th07StageCameraChannel::Fov) {
            ApplyLinearStageCameraScalarTarget(state, layout->channel, vectorArgs.x);
        } else {
            ApplyLinearStageCameraTarget(state, layout->channel, vectorArgs);
        }
        return true;
    case Th07StageCameraOpcodeSemantic::StartLinearInterpolation:
        state->interpolationDurations[StageCameraChannelIndex(layout->channel)] = instruction.args[0];
        ResetStageCameraInterpolationTimer(state, layout->channel, instruction.args[1]);
        return true;
    case Th07StageCameraOpcodeSemantic::SetCurvePoint0:
    case Th07StageCameraOpcodeSemantic::SetCurvePoint1:
    case Th07StageCameraOpcodeSemantic::SetCurvePoint2:
    case Th07StageCameraOpcodeSemantic::SetCurvePoint3:
        ApplyCurveStageCameraPoint(state, layout->channel, layout->semantic, vectorArgs);
        return true;
    case Th07StageCameraOpcodeSemantic::StartCurveInterpolation:
        state->interpolationDurations[StageCameraChannelIndex(layout->channel)] = instruction.args[0];
        ResetStageCameraInterpolationTimer(state, layout->channel, kTh07StageCameraCurveInterpolationMode);
        return true;
    }

    return false;
}

bool IsTh07StageNonCameraOpcode(std::int32_t opcode)
{
    switch (opcode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x0d:
    case 0x1d:
    case 0x1e:
        return true;
    default:
        return false;
    }
}

bool IsTh07StageScriptInstructionDue(Th07StageScriptRuntimeState state,
                                     Th07StageScriptInstruction instruction)
{
    return instruction.frame != -1 && state.scriptTimer.currentFrame >= instruction.frame;
}

Th07StageAnmVmOpcodeEffect BuildTh07StageAnmVmOpcodeEffect(Th07StageScriptInstruction instruction)
{
    if (instruction.opcode != 0x1d && instruction.opcode != 0x1e) {
        return {};
    }

    if (instruction.args[0] < 0) {
        return {
            true,
            instruction.opcode == 0x1d ? Th07StageAnmVmSlot::Primary : Th07StageAnmVmSlot::Secondary,
            false,
            true,
            -1,
        };
    }

    return {
        true,
        instruction.opcode == 0x1d ? Th07StageAnmVmSlot::Primary : Th07StageAnmVmSlot::Secondary,
        true,
        false,
        instruction.args[0] + kTh07StageAnmVmScriptBase,
    };
}

const Th07AnmVmRenderHelperContract *FindTh07AnmVmRenderHelper(std::uint32_t functionAddress)
{
    for (const Th07AnmVmRenderHelperContract &helper : kTh07AnmVmRenderHelpers) {
        if (helper.functionAddress == functionAddress) {
            return &helper;
        }
    }

    return nullptr;
}

const Th07AnmVmExecuteOpcodeContract *FindTh07AnmVmExecuteOpcode(std::int32_t opcode)
{
    for (const Th07AnmVmExecuteOpcodeContract &contract : kTh07AnmVmExecuteOpcodes) {
        if (contract.opcode == opcode) {
            return &contract;
        }
    }

    return nullptr;
}

const Th07AnmVmFollowupOpcodeContract *FindTh07AnmVmFollowupExecuteOpcode(std::int32_t opcode)
{
    for (const Th07AnmVmFollowupOpcodeContract &contract : kTh07AnmVmFollowupExecuteOpcodes) {
        if (contract.opcode == opcode) {
            return &contract;
        }
    }

    return nullptr;
}

const Th07AnmVmVariableOpcodeContract *FindTh07AnmVmVariableExecuteOpcode(std::int32_t opcode)
{
    for (const Th07AnmVmVariableOpcodeContract &contract : kTh07AnmVmVariableExecuteOpcodes) {
        if (contract.opcode == opcode) {
            return &contract;
        }
    }

    return nullptr;
}

const Th07AnmVmConditionalJumpOpcodeContract *FindTh07AnmVmConditionalJumpExecuteOpcode(std::int32_t opcode)
{
    for (const Th07AnmVmConditionalJumpOpcodeContract &contract : kTh07AnmVmConditionalJumpExecuteOpcodes) {
        if (contract.opcode == opcode) {
            return &contract;
        }
    }

    return nullptr;
}

const Th07AnmVmControlOpcodeContract *FindTh07AnmVmControlExecuteOpcode(std::int32_t opcode)
{
    for (const Th07AnmVmControlOpcodeContract &contract : kTh07AnmVmControlExecuteOpcodes) {
        if (contract.opcode == opcode) {
            return &contract;
        }
    }

    return nullptr;
}

const Th07AnmVmPostStepActionContract *FindTh07AnmVmPostStepAction(Th07AnmVmPostStepActionSemantic semantic,
                                                                   std::uint32_t primaryFieldOffset)
{
    for (const Th07AnmVmPostStepActionContract &contract : kTh07AnmVmPostStepActions) {
        if (contract.semantic == semantic && contract.primaryFieldOffset == primaryFieldOffset) {
            return &contract;
        }
    }

    return nullptr;
}

Th07StageScriptStepResult ApplyTh07StageScriptOpcode(Th07StageScriptRuntimeState *state,
                                                     Th07StageScriptInstruction instruction,
                                                     Th07StageScriptInstruction nextPositionKey)
{
    if (state == nullptr) {
        return Th07StageScriptStepResult::Unsupported;
    }

    switch (instruction.opcode) {
    case 0:
        state->position = StageScriptVectorArgs(instruction);
        state->positionInterpolationInitial = state->position;
        state->positionInterpolationStartFrame = instruction.frame;
        if (nextPositionKey.opcode == 0) {
            state->positionInterpolationEndFrame = nextPositionKey.frame;
            state->positionInterpolationFinal = StageScriptVectorArgs(nextPositionKey);
        }
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Applied;
    case 1:
        state->fog.color = static_cast<std::uint32_t>(instruction.args[0]);
        state->fog.nearPlane = FloatFromSignedBits(instruction.args[1]);
        state->fog.farPlane = FloatFromSignedBits(instruction.args[2]);
        state->fogInterpolationFinal = state->fog;
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Applied;
    case 2:
        state->fogInterpolationInitial = state->fog;
        state->fogInterpolationDuration = instruction.args[0];
        state->fogInterpolationTimer = {kTh07StageCameraInitialTimerPreviousFrame, 0.0f, 0};
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Applied;
    case 3:
        if (state->cameraResumeInstruction == 0) {
            return Th07StageScriptStepResult::Paused;
        }
        state->cameraResumeInstruction = 0;
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Applied;
    case 4:
        state->instructionIndex = instruction.args[0];
        state->scriptTimer = {kTh07StageCameraInitialTimerPreviousFrame, 0.0f, instruction.args[1]};
        state->cameraSetupPending = true;
        return Th07StageScriptStepResult::Jumped;
    case 0x0d:
        state->field4d8 = instruction.args[0];
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Applied;
    case 0x1d:
    case 0x1e:
        state->instructionIndex += 1;
        return Th07StageScriptStepResult::Unsupported;
    default:
        return Th07StageScriptStepResult::Unsupported;
    }
}

void AdvanceTh07StageFogInterpolation(Th07StageScriptRuntimeState *state,
                                      float framerateMultiplier,
                                      float effectiveFramerateMultiplier)
{
    if (state == nullptr || state->fogInterpolationDuration == 0) {
        return;
    }

    TickStageCameraInterpolationTimer(&state->fogInterpolationTimer,
                                      framerateMultiplier,
                                      effectiveFramerateMultiplier);
    float ratio = (static_cast<float>(state->fogInterpolationTimer.currentFrame) +
                   state->fogInterpolationTimer.subframe) /
                  static_cast<float>(state->fogInterpolationDuration);
    if (ratio >= 1.0f) {
        ratio = 1.0f;
    }

    state->fog.color =
        BuildInterpolatedColor(state->fogInterpolationInitial.color,
                               state->fogInterpolationFinal.color,
                               ratio);
    state->fog.nearPlane =
        (state->fogInterpolationFinal.nearPlane - state->fogInterpolationInitial.nearPlane) * ratio +
        state->fogInterpolationInitial.nearPlane;
    state->fog.farPlane =
        (state->fogInterpolationFinal.farPlane - state->fogInterpolationInitial.farPlane) * ratio +
        state->fogInterpolationInitial.farPlane;
    if (state->fogInterpolationTimer.currentFrame >= state->fogInterpolationDuration) {
        state->fogInterpolationDuration = 0;
    }
}

void AdvanceTh07StageCameraRuntimeState(Th07StageCameraRuntimeState *state,
                                        float framerateMultiplier,
                                        float effectiveFramerateMultiplier)
{
    if (state == nullptr) {
        return;
    }

    for (const Th07StageCameraVectorChannelLayout &layout : kTh07StageCameraVectorChannels) {
        if (state->interpolationDurations[StageCameraChannelIndex(layout.channel)] != 0) {
            const float ratio = AdvanceStageCameraInterpolationTimer(state,
                                                                     layout.channel,
                                                                     framerateMultiplier,
                                                                     effectiveFramerateMultiplier);
            *StageCameraVectorForChannel(&state->current, layout.channel) =
                BuildTh07StageCameraInterpolatedVector(
                    *StageCameraVectorForChannel(&state->interpolationStart, layout.channel),
                    *StageCameraVectorForChannel(&state->target, layout.channel),
                    *StageCameraVectorForChannel(&state->curvePoint2, layout.channel),
                    *StageCameraVectorForChannel(&state->curvePoint3, layout.channel),
                    ratio,
                    state->interpolationModes[StageCameraChannelIndex(layout.channel)]);
        }
    }

    if (state->interpolationDurations[StageCameraChannelIndex(Th07StageCameraChannel::Fov)] != 0) {
        const float ratio = AdvanceStageCameraInterpolationTimer(state,
                                                                 Th07StageCameraChannel::Fov,
                                                                 framerateMultiplier,
                                                                 effectiveFramerateMultiplier);
        state->current.fov =
            BuildTh07StageCameraInterpolatedScalar(state->interpolationStart.fov,
                                                   state->target.fov,
                                                   ratio,
                                                   state->interpolationModes[StageCameraChannelIndex(
                                                       Th07StageCameraChannel::Fov)]);
    }

    state->current.forward = NormalizeOrZero(state->current.offset);
}

float ApplyTh07StageCameraInterpolationCurve(float ratio, std::int32_t interpolationMode)
{
    switch (interpolationMode) {
    case 1:
        return 1.0f - (1.0f - ratio) * (1.0f - ratio);
    case 2:
        return 1.0f - (1.0f - ratio) * (1.0f - ratio) * (1.0f - ratio);
    case 3:
        return 1.0f - (1.0f - ratio) * (1.0f - ratio) * (1.0f - ratio) * (1.0f - ratio);
    case 4:
        return ratio * ratio;
    case 5:
        return ratio * ratio * ratio;
    case 6:
        return ratio * ratio * ratio * ratio;
    default:
        return ratio;
    }
}

float BuildTh07StageCameraCurveComponent(float point0,
                                         float point1,
                                         float point2,
                                         float point3,
                                         float ratio)
{
    const float inverse = 1.0f - ratio;
    return (ratio - 1.0f) * ratio * ratio * point3 +
           inverse * inverse * ratio * point2 +
           (3.0f - 2.0f * ratio) * ratio * ratio * point1 +
           (2.0f * ratio + 1.0f) * (ratio - 1.0f) * (ratio - 1.0f) * point0;
}

Th07EffectVector3 BuildTh07StageCameraInterpolatedVector(Th07EffectVector3 point0,
                                                         Th07EffectVector3 point1,
                                                         Th07EffectVector3 point2,
                                                         Th07EffectVector3 point3,
                                                         float ratio,
                                                         std::int32_t interpolationMode)
{
    if (interpolationMode == kTh07StageCameraCurveInterpolationMode) {
        return {
            BuildTh07StageCameraCurveComponent(point0.x, point1.x, point2.x, point3.x, ratio),
            BuildTh07StageCameraCurveComponent(point0.y, point1.y, point2.y, point3.y, ratio),
            BuildTh07StageCameraCurveComponent(point0.z, point1.z, point2.z, point3.z, ratio),
        };
    }

    const float curvedRatio = ApplyTh07StageCameraInterpolationCurve(ratio, interpolationMode);
    return {
        (point1.x - point0.x) * curvedRatio + point0.x,
        (point1.y - point0.y) * curvedRatio + point0.y,
        (point1.z - point0.z) * curvedRatio + point0.z,
    };
}

float BuildTh07StageCameraInterpolatedScalar(float point0,
                                             float point1,
                                             float ratio,
                                             std::int32_t interpolationMode)
{
    const float curvedRatio = ApplyTh07StageCameraInterpolationCurve(ratio, interpolationMode);
    return (point1 - point0) * curvedRatio + point0;
}

Th07EffectOrbitState BuildTh07EffectOrbitingRenderMode2State(Th07EffectVector3 origin,
                                                             Th07EffectVector3 direction,
                                                             float radius,
                                                             float angle)
{
    const Th07EffectVector3 axis = NormalizeOrZero(direction);
    const float cosine = std::cos(angle);
    const float sine = std::sin(angle);
    const Th07EffectQuaternion quaternion = {
        axis.x * cosine,
        axis.y * cosine,
        axis.z * cosine,
        sine,
    };

    Th07EffectVector3 sideVector = {
        axis.y,
        -axis.x,
        0.0f,
    };
    if (LengthSquared(sideVector) >= kTh07EffectOrbitSideVectorLengthEpsilon) {
        sideVector = NormalizeOrZero(sideVector);
    }
    sideVector.x *= radius;
    sideVector.y *= radius;
    sideVector.z *= radius;

    const float doubledX = quaternion.x * kTh07EffectOrbitFadeScaleBase;
    const float doubledY = quaternion.y * kTh07EffectOrbitFadeScaleBase;
    const float doubledZ = quaternion.z * kTh07EffectOrbitFadeScaleBase;
    const float matrix00 = (1.0f - doubledY * quaternion.y) - doubledZ * quaternion.z;
    const float matrix01 = doubledY * quaternion.x + doubledZ * quaternion.w;
    const float matrix02 = doubledZ * quaternion.x - doubledY * quaternion.w;
    const float matrix10 = doubledY * quaternion.x - doubledZ * quaternion.w;
    const float matrix11 = (1.0f - doubledX * quaternion.x) - doubledZ * quaternion.z;
    const float matrix12 = doubledZ * quaternion.y + doubledX * quaternion.w;
    const float matrix20 = doubledZ * quaternion.x + doubledY * quaternion.w;
    const float matrix21 = doubledZ * quaternion.y - doubledX * quaternion.w;
    const float matrix22 = (1.0f - doubledX * quaternion.x) - doubledY * quaternion.y;

    Th07EffectVector3 rotated = {
        sideVector.z * matrix20 + sideVector.y * matrix10 + sideVector.x * matrix00,
        sideVector.z * matrix21 + sideVector.y * matrix11 + sideVector.x * matrix01,
        sideVector.z * matrix22 + sideVector.y * matrix12 + sideVector.x * matrix02,
    };
    rotated.z *= kTh07EffectOrbitZScale;

    return {
        {
            rotated.x + origin.x,
            rotated.y + origin.y,
            rotated.z + origin.z,
        },
        quaternion,
    };
}

Th07EffectOrbitFadeStep BuildTh07EffectOrbitFadeStep(std::int32_t currentFadeFrame,
                                                     std::uint32_t color)
{
    const std::int32_t nextFrame = currentFadeFrame + 1;
    if (kTh07EffectOrbitFadeFrames <= static_cast<float>(nextFrame)) {
        return {false, nextFrame, color, 0.0f};
    }

    const float fade = 1.0f - static_cast<float>(nextFrame) / kTh07EffectOrbitFadeFrames;
    const std::uint32_t alpha =
        static_cast<std::uint32_t>(fade * kTh07EffectOrbitFadeAlphaScale) << 24;
    return {
        true,
        nextFrame,
        (color & 0x00ffffff) | alpha,
        kTh07EffectOrbitFadeScaleBase - fade,
    };
}

Th07EffectPerspectiveMotionState BuildTh07EffectPerspectiveMotionState(Th07EffectVector3 velocity,
                                                                       Th07EffectVector3 acceleration,
                                                                       Th07EffectVector3 targetPosition,
                                                                       float rotationX,
                                                                       float rotationZ,
                                                                       std::uint32_t renderFlags,
                                                                       Th07EffectVector3 cameraPosition,
                                                                       Th07EffectVector3 cameraForward)
{
    const Th07EffectVector3 nextVelocity = {
        velocity.x + acceleration.x,
        velocity.y + acceleration.y,
        velocity.z + acceleration.z,
    };
    const Th07EffectVector3 nextTargetPosition = {
        targetPosition.x + nextVelocity.x,
        targetPosition.y + nextVelocity.y,
        targetPosition.z + nextVelocity.z,
    };
    const Th07EffectVector3 position = nextTargetPosition;
    const Th07EffectVector3 directionToEffect = NormalizeOrZero({
        position.x - cameraPosition.x,
        position.y - cameraPosition.y,
        position.z - cameraPosition.z,
    });
    const float forwardDot =
        cameraForward.x * directionToEffect.x +
        cameraForward.y * directionToEffect.y +
        cameraForward.z * directionToEffect.z;
    const bool inForwardCone = forwardDot >= kTh07EffectPerspectiveForwardDotThreshold;
    if (!inForwardCone) {
        return {
            nextVelocity,
            nextTargetPosition,
            position,
            rotationZ,
            renderFlags,
            false,
            false,
        };
    }

    return {
        nextVelocity,
        nextTargetPosition,
        position,
        NormalizeTh07EffectAngle(rotationZ + rotationX),
        renderFlags | kTh07EffectPerspectiveRenderFlagBits,
        true,
        position.z < kTh07EffectPerspectiveZAliveThreshold,
    };
}

bool BuildTh07EffectPerspectiveRgbWhiteGate(std::int32_t maxValue,
                                            std::int32_t currentValue,
                                            std::int32_t denominator,
                                            std::uint32_t randomValue)
{
    const std::uint32_t threshold = static_cast<std::uint32_t>(
        ((maxValue - currentValue) * kTh07EffectPerspectiveRgbWhiteGateScale) / denominator);
    return randomValue % kTh07EffectPerspectiveRgbWhiteGateScale <= threshold;
}

Th07EffectPerspectiveInitState BuildTh07EffectPerspectiveInitState(Th07EffectPerspectiveInitKind kind,
                                                                   Th07EffectVector3 auxVector,
                                                                   Th07EffectPerspectiveCameraBasis cameraBasis,
                                                                   Th07EffectPerspectiveInitRandoms randoms,
                                                                   float framerateMultiplier,
                                                                   bool rgbWhiteGatePassed)
{
    switch (kind) {
    case Th07EffectPerspectiveInitKind::Init0041b0b0:
        return BuildPerspectiveInit0041b0b0(auxVector,
                                           cameraBasis,
                                           randoms,
                                           framerateMultiplier,
                                           rgbWhiteGatePassed);
    case Th07EffectPerspectiveInitKind::Init0041b4a0:
    case Th07EffectPerspectiveInitKind::Init0041b770:
        return BuildPerspectiveInitOrbiting(kind,
                                            auxVector,
                                            cameraBasis,
                                            randoms,
                                            framerateMultiplier,
                                            rgbWhiteGatePassed);
    case Th07EffectPerspectiveInitKind::Init0041b9f0:
    case Th07EffectPerspectiveInitKind::Init0041bc20:
        return BuildPerspectiveInitDrift(kind,
                                         auxVector,
                                         cameraBasis,
                                         randoms,
                                         framerateMultiplier);
    }

    return {};
}

Th07EffectCameraSnapState BuildTh07EffectCameraSnapState(Th07EffectPerspectiveCameraBasis cameraBasis)
{
    const Th07EffectVector3 targetPosition = CameraOffsetPosition(cameraBasis, 1.0f);
    return {
        targetPosition,
        {targetPosition.x, targetPosition.y, 0.0f},
        kTh07EffectCameraSnapRenderMode,
        true,
    };
}

float NormalizeTh07EffectAngle(float angle)
{
    int iterations = 0;
    while (angle > kTh07EffectRadialRandomAngleBias) {
        angle -= kTh07EffectRadialRandomAngleScale;
        if (iterations++ > 16) {
            break;
        }
    }
    while (angle < -kTh07EffectRadialRandomAngleBias) {
        angle += kTh07EffectRadialRandomAngleScale;
        if (iterations++ > 16) {
            break;
        }
    }
    return angle;
}

Th07EffectRadialState BuildTh07EffectEasedRadialStateFromAngle(Th07EffectVector3 position,
                                                               float angle,
                                                               float randomRadiusZeroToOne)
{
    const float normalizedAngle = NormalizeTh07EffectAngle(angle);
    const float radiusScale = randomRadiusZeroToOne * kTh07EffectEasedRadialRadiusScale +
                              kTh07EffectEasedRadialRadiusBase;
    return {
        {position.x, position.y, 0.0f},
        {static_cast<float>(std::sin(normalizedAngle)) * radiusScale,
         static_cast<float>(std::cos(normalizedAngle)) * radiusScale,
         0.0f},
    };
}

Th07EffectRadialState BuildTh07EffectEasedRadialStateFromRandomAngle(Th07EffectVector3 position,
                                                                     float randomAngleZeroToOne,
                                                                     float randomRadiusZeroToOne)
{
    return BuildTh07EffectEasedRadialStateFromAngle(
        position,
        randomAngleZeroToOne * kTh07EffectRadialRandomAngleScale - kTh07EffectRadialRandomAngleBias,
        randomRadiusZeroToOne);
}

Th07EffectVector3 BuildTh07EffectEasedRadialPosition(Th07EffectVector3 origin,
                                                     Th07EffectVector3 scaledDirection,
                                                     float timerFrames)
{
    const float ratio = timerFrames / kTh07EffectEasedRadialFrames;
    const float eased = kTh07EffectEasedRadialRadiusBase -
                        (kTh07EffectEasedRadialRadiusBase - ratio) *
                            (kTh07EffectEasedRadialRadiusBase - ratio);
    return {
        eased * scaledDirection.x * kTh07EffectEasedRadialDistance + origin.x,
        eased * scaledDirection.y * kTh07EffectEasedRadialDistance + origin.y,
        0.0f,
    };
}

} // namespace th07
