#include "Stage.hpp"
#include "AnmIdx.hpp"
#include "AnmManager.hpp"
#include "Chain.hpp"
#include "ChainPriorities.hpp"
#include "FileSystem.hpp"
#include "GameManager.hpp"
#include "Gui.hpp"
#include "Rng.hpp"
#include "ScreenEffect.hpp"
#include "Supervisor.hpp"
#include "ZunColor.hpp"
#include "utils.hpp"
#include <bit>
#include <cmath>
#include <cstring>
#include <d3d8.h>
#include <limits>
#include <span>

namespace th07
{
DIFFABLE_STATIC(ChainElem, g_StageCalcChain)
DIFFABLE_STATIC(ChainElem, g_StageOnDrawHighPrioChain)
DIFFABLE_STATIC(ChainElem, g_StageOnDrawLowPrioChain)

DIFFABLE_STATIC_ARRAY_ASSIGN(StageFile, 9, g_StageFiles) = {
    {"dummy", "dummy"},
    {"data/stg1bg.anm", "data/stage1.std"},
    {"data/stg2bg.anm", "data/stage2.std"},
    {"data/stg3bg.anm", "data/stage3.std"},
    {"data/stg4bg.anm", "data/stage4.std"},
    {"data/stg5bg.anm", "data/stage5.std"},
    {"data/stg6bg.anm", "data/stage6.std"},
    {"data/stg7bg.anm", "data/stage7.std"},
    {"data/stg8bg.anm", "data/stage8.std"},
};
DIFFABLE_STATIC(Stage, g_Stage)

namespace {

D3DXVECTOR3 ToD3dxVector(Th07EffectVector3 value)
{
    return {value.x, value.y, value.z};
}

Th07EffectVector3 ToTh07Vector(D3DXVECTOR3 value)
{
    return {value.x, value.y, value.z};
}

void ApplyStageCameraBasis(StageCameraBasisBlock *basis, const Th07StageCameraState &camera)
{
    basis->position = ToD3dxVector(camera.position);
    basis->offset = ToD3dxVector(camera.offset);
    basis->up = ToD3dxVector(camera.up);
    basis->forward = ToD3dxVector(camera.forward);
    basis->sideVector = ToD3dxVector(camera.sideVector);
    basis->fov = camera.fov;
}

Th07StageCameraState BuildStageCameraState(const StageCameraBasisBlock &basis)
{
    return {
        ToTh07Vector(basis.position),
        ToTh07Vector(basis.offset),
        ToTh07Vector(basis.up),
        ToTh07Vector(basis.forward),
        ToTh07Vector(basis.sideVector),
        basis.fov,
    };
}

Th07StageCameraRuntimeState BuildStageCameraRuntimeState(const Stage *stage)
{
    Th07StageCameraRuntimeState state = {};
    state.target = BuildStageCameraState(stage->th07CameraTargetBasis);
    state.interpolationStart = BuildStageCameraState(stage->th07CameraInterpStartBasis);
    state.curvePoint3 = BuildStageCameraState(stage->th07CameraCurvePoint3Basis);
    state.curvePoint2 = BuildStageCameraState(stage->th07CameraCurvePoint2Basis);
    state.current = BuildStageCameraState(stage->th07CameraCurrentBasis);
    for (i32 idx = 0; idx < 4; idx++)
    {
        state.interpolationDurations[idx] = stage->th07CameraInterpDurations[idx];
        state.interpolationTimers[idx] = {
            stage->th07CameraInterpTimers[idx].previousFrame,
            stage->th07CameraInterpTimers[idx].subframe,
            stage->th07CameraInterpTimers[idx].currentFrame,
        };
        state.interpolationModes[idx] = stage->th07CameraInterpModes[idx];
    }
    return state;
}

void ApplyStageCameraRuntimeState(Stage *stage, const Th07StageCameraRuntimeState &state)
{
    ApplyStageCameraBasis(&stage->th07CameraTargetBasis, state.target);
    ApplyStageCameraBasis(&stage->th07CameraInterpStartBasis, state.interpolationStart);
    ApplyStageCameraBasis(&stage->th07CameraCurvePoint3Basis, state.curvePoint3);
    ApplyStageCameraBasis(&stage->th07CameraCurvePoint2Basis, state.curvePoint2);
    ApplyStageCameraBasis(&stage->th07CameraCurrentBasis, state.current);
    for (i32 idx = 0; idx < 4; idx++)
    {
        stage->th07CameraInterpDurations[idx] = state.interpolationDurations[idx];
        stage->th07CameraInterpTimers[idx].previousFrame = state.interpolationTimers[idx].previousFrame;
        stage->th07CameraInterpTimers[idx].subframe = state.interpolationTimers[idx].subframe;
        stage->th07CameraInterpTimers[idx].currentFrame = state.interpolationTimers[idx].currentFrame;
        stage->th07CameraInterpModes[idx] = state.interpolationModes[idx];
    }
}

Th07StageCameraInterpolationTimerState BuildStageTimerState(const StageCameraInterpolationTimer &timer)
{
    return {timer.previousFrame, timer.subframe, timer.currentFrame};
}

void ApplyStageTimerState(StageCameraInterpolationTimer *timer,
                          Th07StageCameraInterpolationTimerState state)
{
    timer->previousFrame = state.previousFrame;
    timer->subframe = state.subframe;
    timer->currentFrame = state.currentFrame;
}

void ResetTh07StageAnmManagerColorWrite(Stage *stage)
{
    stage->th07AnmManagerColorMultiplier = kTh07StageAnmManagerColorMultiplierIdle;
}

void ConsumeTh07StageAnmManagerColorWrite(Stage *stage)
{
    if ((stage->th07AnmManagerColorMultiplier & 0xff000000) != 0)
    {
        SetTh07AnmManagerColorMultiplier(stage->th07AnmManagerColorMultiplier);
    }
    ResetTh07StageAnmManagerColorWrite(stage);
}

void ResetTh07StageAnmManagerColorAfterLowPrio(Stage *stage)
{
    if (stage->th07AnmManagerColorResetGuard == 0)
    {
        ResetTh07AnmManagerColorMultiplier();
    }
    stage->th07AnmManagerColorResetGuard = 0;
}

Th07StageFogState BuildStageFogState(StageCameraSky fog)
{
    return {fog.nearPlane, fog.farPlane, fog.color};
}

StageCameraSky BuildStageCameraSky(Th07StageFogState fog)
{
    return {fog.nearPlane, fog.farPlane, fog.color};
}

Th07StageScriptRuntimeState BuildStageScriptRuntimeState(const Stage *stage)
{
    return {
        stage->th07InstructionIndex,
        BuildStageTimerState(stage->th07ScriptTime),
        stage->th07FrameCounter,
        ToTh07Vector(stage->th07StagePosition),
        stage->th07StageField4d8,
        BuildStageFogState(stage->th07Fog),
        BuildStageFogState(stage->th07FogInterpInitial),
        BuildStageFogState(stage->th07FogInterpFinal),
        stage->th07FogInterpDuration,
        BuildStageTimerState(stage->th07FogInterpTimer),
        stage->th07CameraResumeInstruction,
        ToTh07Vector(stage->th07StagePositionInterpFinal),
        stage->th07StagePositionInterpEndFrame,
        ToTh07Vector(stage->th07StagePositionInterpInitial),
        stage->th07StagePositionInterpStartFrame,
        stage->th07CameraSetupPendingAfterJump != 0,
    };
}

void ApplyStageScriptRuntimeState(Stage *stage, const Th07StageScriptRuntimeState &state)
{
    stage->th07InstructionIndex = state.instructionIndex;
    ApplyStageTimerState(&stage->th07ScriptTime, state.scriptTimer);
    stage->th07FrameCounter = state.frameCounter;
    stage->th07StagePosition = ToD3dxVector(state.position);
    stage->th07StageField4d8 = state.field4d8;
    stage->th07Fog = BuildStageCameraSky(state.fog);
    stage->th07FogInterpInitial = BuildStageCameraSky(state.fogInterpolationInitial);
    stage->th07FogInterpFinal = BuildStageCameraSky(state.fogInterpolationFinal);
    stage->th07FogInterpDuration = state.fogInterpolationDuration;
    ApplyStageTimerState(&stage->th07FogInterpTimer, state.fogInterpolationTimer);
    stage->th07CameraResumeInstruction = state.cameraResumeInstruction;
    stage->th07StagePositionInterpFinal = ToD3dxVector(state.positionInterpolationFinal);
    stage->th07StagePositionInterpEndFrame = state.positionInterpolationEndFrame;
    stage->th07StagePositionInterpInitial = ToD3dxVector(state.positionInterpolationInitial);
    stage->th07StagePositionInterpStartFrame = state.positionInterpolationStartFrame;
    stage->th07CameraSetupPendingAfterJump = state.cameraSetupPending ? 1 : 0;
}

Th07StageScriptInstruction BuildStageScriptInstruction(const RawStageInstr *instruction)
{
    return {
        instruction->frame,
        instruction->opcode,
        {instruction->args[0], instruction->args[1], instruction->args[2]},
    };
}

Th07StageScriptInstruction FindNextTh07StagePositionKey(const RawStageInstr *instruction)
{
    const RawStageInstr *cursor = instruction + 1;
    while (cursor->frame != -1 && cursor->opcode != 0)
    {
        cursor++;
    }
    if (cursor->opcode != 0)
    {
        return {-1, -1, {0, 0, 0}};
    }
    return BuildStageScriptInstruction(cursor);
}

void TickTh07StageTimer(StageCameraInterpolationTimer *timer)
{
    timer->previousFrame = timer->currentFrame;
    g_Supervisor.TickTimer(&timer->currentFrame, &timer->subframe);
}

void ResolvePendingTh07StageResume(Stage *stage)
{
    if (stage->th07CameraResumeInstruction == 0)
    {
        return;
    }

    i32 idx = 0;
    RawStageInstr *instruction = stage->th07BeginningOfScript;
    stage->th07InstructionIndex = 0;
    while (instruction->frame != -1 &&
           (instruction->opcode != 0x1f || instruction->args[0] != stage->th07CameraResumeInstruction))
    {
        idx++;
        instruction++;
    }
    if (instruction->frame != -1)
    {
        stage->th07InstructionIndex = idx + 1;
        stage->th07ScriptTime.currentFrame = instruction->frame;
        stage->th07ScriptTime.subframe = 0.0f;
        stage->th07ScriptTime.previousFrame = -999;
        stage->th07CameraResumeInstruction = 0;
    }
}

void InitializeTh07StageCamera(Stage *stage)
{
    const Th07StageCameraState camera = BuildTh07InitialStageCameraState();

    stage->th07CameraResumeInstruction = 0;
    ApplyStageCameraBasis(&stage->th07CameraCurrentBasis, camera);
    stage->th07CameraTargetBasis = stage->th07CameraCurrentBasis;
    stage->th07CameraInterpStartBasis = stage->th07CameraCurrentBasis;
    stage->th07CameraCurvePoint2Basis = {};
    stage->th07CameraCurvePoint3Basis = {};
    for (i32 idx = 0; idx < 4; idx++)
    {
        stage->th07CameraInterpDurations[idx] = 0;
        stage->th07CameraInterpTimers[idx].previousFrame = -999;
        stage->th07CameraInterpTimers[idx].subframe = 0.0f;
        stage->th07CameraInterpTimers[idx].currentFrame = 0;
        stage->th07CameraInterpModes[idx] = 0;
    }
}

void RefreshTh07StageAnmVmActiveSprite(Th07StageOriginalAnmVmSlot *slot)
{
    slot->originalActiveSpriteIndex = slot->vm.activeSpriteIndex;
    slot->originalBaseSpriteIndex = slot->vm.baseSpriteIndex;
    slot->originalBeginningOfScript = slot->vm.beginingOfScript;
    slot->originalCurrentInstruction = slot->vm.currentInstruction;
    slot->originalLoadedSprite = slot->vm.sprite;
}

const u8 *Th07StageAnmVmInstructionBytes(AnmRawInstr *instruction)
{
    return reinterpret_cast<const u8 *>(instruction);
}

AnmRawInstr *Th07StageAnmVmRawInstruction(const u8 *instruction)
{
    return reinterpret_cast<AnmRawInstr *>(const_cast<u8 *>(instruction));
}

void SetTh07StageAnmVmCurrentInstruction(Th07StageOriginalAnmVmSlot *slot, const u8 *instruction)
{
    AnmRawInstr *rawInstruction = Th07StageAnmVmRawInstruction(instruction);
    slot->originalCurrentInstruction = rawInstruction;
    slot->vm.currentInstruction = rawInstruction;
}

void WriteTh07StageAnmVmFloat(Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset, f32 value)
{
    std::memcpy(reinterpret_cast<u8 *>(slot) + offset, &value, sizeof(value));
}

f32 ReadTh07StageAnmVmFloat(const Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset)
{
    f32 value = 0.0f;
    std::memcpy(&value, reinterpret_cast<const u8 *>(slot) + offset, sizeof(value));
    return value;
}

u8 ReadTh07StageAnmVmByte(const Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset)
{
    return *(reinterpret_cast<const u8 *>(slot) + offset);
}

i32 ReadTh07StageAnmVmI32(const Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset)
{
    i32 value = 0;
    std::memcpy(&value, reinterpret_cast<const u8 *>(slot) + offset, sizeof(value));
    return value;
}

f32 ReadTh07StageAnmVmF32Memory(const u8 *bytes)
{
    f32 value = 0.0f;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}

void WriteTh07StageAnmVmF32Memory(u8 *bytes, f32 value)
{
    std::memcpy(bytes, &value, sizeof(value));
}

i32 ReadTh07StageAnmVmI32Memory(const u8 *bytes)
{
    i32 value = 0;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
}

void WriteTh07StageAnmVmI32Memory(u8 *bytes, i32 value)
{
    std::memcpy(bytes, &value, sizeof(value));
}

void WriteTh07StageAnmVmI16(Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset, i16 value)
{
    std::memcpy(reinterpret_cast<u8 *>(slot) + offset, &value, sizeof(value));
}

i32 Th07StageAnmVmI32FromU32(u32 value)
{
    return std::bit_cast<i32>(value);
}

i32 AddTh07StageAnmVmI32(i32 lhs, i32 rhs)
{
    return Th07StageAnmVmI32FromU32(static_cast<u32>(lhs) + static_cast<u32>(rhs));
}

i32 SubtractTh07StageAnmVmI32(i32 lhs, i32 rhs)
{
    return Th07StageAnmVmI32FromU32(static_cast<u32>(lhs) - static_cast<u32>(rhs));
}

i32 MultiplyTh07StageAnmVmI32(i32 lhs, i32 rhs)
{
    return Th07StageAnmVmI32FromU32(static_cast<u32>(lhs) * static_cast<u32>(rhs));
}

bool CanDivideTh07StageAnmVmI32(i32 lhs, i32 rhs)
{
    return rhs != 0 && !(lhs == std::numeric_limits<i32>::min() && rhs == -1);
}

f32 ApplyTh07StageAnmVmFmod(f32 lhs, f32 rhs)
{
    return static_cast<f32>(std::fmod(static_cast<double>(lhs), static_cast<double>(rhs)));
}

f32 ApplyTh07StageAnmVmSin(f32 value)
{
    return static_cast<f32>(std::sin(static_cast<double>(value)));
}

f32 ApplyTh07StageAnmVmCos(f32 value)
{
    return static_cast<f32>(std::cos(static_cast<double>(value)));
}

f32 ApplyTh07StageAnmVmTan(f32 value)
{
    return static_cast<f32>(std::tan(static_cast<double>(value)));
}

f32 ApplyTh07StageAnmVmAsin(f32 value)
{
    return static_cast<f32>(std::asin(static_cast<double>(value)));
}

f32 ApplyTh07StageAnmVmAtan(f32 value)
{
    return static_cast<f32>(std::atan(static_cast<double>(value)));
}

void WriteTh07StageAnmVmByte(Th07StageOriginalAnmVmSlot *slot, std::uint32_t offset, u8 value)
{
    *(reinterpret_cast<u8 *>(slot) + offset) = value;
}

bool ReadTh07AnmVmInstructionByte(std::span<const u8> instruction, std::size_t offset, u8 *outValue)
{
    if (outValue == nullptr || offset >= instruction.size())
    {
        return false;
    }
    *outValue = instruction[offset];
    return true;
}

bool ReadTh07AnmVmInstructionI16(std::span<const u8> instruction, std::size_t offset, i16 *outValue)
{
    if (outValue == nullptr || offset + sizeof(i16) > instruction.size())
    {
        return false;
    }
    std::memcpy(outValue, instruction.data() + offset, sizeof(i16));
    return true;
}

bool IsTh07AnmVmOperandReference(Th07AnmVmInstructionHeader header, std::size_t operandIndex)
{
    constexpr std::size_t kOperandFlagBits = sizeof(header.operandFlags) * 8;
    return operandIndex < kOperandFlagBits &&
           (header.operandFlags & (static_cast<std::uint16_t>(1u) << operandIndex)) != 0;
}

bool ReadTh07StageAnmVmI32Operand(Th07StageOriginalAnmVmSlot *slot,
                                  std::span<const u8> instruction,
                                  Th07AnmVmInstructionHeader header,
                                  std::size_t operandIndex,
                                  i32 *outValue)
{
    i32 literal = 0;
    if (outValue == nullptr || !ReadTh07AnmVmInstructionI32Operand(instruction, operandIndex, &literal))
    {
        return false;
    }
    if (!IsTh07AnmVmOperandReference(header, operandIndex))
    {
        *outValue = literal;
        return true;
    }

    const Th07AnmVmVariableSlot *variable = FindTh07AnmVmVariableSlot(literal);
    if (variable == nullptr)
    {
        *outValue = literal;
        return true;
    }

    if (variable->valueKind == Th07AnmVmVariableValueKind::Float)
    {
        *outValue = TruncateTh07AnmVmFloatToI32(ReadTh07StageAnmVmFloat(slot, variable->fieldOffset));
    }
    else
    {
        *outValue = ReadTh07StageAnmVmI32(slot, variable->fieldOffset);
    }
    return true;
}

bool ReadTh07StageAnmVmF32Operand(Th07StageOriginalAnmVmSlot *slot,
                                  std::span<const u8> instruction,
                                  Th07AnmVmInstructionHeader header,
                                  std::size_t operandIndex,
                                  f32 *outValue)
{
    f32 literal = 0.0f;
    if (outValue == nullptr || !ReadTh07AnmVmInstructionF32Operand(instruction, operandIndex, &literal))
    {
        return false;
    }
    if (!IsTh07AnmVmOperandReference(header, operandIndex))
    {
        *outValue = literal;
        return true;
    }

    const i32 variableId = TruncateTh07AnmVmFloatToI32(literal);
    const Th07AnmVmVariableSlot *variable = FindTh07AnmVmVariableSlot(variableId);
    if (variable == nullptr)
    {
        *outValue = literal;
        return true;
    }

    if (variable->valueKind == Th07AnmVmVariableValueKind::Float)
    {
        *outValue = ReadTh07StageAnmVmFloat(slot, variable->fieldOffset);
    }
    else
    {
        *outValue = static_cast<f32>(ReadTh07StageAnmVmI32(slot, variable->fieldOffset));
    }
    return true;
}

bool ResolveTh07StageAnmVmI32WriteTarget(Th07StageOriginalAnmVmSlot *slot,
                                         std::span<const u8> instruction,
                                         Th07AnmVmInstructionHeader header,
                                         std::size_t operandIndex,
                                         u8 **outTarget)
{
    if (outTarget == nullptr)
    {
        return false;
    }

    const std::size_t offset = Th07AnmVmInstructionOperandOffset(operandIndex);
    if (offset > instruction.size() || sizeof(i32) > instruction.size() - offset)
    {
        return false;
    }

    *outTarget = const_cast<u8 *>(instruction.data()) + offset;
    if (!IsTh07AnmVmOperandReference(header, operandIndex))
    {
        return true;
    }

    i32 variableId = 0;
    if (!ReadTh07AnmVmInstructionI32Operand(instruction, operandIndex, &variableId))
    {
        return false;
    }
    const Th07AnmVmVariableSlot *variable = FindTh07AnmVmVariableSlot(variableId);
    if (variable != nullptr && variable->valueKind == Th07AnmVmVariableValueKind::Int)
    {
        *outTarget = reinterpret_cast<u8 *>(slot) + variable->fieldOffset;
    }
    return true;
}

bool ResolveTh07StageAnmVmF32WriteTarget(Th07StageOriginalAnmVmSlot *slot,
                                         std::span<const u8> instruction,
                                         Th07AnmVmInstructionHeader header,
                                         std::size_t operandIndex,
                                         u8 **outTarget)
{
    if (outTarget == nullptr)
    {
        return false;
    }

    const std::size_t offset = Th07AnmVmInstructionOperandOffset(operandIndex);
    if (offset > instruction.size() || sizeof(f32) > instruction.size() - offset)
    {
        return false;
    }

    *outTarget = const_cast<u8 *>(instruction.data()) + offset;
    if (!IsTh07AnmVmOperandReference(header, operandIndex))
    {
        return true;
    }

    f32 variableIdValue = 0.0f;
    if (!ReadTh07AnmVmInstructionF32Operand(instruction, operandIndex, &variableIdValue))
    {
        return false;
    }
    const Th07AnmVmVariableSlot *variable =
        FindTh07AnmVmVariableSlot(TruncateTh07AnmVmFloatToI32(variableIdValue));
    if (variable != nullptr && variable->valueKind == Th07AnmVmVariableValueKind::Float)
    {
        *outTarget = reinterpret_cast<u8 *>(slot) + variable->fieldOffset;
    }
    return true;
}

bool WriteTh07StageAnmVmI32Operand(Th07StageOriginalAnmVmSlot *slot,
                                   std::span<const u8> instruction,
                                   Th07AnmVmInstructionHeader header,
                                   std::size_t operandIndex,
                                   i32 value)
{
    u8 *target = nullptr;
    if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, operandIndex, &target))
    {
        return false;
    }
    WriteTh07StageAnmVmI32Memory(target, value);
    return true;
}

