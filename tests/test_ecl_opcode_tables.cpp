#include "EclOpcodeTables.hpp"

#include <cassert>
#include <cstddef>

int main()
{
    static_assert(th07::kTh07EclDispatchFunctionAddress == 0x00410520);
    static_assert(th07::kTh07EclOpcodeJumpTableAddress == 0x00417838);
    static_assert(th07::kTh07EclDispatchAdvanceAddress == 0x0041677b);
    static_assert(th07::kTh07EclOpcodeJumpTableFirstOpcode == 1);
    static_assert(th07::kTh07EclOpcodeJumpTableLastOpcode == 161);
    static_assert(th07::kTh07EclOpcodeJumpTableEntryCount == 161);
    static_assert(th07::kTh07DeclOpcodeCount == 141);
    static_assert(th07::kTh07CurrentEclEnumOpcodeCount == 136);
    static_assert(th07::kTh07CurrentEclHandledOpcodeCount == 134);
    static_assert(th07::kTh07MissingEnumOpcodeCount == 25);
    static_assert(th07::kTh07MissingDispatchOpcodeCount == 26);
    static_assert(th07::kTh07MissingNonNopDispatchOpcodeCount == 25);
    static_assert(th07::kTh07HighEclOpcodeCaseCount == 25);
    static_assert(th07::kTh07HighEclOpcodeSignatureCount == 31);
    static_assert(th07::kTh07HighEclDirectEffectContractCount == 41);
    static_assert(th07::kTh07HighEclCallContractCount == 17);
    static_assert(th07::kTh07EclResolveFloatTargetFunctionAddress == 0x0040f3c0);
    static_assert(th07::kTh07EclResolveFloatTargetJumpTableAddress == 0x0040f5d0);
    static_assert(th07::kTh07EclResolveFloatTargetClassTableAddress == 0x0040f668);
    static_assert(th07::kTh07EclResolveFloatTargetCaseBase == 0x2714);
    static_assert(th07::kTh07EclResolveFloatTargetMaxRelativeId == 0x45);
    static_assert(th07::kTh07EclResolveFloatTargetContractCount == 37);
    static_assert(th07::kTh07EclIntReadResolverFunctionAddress == 0x0040e5b0);
    static_assert(th07::kTh07EclIntReadResolverJumpTableAddress == 0x0040eace);
    static_assert(th07::kTh07EclIntReadResolverDefaultAddress == 0x0040eac7);
    static_assert(th07::kTh07EclFloatReadResolverFunctionAddress == 0x0040edf0);
    static_assert(th07::kTh07EclFloatReadResolverJumpTableAddress == 0x0040f28a);
    static_assert(th07::kTh07EclFloatReadResolverDefaultAddress == 0x0040f281);
    static_assert(th07::kTh07EclReadResolverCaseBase == 0x2710);
    static_assert(th07::kTh07EclReadResolverMaxRelativeId == 0x49);
    static_assert(th07::kTh07EclIntReadResolverContractCount == 70);
    static_assert(th07::kTh07EclFloatReadResolverContractCount == 74);
    static_assert(th07::kTh07EclReadResolverFloatToIntBridgeAddress == 0x0048b8a0);
    static_assert(th07::kTh07EclReadResolverRandomIntFunctionAddress == 0x004318d0);
    static_assert(th07::kTh07EclReadResolverRandomFloatFunctionAddress == 0x00431900);
    static_assert(th07::kTh07EclReadResolverVectorLengthFunctionAddress == 0x00442370);
    static_assert(th07::kTh07EclReadResolverDistanceFunctionAddress == 0x00404e70);
    static_assert(th07::kTh07EclClearHelperFunctionAddress == 0x00424740);
    static_assert(th07::kTh07EclClearHelperObjectBaseOffset == 0x366628);
    static_assert(th07::kTh07EclClearHelperObjectCount == 0x40);
    static_assert(th07::kTh07EclClearHelperObjectStrideBytes == 0x04ec);
    static_assert(th07::kTh07EclClearHelperObjectActiveOffset == 0x04d4);
    static_assert(th07::kTh07EclClearHelperObjectFlagsOffset == 0x04e4);
    static_assert(th07::kTh07EclClearHelperObjectProtectedFlagMask == 0x04);
    static_assert(th07::kTh07EclClearHelperObjectStateOffset == 0x04e8);
    static_assert(th07::kTh07EclClearHelperObjectStateValue == 2);
    static_assert(th07::kTh07EclClearHelperObjectTimerOffset == 0x04d8);
    static_assert(th07::kTh07EclClearHelperObjectTimerSentinel == 0xfffffc19);
    static_assert(th07::kTh07EclClearHelperObjectCopySourceOffset == 0x04b8);
    static_assert(th07::kTh07EclClearHelperObjectCopyDestOffset == 0x04b4);
    static_assert(th07::kTh07EclClearHelperObjectClearFlagOffset == 0x04d0);
    static_assert(th07::kTh07EclClearHelperObjectAngleOffset == 0x04a4);
    static_assert(th07::kTh07EclClearHelperObjectRangeStartOffset == 0x04a8);
    static_assert(th07::kTh07EclClearHelperObjectRangeEndOffset == 0x04ac);
    static_assert(th07::kTh07EclClearHelperObjectBaseXOffset == 0x0498);
    static_assert(th07::kTh07EclClearHelperObjectBaseYOffset == 0x049c);
    static_assert(th07::kTh07EclClearHelperManagerEffectKindOffset == 0x37a160);
    static_assert(th07::kTh07EclClearHelperFinalCooldownOffset == 0x37a12c);
    static_assert(th07::kTh07EclClearHelperFinalCooldownValue == 10);
    static_assert(th07::kTh07EclClearHelperEffectManagerAddress == 0x00575c70);
    static_assert(th07::kTh07EclClearHelperEffectSpawnFunctionAddress == 0x004326f0);
    static_assert(th07::kTh07EclClearHelperLineDirectionFunctionAddress == 0x00417af0);
    static_assert(th07::kTh07EclClearHelperLineStepConstantAddress == 0x00498a84);
    static_assert(th07::kTh07EclClearHelperMaxEffectMode == 8);
    static_assert(th07::kTh07EclClearHelperDynamicEffectModeLimit == 3);
    static_assert(th07::kTh07EclClearHelperFallbackEffectKind == 8);
    static_assert(th07::kTh07EclClearHelperFallbackEffectArg == 1);
    static_assert(th07::kTh07EclClearHelperProtectedBypassMode == 10);
    static_assert(th07::kTh07EclItemClearRadiusHelperFunctionAddress == 0x00424c00);
    static_assert(th07::kTh07EclItemClearRadiusItemArrayAddress == 0x0063b218);
    static_assert(th07::kTh07EclItemClearRadiusItemCount == 0x400);
    static_assert(th07::kTh07EclItemClearRadiusItemStrideBytes == 0x0d68);
    static_assert(th07::kTh07EclItemClearRadiusItemStrideDwords == 0x35a);
    static_assert(th07::kTh07EclItemClearRadiusPositionOffset == 0x0b8c);
    static_assert(th07::kTh07EclItemClearRadiusStateOffset == 0x0bfc);
    static_assert(th07::kTh07EclItemClearRadiusSkippedState == 5);
    static_assert(th07::kTh07EclItemClearRadiusEffectKind == 6);
    static_assert(th07::kTh07EclItemClearRadiusEffectArg == 1);
    static_assert(th07::kTh07EclVisualEffectSpawnFunctionAddress == 0x004326f0);
    static_assert(th07::kTh07EclVisualEffectManagerAddress == 0x00575c70);
    static_assert(th07::kTh07EclVisualEffectSlotCount == 0x44c);
    static_assert(th07::kTh07EclVisualEffectSlotSizeBytes == 0x0288);
    static_assert(th07::kTh07EclVisualEffectFallbackSlotOffset == 0x0ae060);
    static_assert(th07::kTh07EclVisualEffectNextIndexOffset == 0x0ae2e8);
    static_assert(th07::kTh07EclVisualEffectPositionOffset == 0x024c);
    static_assert(th07::kTh07EclVisualEffectAuxVectorOffset == 0x0258);
    static_assert(th07::kTh07EclVisualEffectVelocityOffset == 0x0264);
    static_assert(th07::kTh07EclVisualEffectTimerPreviousOffset == 0x0270);
    static_assert(th07::kTh07EclVisualEffectModeOffset == 0x027c);
    static_assert(th07::kTh07EclVisualEffectActiveFlagOffset == 0x027d);
    static_assert(th07::kTh07EclVisualEffectInitializedFlagOffset == 0x027e);
    static_assert(th07::kTh07EclVisualEffectVariantOffset == 0x027f);
    static_assert(th07::kTh07EclVisualEffectPostInitByteOffset == 0x0280);
    static_assert(th07::kTh07EclVisualEffectTimerPreviousValue == 0xfffffc19);
    static_assert(th07::kTh07EclVisualEffectRenderFlagBits == 0x1000);
    static_assert(th07::kTh07EclVisualEffectAnmScriptBase == 0x02c4);
    static_assert(th07::kTh07EclVisualEffectModeGatePointerAddress == 0x00626278);
    static_assert(th07::kTh07EclVisualEffectModeGateValueOffset == 0x007c);
    static_assert(th07::kTh07EclVisualEffectModeGateThreshold == 0x80);
    static_assert(th07::kTh07EclVisualEffectModeGateReplacement == 7);
    static_assert(th07::kTh07EclVisualEffectFloatToIntFunctionAddress == 0x0048b8a0);
    static_assert(th07::kTh07EclVisualEffectRandomFloatFunctionAddress == 0x00431900);
    static_assert(th07::kTh07EclVisualEffectAnmManagerPointerAddress == 0x004b9e44);
    static_assert(th07::kTh07EclVisualEffectAnmScriptTableOffset == 0x28ef0);
    static_assert(th07::kTh07EclVisualEffectSetScriptFunctionAddress == 0x0044ea20);
    static_assert(th07::kTh07EclProjectionOpcode == 151);
    static_assert(th07::kTh07EclProjectionHelperContractCount == 2);
    static_assert(th07::kTh07EclProjectionSinWrapperFunctionAddress == 0x0048bb40);
    static_assert(th07::kTh07EclProjectionSinCoreFunctionAddress == 0x0048bb5d);
    static_assert(th07::kTh07EclProjectionCosWrapperFunctionAddress == 0x0048bbf0);
    static_assert(th07::kTh07EclProjectionCosCoreFunctionAddress == 0x0048bc0d);
    static_assert(th07::kTh07EclProjectionFiniteCheckFunctionAddress == 0x00483d38);
    static_assert(th07::kTh07EclProjectionSinOpcodeCallInstructionAddress == 0x0041653f);
    static_assert(th07::kTh07EclProjectionCosOpcodeCallInstructionAddress == 0x004165de);
    static_assert(th07::kTh07EclProjectionSinFiniteCheckCallAddress == 0x0048bb46);
    static_assert(th07::kTh07EclProjectionSinCoreCallAddress == 0x0048bb4b);
    static_assert(th07::kTh07EclProjectionCosFiniteCheckCallAddress == 0x0048bbf6);
    static_assert(th07::kTh07EclProjectionCosCoreCallAddress == 0x0048bbfb);
    static_assert(th07::kTh07EclProjectionWrapperStackBytes == 0x0c);
    static_assert(th07::kTh07EclProjectionFpuDefaultControlWord == 0x027f);
    static_assert(th07::kTh07EclProjectionFpuC2StatusMask == 0x0400);
    static_assert(th07::kTh07EclProjectionFiniteExponentMask == 0x7ff00000);
    static_assert(th07::kTh07EclProjectionControlWordAddress == 0x004911b8);
    static_assert(th07::kTh07EclProjectionRangeReductionConstantAddress == 0x004911ba);
    static_assert(th07::kTh07EclProjectionIndefiniteConstantAddress == 0x0049e010);
    static_assert(th07::kTh07EclProjectionMathErrorFlagAddress == 0x0049f77c);
    static_assert(th07::kTh07EclProjectionSinMathErrorContextAddress == 0x0049f6e0);
    static_assert(th07::kTh07EclProjectionCosMathErrorContextAddress == 0x0049f6f0);
    static_assert(th07::kTh07EclProjectionSinPrimaryInstructionAddress == 0x0048bb72);
    static_assert(th07::kTh07EclProjectionSinRangeLoadAddress == 0x0048bb97);
    static_assert(th07::kTh07EclProjectionSinRangeLoopAddress == 0x0048bb9f);
    static_assert(th07::kTh07EclProjectionSinReducedInstructionAddress == 0x0048bba9);
    static_assert(th07::kTh07EclProjectionCosPrimaryInstructionAddress == 0x0048bc22);
    static_assert(th07::kTh07EclProjectionCosRangeLoadAddress == 0x0048bc47);
    static_assert(th07::kTh07EclProjectionCosRangeLoopAddress == 0x0048bc4f);
    static_assert(th07::kTh07EclProjectionCosReducedInstructionAddress == 0x0048bc59);
    static_assert(th07::kTh07EclProjectionSinMathErrorKind == 0x1e);
    static_assert(th07::kTh07EclProjectionCosMathErrorKind == 0x12);
    static_assert(th07::kTh07EclOpcode138RecordHelperFunctionAddress == 0x00455170);
    static_assert(th07::kTh07EclOpcode138RecordHelperCallInstructionAddress == 0x004160c5);
    static_assert(th07::kTh07EclOpcode138RecordHelperCallThisPointerAddress == 0x004b9e44);
    static_assert(th07::kTh07EclOpcode138RecordHelperOpcode == 138);
    static_assert(th07::kTh07EclOpcode138RecordHelperOutputBaseOffset == 0x39f8);
    static_assert(th07::kTh07EclOpcode138RecordHelperFlagFieldOffset == 0x4f30);
    static_assert(th07::kTh07EclOpcode138RecordHelperEnabledBitMask == 0x08);
    static_assert(th07::kTh07EclOpcode138RecordHelperSegmentNumeratorOffset == 0x4f32);
    static_assert(th07::kTh07EclOpcode138RecordHelperSegmentDenominatorOffset == 0x4f36);
    static_assert(th07::kTh07EclOpcode138RecordHelperSegmentCountShift == 1);
    static_assert(th07::kTh07EclOpcode138RecordHelperMinSegmentCount == 3);
    static_assert(th07::kTh07EclOpcode138RecordHelperFailureReturn == 0xffffffff);
    static_assert(th07::kTh07EclOpcode138RecordHelperSuccessReturn == 0);
    static_assert(th07::kTh07EclOpcode138RecordHelperMetadataPointerOffset == 0x01e4);
    static_assert(th07::kTh07EclOpcode138RecordHelperOwnerBaseXOffset == 0x0028);
    static_assert(th07::kTh07EclOpcode138RecordHelperOwnerBaseYOffset == 0x002c);
    static_assert(th07::kTh07EclOpcode138RecordHelperOwnerColorOffset == 0x01b8);
    static_assert(th07::kTh07EclOpcode138RecordHelperMetadataLeftXOffset == 0x001c);
    static_assert(th07::kTh07EclOpcode138RecordHelperMetadataFirstLaneYOffset == 0x0020);
    static_assert(th07::kTh07EclOpcode138RecordHelperMetadataStartXOffset == 0x0024);
    static_assert(th07::kTh07EclOpcode138RecordHelperMetadataSecondLaneYOffset == 0x0028);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordStrideBytes == 0x0038);
    static_assert(th07::kTh07EclOpcode138RecordHelperSecondLaneBaseOffset == 0x001c);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordScaleOffset == 0x000c);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordColorOffset == 0x0010);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordXOffset == 0x0014);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordYOffset == 0x0018);
    static_assert(th07::kTh07EclOpcode138RecordHelperRecordScaleBits == 0x3f800000);
    static_assert(th07::kTh07EclOpcode138RecordHelperFirstLaneInitialIndex == 0);
    static_assert(th07::kTh07EclOpcode138RecordHelperSecondLaneInitialIndex == 1);
    static_assert(th07::kTh07EclOpcode138RecordHelperIndexStep == 2);

    assert(th07::kTh07MissingEnumOpcodes.front() == 136);
    assert(th07::kTh07MissingEnumOpcodes.back() == 161);
    assert(th07::kTh07MissingDispatchOpcodes.front() == 0);
    assert(th07::kTh07MissingDispatchOpcodes.back() == 161);
    assert(th07::kTh07HighEclOpcodeCases.front().opcode == 136);
    assert(th07::kTh07HighEclOpcodeCases.back().opcode == 161);
    assert(th07::CountTh07HighEclOpcodeCases(th07::Th07EclOpcodeStatus::SourceDerived) == 25);
    assert(th07::CountTh07HighEclOpcodeCases(th07::Th07EclOpcodeStatus::Blocked) == 0);
    assert(th07::kTh07HighEclOpcodeSignatures.front().opcode == 136);
    assert(th07::kTh07HighEclOpcodeSignatures.back().opcode == 161);
    assert(th07::CountTh07HighEclOpcodeSignatures(139) == 4);
    assert(th07::CountTh07HighEclOpcodeSignatures(140) == 2);
    assert(th07::CountTh07HighEclOpcodeSignatures(146) == 1);
    assert(th07::CountTh07HighEclOpcodeSignatures(147) == 0);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(138) == 4);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(140) == 4);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(141) == 0);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(144) == 8);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(148) == 2);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(149) == 4);
    assert(th07::CountTh07HighEclOpcodeEffectContracts(161) == 1);
    assert(th07::CountTh07HighEclOpcodeCallContracts(138) == 1);
    assert(th07::CountTh07HighEclOpcodeCallContracts(151) == 4);
    assert(th07::CountTh07HighEclOpcodeCallContracts(154) == 3);
    assert(th07::CountTh07HighEclOpcodeCallContracts(155) == 5);
    assert(th07::CountTh07HighEclOpcodeCallContracts(th07::Th07EclCallContractKind::HelperCall) == 4);
    assert(th07::CountTh07HighEclOpcodeCallContracts(th07::Th07EclCallContractKind::ResolvedFloatTarget) == 5);
    assert(th07::CountTh07HighEclOpcodeCallContracts(th07::Th07EclCallContractKind::ProjectionHelper) == 3);
    assert(th07::CountTh07HighEclOpcodeCallContracts(th07::Th07EclCallContractKind::RandomFloat) == 4);
    assert(th07::CountTh07HighEclOpcodeCallContracts(th07::Th07EclCallContractKind::EffectSpawn) == 1);
    assert(th07::CountTh07EclFloatTargetResolverContracts(th07::Th07EclFloatTargetOwner::Enemy) == 30);
    assert(th07::CountTh07EclFloatTargetResolverContracts(th07::Th07EclFloatTargetOwner::Global) == 7);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::EnemyI32) == 21);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::EnemyU8) == 1);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::EnemyF32) == 30);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::GlobalI32) == 6);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::GlobalU8) == 1);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::GlobalF32) == 7);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::RandomInt) == 1);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::RandomIntModuloEnemyI32) == 1);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::VectorLength) == 1);
    assert(th07::CountTh07EclIntReadResolverContracts(th07::Th07EclReadSourceKind::DistanceToGlobals) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::EnemyI32) == 21);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::EnemyU8) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::EnemyF32) == 33);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::GlobalI32) == 6);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::GlobalU8) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::GlobalF32) == 7);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::RandomFloat) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::RandomFloatScaledEnemyF32) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::RandomFloatScaledGlobalF32) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::VectorLength) == 1);
    assert(th07::CountTh07EclFloatReadResolverContracts(th07::Th07EclReadSourceKind::DistanceToGlobals) == 1);
    assert(th07::Th07HighEclOpcodeUsesOperandKind(140, th07::Th07EclOperandKind::FloatReference));
    assert(th07::Th07HighEclOpcodeUsesOperandKind(154, th07::Th07EclOperandKind::IntReference));
    assert(!th07::Th07HighEclOpcodeUsesOperandKind(161, th07::Th07EclOperandKind::Float));
    const th07::Th07EclVisualEffectSpawnContract &visualSpawn = th07::kTh07EclVisualEffectSpawnContract;
    assert(visualSpawn.functionAddress == th07::kTh07EclVisualEffectSpawnFunctionAddress);
    assert(visualSpawn.managerAddress == th07::kTh07EclClearHelperEffectManagerAddress);
    assert(visualSpawn.managerAddress == th07::kTh07EclItemClearRadiusEffectManagerAddress);
    assert(visualSpawn.slotCount == 0x44c);
    assert(visualSpawn.slotSizeBytes == 0x0288);
    assert(visualSpawn.fallbackSlotOffset == 0x0ae060);
    assert(visualSpawn.nextIndexOffset == 0x0ae2e8);
    assert(visualSpawn.positionOffset == 0x024c);
    assert(visualSpawn.auxVectorOffset == 0x0258);
    assert(visualSpawn.velocityOffset == 0x0264);
    assert(visualSpawn.timerPreviousOffset == 0x0270);
    assert(visualSpawn.timerSubframeOffset == 0x0274);
    assert(visualSpawn.timerCurrentOffset == 0x0278);
    assert(visualSpawn.modeOffset == 0x027c);
    assert(visualSpawn.activeFlagOffset == 0x027d);
    assert(visualSpawn.initializedFlagOffset == 0x027e);
    assert(visualSpawn.variantOffset == 0x027f);
    assert(visualSpawn.postInitByteOffset == 0x0280);
    assert(visualSpawn.colorOffset == 0x01b8);
    assert(visualSpawn.renderFlagsOffset == 0x01c0);
    assert(visualSpawn.anmScriptOffset == 0x01d8);
    assert(visualSpawn.timerPreviousValue == 0xfffffc19);
    assert(visualSpawn.colorValue == 0xffffffff);
    assert(visualSpawn.renderFlagBits == 0x1000);
    assert(visualSpawn.anmScriptBase == 0x02c4);
    assert(visualSpawn.modeGatePointerAddress == 0x00626278);
    assert(visualSpawn.modeGateValueOffset == 0x007c);
    assert(visualSpawn.modeGateThreshold == 0x80);
    assert(visualSpawn.modeGateReplacement == 7);
    assert(visualSpawn.modeGateFirstAffectedMode == 0);
    assert(visualSpawn.modeGateSecondAffectedMode == 2);
    assert(visualSpawn.floatToIntFunctionAddress == 0x0048b8a0);
    assert(visualSpawn.randomFloatFunctionAddress == 0x00431900);
    assert(visualSpawn.anmManagerPointerAddress == 0x004b9e44);
    assert(visualSpawn.anmScriptTableOffset == 0x28ef0);
    assert(visualSpawn.setScriptFunctionAddress == 0x0044ea20);
    assert(visualSpawn.randomVelocityMode == 2);
    assert(visualSpawn.forceVariantOneMode == 3);
    assert(visualSpawn.forceVariantZeroMode == 4);
    assert(visualSpawn.randomXScaleAddress == 0x00498c78);
    assert(visualSpawn.randomXBiasAddress == 0x00498b34);
    assert(visualSpawn.randomYScaleAddress == 0x00498c74);
    assert(visualSpawn.randomYBiasAddress == 0x00498b08);
    assert(visualSpawn.evidenceLines == "004326f0:15-75");
    assert(th07::kTh07EclProjectionHelperContracts.size() == 2);
    const th07::Th07EclProjectionHelperContract *projectionSin =
        th07::FindTh07EclProjectionHelperContract(th07::Th07EclProjectionHelperKind::Sin);
    const th07::Th07EclProjectionHelperContract *projectionCos =
        th07::FindTh07EclProjectionHelperContract(th07::Th07EclProjectionHelperKind::Cos);
    assert(projectionSin != nullptr);
    assert(projectionCos != nullptr);
    assert(projectionSin->opcode == 151);
    assert(projectionSin->wrapperFunctionAddress == th07::kTh07EclProjectionSinWrapperFunctionAddress);
    assert(projectionSin->coreFunctionAddress == th07::kTh07EclProjectionSinCoreFunctionAddress);
    assert(projectionSin->opcodeCallInstructionAddress == th07::kTh07EclProjectionSinOpcodeCallInstructionAddress);
    assert(projectionSin->finiteCheckCallAddress == th07::kTh07EclProjectionSinFiniteCheckCallAddress);
    assert(projectionSin->coreCallAddress == th07::kTh07EclProjectionSinCoreCallAddress);
    assert(projectionSin->finiteCheckFunctionAddress == th07::kTh07EclProjectionFiniteCheckFunctionAddress);
    assert(projectionSin->primaryMathInstructionAddress == th07::kTh07EclProjectionSinPrimaryInstructionAddress);
    assert(projectionSin->rangeReductionConstantLoadAddress == th07::kTh07EclProjectionSinRangeLoadAddress);
    assert(projectionSin->rangeReductionLoopAddress == th07::kTh07EclProjectionSinRangeLoopAddress);
    assert(projectionSin->reducedMathInstructionAddress == th07::kTh07EclProjectionSinReducedInstructionAddress);
    assert(projectionSin->mathErrorKind == 0x1e);
    assert(projectionSin->mathErrorContextAddress == 0x0049f6e0);
    assert(projectionSin->wrapperEvidenceLines == "0048bb40:8-10");
    assert(projectionSin->coreEvidenceLines == "0048bb5d:15-40");
    assert(projectionSin->opcodeEvidenceLines == "00410520:3083-3096");
    assert(projectionCos->opcode == 151);
    assert(projectionCos->wrapperFunctionAddress == th07::kTh07EclProjectionCosWrapperFunctionAddress);
    assert(projectionCos->coreFunctionAddress == th07::kTh07EclProjectionCosCoreFunctionAddress);
    assert(projectionCos->opcodeCallInstructionAddress == th07::kTh07EclProjectionCosOpcodeCallInstructionAddress);
    assert(projectionCos->finiteCheckCallAddress == th07::kTh07EclProjectionCosFiniteCheckCallAddress);
    assert(projectionCos->coreCallAddress == th07::kTh07EclProjectionCosCoreCallAddress);
    assert(projectionCos->finiteCheckFunctionAddress == th07::kTh07EclProjectionFiniteCheckFunctionAddress);
    assert(projectionCos->primaryMathInstructionAddress == th07::kTh07EclProjectionCosPrimaryInstructionAddress);
    assert(projectionCos->rangeReductionConstantLoadAddress == th07::kTh07EclProjectionCosRangeLoadAddress);
    assert(projectionCos->rangeReductionLoopAddress == th07::kTh07EclProjectionCosRangeLoopAddress);
    assert(projectionCos->reducedMathInstructionAddress == th07::kTh07EclProjectionCosReducedInstructionAddress);
    assert(projectionCos->mathErrorKind == 0x12);
    assert(projectionCos->mathErrorContextAddress == 0x0049f6f0);
    assert(projectionCos->wrapperEvidenceLines == "0048bbf0:8-10");
    assert(projectionCos->coreEvidenceLines == "0048bc0d:15-40");
    assert(projectionCos->opcodeEvidenceLines == "00410520:3097-3110");
    const th07::Th07EclOpcode138RecordHelperContract &opcode138Record =
        th07::kTh07EclOpcode138RecordHelperContract;
    assert(opcode138Record.functionAddress == th07::kTh07EclOpcode138RecordHelperFunctionAddress);
    assert(opcode138Record.callInstructionAddress == th07::kTh07EclOpcode138RecordHelperCallInstructionAddress);
    assert(opcode138Record.callThisPointerAddress == 0x004b9e44);
    assert(opcode138Record.opcode == 138);
    assert(opcode138Record.outputBaseOffset == 0x39f8);
    assert(opcode138Record.flagFieldOffset == 0x4f30);
    assert(opcode138Record.enabledBitMask == 0x08);
    assert(opcode138Record.segmentNumeratorOffset == 0x4f32);
    assert(opcode138Record.segmentDenominatorOffset == 0x4f36);
    assert(opcode138Record.segmentCountShift == 1);
    assert(opcode138Record.minSegmentCount == 3);
    assert(opcode138Record.failureReturn == 0xffffffff);
    assert(opcode138Record.successReturn == 0);
    assert(opcode138Record.metadataPointerOffset == 0x01e4);
    assert(opcode138Record.ownerBaseXOffset == 0x0028);
    assert(opcode138Record.ownerBaseYOffset == 0x002c);
    assert(opcode138Record.ownerColorOffset == 0x01b8);
    assert(opcode138Record.metadataLeftXOffset == 0x001c);
    assert(opcode138Record.metadataFirstLaneYOffset == 0x0020);
    assert(opcode138Record.metadataStartXOffset == 0x0024);
    assert(opcode138Record.metadataSecondLaneYOffset == 0x0028);
    assert(opcode138Record.recordStrideBytes == 0x0038);
    assert(opcode138Record.secondLaneBaseOffset == 0x001c);
    assert(opcode138Record.recordScaleOffset == 0x000c);
    assert(opcode138Record.recordColorOffset == 0x0010);
    assert(opcode138Record.recordXOffset == 0x0014);
    assert(opcode138Record.recordYOffset == 0x0018);
    assert(opcode138Record.recordScaleBits == 0x3f800000);
    assert(opcode138Record.firstLaneInitialIndex == 0);
    assert(opcode138Record.secondLaneInitialIndex == 1);
    assert(opcode138Record.indexStep == 2);
    assert(opcode138Record.helperEvidenceLines == "00455170:15-48");
    assert(opcode138Record.callEvidenceLines == "00410520:2833-2860");

    for (std::size_t idx = 1; idx < th07::kTh07HighEclOpcodeCases.size(); ++idx) {
        assert(th07::kTh07HighEclOpcodeCases[idx - 1].opcode < th07::kTh07HighEclOpcodeCases[idx].opcode);
    }

    const th07::Th07EclHighOpcodeCase *opcode136 = th07::FindTh07HighEclOpcodeCase(136);
    assert(opcode136 != nullptr);
    assert(opcode136->caseLabel == 0x87);
    assert(opcode136->jumpTableEntryAddress == 0x00417a54);
    assert(opcode136->targetAddress == 0x00415f5a);
    assert(opcode136->hasDispatchCase());
    assert(opcode136->hasDecompilerCase());
    assert(opcode136->evidenceLineStart == 2825);
    assert(opcode136->evidenceLineEnd == 2829);
    const th07::Th07EclHighOpcodeEffectContract *opcode136Bit =
        th07::FindTh07HighEclOpcodeEffectContract(136, th07::Th07EclEffectOwner::Enemy, 0x2e29);
    assert(opcode136Bit != nullptr);
    assert(opcode136Bit->writeKind == th07::Th07EclEffectWriteKind::Bit);
    assert(opcode136Bit->valueOperandIndex == 0);
    assert(opcode136Bit->bitIndex == 5);

    const th07::Th07EclHighOpcodeCase *opcode142 = th07::FindTh07HighEclOpcodeCase(142);
    assert(opcode142 != nullptr);
    assert(opcode142->declCount == 292);
    assert(opcode142->caseLabel == 0x8d);

    const th07::Th07EclHighOpcodeEffectContract *opcode138Table =
        th07::FindTh07HighEclOpcodeEffectContract(138, th07::Th07EclEffectOwner::Enemy, 0x4f30);
    assert(opcode138Table != nullptr);
    assert(opcode138Table->writeKind == th07::Th07EclEffectWriteKind::Byte);
    assert(opcode138Table->valueOperandIndex == 0);
    const th07::Th07EclHighOpcodeCallContract *opcode138Call =
        th07::FindTh07HighEclOpcodeCallContract(138, 0x004160c5);
    assert(opcode138Call != nullptr);
    assert(opcode138Call->calleeAddress == opcode138Record.functionAddress);
    assert(opcode138Call->kind == th07::Th07EclCallContractKind::HelperCall);
    assert(opcode138Call->caseTargetAddress == 0x00415fb4);

    const th07::Th07EclHighOpcodeEffectContract *opcode139Table =
        th07::FindTh07HighEclOpcodeEffectContract(139, th07::Th07EclEffectOwner::GlobalIndexedTable, 0);
    assert(opcode139Table != nullptr);
    assert(opcode139Table->ownerOffsetOrAddress == 0x0049fc44);
    assert(opcode139Table->divisorOffset == 0x2bbc);
    assert(opcode139Table->indexOperandIndex == 0);
    assert(opcode139Table->valueOperandIndex == 1);

    const th07::Th07EclHighOpcodeEffectContract *opcode140Global =
        th07::FindTh07HighEclOpcodeEffectContract(140, th07::Th07EclEffectOwner::Global, 0);
    assert(opcode140Global != nullptr);
    assert(opcode140Global->ownerOffsetOrAddress == 0x012fe25c);

    const th07::Th07EclHighOpcodeEffectContract *opcode143Copy =
        th07::FindTh07HighEclOpcodeEffectContract(144, th07::Th07EclEffectOwner::Enemy, 0x2ee8);
    assert(opcode143Copy != nullptr);
    assert(opcode143Copy->writeKind == th07::Th07EclEffectWriteKind::CopyDwords);
    assert(opcode143Copy->sourceOffset == 0x06fc);
    assert(opcode143Copy->elementCount == 0x1a);

    const th07::Th07EclHighOpcodeEffectContract *opcode144Table =
        th07::FindTh07HighEclOpcodeEffectContract(145, th07::Th07EclEffectOwner::GlobalPointerTableEntry, 0x2b08);
    assert(opcode144Table != nullptr);
    assert(opcode144Table->ownerOffsetOrAddress == 0x012fe098);
    assert(opcode144Table->indexOperandIndex == 0);
    assert(opcode144Table->valueOperandIndex == 1);

    const th07::Th07EclHighOpcodeEffectContract *opcode148Indexed =
        th07::FindTh07HighEclOpcodeEffectContract(148, th07::Th07EclEffectOwner::Enemy, 0x2ecc);
    assert(opcode148Indexed != nullptr);
    assert(opcode148Indexed->indexOperandIndex == 0);
    assert(opcode148Indexed->valueOperandIndex == 2);

    const th07::Th07EclHighOpcodeEffectContract *opcode148Pointed =
        th07::FindTh07HighEclOpcodeEffectContract(149, th07::Th07EclEffectOwner::EnemyPointerTableEntry, 0x0254);
    assert(opcode148Pointed != nullptr);
    assert(opcode148Pointed->ownerOffsetOrAddress == 0x2eb0);
    assert(opcode148Pointed->writeKind == th07::Th07EclEffectWriteKind::F32);
    assert(opcode148Pointed->valueOperandIndex == 3);

    const th07::Th07EclHighOpcodeCallContract *opcode151FirstTarget =
        th07::FindTh07HighEclOpcodeCallContract(151, 0x00416569);
    assert(opcode151FirstTarget != nullptr);
    assert(opcode151FirstTarget->calleeAddress == 0x0040f3c0);
    assert(opcode151FirstTarget->kind == th07::Th07EclCallContractKind::ResolvedFloatTarget);
    assert(opcode151FirstTarget->primaryOperandIndex == 1);
    assert(opcode151FirstTarget->secondaryOperandIndex == 3);
    const th07::Th07EclHighOpcodeCallContract *opcode151Projection =
        th07::FindTh07HighEclOpcodeCallContract(151, 0x004165de);
    assert(opcode151Projection != nullptr);
    assert(opcode151Projection->calleeAddress == projectionCos->wrapperFunctionAddress);
    assert(opcode151Projection->kind == th07::Th07EclCallContractKind::ProjectionHelper);
    const th07::Th07EclHighOpcodeCallContract *opcode151Sin =
        th07::FindTh07HighEclOpcodeCallContract(151, projectionSin->opcodeCallInstructionAddress);
    assert(opcode151Sin != nullptr);
    assert(opcode151Sin->calleeAddress == projectionSin->wrapperFunctionAddress);
    assert(opcode151Sin->kind == th07::Th07EclCallContractKind::ProjectionHelper);

    const th07::Th07EclHighOpcodeCallContract *opcode154Spawn =
        th07::FindTh07HighEclOpcodeCallContract(154, 0x004155a5);
    assert(opcode154Spawn != nullptr);
    assert(opcode154Spawn->calleeAddress == visualSpawn.functionAddress);
    assert(opcode154Spawn->kind == th07::Th07EclCallContractKind::EffectSpawn);

    const th07::Th07EclHighOpcodeCallContract *opcode155Target =
        th07::FindTh07HighEclOpcodeCallContract(155, 0x00416696);
    assert(opcode155Target != nullptr);
    assert(opcode155Target->calleeAddress == 0x0040f3c0);
    assert(opcode155Target->kind == th07::Th07EclCallContractKind::ResolvedFloatTarget);

    const th07::Th07EclHighOpcodeCallContract *opcode159Target =
        th07::FindTh07HighEclOpcodeCallContract(159, 0x004116db);
    assert(opcode159Target != nullptr);
    assert(opcode159Target->calleeAddress == 0x0040f3c0);
    assert(opcode159Target->caseLabel == 0x9e);

    const th07::Th07EclFloatTargetResolverContract *targetLocal0 =
        th07::FindTh07EclFloatTargetResolverContract(0x2714);
    assert(targetLocal0 != nullptr);
    assert(targetLocal0->owner == th07::Th07EclFloatTargetOwner::Enemy);
    assert(targetLocal0->ownerOffsetOrAddress == 0x070c);
    assert(targetLocal0->caseAddress == 0x0040f422);
    const th07::Th07EclFloatTargetResolverContract *targetPositionX =
        th07::FindTh07EclFloatTargetResolverContract(0x2722);
    assert(targetPositionX != nullptr);
    assert(targetPositionX->owner == th07::Th07EclFloatTargetOwner::Enemy);
    assert(targetPositionX->ownerOffsetOrAddress == 0x2b0c);
    const th07::Th07EclFloatTargetResolverContract *targetGlobalA =
        th07::FindTh07EclFloatTargetResolverContract(0x2725);
    assert(targetGlobalA != nullptr);
    assert(targetGlobalA->owner == th07::Th07EclFloatTargetOwner::Global);
    assert(targetGlobalA->ownerOffsetOrAddress == 0x004be408);
    const th07::Th07EclFloatTargetResolverContract *targetGlobalB =
        th07::FindTh07EclFloatTargetResolverContract(0x2739);
    assert(targetGlobalB != nullptr);
    assert(targetGlobalB->owner == th07::Th07EclFloatTargetOwner::Global);
    assert(targetGlobalB->ownerOffsetOrAddress == 0x01347ab0);
    const th07::Th07EclFloatTargetResolverContract *targetMotion =
        th07::FindTh07EclFloatTargetResolverContract(0x2746);
    assert(targetMotion != nullptr);
    assert(targetMotion->ownerOffsetOrAddress == 0x2b60);
    assert(targetMotion->caseAddress == 0x0040f5bd);
    const th07::Th07EclFloatTargetResolverContract *targetTail =
        th07::FindTh07EclFloatTargetResolverContract(0x2759);
    assert(targetTail != nullptr);
    assert(targetTail->ownerOffsetOrAddress == 0x0740);
    assert(targetTail->caseAddress == 0x0040f510);
    assert(th07::FindTh07EclFloatTargetResolverContract(0x2720) == nullptr);

    const th07::Th07EclReadResolverContract *intLocal0 =
        th07::FindTh07EclIntReadResolverContract(0x2710);
    assert(intLocal0 != nullptr);
    assert(intLocal0->sourceKind == th07::Th07EclReadSourceKind::EnemyI32);
    assert(intLocal0->primaryOffsetOrAddress == 0x06fc);
    assert(intLocal0->helperAddress == th07::kTh07EclReadResolverNoHelper);
    const th07::Th07EclReadResolverContract *intFloatLocal0 =
        th07::FindTh07EclIntReadResolverContract(0x2714);
    assert(intFloatLocal0 != nullptr);
    assert(intFloatLocal0->sourceKind == th07::Th07EclReadSourceKind::EnemyF32);
    assert(intFloatLocal0->primaryOffsetOrAddress == 0x070c);
    assert(intFloatLocal0->helperAddress == th07::kTh07EclReadResolverFloatToIntBridgeAddress);
    const th07::Th07EclReadResolverContract *intDistance =
        th07::FindTh07EclIntReadResolverContract(0x272a);
    assert(intDistance != nullptr);
    assert(intDistance->sourceKind == th07::Th07EclReadSourceKind::DistanceToGlobals);
    assert(intDistance->primaryOffsetOrAddress == 0x2b0c);
    assert(intDistance->secondaryOffsetOrAddress == 0x004be408);
    assert(intDistance->helperAddress == th07::kTh07EclReadResolverDistanceFunctionAddress);
    const th07::Th07EclReadResolverContract *intRandomRange =
        th07::FindTh07EclIntReadResolverContract(0x2748);
    assert(intRandomRange != nullptr);
    assert(intRandomRange->sourceKind == th07::Th07EclReadSourceKind::RandomIntModuloEnemyI32);
    assert(intRandomRange->primaryOffsetOrAddress == 0x0744);
    assert(intRandomRange->secondaryOffsetOrAddress == 0x0748);
    assert(th07::FindTh07EclIntReadResolverContract(0x2749) == nullptr);

    const th07::Th07EclReadResolverContract *floatLocal0 =
        th07::FindTh07EclFloatReadResolverContract(0x2710);
    assert(floatLocal0 != nullptr);
    assert(floatLocal0->sourceKind == th07::Th07EclReadSourceKind::EnemyI32);
    assert(floatLocal0->primaryOffsetOrAddress == 0x06fc);
    const th07::Th07EclReadResolverContract *floatPositionX =
        th07::FindTh07EclFloatReadResolverContract(0x2722);
    assert(floatPositionX != nullptr);
    assert(floatPositionX->sourceKind == th07::Th07EclReadSourceKind::EnemyF32);
    assert(floatPositionX->primaryOffsetOrAddress == 0x2b0c);
    const th07::Th07EclReadResolverContract *floatRandomRange =
        th07::FindTh07EclFloatReadResolverContract(0x2748);
    assert(floatRandomRange != nullptr);
    assert(floatRandomRange->sourceKind == th07::Th07EclReadSourceKind::RandomFloatScaledEnemyF32);
    assert(floatRandomRange->primaryOffsetOrAddress == 0x0754);
    assert(floatRandomRange->secondaryOffsetOrAddress == 0x0758);
    const th07::Th07EclReadResolverContract *floatGlobalRange =
        th07::FindTh07EclFloatReadResolverContract(0x274c);
    assert(floatGlobalRange != nullptr);
    assert(floatGlobalRange->sourceKind == th07::Th07EclReadSourceKind::RandomFloatScaledGlobalF32);
    assert(floatGlobalRange->primaryOffsetOrAddress == 0x00498a60);
    assert(floatGlobalRange->secondaryOffsetOrAddress == 0x00498a64);
    assert(th07::FindTh07EclFloatReadResolverContract(0x275a) == nullptr);

    assert(th07::kTh07EclClearHelperContract.functionAddress == 0x00424740);
    assert(th07::kTh07EclClearHelperContract.itemArrayAddress == 0x0063b218);
    assert(th07::kTh07EclClearHelperContract.itemCount == 0x400);
    assert(th07::kTh07EclClearHelperContract.itemStrideBytes == 0x0d68);
    assert(th07::kTh07EclClearHelperContract.itemPositionOffset == 0x0b8c);
    assert(th07::kTh07EclClearHelperContract.itemStateOffset == 0x0bfc);
    assert(th07::kTh07EclClearHelperContract.itemSkippedState == 5);
    assert(th07::kTh07EclClearHelperContract.objectBaseOffset == 0x366628);
    assert(th07::kTh07EclClearHelperContract.objectCount == 0x40);
    assert(th07::kTh07EclClearHelperContract.objectStrideBytes == 0x04ec);
    assert(th07::kTh07EclClearHelperContract.objectActiveOffset == 0x04d4);
    assert(th07::kTh07EclClearHelperContract.objectFlagsOffset == 0x04e4);
    assert(th07::kTh07EclClearHelperContract.objectProtectedFlagMask == 0x04);
    assert(th07::kTh07EclClearHelperContract.objectStateOffset == 0x04e8);
    assert(th07::kTh07EclClearHelperContract.objectStateValue == 2);
    assert(th07::kTh07EclClearHelperContract.objectTimerOffset == 0x04d8);
    assert(th07::kTh07EclClearHelperContract.objectTimerSentinel == 0xfffffc19);
    assert(th07::kTh07EclClearHelperContract.objectCopySourceOffset == 0x04b8);
    assert(th07::kTh07EclClearHelperContract.objectCopyDestOffset == 0x04b4);
    assert(th07::kTh07EclClearHelperContract.objectClearFlagOffset == 0x04d0);
    assert(th07::kTh07EclClearHelperContract.objectAngleOffset == 0x04a4);
    assert(th07::kTh07EclClearHelperContract.objectRangeStartOffset == 0x04a8);
    assert(th07::kTh07EclClearHelperContract.objectRangeEndOffset == 0x04ac);
    assert(th07::kTh07EclClearHelperContract.objectBaseXOffset == 0x0498);
    assert(th07::kTh07EclClearHelperContract.objectBaseYOffset == 0x049c);
    assert(th07::kTh07EclClearHelperContract.managerEffectKindOffset == 0x37a160);
    assert(th07::kTh07EclClearHelperContract.finalCooldownOffset == 0x37a12c);
    assert(th07::kTh07EclClearHelperContract.finalCooldownValue == 10);
    assert(th07::kTh07EclClearHelperContract.effectManagerAddress == 0x00575c70);
    assert(th07::kTh07EclClearHelperContract.effectSpawnFunctionAddress == 0x004326f0);
    assert(th07::kTh07EclClearHelperContract.lineDirectionFunctionAddress == 0x00417af0);
    assert(th07::kTh07EclClearHelperContract.lineStepConstantAddress == 0x00498a84);
    assert(th07::kTh07EclClearHelperContract.maxEffectMode == 8);
    assert(th07::kTh07EclClearHelperContract.dynamicEffectModeLimit == 3);
    assert(th07::kTh07EclClearHelperContract.fallbackEffectKind == 8);
    assert(th07::kTh07EclClearHelperContract.fallbackEffectArg == 1);
    assert(th07::kTh07EclClearHelperContract.protectedBypassMode == 10);
    const th07::Th07EclHighOpcodeCallContract *opcode146Clear =
        th07::FindTh07HighEclOpcodeCallContract(146, 0x00416349);
    assert(opcode146Clear != nullptr);
    assert(opcode146Clear->calleeAddress == th07::kTh07EclClearHelperContract.functionAddress);

    const th07::Th07EclHighOpcodeCase *opcode160 = th07::FindTh07HighEclOpcodeCase(160);
    assert(opcode160 != nullptr);
    assert(opcode160->firstUse == "ecldata1.decl:718");
    assert(opcode160->caseLabel == 0x9f);
    const th07::Th07EclHighOpcodeCallContract *opcode160Call =
        th07::FindTh07HighEclOpcodeCallContract(160, 0x00416722);
    assert(opcode160Call != nullptr);
    assert(opcode160Call->calleeAddress == 0x0042f5a2);
    assert(opcode160Call->primaryOperandIndex == 0);

    assert(th07::kTh07EclItemClearRadiusHelperContract.functionAddress == 0x00424c00);
    assert(th07::kTh07EclItemClearRadiusHelperContract.itemArrayAddress == 0x0063b218);
    assert(th07::kTh07EclItemClearRadiusHelperContract.itemCount == 0x400);
    assert(th07::kTh07EclItemClearRadiusHelperContract.itemStrideBytes == 0x0d68);
    assert(th07::kTh07EclItemClearRadiusHelperContract.itemStrideDwords == 0x35a);
    assert(th07::kTh07EclItemClearRadiusHelperContract.positionOffset == 0x0b8c);
    assert(th07::kTh07EclItemClearRadiusHelperContract.stateOffset == 0x0bfc);
    assert(th07::kTh07EclItemClearRadiusHelperContract.skippedState == 5);
    assert(th07::kTh07EclItemClearRadiusHelperContract.effectManagerAddress == 0x00575c70);
    assert(th07::kTh07EclItemClearRadiusHelperContract.effectSpawnCallAddress == 0x00424cf6);
    assert(th07::kTh07EclItemClearRadiusHelperContract.effectSpawnFunctionAddress == 0x004326f0);
    assert(th07::kTh07EclItemClearRadiusHelperContract.effectKind == 6);
    assert(th07::kTh07EclItemClearRadiusHelperContract.effectArg == 1);
    const th07::Th07EclHighOpcodeCallContract *opcode143ItemClear =
        th07::FindTh07HighEclOpcodeCallContract(143, 0x0041627a);
    assert(opcode143ItemClear != nullptr);
    assert(opcode143ItemClear->calleeAddress == th07::kTh07EclItemClearRadiusHelperContract.functionAddress);

    const th07::Th07EclHighOpcodeCase *opcode140 = th07::FindTh07HighEclOpcodeCase(140);
    assert(opcode140 != nullptr);
    assert(opcode140->hasDispatchCase());
    assert(opcode140->hasDecompilerCase());
    assert(opcode140->caseLabel == 0x8b);
    assert(opcode140->targetAddress == 0x004160cf);
    assert(opcode140->status == th07::Th07EclOpcodeStatus::SourceDerived);

    const th07::Th07EclHighOpcodeCase *opcode141 = th07::FindTh07HighEclOpcodeCase(141);
    assert(opcode141 != nullptr);
    assert(opcode141->hasDispatchCase());
    assert(!opcode141->hasDecompilerCase());
    assert(opcode141->targetsCommonAdvance());
    assert(opcode141->jumpTableEntryAddress == 0x00417a68);

    const th07::Th07EclHighOpcodeCase *opcode161 = th07::FindTh07HighEclOpcodeCase(161);
    assert(opcode161 != nullptr);
    assert(opcode161->caseLabel == 0xa0);
    assert(opcode161->targetAddress == 0x00416729);
    assert(!th07::IsTh07HighEclOpcodeBlocked(140));
    assert(!th07::IsTh07HighEclOpcodeBlocked(161));
    assert(!th07::IsTh07HighEclOpcodeBlocked(136));
    assert(th07::IsTh07MissingEnumOpcode(136));
    assert(th07::IsTh07MissingEnumOpcode(161));
    assert(!th07::IsTh07MissingEnumOpcode(135));
    assert(th07::IsTh07MissingDispatchOpcode(0));
    assert(th07::IsTh07MissingDispatchOpcode(161));
    assert(!th07::IsTh07MissingDispatchOpcode(1));
    assert(th07::FindTh07HighEclOpcodeCase(147) == nullptr);

    const th07::Th07EclHighOpcodeSignature &opcode146Signature = th07::kTh07HighEclOpcodeSignatures[15];
    assert(opcode146Signature.opcode == 146);
    assert(opcode146Signature.operandCount == 0);
    assert(opcode146Signature.count == 1);
    assert(opcode146Signature.sample == "ins_146()");

    const th07::Th07EclHighOpcodeSignature &opcode151Signature = th07::kTh07HighEclOpcodeSignatures[19];
    assert(opcode151Signature.opcode == 151);
    assert(opcode151Signature.operandCount == 4);
    assert(opcode151Signature.usesOperandKind(th07::Th07EclOperandKind::FloatReference));
    assert(opcode151Signature.usesOperandKind(th07::Th07EclOperandKind::Float));

    return 0;
}
