#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace th07 {

inline constexpr std::uint32_t kTh07EclDispatchFunctionAddress = 0x00410520;
inline constexpr std::uint32_t kTh07EclOpcodeJumpTableAddress = 0x00417838;
inline constexpr std::uint32_t kTh07EclDispatchAdvanceAddress = 0x0041677b;
inline constexpr std::uint16_t kTh07EclOpcodeJumpTableFirstOpcode = 1;
inline constexpr std::uint16_t kTh07EclOpcodeJumpTableLastOpcode = 161;
inline constexpr std::size_t kTh07EclOpcodeJumpTableEntryCount = 161;
inline constexpr std::size_t kTh07DeclOpcodeCount = 141;
inline constexpr std::size_t kTh07CurrentEclEnumOpcodeCount = 136;
inline constexpr std::size_t kTh07CurrentEclHandledOpcodeCount = 134;
inline constexpr std::size_t kTh07MissingEnumOpcodeCount = 25;
inline constexpr std::size_t kTh07MissingDispatchOpcodeCount = 26;
inline constexpr std::size_t kTh07MissingNonNopDispatchOpcodeCount = 25;
inline constexpr std::size_t kTh07HighEclOpcodeCaseCount = 25;
inline constexpr std::size_t kTh07HighEclOpcodeSignatureCount = 31;
inline constexpr std::size_t kTh07HighEclDirectEffectContractCount = 41;
inline constexpr std::size_t kTh07HighEclCallContractCount = 17;
inline constexpr std::uint32_t kTh07EclResolveFloatTargetFunctionAddress = 0x0040f3c0;
inline constexpr std::uint32_t kTh07EclResolveFloatTargetJumpTableAddress = 0x0040f5d0;
inline constexpr std::uint32_t kTh07EclResolveFloatTargetClassTableAddress = 0x0040f668;
inline constexpr std::uint32_t kTh07EclResolveFloatTargetCaseBase = 0x2714;
inline constexpr std::uint32_t kTh07EclResolveFloatTargetMaxRelativeId = 0x45;
inline constexpr std::size_t kTh07EclResolveFloatTargetContractCount = 37;
inline constexpr std::uint32_t kTh07EclIntReadResolverFunctionAddress = 0x0040e5b0;
inline constexpr std::uint32_t kTh07EclIntReadResolverJumpTableAddress = 0x0040eace;
inline constexpr std::uint32_t kTh07EclIntReadResolverDefaultAddress = 0x0040eac7;
inline constexpr std::uint32_t kTh07EclFloatReadResolverFunctionAddress = 0x0040edf0;
inline constexpr std::uint32_t kTh07EclFloatReadResolverJumpTableAddress = 0x0040f28a;
inline constexpr std::uint32_t kTh07EclFloatReadResolverDefaultAddress = 0x0040f281;
inline constexpr std::uint32_t kTh07EclReadResolverCaseBase = 0x2710;
inline constexpr std::uint32_t kTh07EclReadResolverMaxRelativeId = 0x49;
inline constexpr std::size_t kTh07EclIntReadResolverContractCount = 70;
inline constexpr std::size_t kTh07EclFloatReadResolverContractCount = 74;
inline constexpr std::uint32_t kTh07EclReadResolverNoField = 0xffffffff;
inline constexpr std::uint32_t kTh07EclReadResolverNoHelper = 0x00000000;
inline constexpr std::uint32_t kTh07EclReadResolverFloatToIntBridgeAddress = 0x0048b8a0;
inline constexpr std::uint32_t kTh07EclReadResolverRandomIntFunctionAddress = 0x004318d0;
inline constexpr std::uint32_t kTh07EclReadResolverRandomFloatFunctionAddress = 0x00431900;
inline constexpr std::uint32_t kTh07EclReadResolverVectorLengthFunctionAddress = 0x00442370;
inline constexpr std::uint32_t kTh07EclReadResolverDistanceFunctionAddress = 0x00404e70;
inline constexpr std::uint32_t kTh07EclClearHelperFunctionAddress = 0x00424740;
inline constexpr std::uint32_t kTh07EclClearHelperObjectBaseOffset = 0x366628;
inline constexpr std::uint32_t kTh07EclClearHelperObjectCount = 0x40;
inline constexpr std::uint32_t kTh07EclClearHelperObjectStrideBytes = 0x04ec;
inline constexpr std::uint32_t kTh07EclClearHelperObjectActiveOffset = 0x04d4;
inline constexpr std::uint32_t kTh07EclClearHelperObjectFlagsOffset = 0x04e4;
inline constexpr std::uint32_t kTh07EclClearHelperObjectProtectedFlagMask = 0x04;
inline constexpr std::uint32_t kTh07EclClearHelperObjectStateOffset = 0x04e8;
inline constexpr std::uint8_t kTh07EclClearHelperObjectStateValue = 2;
inline constexpr std::uint32_t kTh07EclClearHelperObjectTimerOffset = 0x04d8;
inline constexpr std::uint32_t kTh07EclClearHelperObjectTimerSentinel = 0xfffffc19;
inline constexpr std::uint32_t kTh07EclClearHelperObjectCopySourceOffset = 0x04b8;
inline constexpr std::uint32_t kTh07EclClearHelperObjectCopyDestOffset = 0x04b4;
inline constexpr std::uint32_t kTh07EclClearHelperObjectClearFlagOffset = 0x04d0;
inline constexpr std::uint32_t kTh07EclClearHelperObjectAngleOffset = 0x04a4;
inline constexpr std::uint32_t kTh07EclClearHelperObjectRangeStartOffset = 0x04a8;
inline constexpr std::uint32_t kTh07EclClearHelperObjectRangeEndOffset = 0x04ac;
inline constexpr std::uint32_t kTh07EclClearHelperObjectBaseXOffset = 0x0498;
inline constexpr std::uint32_t kTh07EclClearHelperObjectBaseYOffset = 0x049c;
inline constexpr std::uint32_t kTh07EclClearHelperManagerEffectKindOffset = 0x37a160;
inline constexpr std::uint32_t kTh07EclClearHelperFinalCooldownOffset = 0x37a12c;
inline constexpr std::uint32_t kTh07EclClearHelperFinalCooldownValue = 10;
inline constexpr std::uint32_t kTh07EclClearHelperEffectManagerAddress = 0x00575c70;
inline constexpr std::uint32_t kTh07EclClearHelperEffectSpawnFunctionAddress = 0x004326f0;
inline constexpr std::uint32_t kTh07EclClearHelperLineDirectionFunctionAddress = 0x00417af0;
inline constexpr std::uint32_t kTh07EclClearHelperLineStepConstantAddress = 0x00498a84;
inline constexpr std::uint8_t kTh07EclClearHelperMaxEffectMode = 8;
inline constexpr std::uint8_t kTh07EclClearHelperDynamicEffectModeLimit = 3;
inline constexpr std::uint8_t kTh07EclClearHelperFallbackEffectKind = 8;
inline constexpr std::uint8_t kTh07EclClearHelperFallbackEffectArg = 1;
inline constexpr std::uint8_t kTh07EclClearHelperProtectedBypassMode = 10;
inline constexpr std::uint32_t kTh07EclItemClearRadiusHelperFunctionAddress = 0x00424c00;
inline constexpr std::uint32_t kTh07EclItemClearRadiusItemArrayAddress = 0x0063b218;
inline constexpr std::uint32_t kTh07EclItemClearRadiusEffectManagerAddress = 0x00575c70;
inline constexpr std::uint32_t kTh07EclItemClearRadiusEffectSpawnFunctionAddress = 0x004326f0;
inline constexpr std::uint32_t kTh07EclItemClearRadiusItemCount = 0x400;
inline constexpr std::uint32_t kTh07EclItemClearRadiusItemStrideBytes = 0x0d68;
inline constexpr std::uint32_t kTh07EclItemClearRadiusItemStrideDwords = 0x35a;
inline constexpr std::uint32_t kTh07EclItemClearRadiusPositionOffset = 0x0b8c;
inline constexpr std::uint32_t kTh07EclItemClearRadiusStateOffset = 0x0bfc;
inline constexpr std::uint16_t kTh07EclItemClearRadiusSkippedState = 5;
inline constexpr std::uint8_t kTh07EclItemClearRadiusEffectKind = 6;
inline constexpr std::uint8_t kTh07EclItemClearRadiusEffectArg = 1;
inline constexpr std::uint32_t kTh07EclVisualEffectSpawnFunctionAddress = 0x004326f0;
inline constexpr std::uint32_t kTh07EclVisualEffectManagerAddress = 0x00575c70;
inline constexpr std::uint32_t kTh07EclVisualEffectSlotCount = 0x44c;
inline constexpr std::uint32_t kTh07EclVisualEffectSlotSizeBytes = 0x0288;
inline constexpr std::uint32_t kTh07EclVisualEffectFallbackSlotOffset = 0x0ae060;
inline constexpr std::uint32_t kTh07EclVisualEffectNextIndexOffset = 0x0ae2e8;
inline constexpr std::uint32_t kTh07EclVisualEffectPositionOffset = 0x024c;
inline constexpr std::uint32_t kTh07EclVisualEffectAuxVectorOffset = 0x0258;
inline constexpr std::uint32_t kTh07EclVisualEffectVelocityOffset = 0x0264;
inline constexpr std::uint32_t kTh07EclVisualEffectTimerPreviousOffset = 0x0270;
inline constexpr std::uint32_t kTh07EclVisualEffectTimerSubframeOffset = 0x0274;
inline constexpr std::uint32_t kTh07EclVisualEffectTimerCurrentOffset = 0x0278;
inline constexpr std::uint32_t kTh07EclVisualEffectModeOffset = 0x027c;
inline constexpr std::uint32_t kTh07EclVisualEffectActiveFlagOffset = 0x027d;
inline constexpr std::uint32_t kTh07EclVisualEffectInitializedFlagOffset = 0x027e;
inline constexpr std::uint32_t kTh07EclVisualEffectVariantOffset = 0x027f;
inline constexpr std::uint32_t kTh07EclVisualEffectPostInitByteOffset = 0x0280;
inline constexpr std::uint32_t kTh07EclVisualEffectColorOffset = 0x01b8;
inline constexpr std::uint32_t kTh07EclVisualEffectRenderFlagsOffset = 0x01c0;
inline constexpr std::uint32_t kTh07EclVisualEffectAnmScriptOffset = 0x01d8;
inline constexpr std::uint32_t kTh07EclVisualEffectTimerPreviousValue = 0xfffffc19;
inline constexpr std::uint32_t kTh07EclVisualEffectColorValue = 0xffffffff;
inline constexpr std::uint32_t kTh07EclVisualEffectRenderFlagBits = 0x1000;
inline constexpr std::uint32_t kTh07EclVisualEffectAnmScriptBase = 0x02c4;
inline constexpr std::uint32_t kTh07EclVisualEffectModeGatePointerAddress = 0x00626278;
inline constexpr std::uint32_t kTh07EclVisualEffectModeGateValueOffset = 0x007c;
inline constexpr std::uint32_t kTh07EclVisualEffectModeGateThreshold = 0x80;
inline constexpr std::uint8_t kTh07EclVisualEffectModeGateReplacement = 7;
inline constexpr std::uint8_t kTh07EclVisualEffectModeGateFirstAffectedMode = 0;
inline constexpr std::uint8_t kTh07EclVisualEffectModeGateSecondAffectedMode = 2;
inline constexpr std::uint32_t kTh07EclVisualEffectFloatToIntFunctionAddress = 0x0048b8a0;
inline constexpr std::uint32_t kTh07EclVisualEffectRandomFloatFunctionAddress = 0x00431900;
inline constexpr std::uint32_t kTh07EclVisualEffectAnmManagerPointerAddress = 0x004b9e44;
inline constexpr std::uint32_t kTh07EclVisualEffectAnmScriptTableOffset = 0x28ef0;
inline constexpr std::uint32_t kTh07EclVisualEffectSetScriptFunctionAddress = 0x0044ea20;
inline constexpr std::uint8_t kTh07EclVisualEffectRandomVelocityMode = 2;
inline constexpr std::uint8_t kTh07EclVisualEffectForceVariantOneMode = 3;
inline constexpr std::uint8_t kTh07EclVisualEffectForceVariantZeroMode = 4;
inline constexpr std::uint32_t kTh07EclVisualEffectRandomXScaleAddress = 0x00498c78;
inline constexpr std::uint32_t kTh07EclVisualEffectRandomXBiasAddress = 0x00498b34;
inline constexpr std::uint32_t kTh07EclVisualEffectRandomYScaleAddress = 0x00498c74;
inline constexpr std::uint32_t kTh07EclVisualEffectRandomYBiasAddress = 0x00498b08;
inline constexpr std::uint16_t kTh07EclProjectionOpcode = 151;
inline constexpr std::size_t kTh07EclProjectionHelperContractCount = 2;
inline constexpr std::uint32_t kTh07EclProjectionSinWrapperFunctionAddress = 0x0048bb40;
inline constexpr std::uint32_t kTh07EclProjectionSinCoreFunctionAddress = 0x0048bb5d;
inline constexpr std::uint32_t kTh07EclProjectionCosWrapperFunctionAddress = 0x0048bbf0;
inline constexpr std::uint32_t kTh07EclProjectionCosCoreFunctionAddress = 0x0048bc0d;
inline constexpr std::uint32_t kTh07EclProjectionFiniteCheckFunctionAddress = 0x00483d38;
inline constexpr std::uint32_t kTh07EclProjectionSinOpcodeCallInstructionAddress = 0x0041653f;
inline constexpr std::uint32_t kTh07EclProjectionCosOpcodeCallInstructionAddress = 0x004165de;
inline constexpr std::uint32_t kTh07EclProjectionSinFiniteCheckCallAddress = 0x0048bb46;
inline constexpr std::uint32_t kTh07EclProjectionSinCoreCallAddress = 0x0048bb4b;
inline constexpr std::uint32_t kTh07EclProjectionCosFiniteCheckCallAddress = 0x0048bbf6;
inline constexpr std::uint32_t kTh07EclProjectionCosCoreCallAddress = 0x0048bbfb;
inline constexpr std::uint32_t kTh07EclProjectionWrapperStackBytes = 0x0c;
inline constexpr std::uint32_t kTh07EclProjectionFpuDefaultControlWord = 0x027f;
inline constexpr std::uint32_t kTh07EclProjectionFpuC2StatusMask = 0x0400;
inline constexpr std::uint32_t kTh07EclProjectionFiniteExponentMask = 0x7ff00000;
inline constexpr std::uint32_t kTh07EclProjectionControlWordAddress = 0x004911b8;
inline constexpr std::uint32_t kTh07EclProjectionRangeReductionConstantAddress = 0x004911ba;
inline constexpr std::uint32_t kTh07EclProjectionIndefiniteConstantAddress = 0x0049e010;
inline constexpr std::uint32_t kTh07EclProjectionMathErrorFlagAddress = 0x0049f77c;
inline constexpr std::uint32_t kTh07EclProjectionSinMathErrorContextAddress = 0x0049f6e0;
inline constexpr std::uint32_t kTh07EclProjectionCosMathErrorContextAddress = 0x0049f6f0;
inline constexpr std::uint32_t kTh07EclProjectionSinPrimaryInstructionAddress = 0x0048bb72;
inline constexpr std::uint32_t kTh07EclProjectionSinRangeLoadAddress = 0x0048bb97;
inline constexpr std::uint32_t kTh07EclProjectionSinRangeLoopAddress = 0x0048bb9f;
inline constexpr std::uint32_t kTh07EclProjectionSinReducedInstructionAddress = 0x0048bba9;
inline constexpr std::uint32_t kTh07EclProjectionCosPrimaryInstructionAddress = 0x0048bc22;
inline constexpr std::uint32_t kTh07EclProjectionCosRangeLoadAddress = 0x0048bc47;
inline constexpr std::uint32_t kTh07EclProjectionCosRangeLoopAddress = 0x0048bc4f;
inline constexpr std::uint32_t kTh07EclProjectionCosReducedInstructionAddress = 0x0048bc59;
inline constexpr std::uint8_t kTh07EclProjectionSinMathErrorKind = 0x1e;
inline constexpr std::uint8_t kTh07EclProjectionCosMathErrorKind = 0x12;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperFunctionAddress = 0x00455170;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperCallInstructionAddress = 0x004160c5;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperCallThisPointerAddress = 0x004b9e44;
inline constexpr std::uint16_t kTh07EclOpcode138RecordHelperOpcode = 138;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperOutputBaseOffset = 0x39f8;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperFlagFieldOffset = 0x4f30;
inline constexpr std::uint8_t kTh07EclOpcode138RecordHelperEnabledBitMask = 0x08;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperSegmentNumeratorOffset = 0x4f32;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperSegmentDenominatorOffset = 0x4f36;
inline constexpr std::uint8_t kTh07EclOpcode138RecordHelperSegmentCountShift = 1;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMinSegmentCount = 3;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperFailureReturn = 0xffffffff;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperSuccessReturn = 0;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMetadataPointerOffset = 0x01e4;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperOwnerBaseXOffset = 0x0028;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperOwnerBaseYOffset = 0x002c;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperOwnerColorOffset = 0x01b8;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMetadataLeftXOffset = 0x001c;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMetadataFirstLaneYOffset = 0x0020;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMetadataStartXOffset = 0x0024;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperMetadataSecondLaneYOffset = 0x0028;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordStrideBytes = 0x0038;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperSecondLaneBaseOffset = 0x001c;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordScaleOffset = 0x000c;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordColorOffset = 0x0010;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordXOffset = 0x0014;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordYOffset = 0x0018;
inline constexpr std::uint32_t kTh07EclOpcode138RecordHelperRecordScaleBits = 0x3f800000;
inline constexpr std::uint8_t kTh07EclOpcode138RecordHelperFirstLaneInitialIndex = 0;
inline constexpr std::uint8_t kTh07EclOpcode138RecordHelperSecondLaneInitialIndex = 1;
inline constexpr std::uint8_t kTh07EclOpcode138RecordHelperIndexStep = 2;
inline constexpr std::uint8_t kTh07EclEffectNoOperand = 0xff;
inline constexpr std::uint8_t kTh07EclEffectNoBit = 0xff;
inline constexpr std::uint32_t kTh07EclEffectNoOffset = 0xffffffff;