bool WriteTh07StageAnmVmF32Operand(Th07StageOriginalAnmVmSlot *slot,
                                   std::span<const u8> instruction,
                                   Th07AnmVmInstructionHeader header,
                                   std::size_t operandIndex,
                                   f32 value)
{
    u8 *target = nullptr;
    if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, operandIndex, &target))
    {
        return false;
    }
    WriteTh07StageAnmVmF32Memory(target, value);
    return true;
}

bool ApplyTh07StageAnmVmUnaryMathInstruction(Th07StageOriginalAnmVmSlot *slot,
                                             std::span<const u8> instruction,
                                             Th07AnmVmInstructionHeader header,
                                             f32 (*operation)(f32))
{
    f32 floatSource = 0.0f;
    return operation != nullptr &&
           ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
           WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, operation(floatSource));
}

bool CompareTh07StageAnmVmI32Condition(Th07AnmVmConditionalJumpComparison comparison, i32 lhs, i32 rhs)
{
    switch (comparison)
    {
    case Th07AnmVmConditionalJumpComparison::Equal:
        return lhs == rhs;
    case Th07AnmVmConditionalJumpComparison::NotEqual:
        return lhs != rhs;
    case Th07AnmVmConditionalJumpComparison::LessThan:
        return lhs < rhs;
    case Th07AnmVmConditionalJumpComparison::LessOrEqual:
        return lhs <= rhs;
    case Th07AnmVmConditionalJumpComparison::GreaterThan:
        return lhs > rhs;
    case Th07AnmVmConditionalJumpComparison::GreaterOrEqual:
        return lhs >= rhs;
    }
    return false;
}

bool CompareTh07StageAnmVmF32Condition(Th07AnmVmConditionalJumpComparison comparison, f32 lhs, f32 rhs)
{
    switch (comparison)
    {
    case Th07AnmVmConditionalJumpComparison::Equal:
        return lhs == rhs;
    case Th07AnmVmConditionalJumpComparison::NotEqual:
        return lhs != rhs;
    case Th07AnmVmConditionalJumpComparison::LessThan:
        return lhs < rhs;
    case Th07AnmVmConditionalJumpComparison::LessOrEqual:
        return lhs <= rhs;
    case Th07AnmVmConditionalJumpComparison::GreaterThan:
        return lhs > rhs;
    case Th07AnmVmConditionalJumpComparison::GreaterOrEqual:
        return lhs >= rhs;
    }
    return false;
}

void SetTh07StageAnmVmTimer(Th07StageOriginalAnmVmSlot *slot,
                            std::uint32_t previousOffset,
                            std::uint32_t subframeOffset,
                            std::uint32_t currentOffset,
                            i32 current)
{
    WriteTh07StageAnmVmI32(slot, previousOffset, -999);
    WriteTh07StageAnmVmFloat(slot, subframeOffset, 0.0f);
    WriteTh07StageAnmVmI32(slot, currentOffset, current);
}

