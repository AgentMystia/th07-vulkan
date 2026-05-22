#include "Th07ScoreTables.hpp"

namespace th07
{
Th07GuiScoreStepState StepTh07GuiScore(u32 guiScore, u32 score, u32 nextScoreIncrement)
{
    Th07GuiScoreStepState state {guiScore, score, nextScoreIncrement};

    if (kTh07GuiScoreMax < state.score) {
        state.score = kTh07GuiScoreMax;
    }
    if (state.guiScore != state.score) {
        if (state.score < state.guiScore) {
            state.score = state.guiScore;
        }

        u32 scoreIncrement = (state.score - state.guiScore) >> kTh07GuiScoreStepShift;
        if (scoreIncrement < kTh07GuiScoreStepMax) {
            if (scoreIncrement == 0) {
                scoreIncrement = kTh07GuiScoreStepMin;
            }
        }
        else {
            scoreIncrement = kTh07GuiScoreStepMax;
        }

        if (state.nextScoreIncrement < scoreIncrement) {
            state.nextScoreIncrement = scoreIncrement;
        }
        if (state.score < state.guiScore + state.nextScoreIncrement) {
            state.nextScoreIncrement = state.score - state.guiScore;
        }

        state.guiScore += state.nextScoreIncrement;
        if (state.score <= state.guiScore) {
            state.nextScoreIncrement = 0;
            state.guiScore = state.score;
        }
    }

    return state;
}

Th07HighScoreStepState StepTh07HighScore(u32 guiScore, u32 highScore, u8 highScoreShotType,
                                         u8 currentShotType)
{
    Th07HighScoreStepState state {guiScore, highScore, highScoreShotType, currentShotType};

    if (state.highScore < state.guiScore) {
        state.highScore = state.guiScore;
        state.highScoreShotType = state.currentShotType;
    }

    return state;
}

Th07ExtendAwardState ApplyTh07ExtendAward(int lifeStock, int bombStock, u32 notificationFlags)
{
    Th07ExtendAwardState state {lifeStock, bombStock, notificationFlags, Th07ExtendAwardKind::None};

    if (state.lifeStock < kTh07ExtendStockMaxExclusive) {
        state.lifeStock += kTh07ExtendStockIncrement;
        state.notificationFlags =
            (state.notificationFlags & kTh07ExtendLifeNotificationFlagMask) |
            kTh07ExtendLifeNotificationFlagBits;
        state.award = Th07ExtendAwardKind::Life;
    }
    else if (state.bombStock < kTh07ExtendStockMaxExclusive) {
        state.bombStock += kTh07ExtendStockIncrement;
        state.notificationFlags =
            (state.notificationFlags & kTh07ExtendBombNotificationFlagMask) |
            kTh07ExtendBombNotificationFlagBits;
        state.award = Th07ExtendAwardKind::Bomb;
    }

    return state;
}

int ComputeTh07PointItemExtendThreshold(int difficultyIndex, int extendCount)
{
    if (difficultyIndex < kTh07PointItemExtendNormalDifficultyCount) {
        if (extendCount < kTh07PointItemExtendNormalEarlyCountLimit) {
            return extendCount * kTh07PointItemExtendNormalEarlyStep +
                   kTh07PointItemExtendNormalEarlyBase;
        }
        if (extendCount < kTh07PointItemExtendNormalMidCountLimit) {
            return (extendCount - kTh07PointItemExtendNormalMidSubtract) *
                       kTh07PointItemExtendNormalMidStep +
                   kTh07PointItemExtendNormalMidBase;
        }
        return (extendCount - kTh07PointItemExtendNormalLateSubtract) *
                   kTh07PointItemExtendNormalLateStep +
               kTh07PointItemExtendNormalLateBase;
    }

    if (extendCount == 0) {
        return kTh07PointItemExtendExtraFirstThreshold;
    }
    if (extendCount == 1) {
        return kTh07PointItemExtendExtraSecondThreshold;
    }
    return (extendCount - kTh07PointItemExtendExtraLateSubtract) *
               kTh07PointItemExtendExtraLateStep +
           kTh07PointItemExtendExtraLateBase;
}
}; // namespace th07
