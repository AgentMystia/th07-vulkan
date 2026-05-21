#pragma once

#include <cmath>
#include <cstddef>
#include <d3dx8math.h>

#include "AnmManager.hpp"
#include "AnmVm.hpp"
#include "BulletManager.hpp"
#include "Chain.hpp"
#include "GameManager.hpp"
#include "PlayerLayout.hpp"
#include "ZunBool.hpp"
#include "ZunResult.hpp"
#include "inttypes.hpp"

namespace th07
{
struct Player;

enum PlayerDirection
{
    MOVEMENT_NONE,
    MOVEMENT_UP,
    MOVEMENT_DOWN,
    MOVEMENT_LEFT,
    MOVEMENT_RIGHT,
    MOVEMENT_UP_LEFT,
    MOVEMENT_UP_RIGHT,
    MOVEMENT_DOWN_LEFT,
    MOVEMENT_DOWN_RIGHT
};

enum Character
{
    CHARA_REIMU,
    CHARA_MARISA,
    CHARA_SAKUYA,
};

enum ShotType
{
    SHOT_TYPE_A,
    SHOT_TYPE_B,
};

enum BulletType
{
    BULLET_TYPE_0,
    BULLET_TYPE_1,
    BULLET_TYPE_2,
    BULLET_TYPE_LASER
};

enum PlayerState
{
    PLAYER_STATE_ALIVE,
    PLAYER_STATE_SPAWNING,
    PLAYER_STATE_DEAD,
    PLAYER_STATE_INVULNERABLE,
};

enum OrbState
{
    ORB_HIDDEN,
    ORB_UNFOCUSED,
    ORB_FOCUSING,
    ORB_FOCUSED,
    ORB_UNFOCUSING,
};

enum BulletState
{
    BULLET_STATE_UNUSED,
    BULLET_STATE_FIRED,
    BULLET_STATE_COLLIDED,
};
struct PlayerRect
{
    f32 posX;
    f32 posY;
    f32 sizeX;
    f32 sizeY;
};
ZUN_ASSERT_SIZE(PlayerRect, 0x10);

struct PlayerBullet
{
    AnmVm sprite;
    D3DXVECTOR3 position;
    D3DXVECTOR3 size;
    D3DXVECTOR2 velocity;
    f32 sidewaysMotion;
    D3DXVECTOR3 unk_134;
    ZunTimer unk_140;
    i16 damage;
    i16 bulletState;
    i16 bulletType;
    i16 unk_152;
    i16 spawnPositionIdx;

    void MoveHorizontal(f32 *position)
    {
        *position += this->velocity.x * g_Supervisor.effectiveFramerateMultiplier;
        this->sprite.pos.x = *position;
    }