void DecrementTh07StageAnmVmTimer(Th07StageOriginalAnmVmSlot *slot,
                                  std::uint32_t previousOffset,
                                  std::uint32_t subframeOffset,
                                  std::uint32_t currentOffset,
                                  i32 value)
{
    ZunTimer timer;
    timer.previous = ReadTh07StageAnmVmI32(slot, previousOffset);
    timer.subFrame = ReadTh07StageAnmVmFloat(slot, subframeOffset);
    timer.current = ReadTh07StageAnmVmI32(slot, currentOffset);
    timer.Decrement(value);
    WriteTh07StageAnmVmI32(slot, previousOffset, timer.previous);
    WriteTh07StageAnmVmFloat(slot, subframeOffset, timer.subFrame);
    WriteTh07StageAnmVmI32(slot, currentOffset, timer.current);
}

void ResetTh07StageAnmVmTimerAt(Th07StageOriginalAnmVmSlot *slot, std::uint32_t timerOffset, i32 current)
{
    SetTh07StageAnmVmTimer(slot, timerOffset, timerOffset + sizeof(i32),
                           timerOffset + sizeof(i32) + sizeof(f32), current);
}

void ResetTh07StageAnmVmDurationTimer(Th07StageOriginalAnmVmSlot *slot,
                                      std::uint32_t durationOffset,
                                      i32 current)
{
    SetTh07StageAnmVmTimer(slot,
                           durationOffset - sizeof(i32) - sizeof(f32),
                           durationOffset - sizeof(i32),
                           durationOffset,
                           current);
}

void TickTh07StageAnmVmTimerAt(Th07StageOriginalAnmVmSlot *slot, std::uint32_t timerOffset)
{
    ZunTimer timer;
    timer.previous = ReadTh07StageAnmVmI32(slot, timerOffset);
    timer.subFrame = ReadTh07StageAnmVmFloat(slot, timerOffset + sizeof(i32));
    timer.current = ReadTh07StageAnmVmI32(slot, timerOffset + sizeof(i32) + sizeof(f32));
    timer.Tick();
    WriteTh07StageAnmVmI32(slot, timerOffset, timer.previous);
    WriteTh07StageAnmVmFloat(slot, timerOffset + sizeof(i32), timer.subFrame);
    WriteTh07StageAnmVmI32(slot, timerOffset + sizeof(i32) + sizeof(f32), timer.current);
}

f32 BuildTh07StageAnmVmInterpolationRatio(Th07StageOriginalAnmVmSlot *slot,
                                          const Th07AnmVmPostStepActionContract &contract)
{
    TickTh07StageAnmVmTimerAt(slot, contract.timerOffset);

    const i32 current = ReadTh07StageAnmVmI32(slot, contract.timerOffset + sizeof(i32) + sizeof(f32));
    const f32 subframe = ReadTh07StageAnmVmFloat(slot, contract.timerOffset + sizeof(i32));
    const i32 duration = ReadTh07StageAnmVmI32(slot, contract.durationOffset);
    const f32 durationSubframe = ReadTh07StageAnmVmFloat(slot, contract.durationOffset - sizeof(i32));
    if (current < duration)
    {
        return (static_cast<f32>(current) + subframe) / (static_cast<f32>(duration) + durationSubframe);
    }

    ResetTh07StageAnmVmDurationTimer(slot, contract.durationOffset, 0);
    return 1.0f;
}

f32 ApplyTh07StageAnmVmInterpolationCurve(f32 ratio, i32 interpolationMode)
{
    return ApplyTh07StageCameraInterpolationCurve(ratio, interpolationMode);
}

f32 InterpolateTh07StageAnmVmFloat(f32 initial, f32 target, f32 ratio)
{
    return (target - initial) * ratio + initial;
}

u8 InterpolateTh07StageAnmVmByte(u8 initial, u8 target, f32 ratio)
{
    const i32 value = TruncateTh07AnmVmFloatToI32(
        static_cast<f32>(static_cast<i32>(target) - static_cast<i32>(initial)) * ratio +
        static_cast<f32>(initial));
    return static_cast<u8>(value);
}

void CopyTh07StageAnmVmVector(Th07StageOriginalAnmVmSlot *slot,
                              std::uint32_t destinationOffset,
                              std::uint32_t sourceOffset)
{
    std::memcpy(reinterpret_cast<u8 *>(slot) + destinationOffset,
                reinterpret_cast<const u8 *>(slot) + sourceOffset,
                sizeof(D3DXVECTOR3));
}

void WriteTh07StageAnmVmVector(Th07StageOriginalAnmVmSlot *slot,
                               std::uint32_t offset,
                               f32 x,
                               f32 y,
                               f32 z)
{
    WriteTh07StageAnmVmFloat(slot, offset, x);
    WriteTh07StageAnmVmFloat(slot, offset + sizeof(f32), y);
    WriteTh07StageAnmVmFloat(slot, offset + 2 * sizeof(f32), z);
}

void SyncTh07StageAnmVmPositionMirror(Th07StageOriginalAnmVmSlot *slot)
{
    if ((slot->originalRenderFlags & kTh07AnmVmRenderFlagPositionTargetsAux) == 0)
    {
        slot->vm.pos = slot->originalDrawPosition;
    }
    else
    {
        slot->vm.posOffset = slot->originalVector230;
    }
}

void SyncTh07StageAnmVmColorMirror(Th07StageOriginalAnmVmSlot *slot)
{
    slot->vm.color = slot->originalColor;
}

bool ApplyTh07StageAnmVmConditionalJumpInstruction(Th07StageOriginalAnmVmSlot *slot,
                                                  std::span<const u8> instruction,
                                                  Th07AnmVmInstructionHeader header,
                                                  bool *outJumped)
{
    if (outJumped != nullptr)
    {
        *outJumped = false;
    }

    const Th07AnmVmConditionalJumpOpcodeContract *contract =
        FindTh07AnmVmConditionalJumpExecuteOpcode(header.opcode);
    if (contract == nullptr)
    {
        return false;
    }

    bool shouldJump = false;
    if (contract->valueKind == Th07AnmVmVariableValueKind::Int)
    {
        i32 lhs = 0;
        i32 rhs = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &lhs) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &rhs))
        {
            return false;
        }
        shouldJump = CompareTh07StageAnmVmI32Condition(contract->comparison, lhs, rhs);
    }
    else
    {
        f32 lhs = 0.0f;
        f32 rhs = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &lhs) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &rhs))
        {
            return false;
        }
        shouldJump = CompareTh07StageAnmVmF32Condition(contract->comparison, lhs, rhs);
    }

    if (!shouldJump)
    {
        return true;
    }

    i32 relativeOffset = 0;
    i32 frame = 0;
    if (!ReadTh07AnmVmInstructionI32Operand(instruction, contract->jumpOffsetOperandIndex, &relativeOffset) ||
        !ReadTh07AnmVmInstructionI32Operand(instruction, contract->jumpFrameOperandIndex, &frame))
    {
        return false;
    }
    slot->vm.currentTimeInScript.SetCurrent(frame);
    SetTh07StageAnmVmCurrentInstruction(slot,
                                        Th07StageAnmVmInstructionBytes(slot->originalBeginningOfScript) +
                                            relativeOffset);
    if (outJumped != nullptr)
    {
        *outJumped = true;
    }
    return true;
}

bool StartTh07StageAnmVmPositionInterpolation(Th07StageOriginalAnmVmSlot *slot,
                                              std::span<const u8> instruction,
                                              Th07AnmVmInstructionHeader header,
                                              i32 interpolationMode,
                                              std::size_t durationOperandIndex,
                                              std::size_t targetOperandIndex)
{
    i32 duration = 0;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, durationOperandIndex, &duration) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex, &x) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex + 1, &y) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex + 2, &z))
    {
        return false;
    }

    ResetTh07StageAnmVmTimerAt(slot, kTh07AnmVmPositionInterpolationTimerOffset, 0);
    ResetTh07StageAnmVmDurationTimer(slot, kTh07AnmVmPositionInterpolationDurationOffset, duration);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmPositionInterpolationModeOffset, static_cast<u8>(interpolationMode));
    CopyTh07StageAnmVmVector(slot,
                             kTh07AnmVmPositionInterpolationInitialOffset,
                             (slot->originalRenderFlags & kTh07AnmVmRenderFlagPositionTargetsAux) == 0 ?
                                 kTh07AnmVmDrawPositionOffset :
                                 kTh07AnmVmAuxVectorOffset);
    WriteTh07StageAnmVmVector(slot, kTh07AnmVmPositionInterpolationTargetOffset, x, y, z);
    return true;
}

bool StartTh07StageAnmVmVisibilityInterpolation(Th07StageOriginalAnmVmSlot *slot,
                                                std::span<const u8> instruction,
                                                Th07AnmVmInstructionHeader header,
                                                i32 interpolationMode,
                                                std::size_t durationOperandIndex,
                                                std::size_t targetByteOffset)
{
    i32 duration = 0;
    u8 target = 0;
    if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, durationOperandIndex, &duration) ||
        !ReadTh07AnmVmInstructionByte(instruction, targetByteOffset, &target))
    {
        return false;
    }

    ResetTh07StageAnmVmTimerAt(slot, kTh07AnmVmVisibilityInterpolationTimerOffset, 0);
    ResetTh07StageAnmVmDurationTimer(slot, kTh07AnmVmVisibilityInterpolationDurationOffset, duration);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityInterpolationModeOffset, static_cast<u8>(interpolationMode));
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityInterpolationInitialOffset,
                            ReadTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityByteOffset));
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityInterpolationTargetOffset, target);
    return true;
}

bool StartTh07StageAnmVmColorInterpolation(Th07StageOriginalAnmVmSlot *slot,
                                           std::span<const u8> instruction,
                                           Th07AnmVmInstructionHeader header,
                                           i32 interpolationMode,
                                           std::size_t durationOperandIndex,
                                           std::size_t targetByteOffset)
{
    i32 duration = 0;
    u8 targetB = 0;
    u8 targetG = 0;
    u8 targetR = 0;
    if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, durationOperandIndex, &duration) ||
        !ReadTh07AnmVmInstructionByte(instruction, targetByteOffset, &targetB) ||
        !ReadTh07AnmVmInstructionByte(instruction, targetByteOffset + 1, &targetG) ||
        !ReadTh07AnmVmInstructionByte(instruction, targetByteOffset + 2, &targetR))
    {
        return false;
    }

    ResetTh07StageAnmVmTimerAt(slot, kTh07AnmVmColorInterpolationTimerOffset, 0);
    ResetTh07StageAnmVmDurationTimer(slot, kTh07AnmVmColorInterpolationDurationOffset, duration);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationModeOffset, static_cast<u8>(interpolationMode));
    for (std::uint32_t idx = 0; idx < 3; idx++)
    {
        WriteTh07StageAnmVmByte(slot,
                                kTh07AnmVmColorInterpolationInitialOffset + idx,
                                ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorOffset + idx));
    }
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset, targetB);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset + 1, targetG);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset + 2, targetR);
    return true;
}

bool StartTh07StageAnmVmRotationInterpolation(Th07StageOriginalAnmVmSlot *slot,
                                              std::span<const u8> instruction,
                                              Th07AnmVmInstructionHeader header,
                                              i32 interpolationMode,
                                              std::size_t durationOperandIndex,
                                              std::size_t targetOperandIndex)
{
    i32 duration = 0;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, durationOperandIndex, &duration) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex, &x) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex + 1, &y) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex + 2, &z))
    {
        return false;
    }

    ResetTh07StageAnmVmTimerAt(slot, kTh07AnmVmRotationInterpolationTimerOffset, 0);
    ResetTh07StageAnmVmDurationTimer(slot, kTh07AnmVmRotationInterpolationDurationOffset, duration);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmRotationInterpolationModeOffset, static_cast<u8>(interpolationMode));
    CopyTh07StageAnmVmVector(slot, kTh07AnmVmRotationInterpolationInitialOffset, kTh07AnmVmRotationOffset);
    WriteTh07StageAnmVmVector(slot, kTh07AnmVmRotationInterpolationTargetOffset, x, y, z);
    slot->originalRenderFlags |= kTh07AnmVmRenderFlagRotationDirty;
    return true;
}

