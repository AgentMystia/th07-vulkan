#pragma once

#include "Player.hpp"

namespace th07
{
struct BombData
{
    void (*calc)(Player *p);
    void (*draw)(Player *p);

    static void BombReimuACalc(Player *);
    static void BombReimuBCalc(Player *);
    static void BombMarisaACalc(Player *);
    static void BombMarisaBCalc(Player *);
    static void BombReimuADraw(Player *);
    static void BombReimuBDraw(Player *);
    static void BombMarisaADraw(Player *);
    static void BombMarisaBDraw(Player *);
    static void DarkenViewport(Player *player);
};
DIFFABLE_EXTERN_ARRAY(BombData, 6, g_BombData);
}; // namespace th07