enum class Th07EclOpcodeStatus {
    SourceDerived,
    Blocked,
};

enum class Th07EclOperandKind {
    Int,
    Float,
    IntReference,
    FloatReference,
};

enum class Th07EclEffectOwner {
    Enemy,
    EnemyPointerTableEntry,
    Global,
    GlobalIndexedTable,
    GlobalPointerTableEntry,
};

enum class Th07EclEffectWriteKind {
    Bit,
    Byte,
    I16,
    I32,
    F32,
    TimerPreviousMinus999,
    TimerSubframeZero,
    TimerCurrentZero,
    CopyDwords,
};

enum class Th07EclCallContractKind {
    HelperCall,
    ResolvedFloatTarget,
    ProjectionHelper,
    RandomFloat,
    EffectSpawn,
};

enum class Th07EclFloatTargetOwner {
    Enemy,
    Global,
};

enum class Th07EclReadSourceKind {
    EnemyI32,
    EnemyU8,
    EnemyF32,
    GlobalI32,
    GlobalU8,
    GlobalF32,
    RandomInt,
    RandomIntModuloEnemyI32,
    RandomFloat,
    RandomFloatScaledEnemyF32,
    RandomFloatScaledGlobalF32,
    VectorLength,
    DistanceToGlobals,
};

enum class Th07EclProjectionHelperKind {
    Sin,
    Cos,
};