bool StartTh07StageAnmVmScaleInterpolation(Th07StageOriginalAnmVmSlot *slot,
                                           std::span<const u8> instruction,
                                           Th07AnmVmInstructionHeader header,
                                           i32 interpolationMode,
                                           std::size_t durationOperandIndex,
                                           std::size_t targetOperandIndex)
{
    i32 duration = 0;
    f32 x = 0.0f;
    f32 y = 0.0f;
    if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, durationOperandIndex, &duration) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex, &x) ||
        !ReadTh07StageAnmVmF32Operand(slot, instruction, header, targetOperandIndex + 1, &y))
    {
        return false;
    }

    ResetTh07StageAnmVmTimerAt(slot, kTh07AnmVmScaleInterpolationTimerOffset, 0);
    ResetTh07StageAnmVmDurationTimer(slot, kTh07AnmVmScaleInterpolationDurationOffset, duration);
    WriteTh07StageAnmVmByte(slot, kTh07AnmVmScaleInterpolationModeOffset, static_cast<u8>(interpolationMode));
    WriteTh07StageAnmVmFloat(slot,
                             kTh07AnmVmScaleInterpolationInitialXOffset,
                             ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset));
    WriteTh07StageAnmVmFloat(slot,
                             kTh07AnmVmScaleInterpolationInitialYOffset,
                             ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset));
    WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationFinalXOffset, x);
    WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationFinalYOffset, y);
    slot->originalRenderFlags |= kTh07AnmVmRenderFlagScaleDirty;
    return true;
}

bool ApplyTh07StageAnmVmStopUntilInterruptInstruction(Th07StageOriginalAnmVmSlot *slot,
                                                      bool clearActive,
                                                      bool *outStopAfterInstruction)
{
    if (clearActive)
    {
        slot->originalRenderFlags &= ~kTh07AnmVmRenderFlagActive;
        slot->vm.flags.isVisible = 0;
    }

    if (slot->originalPendingInterrupt == 0)
    {
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagInterruptPending;
        slot->vm.currentTimeInScript.Decrement(1);
        if (outStopAfterInstruction != nullptr)
        {
            *outStopAfterInstruction = true;
        }
    }
    return true;
}

bool ApplyTh07StageAnmVmFollowupInstruction(Th07StageOriginalAnmVmSlot *slot,
                                            std::span<const u8> instruction,
                                            Th07AnmVmInstructionHeader header,
                                            bool *outStopAfterInstruction)
{
    const Th07AnmVmFollowupOpcodeContract *contract = FindTh07AnmVmFollowupExecuteOpcode(header.opcode);
    if (contract == nullptr)
    {
        return false;
    }

    u8 mode = 0;
    switch (contract->semantic)
    {
    case Th07AnmVmFollowupOpcodeSemantic::SetScaleVelocity:
    {
        f32 x = 0.0f;
        f32 y = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &x) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &y))
        {
            return false;
        }
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleVelocityXOffset, x);
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleVelocityYOffset, y);
        return true;
    }
    case Th07AnmVmFollowupOpcodeSemantic::StartVisibilityInterpolation:
        if (contract->interpolationMode == kTh07AnmVmInterpolationModeFromInstruction)
        {
            if (!ReadTh07AnmVmInstructionByte(instruction, Th07AnmVmInstructionOperandOffset(1), &mode))
            {
                return false;
            }
            return StartTh07StageAnmVmVisibilityInterpolation(slot, instruction, header, mode, 0,
                                                              Th07AnmVmInstructionOperandOffset(2));
        }
        return StartTh07StageAnmVmVisibilityInterpolation(slot, instruction, header, contract->interpolationMode, 1,
                                                          Th07AnmVmInstructionOperandOffset(0));
    case Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit4:
    case Th07AnmVmFollowupOpcodeSemantic::SetPositionTargetsAux:
    case Th07AnmVmFollowupOpcodeSemantic::SetActiveRenderFlag:
    case Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit12:
    case Th07AnmVmFollowupOpcodeSemantic::SetRenderFlagBit14:
    {
        i32 enabled = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &enabled))
        {
            return false;
        }
        slot->originalRenderFlags = (slot->originalRenderFlags & ~contract->controlledRenderFlag) |
                                    ((static_cast<u32>(enabled) & 1u) *
                                     contract->controlledRenderFlag);
        if (contract->controlledRenderFlag == kTh07AnmVmRenderFlagActive)
        {
            slot->vm.flags.isVisible = (slot->originalRenderFlags & kTh07AnmVmRenderFlagActive) != 0;
        }
        return true;
    }
    case Th07AnmVmFollowupOpcodeSemantic::StartPositionInterpolation:
        if (contract->interpolationMode == kTh07AnmVmInterpolationModeFromInstruction)
        {
            if (!ReadTh07AnmVmInstructionByte(instruction, Th07AnmVmInstructionOperandOffset(1), &mode))
            {
                return false;
            }
            return StartTh07StageAnmVmPositionInterpolation(slot, instruction, header, mode, 0, 2);
        }
        return StartTh07StageAnmVmPositionInterpolation(slot, instruction, header, contract->interpolationMode, 3, 0);
    case Th07AnmVmFollowupOpcodeSemantic::StopUntilInterrupt:
        return ApplyTh07StageAnmVmStopUntilInterruptInstruction(slot, false, outStopAfterInstruction);
    case Th07AnmVmFollowupOpcodeSemantic::SetAnchorTopLeft:
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagAnchorTopLeft;
        slot->vm.flags.anchor = AnmVmAnchor_TopLeft;
        return true;
    case Th07AnmVmFollowupOpcodeSemantic::StopAndClearActiveUntilInterrupt:
        return ApplyTh07StageAnmVmStopUntilInterruptInstruction(slot, true, outStopAfterInstruction);
    case Th07AnmVmFollowupOpcodeSemantic::SetField1c4:
    {
        i16 value = 0;
        if (!ReadTh07AnmVmInstructionI16(instruction, Th07AnmVmInstructionOperandOffset(0), &value))
        {
            return false;
        }
        WriteTh07StageAnmVmI16(slot, contract->primaryFieldOffset, value);
        return true;
    }
    case Th07AnmVmFollowupOpcodeSemantic::AddAngleField28:
    case Th07AnmVmFollowupOpcodeSemantic::AddAngleField2c:
    {
        f32 value = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &value))
        {
            return false;
        }
        WriteTh07StageAnmVmFloat(slot, contract->primaryFieldOffset,
                                 NormalizeTh07EffectAngle(ReadTh07StageAnmVmFloat(slot,
                                                                                   contract->primaryFieldOffset) +
                                                          value));
        return true;
    }
    case Th07AnmVmFollowupOpcodeSemantic::StartScaleInterpolation:
        if (contract->interpolationMode == kTh07AnmVmInterpolationModeFromInstruction)
        {
            if (!ReadTh07AnmVmInstructionByte(instruction, Th07AnmVmInstructionOperandOffset(1), &mode))
            {
                return false;
            }
            return StartTh07StageAnmVmScaleInterpolation(slot, instruction, header, mode, 0, 2);
        }
        return StartTh07StageAnmVmScaleInterpolation(slot, instruction, header, contract->interpolationMode, 2, 0);
    case Th07AnmVmFollowupOpcodeSemantic::StartColorInterpolation:
        if (!ReadTh07AnmVmInstructionByte(instruction, Th07AnmVmInstructionOperandOffset(1), &mode))
        {
            return false;
        }
        return StartTh07StageAnmVmColorInterpolation(slot, instruction, header, mode, 0,
                                                     Th07AnmVmInstructionOperandOffset(2));
    case Th07AnmVmFollowupOpcodeSemantic::StartRotationInterpolation:
        if (!ReadTh07AnmVmInstructionByte(instruction, Th07AnmVmInstructionOperandOffset(1), &mode))
        {
            return false;
        }
        return StartTh07StageAnmVmRotationInterpolation(slot, instruction, header, mode, 0, 2);
    }
    return false;
}

void AdvanceTh07StageAnmVmInterpolationLane(Th07StageOriginalAnmVmSlot *slot,
                                            const Th07AnmVmPostStepActionContract &contract)
{
    if (ReadTh07StageAnmVmI32(slot, contract.durationOffset) <= 0)
    {
        return;
    }

    const f32 ratio = ApplyTh07StageAnmVmInterpolationCurve(
        BuildTh07StageAnmVmInterpolationRatio(slot, contract),
        ReadTh07StageAnmVmByte(slot, contract.modeOffset));

    switch (contract.interpolationLane)
    {
    case Th07AnmVmInterpolationLane::Position:
    {
        const f32 x = InterpolateTh07StageAnmVmFloat(ReadTh07StageAnmVmFloat(slot, contract.secondaryFieldOffset),
                                                     ReadTh07StageAnmVmFloat(slot, contract.targetFieldOffset),
                                                     ratio);
        const f32 y = InterpolateTh07StageAnmVmFloat(ReadTh07StageAnmVmFloat(slot,
                                                                             contract.secondaryFieldOffset +
                                                                                 sizeof(f32)),
                                                     ReadTh07StageAnmVmFloat(slot,
                                                                             contract.targetFieldOffset +
                                                                                 sizeof(f32)),
                                                     ratio);
        const f32 z = InterpolateTh07StageAnmVmFloat(ReadTh07StageAnmVmFloat(slot,
                                                                             contract.secondaryFieldOffset +
                                                                                 2 * sizeof(f32)),
                                                     ReadTh07StageAnmVmFloat(slot,
                                                                             contract.targetFieldOffset +
                                                                                 2 * sizeof(f32)),
                                                     ratio);
        WriteTh07StageAnmVmVector(slot,
                                  (slot->originalRenderFlags & kTh07AnmVmRenderFlagPositionTargetsAux) == 0 ?
                                      kTh07AnmVmDrawPositionOffset :
                                      kTh07AnmVmAuxVectorOffset,
                                  x,
                                  y,
                                  z);
        SyncTh07StageAnmVmPositionMirror(slot);
        break;
    }
    case Th07AnmVmInterpolationLane::Color:
        WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorOffset + 2,
                                InterpolateTh07StageAnmVmByte(
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationInitialOffset + 2),
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset + 2),
                                    ratio));
        WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorOffset + 1,
                                InterpolateTh07StageAnmVmByte(
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationInitialOffset + 1),
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset + 1),
                                    ratio));
        WriteTh07StageAnmVmByte(slot, kTh07AnmVmColorOffset,
                                InterpolateTh07StageAnmVmByte(
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationInitialOffset),
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmColorInterpolationTargetOffset),
                                    ratio));
        SyncTh07StageAnmVmColorMirror(slot);
        break;
    case Th07AnmVmInterpolationLane::Visibility:
        WriteTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityByteOffset,
                                InterpolateTh07StageAnmVmByte(
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityInterpolationInitialOffset),
                                    ReadTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityInterpolationTargetOffset),
                                    ratio));
        SyncTh07StageAnmVmColorMirror(slot);
        break;
    case Th07AnmVmInterpolationLane::Rotation:
        for (std::uint32_t idx = 0; idx < 3; idx++)
        {
            const std::uint32_t fieldOffset = kTh07AnmVmRotationOffset + idx * sizeof(f32);
            const std::uint32_t initialOffset = contract.secondaryFieldOffset + idx * sizeof(f32);
            const std::uint32_t targetOffset = contract.targetFieldOffset + idx * sizeof(f32);
            WriteTh07StageAnmVmFloat(slot, fieldOffset,
                                     NormalizeTh07EffectAngle(
                                         InterpolateTh07StageAnmVmFloat(ReadTh07StageAnmVmFloat(slot, initialOffset),
                                                                        ReadTh07StageAnmVmFloat(slot, targetOffset),
                                                                        ratio)));
        }
        slot->originalRenderFlags |= contract.setRenderFlags;
        break;
    case Th07AnmVmInterpolationLane::Scale:
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset,
                                 InterpolateTh07StageAnmVmFloat(
                                     ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationInitialXOffset),
                                     ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationFinalXOffset),
                                     ratio));
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset,
                                 InterpolateTh07StageAnmVmFloat(
                                     ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationInitialYOffset),
                                     ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleInterpolationFinalYOffset),
                                     ratio));
        slot->originalRenderFlags |= contract.setRenderFlags;
        break;
    case Th07AnmVmInterpolationLane::None:
        break;
    }
}

