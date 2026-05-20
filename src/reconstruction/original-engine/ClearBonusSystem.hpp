#pragma once

#include "GameManager.hpp"

namespace th07
{
enum class BonusToastKind
{
    None = 0,
    FullPowerMode = 1,
    SupernaturalBorder = 2,
    CherryPointMax = 3,
    BorderBonus = 4,
};

struct RankBonusRule
{
    Difficulty difficulty;
    const char *label;
    float multiplier;
    const char *ghidraStringAddress;
};

constexpr int kStageClearBonusPerStage = 1000000;
constexpr int kPointItemClearBonus = 50000;
constexpr int kGrazeClearBonus = 500;
constexpr int kCherryClearBonus = 10;
constexpr int kLifeClearBonus = 20000000;
constexpr int kBombClearBonus = 4000000;
constexpr float kPlayerPenaltyHalf = 0.5f;
constexpr float kPlayerPenaltyLow = 0.2f;

extern const RankBonusRule kRankBonusRules[6];

int StageClearBonus(int currentStage);
int PointItemClearBonus(int pointItemsInStage);
int GrazeClearBonus(int grazeInStage);
int CherryClearBonus(int cherry);
int LifeClearBonus(int livesRemaining);
int BombClearBonus(int bombsRemaining);
} // namespace th07
