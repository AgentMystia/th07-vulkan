#include "ClearBonusSystem.hpp"

namespace th07
{
const RankBonusRule kRankBonusRules[6] = {
    {EASY, "Easy Rank    *0.5", 0.5f, "0049849c"},
    {NORMAL, "Normal Rank  *1.0", 1.0f, "00498488"},
    {HARD, "Hard Rank    *1.2", 1.2f, "00498474"},
    {LUNATIC, "Lunatic Rank *1.5", 1.5f, "00498460"},
    {EXTRA, "Extra Rank   *2.0", 2.0f, "0049844c"},
    {PHANTASM, "Phantasm Rank*2.0", 2.0f, "00498438"},
};

int StageClearBonus(int currentStage)
{
    return currentStage * kStageClearBonusPerStage;
}

int PointItemClearBonus(int pointItemsInStage)
{
    return pointItemsInStage * kPointItemClearBonus;
}

int GrazeClearBonus(int grazeInStage)
{
    return grazeInStage * kGrazeClearBonus;
}

int CherryClearBonus(int cherry)
{
    return cherry * kCherryClearBonus;
}

int LifeClearBonus(int livesRemaining)
{
    return livesRemaining * kLifeClearBonus;
}

int BombClearBonus(int bombsRemaining)
{
    return bombsRemaining * kBombClearBonus;
}
} // namespace th07