bool ApplyTh07StageAnmVmWaitTimerInstruction(Th07StageOriginalAnmVmSlot *slot,
                                             std::span<const u8> instruction,
                                             Th07AnmVmInstructionHeader header,
                                             bool *outStopAfterInstruction)
{
    const Th07AnmVmControlOpcodeContract *contract = FindTh07AnmVmControlExecuteOpcode(header.opcode);
    if (contract == nullptr || contract->semantic != Th07AnmVmControlOpcodeSemantic::WaitTimer)
    {
        return false;
    }

    if (ReadTh07StageAnmVmI32(slot, contract->timerCurrentOffset) == 0)
    {
        i32 duration = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &duration))
        {
            return false;
        }
        SetTh07StageAnmVmTimer(slot,
                               contract->timerPreviousOffset,
                               contract->timerSubframeOffset,
                               contract->timerCurrentOffset,
                               duration);
    }
    else
    {
        DecrementTh07StageAnmVmTimer(slot,
                                     contract->timerPreviousOffset,
                                     contract->timerSubframeOffset,
                                     contract->timerCurrentOffset,
                                     1);
    }

    if (ReadTh07StageAnmVmI32(slot, contract->timerCurrentOffset) <= 0)
    {
        SetTh07StageAnmVmTimer(slot,
                               contract->timerPreviousOffset,
                               contract->timerSubframeOffset,
                               contract->timerCurrentOffset,
                               0);
        return true;
    }

    slot->vm.currentTimeInScript.Decrement(1);
    if (outStopAfterInstruction != nullptr)
    {
        *outStopAfterInstruction = true;
    }
    return true;
}

bool ApplyTh07StageAnmVmVariableInstruction(Th07StageOriginalAnmVmSlot *slot,
                                            std::span<const u8> instruction,
                                            Th07AnmVmInstructionHeader header)
{
    u8 *target = nullptr;
    i32 intSource = 0;
    i32 intSource2 = 0;
    f32 floatSource = 0.0f;
    f32 floatSource2 = 0.0f;

    switch (header.opcode)
    {
    case 0x25:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0, intSource);
    case 0x26:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, floatSource);
    case 0x27:
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(target, AddTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(target),
                                                                  intSource));
        return true;
    case 0x28:
        if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource))
        {
            return false;
        }
        WriteTh07StageAnmVmF32Memory(target, ReadTh07StageAnmVmF32Memory(target) + floatSource);
        return true;
    case 0x29:
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(target, SubtractTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(target),
                                                                       intSource));
        return true;
    case 0x2a:
        if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource))
        {
            return false;
        }
        WriteTh07StageAnmVmF32Memory(target, ReadTh07StageAnmVmF32Memory(target) - floatSource);
        return true;
    case 0x2b:
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(target, MultiplyTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(target),
                                                                       intSource));
        return true;
    case 0x2c:
        if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource))
        {
            return false;
        }
        WriteTh07StageAnmVmF32Memory(target, ReadTh07StageAnmVmF32Memory(target) * floatSource);
        return true;
    case 0x2d:
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) ||
            !CanDivideTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(target), intSource))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(target, ReadTh07StageAnmVmI32Memory(target) / intSource);
        return true;
    case 0x2e:
        if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource))
        {
            return false;
        }
        WriteTh07StageAnmVmF32Memory(target, ReadTh07StageAnmVmF32Memory(target) / floatSource);
        return true;
    case 0x2f:
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) ||
            !CanDivideTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(target), intSource))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(target, ReadTh07StageAnmVmI32Memory(target) % intSource);
        return true;
    case 0x30:
        if (!ResolveTh07StageAnmVmF32WriteTarget(slot, instruction, header, 0, &target) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource))
        {
            return false;
        }
        WriteTh07StageAnmVmF32Memory(target,
                                     ApplyTh07StageAnmVmFmod(ReadTh07StageAnmVmF32Memory(target),
                                                             floatSource));
        return true;
    case 0x31:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               ReadTh07StageAnmVmI32Operand(slot, instruction, header, 2, &intSource2) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0,
                                             AddTh07StageAnmVmI32(intSource, intSource2));
    case 0x32:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &floatSource2) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, floatSource + floatSource2);
    case 0x33:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               ReadTh07StageAnmVmI32Operand(slot, instruction, header, 2, &intSource2) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0,
                                             SubtractTh07StageAnmVmI32(intSource, intSource2));
    case 0x34:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &floatSource2) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, floatSource - floatSource2);
    case 0x35:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               ReadTh07StageAnmVmI32Operand(slot, instruction, header, 2, &intSource2) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0,
                                             MultiplyTh07StageAnmVmI32(intSource, intSource2));
    case 0x36:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &floatSource2) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, floatSource * floatSource2);
    case 0x37:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               ReadTh07StageAnmVmI32Operand(slot, instruction, header, 2, &intSource2) &&
               CanDivideTh07StageAnmVmI32(intSource, intSource2) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0, intSource / intSource2);
    case 0x38:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &floatSource2) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0, floatSource / floatSource2);
    case 0x39:
        return ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource) &&
               ReadTh07StageAnmVmI32Operand(slot, instruction, header, 2, &intSource2) &&
               CanDivideTh07StageAnmVmI32(intSource, intSource2) &&
               WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0, intSource % intSource2);
    case 0x3a:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &floatSource2) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0,
                                             ApplyTh07StageAnmVmFmod(floatSource, floatSource2));
    case 0x3b:
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 1, &intSource))
        {
            return false;
        }
        return WriteTh07StageAnmVmI32Operand(slot, instruction, header, 0,
                                             intSource == 0 ? 0 :
                                                              Th07StageAnmVmI32FromU32(g_Rng.GetRandomU32() %
                                                                                       static_cast<u32>(intSource)));
    case 0x3c:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &floatSource) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0,
                                             g_Rng.GetRandomF32ZeroToOne() * floatSource);
    case 0x3d:
        return ApplyTh07StageAnmVmUnaryMathInstruction(slot, instruction, header,
                                                       ApplyTh07StageAnmVmSin);
    case 0x3e:
        return ApplyTh07StageAnmVmUnaryMathInstruction(slot, instruction, header,
                                                       ApplyTh07StageAnmVmCos);
    case 0x3f:
        return ApplyTh07StageAnmVmUnaryMathInstruction(slot, instruction, header,
                                                       ApplyTh07StageAnmVmTan);
    case 0x40:
        return ApplyTh07StageAnmVmUnaryMathInstruction(slot, instruction, header,
                                                       ApplyTh07StageAnmVmAsin);
    case 0x41:
        return ApplyTh07StageAnmVmUnaryMathInstruction(slot, instruction, header,
                                                       ApplyTh07StageAnmVmAtan);
    case 0x42:
        return ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &floatSource) &&
               WriteTh07StageAnmVmF32Operand(slot, instruction, header, 0,
                                             NormalizeTh07EffectAngle(floatSource));
    default:
        return false;
    }
}

void AdvanceTh07StageAnmVmPostStep(Th07StageOriginalAnmVmSlot *slot)
{
    const f32 multiplier = g_Supervisor.effectiveFramerateMultiplier;

    for (std::uint32_t idx = 0; idx < 3; idx++)
    {
        const std::uint32_t fieldOffset = kTh07AnmVmRotationOffset + idx * sizeof(f32);
        const std::uint32_t velocityOffset = kTh07AnmVmAngularVelocityOffset + idx * sizeof(f32);
        const f32 velocity = ReadTh07StageAnmVmFloat(slot, velocityOffset);
        if (velocity != 0.0f)
        {
            WriteTh07StageAnmVmFloat(slot, fieldOffset,
                                     NormalizeTh07EffectAngle(ReadTh07StageAnmVmFloat(slot, fieldOffset) +
                                                              multiplier * velocity));
            slot->originalRenderFlags |= kTh07AnmVmRenderFlagRotationDirty;
        }
    }

    for (const Th07AnmVmPostStepActionContract &contract : kTh07AnmVmPostStepActions)
    {
        if (contract.semantic == Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane)
        {
            AdvanceTh07StageAnmVmInterpolationLane(slot, contract);
        }
    }

    const f32 scaleVelocityX = ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleVelocityXOffset);
    const f32 scaleVelocityY = ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleVelocityYOffset);
    if (scaleVelocityX != 0.0f || scaleVelocityY != 0.0f)
    {
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset,
                                 ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset) +
                                     multiplier * scaleVelocityX);
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset,
                                 ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset) +
                                     multiplier * scaleVelocityY);
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagScaleDirty | kTh07AnmVmRenderFlagRotationDirty;
    }

    const f32 angleVelocity28 = ReadTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField28VelocityOffset);
    const f32 angleVelocity2c = ReadTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField2cVelocityOffset);
    if (angleVelocity28 != 0.0f)
    {
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField28Offset,
                                 NormalizeTh07EffectAngle(ReadTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField28Offset) +
                                                          multiplier * angleVelocity28));
    }
    if (angleVelocity2c != 0.0f)
    {
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField2cOffset,
                                 NormalizeTh07EffectAngle(ReadTh07StageAnmVmFloat(slot, kTh07AnmVmAngleField2cOffset) +
                                                          multiplier * angleVelocity2c));
    }

    slot->vm.currentTimeInScript.Tick();
}

enum class Th07StageAnmVmPendingInterruptResult {
    None,
    Dispatched,
    NoMatchingLabel,
};

Th07StageAnmVmPendingInterruptResult ApplyTh07StageAnmVmPendingInterrupt(Th07StageOriginalAnmVmSlot *slot)
{
    const i16 pendingInterrupt = slot->originalPendingInterrupt;
    if (pendingInterrupt == 0)
    {
        return Th07StageAnmVmPendingInterruptResult::None;
    }

    const u8 *matchedLabel = nullptr;
    const u8 *cursor = Th07StageAnmVmInstructionBytes(slot->originalBeginningOfScript);
    for (i32 guard = 0; cursor != nullptr && guard < 4096; guard++)
    {
        Th07AnmVmInstructionHeader header = {};
        if (!ReadTh07AnmVmInstructionHeader(std::span<const u8>(cursor,
                                                                kTh07AnmVmInstructionHeaderSize),
                                            &header))
        {
            break;
        }
        std::span<const u8> instruction(cursor, header.size);
        if (header.opcode == kTh07AnmVmInterruptLabelOpcode)
        {
            i32 label = 0;
            if (ReadTh07AnmVmInstructionI32Operand(instruction, 0, &label))
            {
                if (label == pendingInterrupt)
                {
                    matchedLabel = cursor;
                    break;
                }
                if (label == kTh07AnmVmFallbackInterruptLabel)
                {
                    matchedLabel = cursor;
                }
            }
        }
        if (header.opcode == -1)
        {
            break;
        }
        cursor += header.size;
    }

    slot->originalPendingInterrupt = 0;
    slot->originalRenderFlags &= ~kTh07AnmVmRenderFlagInterruptPending;
    if (matchedLabel == nullptr)
    {
        slot->vm.currentTimeInScript.Decrement(1);
        return Th07StageAnmVmPendingInterruptResult::NoMatchingLabel;
    }

    Th07AnmVmInstructionHeader labelHeader = {};
    if (!ReadTh07AnmVmInstructionHeader(std::span<const u8>(matchedLabel,
                                                            kTh07AnmVmInstructionHeaderSize),
                                        &labelHeader))
    {
        return Th07StageAnmVmPendingInterruptResult::NoMatchingLabel;
    }
    const u8 *targetInstruction = matchedLabel + labelHeader.size;
    Th07AnmVmInstructionHeader targetHeader = {};
    if (!ReadTh07AnmVmInstructionHeader(std::span<const u8>(targetInstruction,
                                                            kTh07AnmVmInstructionHeaderSize),
                                        &targetHeader))
    {
        return Th07StageAnmVmPendingInterruptResult::NoMatchingLabel;
    }

    SetTh07StageAnmVmCurrentInstruction(slot, targetInstruction);
    slot->vm.currentTimeInScript.SetCurrent(targetHeader.time);
    slot->originalRenderFlags |= kTh07AnmVmRenderFlagActive;
    slot->vm.flags.isVisible = 1;
    return Th07StageAnmVmPendingInterruptResult::Dispatched;
}