struct Th07EclHighOpcodeCase {
    std::uint16_t opcode;
    std::uint16_t declCount;
    std::string_view firstUse;
    std::uint16_t caseLabel;
    std::uint32_t jumpTableEntryAddress;
    std::uint32_t targetAddress;
    std::uint16_t evidenceLineStart;
    std::uint16_t evidenceLineEnd;
    std::string_view effectSurface;
    Th07EclOpcodeStatus status;
    std::string_view nextEvidence;

    bool hasDispatchCase() const;
    bool hasDecompilerCase() const;
    bool targetsCommonAdvance() const;
};

struct Th07EclHighOpcodeSignature {
    std::uint16_t opcode;
    std::uint16_t count;
    std::uint8_t operandCount;
    std::array<Th07EclOperandKind, 4> operandKinds;
    std::string_view firstUse;
    std::string_view sample;

    bool usesOperandKind(Th07EclOperandKind kind) const;
};

struct Th07EclHighOpcodeEffectContract {
    std::uint16_t opcode;
    std::uint16_t caseLabel;
    Th07EclEffectOwner owner;
    Th07EclEffectWriteKind writeKind;
    std::uint32_t ownerOffsetOrAddress;
    std::uint32_t fieldOffset;
    std::uint32_t sourceOffset;
    std::uint32_t divisorOffset;
    std::uint8_t valueOperandIndex;
    std::uint8_t indexOperandIndex;
    std::uint8_t bitIndex;
    std::uint16_t elementCount;
    std::string_view evidenceLines;
};

