#pragma once

#include "inttypes.hpp"

#include <array>
#include <cstddef>

namespace th07
{
inline constexpr u32 kTh07GuiSpellcardSetupFunctionAddress = 0x0042868d;
inline constexpr u32 kTh07GuiSpellcardOwnerPointerOffset = 0x08;
inline constexpr u32 kTh07GuiSpellcardTextWidthOffset = 0x0c;

inline constexpr u32 kTh07GuiSpellcardPlayerPortraitVmOffset = 0x574c;
inline constexpr u32 kTh07GuiSpellcardEnemyNameVmOffset = 0x5be4;
inline constexpr u32 kTh07GuiSpellcardEnemyNameCopyVmOffset = 0x607c;
inline constexpr u32 kTh07GuiSpellcardTextVmOffset = 0x6514;
inline constexpr u32 kTh07GuiSpellcardInterruptVmOffset = 0x69ac;

inline constexpr u32 kTh07GuiAnmVmScriptIndexOffset = 0x01d8;
inline constexpr u32 kTh07GuiAnmVmPendingInterruptOffset = 0x01c6;
inline constexpr u16 kTh07GuiSpellcardPlayerPortraitScript = 0x04a1;
inline constexpr u16 kTh07GuiSpellcardEnemyNameScript = 0x04a4;
inline constexpr u16 kTh07GuiSpellcardEnemyNameCopyScript = 0x04a6;
inline constexpr u16 kTh07GuiSpellcardTextScript = 0x0704;
inline constexpr u16 kTh07GuiSpellcardDefaultNameSprite = 0x04ac;
inline constexpr u16 kTh07GuiSpellcardPendingInterrupt = 0x0001;
inline constexpr u32 kTh07GuiSpellcardTextColor = 0x00f0f0ff;
inline constexpr u32 kTh07GuiSpellcardTextGlyphWidth = 0x0f;
inline constexpr u32 kTh07GuiSpellcardSoundId = 0x0e;
inline constexpr u32 kTh07GuiSpellcardGlobalStateAddress = 0x00575ab4;
inline constexpr u32 kTh07GuiSpellcardGlobalStateValue = 0x02;
inline constexpr u32 kTh07GuiRunMsgFunctionAddress = 0x00429c42;
inline constexpr u32 kTh07GuiMsgOpcodeMaxCompareAddress = 0x00429d2e;
inline constexpr u32 kTh07GuiMsgOpcodeSwitchTableAddress = 0x0042a83a;
inline constexpr u32 kTh07GuiMsgOpcode00CaseAddress = 0x00429d42;
inline constexpr u32 kTh07GuiMsgOpcode01CaseAddress = 0x00429d54;
inline constexpr u32 kTh07GuiMsgOpcode02CaseAddress = 0x00429e29;
inline constexpr u32 kTh07GuiMsgPortraitVmBaseOffset = 0x1fbc0;
inline constexpr u32 kTh07GuiMsgPortraitVmStride = 0x024c;
inline constexpr u32 kTh07GuiMsgPortraitScriptBase = 0x04a0;
inline constexpr u32 kTh07GuiMsgPortraitScriptEnemyOffset = 0x02;
inline constexpr u32 kTh07GuiMsgPortraitSpriteEnemyOffset = 0x0d;
inline constexpr u32 kTh07GuiMsgPortraitScriptEnemyOffsetAndAddress = 0x00429d6d;
inline constexpr u32 kTh07GuiMsgPortraitScriptBaseAddAddress = 0x00429d70;
inline constexpr u32 kTh07GuiMsgPortraitSpriteEnemyOffsetAndAddress = 0x00429e49;
inline constexpr u32 kTh07GuiMsgPortraitSpriteBaseAddAddress = 0x00429e4c;
inline constexpr u32 kTh07GuiMsgPortraitAnmScriptSetCallAddress = 0x00429dc3;
inline constexpr u32 kTh07GuiMsgPortraitAnmSpriteSetCallAddress = 0x00429e72;
inline constexpr u32 kTh07GuiMsgOpcode03CaseAddress = 0x00429f38;
inline constexpr u32 kTh07GuiMsgOpcode04CaseAddress = 0x0042a09d;
inline constexpr u32 kTh07GuiMsgOpcode05CaseAddress = 0x0042a126;
inline constexpr u32 kTh07GuiMsgOpcode06CaseAddress = 0x0042a187;
inline constexpr u32 kTh07GuiMsgOpcode07CaseAddress = 0x0042a19f;
inline constexpr u32 kTh07GuiMsgOpcode08CaseAddress = 0x0042a28e;
inline constexpr u32 kTh07GuiMsgOpcode09CaseAddress = 0x0042a350;
inline constexpr u32 kTh07GuiMsgOpcode10CaseAddress = 0x0042a51d;
inline constexpr u32 kTh07GuiMsgOpcode11CaseAddress = 0x0042a560;
inline constexpr u32 kTh07GuiMsgOpcode12CaseAddress = 0x0042a522;
inline constexpr u32 kTh07GuiMsgOpcode12DurationConstantAddress = 0x00498aa8;
inline constexpr u32 kTh07GuiMsgOpcode12DurationBits = 0x40800000;
inline constexpr u32 kTh07GuiMsgOpcode12SupervisorThisMovAddress = 0x0042a52c;
inline constexpr u32 kTh07GuiMsgOpcode12FadeOutCallAddress = 0x0042a531;
inline constexpr u32 kTh07SupervisorFadeOutMusicFunctionAddress = 0x0043a0d6;
inline constexpr u32 kTh07GuiMsgOpcode13CaseAddress = 0x0042a71b;
inline constexpr u32 kTh07GuiMsgOpcode13ArgReadAddress = 0x0042a727;
inline constexpr u32 kTh07GuiMsgOpcode13SkippableWriteAddress = 0x0042a72a;
inline constexpr u32 kTh07GuiMsgDialogueSkippableOwnerOffset = 0x209b0;
inline constexpr u32 kTh07GuiMsgOpcode14CaseAddress = 0x0042a53b;
inline constexpr u32 kTh07GuiMsgOpcode14ScreenEffectColorPushAddress = 0x0042a53f;
inline constexpr u32 kTh07GuiMsgOpcode14ScreenEffectDurationMovAddress = 0x0042a544;
inline constexpr u32 kTh07GuiMsgOpcode14ScreenEffectKindAddress = 0x0042a549;
inline constexpr u32 kTh07GuiMsgOpcode14ScreenEffectCallAddress = 0x0042a54c;
inline constexpr u32 kTh07GuiMsgOpcode14GlobalStateWriteAddress = 0x0042a551;
inline constexpr u8 kTh07GuiMsgOpcodeMusicFadeout = 0x0c;
inline constexpr u8 kTh07GuiMsgOpcodeWaitSkippable = 0x0d;
inline constexpr float kTh07GuiMsgOpcodeMusicFadeoutSeconds = 4.0f;
inline constexpr u8 kTh07GuiMsgOpcodeScreenFade = 0x0e;
inline constexpr u32 kTh07GuiMsgOpcodeScreenFadeEffectKind = 4;
inline constexpr u32 kTh07GuiMsgOpcodeScreenFadeDurationFrames = 0x192;
inline constexpr u32 kTh07GuiMsgOpcodeScreenFadeColor = 0x00ffffff;
inline constexpr u32 kTh07GuiMsgOpcodeScreenFadeGlobalStateValue = 0x192;
inline constexpr u32 kTh07GuiMsgCurrentInstructionOffset = 0x1fba8;
inline constexpr u32 kTh07GuiMsgCurrentIndexOffset = 0x1fbac;
inline constexpr u32 kTh07GuiMsgTimerPreviousOffset = 0x1fbb0;
inline constexpr u32 kTh07GuiMsgTimerSubframeOffset = 0x1fbb4;
inline constexpr u32 kTh07GuiMsgTimerCurrentOffset = 0x1fbb8;
inline constexpr u32 kTh07GuiMsgWaitFrameCounterOffset = 0x1fbbc;
inline constexpr u32 kTh07GuiMsgIgnoreWaitCounterOffset = 0x209ac;
inline constexpr u32 kTh07GuiMsgAdvanceInstructionAddress = 0x0042a730;
inline constexpr u32 kTh07GuiMsgTimerTickPathAddress = 0x0042a758;
inline constexpr u32 kTh07GuiMsgTimerTickCallAddress = 0x0042a761;
inline constexpr u32 kTh07GuiMsgTimerTickFunctionAddress = 0x004010b0;
inline constexpr u32 kTh07GuiMsgPostTickPathAddress = 0x0042a766;
inline constexpr u32 kTh07GuiMsgAnmVmTickFirstCallAddress = 0x0042a775;
inline constexpr u32 kTh07GuiMsgAnmVmTickFunctionAddress = 0x00450d60;

enum class Th07GuiMsgOpcodeSemantic
{
    Delete,
    PortraitAnmScript,
    PortraitAnmSprite,
    TextDialogue,
    Wait,
    AnmInterrupt,
    EclResume,
    Music,
    TextIntro,
    StageResults,
    Halt,
    StageEnd,
    MusicFadeout,
    WaitSkippable,
    ScreenFade,
};

enum class Th07GuiMsgAdvanceMode
{
    ReturnImmediately,
    AlwaysAdvanceInstruction,
    ConditionalWaitAdvance,
    PostTickWithoutAdvance,
};

struct Th07GuiMsgOpcodeCase
{
    u8 opcode;
    Th07GuiMsgOpcodeSemantic semantic;
    u32 caseAddress;
    u8 minArgSize;
    u8 maxArgSize;
    Th07GuiMsgAdvanceMode advanceMode;
    const char *name;
};

inline constexpr std::size_t kTh07GuiMsgOpcodeCaseCount = 15;
// Argument sizes mirror the extracted MSG binary audit; dispatch and advance modes come from FUN_00429c42.
inline constexpr std::array<Th07GuiMsgOpcodeCase, kTh07GuiMsgOpcodeCaseCount>
    kTh07GuiMsgOpcodeCases = {{
        {0x00, Th07GuiMsgOpcodeSemantic::Delete, kTh07GuiMsgOpcode00CaseAddress, 0, 0,
         Th07GuiMsgAdvanceMode::ReturnImmediately, "delete"},
        {0x01, Th07GuiMsgOpcodeSemantic::PortraitAnmScript, kTh07GuiMsgOpcode01CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "portrait ANM script"},
        {0x02, Th07GuiMsgOpcodeSemantic::PortraitAnmSprite, kTh07GuiMsgOpcode02CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "portrait ANM sprite"},
        {0x03, Th07GuiMsgOpcodeSemantic::TextDialogue, kTh07GuiMsgOpcode03CaseAddress, 8, 48,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "text dialogue"},
        {0x04, Th07GuiMsgOpcodeSemantic::Wait, kTh07GuiMsgOpcode04CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::ConditionalWaitAdvance, "wait"},
        {0x05, Th07GuiMsgOpcodeSemantic::AnmInterrupt, kTh07GuiMsgOpcode05CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "ANM interrupt"},
        {0x06, Th07GuiMsgOpcodeSemantic::EclResume, kTh07GuiMsgOpcode06CaseAddress, 0, 0,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "ECL resume"},
        {0x07, Th07GuiMsgOpcodeSemantic::Music, kTh07GuiMsgOpcode07CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "music"},
        {0x08, Th07GuiMsgOpcodeSemantic::TextIntro, kTh07GuiMsgOpcode08CaseAddress, 24, 40,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "text intro"},
        {0x09, Th07GuiMsgOpcodeSemantic::StageResults, kTh07GuiMsgOpcode09CaseAddress, 4, 4,
         Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction, "stage results"},
        {0x0a, Th07GuiMsgOpcodeSemantic::Halt, kTh07GuiMsgOpcode10CaseAddress, 0, 0,
         Th07GuiMsgAdvanceMode::PostTickWithoutAdvance, "halt"},
        {0x0b, Th07GuiMsgOpcodeSemantic::StageEnd, kTh07GuiMsgOpcode11CaseAddress, 0, 0,
         Th07GuiMsgAdvanceMode::PostTickWithoutAdvance, "stage end"},
        {kTh07GuiMsgOpcodeMusicFadeout, Th07GuiMsgOpcodeSemantic::MusicFadeout,
         kTh07GuiMsgOpcode12CaseAddress, 0, 0, Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction,
         "music fadeout"},
        {kTh07GuiMsgOpcodeWaitSkippable, Th07GuiMsgOpcodeSemantic::WaitSkippable,
         kTh07GuiMsgOpcode13CaseAddress, 4, 4, Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction,
         "wait skippable"},
        {kTh07GuiMsgOpcodeScreenFade, Th07GuiMsgOpcodeSemantic::ScreenFade,
         kTh07GuiMsgOpcode14CaseAddress, 0, 0, Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction,
         "screen fade"},
    }};

constexpr const Th07GuiMsgOpcodeCase *FindTh07GuiMsgOpcodeCase(u8 opcode)
{
    for (const Th07GuiMsgOpcodeCase &opcodeCase : kTh07GuiMsgOpcodeCases) {
        if (opcodeCase.opcode == opcode) {
            return &opcodeCase;
        }
    }

    return nullptr;
}

inline constexpr u32 kTh07GuiSpellcardPlayerPortraitVmAddAddress = 0x0042869c;
inline constexpr u32 kTh07GuiSpellcardPlayerPortraitScriptWriteAddress = 0x004286af;
inline constexpr u32 kTh07GuiSpellcardPlayerPortraitActiveSpriteAddAddress = 0x004286de;
inline constexpr u32 kTh07GuiSpellcardEnemyNameVmAddAddress = 0x004286f5;
inline constexpr u32 kTh07GuiSpellcardEnemyNameScriptWriteAddress = 0x00428708;
inline constexpr u32 kTh07GuiSpellcardEnemyNameActiveSpritePushAddress = 0x0042872e;
inline constexpr u32 kTh07GuiSpellcardEnemyNameCopyVmAddAddress = 0x00428750;
inline constexpr u32 kTh07GuiSpellcardEnemyNameCopyScriptWriteAddress = 0x00428763;
inline constexpr u32 kTh07GuiSpellcardEnemyNameCopyActiveSpritePushAddress = 0x00428789;
inline constexpr u32 kTh07GuiSpellcardTextVmAddAddress = 0x004287ab;
inline constexpr u32 kTh07GuiSpellcardTextScriptWriteAddress = 0x004287be;
inline constexpr u32 kTh07GuiSpellcardTextColorPushAddress = 0x004287e9;
inline constexpr u32 kTh07GuiSpellcardTextInitVmAddAddress = 0x004287f4;
inline constexpr u32 kTh07GuiSpellcardTextWidthWriteAddress = 0x0042884e;
inline constexpr u32 kTh07GuiSpellcardInterruptVmAddAddress = 0x00428857;
inline constexpr u32 kTh07GuiSpellcardInterruptWriteAddress = 0x00428862;
inline constexpr u32 kTh07GuiSpellcardSoundPushAddress = 0x0042886d;
inline constexpr u32 kTh07GuiSpellcardGlobalStateWriteAddress = 0x00428879;

enum class Th07GuiSpellcardVmKind
{
    PlayerPortrait,
    EnemyName,
    EnemyNameCopy,
    Text,
};

struct Th07GuiSpellcardVmSlot
{
    Th07GuiSpellcardVmKind kind;
    u32 vmOffset;
    u16 scriptIndex;
    u32 vmAddAddress;
    u32 scriptWriteAddress;
    const char *name;
};

inline constexpr std::size_t kTh07GuiSpellcardVmSlotCount = 4;
inline constexpr std::array<Th07GuiSpellcardVmSlot, kTh07GuiSpellcardVmSlotCount>
    kTh07GuiSpellcardVmSlots = {{
        {Th07GuiSpellcardVmKind::PlayerPortrait, kTh07GuiSpellcardPlayerPortraitVmOffset,
         kTh07GuiSpellcardPlayerPortraitScript, kTh07GuiSpellcardPlayerPortraitVmAddAddress,
         kTh07GuiSpellcardPlayerPortraitScriptWriteAddress, "player spell-card portrait"},
        {Th07GuiSpellcardVmKind::EnemyName, kTh07GuiSpellcardEnemyNameVmOffset,
         kTh07GuiSpellcardEnemyNameScript, kTh07GuiSpellcardEnemyNameVmAddAddress,
         kTh07GuiSpellcardEnemyNameScriptWriteAddress, "enemy spell-card name"},
        {Th07GuiSpellcardVmKind::EnemyNameCopy, kTh07GuiSpellcardEnemyNameCopyVmOffset,
         kTh07GuiSpellcardEnemyNameCopyScript, kTh07GuiSpellcardEnemyNameCopyVmAddAddress,
         kTh07GuiSpellcardEnemyNameCopyScriptWriteAddress, "enemy spell-card name copy"},
        {Th07GuiSpellcardVmKind::Text, kTh07GuiSpellcardTextVmOffset, kTh07GuiSpellcardTextScript,
         kTh07GuiSpellcardTextVmAddAddress, kTh07GuiSpellcardTextScriptWriteAddress,
         "spell-card text"},
    }};

constexpr const Th07GuiSpellcardVmSlot *FindTh07GuiSpellcardVmSlot(Th07GuiSpellcardVmKind kind)
{
    for (const Th07GuiSpellcardVmSlot &slot : kTh07GuiSpellcardVmSlots) {
        if (slot.kind == kind) {
            return &slot;
        }
    }

    return nullptr;
}
} // namespace th07