bool ApplyTh07StageAnmVmInstruction(Th07StageOriginalAnmVmSlot *slot,
                                    const u8 *instructionBytes,
                                    Th07AnmVmInstructionHeader header,
                                    bool *outStopAfterInstruction)
{
    if (outStopAfterInstruction != nullptr)
    {
        *outStopAfterInstruction = false;
    }

    std::span<const u8> instruction(instructionBytes, header.size);
    switch (header.opcode)
    {
    case -1:
    case 1:
        slot->originalRenderFlags &= ~kTh07AnmVmRenderFlagActive;
        slot->vm.flags.isVisible = 0;
        SetTh07StageAnmVmCurrentInstruction(slot, nullptr);
        return true;
    case 0:
        break;
    case 2:
        SetTh07StageAnmVmCurrentInstruction(slot, nullptr);
        return true;
    case 3:
    {
        i32 spriteIndex = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &spriteIndex))
        {
            return false;
        }
        const i32 scriptIndex = slot->originalScriptIndex;
        if (g_AnmManager->SetActiveSprite(&slot->vm, spriteIndex + g_AnmManager->spriteIndices[scriptIndex]) ==
            ZUN_SUCCESS)
        {
            slot->originalRenderFlags |= kTh07AnmVmRenderFlagActive;
            slot->vm.flags.isVisible = 1;
            slot->originalLastSpriteSetFrame = slot->vm.currentTimeInScript.AsFrames();
            RefreshTh07StageAnmVmActiveSprite(slot);
        }
        break;
    }
    case 4:
    {
        i32 relativeOffset = 0;
        i32 frame = 0;
        if (!ReadTh07AnmVmInstructionI32Operand(instruction, 0, &relativeOffset) ||
            !ReadTh07AnmVmInstructionI32Operand(instruction, 1, &frame))
        {
            return false;
        }
        slot->vm.currentTimeInScript.SetCurrent(frame);
        SetTh07StageAnmVmCurrentInstruction(
            slot, Th07StageAnmVmInstructionBytes(slot->originalBeginningOfScript) + relativeOffset);
        return true;
    }
    case 5:
    {
        u8 *counterTarget = nullptr;
        i32 remaining = 0;
        i32 relativeOffset = 0;
        i32 frame = 0;
        if (!ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &counterTarget))
        {
            return false;
        }
        WriteTh07StageAnmVmI32Memory(counterTarget,
                                     SubtractTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(counterTarget), 1));
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &remaining))
        {
            return false;
        }
        if (remaining <= 0)
        {
            break;
        }
        if (!ReadTh07AnmVmInstructionI32Operand(instruction, 1, &relativeOffset) ||
            !ReadTh07AnmVmInstructionI32Operand(instruction, 2, &frame))
        {
            return false;
        }
        slot->vm.currentTimeInScript.SetCurrent(frame);
        SetTh07StageAnmVmCurrentInstruction(
            slot, Th07StageAnmVmInstructionBytes(slot->originalBeginningOfScript) + relativeOffset);
        return true;
    }
    case 6:
    {
        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &x) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &y) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &z))
        {
            return false;
        }
        if ((slot->originalRenderFlags & kTh07AnmVmRenderFlagPositionTargetsAux) == 0)
        {
            slot->originalDrawPosition = {x, y, z};
            slot->vm.pos = slot->originalDrawPosition;
        }
        else
        {
            slot->originalVector230 = {x, y, z};
            slot->vm.posOffset = slot->originalVector230;
        }
        break;
    }
    case 7:
    {
        f32 scaleX = 0.0f;
        f32 scaleY = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &scaleX) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &scaleY))
        {
            return false;
        }
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset, scaleX);
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset, scaleY);
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagScaleDirty;
        break;
    }
    case 8:
    {
        i32 alpha = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &alpha))
        {
            return false;
        }
        WriteTh07StageAnmVmByte(slot, kTh07AnmVmVisibilityByteOffset, static_cast<u8>(alpha));
        slot->vm.color = slot->originalColor;
        break;
    }
    case 9:
    {
        i32 rgb = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &rgb))
        {
            return false;
        }
        slot->originalColor = COLOR_COMBINE_ALPHA(static_cast<u32>(rgb), slot->originalColor);
        slot->vm.color = slot->originalColor;
        break;
    }
    case 0x0a:
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset,
                                 ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleXOffset) * -1.0f);
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagScaleDirty;
        break;
    case 0x0b:
        WriteTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset,
                                 ReadTh07StageAnmVmFloat(slot, kTh07AnmVmScaleYOffset) * -1.0f);
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagScaleDirty;
        break;
    case 0x0c:
    case 0x0d:
    {
        f32 x = 0.0f;
        f32 y = 0.0f;
        f32 z = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &x) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 1, &y) ||
            !ReadTh07StageAnmVmF32Operand(slot, instruction, header, 2, &z))
        {
            return false;
        }
        const std::uint32_t fieldOffset = header.opcode == 0x0c ? kTh07AnmVmRotationOffset :
                                                                 kTh07AnmVmAngularVelocityOffset;
        WriteTh07StageAnmVmFloat(slot, fieldOffset, x);
        WriteTh07StageAnmVmFloat(slot, fieldOffset + sizeof(f32), y);
        WriteTh07StageAnmVmFloat(slot, fieldOffset + 2 * sizeof(f32), z);
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagRotationDirty;
        break;
    }
    case 0x0e:
    case 0x0f:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
        if (!ApplyTh07StageAnmVmFollowupInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        if (outStopAfterInstruction != nullptr && *outStopAfterInstruction)
        {
            return true;
        }
        break;
    case 0x16:
        slot->originalRenderFlags |= kTh07AnmVmRenderFlagAnchorTopLeft;
        slot->vm.flags.anchor = AnmVmAnchor_TopLeft;
        break;
    case 0x17:
        if (!ApplyTh07StageAnmVmFollowupInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        if (outStopAfterInstruction != nullptr && *outStopAfterInstruction)
        {
            return true;
        }
        break;
    case 0x18:
    {
        i32 enabled = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &enabled))
        {
            return false;
        }
        slot->originalRenderFlags =
            (slot->originalRenderFlags & ~kTh07AnmVmRenderFlagPositionTargetsAux) |
            ((static_cast<u32>(enabled) & 1u) << 7);
        break;
    }
    case 0x19:
        if (!ApplyTh07StageAnmVmFollowupInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        break;
    case 0x1a:
    case 0x1b:
    {
        f32 value = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &value))
        {
            return false;
        }
        const std::uint32_t fieldOffset = header.opcode == 0x1a ? kTh07AnmVmAngleField28Offset :
                                                                 kTh07AnmVmAngleField2cOffset;
        WriteTh07StageAnmVmFloat(slot, fieldOffset,
                                 NormalizeTh07EffectAngle(ReadTh07StageAnmVmFloat(slot, fieldOffset) + value));
        break;
    }
    case 0x1c:
    {
        i32 enabled = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &enabled))
        {
            return false;
        }
        slot->originalRenderFlags = (slot->originalRenderFlags & ~kTh07AnmVmRenderFlagActive) |
                                    (static_cast<u32>(enabled) & 1u);
        slot->vm.flags.isVisible = (slot->originalRenderFlags & kTh07AnmVmRenderFlagActive) != 0;
        break;
    }
    case 0x1d:
        if (!ApplyTh07StageAnmVmFollowupInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        break;
    case 0x1e:
    {
        i32 enabled = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &enabled))
        {
            return false;
        }
        slot->originalRenderFlags = (slot->originalRenderFlags & ~kTh07AnmVmRenderFlagBit12) |
                                    ((static_cast<u32>(enabled) & 1u) << 12);
        break;
    }
    case 0x1f:
    {
        i32 enabled = 0;
        if (!ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &enabled))
        {
            return false;
        }
        slot->originalRenderFlags = (slot->originalRenderFlags & ~kTh07AnmVmRenderFlagBit14) |
                                    ((static_cast<u32>(enabled) & 1u) << 14);
        break;
    }
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
        if (!ApplyTh07StageAnmVmFollowupInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        break;
    case 0x4f:
        if (!ApplyTh07StageAnmVmWaitTimerInstruction(slot, instruction, header, outStopAfterInstruction))
        {
            return false;
        }
        if (outStopAfterInstruction != nullptr && *outStopAfterInstruction)
        {
            return true;
        }
        break;
    case 0x50:
    case 0x51:
    {
        f32 velocity = 0.0f;
        if (!ReadTh07StageAnmVmF32Operand(slot, instruction, header, 0, &velocity))
        {
            return false;
        }
        WriteTh07StageAnmVmFloat(slot,
                                 header.opcode == 0x50 ? kTh07AnmVmAngleField28VelocityOffset :
                                                         kTh07AnmVmAngleField2cVelocityOffset,
                                 velocity);
        break;
    }
    case 0x15:
        break;
    default:
        if (FindTh07AnmVmVariableExecuteOpcode(header.opcode) != nullptr)
        {
            if (!ApplyTh07StageAnmVmVariableInstruction(slot, instruction, header))
            {
                return false;
            }
            break;
        }
        if (FindTh07AnmVmConditionalJumpExecuteOpcode(header.opcode) != nullptr)
        {
            bool jumped = false;
            if (!ApplyTh07StageAnmVmConditionalJumpInstruction(slot, instruction, header, &jumped))
            {
                return false;
            }
            if (jumped)
            {
                return true;
            }
            break;
        }
        return false;
    }

    SetTh07StageAnmVmCurrentInstruction(slot, instructionBytes + header.size);
    return true;
}

void ExecuteTh07StageAnmVm(Th07StageOriginalAnmVmSlot *slot);

void SetTh07StageAnmVmScript(Th07StageOriginalAnmVmSlot *slot, i32 scriptIndex)
{
    slot->originalScriptIndex = scriptIndex;
    slot->originalPendingInterrupt = 0;
    slot->originalDrawPosition = {};
    slot->originalVector230 = {};
    slot->originalByte240 = 0x0f;
    slot->originalByte241 = 0x0f;
    slot->vm.flags.flip = 0;
    slot->vm.Initialize();
    slot->vm.anmFileIndex = scriptIndex;
    slot->vm.beginingOfScript = g_AnmManager->scripts[scriptIndex];
    slot->vm.currentTimeInScript.SetCurrent(0);
    slot->vm.flags.isVisible = 0;
    slot->originalColor = slot->vm.color;
    slot->originalAlternateColor = slot->vm.color;
    slot->originalRenderFlags &= ~kTh07AnmVmRenderFlagActive;
    slot->originalBeginningOfScript = slot->vm.beginingOfScript;
    SetTh07StageAnmVmCurrentInstruction(slot, Th07StageAnmVmInstructionBytes(slot->originalBeginningOfScript));
    slot->originalLoadedSprite = nullptr;
    slot->originalActiveSpriteIndex = -1;
    slot->originalBaseSpriteIndex = 0;
    slot->originalLastSpriteSetFrame = 0;
    ExecuteTh07StageAnmVm(slot);
}

void ExecuteTh07StageAnmVm(Th07StageOriginalAnmVmSlot *slot)
{
    bool shouldAdvancePostStep = slot->originalCurrentInstruction != nullptr;
    for (i32 guard = 0; slot->originalCurrentInstruction != nullptr && guard < 128; guard++)
    {
        const Th07StageAnmVmPendingInterruptResult interruptResult =
            ApplyTh07StageAnmVmPendingInterrupt(slot);
        if (interruptResult == Th07StageAnmVmPendingInterruptResult::Dispatched)
        {
            continue;
        }
        if (interruptResult == Th07StageAnmVmPendingInterruptResult::NoMatchingLabel)
        {
            break;
        }

        const u8 *instructionBytes = Th07StageAnmVmInstructionBytes(slot->originalCurrentInstruction);
        Th07AnmVmInstructionHeader header = {};
        if (!ReadTh07AnmVmInstructionHeader(std::span<const u8>(instructionBytes,
                                                                kTh07AnmVmInstructionHeaderSize),
                                            &header))
        {
            shouldAdvancePostStep = false;
            break;
        }
        if (slot->vm.currentTimeInScript.AsFrames() < header.time)
        {
            break;
        }
        bool stopAfterInstruction = false;
        if (!ApplyTh07StageAnmVmInstruction(slot, instructionBytes, header, &stopAfterInstruction))
        {
            shouldAdvancePostStep = false;
            break;
        }
        if (stopAfterInstruction)
        {
            break;
        }
        if (slot->originalCurrentInstruction == nullptr)
        {
            shouldAdvancePostStep = false;
            break;
        }
    }

    if (shouldAdvancePostStep && slot->originalCurrentInstruction != nullptr)
    {
        AdvanceTh07StageAnmVmPostStep(slot);
    }
}

void DrawTh07StageAnmVm(Th07StageOriginalAnmVmSlot *slot)
{
    if ((slot->originalRenderFlags & kTh07AnmVmRenderFlagActive) != 0 && slot->originalActiveSpriteIndex >= 0)
    {
        g_AnmManager->DrawTh07ScreenQuad(slot);
    }
}

Th07StageScriptStepResult ApplyDueTh07StageAnmVmInstruction(Stage *stage,
                                                            const RawStageInstr *instruction)
{
    const Th07StageAnmVmOpcodeEffect effect =
        BuildTh07StageAnmVmOpcodeEffect(BuildStageScriptInstruction(instruction));
    if (!effect.handled)
    {
        return Th07StageScriptStepResult::Unsupported;
    }

    if (effect.clearPrimaryActiveSprite)
    {
        stage->th07StageAnmVms[0].originalActiveSpriteIndex = -1;
    }
    if (effect.loadScript)
    {
        const i32 slotIndex = effect.slot == Th07StageAnmVmSlot::Primary ? 0 : 1;
        SetTh07StageAnmVmScript(&stage->th07StageAnmVms[slotIndex], effect.scriptIndex);
    }
    stage->th07InstructionIndex++;
    return Th07StageScriptStepResult::Applied;
}