struct Th07EclHighOpcodeCallContract {
    std::uint16_t opcode;
    std::uint16_t caseLabel;
    std::uint32_t caseTargetAddress;
    std::uint32_t callInstructionAddress;
    std::uint32_t calleeAddress;
    Th07EclCallContractKind kind;
    std::uint8_t primaryOperandIndex;
    std::uint8_t secondaryOperandIndex;
    std::string_view evidenceLines;
    std::string_view effectSurface;
};

struct Th07EclFloatTargetResolverContract {
    std::uint16_t variableId;
    Th07EclFloatTargetOwner owner;
    std::uint32_t ownerOffsetOrAddress;
    std::uint32_t caseAddress;
    std::string_view evidenceLines;
};

struct Th07EclReadResolverContract {
    std::uint16_t variableId;
    Th07EclReadSourceKind sourceKind;
    std::uint32_t primaryOffsetOrAddress;
    std::uint32_t secondaryOffsetOrAddress;
    std::uint32_t helperAddress;
    std::string_view evidenceLines;
};

struct Th07EclClearHelperContract {
    std::uint32_t functionAddress;
    std::uint32_t itemArrayAddress;
    std::uint32_t itemCount;
    std::uint32_t itemStrideBytes;
    std::uint32_t itemStrideDwords;
    std::uint32_t itemPositionOffset;
    std::uint32_t itemStateOffset;
    std::uint16_t itemSkippedState;
    std::uint32_t objectBaseOffset;
    std::uint32_t objectCount;
    std::uint32_t objectStrideBytes;
    std::uint32_t objectActiveOffset;
    std::uint32_t objectFlagsOffset;
    std::uint32_t objectProtectedFlagMask;
    std::uint32_t objectStateOffset;
    std::uint8_t objectStateValue;
    std::uint32_t objectTimerOffset;
    std::uint32_t objectTimerSentinel;
    std::uint32_t objectCopySourceOffset;
    std::uint32_t objectCopyDestOffset;
    std::uint32_t objectClearFlagOffset;
    std::uint32_t objectAngleOffset;
    std::uint32_t objectRangeStartOffset;
    std::uint32_t objectRangeEndOffset;
    std::uint32_t objectBaseXOffset;
    std::uint32_t objectBaseYOffset;
    std::uint32_t managerEffectKindOffset;
    std::uint32_t finalCooldownOffset;
    std::uint32_t finalCooldownValue;
    std::uint32_t effectManagerAddress;
    std::uint32_t effectSpawnFunctionAddress;
    std::uint32_t lineDirectionFunctionAddress;
    std::uint32_t lineStepConstantAddress;
    std::uint8_t maxEffectMode;
    std::uint8_t dynamicEffectModeLimit;
    std::uint8_t fallbackEffectKind;
    std::uint8_t fallbackEffectArg;
    std::uint8_t protectedBypassMode;
    std::string_view evidenceLines;
};

