#pragma once

#include "inttypes.hpp"

namespace th07
{
inline constexpr u32 kTh07GuiScoreUpdateFunctionAddress = 0x0042d8d5;
inline constexpr u32 kTh07GuiScoreMaxClampInstructionAddress = 0x0042deb5;
inline constexpr u32 kTh07GuiScoreMaxImmediateAddress = 0x0042deb8;
inline constexpr u32 kTh07GuiScoreStepShiftInstructionAddress = 0x0042df08;
inline constexpr u32 kTh07GuiScoreStepMaxCompareInstructionAddress = 0x0042df0e;
inline constexpr u32 kTh07GuiScoreStepMaxCompareImmediateAddress = 0x0042df11;
inline constexpr u32 kTh07GuiScoreStepMaxWriteInstructionAddress = 0x0042df17;
inline constexpr u32 kTh07GuiScoreStepMaxWriteImmediateAddress = 0x0042df1a;
inline constexpr u32 kTh07GuiScoreStepMinWriteInstructionAddress = 0x0042df26;
inline constexpr u32 kTh07GuiScoreStepMinWriteImmediateAddress = 0x0042df29;
inline constexpr u32 kTh07HighScoreCompareInstructionAddress = 0x0042dfd0;
inline constexpr u32 kTh07HighScoreWriteInstructionAddress = 0x0042dfe5;
inline constexpr u32 kTh07HighScoreShotCopyReadInstructionAddress = 0x0042dff4;
inline constexpr u32 kTh07HighScoreShotCopyWriteInstructionAddress = 0x0042dff7;
inline constexpr u32 kTh07ExtendAwardFunctionAddress = 0x0042d83a;
inline constexpr u32 kTh07AddLifeFunctionAddress = 0x0042d5cd;
inline constexpr u32 kTh07AddBombFunctionAddress = 0x0042d612;
inline constexpr u32 kTh07ExtendAwardLifeStockReadAddress = 0x0042d847;
inline constexpr u32 kTh07ExtendAwardLifeStockCompareAddress = 0x0042d84f;
inline constexpr u32 kTh07ExtendAwardLifeAddCallAddress = 0x0042d859;
inline constexpr u32 kTh07ExtendAwardLifeSoundPushAddress = 0x0042d860;
inline constexpr u32 kTh07ExtendAwardLifeNotificationDurationPushAddress = 0x0042d86c;
inline constexpr u32 kTh07ExtendAwardLifeFlagAndAddress = 0x0042d87e;
inline constexpr u32 kTh07ExtendAwardLifeFlagOrAddress = 0x0042d881;
inline constexpr u32 kTh07ExtendAwardLifeFlagWriteAddress = 0x0042d884;
inline constexpr u32 kTh07ExtendAwardBombStockReadAddress = 0x0042d891;
inline constexpr u32 kTh07ExtendAwardBombStockCompareAddress = 0x0042d899;
inline constexpr u32 kTh07ExtendAwardBombAddCallAddress = 0x0042d8a3;
inline constexpr u32 kTh07ExtendAwardBombSoundPushAddress = 0x0042d8aa;
inline constexpr u32 kTh07ExtendAwardBombNotificationDurationPushAddress = 0x0042d8b6;
inline constexpr u32 kTh07ExtendAwardBombFlagAndAddress = 0x0042d8c8;
inline constexpr u32 kTh07ExtendAwardBombFlagOrAddress = 0x0042d8cb;
inline constexpr u32 kTh07ExtendAwardBombFlagWriteAddress = 0x0042d8ce;
inline constexpr u32 kTh07ItemCollectionFunctionAddress = 0x00432990;
inline constexpr u32 kTh07PointItemExtendDifficultyCompareAddress = 0x004332e0;
inline constexpr u32 kTh07PointItemExtendNormalEarlyCompareAddress = 0x004332ee;
inline constexpr u32 kTh07PointItemExtendNormalEarlyScaleAddress = 0x004332fd;
inline constexpr u32 kTh07PointItemExtendNormalEarlyBaseAddAddress = 0x00433300;
inline constexpr u32 kTh07PointItemExtendNormalMidCompareAddress = 0x00433313;
inline constexpr u32 kTh07PointItemExtendNormalMidScaleAddress = 0x00433325;
inline constexpr u32 kTh07PointItemExtendNormalMidBaseAddAddress = 0x0043332b;
inline constexpr u32 kTh07PointItemExtendNormalLateScaleAddress = 0x00433347;
inline constexpr u32 kTh07PointItemExtendNormalLateBaseAddAddress = 0x0043334d;
inline constexpr u32 kTh07PointItemExtendExtraFirstWriteAddress = 0x0043336e;
inline constexpr u32 kTh07PointItemExtendExtraSecondWriteAddress = 0x00433389;
inline constexpr u32 kTh07PointItemExtendExtraLateScaleAddress = 0x0043339d;
inline constexpr u32 kTh07PointItemExtendExtraLateBaseAddAddress = 0x004333a3;
inline constexpr u32 kTh07PointItemExtendProgressCompareAddress = 0x004333bd;
inline constexpr u32 kTh07PointItemExtendAwardCallAddress = 0x004333ca;
inline constexpr u32 kTh07LifeItemExtendAwardCallAddress = 0x004335ba;

inline constexpr u32 kTh07GuiScoreFieldOffset = 0x00;
inline constexpr u32 kTh07ScoreFieldOffset = 0x04;
inline constexpr u32 kTh07NextScoreIncrementFieldOffset = 0x08;
inline constexpr u32 kTh07HighScoreFieldOffset = 0x0c;
inline constexpr u32 kTh07HighScoreShotTypeFieldOffset = 0x10;
inline constexpr u32 kTh07CurrentShotTypeFieldOffset = 0x20;
inline constexpr u32 kTh07PointItemExtendProgressFieldOffset = 0x28;
inline constexpr u32 kTh07PointItemExtendCountFieldOffset = 0x2c;
inline constexpr u32 kTh07PointItemExtendNextThresholdFieldOffset = 0x30;
inline constexpr u32 kTh07ExtendLifeStockFieldOffset = 0x5c;
inline constexpr u32 kTh07ExtendBombStockFieldOffset = 0x68;
inline constexpr u32 kTh07GuiScoreMax = 999999999;
inline constexpr u32 kTh07GuiScoreStepShift = 5;
inline constexpr u32 kTh07GuiScoreStepMax = 0x0008d55e;
inline constexpr u32 kTh07GuiScoreStepMin = 1;
inline constexpr int kTh07ExtendStockMaxExclusive = 8;
inline constexpr int kTh07ExtendStockIncrement = 1;
inline constexpr int kTh07ExtendSoundIndex = 0x1c;
inline constexpr int kTh07ExtendNotificationDurationFrames = 200;
inline constexpr u32 kTh07ExtendNotificationFlagAddress = 0x0049fbf4;
inline constexpr u32 kTh07ExtendLifeNotificationFlagMask = 0xfffffffc;
inline constexpr u32 kTh07ExtendLifeNotificationFlagBits = 0x00000002;
inline constexpr u32 kTh07ExtendBombNotificationFlagMask = 0xfffffff3;
inline constexpr u32 kTh07ExtendBombNotificationFlagBits = 0x00000008;
inline constexpr int kTh07PointItemExtendNormalDifficultyCount = 4;
inline constexpr int kTh07PointItemExtendNormalEarlyCountLimit = 3;
inline constexpr int kTh07PointItemExtendNormalEarlyStep = 75;
inline constexpr int kTh07PointItemExtendNormalEarlyBase = 50;
inline constexpr int kTh07PointItemExtendNormalMidCountLimit = 5;
inline constexpr int kTh07PointItemExtendNormalMidSubtract = 3;
inline constexpr int kTh07PointItemExtendNormalMidStep = 150;
inline constexpr int kTh07PointItemExtendNormalMidBase = 300;
inline constexpr int kTh07PointItemExtendNormalLateSubtract = 5;
inline constexpr int kTh07PointItemExtendNormalLateStep = 200;
inline constexpr int kTh07PointItemExtendNormalLateBase = 800;
inline constexpr int kTh07PointItemExtendExtraFirstThreshold = 200;
inline constexpr int kTh07PointItemExtendExtraSecondThreshold = 500;
inline constexpr int kTh07PointItemExtendExtraLateSubtract = 2;
inline constexpr int kTh07PointItemExtendExtraLateStep = 500;
inline constexpr int kTh07PointItemExtendExtraLateBase = 800;

struct Th07GuiScoreStepState
{
    u32 guiScore;
    u32 score;
    u32 nextScoreIncrement;
};

struct Th07HighScoreStepState
{
    u32 guiScore;
    u32 highScore;
    u8 highScoreShotType;
    u8 currentShotType;
};

enum class Th07ExtendAwardKind
{
    None,
    Life,
    Bomb,
};

struct Th07ExtendAwardState
{
    int lifeStock;
    int bombStock;
    u32 notificationFlags;
    Th07ExtendAwardKind award;
};

Th07GuiScoreStepState StepTh07GuiScore(u32 guiScore, u32 score, u32 nextScoreIncrement);
Th07HighScoreStepState StepTh07HighScore(u32 guiScore, u32 highScore, u8 highScoreShotType,
                                         u8 currentShotType);
Th07ExtendAwardState ApplyTh07ExtendAward(int lifeStock, int bombStock, u32 notificationFlags);
int ComputeTh07PointItemExtendThreshold(int difficultyIndex, int extendCount);
}; // namespace th07