bool ApplyDueTh07StageCameraInstruction(Stage *stage, const RawStageInstr *instruction)
{
    if (FindTh07StageCameraOpcode(instruction->opcode) == nullptr)
    {
        return false;
    }

    Th07StageCameraRuntimeState state = BuildStageCameraRuntimeState(stage);
    const Th07StageCameraInstruction cameraInstruction = {
        instruction->opcode,
        {instruction->args[0], instruction->args[1], instruction->args[2]},
    };
    const bool handled = ApplyTh07StageCameraOpcode(&state, cameraInstruction);
    if (handled)
    {
        ApplyStageCameraRuntimeState(stage, state);
    }
    return handled;
}

void AdvanceTh07StageCameraRuntime(Stage *stage)
{
    Th07StageCameraRuntimeState state = BuildStageCameraRuntimeState(stage);
    AdvanceTh07StageCameraRuntimeState(&state,
                                       g_Supervisor.framerateMultiplier,
                                       g_Supervisor.effectiveFramerateMultiplier);
    ApplyStageCameraRuntimeState(stage, state);
}

Th07StageScriptStepResult ApplyDueTh07StageScriptInstruction(Stage *stage,
                                                             const RawStageInstr *instruction)
{
    if (FindTh07StageCameraOpcode(instruction->opcode) != nullptr)
    {
        if (ApplyDueTh07StageCameraInstruction(stage, instruction))
        {
            stage->th07InstructionIndex++;
            return Th07StageScriptStepResult::Applied;
        }
        return Th07StageScriptStepResult::Unsupported;
    }

    if (instruction->opcode == 0x1d || instruction->opcode == 0x1e)
    {
        return ApplyDueTh07StageAnmVmInstruction(stage, instruction);
    }

    if (!IsTh07StageNonCameraOpcode(instruction->opcode))
    {
        stage->th07InstructionIndex++;
        return Th07StageScriptStepResult::Applied;
    }

    Th07StageScriptRuntimeState state = BuildStageScriptRuntimeState(stage);
    const Th07StageScriptStepResult result =
        ApplyTh07StageScriptOpcode(&state,
                                   BuildStageScriptInstruction(instruction),
                                   FindNextTh07StagePositionKey(instruction));
    ApplyStageScriptRuntimeState(stage, state);
    if (result == Th07StageScriptStepResult::Jumped)
    {
        stage->th07CameraInterpDurations[0] = 0;
    }
    return result;
}

} // namespace

void ApplyTh07StageAnmManagerColorWrite(Stage *stage, D3DCOLOR color)
{
    if ((stage->th07AnmManagerColorMultiplier & 0xff000000) == 0)
    {
        stage->th07AnmManagerColorMultiplier = color;
        return;
    }

    const u32 current = stage->th07AnmManagerColorMultiplier;
    const u8 blue = static_cast<u8>(((color & 0xff) + (current & 0xff)) >> 1);
    const u8 green = static_cast<u8>((((color >> 8) & 0xff) + ((current >> 8) & 0xff)) >> 1);
    const u8 red = static_cast<u8>((((color >> 16) & 0xff) + ((current >> 16) & 0xff)) >> 1);
    const u8 alpha = static_cast<u8>(((color >> 24) + (current >> 24)) >> 1);
    stage->th07AnmManagerColorMultiplier = (static_cast<D3DCOLOR>(alpha) << 24) |
                                           (static_cast<D3DCOLOR>(red) << 16) |
                                           (static_cast<D3DCOLOR>(green) << 8) | blue;
}

void GuardTh07StageAnmManagerColorReset(Stage *stage)
{
    stage->th07AnmManagerColorResetGuard = 1;
}

Stage::Stage()
{
}

#pragma var_order(curInsn, stepResult)
ChainCallbackResult Stage::OnUpdate(Stage *stage)
{
    RawStageInstr *curInsn;
    Th07StageScriptStepResult stepResult;

    if (stage->th07StdData == NULL)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    if (g_GameManager.isTimeStopped)
    {
        // When Sakuya uses her time stop ability, we want to darken her
        // spellcard background a bit, to give a visual indication of what's
        // going on.
        COLOR_SET_COMPONENT(stage->spellcardBackground.color, COLOR_ALPHA_BYTE_IDX, 0x60);
        COLOR_SET_COMPONENT(stage->spellcardBackground.color, COLOR_BLUE_BYTE_IDX, 0x80);
        COLOR_SET_COMPONENT(stage->spellcardBackground.color, COLOR_GREEN_BYTE_IDX, 0x30);
        COLOR_SET_COMPONENT(stage->spellcardBackground.color, COLOR_RED_BYTE_IDX, 0x30);
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }

    ResolvePendingTh07StageResume(stage);
    for (;;)
    {
        curInsn = stage->th07BeginningOfScript + stage->th07InstructionIndex;
        if (!IsTh07StageScriptInstructionDue(BuildStageScriptRuntimeState(stage),
                                             BuildStageScriptInstruction(curInsn)))
        {
            break;
        }

        stepResult = ApplyDueTh07StageScriptInstruction(stage, curInsn);
        if (stepResult == Th07StageScriptStepResult::Paused)
        {
            break;
        }
        continue;
    }

    AdvanceTh07StageCameraRuntime(stage);
    {
        Th07StageScriptRuntimeState state = BuildStageScriptRuntimeState(stage);
        AdvanceTh07StageFogInterpolation(&state,
                                         g_Supervisor.framerateMultiplier,
                                         g_Supervisor.effectiveFramerateMultiplier);
        ApplyStageScriptRuntimeState(stage, state);
    }
    if (curInsn->opcode != 3)
    {
        TickTh07StageTimer(&stage->th07ScriptTime);
        ApplyStageScriptRuntimeState(stage, BuildStageScriptRuntimeState(stage));
    }
    stage->UpdateObjects();
    if (stage->spellcardState >= RUNNING)
    {
        if (stage->ticksSinceSpellcardStarted == 60)
        {
            stage->spellcardState = static_cast<SpellcardState>(stage->spellcardState + 1);
        }
        stage->ticksSinceSpellcardStarted = stage->ticksSinceSpellcardStarted + 1;
        g_AnmManager->ExecuteScript(&stage->spellcardBackground);
    }
    ExecuteTh07StageAnmVm(&stage->th07StageAnmVms[0]);
    ExecuteTh07StageAnmVm(&stage->th07StageAnmVms[1]);
    stage->th07FrameCounter++;
    ApplyStageScriptRuntimeState(stage, BuildStageScriptRuntimeState(stage));
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult Stage::OnDrawHighPrio(Stage *stage)
{
    if (stage->skyFogNeedsSetup)
    {
        stage->skyFogNeedsSetup = 0;
        g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGCOLOR, stage->th07Fog.color);
    }
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGSTART, *(u32 *)&stage->th07Fog.nearPlane);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGEND, *(u32 *)&stage->th07Fog.farPlane);
    ConsumeTh07StageAnmManagerColorWrite(stage);
    DrawTh07StageAnmVm(&stage->th07StageAnmVms[0]);
    DrawTh07StageAnmVm(&stage->th07StageAnmVms[1]);
    if (stage->spellcardState <= RUNNING)
    {
        if (!g_Gui.IsStageFinished())
        {
            stage->RenderObjects(0);
            stage->RenderObjects(1);
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#pragma var_order(val, stageToSpellcardBackgroundAlpha, gameRegion)
ChainCallbackResult Stage::OnDrawLowPrio(Stage *stage)
{
    f32 val;
    i32 stageToSpellcardBackgroundAlpha;
    ZunRect gameRegion;

    if (stage->spellcardState <= RUNNING)
    {
        if (!g_Gui.IsStageFinished())
        {
            stage->RenderObjects(2);
            stage->RenderObjects(3);
            if (stage->spellcardState == RUNNING)
            {
                gameRegion.left = GAME_REGION_LEFT;
                gameRegion.top = GAME_REGION_TOP;
                gameRegion.right = GAME_REGION_LEFT + GAME_REGION_WIDTH;
                gameRegion.bottom = GAME_REGION_TOP + GAME_REGION_HEIGHT;
                stageToSpellcardBackgroundAlpha = (stage->ticksSinceSpellcardStarted * 255) / 60;
                ScreenEffect::DrawSquare(&gameRegion, stageToSpellcardBackgroundAlpha << 24);
            }
        }
    }
    if (RUNNING <= stage->spellcardState)
    {
        if (stage->ticksSinceSpellcardStarted <= g_Supervisor.cfg.frameskipConfig)
        {
            g_AnmManager->SetAndExecuteScriptIdx(&stage->spellcardBackground, ANM_SCRIPT_EFFECTS_SPELLCARD_BACKGROUND);
        }
        g_AnmManager->Draw(&stage->spellcardBackground);
    }
    g_Supervisor.viewport.MinZ = 0.0;
    g_Supervisor.viewport.MaxZ = 0.5;
    GameManager::SetupCameraStageBackground(0);
    g_Supervisor.d3dDevice->SetViewport(&g_Supervisor.viewport);
    val = 1000.0f;
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGSTART, *(u32 *)&val);
    val = 2000.0f;
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGEND, *(u32 *)&val);
    ResetTh07StageAnmManagerColorAfterLowPrio(stage);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#pragma var_order(interpFinal, interpInitial, scriptTimer, facingDirTimer)
ZunResult Stage::AddedCallback(Stage *stage)
{
    stage->th07ScriptTime.previousFrame = -999;
    stage->th07ScriptTime.subframe = 0.0f;
    stage->th07ScriptTime.currentFrame = 0;

    stage->th07InstructionIndex = 0;
    stage->th07FrameCounter = 0;
    stage->th07StagePosition.x = 0.0;
    stage->th07StagePosition.y = 0.0;
    stage->th07StagePosition.z = 0.0;
    stage->spellcardState = NOT_RUNNING;
    stage->th07FogInterpDuration = 0;
    stage->th07StageAnmVms[0].originalActiveSpriteIndex = 0;
    stage->th07StageAnmVms[1].originalActiveSpriteIndex = 0;

    if (stage->LoadStageData(g_StageFiles[g_GameManager.currentStage].anmFile,
                             g_StageFiles[g_GameManager.currentStage].stdFile) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    stage->th07Fog.color = COLOR_BLACK;
    stage->th07Fog.nearPlane = 200.0;
    stage->th07Fog.farPlane = 500.0;
    stage->th07FogInterpInitial = stage->th07Fog;
    stage->th07FogInterpFinal = stage->th07Fog;
    stage->th07FogInterpTimer.previousFrame = -999;
    stage->th07FogInterpTimer.subframe = 0.0f;
    stage->th07FogInterpTimer.currentFrame = 0;
    stage->th07StageField4d8 = 0;
    stage->th07StagePositionInterpFinal = {};
    stage->th07StagePositionInterpEndFrame = 0;
    stage->th07StagePositionInterpInitial = {};
    stage->th07StagePositionInterpStartFrame = 0;
    stage->th07CameraSetupPendingAfterJump = 0;
    ResetTh07StageAnmManagerColorWrite(stage);
    stage->th07AnmManagerColorResetGuard = 0;
    ResetTh07AnmManagerDrawOffset();
    ResetTh07AnmManagerColorMultiplier();
    InitializeTh07StageCamera(stage);
    ApplyStageScriptRuntimeState(stage, BuildStageScriptRuntimeState(stage));

    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGCOLOR, stage->th07Fog.color);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)&stage->th07Fog.nearPlane);
    g_Supervisor.d3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)&stage->th07Fog.farPlane);
    return ZUN_SUCCESS;
}