struct Th07EclItemClearRadiusHelperContract {
    std::uint32_t functionAddress;
    std::uint32_t itemArrayAddress;
    std::uint32_t itemCount;
    std::uint32_t itemStrideBytes;
    std::uint32_t itemStrideDwords;
    std::uint32_t positionOffset;
    std::uint32_t stateOffset;
    std::uint16_t skippedState;
    std::uint32_t effectManagerAddress;
    std::uint32_t effectSpawnCallAddress;
    std::uint32_t effectSpawnFunctionAddress;
    std::uint8_t effectKind;
    std::uint8_t effectArg;
    std::string_view evidenceLines;
};

struct Th07EclVisualEffectSpawnContract {
    std::uint32_t functionAddress;
    std::uint32_t managerAddress;
    std::uint32_t slotCount;
    std::uint32_t slotSizeBytes;
    std::uint32_t fallbackSlotOffset;
    std::uint32_t nextIndexOffset;
    std::uint32_t positionOffset;
    std::uint32_t auxVectorOffset;
    std::uint32_t velocityOffset;
    std::uint32_t timerPreviousOffset;
    std::uint32_t timerSubframeOffset;
    std::uint32_t timerCurrentOffset;
    std::uint32_t modeOffset;
    std::uint32_t activeFlagOffset;
    std::uint32_t initializedFlagOffset;
    std::uint32_t variantOffset;
    std::uint32_t postInitByteOffset;
    std::uint32_t colorOffset;
    std::uint32_t renderFlagsOffset;
    std::uint32_t anmScriptOffset;
    std::uint32_t timerPreviousValue;
    std::uint32_t colorValue;
    std::uint32_t renderFlagBits;
    std::uint32_t anmScriptBase;
    std::uint32_t modeGatePointerAddress;
    std::uint32_t modeGateValueOffset;
    std::uint32_t modeGateThreshold;
    std::uint8_t modeGateReplacement;
    std::uint8_t modeGateFirstAffectedMode;
    std::uint8_t modeGateSecondAffectedMode;
    std::uint32_t floatToIntFunctionAddress;
    std::uint32_t randomFloatFunctionAddress;
    std::uint32_t anmManagerPointerAddress;
    std::uint32_t anmScriptTableOffset;
    std::uint32_t setScriptFunctionAddress;
    std::uint8_t randomVelocityMode;
    std::uint8_t forceVariantOneMode;
    std::uint8_t forceVariantZeroMode;
    std::uint32_t randomXScaleAddress;
    std::uint32_t randomXBiasAddress;
    std::uint32_t randomYScaleAddress;
    std::uint32_t randomYBiasAddress;
    std::string_view evidenceLines;
};