    void MoveVertical(f32 *position)
    {
        *position += this->velocity.y * g_Supervisor.effectiveFramerateMultiplier;
        this->sprite.pos.y = *position;
    }
};
ZUN_ASSERT_SIZE(PlayerBullet, 0x158);

struct PlayerBombInfo
{
    u32 isInUse;
    i32 duration;
    ZunTimer timer;
    void (*calc)(Player *p);
    void (*draw)(Player *p);
    i32 reimuABombProjectilesState[8];
    f32 reimuABombProjectilesRelated[8];
    D3DXVECTOR3 bombRegionPositions[8];
    D3DXVECTOR3 bombRegionVelocities[8];
    AnmVm sprites[8][4];
};
ZUN_ASSERT_SIZE(PlayerBombInfo, 0x231c);

typedef i32 FireBulletResult;
#define FBR_STOP_SPAWNING (-2)
#define FBR_SPAWN_MORE (-1)

typedef FireBulletResult (*FireBulletCallback)(Player *, PlayerBullet *, u32, u32);
struct CharacterData
{
    f32 orthogonalMovementSpeed;
    f32 orthogonalMovementSpeedFocus;
    f32 diagonalMovementSpeed;
    f32 diagonalMovementSpeedFocus;
    FireBulletCallback fireBulletCallback;
    FireBulletCallback fireBulletFocusCallback;
};
ZUN_ASSERT_SIZE(CharacterData, 0x18);

struct CharacterPowerBulletData
{
    i16 waitBetweenBullets;
    i16 bulletFrame;
    ZunVec2 motion;
    ZunVec2 size;
    f32 direction;
    f32 velocity;
    u16 damage;
    u8 spawnPositionIdx;
    u8 bulletType;
    i16 anmFileIdx;
    i16 bulletSoundIdx;
};
ZUN_ASSERT_SIZE(CharacterPowerBulletData, 0x24);

struct CharacterPowerData
{
    i32 numBullets;
    i32 power;
    CharacterPowerBulletData *bullets;
};
ZUN_ASSERT_SIZE(CharacterPowerData, 0xc);

struct Th07PlayerOriginalAnmVmSlot : AnmVm {
    u8 th07PendingAnmVmFields_110_1b8[kTh07AnmVmColorOffset - sizeof(AnmVm)];
    D3DCOLOR originalColor;
    D3DCOLOR originalAlternateColor;
    u32 originalRenderFlags;
    u8 th07PendingAnmVmFields_1c4_1c6[kTh07AnmVmPendingInterruptOffset - 0x1c4];
    i16 originalPendingInterrupt;
    D3DXVECTOR3 originalDrawPosition;
    i16 originalActiveSpriteIndex;
    i16 originalBaseSpriteIndex;
    i16 originalScriptIndex;
    AnmRawInstr *originalBeginningOfScript;
    AnmRawInstr *originalCurrentInstruction;
    AnmLoadedSprite *originalLoadedSprite;
    u8 th07PendingAnmVmFields_1e8_230[kTh07AnmVmAuxVectorOffset - 0x1e8];
    D3DXVECTOR3 originalVector230;
    i32 originalLastSpriteSetFrame;
    u8 originalByte240;
    u8 originalByte241;
    u8 th07PendingAnmVmFields_242_24c[kPlayerAnmVmSlotSize - 0x242];
};
ZUN_ASSERT_SIZE(Th07PlayerOriginalAnmVmSlot, kPlayerAnmVmSlotSize);
static_assert(sizeof(AnmVm) == 0x0110);

struct Player
{
    Player();

    static ZunResult RegisterChain(u8 unk);
    static void CutChain();
    static ChainCallbackResult OnUpdate(Player *p);
    static ChainCallbackResult OnDrawHighPrio(Player *p);
    static ChainCallbackResult OnDrawLowPrio(Player *p);
    static ZunResult AddedCallback(Player *p);
    static ZunResult DeletedCallback(Player *p);

    static FireBulletResult FireSingleBullet(Player *, PlayerBullet *bullet, i32 bullet_idx, i32 framesSinceLastBullet,
                                             CharacterPowerData *powerData);

