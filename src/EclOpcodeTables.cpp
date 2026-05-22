#include "EclOpcodeTables.hpp"

namespace th07 {

const std::array<std::uint16_t, kTh07MissingEnumOpcodeCount> kTh07MissingEnumOpcodes = {{
    136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
}};

const std::array<std::uint16_t, kTh07MissingDispatchOpcodeCount> kTh07MissingDispatchOpcodes = {{
    0, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
}};

const std::array<Th07EclHighOpcodeEffectContract, kTh07HighEclDirectEffectContractCount>
    kTh07HighEclDirectEffectContracts = {{
        {136, 0x87, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::Bit, 0,
         0x2e29, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, 5, 1, "00410520:2825-2829"},
        {137, 0x88, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::Bit, 0,
         0x2e2a, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, 7, 1, "00410520:2830-2831"},
        {138, 0x89, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::Byte, 0,
         0x4f30, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2833-2834"},
        {138, 0x89, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I16, 0,
         0x4f32, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2835-2842"},
        {138, 0x89, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I16, 0,
         0x4f34, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2843-2850"},
        {138, 0x89, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I16, 0,
         0x4f36, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 3,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2851-2860"},
        {139, 0x8a, Th07EclEffectOwner::GlobalIndexedTable, Th07EclEffectWriteKind::F32,
         0x0049fc44, 0, kTh07EclEffectNoOffset, 0x2bbc, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:2861-2885"},
        {139, 0x8a, Th07EclEffectOwner::GlobalIndexedTable, Th07EclEffectWriteKind::F32,
         0x0049fc24, 0, kTh07EclEffectNoOffset, 0x2bbc, 2, 0,
         kTh07EclEffectNoBit, 1, "00410520:2861-2885"},
        {139, 0x8a, Th07EclEffectOwner::GlobalIndexedTable, Th07EclEffectWriteKind::I32,
         0x0049fc64, 0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 3, 0,
         kTh07EclEffectNoBit, 1, "00410520:2886-2891"},
        {140, 0x8b, Th07EclEffectOwner::Global, Th07EclEffectWriteKind::F32,
         0x012fe25c, 0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2892-2900"},
        {140, 0x8b, Th07EclEffectOwner::Global, Th07EclEffectWriteKind::F32,
         0x012fe260, 0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2901-2908"},
        {140, 0x8b, Th07EclEffectOwner::Global, Th07EclEffectWriteKind::F32,
         0x012fe264, 0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2909-2917"},
        {140, 0x8b, Th07EclEffectOwner::Global, Th07EclEffectWriteKind::F32,
         0x012fe268, 0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 3,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2918-2925"},
        {142, 0x8d, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I32, 0,
         0x4f40, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2926-2934"},
        {142, 0x8d, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerSubframeZero, 0,
         0x4f3c, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2935"},
        {142, 0x8d, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerPreviousMinus999, 0,
         0x4f38, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2936"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I32, 0,
         0x2f58, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2947-2955"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerSubframeZero, 0,
         0x2f54, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2956"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerPreviousMinus999, 0,
         0x2f50, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2957"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I32, 0,
         0x2ee4, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:2958-2965"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerCurrentZero, 0,
         0x2f64, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2966"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerSubframeZero, 0,
         0x2f60, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2967"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::TimerPreviousMinus999, 0,
         0x2f5c, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1,
         "00410520:2968"},
        {144, 0x8f, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::CopyDwords, 0,
         0x2ee8, 0x06fc, kTh07EclEffectNoOffset,
         kTh07EclEffectNoOperand, kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 0x1a,
         "00410520:2969-2974"},
        {145, 0x90, Th07EclEffectOwner::GlobalPointerTableEntry, Th07EclEffectWriteKind::I32,
         0x012fe098, 0x2b08, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:2975-2997"},
        {148, 0x93, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I32, 0,
         0x2ebc, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:3010-3023"},
        {148, 0x93, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::I32, 0,
         0x2ecc, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2, 0,
         kTh07EclEffectNoBit, 1, "00410520:3024-3037"},
        {149, 0x94, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::Bit, 0,
         0x2e2b, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, 1, 1, "00410520:3038-3046"},
        {149, 0x94, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2eb0, 0x024c, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3047-3055"},
        {149, 0x94, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2eb0, 0x0250, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3056-3063"},
        {149, 0x94, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2eb0, 0x0254, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 3,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3064-3072"},
        {150, 0x95, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::F32, 0,
         0x0008, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3073-3082"},
        {152, 0x97, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2d8c, 0x04a4, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:3111-3129"},
        {153, 0x98, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::F32, 0,
         0x2b48, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3130-3138"},
        {153, 0x98, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::F32, 0,
         0x2b4c, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3139-3147"},
        {153, 0x98, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::F32, 0,
         0x2b50, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2,
         kTh07EclEffectNoOperand, kTh07EclEffectNoBit, 1, "00410520:3148-3155"},
        {156, 0x9b, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::Byte,
         0x2d8c, 0x04e9, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:3198-3215"},
        {157, 0x9c, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2d8c, 0x04b0, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:3216-3234"},
        {158, 0x9d, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2d8c, 0x04a8, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 1, 0,
         kTh07EclEffectNoBit, 1, "00410520:3235-3251"},
        {158, 0x9d, Th07EclEffectOwner::EnemyPointerTableEntry, Th07EclEffectWriteKind::F32,
         0x2d8c, 0x04ac, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 2, 0,
         kTh07EclEffectNoBit, 1, "00410520:3252-3261"},
        {161, 0xa0, Th07EclEffectOwner::Enemy, Th07EclEffectWriteKind::Bit, 0,
         0x2e2b, kTh07EclEffectNoOffset, kTh07EclEffectNoOffset, 0,
         kTh07EclEffectNoOperand, 3, 1, "00410520:3305-3313"},
    }};

const std::array<Th07EclHighOpcodeCase, kTh07HighEclOpcodeCaseCount> kTh07HighEclOpcodeCases = {{
    {136, 17, "ecldata1.decl:1162", 0x87, 0x00417a54, 0x00415f5a, 2825, 2829,
     "writes enemy+0x2e29 bit5 from arg0 and sets enemy+0x2e2f to 2", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {137, 99, "ecldata1.decl:16", 0x88, 0x00417a58, 0x00415f8c, 2830, 2831,
     "writes enemy+0x2e2a bit7 from arg0", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {138, 24, "ecldata1.decl:1164", 0x89, 0x00417a5c, 0x00415fb4, 2833, 2860,
     "writes enemy+0x4f30 byte and +0x4f32/+0x4f34/+0x4f36 i16 fields; may call FUN_00455170(enemy, enemy+0x39f8, ratio<<1) when bit3 is set", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {139, 52, "ecldata1.decl:553", 0x8a, 0x00417a60, 0x00414cae, 2861, 2891,
     "writes global tables DAT_0049fc44/DAT_0049fc24/DAT_0049fc64 indexed by arg0; divides args by enemy+0x2bbc", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {140, 27, "ecldata1.decl:18", 0x8b, 0x00417a64, 0x004160cf, 2892, 2925,
     "writes globals DAT_012fe25c/DAT_012fe260/DAT_012fe264/DAT_012fe268 from four f32 operands", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {141, 18, "ecldata2.decl:1434", 0, 0x00417a68, 0x0041677b, 3314, 3316,
     "jump-table entry targets the common instruction-advance path with no side effects", Th07EclOpcodeStatus::SourceDerived,
     "treat-as-no-op-until-runtime-naming-is-proven"},
    {142, 292, "ecldata1.decl:752", 0x8d, 0x00417a6c, 0x004161c7, 2926, 2936,
     "writes enemy+0x4f40; resets timer-like fields enemy+0x4f3c=0 and enemy+0x4f38=-999", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {143, 2, "ecldata4.decl:196", 0x8e, 0x00417a70, 0x00416233, 2937, 2946,
     "calls FUN_00424c00(enemy+0x2b0c, arg0_f32)", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {144, 78, "ecldata4.decl:5164", 0x8f, 0x00417a74, 0x00415097, 2947, 2974,
     "writes enemy+0x2f58 and enemy+0x2ee4; resets enemy+0x2f50/+0x2f54/+0x2f5c/+0x2f60/+0x2f64; copies 0x1a dwords from enemy+0x6fc to enemy+0x2ee8", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {145, 84, "ecldata4.decl:959", 0x90, 0x00417a78, 0x00416284, 2975, 2997,
     "if DAT_012fe098[arg0] is nonzero writes *(DAT_012fe098[arg0]+0x2b08)=arg1", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {146, 1, "ecldata4.decl:714", 0x91, 0x00417a7c, 0x00416342, 2998, 3000,
     "calls FUN_00424740(0)", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {148, 32, "ecldata1.decl:550", 0x93, 0x00417a84, 0x00414ee9, 3010, 3037,
     "writes enemy indexed tables +0x2ebc[arg0]=arg1 and +0x2ecc[arg0]=arg2", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {149, 1, "ecldata4.decl:5252", 0x94, 0x00417a88, 0x00416353, 3038, 3072,
     "writes enemy+0x2e2b bit1 from arg0; when bit is clear writes f32 args to *(enemy+0x2eb0)+0x24c/+0x250/+0x254", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {150, 1, "ecldata6.decl:632", 0x95, 0x00417a8c, 0x00416495, 3073, 3082,
     "writes f32 arg0 to enemy+0x0008", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {151, 8, "ecldata6.decl:790", 0x96, 0x00417a90, 0x004164d7, 3083, 3110,
     "writes two resolved float targets via FUN_0040f3c0 using FUN_0048bb40/FUN_0048bbf0 results scaled by args", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {152, 10, "ecldata6.decl:1509", 0x97, 0x00417a94, 0x0041365d, 3111, 3129,
     "if enemy pointer table +0x2d8c[arg0] is nonzero writes f32 arg1 to pointed object +0x4a4", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {153, 3, "ecldata6.decl:974", 0x98, 0x00417a98, 0x004148df, 3130, 3155,
     "writes f32 args to enemy+0x2b48/+0x2b4c/+0x2b50", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {154, 80, "ecldata1.decl:6", 0x99, 0x00417a9c, 0x004154c9, 3156, 3176,
     "loops arg0 times; randomizes around enemy+0x2b0c/+0x2b10 using FUN_00431900 and constants; calls FUN_004326f0(&local_74,1,0)", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {155, 47, "ecldata7.decl:949", 0x9a, 0x00417aa0, 0x0041661a, 3177, 3197,
     "writes a resolved float target via FUN_0040f3c0; branch uses player/global position comparison and FUN_00431900/FUN_00431930", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {156, 12, "ecldata7.decl:2375", 0x9b, 0x00417aa4, 0x004138f6, 3198, 3215,
     "if enemy pointer table +0x2d8c[arg0] is nonzero writes byte arg1 to pointed object +0x4e9", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {157, 4, "ecldata7.decl:3118", 0x9c, 0x00417aa8, 0x00413b3b, 3216, 3234,
     "if enemy pointer table +0x2d8c[arg0] is nonzero writes f32 arg1 to pointed object +0x4b0", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {158, 4, "ecldata7.decl:3117", 0x9d, 0x00417aac, 0x00413bd3, 3235, 3261,
     "if enemy pointer table +0x2d8c[arg0] is nonzero writes f32 args to pointed object +0x4a8/+0x4ac", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
    {159, 16, "ecldata7.decl:1727", 0x9e, 0x00417ab0, 0x004115e0, 3262, 3295,
     "computes (arg1-arg2)*arg3+arg2 and writes the result to a resolved float target via FUN_0040f3c0", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {160, 2, "ecldata1.decl:718", 0x9f, 0x00417ab4, 0x004166e6, 3296, 3304,
     "calls FUN_0042f5a2(arg0)", Th07EclOpcodeStatus::SourceDerived,
     "helper-call-contract-compiled-owner-lifetime-pending"},
    {161, 7, "ecldata8.decl:3484", 0xa0, 0x00417ab8, 0x00416729, 3305, 3313,
     "writes enemy+0x2e2b bit3 from arg0", Th07EclOpcodeStatus::SourceDerived,
     "turn-case-surface-into-named-owner-fields"},
}};

const std::array<Th07EclHighOpcodeSignature, kTh07HighEclOpcodeSignatureCount> kTh07HighEclOpcodeSignatures = {{
    {136, 17, 1, {Th07EclOperandKind::Int}, "ecldata1.decl:1162", "ins_136(1)"},
    {137, 99, 1, {Th07EclOperandKind::Int}, "ecldata1.decl:16", "ins_137(1)"},
    {138, 24, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata1.decl:1164", "ins_138(25, 32, 16, 1)"},
    {139, 14, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata1.decl:553", "ins_139(0, 0, 1200, 16752800)"},
    {139, 36, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::IntReference, Th07EclOperandKind::Int}, "ecldata1.decl:1036", "ins_139(0, 0, [10066], 16752800)"},
    {139, 1, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::IntReference, Th07EclOperandKind::IntReference, Th07EclOperandKind::Int}, "ecldata4.decl:1199", "ins_139(1, [10066], [10027], 16752800)"},
    {139, 1, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::IntReference, Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata5.decl:1570", "ins_139(1, [10066], 4400, 16752800)"},
    {140, 18, 4, {Th07EclOperandKind::Float, Th07EclOperandKind::Float, Th07EclOperandKind::Float, Th07EclOperandKind::Float}, "ecldata1.decl:18", "ins_140(1.0f, 1.0f, 1.0f, 1.0f)"},
    {140, 9, 4, {Th07EclOperandKind::Float, Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference}, "ecldata1.decl:40", "ins_140(1.0f, [10005.0f], [10006.0f], [10004.0f])"},
    {141, 18, 1, {Th07EclOperandKind::Int}, "ecldata2.decl:1434", "ins_141(1)"},
    {142, 292, 1, {Th07EclOperandKind::Int}, "ecldata1.decl:752", "ins_142(60)"},
    {143, 2, 1, {Th07EclOperandKind::Float}, "ecldata4.decl:196", "ins_143(96.0f)"},
    {144, 75, 2, {Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata4.decl:5164", "ins_144(10, 134)"},
    {144, 3, 2, {Th07EclOperandKind::IntReference, Th07EclOperandKind::Int}, "ecldata6.decl:1813", "ins_144([10002], 54)"},
    {145, 84, 2, {Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata4.decl:959", "ins_145(1, 0)"},
    {146, 1, 0, {}, "ecldata4.decl:714", "ins_146()"},
    {148, 32, 3, {Th07EclOperandKind::Int, Th07EclOperandKind::Int, Th07EclOperandKind::Int}, "ecldata1.decl:550", "ins_148(0, 1200, 29)"},
    {149, 1, 4, {Th07EclOperandKind::Int, Th07EclOperandKind::Float, Th07EclOperandKind::Float, Th07EclOperandKind::Int}, "ecldata4.decl:5252", "ins_149(1, 192.0f, 160.0f, 0)"},
    {150, 1, 1, {Th07EclOperandKind::FloatReference}, "ecldata6.decl:632", "ins_150([10008.0f])"},
    {151, 8, 4, {Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::Float}, "ecldata6.decl:790", "ins_151([10072.0f], [10073.0f], [10004.0f], 32.0f)"},
    {152, 10, 2, {Th07EclOperandKind::Int, Th07EclOperandKind::FloatReference}, "ecldata6.decl:1509", "ins_152(0, [10004.0f])"},
    {153, 3, 3, {Th07EclOperandKind::Float, Th07EclOperandKind::Float, Th07EclOperandKind::Float}, "ecldata6.decl:974", "ins_153(356.0f, 32.0f, 32.0f)"},
    {154, 11, 1, {Th07EclOperandKind::IntReference}, "ecldata1.decl:6", "ins_154([10003])"},
    {154, 69, 1, {Th07EclOperandKind::Int}, "ecldata1.decl:717", "ins_154(5)"},
    {155, 47, 1, {Th07EclOperandKind::FloatReference}, "ecldata7.decl:949", "ins_155([10004.0f])"},
    {156, 12, 2, {Th07EclOperandKind::IntReference, Th07EclOperandKind::Int}, "ecldata7.decl:2375", "ins_156([10000], 1)"},
    {157, 4, 2, {Th07EclOperandKind::IntReference, Th07EclOperandKind::FloatReference}, "ecldata7.decl:3118", "ins_157([10000], [10034.0f])"},
    {158, 4, 3, {Th07EclOperandKind::IntReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference}, "ecldata7.decl:3117", "ins_158([10000], [10033.0f], [10005.0f])"},
    {159, 16, 4, {Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::FloatReference, Th07EclOperandKind::Float}, "ecldata7.decl:1727", "ins_159([10010.0f], [10033.0f], [10035.0f], 0.5f)"},
    {160, 2, 1, {Th07EclOperandKind::Int}, "ecldata1.decl:718", "ins_160(10000)"},
    {161, 7, 1, {Th07EclOperandKind::Int}, "ecldata8.decl:3484", "ins_161(1)"},
}};

const std::array<Th07EclHighOpcodeCallContract, kTh07HighEclCallContractCount>
    kTh07HighEclCallContracts = {{
        {138, 0x89, 0x00415fb4, 0x004160c5, 0x00455170,
         Th07EclCallContractKind::HelperCall, kTh07EclEffectNoOperand,
         kTh07EclEffectNoOperand, "00410520:2857-2859",
         "conditional helper call with enemy, enemy+0x39f8, and ratio<<1"},
        {143, 0x8e, 0x00416233, 0x0041627a, 0x00424c00,
         Th07EclCallContractKind::HelperCall, 0, kTh07EclEffectNoOperand,
         "00410520:2945", "helper call with enemy+0x2b0c and arg0_f32"},
        {146, 0x91, 0x00416342, 0x00416349, 0x00424740,
         Th07EclCallContractKind::HelperCall, kTh07EclEffectNoOperand,
         kTh07EclEffectNoOperand, "00410520:2999", "helper call with immediate 0"},
        {151, 0x96, 0x004164d7, 0x0041653f, 0x0048bb40,
         Th07EclCallContractKind::ProjectionHelper, 2, 3, "00410520:3094",
         "first projection helper before resolved float-target write"},
        {151, 0x96, 0x004164d7, 0x00416569, 0x0040f3c0,
         Th07EclCallContractKind::ResolvedFloatTarget, 1, 3, "00410520:3095",
         "first resolved float-target write through operand +0x10"},
        {151, 0x96, 0x004164d7, 0x004165de, 0x0048bbf0,
         Th07EclCallContractKind::ProjectionHelper, 2, 3, "00410520:3107",
         "second projection helper before resolved float-target write"},
        {151, 0x96, 0x004164d7, 0x00416608, 0x0040f3c0,
         Th07EclCallContractKind::ResolvedFloatTarget, 0, 3, "00410520:3108",
         "second resolved float-target write through operand +0x0c"},
        {154, 0x99, 0x004154c9, 0x00415548, 0x00431900,
         Th07EclCallContractKind::RandomFloat, 0, kTh07EclEffectNoOperand,
         "00410520:3168", "random X offset around enemy+0x2b0c"},
        {154, 0x99, 0x004154c9, 0x00415577, 0x00431900,
         Th07EclCallContractKind::RandomFloat, 0, kTh07EclEffectNoOperand,
         "00410520:3171", "random Y offset around enemy+0x2b10"},
        {154, 0x99, 0x004154c9, 0x004155a5, 0x004326f0,
         Th07EclCallContractKind::EffectSpawn, 0, kTh07EclEffectNoOperand,
         "00410520:3174", "spawn helper call with randomized local position"},
        {155, 0x9a, 0x0041661a, 0x00416663, 0x00431900,
         Th07EclCallContractKind::RandomFloat, 0, kTh07EclEffectNoOperand,
         "00410520:3182", "branch random value before angle normalization"},
        {155, 0x9a, 0x0041661a, 0x00416678, 0x00431930,
         Th07EclCallContractKind::ProjectionHelper, 0, kTh07EclEffectNoOperand,
         "00410520:3183", "angle normalization helper before resolved target write"},
        {155, 0x9a, 0x0041661a, 0x00416696, 0x0040f3c0,
         Th07EclCallContractKind::ResolvedFloatTarget, 0, kTh07EclEffectNoOperand,
         "00410520:3184-3186", "branch resolved float-target write through operand +0x0c"},
        {155, 0x9a, 0x0041661a, 0x004166aa, 0x00431900,
         Th07EclCallContractKind::RandomFloat, 0, kTh07EclEffectNoOperand,
         "00410520:3190", "fallback branch random value before resolved target write"},
        {155, 0x9a, 0x0041661a, 0x004166d4, 0x0040f3c0,
         Th07EclCallContractKind::ResolvedFloatTarget, 0, kTh07EclEffectNoOperand,
         "00410520:3192-3194", "fallback resolved float-target write through operand +0x0c"},
        {159, 0x9e, 0x004115e0, 0x004116db, 0x0040f3c0,
         Th07EclCallContractKind::ResolvedFloatTarget, 0, kTh07EclEffectNoOperand,
         "00410520:3293", "linear interpolation result written through operand +0x0c"},
        {160, 0x9f, 0x004166e6, 0x00416722, 0x0042f5a2,
         Th07EclCallContractKind::HelperCall, 0, kTh07EclEffectNoOperand,
         "00410520:3303", "helper call with resolved arg0"},
    }};

const std::array<Th07EclFloatTargetResolverContract, kTh07EclResolveFloatTargetContractCount>
    kTh07EclFloatTargetResolverContracts = {{
        {0x2714, Th07EclFloatTargetOwner::Enemy, 0x070c, 0x0040f422, "0040f3c0:11-13"},
        {0x2715, Th07EclFloatTargetOwner::Enemy, 0x0710, 0x0040f42f, "0040f3c0:14-16"},
        {0x2716, Th07EclFloatTargetOwner::Enemy, 0x0714, 0x0040f43c, "0040f3c0:17-19"},
        {0x2717, Th07EclFloatTargetOwner::Enemy, 0x0718, 0x0040f449, "0040f3c0:20-22"},
        {0x2718, Th07EclFloatTargetOwner::Enemy, 0x071c, 0x0040f456, "0040f3c0:23-25"},
        {0x2719, Th07EclFloatTargetOwner::Enemy, 0x0720, 0x0040f463, "0040f3c0:26-28"},
        {0x271a, Th07EclFloatTargetOwner::Enemy, 0x0724, 0x0040f470, "0040f3c0:29-31"},
        {0x271b, Th07EclFloatTargetOwner::Enemy, 0x0728, 0x0040f47d, "0040f3c0:32-34"},
        {0x2722, Th07EclFloatTargetOwner::Enemy, 0x2b0c, 0x0040f4be, "0040f3c0:37-39"},
        {0x2723, Th07EclFloatTargetOwner::Enemy, 0x2b10, 0x0040f4cb, "0040f3c0:40-42"},
        {0x2724, Th07EclFloatTargetOwner::Enemy, 0x2b14, 0x0040f4d8, "0040f3c0:43-45"},
        {0x2725, Th07EclFloatTargetOwner::Global, 0x004be408, 0x0040f4e5, "0040f3c0:46-48"},
        {0x2726, Th07EclFloatTargetOwner::Global, 0x004be40c, 0x0040f4ef, "0040f3c0:49-51"},
        {0x2727, Th07EclFloatTargetOwner::Global, 0x004be410, 0x0040f4f9, "0040f3c0:52-54"},
        {0x2731, Th07EclFloatTargetOwner::Enemy, 0x0754, 0x0040f48a, "0040f3c0:55-57"},
        {0x2732, Th07EclFloatTargetOwner::Enemy, 0x0758, 0x0040f497, "0040f3c0:58-60"},
        {0x2733, Th07EclFloatTargetOwner::Enemy, 0x075c, 0x0040f4a4, "0040f3c0:61-63"},
        {0x2734, Th07EclFloatTargetOwner::Enemy, 0x0760, 0x0040f4b1, "0040f3c0:64-66"},
        {0x2739, Th07EclFloatTargetOwner::Global, 0x01347ab0, 0x0040f51d, "0040f3c0:67-69"},
        {0x273a, Th07EclFloatTargetOwner::Global, 0x01347ab4, 0x0040f527, "0040f3c0:70-72"},
        {0x273b, Th07EclFloatTargetOwner::Global, 0x01347ab8, 0x0040f531, "0040f3c0:73-75"},
        {0x273c, Th07EclFloatTargetOwner::Global, 0x01347abc, 0x0040f53b, "0040f3c0:76-78"},
        {0x273d, Th07EclFloatTargetOwner::Enemy, 0x2b54, 0x0040f581, "0040f3c0:79-81"},
        {0x273e, Th07EclFloatTargetOwner::Enemy, 0x2b58, 0x0040f58b, "0040f3c0:82-84"},
        {0x273f, Th07EclFloatTargetOwner::Enemy, 0x2b64, 0x0040f595, "0040f3c0:85-87"},
        {0x2740, Th07EclFloatTargetOwner::Enemy, 0x2b68, 0x0040f59f, "0040f3c0:88-90"},
        {0x2741, Th07EclFloatTargetOwner::Enemy, 0x2b6c, 0x0040f5a9, "0040f3c0:91-93"},
        {0x2742, Th07EclFloatTargetOwner::Enemy, 0x2b8c, 0x0040f545, "0040f3c0:94-96"},
        {0x2743, Th07EclFloatTargetOwner::Enemy, 0x2b90, 0x0040f54f, "0040f3c0:97-99"},
        {0x2744, Th07EclFloatTargetOwner::Enemy, 0x2b94, 0x0040f559, "0040f3c0:100-102"},
        {0x2745, Th07EclFloatTargetOwner::Enemy, 0x2b5c, 0x0040f5b3, "0040f3c0:103-105"},
        {0x2746, Th07EclFloatTargetOwner::Enemy, 0x2b60, 0x0040f5bd, "0040f3c0:106-108"},
        {0x2749, Th07EclFloatTargetOwner::Enemy, 0x2b80, 0x0040f563, "0040f3c0:109-111"},
        {0x274a, Th07EclFloatTargetOwner::Enemy, 0x2b84, 0x0040f56d, "0040f3c0:112-114"},
        {0x274b, Th07EclFloatTargetOwner::Enemy, 0x2b88, 0x0040f577, "0040f3c0:115-117"},
        {0x2758, Th07EclFloatTargetOwner::Enemy, 0x073c, 0x0040f503, "0040f3c0:118-120"},
        {0x2759, Th07EclFloatTargetOwner::Enemy, 0x0740, 0x0040f510, "0040f3c0:121-122"},
    }};

const std::array<Th07EclReadResolverContract, kTh07EclIntReadResolverContractCount>
    kTh07EclIntReadResolverContracts = {{
        {0x2710, Th07EclReadSourceKind::EnemyI32, 0x06fc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:10-12"},
        {0x2711, Th07EclReadSourceKind::EnemyI32, 0x0700, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:13-15"},
        {0x2712, Th07EclReadSourceKind::EnemyI32, 0x0704, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:16-18"},
        {0x2713, Th07EclReadSourceKind::EnemyI32, 0x0708, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:19-21"},
        {0x2714, Th07EclReadSourceKind::EnemyF32, 0x070c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:22-24"},
        {0x2715, Th07EclReadSourceKind::EnemyF32, 0x0710, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:25-27"},
        {0x2716, Th07EclReadSourceKind::EnemyF32, 0x0714, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:28-30"},
        {0x2717, Th07EclReadSourceKind::EnemyF32, 0x0718, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:31-33"},
        {0x2718, Th07EclReadSourceKind::EnemyF32, 0x071c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:34-36"},
        {0x2719, Th07EclReadSourceKind::EnemyF32, 0x0720, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:37-39"},
        {0x271a, Th07EclReadSourceKind::EnemyF32, 0x0724, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:40-42"},
        {0x271b, Th07EclReadSourceKind::EnemyF32, 0x0728, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:43-45"},
        {0x271c, Th07EclReadSourceKind::EnemyI32, 0x072c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:46-48"},
        {0x271d, Th07EclReadSourceKind::EnemyI32, 0x0730, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:49-51"},
        {0x271e, Th07EclReadSourceKind::EnemyI32, 0x0734, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:52-54"},
        {0x271f, Th07EclReadSourceKind::EnemyI32, 0x0738, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:55-57"},
        {0x2720, Th07EclReadSourceKind::GlobalI32, 0x00626280, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:58-60"},
        {0x2721, Th07EclReadSourceKind::GlobalI32, 0x0062f8a4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:61-63"},
        {0x2722, Th07EclReadSourceKind::EnemyF32, 0x2b0c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:64-66"},
        {0x2723, Th07EclReadSourceKind::EnemyF32, 0x2b10, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:67-69"},
        {0x2724, Th07EclReadSourceKind::EnemyF32, 0x2b14, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:70-72"},
        {0x2725, Th07EclReadSourceKind::GlobalF32, 0x004be408, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:73-75"},
        {0x2726, Th07EclReadSourceKind::GlobalF32, 0x004be40c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:76-78"},
        {0x2727, Th07EclReadSourceKind::GlobalF32, 0x004be410, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:79-81"},
        {0x2728, Th07EclReadSourceKind::VectorLength, 0x2b0c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverVectorLengthFunctionAddress, "0040e5b0:82-85"},
        {0x2729, Th07EclReadSourceKind::EnemyI32, 0x2bcc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:86-88"},
        {0x272a, Th07EclReadSourceKind::DistanceToGlobals, 0x2b0c, 0x004be408,
         kTh07EclReadResolverDistanceFunctionAddress, "0040e5b0:89-92"},
        {0x272b, Th07EclReadSourceKind::EnemyI32, 0x2bb8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:93-95"},
        {0x272c, Th07EclReadSourceKind::GlobalU8, 0x0062f647, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:96-98"},
        {0x272d, Th07EclReadSourceKind::EnemyI32, 0x0744, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:99-101"},
        {0x272e, Th07EclReadSourceKind::EnemyI32, 0x0748, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:102-104"},
        {0x272f, Th07EclReadSourceKind::EnemyI32, 0x074c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:105-107"},
        {0x2730, Th07EclReadSourceKind::EnemyI32, 0x0750, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:108-110"},
        {0x2731, Th07EclReadSourceKind::EnemyF32, 0x0754, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:111-113"},
        {0x2732, Th07EclReadSourceKind::EnemyF32, 0x0758, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:114-116"},
        {0x2733, Th07EclReadSourceKind::EnemyF32, 0x075c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:117-119"},
        {0x2734, Th07EclReadSourceKind::EnemyF32, 0x0760, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:120-122"},
        {0x2735, Th07EclReadSourceKind::GlobalI32, 0x01347aa0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:123-125"},
        {0x2736, Th07EclReadSourceKind::GlobalI32, 0x01347aa4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:126-128"},
        {0x2737, Th07EclReadSourceKind::GlobalI32, 0x01347aa8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:129-131"},
        {0x2738, Th07EclReadSourceKind::GlobalI32, 0x01347aac, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:132-134"},
        {0x2739, Th07EclReadSourceKind::GlobalF32, 0x01347ab0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:135-137"},
        {0x273a, Th07EclReadSourceKind::GlobalF32, 0x01347ab4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:138-140"},
        {0x273b, Th07EclReadSourceKind::GlobalF32, 0x01347ab8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:141-143"},
        {0x273c, Th07EclReadSourceKind::GlobalF32, 0x01347abc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:144-146"},
        {0x273d, Th07EclReadSourceKind::EnemyF32, 0x2b54, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:147-149"},
        {0x273e, Th07EclReadSourceKind::EnemyF32, 0x2b58, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:150-152"},
        {0x273f, Th07EclReadSourceKind::EnemyF32, 0x2b64, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:153-155"},
        {0x2740, Th07EclReadSourceKind::EnemyF32, 0x2b68, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:156-158"},
        {0x2741, Th07EclReadSourceKind::EnemyF32, 0x2b6c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:159-161"},
        {0x2742, Th07EclReadSourceKind::EnemyF32, 0x2b8c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:162-164"},
        {0x2743, Th07EclReadSourceKind::EnemyF32, 0x2b90, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:165-167"},
        {0x2744, Th07EclReadSourceKind::EnemyF32, 0x2b94, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:168-170"},
        {0x2745, Th07EclReadSourceKind::EnemyF32, 0x2b5c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:171-173"},
        {0x2746, Th07EclReadSourceKind::EnemyF32, 0x2b60, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:174-176"},
        {0x2747, Th07EclReadSourceKind::RandomInt, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoField, kTh07EclReadResolverRandomIntFunctionAddress,
         "0040e5b0:177-179"},
        {0x2748, Th07EclReadSourceKind::RandomIntModuloEnemyI32, 0x0744, 0x0748,
         kTh07EclReadResolverRandomIntFunctionAddress, "0040e5b0:180-190"},
        {0x274d, Th07EclReadSourceKind::EnemyI32, 0x2e4c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:193-195"},
        {0x274e, Th07EclReadSourceKind::EnemyU8, 0x2e17, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:196-198"},
        {0x274f, Th07EclReadSourceKind::EnemyF32, 0x2b30, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:199-201"},
        {0x2750, Th07EclReadSourceKind::EnemyF32, 0x2b34, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:202-204"},
        {0x2751, Th07EclReadSourceKind::EnemyF32, 0x2b38, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:205-207"},
        {0x2752, Th07EclReadSourceKind::EnemyI32, 0x2ebc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:208-210"},
        {0x2753, Th07EclReadSourceKind::EnemyI32, 0x2ec0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:211-213"},
        {0x2754, Th07EclReadSourceKind::EnemyI32, 0x2ec4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:214-216"},
        {0x2755, Th07EclReadSourceKind::EnemyI32, 0x2ec8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:217-219"},
        {0x2756, Th07EclReadSourceKind::EnemyI32, 0x2e10, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:220-222"},
        {0x2757, Th07EclReadSourceKind::EnemyI32, 0x2bc0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040e5b0:223-225"},
        {0x2758, Th07EclReadSourceKind::EnemyF32, 0x073c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:226-228"},
        {0x2759, Th07EclReadSourceKind::EnemyF32, 0x0740, kTh07EclReadResolverNoField,
         kTh07EclReadResolverFloatToIntBridgeAddress, "0040e5b0:229-231"},
    }};

const std::array<Th07EclReadResolverContract, kTh07EclFloatReadResolverContractCount>
    kTh07EclFloatReadResolverContracts = {{
        {0x2710, Th07EclReadSourceKind::EnemyI32, 0x06fc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:14-16"},
        {0x2711, Th07EclReadSourceKind::EnemyI32, 0x0700, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:17-19"},
        {0x2712, Th07EclReadSourceKind::EnemyI32, 0x0704, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:20-22"},
        {0x2713, Th07EclReadSourceKind::EnemyI32, 0x0708, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:23-25"},
        {0x2714, Th07EclReadSourceKind::EnemyF32, 0x070c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:26-28"},
        {0x2715, Th07EclReadSourceKind::EnemyF32, 0x0710, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:29-31"},
        {0x2716, Th07EclReadSourceKind::EnemyF32, 0x0714, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:32-34"},
        {0x2717, Th07EclReadSourceKind::EnemyF32, 0x0718, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:35-37"},
        {0x2718, Th07EclReadSourceKind::EnemyF32, 0x071c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:38-40"},
        {0x2719, Th07EclReadSourceKind::EnemyF32, 0x0720, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:41-43"},
        {0x271a, Th07EclReadSourceKind::EnemyF32, 0x0724, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:44-46"},
        {0x271b, Th07EclReadSourceKind::EnemyF32, 0x0728, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:47-49"},
        {0x271c, Th07EclReadSourceKind::EnemyI32, 0x072c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:50-52"},
        {0x271d, Th07EclReadSourceKind::EnemyI32, 0x0730, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:53-55"},
        {0x271e, Th07EclReadSourceKind::EnemyI32, 0x0734, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:56-58"},
        {0x271f, Th07EclReadSourceKind::EnemyI32, 0x0738, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:59-61"},
        {0x2720, Th07EclReadSourceKind::GlobalI32, 0x00626280, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:62-64"},
        {0x2721, Th07EclReadSourceKind::GlobalI32, 0x0062f8a4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:65-67"},
        {0x2722, Th07EclReadSourceKind::EnemyF32, 0x2b0c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:68-70"},
        {0x2723, Th07EclReadSourceKind::EnemyF32, 0x2b10, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:71-73"},
        {0x2724, Th07EclReadSourceKind::EnemyF32, 0x2b14, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:74-76"},
        {0x2725, Th07EclReadSourceKind::GlobalF32, 0x004be408, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:77-79"},
        {0x2726, Th07EclReadSourceKind::GlobalF32, 0x004be40c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:80-82"},
        {0x2727, Th07EclReadSourceKind::GlobalF32, 0x004be410, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:83-85"},
        {0x2728, Th07EclReadSourceKind::VectorLength, 0x2b0c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverVectorLengthFunctionAddress, "0040edf0:86-88"},
        {0x2729, Th07EclReadSourceKind::EnemyI32, 0x2bcc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:89-91"},
        {0x272a, Th07EclReadSourceKind::DistanceToGlobals, 0x2b0c, 0x004be408,
         kTh07EclReadResolverDistanceFunctionAddress, "0040edf0:92-94"},
        {0x272b, Th07EclReadSourceKind::EnemyI32, 0x2bb8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:95-97"},
        {0x272c, Th07EclReadSourceKind::GlobalU8, 0x0062f647, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:98-100"},
        {0x272d, Th07EclReadSourceKind::EnemyI32, 0x0744, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:101-103"},
        {0x272e, Th07EclReadSourceKind::EnemyI32, 0x0748, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:104-106"},
        {0x272f, Th07EclReadSourceKind::EnemyI32, 0x074c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:107-109"},
        {0x2730, Th07EclReadSourceKind::EnemyI32, 0x0750, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:110-112"},
        {0x2731, Th07EclReadSourceKind::EnemyF32, 0x0754, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:113-115"},
        {0x2732, Th07EclReadSourceKind::EnemyF32, 0x0758, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:116-118"},
        {0x2733, Th07EclReadSourceKind::EnemyF32, 0x075c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:119-121"},
        {0x2734, Th07EclReadSourceKind::EnemyF32, 0x0760, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:122-124"},
        {0x2735, Th07EclReadSourceKind::GlobalI32, 0x01347aa0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:125-127"},
        {0x2736, Th07EclReadSourceKind::GlobalI32, 0x01347aa4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:128-130"},
        {0x2737, Th07EclReadSourceKind::GlobalI32, 0x01347aa8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:131-133"},
        {0x2738, Th07EclReadSourceKind::GlobalI32, 0x01347aac, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:134-136"},
        {0x2739, Th07EclReadSourceKind::GlobalF32, 0x01347ab0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:137-139"},
        {0x273a, Th07EclReadSourceKind::GlobalF32, 0x01347ab4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:140-142"},
        {0x273b, Th07EclReadSourceKind::GlobalF32, 0x01347ab8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:143-145"},
        {0x273c, Th07EclReadSourceKind::GlobalF32, 0x01347abc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:146-148"},
        {0x273d, Th07EclReadSourceKind::EnemyF32, 0x2b54, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:149-151"},
        {0x273e, Th07EclReadSourceKind::EnemyF32, 0x2b58, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:152-154"},
        {0x273f, Th07EclReadSourceKind::EnemyF32, 0x2b64, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:155-157"},
        {0x2740, Th07EclReadSourceKind::EnemyF32, 0x2b68, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:158-160"},
        {0x2741, Th07EclReadSourceKind::EnemyF32, 0x2b6c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:161-163"},
        {0x2742, Th07EclReadSourceKind::EnemyF32, 0x2b8c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:164-166"},
        {0x2743, Th07EclReadSourceKind::EnemyF32, 0x2b90, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:167-169"},
        {0x2744, Th07EclReadSourceKind::EnemyF32, 0x2b94, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:170-172"},
        {0x2745, Th07EclReadSourceKind::EnemyF32, 0x2b5c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:173-175"},
        {0x2746, Th07EclReadSourceKind::EnemyF32, 0x2b60, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:176-178"},
        {0x2747, Th07EclReadSourceKind::RandomFloat, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoField, kTh07EclReadResolverRandomFloatFunctionAddress,
         "0040edf0:179-181"},
        {0x2748, Th07EclReadSourceKind::RandomFloatScaledEnemyF32, 0x0754, 0x0758,
         kTh07EclReadResolverRandomFloatFunctionAddress, "0040edf0:182-186"},
        {0x2749, Th07EclReadSourceKind::EnemyF32, 0x2b80, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:187-189"},
        {0x274a, Th07EclReadSourceKind::EnemyF32, 0x2b84, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:190-192"},
        {0x274b, Th07EclReadSourceKind::EnemyF32, 0x2b88, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:193-195"},
        {0x274c, Th07EclReadSourceKind::RandomFloatScaledGlobalF32, 0x00498a60, 0x00498a64,
         kTh07EclReadResolverRandomFloatFunctionAddress, "0040edf0:196-199"},
        {0x274d, Th07EclReadSourceKind::EnemyI32, 0x2e4c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:200-202"},
        {0x274e, Th07EclReadSourceKind::EnemyU8, 0x2e17, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:203-205"},
        {0x274f, Th07EclReadSourceKind::EnemyF32, 0x2b30, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:206-208"},
        {0x2750, Th07EclReadSourceKind::EnemyF32, 0x2b34, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:209-211"},
        {0x2751, Th07EclReadSourceKind::EnemyF32, 0x2b38, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:212-214"},
        {0x2752, Th07EclReadSourceKind::EnemyI32, 0x2ebc, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:215-217"},
        {0x2753, Th07EclReadSourceKind::EnemyI32, 0x2ec0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:218-220"},
        {0x2754, Th07EclReadSourceKind::EnemyI32, 0x2ec4, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:221-223"},
        {0x2755, Th07EclReadSourceKind::EnemyI32, 0x2ec8, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:224-226"},
        {0x2756, Th07EclReadSourceKind::EnemyI32, 0x2e10, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:227-229"},
        {0x2757, Th07EclReadSourceKind::EnemyI32, 0x2bc0, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:230-232"},
        {0x2758, Th07EclReadSourceKind::EnemyF32, 0x073c, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:233-235"},
        {0x2759, Th07EclReadSourceKind::EnemyF32, 0x0740, kTh07EclReadResolverNoField,
         kTh07EclReadResolverNoHelper, "0040edf0:236-238"},
    }};

const Th07EclClearHelperContract kTh07EclClearHelperContract = {
    kTh07EclClearHelperFunctionAddress,
    kTh07EclItemClearRadiusItemArrayAddress,
    kTh07EclItemClearRadiusItemCount,
    kTh07EclItemClearRadiusItemStrideBytes,
    kTh07EclItemClearRadiusItemStrideDwords,
    kTh07EclItemClearRadiusPositionOffset,
    kTh07EclItemClearRadiusStateOffset,
    kTh07EclItemClearRadiusSkippedState,
    kTh07EclClearHelperObjectBaseOffset,
    kTh07EclClearHelperObjectCount,
    kTh07EclClearHelperObjectStrideBytes,
    kTh07EclClearHelperObjectActiveOffset,
    kTh07EclClearHelperObjectFlagsOffset,
    kTh07EclClearHelperObjectProtectedFlagMask,
    kTh07EclClearHelperObjectStateOffset,
    kTh07EclClearHelperObjectStateValue,
    kTh07EclClearHelperObjectTimerOffset,
    kTh07EclClearHelperObjectTimerSentinel,
    kTh07EclClearHelperObjectCopySourceOffset,
    kTh07EclClearHelperObjectCopyDestOffset,
    kTh07EclClearHelperObjectClearFlagOffset,
    kTh07EclClearHelperObjectAngleOffset,
    kTh07EclClearHelperObjectRangeStartOffset,
    kTh07EclClearHelperObjectRangeEndOffset,
    kTh07EclClearHelperObjectBaseXOffset,
    kTh07EclClearHelperObjectBaseYOffset,
    kTh07EclClearHelperManagerEffectKindOffset,
    kTh07EclClearHelperFinalCooldownOffset,
    kTh07EclClearHelperFinalCooldownValue,
    kTh07EclClearHelperEffectManagerAddress,
    kTh07EclClearHelperEffectSpawnFunctionAddress,
    kTh07EclClearHelperLineDirectionFunctionAddress,
    kTh07EclClearHelperLineStepConstantAddress,
    kTh07EclClearHelperMaxEffectMode,
    kTh07EclClearHelperDynamicEffectModeLimit,
    kTh07EclClearHelperFallbackEffectKind,
    kTh07EclClearHelperFallbackEffectArg,
    kTh07EclClearHelperProtectedBypassMode,
    "00424740:20-72",
};

const Th07EclItemClearRadiusHelperContract kTh07EclItemClearRadiusHelperContract = {
    kTh07EclItemClearRadiusHelperFunctionAddress,
    kTh07EclItemClearRadiusItemArrayAddress,
    kTh07EclItemClearRadiusItemCount,
    kTh07EclItemClearRadiusItemStrideBytes,
    kTh07EclItemClearRadiusItemStrideDwords,
    kTh07EclItemClearRadiusPositionOffset,
    kTh07EclItemClearRadiusStateOffset,
    kTh07EclItemClearRadiusSkippedState,
    kTh07EclItemClearRadiusEffectManagerAddress,
    0x00424cf6,
    kTh07EclItemClearRadiusEffectSpawnFunctionAddress,
    kTh07EclItemClearRadiusEffectKind,
    kTh07EclItemClearRadiusEffectArg,
    "00424c00:11-25",
};

const Th07EclVisualEffectSpawnContract kTh07EclVisualEffectSpawnContract = {
    kTh07EclVisualEffectSpawnFunctionAddress,
    kTh07EclVisualEffectManagerAddress,
    kTh07EclVisualEffectSlotCount,
    kTh07EclVisualEffectSlotSizeBytes,
    kTh07EclVisualEffectFallbackSlotOffset,
    kTh07EclVisualEffectNextIndexOffset,
    kTh07EclVisualEffectPositionOffset,
    kTh07EclVisualEffectAuxVectorOffset,
    kTh07EclVisualEffectVelocityOffset,
    kTh07EclVisualEffectTimerPreviousOffset,
    kTh07EclVisualEffectTimerSubframeOffset,
    kTh07EclVisualEffectTimerCurrentOffset,
    kTh07EclVisualEffectModeOffset,
    kTh07EclVisualEffectActiveFlagOffset,
    kTh07EclVisualEffectInitializedFlagOffset,
    kTh07EclVisualEffectVariantOffset,
    kTh07EclVisualEffectPostInitByteOffset,
    kTh07EclVisualEffectColorOffset,
    kTh07EclVisualEffectRenderFlagsOffset,
    kTh07EclVisualEffectAnmScriptOffset,
    kTh07EclVisualEffectTimerPreviousValue,
    kTh07EclVisualEffectColorValue,
    kTh07EclVisualEffectRenderFlagBits,
    kTh07EclVisualEffectAnmScriptBase,
    kTh07EclVisualEffectModeGatePointerAddress,
    kTh07EclVisualEffectModeGateValueOffset,
    kTh07EclVisualEffectModeGateThreshold,
    kTh07EclVisualEffectModeGateReplacement,
    kTh07EclVisualEffectModeGateFirstAffectedMode,
    kTh07EclVisualEffectModeGateSecondAffectedMode,
    kTh07EclVisualEffectFloatToIntFunctionAddress,
    kTh07EclVisualEffectRandomFloatFunctionAddress,
    kTh07EclVisualEffectAnmManagerPointerAddress,
    kTh07EclVisualEffectAnmScriptTableOffset,
    kTh07EclVisualEffectSetScriptFunctionAddress,
    kTh07EclVisualEffectRandomVelocityMode,
    kTh07EclVisualEffectForceVariantOneMode,
    kTh07EclVisualEffectForceVariantZeroMode,
    kTh07EclVisualEffectRandomXScaleAddress,
    kTh07EclVisualEffectRandomXBiasAddress,
    kTh07EclVisualEffectRandomYScaleAddress,
    kTh07EclVisualEffectRandomYBiasAddress,
    "004326f0:15-75",
};

const std::array<Th07EclProjectionHelperContract, kTh07EclProjectionHelperContractCount>
    kTh07EclProjectionHelperContracts = {{
        {Th07EclProjectionHelperKind::Sin,
         kTh07EclProjectionOpcode,
         kTh07EclProjectionSinWrapperFunctionAddress,
         kTh07EclProjectionSinCoreFunctionAddress,
         kTh07EclProjectionSinOpcodeCallInstructionAddress,
         kTh07EclProjectionSinFiniteCheckCallAddress,
         kTh07EclProjectionSinCoreCallAddress,
         kTh07EclProjectionFiniteCheckFunctionAddress,
         kTh07EclProjectionSinPrimaryInstructionAddress,
         kTh07EclProjectionSinRangeLoadAddress,
         kTh07EclProjectionSinRangeLoopAddress,
         kTh07EclProjectionSinReducedInstructionAddress,
         kTh07EclProjectionSinMathErrorKind,
         kTh07EclProjectionSinMathErrorContextAddress,
         "0048bb40:8-10",
         "0048bb5d:15-40",
         "00410520:3083-3096"},
        {Th07EclProjectionHelperKind::Cos,
         kTh07EclProjectionOpcode,
         kTh07EclProjectionCosWrapperFunctionAddress,
         kTh07EclProjectionCosCoreFunctionAddress,
         kTh07EclProjectionCosOpcodeCallInstructionAddress,
         kTh07EclProjectionCosFiniteCheckCallAddress,
         kTh07EclProjectionCosCoreCallAddress,
         kTh07EclProjectionFiniteCheckFunctionAddress,
         kTh07EclProjectionCosPrimaryInstructionAddress,
         kTh07EclProjectionCosRangeLoadAddress,
         kTh07EclProjectionCosRangeLoopAddress,
         kTh07EclProjectionCosReducedInstructionAddress,
         kTh07EclProjectionCosMathErrorKind,
         kTh07EclProjectionCosMathErrorContextAddress,
         "0048bbf0:8-10",
         "0048bc0d:15-40",
         "00410520:3097-3110"},
    }};

const Th07EclOpcode138RecordHelperContract kTh07EclOpcode138RecordHelperContract = {
    kTh07EclOpcode138RecordHelperFunctionAddress,
    kTh07EclOpcode138RecordHelperCallInstructionAddress,
    kTh07EclOpcode138RecordHelperCallThisPointerAddress,
    kTh07EclOpcode138RecordHelperOpcode,
    kTh07EclOpcode138RecordHelperOutputBaseOffset,
    kTh07EclOpcode138RecordHelperFlagFieldOffset,
    kTh07EclOpcode138RecordHelperEnabledBitMask,
    kTh07EclOpcode138RecordHelperSegmentNumeratorOffset,
    kTh07EclOpcode138RecordHelperSegmentDenominatorOffset,
    kTh07EclOpcode138RecordHelperSegmentCountShift,
    kTh07EclOpcode138RecordHelperMinSegmentCount,
    kTh07EclOpcode138RecordHelperFailureReturn,
    kTh07EclOpcode138RecordHelperSuccessReturn,
    kTh07EclOpcode138RecordHelperMetadataPointerOffset,
    kTh07EclOpcode138RecordHelperOwnerBaseXOffset,
    kTh07EclOpcode138RecordHelperOwnerBaseYOffset,
    kTh07EclOpcode138RecordHelperOwnerColorOffset,
    kTh07EclOpcode138RecordHelperMetadataLeftXOffset,
    kTh07EclOpcode138RecordHelperMetadataFirstLaneYOffset,
    kTh07EclOpcode138RecordHelperMetadataStartXOffset,
    kTh07EclOpcode138RecordHelperMetadataSecondLaneYOffset,
    kTh07EclOpcode138RecordHelperRecordStrideBytes,
    kTh07EclOpcode138RecordHelperSecondLaneBaseOffset,
    kTh07EclOpcode138RecordHelperRecordScaleOffset,
    kTh07EclOpcode138RecordHelperRecordColorOffset,
    kTh07EclOpcode138RecordHelperRecordXOffset,
    kTh07EclOpcode138RecordHelperRecordYOffset,
    kTh07EclOpcode138RecordHelperRecordScaleBits,
    kTh07EclOpcode138RecordHelperFirstLaneInitialIndex,
    kTh07EclOpcode138RecordHelperSecondLaneInitialIndex,
    kTh07EclOpcode138RecordHelperIndexStep,
    "00455170:15-48",
    "00410520:2833-2860",
};

bool Th07EclHighOpcodeCase::hasDispatchCase() const
{
    return targetAddress != 0;
}

bool Th07EclHighOpcodeCase::hasDecompilerCase() const
{
    return caseLabel != 0;
}

bool Th07EclHighOpcodeCase::targetsCommonAdvance() const
{
    return targetAddress == kTh07EclDispatchAdvanceAddress;
}

bool Th07EclHighOpcodeSignature::usesOperandKind(Th07EclOperandKind kind) const
{
    for (std::uint8_t index = 0; index < operandCount; index++) {
        if (operandKinds[index] == kind) {
            return true;
        }
    }
    return false;
}

const Th07EclHighOpcodeCase *FindTh07HighEclOpcodeCase(std::uint16_t opcode)
{
    for (const Th07EclHighOpcodeCase &entry : kTh07HighEclOpcodeCases) {
        if (entry.opcode == opcode) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclHighOpcodeEffectContract *FindTh07HighEclOpcodeEffectContract(std::uint16_t opcode,
                                                                           Th07EclEffectOwner owner,
                                                                           std::uint32_t fieldOffset)
{
    for (const Th07EclHighOpcodeEffectContract &entry : kTh07HighEclDirectEffectContracts) {
        if (entry.opcode == opcode && entry.owner == owner && entry.fieldOffset == fieldOffset) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclHighOpcodeCallContract *FindTh07HighEclOpcodeCallContract(std::uint16_t opcode,
                                                                       std::uint32_t callInstructionAddress)
{
    for (const Th07EclHighOpcodeCallContract &entry : kTh07HighEclCallContracts) {
        if (entry.opcode == opcode && entry.callInstructionAddress == callInstructionAddress) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclFloatTargetResolverContract *FindTh07EclFloatTargetResolverContract(std::uint16_t variableId)
{
    for (const Th07EclFloatTargetResolverContract &entry : kTh07EclFloatTargetResolverContracts) {
        if (entry.variableId == variableId) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclReadResolverContract *FindTh07EclIntReadResolverContract(std::uint16_t variableId)
{
    for (const Th07EclReadResolverContract &entry : kTh07EclIntReadResolverContracts) {
        if (entry.variableId == variableId) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclReadResolverContract *FindTh07EclFloatReadResolverContract(std::uint16_t variableId)
{
    for (const Th07EclReadResolverContract &entry : kTh07EclFloatReadResolverContracts) {
        if (entry.variableId == variableId) {
            return &entry;
        }
    }
    return nullptr;
}

const Th07EclProjectionHelperContract *FindTh07EclProjectionHelperContract(Th07EclProjectionHelperKind kind)
{
    for (const Th07EclProjectionHelperContract &entry : kTh07EclProjectionHelperContracts) {
        if (entry.kind == kind) {
            return &entry;
        }
    }
    return nullptr;
}

std::size_t CountTh07HighEclOpcodeCases(Th07EclOpcodeStatus status)
{
    std::size_t count = 0;
    for (const Th07EclHighOpcodeCase &entry : kTh07HighEclOpcodeCases) {
        if (entry.status == status) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07HighEclOpcodeSignatures(std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07EclHighOpcodeSignature &entry : kTh07HighEclOpcodeSignatures) {
        if (entry.opcode == opcode) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07HighEclOpcodeEffectContracts(std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07EclHighOpcodeEffectContract &entry : kTh07HighEclDirectEffectContracts) {
        if (entry.opcode == opcode) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07HighEclOpcodeCallContracts(std::uint16_t opcode)
{
    std::size_t count = 0;
    for (const Th07EclHighOpcodeCallContract &entry : kTh07HighEclCallContracts) {
        if (entry.opcode == opcode) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07HighEclOpcodeCallContracts(Th07EclCallContractKind kind)
{
    std::size_t count = 0;
    for (const Th07EclHighOpcodeCallContract &entry : kTh07HighEclCallContracts) {
        if (entry.kind == kind) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07EclFloatTargetResolverContracts(Th07EclFloatTargetOwner owner)
{
    std::size_t count = 0;
    for (const Th07EclFloatTargetResolverContract &entry : kTh07EclFloatTargetResolverContracts) {
        if (entry.owner == owner) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07EclIntReadResolverContracts(Th07EclReadSourceKind sourceKind)
{
    std::size_t count = 0;
    for (const Th07EclReadResolverContract &entry : kTh07EclIntReadResolverContracts) {
        if (entry.sourceKind == sourceKind) {
            count++;
        }
    }
    return count;
}

std::size_t CountTh07EclFloatReadResolverContracts(Th07EclReadSourceKind sourceKind)
{
    std::size_t count = 0;
    for (const Th07EclReadResolverContract &entry : kTh07EclFloatReadResolverContracts) {
        if (entry.sourceKind == sourceKind) {
            count++;
        }
    }
    return count;
}

bool Th07HighEclOpcodeUsesOperandKind(std::uint16_t opcode, Th07EclOperandKind kind)
{
    for (const Th07EclHighOpcodeSignature &entry : kTh07HighEclOpcodeSignatures) {
        if (entry.opcode == opcode && entry.usesOperandKind(kind)) {
            return true;
        }
    }
    return false;
}

bool IsTh07HighEclOpcodeBlocked(std::uint16_t opcode)
{
    const Th07EclHighOpcodeCase *entry = FindTh07HighEclOpcodeCase(opcode);
    return entry != nullptr && entry->status == Th07EclOpcodeStatus::Blocked;
}

bool IsTh07MissingEnumOpcode(std::uint16_t opcode)
{
    for (std::uint16_t missing : kTh07MissingEnumOpcodes) {
        if (missing == opcode) {
            return true;
        }
    }
    return false;
}

bool IsTh07MissingDispatchOpcode(std::uint16_t opcode)
{
    for (std::uint16_t missing : kTh07MissingDispatchOpcodes) {
        if (missing == opcode) {
            return true;
        }
    }
    return false;
}

} // namespace th07