struct Th07EclProjectionHelperContract {
    Th07EclProjectionHelperKind kind;
    std::uint16_t opcode;
    std::uint32_t wrapperFunctionAddress;
    std::uint32_t coreFunctionAddress;
    std::uint32_t opcodeCallInstructionAddress;
    std::uint32_t finiteCheckCallAddress;
    std::uint32_t coreCallAddress;
    std::uint32_t finiteCheckFunctionAddress;
    std::uint32_t primaryMathInstructionAddress;
    std::uint32_t rangeReductionConstantLoadAddress;
    std::uint32_t rangeReductionLoopAddress;
    std::uint32_t reducedMathInstructionAddress;
    std::uint8_t mathErrorKind;
    std::uint32_t mathErrorContextAddress;
    std::string_view wrapperEvidenceLines;
    std::string_view coreEvidenceLines;
    std::string_view opcodeEvidenceLines;
};

struct Th07EclOpcode138RecordHelperContract {
    std::uint32_t functionAddress;
    std::uint32_t callInstructionAddress;
    std::uint32_t callThisPointerAddress;
    std::uint16_t opcode;
    std::uint32_t outputBaseOffset;
    std::uint32_t flagFieldOffset;
    std::uint8_t enabledBitMask;
    std::uint32_t segmentNumeratorOffset;
    std::uint32_t segmentDenominatorOffset;
    std::uint8_t segmentCountShift;
    std::uint32_t minSegmentCount;
    std::uint32_t failureReturn;
    std::uint32_t successReturn;
    std::uint32_t metadataPointerOffset;
    std::uint32_t ownerBaseXOffset;
    std::uint32_t ownerBaseYOffset;
    std::uint32_t ownerColorOffset;
    std::uint32_t metadataLeftXOffset;
    std::uint32_t metadataFirstLaneYOffset;
    std::uint32_t metadataStartXOffset;
    std::uint32_t metadataSecondLaneYOffset;
    std::uint32_t recordStrideBytes;
    std::uint32_t secondLaneBaseOffset;
    std::uint32_t recordScaleOffset;
    std::uint32_t recordColorOffset;
    std::uint32_t recordXOffset;
    std::uint32_t recordYOffset;
    std::uint32_t recordScaleBits;
    std::uint8_t firstLaneInitialIndex;
    std::uint8_t secondLaneInitialIndex;
    std::uint8_t indexStep;
    std::string_view helperEvidenceLines;
    std::string_view callEvidenceLines;
};