#pragma var_order(stg)
ZunResult Stage::RegisterChain(u32 stage)
{

    Stage *stg = &g_Stage;

    memset(stg, 0, sizeof(Stage));
    stg->th07StdData = NULL;

    stg->th07StageRoute = stage;
    g_StageCalcChain.callback = (ChainCallback)Stage::OnUpdate;
    g_StageCalcChain.addedCallback = NULL;
    g_StageCalcChain.deletedCallback = NULL;
    g_StageCalcChain.addedCallback = (ChainAddedCallback)Stage::AddedCallback;
    g_StageCalcChain.deletedCallback = (ChainDeletedCallback)Stage::DeletedCallback;
    g_StageCalcChain.arg = stg;

    if (g_Chain.AddToCalcChain(&g_StageCalcChain, TH_CHAIN_PRIO_CALC_STAGE))
    {
        return ZUN_ERROR;
    }
    g_StageOnDrawHighPrioChain.callback = (ChainCallback)OnDrawHighPrio;
    g_StageOnDrawHighPrioChain.addedCallback = NULL;
    g_StageOnDrawHighPrioChain.deletedCallback = NULL;
    g_StageOnDrawHighPrioChain.arg = stg;
    g_Chain.AddToDrawChain(&g_StageOnDrawHighPrioChain, TH_CHAIN_PRIO_DRAW_HIGH_PRIO_STAGE);
    g_StageOnDrawLowPrioChain.callback = (ChainCallback)OnDrawLowPrio;
    g_StageOnDrawLowPrioChain.addedCallback = NULL;
    g_StageOnDrawLowPrioChain.deletedCallback = NULL;
    g_StageOnDrawLowPrioChain.arg = stg;
    g_Chain.AddToDrawChain(&g_StageOnDrawLowPrioChain, TH_CHAIN_PRIO_DRAW_LOW_PRIO_STAGE);

    return ZUN_SUCCESS;
}

ZunResult Stage::DeletedCallback(Stage *s)
{
    g_AnmManager->ReleaseAnm(ANM_FILE_STAGEBG);
    g_AnmManager->ReleaseAnm(ANM_FILE_STAGEBG2);
    g_AnmManager->ReleaseAnm(ANM_FILE_STAGEBG3);
    g_AnmManager->ReleaseAnm(ANM_FILE_STAGEBG4);
    g_AnmManager->ReleaseAnm(ANM_FILE_STAGEBG5);
    if (s->quadVms != NULL)
    {
        void *quadVms = s->quadVms;
        free(quadVms);
        s->quadVms = NULL;
    }
    if (s->th07StdData != NULL)
    {
        void *stdData = s->th07StdData;
        free(stdData);
        s->th07StdData = NULL;
    }
    return ZUN_SUCCESS;
}

void Stage::CutChain()
{
    g_Chain.Cut(&g_StageCalcChain);
    g_Chain.Cut(&g_StageOnDrawHighPrioChain);
    g_Chain.Cut(&g_StageOnDrawLowPrioChain);
}

#pragma var_order(vmIdx, idx, curObj, curQuad, sizeVmArr)
ZunResult Stage::LoadStageData(char *anmpath, char *stdpath)
{
    RawStageObject *curObj;
    RawStageQuadBasic *curQuad;
    i32 idx;
    i32 vmIdx;
    u32 sizeVmArr;

    if (g_AnmManager->LoadAnm(ANM_FILE_STAGEBG, anmpath, ANM_OFFSET_STAGEBG) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }
    // TH07 FUN_00407000: Stage 4 owns four extra background ANM files.
    if (g_GameManager.currentStage == 4)
    {
        if (g_AnmManager->LoadAnm(ANM_FILE_STAGEBG2, "data/stg4bg2.anm", ANM_OFFSET_STAGEBG2) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        if (g_AnmManager->LoadAnm(ANM_FILE_STAGEBG3, "data/stg4bg3.anm", ANM_OFFSET_STAGEBG3) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        if (g_AnmManager->LoadAnm(ANM_FILE_STAGEBG4, "data/stg4bg4.anm", ANM_OFFSET_STAGEBG4) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        if (g_AnmManager->LoadAnm(ANM_FILE_STAGEBG5, "data/stg4bg5.anm", ANM_OFFSET_STAGEBG5) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
    }
    this->th07StdData = (RawStageHeader *)FileSystem::OpenPath(stdpath, false);
    if (this->th07StdData == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_STAGE_DATA_CORRUPTED);
        return ZUN_ERROR;
    }
    this->th07ObjectsCount = this->th07StdData->nbObjects;
    this->th07QuadCount = this->th07StdData->nbFaces;
    this->th07ObjectInstances = (RawStageObjectInstance *)(this->th07StdData->facesOffset + (i32)this->th07StdData);
    this->th07BeginningOfScript = (RawStageInstr *)(this->th07StdData->scriptOffset + (i32)this->th07StdData);
    this->th07Objects = (RawStageObject **)(this->th07StdData + 1);
    for (idx = 0; idx < this->th07ObjectsCount; idx++)
    {
        this->th07Objects[idx] = (RawStageObject *)((i32)this->th07Objects[idx] + (i32)this->th07StdData);
    }
    sizeVmArr = this->th07QuadCount * sizeof(AnmVm);
    this->quadVms = (AnmVm *)malloc(sizeVmArr);
    for (idx = 0, vmIdx = 0; idx < this->th07ObjectsCount; idx++)
    {
        curObj = this->th07Objects[idx];
        curObj->flags = 1;
        curQuad = &curObj->firstQuad;
        while (0 <= curQuad->type)
        {
            g_AnmManager->ExecuteAnmIdx(&this->quadVms[vmIdx], curQuad->anmScript + ANM_OFFSET_STAGEBG);
            curQuad->vmIdx = vmIdx++;
            curQuad = (RawStageQuadBasic *)((u8 *)curQuad + curQuad->byteSize);
        }
    }
    return ZUN_SUCCESS;
}

#pragma var_order(objQuadType1, vmsNotFinished, objIdx, vm, obj, objQuad)
ZunResult Stage::UpdateObjects()
{
    AnmVm *vm;
    RawStageQuadBasic *objQuad;
    RawStageQuadBasic *objQuadType1;
    i32 objIdx;
    i32 vmsNotFinished;
    RawStageObject *obj;

    for (objIdx = 0; objIdx < this->th07ObjectsCount; objIdx++)
    {
        obj = this->th07Objects[objIdx];
        if (obj->flags & 1 != 0)
        {
            vmsNotFinished = 0;
            objQuad = &obj->firstQuad;
            while (0 <= objQuad->type)
            {
                vm = &this->quadVms[objQuad->vmIdx];
                switch (objQuad->type)
                {
                case 0:
                    g_AnmManager->ExecuteScript(vm);
                    break;
                case 1:
                    // I assume this casts it, but this is all dead code
                    // as the engine doesn't contain any other reference
                    // to type 1 quads.
                    objQuadType1 = objQuad;
                    g_AnmManager->ExecuteScript(vm);
                    break;
                }
                if (vm->currentInstruction != NULL)
                {
                    vmsNotFinished++;
                }
                objQuad = (RawStageQuadBasic *)((i32)&objQuad->type + objQuad->byteSize);
            }
            if (vmsNotFinished == 0)
            {
                obj->flags = obj->flags & ~1;
            }
        }
    }
    return ZUN_SUCCESS;
}

#pragma var_order(unk8, curQuadVm, instancesDrawn, instance, worldMatrix, obj, quadScaledPos, quadPos, curQuad,        \
                  didDraw, projectSrc, quadWidth)
ZunResult Stage::RenderObjects(i32 zLevel)
{
    f32 quadWidth;
    D3DXVECTOR3 projectSrc;
    ZunBool didDraw;
    RawStageQuadBasic *curQuad;
    D3DXVECTOR3 quadPos;
    D3DXVECTOR3 quadScaledPos;
    RawStageObject *obj;
    D3DXMATRIX worldMatrix;
    RawStageObjectInstance *instance;
    i32 instancesDrawn;
    AnmVm *curQuadVm;
    i32 unk8;

    instance = &this->th07ObjectInstances[0];
    instancesDrawn = 0;
    didDraw = 0;
    projectSrc.x = 0.0;
    projectSrc.y = 0.0;
    projectSrc.z = 0.0;
    D3DXMatrixIdentity(&worldMatrix);
    while (instance->id >= 0)
    {
        obj = this->th07Objects[instance->id];
        if (obj->zLevel == zLevel)
        {
            curQuad = &obj->firstQuad;
            unk8 = 0;

            //  Say hello to helper cube:
            //
            //    ^
            //    |           A------B.
            //  y |           |`.    | `.
            //    |           |  `C--+---D
            //    |  x        |   |  |   |
            //    o----->     E---+--F.  |
            //     `.          `. |    `.|
            //    z  `_          `G------H
            //
            //   It's beautiful, I know. E is at point 0, 0, 0 here.
            //
            // During this process, zun will project the world matrix to each of
            // the 8 corner of the kube that reprents this object, and check if
            // any of them is visible on the viewport.
            //
            // It will check them in the following order: C, G, E, A, D, H, F, B.

            // It first starts by checking point C
            worldMatrix.m[3][0] = obj->position.x + instance->position.x - this->th07StagePosition.x;
            worldMatrix.m[3][1] = -(obj->position.y + instance->position.y - this->th07StagePosition.y);
            worldMatrix.m[3][2] = obj->position.z + instance->position.z - this->th07StagePosition.z + obj->size.z;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);

            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then G:
            worldMatrix.m[3][1] = worldMatrix.m[3][1] - obj->size.y;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then E
            worldMatrix.m[3][2] = worldMatrix.m[3][2] - obj->size.z;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then A
            worldMatrix.m[3][1] = worldMatrix.m[3][1] + obj->size.y;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then D
            worldMatrix.m[3][0] = obj->position.x + instance->position.x - this->th07StagePosition.x + obj->size.x;
            worldMatrix.m[3][1] = -(obj->position.y + instance->position.y - this->th07StagePosition.y);
            worldMatrix.m[3][2] = obj->position.z + instance->position.z - this->th07StagePosition.z + obj->size.z;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then H
            worldMatrix.m[3][1] = worldMatrix.m[3][1] - obj->size.y;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // Then F
            worldMatrix.m[3][2] = worldMatrix.m[3][2] - (obj->size).z;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // And finally B
            worldMatrix.m[3][1] = worldMatrix.m[3][1] + (obj->size).y;
            D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                            &g_Supervisor.viewMatrix, &worldMatrix);
            if (quadPos.y >= g_Supervisor.viewport.Y &&
                quadPos.y <= g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
            {
                goto render;
            }

            // If none of the points were in the viewport, we can skip this object
            // entirely.
            goto skip;

        render:
            didDraw = 1;
            while (0 <= curQuad->type)
            {
                curQuadVm = this->quadVms + curQuad->vmIdx;
                switch (curQuad->type)
                {
                case 0:
                    curQuadVm->pos.x = curQuad->position.x + instance->position.x - this->th07StagePosition.x;
                    curQuadVm->pos.y = curQuad->position.y + instance->position.y - this->th07StagePosition.y;
                    curQuadVm->pos.z = curQuad->position.z + instance->position.z - this->th07StagePosition.z;
                    if (curQuad->size.x != 0.0f)
                    {
                        curQuadVm->scaleX = curQuad->size.x / curQuadVm->sprite->widthPx;
                    }
                    if (curQuad->size.y != 0.0f)
                    {
                        curQuadVm->scaleY = curQuad->size.y / curQuadVm->sprite->heightPx;
                    }
                    if (curQuadVm->autoRotate == 2)
                    {
                        if (curQuad->size.x != 0.0f)
                        {
                            quadWidth = curQuad->size.x;
                        }
                        else
                        {
                            quadWidth = curQuadVm->sprite->widthPx;
                        }
                        worldMatrix.m[3][0] = curQuadVm->pos.x;
                        worldMatrix.m[3][1] = -curQuadVm->pos.y;
                        worldMatrix.m[3][2] = curQuadVm->pos.z;
                        D3DXVec3Project(&quadPos, &projectSrc, &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                                        &g_Supervisor.viewMatrix, &worldMatrix);
                        worldMatrix.m[3][0] = quadWidth * curQuadVm->scaleX + worldMatrix.m[3][0];
                        D3DXVec3Project(&quadScaledPos, &projectSrc, &g_Supervisor.viewport,
                                        &g_Supervisor.projectionMatrix, &g_Supervisor.viewMatrix, &worldMatrix);
                        curQuadVm->scaleX = (quadScaledPos.x - quadPos.x) / quadWidth;
                        curQuadVm->scaleY = curQuadVm->scaleX;
                        curQuadVm->pos = quadPos;
                        g_AnmManager->DrawFacingCamera(curQuadVm);
                    }
                    else
                    {
                        g_AnmManager->Draw3(curQuadVm);
                    }
                    break;
                }
                curQuad = (RawStageQuadBasic *)((i32)&curQuad->type + curQuad->byteSize);
            }
            instancesDrawn++;
        }
    skip:
        instance++;
    }
    return ZUN_SUCCESS;
}
}; // namespace th07