    static FireBulletResult FireBulletReimuA(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletReimuB(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletMarisaA(Player *, PlayerBullet *, u32, u32);
    static FireBulletResult FireBulletMarisaB(Player *, PlayerBullet *, u32, u32);

    static void StartFireBulletTimer(Player *);
    ZunResult HandlePlayerInputs();
    static void UpdatePlayerBullets(Player *);
    static ZunResult UpdateFireBulletsTimer(Player *);

    static void SpawnBullets(Player *, u32 timer);
    static void DrawBullets(Player *p);
    static void DrawBulletExplosions(Player *p);

    f32 AngleFromPlayer(D3DXVECTOR3 *pos);
    f32 AngleToPlayer(D3DXVECTOR3 *pos);
    i32 CheckGraze(D3DXVECTOR3 *center, D3DXVECTOR3 *size);
    i32 CalcKillBoxCollision(D3DXVECTOR3 *bulletCenter, D3DXVECTOR3 *bulletSize);
    i32 CalcLaserHitbox(D3DXVECTOR3 *laserCenter, D3DXVECTOR3 *laserSize, D3DXVECTOR3 *rotation, f32 angle,
                        i32 canGraze);
    i32 CalcDamageToEnemy(D3DXVECTOR3 *enemyPos, D3DXVECTOR3 *enemySize, i32 *unk);
    i32 CalcItemBoxCollision(D3DXVECTOR3 *center, D3DXVECTOR3 *size);
    void ScoreGraze(D3DXVECTOR3 *center);
    void Die();

    Th07PlayerOriginalAnmVmSlot playerSprite;
    Th07PlayerOriginalAnmVmSlot orbsSprite[3];
    D3DXVECTOR3 positionCenter;
    D3DXVECTOR3 th07PendingVector93c;
    D3DXVECTOR3 hitboxTopLeft;
    D3DXVECTOR3 hitboxBottomRight;
    D3DXVECTOR3 grazeTopLeft;
    D3DXVECTOR3 grazeBottomRight;
    D3DXVECTOR3 grabItemTopLeft;
    D3DXVECTOR3 grabItemBottomRight;
    D3DXVECTOR3 hitboxSize;
    D3DXVECTOR3 grazeSize;
    D3DXVECTOR3 grabItemSize;
    D3DXVECTOR3 orbsPosition[2];
    D3DXVECTOR2 movementDelta;
    D3DXVECTOR3 bombRegionPositions[32];
    D3DXVECTOR3 bombRegionSizes[32];
    i32 bombRegionDamages[32];
    i32 unk_838[32];
    PlayerRect bombProjectiles[16];
    ZunTimer laserTimer[2];
    u8 th07PendingPlayerFields_eec_23f0[0x23f0 - 0x0eec];
    f32 horizontalMovementSpeedMultiplierDuringBomb;
    f32 verticalMovementSpeedMultiplierDuringBomb;
    i32 respawnTimer;
    i32 bulletGracePeriod;
    i8 playerState;
    u8 unk_9e1;
    i8 orbState;
    i8 isFocus;
    u8 unk_9e4;
    ZunTimer focusMovementTimer;
    CharacterData characterData;
    PlayerDirection playerDirection;
    f32 previousHorizontalSpeed;
    f32 previousVerticalSpeed;
    i16 previousFrameInput;
    D3DXVECTOR3 positionOfLastEnemyHit;
    PlayerBullet bullets[80];
    ZunTimer fireBulletTimer;
    ZunTimer invulnerabilityTimer;
    FireBulletCallback fireBulletCallback;
    FireBulletCallback fireBulletFocusCallback;
    PlayerBombInfo bombInfo;
    ChainElem *chainCalc;
    ChainElem *chainDraw1;
    ChainElem *chainDraw2;
#pragma var_order(x, y)
    void inline SetToTopLeftPos(AnmVm *sprite)
    {
        sprite->pos[0] += g_GameManager.arcadeRegionTopLeftPos.x;
        sprite->pos[1] += g_GameManager.arcadeRegionTopLeftPos.y;
        sprite->pos[2] = 0.0;
    };
};
static_assert(offsetof(Player, playerSprite) == kPlayerPrimaryAnmVmOffset);
static_assert(offsetof(Player, orbsSprite[0]) == kPlayerLeftOptionAnmVmOffset);
static_assert(offsetof(Player, orbsSprite[1]) == kPlayerRightOptionAnmVmOffset);
static_assert(offsetof(Player, orbsSprite[2]) == kPlayerFourthAnmVmOffset);
static_assert(offsetof(Player, positionCenter) == kPlayerPositionOffset);
static_assert(offsetof(Player, th07PendingVector93c) == kPlayerPendingVector93cOffset);
static_assert(offsetof(Player, hitboxTopLeft) == kPlayerHitboxMinOffset);
static_assert(offsetof(Player, hitboxBottomRight) == kPlayerHitboxMaxOffset);
static_assert(offsetof(Player, grazeTopLeft) == kPlayerGrazeBoxMinOffset);
static_assert(offsetof(Player, grazeBottomRight) == kPlayerGrazeBoxMaxOffset);
static_assert(offsetof(Player, grabItemTopLeft) == kPlayerGrabItemBoxMinOffset);
static_assert(offsetof(Player, grabItemBottomRight) == kPlayerGrabItemBoxMaxOffset);
static_assert(offsetof(Player, hitboxSize) == kPlayerHitboxSizeOffset);
static_assert(offsetof(Player, grazeSize) == kPlayerGrazeBoxSizeOffset);
static_assert(offsetof(Player, grabItemSize) == kPlayerGrabItemSizeOffset);
static_assert(offsetof(Player, orbsPosition[0]) == kPlayerLeftOptionPositionOffset);
static_assert(offsetof(Player, orbsPosition[1]) == kPlayerRightOptionPositionOffset);
static_assert(offsetof(Player, movementDelta) == kPlayerMovementDeltaOffset);
static_assert(offsetof(Player, horizontalMovementSpeedMultiplierDuringBomb) ==
              kPlayerBombHorizontalSpeedMultiplierOffset);
static_assert(offsetof(Player, verticalMovementSpeedMultiplierDuringBomb) ==
              kPlayerBombVerticalSpeedMultiplierOffset);

DIFFABLE_EXTERN(Player, g_Player);
}; // namespace th07