extern const std::array<Th07EclHighOpcodeCase, kTh07HighEclOpcodeCaseCount> kTh07HighEclOpcodeCases;
extern const std::array<Th07EclHighOpcodeSignature, kTh07HighEclOpcodeSignatureCount>
    kTh07HighEclOpcodeSignatures;
extern const std::array<Th07EclHighOpcodeEffectContract, kTh07HighEclDirectEffectContractCount>
    kTh07HighEclDirectEffectContracts;
extern const std::array<Th07EclHighOpcodeCallContract, kTh07HighEclCallContractCount>
    kTh07HighEclCallContracts;
extern const std::array<Th07EclFloatTargetResolverContract, kTh07EclResolveFloatTargetContractCount>
    kTh07EclFloatTargetResolverContracts;
extern const std::array<Th07EclReadResolverContract, kTh07EclIntReadResolverContractCount>
    kTh07EclIntReadResolverContracts;
extern const std::array<Th07EclReadResolverContract, kTh07EclFloatReadResolverContractCount>
    kTh07EclFloatReadResolverContracts;
extern const Th07EclClearHelperContract kTh07EclClearHelperContract;
extern const Th07EclItemClearRadiusHelperContract kTh07EclItemClearRadiusHelperContract;
extern const Th07EclVisualEffectSpawnContract kTh07EclVisualEffectSpawnContract;
extern const std::array<Th07EclProjectionHelperContract, kTh07EclProjectionHelperContractCount>
    kTh07EclProjectionHelperContracts;
