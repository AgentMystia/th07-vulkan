#include "GuiLayout.hpp"

#include <cassert>
#include <cstddef>
#include <cstring>

int main()
{
    static_assert(th07::kTh07GuiSpellcardSetupFunctionAddress == 0x0042868d);
    static_assert(th07::kTh07GuiSpellcardOwnerPointerOffset == 0x08);
    static_assert(th07::kTh07GuiSpellcardTextWidthOffset == 0x0c);
    static_assert(th07::kTh07GuiSpellcardPlayerPortraitVmOffset == 0x574c);
    static_assert(th07::kTh07GuiSpellcardEnemyNameVmOffset == 0x5be4);
    static_assert(th07::kTh07GuiSpellcardEnemyNameCopyVmOffset == 0x607c);
    static_assert(th07::kTh07GuiSpellcardTextVmOffset == 0x6514);
    static_assert(th07::kTh07GuiSpellcardInterruptVmOffset == 0x69ac);
    static_assert(th07::kTh07GuiAnmVmScriptIndexOffset == 0x01d8);
    static_assert(th07::kTh07GuiAnmVmPendingInterruptOffset == 0x01c6);
    static_assert(th07::kTh07GuiSpellcardPlayerPortraitScript == 0x04a1);
    static_assert(th07::kTh07GuiSpellcardEnemyNameScript == 0x04a4);
    static_assert(th07::kTh07GuiSpellcardEnemyNameCopyScript == 0x04a6);
    static_assert(th07::kTh07GuiSpellcardTextScript == 0x0704);
    static_assert(th07::kTh07GuiSpellcardDefaultNameSprite == 0x04ac);
    static_assert(th07::kTh07GuiSpellcardPendingInterrupt == 0x0001);
    static_assert(th07::kTh07GuiSpellcardTextColor == 0x00f0f0ff);
    static_assert(th07::kTh07GuiSpellcardTextGlyphWidth == 0x0f);
    static_assert(th07::kTh07GuiSpellcardSoundId == 0x0e);
    static_assert(th07::kTh07GuiSpellcardGlobalStateAddress == 0x00575ab4);
    static_assert(th07::kTh07GuiSpellcardGlobalStateValue == 0x02);
    static_assert(th07::kTh07GuiRunMsgFunctionAddress == 0x00429c42);
    static_assert(th07::kTh07GuiMsgOpcodeMaxCompareAddress == 0x00429d2e);
    static_assert(th07::kTh07GuiMsgOpcodeSwitchTableAddress == 0x0042a83a);
    static_assert(th07::kTh07GuiMsgOpcode00CaseAddress == 0x00429d42);
    static_assert(th07::kTh07GuiMsgOpcode01CaseAddress == 0x00429d54);
    static_assert(th07::kTh07GuiMsgOpcode02CaseAddress == 0x00429e29);
    static_assert(th07::kTh07GuiMsgPortraitVmBaseOffset == 0x1fbc0);
    static_assert(th07::kTh07GuiMsgPortraitVmStride == 0x024c);
    static_assert(th07::kTh07GuiMsgPortraitScriptBase == 0x04a0);
    static_assert(th07::kTh07GuiMsgPortraitScriptEnemyOffset == 0x02);
    static_assert(th07::kTh07GuiMsgPortraitSpriteEnemyOffset == 0x0d);
    static_assert(th07::kTh07GuiMsgPortraitScriptEnemyOffsetAndAddress == 0x00429d6d);
    static_assert(th07::kTh07GuiMsgPortraitScriptBaseAddAddress == 0x00429d70);
    static_assert(th07::kTh07GuiMsgPortraitSpriteEnemyOffsetAndAddress == 0x00429e49);
    static_assert(th07::kTh07GuiMsgPortraitSpriteBaseAddAddress == 0x00429e4c);
    static_assert(th07::kTh07GuiMsgPortraitAnmScriptSetCallAddress == 0x00429dc3);
    static_assert(th07::kTh07GuiMsgPortraitAnmSpriteSetCallAddress == 0x00429e72);
    static_assert(th07::kTh07GuiMsgOpcode03CaseAddress == 0x00429f38);
    static_assert(th07::kTh07GuiMsgOpcode04CaseAddress == 0x0042a09d);
    static_assert(th07::kTh07GuiMsgOpcode05CaseAddress == 0x0042a126);
    static_assert(th07::kTh07GuiMsgOpcode06CaseAddress == 0x0042a187);
    static_assert(th07::kTh07GuiMsgOpcode07CaseAddress == 0x0042a19f);
    static_assert(th07::kTh07GuiMsgOpcode08CaseAddress == 0x0042a28e);
    static_assert(th07::kTh07GuiMsgOpcode09CaseAddress == 0x0042a350);
    static_assert(th07::kTh07GuiMsgOpcode10CaseAddress == 0x0042a51d);
    static_assert(th07::kTh07GuiMsgOpcode11CaseAddress == 0x0042a560);
    static_assert(th07::kTh07GuiMsgOpcode12CaseAddress == 0x0042a522);
    static_assert(th07::kTh07GuiMsgOpcode12DurationConstantAddress == 0x00498aa8);
    static_assert(th07::kTh07GuiMsgOpcode12DurationBits == 0x40800000);
    static_assert(th07::kTh07GuiMsgOpcode12SupervisorThisMovAddress == 0x0042a52c);
    static_assert(th07::kTh07GuiMsgOpcode12FadeOutCallAddress == 0x0042a531);
    static_assert(th07::kTh07SupervisorFadeOutMusicFunctionAddress == 0x0043a0d6);
    static_assert(th07::kTh07GuiMsgOpcode13CaseAddress == 0x0042a71b);
    static_assert(th07::kTh07GuiMsgOpcode13ArgReadAddress == 0x0042a727);
    static_assert(th07::kTh07GuiMsgOpcode13SkippableWriteAddress == 0x0042a72a);
    static_assert(th07::kTh07GuiMsgDialogueSkippableOwnerOffset == 0x209b0);
    static_assert(th07::kTh07GuiMsgOpcode14CaseAddress == 0x0042a53b);
    static_assert(th07::kTh07GuiMsgOpcode14ScreenEffectColorPushAddress == 0x0042a53f);
    static_assert(th07::kTh07GuiMsgOpcode14ScreenEffectDurationMovAddress == 0x0042a544);
    static_assert(th07::kTh07GuiMsgOpcode14ScreenEffectKindAddress == 0x0042a549);
    static_assert(th07::kTh07GuiMsgOpcode14ScreenEffectCallAddress == 0x0042a54c);
    static_assert(th07::kTh07GuiMsgOpcode14GlobalStateWriteAddress == 0x0042a551);
    static_assert(th07::kTh07GuiMsgOpcodeMusicFadeout == 0x0c);
    static_assert(th07::kTh07GuiMsgOpcodeWaitSkippable == 0x0d);
    static_assert(th07::kTh07GuiMsgOpcodeMusicFadeoutSeconds == 4.0f);
    static_assert(th07::kTh07GuiMsgOpcodeScreenFade == 0x0e);
    static_assert(th07::kTh07GuiMsgOpcodeScreenFadeEffectKind == 4);
    static_assert(th07::kTh07GuiMsgOpcodeScreenFadeDurationFrames == 0x192);
    static_assert(th07::kTh07GuiMsgOpcodeScreenFadeColor == 0x00ffffff);
    static_assert(th07::kTh07GuiMsgOpcodeScreenFadeGlobalStateValue == 0x192);
    static_assert(th07::kTh07GuiMsgCurrentInstructionOffset == 0x1fba8);
    static_assert(th07::kTh07GuiMsgCurrentIndexOffset == 0x1fbac);
    static_assert(th07::kTh07GuiMsgTimerPreviousOffset == 0x1fbb0);
    static_assert(th07::kTh07GuiMsgTimerSubframeOffset == 0x1fbb4);
    static_assert(th07::kTh07GuiMsgTimerCurrentOffset == 0x1fbb8);
    static_assert(th07::kTh07GuiMsgWaitFrameCounterOffset == 0x1fbbc);
    static_assert(th07::kTh07GuiMsgIgnoreWaitCounterOffset == 0x209ac);
    static_assert(th07::kTh07GuiMsgAdvanceInstructionAddress == 0x0042a730);
    static_assert(th07::kTh07GuiMsgTimerTickPathAddress == 0x0042a758);
    static_assert(th07::kTh07GuiMsgTimerTickCallAddress == 0x0042a761);
    static_assert(th07::kTh07GuiMsgTimerTickFunctionAddress == 0x004010b0);
    static_assert(th07::kTh07GuiMsgPostTickPathAddress == 0x0042a766);
    static_assert(th07::kTh07GuiMsgAnmVmTickFirstCallAddress == 0x0042a775);
    static_assert(th07::kTh07GuiMsgAnmVmTickFunctionAddress == 0x00450d60);
    static_assert(th07::kTh07GuiMsgOpcodeCaseCount == 15);
    static_assert(th07::kTh07GuiSpellcardVmSlotCount == 4);

    assert(th07::kTh07GuiMsgOpcodeCases.size() == th07::kTh07GuiMsgOpcodeCaseCount);
    for (std::size_t i = 0; i < th07::kTh07GuiMsgOpcodeCases.size(); ++i) {
        assert(th07::kTh07GuiMsgOpcodeCases[i].opcode == i);
    }
    const th07::Th07GuiMsgOpcodeCase *deleteCase = th07::FindTh07GuiMsgOpcodeCase(0);
    assert(deleteCase != nullptr);
    assert(deleteCase->semantic == th07::Th07GuiMsgOpcodeSemantic::Delete);
    assert(deleteCase->advanceMode == th07::Th07GuiMsgAdvanceMode::ReturnImmediately);
    assert(deleteCase->caseAddress == th07::kTh07GuiMsgOpcode00CaseAddress);
    assert(deleteCase->minArgSize == 0);
    assert(deleteCase->maxArgSize == 0);
    const th07::Th07GuiMsgOpcodeCase *textDialogueCase = th07::FindTh07GuiMsgOpcodeCase(3);
    assert(textDialogueCase != nullptr);
    assert(textDialogueCase->semantic == th07::Th07GuiMsgOpcodeSemantic::TextDialogue);
    assert(textDialogueCase->minArgSize == 8);
    assert(textDialogueCase->maxArgSize == 48);
    assert(textDialogueCase->advanceMode == th07::Th07GuiMsgAdvanceMode::AlwaysAdvanceInstruction);
    assert(th07::FindTh07GuiMsgOpcodeCase(4)->advanceMode ==
           th07::Th07GuiMsgAdvanceMode::ConditionalWaitAdvance);
    assert(th07::FindTh07GuiMsgOpcodeCase(10)->advanceMode ==
           th07::Th07GuiMsgAdvanceMode::PostTickWithoutAdvance);
    assert(th07::FindTh07GuiMsgOpcodeCase(11)->advanceMode ==
           th07::Th07GuiMsgAdvanceMode::PostTickWithoutAdvance);
    assert(th07::FindTh07GuiMsgOpcodeCase(th07::kTh07GuiMsgOpcodeMusicFadeout)->caseAddress ==
           th07::kTh07GuiMsgOpcode12CaseAddress);
    assert(th07::FindTh07GuiMsgOpcodeCase(th07::kTh07GuiMsgOpcodeWaitSkippable)->maxArgSize == 4);
    assert(th07::FindTh07GuiMsgOpcodeCase(th07::kTh07GuiMsgOpcodeScreenFade)->caseAddress ==
           th07::kTh07GuiMsgOpcode14CaseAddress);
    assert(th07::FindTh07GuiMsgOpcodeCase(15) == nullptr);

    assert(th07::kTh07GuiSpellcardVmSlots.size() == th07::kTh07GuiSpellcardVmSlotCount);
    assert(th07::kTh07GuiSpellcardVmSlots[0].kind == th07::Th07GuiSpellcardVmKind::PlayerPortrait);
    assert(th07::kTh07GuiSpellcardVmSlots[0].vmOffset == th07::kTh07GuiSpellcardPlayerPortraitVmOffset);
    assert(th07::kTh07GuiSpellcardVmSlots[0].scriptIndex == th07::kTh07GuiSpellcardPlayerPortraitScript);
    assert(th07::kTh07GuiSpellcardVmSlots[0].scriptWriteAddress ==
           th07::kTh07GuiSpellcardPlayerPortraitScriptWriteAddress);
    assert(std::strcmp(th07::kTh07GuiSpellcardVmSlots[0].name, "player spell-card portrait") == 0);
    assert(th07::kTh07GuiSpellcardVmSlots[3].kind == th07::Th07GuiSpellcardVmKind::Text);
    assert(th07::kTh07GuiSpellcardVmSlots[3].vmAddAddress == th07::kTh07GuiSpellcardTextVmAddAddress);
    assert(th07::FindTh07GuiSpellcardVmSlot(th07::Th07GuiSpellcardVmKind::EnemyName)->scriptIndex ==
           th07::kTh07GuiSpellcardEnemyNameScript);
    assert(th07::FindTh07GuiSpellcardVmSlot(th07::Th07GuiSpellcardVmKind::EnemyNameCopy)->vmOffset ==
           th07::kTh07GuiSpellcardEnemyNameCopyVmOffset);

    return 0;
}
