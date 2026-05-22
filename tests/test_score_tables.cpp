#include "Th07ScoreTables.hpp"

#include <cassert>

int main()
{
    static_assert(th07::kTh07GuiScoreUpdateFunctionAddress == 0x0042d8d5);
    static_assert(th07::kTh07GuiScoreMaxClampInstructionAddress == 0x0042deb5);
    static_assert(th07::kTh07GuiScoreMaxImmediateAddress == 0x0042deb8);
    static_assert(th07::kTh07GuiScoreStepShiftInstructionAddress == 0x0042df08);
    static_assert(th07::kTh07GuiScoreStepMaxCompareInstructionAddress == 0x0042df0e);
    static_assert(th07::kTh07GuiScoreStepMaxCompareImmediateAddress == 0x0042df11);
    static_assert(th07::kTh07GuiScoreStepMaxWriteInstructionAddress == 0x0042df17);
    static_assert(th07::kTh07GuiScoreStepMaxWriteImmediateAddress == 0x0042df1a);
    static_assert(th07::kTh07GuiScoreStepMinWriteInstructionAddress == 0x0042df26);
    static_assert(th07::kTh07GuiScoreStepMinWriteImmediateAddress == 0x0042df29);
    static_assert(th07::kTh07HighScoreCompareInstructionAddress == 0x0042dfd0);
    static_assert(th07::kTh07HighScoreWriteInstructionAddress == 0x0042dfe5);
    static_assert(th07::kTh07HighScoreShotCopyReadInstructionAddress == 0x0042dff4);
    static_assert(th07::kTh07HighScoreShotCopyWriteInstructionAddress == 0x0042dff7);
    static_assert(th07::kTh07ExtendAwardFunctionAddress == 0x0042d83a);
    static_assert(th07::kTh07AddLifeFunctionAddress == 0x0042d5cd);
    static_assert(th07::kTh07AddBombFunctionAddress == 0x0042d612);
    static_assert(th07::kTh07ExtendAwardLifeStockReadAddress == 0x0042d847);
    static_assert(th07::kTh07ExtendAwardLifeStockCompareAddress == 0x0042d84f);
    static_assert(th07::kTh07ExtendAwardLifeAddCallAddress == 0x0042d859);
    static_assert(th07::kTh07ExtendAwardLifeSoundPushAddress == 0x0042d860);
    static_assert(th07::kTh07ExtendAwardLifeNotificationDurationPushAddress == 0x0042d86c);
    static_assert(th07::kTh07ExtendAwardLifeFlagAndAddress == 0x0042d87e);
    static_assert(th07::kTh07ExtendAwardLifeFlagOrAddress == 0x0042d881);
    static_assert(th07::kTh07ExtendAwardLifeFlagWriteAddress == 0x0042d884);
    static_assert(th07::kTh07ExtendAwardBombStockReadAddress == 0x0042d891);
    static_assert(th07::kTh07ExtendAwardBombStockCompareAddress == 0x0042d899);
    static_assert(th07::kTh07ExtendAwardBombAddCallAddress == 0x0042d8a3);
    static_assert(th07::kTh07ExtendAwardBombSoundPushAddress == 0x0042d8aa);
    static_assert(th07::kTh07ExtendAwardBombNotificationDurationPushAddress == 0x0042d8b6);
    static_assert(th07::kTh07ExtendAwardBombFlagAndAddress == 0x0042d8c8);
    static_assert(th07::kTh07ExtendAwardBombFlagOrAddress == 0x0042d8cb);
    static_assert(th07::kTh07ExtendAwardBombFlagWriteAddress == 0x0042d8ce);
    static_assert(th07::kTh07ItemCollectionFunctionAddress == 0x00432990);
    static_assert(th07::kTh07PointItemExtendDifficultyCompareAddress == 0x004332e0);
    static_assert(th07::kTh07PointItemExtendNormalEarlyCompareAddress == 0x004332ee);
    static_assert(th07::kTh07PointItemExtendNormalEarlyScaleAddress == 0x004332fd);
    static_assert(th07::kTh07PointItemExtendNormalEarlyBaseAddAddress == 0x00433300);
    static_assert(th07::kTh07PointItemExtendNormalMidCompareAddress == 0x00433313);
    static_assert(th07::kTh07PointItemExtendNormalMidScaleAddress == 0x00433325);
    static_assert(th07::kTh07PointItemExtendNormalMidBaseAddAddress == 0x0043332b);
    static_assert(th07::kTh07PointItemExtendNormalLateScaleAddress == 0x00433347);
    static_assert(th07::kTh07PointItemExtendNormalLateBaseAddAddress == 0x0043334d);
    static_assert(th07::kTh07PointItemExtendExtraFirstWriteAddress == 0x0043336e);
    static_assert(th07::kTh07PointItemExtendExtraSecondWriteAddress == 0x00433389);
    static_assert(th07::kTh07PointItemExtendExtraLateScaleAddress == 0x0043339d);
    static_assert(th07::kTh07PointItemExtendExtraLateBaseAddAddress == 0x004333a3);
    static_assert(th07::kTh07PointItemExtendProgressCompareAddress == 0x004333bd);
    static_assert(th07::kTh07PointItemExtendAwardCallAddress == 0x004333ca);
    static_assert(th07::kTh07LifeItemExtendAwardCallAddress == 0x004335ba);
    static_assert(th07::kTh07GuiScoreFieldOffset == 0x00);
    static_assert(th07::kTh07ScoreFieldOffset == 0x04);
    static_assert(th07::kTh07NextScoreIncrementFieldOffset == 0x08);
    static_assert(th07::kTh07HighScoreFieldOffset == 0x0c);
    static_assert(th07::kTh07HighScoreShotTypeFieldOffset == 0x10);
    static_assert(th07::kTh07CurrentShotTypeFieldOffset == 0x20);
    static_assert(th07::kTh07PointItemExtendProgressFieldOffset == 0x28);
    static_assert(th07::kTh07PointItemExtendCountFieldOffset == 0x2c);
    static_assert(th07::kTh07PointItemExtendNextThresholdFieldOffset == 0x30);
    static_assert(th07::kTh07ExtendLifeStockFieldOffset == 0x5c);
    static_assert(th07::kTh07ExtendBombStockFieldOffset == 0x68);
    static_assert(th07::kTh07GuiScoreMax == 999999999);
    static_assert(th07::kTh07GuiScoreStepShift == 5);
    static_assert(th07::kTh07GuiScoreStepMax == 0x0008d55e);
    static_assert(th07::kTh07GuiScoreStepMin == 1);
    static_assert(th07::kTh07ExtendStockMaxExclusive == 8);
    static_assert(th07::kTh07ExtendStockIncrement == 1);
    static_assert(th07::kTh07ExtendSoundIndex == 0x1c);
    static_assert(th07::kTh07ExtendNotificationDurationFrames == 200);
    static_assert(th07::kTh07ExtendNotificationFlagAddress == 0x0049fbf4);
    static_assert(th07::kTh07ExtendLifeNotificationFlagMask == 0xfffffffc);
    static_assert(th07::kTh07ExtendLifeNotificationFlagBits == 0x00000002);
    static_assert(th07::kTh07ExtendBombNotificationFlagMask == 0xfffffff3);
    static_assert(th07::kTh07ExtendBombNotificationFlagBits == 0x00000008);
    static_assert(th07::kTh07PointItemExtendNormalDifficultyCount == 4);
    static_assert(th07::kTh07PointItemExtendNormalEarlyCountLimit == 3);
    static_assert(th07::kTh07PointItemExtendNormalEarlyStep == 75);
    static_assert(th07::kTh07PointItemExtendNormalEarlyBase == 50);
    static_assert(th07::kTh07PointItemExtendNormalMidCountLimit == 5);
    static_assert(th07::kTh07PointItemExtendNormalMidSubtract == 3);
    static_assert(th07::kTh07PointItemExtendNormalMidStep == 150);
    static_assert(th07::kTh07PointItemExtendNormalMidBase == 300);
    static_assert(th07::kTh07PointItemExtendNormalLateSubtract == 5);
    static_assert(th07::kTh07PointItemExtendNormalLateStep == 200);
    static_assert(th07::kTh07PointItemExtendNormalLateBase == 800);
    static_assert(th07::kTh07PointItemExtendExtraFirstThreshold == 200);
    static_assert(th07::kTh07PointItemExtendExtraSecondThreshold == 500);
    static_assert(th07::kTh07PointItemExtendExtraLateSubtract == 2);
    static_assert(th07::kTh07PointItemExtendExtraLateStep == 500);
    static_assert(th07::kTh07PointItemExtendExtraLateBase == 800);

    th07::Th07GuiScoreStepState state = th07::StepTh07GuiScore(0, 1000000000, 0);
    assert(state.score == th07::kTh07GuiScoreMax);
    assert(state.nextScoreIncrement == th07::kTh07GuiScoreStepMax);
    assert(state.guiScore == th07::kTh07GuiScoreStepMax);

    state = th07::StepTh07GuiScore(100, 90, 5);
    assert(state.guiScore == 100);
    assert(state.score == 100);
    assert(state.nextScoreIncrement == 0);

    state = th07::StepTh07GuiScore(100, 101, 0);
    assert(state.guiScore == 101);
    assert(state.score == 101);
    assert(state.nextScoreIncrement == 0);

    state = th07::StepTh07GuiScore(100, 10000, 500);
    assert(state.guiScore == 600);
    assert(state.score == 10000);
    assert(state.nextScoreIncrement == 500);

    state = th07::StepTh07GuiScore(90, 100, 50);
    assert(state.guiScore == 100);
    assert(state.score == 100);
    assert(state.nextScoreIncrement == 0);

    state = th07::StepTh07GuiScore(500, 500, 7);
    assert(state.guiScore == 500);
    assert(state.score == 500);
    assert(state.nextScoreIncrement == 7);

    th07::Th07HighScoreStepState highScoreState = th07::StepTh07HighScore(1000, 999, 2, 5);
    assert(highScoreState.guiScore == 1000);
    assert(highScoreState.highScore == 1000);
    assert(highScoreState.highScoreShotType == 5);
    assert(highScoreState.currentShotType == 5);

    highScoreState = th07::StepTh07HighScore(1000, 1000, 2, 5);
    assert(highScoreState.highScore == 1000);
    assert(highScoreState.highScoreShotType == 2);

    highScoreState = th07::StepTh07HighScore(900, 1000, 2, 5);
    assert(highScoreState.highScore == 1000);
    assert(highScoreState.highScoreShotType == 2);

    th07::Th07ExtendAwardState extendState = th07::ApplyTh07ExtendAward(7, 3, 0xffffffff);
    assert(extendState.award == th07::Th07ExtendAwardKind::Life);
    assert(extendState.lifeStock == 8);
    assert(extendState.bombStock == 3);
    assert(extendState.notificationFlags == 0xfffffffe);

    extendState = th07::ApplyTh07ExtendAward(8, 7, 0xffffffff);
    assert(extendState.award == th07::Th07ExtendAwardKind::Bomb);
    assert(extendState.lifeStock == 8);
    assert(extendState.bombStock == 8);
    assert(extendState.notificationFlags == 0xfffffffb);

    extendState = th07::ApplyTh07ExtendAward(8, 8, 0x12345678);
    assert(extendState.award == th07::Th07ExtendAwardKind::None);
    assert(extendState.lifeStock == 8);
    assert(extendState.bombStock == 8);
    assert(extendState.notificationFlags == 0x12345678);

    assert(th07::ComputeTh07PointItemExtendThreshold(0, 0) == 50);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 1) == 125);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 2) == 200);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 3) == 300);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 4) == 450);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 5) == 800);
    assert(th07::ComputeTh07PointItemExtendThreshold(0, 6) == 1000);
    assert(th07::ComputeTh07PointItemExtendThreshold(3, 7) == 1200);
    assert(th07::ComputeTh07PointItemExtendThreshold(4, 0) == 200);
    assert(th07::ComputeTh07PointItemExtendThreshold(4, 1) == 500);
    assert(th07::ComputeTh07PointItemExtendThreshold(4, 2) == 800);
    assert(th07::ComputeTh07PointItemExtendThreshold(5, 3) == 1300);

    return 0;
}