extern const Th07EclOpcode138RecordHelperContract kTh07EclOpcode138RecordHelperContract;
extern const std::array<std::uint16_t, kTh07MissingEnumOpcodeCount> kTh07MissingEnumOpcodes;
extern const std::array<std::uint16_t, kTh07MissingDispatchOpcodeCount> kTh07MissingDispatchOpcodes;

const Th07EclHighOpcodeCase *FindTh07HighEclOpcodeCase(std::uint16_t opcode);
const Th07EclHighOpcodeEffectContract *FindTh07HighEclOpcodeEffectContract(std::uint16_t opcode,
                                                                           Th07EclEffectOwner owner,
                                                                           std::uint32_t fieldOffset);
const Th07EclHighOpcodeCallContract *FindTh07HighEclOpcodeCallContract(std::uint16_t opcode,
                                                                       std::uint32_t callInstructionAddress);
const Th07EclFloatTargetResolverContract *FindTh07EclFloatTargetResolverContract(std::uint16_t variableId);
const Th07EclReadResolverContract *FindTh07EclIntReadResolverContract(std::uint16_t variableId);
const Th07EclReadResolverContract *FindTh07EclFloatReadResolverContract(std::uint16_t variableId);
const Th07EclProjectionHelperContract *FindTh07EclProjectionHelperContract(Th07EclProjectionHelperKind kind);
std::size_t CountTh07HighEclOpcodeCases(Th07EclOpcodeStatus status);
std::size_t CountTh07HighEclOpcodeSignatures(std::uint16_t opcode);
std::size_t CountTh07HighEclOpcodeEffectContracts(std::uint16_t opcode);
std::size_t CountTh07HighEclOpcodeCallContracts(std::uint16_t opcode);
std::size_t CountTh07HighEclOpcodeCallContracts(Th07EclCallContractKind kind);
std::size_t CountTh07EclFloatTargetResolverContracts(Th07EclFloatTargetOwner owner);
std::size_t CountTh07EclIntReadResolverContracts(Th07EclReadSourceKind sourceKind);
std::size_t CountTh07EclFloatReadResolverContracts(Th07EclReadSourceKind sourceKind);
bool Th07HighEclOpcodeUsesOperandKind(std::uint16_t opcode, Th07EclOperandKind kind);
bool IsTh07HighEclOpcodeBlocked(std::uint16_t opcode);
bool IsTh07MissingEnumOpcode(std::uint16_t opcode);
bool IsTh07MissingDispatchOpcode(std::uint16_t opcode);

} // namespace th07
