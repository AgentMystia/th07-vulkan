#include "Player.hpp"

#include "AnmManager.hpp"
#include "AnmVm.hpp"
#include "BombData.hpp"
#include "BulletData.hpp"
#include "BulletManager.hpp"
#include "ChainPriorities.hpp"
#include "EclManager.hpp"
#include "EffectManager.hpp"
#include "EnemyManager.hpp"
#include "GameManager.hpp"
#include "Gui.hpp"
#include "ItemManager.hpp"
#include "PlayerBombTables.hpp"
#include "Rng.hpp"
#include "ScreenEffect.hpp"
#include "SoundPlayer.hpp"
#include "Supervisor.hpp"
#include "ZunBool.hpp"
#include "Th07PlayerShotTables.hpp"
#include "i18n.hpp"
#include "utils.hpp"

namespace th07
{
struct Th07PlayerModeConfigOwner {
    u8 pending_0_8[kPlayerModeConfigCurrentPowerCapOffset];
    i32 currentPowerCap;
};

#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
extern "C" float FUN_00431930(float, float);
extern "C" Effect *TH07_PLAYER_THISCALL FUN_0041c1c0(EffectManager *, i32, D3DXVECTOR3 *, i32, ZunColor);
extern "C" i32 TH07_PLAYER_THISCALL FUN_0042d6d8(void *, u32, u32, u32, u32);
extern "C" void TH07_PLAYER_THISCALL FUN_004394c7(void *, i32);
extern "C" void TH07_PLAYER_THISCALL FUN_0043958d(void *, void *, void *);
extern "C" i32 TH07_PLAYER_FASTCALL FUN_0043bdc0(Player *, void *, u32, void *);
extern "C" void TH07_PLAYER_THISCALL FUN_0044ea20(void *, void *, void *);
extern "C" void TH07_PLAYER_THISCALL FUN_0044f9a0(void *, void *);
extern "C" i32 TH07_PLAYER_THISCALL FUN_00450d60(void *, void *);
#endif

DIFFABLE_STATIC(Player, g_Player);
DIFFABLE_STATIC(u16, g_Th07PlayerOptionResetPendingInterruptMirror);
DIFFABLE_STATIC(u32, g_Th07PlayerOptionResetPendingInterruptState);
DIFFABLE_STATIC(Th07PlayerModeConfigOwner *, g_Th07PlayerModeConfigOwner);
DIFFABLE_STATIC_ASSIGN(f32, g_Th07PlayerModeOneFrame) = kTh07TimerOneFrame;
DIFFABLE_STATIC_ASSIGN(f32, g_Th07PlayerMode1ScaleYMultiplier) = kPlayerMode1FadeScaleYMultiplier;
DIFFABLE_STATIC_ASSIGN(f32, g_Th07PlayerMode1FadeFramesFloat) = kPlayerMode1FadeFramesFloat;
DIFFABLE_STATIC_ASSIGN(i32, g_Th07PlayerMode1FadeFrames) = kPlayerMode1FadeFrames;
DIFFABLE_STATIC_ASSIGN(i32, g_Th07PlayerMode1AlphaScale) = 0xff;

namespace {

#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
using Th07ShtSpawnCallback = i32(TH07_PLAYER_FASTCALL *)(Player *, void *, u32, void *);
using Th07ShtUpdateCallback = i32(TH07_PLAYER_FASTCALL *)(Player *, void *);
using Th07ShtDrawCallback = void(TH07_PLAYER_FASTCALL *)(Player *, void *);
using Th07ShtCollisionCallback = i32(TH07_PLAYER_FASTCALL *)(Player *, void *, D3DXVECTOR3 *);

inline i32 ReadTh07PlayerSpawnPowerForObjdiff()
{
    i32 currentPower;
    __asm__ __volatile__(
        "movl 0x00626278, %%ecx\n\t"
        "flds 0x7c(%%ecx)\n\t"
        "call FUN_0048b8a0"
        : "=a"(currentPower)
        :
        : "ecx", "edx", "memory");
    return currentPower;
}
#endif

void ApplyTh07InitialPlayerModeOwnerState(Player *p)
{
    const PlayerInitialModeOwnerState state = BuildPlayerInitialModeOwnerState();
    p->Th07ModeState() = state.modeState;
    p->Th07OptionState() = state.optionState;
    p->Th07FocusHeld() = state.focusHeld;
    p->Th07ModeTransitionRequest() = state.transitionRequest;
    p->Th07BombCommonEffectXBits() = state.commonEffectXBits;
    p->Th07BombCommonEffectYBits() = state.commonEffectYBits;
    p->Th07BombCommonEffectDuration() = state.commonEffectDurationFrames;
}

constexpr float Th07OrthogonalSpeed(Th07ShotType shotType)
{
    return GetTh07UnfocusedPlayerShotSummary(shotType).orthogonalMovementSpeed();
}

constexpr float Th07FocusedOrthogonalSpeed(Th07ShotType shotType)
{
    return GetTh07UnfocusedPlayerShotSummary(shotType).focusedOrthogonalMovementSpeed();
}

constexpr float Th07DiagonalSpeed(Th07ShotType shotType)
{
    return GetTh07UnfocusedPlayerShotSummary(shotType).diagonalMovementSpeed();
}

constexpr float Th07FocusedDiagonalSpeed(Th07ShotType shotType)
{
    return GetTh07UnfocusedPlayerShotSummary(shotType).focusedDiagonalMovementSpeed();
}

Th07ShotType Th07ShotTypeFromGameManager()
{
    return static_cast<Th07ShotType>(g_GameManager.CharacterShotType());
}

PlayerMovementSpeedInput BuildTh07PlayerMovementSpeedInput(Th07ShotType shotType)
{
    const Th07PlayerShotSummary &summary = GetTh07UnfocusedPlayerShotSummary(shotType);
    return {
        summary.orthogonalMovementSpeed(),
        summary.focusedOrthogonalMovementSpeed(),
        summary.diagonalMovementSpeed(),
        summary.focusedDiagonalMovementSpeed(),
    };
}

void ApplyTh07PlayerHorizontalAnimation(Player *p, float horizontalSpeed)
{
    const float previousHorizontalSpeed = p->Th07MovementHorizontalSpeed();
    if (horizontalSpeed < 0.0f && previousHorizontalSpeed >= 0.0f)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, kPlayerMovementLeanLeftAnmScript);
    }
    else if (horizontalSpeed == 0.0f && previousHorizontalSpeed < 0.0f)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, kPlayerMovementCenterFromLeftAnmScript);
    }

    if (horizontalSpeed > 0.0f && previousHorizontalSpeed <= 0.0f)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, kPlayerMovementLeanRightAnmScript);
    }
    else if (horizontalSpeed == 0.0f && previousHorizontalSpeed > 0.0f)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, kPlayerMovementCenterFromRightAnmScript);
    }
}

void ApplyTh07PlayerCollisionBoxes(Player *p)
{
    p->hitboxTopLeft = p->positionCenter - p->hitboxSize;
    p->hitboxBottomRight = p->positionCenter + p->hitboxSize;
    p->grazeTopLeft = p->positionCenter - p->grazeSize;
    p->grazeBottomRight = p->positionCenter + p->grazeSize;
    p->grabItemTopLeft = p->positionCenter - p->grabItemSize;
    p->grabItemBottomRight = p->positionCenter + p->grabItemSize;
}

void ApplyTh07PlayerCollisionSizes(Player *p, const Th07PlayerShotSummary &shotSummary)
{
    p->hitboxSize.x = shotSummary.hitboxDiagonalExtent() / 2.0f;
    p->hitboxSize.y = p->hitboxSize.x;
    p->hitboxSize.z = 5.0f;

    p->grazeSize.x = shotSummary.grazeDiagonalExtent() / 2.0f;
    p->grazeSize.y = p->grazeSize.x;
    p->grazeSize.z = 5.0f;

    p->grabItemSize.x = 12.0f;
    p->grabItemSize.y = 12.0f;
    p->grabItemSize.z = 5.0f;
}

void SyncLegacyPlayerMovementBridge(Player *p)
{
    p->playerDirection = static_cast<PlayerDirection>(p->Th07MovementDirectionState());
    p->orbState = static_cast<OrbState>(p->Th07OptionState());
    p->isFocus = p->Th07FocusHeld() != 0;
}

void ApplyTh07PlayerMovementCore(Player *p, u16 inputMask)
{
    const PlayerMovementVector movement =
        BuildTh07PlayerMovementVector(inputMask, BuildTh07PlayerMovementSpeedInput(Th07ShotTypeFromGameManager()));

    p->Th07MovementDirectionState() = movement.directionState;
    p->Th07FocusHeld() = movement.focusHeld ? kPlayerFocusHeld : kPlayerFocusNotHeld;
    SyncLegacyPlayerMovementBridge(p);

    ApplyTh07PlayerHorizontalAnimation(p, movement.horizontalSpeed);

    p->Th07MovementHorizontalSpeed() = movement.horizontalSpeed;
    p->Th07MovementVerticalSpeed() = movement.verticalSpeed;
    p->previousHorizontalSpeed = movement.horizontalSpeed;
    p->previousVerticalSpeed = movement.verticalSpeed;

    p->movementDelta.x =
        movement.horizontalSpeed * p->horizontalMovementSpeedMultiplierDuringBomb *
        g_Supervisor.effectiveFramerateMultiplier;
    p->movementDelta.y =
        movement.verticalSpeed * p->verticalMovementSpeedMultiplierDuringBomb *
        g_Supervisor.effectiveFramerateMultiplier;

    p->positionCenter.x += p->movementDelta.x;
    p->positionCenter.y += p->movementDelta.y;

    if (p->positionCenter.x < g_GameManager.playerMovementAreaTopLeftPos.x)
    {
        p->positionCenter.x = g_GameManager.playerMovementAreaTopLeftPos.x;
    }
    else if (g_GameManager.playerMovementAreaTopLeftPos.x + g_GameManager.playerMovementAreaSize.x <
             p->positionCenter.x)
    {
        p->positionCenter.x =
            g_GameManager.playerMovementAreaTopLeftPos.x + g_GameManager.playerMovementAreaSize.x;
    }

    if (p->positionCenter.y < g_GameManager.playerMovementAreaTopLeftPos.y)
    {
        p->positionCenter.y = g_GameManager.playerMovementAreaTopLeftPos.y;
    }
    else if (g_GameManager.playerMovementAreaTopLeftPos.y + g_GameManager.playerMovementAreaSize.y <
             p->positionCenter.y)
    {
        p->positionCenter.y =
            g_GameManager.playerMovementAreaTopLeftPos.y + g_GameManager.playerMovementAreaSize.y;
    }

    ApplyTh07PlayerCollisionBoxes(p);
    p->orbsPosition[0] = p->positionCenter;
    p->orbsPosition[1] = p->positionCenter;
}

} // namespace

DIFFABLE_STATIC_ARRAY_ASSIGN(CharacterData, 6, g_CharData) = {
    /* ReimuA  */ {Th07OrthogonalSpeed(Th07ShotType::ReimuA), Th07FocusedOrthogonalSpeed(Th07ShotType::ReimuA),
                   Th07DiagonalSpeed(Th07ShotType::ReimuA), Th07FocusedDiagonalSpeed(Th07ShotType::ReimuA),
                   Player::FireBulletReimuA, Player::FireBulletReimuA},
    /* ReimuB  */ {Th07OrthogonalSpeed(Th07ShotType::ReimuB), Th07FocusedOrthogonalSpeed(Th07ShotType::ReimuB),
                   Th07DiagonalSpeed(Th07ShotType::ReimuB), Th07FocusedDiagonalSpeed(Th07ShotType::ReimuB),
                   Player::FireBulletReimuB, Player::FireBulletReimuB},
    /* MarisaA */ {Th07OrthogonalSpeed(Th07ShotType::MarisaA), Th07FocusedOrthogonalSpeed(Th07ShotType::MarisaA),
                   Th07DiagonalSpeed(Th07ShotType::MarisaA), Th07FocusedDiagonalSpeed(Th07ShotType::MarisaA),
                   Player::FireBulletMarisaA, Player::FireBulletMarisaA},
    /* MarisaB */ {Th07OrthogonalSpeed(Th07ShotType::MarisaB), Th07FocusedOrthogonalSpeed(Th07ShotType::MarisaB),
                   Th07DiagonalSpeed(Th07ShotType::MarisaB), Th07FocusedDiagonalSpeed(Th07ShotType::MarisaB),
                   Player::FireBulletMarisaB, Player::FireBulletMarisaB},
    /* SakuyaA */ {Th07OrthogonalSpeed(Th07ShotType::SakuyaA), Th07FocusedOrthogonalSpeed(Th07ShotType::SakuyaA),
                   Th07DiagonalSpeed(Th07ShotType::SakuyaA), Th07FocusedDiagonalSpeed(Th07ShotType::SakuyaA),
                   Player::FireBulletSakuyaA, Player::FireBulletSakuyaA},
    /* SakuyaB */ {Th07OrthogonalSpeed(Th07ShotType::SakuyaB), Th07FocusedOrthogonalSpeed(Th07ShotType::SakuyaB),
                   Th07DiagonalSpeed(Th07ShotType::SakuyaB), Th07FocusedDiagonalSpeed(Th07ShotType::SakuyaB),
                   Player::FireBulletSakuyaB, Player::FireBulletSakuyaB},
};

Player::Player()
{
}

ZunResult Player::RegisterChain(u8 unk)
{
    Player *p = &g_Player;
    memset(p, 0, sizeof(Player));

    p->invulnerabilityTimer.InitializeForPopup();
    p->unk_9e1 = unk;
    p->chainCalc = g_Chain.CreateElem((ChainCallback)Player::OnUpdate);
    p->chainDraw1 = g_Chain.CreateElem((ChainCallback)Player::OnDrawHighPrio);
    p->chainDraw2 = g_Chain.CreateElem((ChainCallback)Player::OnDrawLowPrio);
    p->chainCalc->arg = p;
    p->chainDraw1->arg = p;
    p->chainDraw2->arg = p;
    p->chainCalc->addedCallback = (ChainAddedCallback)Player::AddedCallback;
    p->chainCalc->deletedCallback = (ChainDeletedCallback)Player::DeletedCallback;
    if (g_Chain.AddToCalcChain(p->chainCalc, TH_CHAIN_PRIO_CALC_PLAYER))
    {
        return ZUN_ERROR;
    }
    g_Chain.AddToDrawChain(p->chainDraw1, TH_CHAIN_PRIO_DRAW_LOW_PRIO_PLAYER);
    g_Chain.AddToDrawChain(p->chainDraw2, TH_CHAIN_PRIO_DRAW_HIGH_PRIO_PLAYER);
    return ZUN_SUCCESS;
}

void Player::CutChain()
{
    g_Chain.Cut(g_Player.chainCalc);
    g_Player.chainCalc = NULL;
    g_Chain.Cut(g_Player.chainDraw1);
    g_Player.chainDraw1 = NULL;
    g_Chain.Cut(g_Player.chainDraw2);
    g_Player.chainDraw2 = NULL;
    return;
}

ZunResult TH07_PLAYER_FASTCALL Player::AddedCallback(Player *p)
{
    PlayerBullet *curBullet;
    i32 idx;
    const Th07ShotType shotType = Th07ShotTypeFromGameManager();
    const Th07PlayerShotSummary &shotSummary = GetTh07UnfocusedPlayerShotSummary(shotType);

    switch (g_GameManager.character)
    {
    case CHARA_REIMU:
        // This is likely an inline function from g_Supervisor returning an i32.
        if ((i32)(g_Supervisor.curState != SUPERVISOR_STATE_GAMEMANAGER_REINIT) &&
            g_AnmManager->LoadAnm(ANM_FILE_PLAYER, "data/player00.anm", ANM_OFFSET_PLAYER) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, ANM_SCRIPT_PLAYER_IDLE);
        break;
    case CHARA_MARISA:
        if ((i32)(g_Supervisor.curState != SUPERVISOR_STATE_GAMEMANAGER_REINIT) &&
            g_AnmManager->LoadAnm(ANM_FILE_PLAYER, "data/player01.anm", ANM_OFFSET_PLAYER) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, ANM_SCRIPT_PLAYER_IDLE);
        break;
    case CHARA_SAKUYA:
        if ((i32)(g_Supervisor.curState != SUPERVISOR_STATE_GAMEMANAGER_REINIT) &&
            g_AnmManager->LoadAnm(ANM_FILE_PLAYER, "data/player02.anm", ANM_OFFSET_PLAYER) != ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
        g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, ANM_SCRIPT_PLAYER_IDLE);
        break;
    }
    p->positionCenter.x = g_GameManager.arcadeRegionSize.x / 2.0f;
    p->positionCenter.y = g_GameManager.arcadeRegionSize.y - 64.0f;
    p->positionCenter.z = 0.49;
    p->orbsPosition[0].z = 0.49;
    p->orbsPosition[1].z = 0.49;
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(p->bombRegionSizes); idx++)
    {
        p->bombRegionSizes[idx].x = 0.0;
    }
    ApplyTh07PlayerCollisionSizes(p, shotSummary);
    memcpy(&p->characterData, &g_CharData[g_GameManager.CharacterShotType()], sizeof(CharacterData));
    p->fireBulletCallback = p->characterData.fireBulletCallback;
    p->fireBulletFocusCallback = p->characterData.fireBulletFocusCallback;
    p->playerState = PLAYER_STATE_SPAWNING;
    p->invulnerabilityTimer.SetCurrent(120);
    g_AnmManager->SetAndExecuteScriptIdx(&p->orbsSprite[0], ANM_SCRIPT_PLAYER_ORB_LEFT);
    g_AnmManager->SetAndExecuteScriptIdx(&p->orbsSprite[1], ANM_SCRIPT_PLAYER_ORB_RIGHT);
    for (curBullet = &p->bullets[0], idx = 0; idx < ARRAY_SIZE_SIGNED(p->bullets); idx++, curBullet++)
    {
        curBullet->bulletState = 0;
    }
    p->fireBulletTimer.SetCurrent(-1);
    p->bombInfo.calc = g_BombData[g_GameManager.CharacterShotType()].calc;
    p->bombInfo.draw = g_BombData[g_GameManager.CharacterShotType()].draw;
    p->bombInfo.isInUse = 0;
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(p->laserTimer); idx++)
    {
        p->laserTimer[idx].InitializeForPopup();
    }
    p->verticalMovementSpeedMultiplierDuringBomb = 1.0;
    p->horizontalMovementSpeedMultiplierDuringBomb = 1.0;
    ApplyTh07InitialPlayerModeOwnerState(p);
    p->Th07OptionInterpolationTimer().InitializeForPopup();
    p->Th07MovementDirectionState() = kPlayerDirectionStateNone;
    p->Th07MovementHorizontalSpeed() = 0.0f;
    p->Th07MovementVerticalSpeed() = 0.0f;
    SyncLegacyPlayerMovementBridge(p);
    p->respawnTimer = 8;
    return ZUN_SUCCESS;
}

ZunResult TH07_PLAYER_FASTCALL Player::DeletedCallback(Player *p)
{
    if ((i32)(g_Supervisor.curState != SUPERVISOR_STATE_GAMEMANAGER_REINIT))
    {
        g_AnmManager->ReleaseAnm(ANM_FILE_PLAYER);
    }
    return ZUN_SUCCESS;
}

ZunResult TH07_PLAYER_FASTCALL Player::OptionShotEffectCallback(Player *p, void *shot,
                                                                D3DXVECTOR3 *impactPos)
{
    *reinterpret_cast<char *>(reinterpret_cast<u8 *>(p) + kPlayerOptionShotEffectCounterOffset) =
        *reinterpret_cast<char *>(reinterpret_cast<u8 *>(p) + kPlayerOptionShotEffectCounterOffset) + '\x01';

    if (static_cast<i32>(
            *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionShotEffectCounterOffset)) %
            (kPlayerOptionShotEffectCounterMask + 1) ==
        0)
    {
        D3DXVECTOR3 spawnPosition;
        spawnPosition.x = impactPos->x;
        spawnPosition.y = impactPos->y;
        spawnPosition.z = impactPos->z;
        spawnPosition.x =
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(shot) + kPlayerOptionShotEffectSourceXOffset);
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
        FUN_0041c1c0(reinterpret_cast<EffectManager *>(kPlayerOptionShotEffectManagerAddress),
                     kPlayerOptionShotEffectSpawnId, &spawnPosition, kPlayerOptionShotEffectSpawnCount,
                     kPlayerOptionShotEffectSpawnColor);
#else
        g_EffectManager.SpawnParticles(kPlayerOptionShotEffectSpawnId, &spawnPosition,
                                       kPlayerOptionShotEffectSpawnCount,
                                       kPlayerOptionShotEffectSpawnColor);
#endif
    }
    return ZUN_SUCCESS;
}

#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC push_options
#pragma GCC optimize("Og")
#endif
void TH07_PLAYER_FASTCALL Player::UpdateMode1Invulnerable(Player *p)
{
    u8 *base = reinterpret_cast<u8 *>(p);
    u8 *commonEffect = base + kPlayerBombCommonEffectXOffset;

    *reinterpret_cast<u32 *>(base + kPlayerModeSoundTimerOffset) = kPlayerMode1SoundTimerValue;

    const float fade =
        g_Th07PlayerModeOneFrame -
        (static_cast<float>(*reinterpret_cast<i32 *>(commonEffect + 8)) +
         *reinterpret_cast<float *>(commonEffect + 4)) /
            g_Th07PlayerMode1FadeFramesFloat;
    *reinterpret_cast<float *>(base + kTh07AnmVmScaleYOffset) =
        g_Th07PlayerMode1ScaleYMultiplier * fade + g_Th07PlayerModeOneFrame;
    *reinterpret_cast<float *>(base + kTh07AnmVmScaleXOffset) =
        g_Th07PlayerModeOneFrame - g_Th07PlayerModeOneFrame * fade;
    *reinterpret_cast<u32 *>(base + kTh07AnmVmRenderFlagsOffset) |= kPlayerMode1BlendFlag;
    *reinterpret_cast<u32 *>(base + kPlayerBombVerticalSpeedMultiplierOffset) =
        kPlayerMode1SpeedMultiplierBits;
    *reinterpret_cast<u32 *>(base + kPlayerBombHorizontalSpeedMultiplierOffset) =
        kPlayerMode1SpeedMultiplierBits;

    const i32 duration = *reinterpret_cast<i32 *>(commonEffect + 8);
    *reinterpret_cast<u32 *>(base + kTh07AnmVmColorOffset) =
        (static_cast<u32>((duration * g_Th07PlayerMode1AlphaScale) / g_Th07PlayerMode1FadeFrames) << 24) |
        kPlayerMode1WhiteRgbBits;
    *reinterpret_cast<u32 *>(base + kPlayerCurrentPowerOffset) = 0;

    if (g_Th07PlayerMode1FadeFrames <= *reinterpret_cast<i32 *>(commonEffect + 8))
    {
        *reinterpret_cast<u8 *>(base + kPlayerModeStateOffset) = kPlayerMode1CompleteModeState;
        *reinterpret_cast<u32 *>(base + kTh07AnmVmScaleYOffset) = kTh07TimerOneFrameBits;
        *reinterpret_cast<u32 *>(base + kTh07AnmVmScaleXOffset) = kTh07TimerOneFrameBits;
        *reinterpret_cast<u32 *>(base + kTh07AnmVmColorOffset) = kPlayerMode1WhiteArgbBits;
        *reinterpret_cast<u32 *>(base + kTh07AnmVmRenderFlagsOffset) =
            *reinterpret_cast<u32 *>(base + kTh07AnmVmRenderFlagsOffset) &
            kPlayerMode1BlendFlagClearMask;
        *reinterpret_cast<i32 *>(commonEffect + 8) = kPlayerMode1CompleteCommonEffectDuration;
        *reinterpret_cast<u32 *>(commonEffect + 4) = kTh07TimerZeroBits;
        *reinterpret_cast<u32 *>(commonEffect) = kTh07TimerPreviousSentinelBits;
        *reinterpret_cast<i32 *>(base + kPlayerCurrentPowerOffset) =
            g_Th07PlayerModeConfigOwner->currentPowerCap;
    }
}
#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC pop_options
#endif

void TH07_PLAYER_FASTCALL Player::ResetOptionFields(Player *p)
{
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetLeftXOffset) =
        kPlayerOptionResetSentinelBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetLeftYOffset) =
        kPlayerOptionResetSentinelBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetLeftZOffset) =
        kPlayerOptionResetZeroBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetRightXOffset) =
        kPlayerOptionResetSentinelBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetRightYOffset) =
        kPlayerOptionResetSentinelBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetRightZOffset) =
        kPlayerOptionResetZeroBits;
    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(p) + kPlayerOptionResetAuxOffset) =
        kPlayerOptionResetZeroBits;

    if (p->positionCenter.y < kPlayerOptionResetTopYThreshold)
    {
        if (g_Th07PlayerOptionResetPendingInterruptState == kPlayerOptionResetStateEntering)
        {
            g_Th07PlayerOptionResetPendingInterruptMirror = kPlayerOptionResetStateInterrupted;
            g_Th07PlayerOptionResetPendingInterruptState = kPlayerOptionResetStateInterrupted;
        }
    }
    else if (g_Th07PlayerOptionResetPendingInterruptState == kPlayerOptionResetStateEntering ||
             kPlayerOptionResetRightXThreshold <= p->positionCenter.x)
    {
        if (g_Th07PlayerOptionResetPendingInterruptState == kPlayerOptionResetStateEntering &&
            kPlayerOptionResetRightXThreshold < p->positionCenter.x)
        {
            g_Th07PlayerOptionResetPendingInterruptMirror = kPlayerOptionResetStateInterrupted;
            g_Th07PlayerOptionResetPendingInterruptState = kPlayerOptionResetStateInterrupted;
        }
    }
    else
    {
        g_Th07PlayerOptionResetPendingInterruptMirror = kPlayerOptionResetStateEntering;
        g_Th07PlayerOptionResetPendingInterruptState = kPlayerOptionResetStateEntering;
    }
}

#pragma var_order(idx, scaleFactor1, scaleFactor2, lastEnemyHit)
ChainCallbackResult TH07_PLAYER_FASTCALL Player::OnUpdate(Player *p)
{
    f32 scaleFactor1, scaleFactor2;
    i32 idx;
    D3DXVECTOR3 lastEnemyHit;

    if (g_GameManager.isTimeStopped)
    {
        return CHAIN_CALLBACK_RESULT_CONTINUE;
    }
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(p->bombRegionSizes); idx++)
    {
        p->bombRegionSizes[idx].x = 0.0;
    }
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(p->bombProjectiles); idx++)
    {
        p->bombProjectiles[idx].sizeX = 0.0;
    }
    if (p->bombInfo.isInUse)
    {
        p->bombInfo.calc(p);
    }
    else if (!g_Gui.HasCurrentMsgIdx() && p->respawnTimer != 0 && 0 < g_GameManager.bombsRemaining &&
             WAS_PRESSED(TH_BUTTON_BOMB) && p->bombInfo.calc != NULL)
    {
        g_GameManager.bombsUsed++;
        g_GameManager.bombsRemaining--;
        g_Gui.flags.flag1 = 2;
        p->bombInfo.isInUse = 1;
        p->bombInfo.timer.SetCurrent(0);
        p->bombInfo.duration = 999;
        p->bombInfo.calc(p);
        g_EnemyManager.spellcardInfo.isCapturing = false;
        g_GameManager.DecreaseSubrank(200);
        g_EnemyManager.spellcardInfo.usedBomb = g_EnemyManager.spellcardInfo.isActive;
    }
    if (p->playerState == PLAYER_STATE_DEAD)
    {
        if (p->respawnTimer != 0)
        {
            p->respawnTimer--;
            if (p->respawnTimer == 0)
            {
                g_GameManager.powerItemCountForScore = 0;
                if (g_GameManager.livesRemaining > 0)
                {
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_BIG, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_SMALL, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_SMALL, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_SMALL, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_SMALL, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_POWER_SMALL, 2);
                    if (g_GameManager.currentPower <= 16)
                    {
                        g_GameManager.currentPower = 0;
                    }
                    else
                    {
                        g_GameManager.currentPower -= 16;
                    }
                    g_Gui.flags.flag2 = 2;
                }
                else
                {
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_FULL_POWER, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_FULL_POWER, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_FULL_POWER, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_FULL_POWER, 2);
                    g_ItemManager.SpawnItem(&p->positionCenter, ITEM_FULL_POWER, 2);
                    g_GameManager.currentPower = 0;
                    g_Gui.flags.flag2 = 2;
                    g_GameManager.extraLives = 255;
                }
                g_GameManager.DecreaseSubrank(1600);
            }
        }
        else
        {
            scaleFactor1 = p->invulnerabilityTimer.AsFramesFloat() / 30.0f;
            p->playerSprite.scaleY = 3.0f * scaleFactor1 + 1.0f;
            p->playerSprite.scaleX = 1.0f - 1.0f * scaleFactor1;
            p->playerSprite.color =
                COLOR_SET_ALPHA(COLOR_WHITE, (u32)(255.0f - p->invulnerabilityTimer.AsFramesFloat() * 255.0f / 30.0f));
            p->playerSprite.flags.blendMode = AnmVmBlendMode_One;
            p->previousHorizontalSpeed = 0.0f;
            p->previousVerticalSpeed = 0.0f;
            if (p->invulnerabilityTimer.AsFrames() >= 30)
            {
                p->playerState = PLAYER_STATE_SPAWNING;
                p->positionCenter.x = g_GameManager.arcadeRegionSize.x / 2.0f;
                p->positionCenter.y = g_GameManager.arcadeRegionSize.y - 64.0f;
                p->positionCenter.z = 0.2;
                p->invulnerabilityTimer.SetCurrent(0);
                p->playerSprite.scaleX = 3.0;
                p->playerSprite.scaleY = 3.0;
                g_AnmManager->SetAndExecuteScriptIdx(&p->playerSprite, ANM_SCRIPT_PLAYER_IDLE);
                if (g_GameManager.livesRemaining <= 0)
                {
                    g_GameManager.isInRetryMenu = 1;
                }
                else
                {
                    g_GameManager.livesRemaining--;
                    g_Gui.flags.flag0 = 2;
                    if (g_GameManager.difficulty < 4 && g_GameManager.isInPracticeMode == 0)
                    {
                        g_GameManager.bombsRemaining = g_Supervisor.defaultConfig.bombCount;
                    }
                    else
                    {
                        g_GameManager.bombsRemaining = 3;
                    }
                    g_Gui.flags.flag1 = 2;
                    goto spawning;
                }
            }
        }
    }
    else if (p->playerState == PLAYER_STATE_SPAWNING)
    {
    spawning:
        p->bulletGracePeriod = 90;
        scaleFactor2 = 1.0f - p->invulnerabilityTimer.AsFramesFloat() / 30.0f;
        p->playerSprite.scaleY = 2.0f * scaleFactor2 + 1.0f;
        p->playerSprite.scaleX = 1.0f - 1.0f * scaleFactor2;
        p->playerSprite.flags.blendMode = AnmVmBlendMode_One;
        p->verticalMovementSpeedMultiplierDuringBomb = 1.0;
        p->horizontalMovementSpeedMultiplierDuringBomb = 1.0;
        p->playerSprite.color = COLOR_SET_ALPHA(COLOR_WHITE, p->invulnerabilityTimer.AsFrames() * 255 / 30);
        p->respawnTimer = 0;
        if (30 <= p->invulnerabilityTimer.AsFrames())
        {
            p->playerState = PLAYER_STATE_INVULNERABLE;
            p->playerSprite.scaleX = 1.0;
            p->playerSprite.scaleY = 1.0;
            p->playerSprite.color = COLOR_WHITE;
            p->playerSprite.flags.blendMode = AnmVmBlendMode_InvSrcAlpha;
            p->invulnerabilityTimer.SetCurrent(240);
            p->respawnTimer = 6;
        }
    }
    if (p->bulletGracePeriod != 0)
    {
        p->bulletGracePeriod--;
        g_BulletManager.RemoveAllBullets(0);
    }
    if (p->playerState == PLAYER_STATE_INVULNERABLE)
    {
        p->invulnerabilityTimer.Decrement(1);
        if (p->invulnerabilityTimer.AsFrames() <= 0)
        {
            p->playerState = PLAYER_STATE_ALIVE;
            p->invulnerabilityTimer.SetCurrent(0);
            p->playerSprite.flags.colorOp = AnmVmColorOp_Modulate;
            p->playerSprite.color = COLOR_WHITE;
        }
        else if (p->invulnerabilityTimer.AsFrames() % 8 < 2)
        {
            p->playerSprite.flags.colorOp = AnmVmColorOp_Add;
            p->playerSprite.color = 0xff404040;
        }
        else
        {
            p->playerSprite.flags.colorOp = AnmVmColorOp_Modulate;
            p->playerSprite.color = COLOR_WHITE;
        }
    }
    else
    {
        p->invulnerabilityTimer.Tick();
    }
    if (p->playerState != PLAYER_STATE_DEAD && p->playerState != PLAYER_STATE_SPAWNING)
    {
        p->HandlePlayerInputs();
    }
    g_AnmManager->ExecuteScript(&p->playerSprite);
    Player::UpdatePlayerBullets(p);
    if (p->orbState != ORB_HIDDEN)
    {
        g_AnmManager->ExecuteScript(&p->orbsSprite[0]);
        g_AnmManager->ExecuteScript(&p->orbsSprite[1]);
    }
    lastEnemyHit.x = -999.0;
    lastEnemyHit.y = -999.0;
    lastEnemyHit.z = 0.0;
    p->positionOfLastEnemyHit = lastEnemyHit;
    Player::UpdateFireBulletsTimer(p);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#pragma var_order(bullet, idx, enemyBottomRight, bulletBottomRight, enemyTopLeft, damage, bulletTopLeft)
i32 TH07_PLAYER_THISCALL Player::CalcDamageToEnemy(D3DXVECTOR3 *enemyPos, D3DXVECTOR3 *enemyHitboxSize,
                                                   ZunBool *hitWithLazerDuringBomb)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *base = reinterpret_cast<u8 *>(this);
    i32 damage;
    f32 enemyLeft;
    f32 enemyTop;
    f32 enemyRight;
    f32 enemyBottom;
    u8 *shotSlot;
    i32 idx;

    damage = 0;
    if (*reinterpret_cast<i32 *>(base + kPlayerBombCommonEffectDurationOffset) ==
        *reinterpret_cast<i32 *>(base + kPlayerBombCommonEffectXOffset))
    {
        return 0;
    }

    enemyLeft = enemyPos->x - enemyHitboxSize->x * *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress);
    enemyTop = enemyPos->y - enemyHitboxSize->y * *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress);
    enemyRight = enemyHitboxSize->x * *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) + enemyPos->x;
    enemyBottom = enemyHitboxSize->y * *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) + enemyPos->y;
    shotSlot = base + kPlayerShotSlotArrayOffset;
    if (hitWithLazerDuringBomb != nullptr)
    {
        *hitWithLazerDuringBomb = 0;
    }

    for (idx = 0; idx < static_cast<i32>(kPlayerShotSlotCount); idx = idx + 1)
    {
        if ((*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) != 0) &&
            ((*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) ==
              static_cast<i16>(kPlayerShotSlotActiveState)) ||
             (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) == 3)))
        {
            if ((*reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset) -
                     *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotSizeYOffset) *
                         *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) <=
                 enemyBottom) &&
                ((*reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset) -
                      *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotSizeXOffset) *
                          *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) <=
                  enemyRight) &&
                 (enemyTop <=
                  *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotSizeYOffset) *
                          *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) +
                      *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset))) &&
                (enemyLeft <=
                 *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotSizeXOffset) *
                         *reinterpret_cast<f32 *>(kPlayerDamageHalfConstantAddress) +
                     *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset)))
            {
                if ((*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) ==
                     kPlayerUpdateLaserTypeFirst) ||
                    (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) ==
                     kPlayerUpdateLaserTypeSecond))
                {
                    u32 parity =
                        *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotTimerCurrentOffset) &
                        kPlayerDamageLaserTimerParityMask;
                    if (static_cast<i32>(parity) < 0)
                    {
                        parity = ((parity - 1) | 0xfffffffe) + 1;
                    }
                    if (parity != 0)
                    {
                        shotSlot += kPlayerShotSlotStride;
                        continue;
                    }
                }

                if ((*reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotCollisionCallbackOffset) == 0) ||
                    ((*reinterpret_cast<Th07ShtCollisionCallback *>(
                          shotSlot + kPlayerShotSlotCollisionCallbackOffset))(this, shotSlot, enemyPos) == 0))
                {
                    if (*reinterpret_cast<i32 *>(base + kPlayerDamageBombActiveFlagOffset) == 0)
                    {
                        damage += *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotDamageOffset);
                    }
                    else if (static_cast<i32>(
                                 *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotDamageOffset)) /
                                 3 ==
                             0)
                    {
                        damage += 1;
                    }
                    else
                    {
                        damage += static_cast<i32>(
                                      *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotDamageOffset)) /
                                  3;
                    }

                    if ((*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) !=
                         kPlayerUpdateLaserTypeFirst) &&
                        (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) !=
                         kPlayerUpdateLaserTypeSecond))
                    {
                        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) ==
                            static_cast<i16>(kPlayerShotSlotActiveState))
                        {
                            void *anmManager =
                                *reinterpret_cast<void **>(kPlayerShotDrawAnmManagerPointerAddress);
                            i32 scriptIdx = *reinterpret_cast<i16 *>(
                                                shotSlot + kPlayerShotSlotScriptIndexOffset) +
                                            kPlayerDamageCollisionScriptDelta;
                            *reinterpret_cast<u16 *>(shotSlot + kPlayerShotSlotScriptIndexOffset) =
                                static_cast<u16>(scriptIdx);
                            void *script = *reinterpret_cast<void **>(
                                reinterpret_cast<u8 *>(anmManager) +
                                kPlayerDamageCollisionScriptTableOffset + scriptIdx * 4);
                            FUN_0044ea20(anmManager, shotSlot, script);
                            FUN_0041c1c0(reinterpret_cast<EffectManager *>(kTh07EffectManagerAddress),
                                         kPlayerDamageShotEffectId,
                                         reinterpret_cast<D3DXVECTOR3 *>(
                                             shotSlot + kPlayerShotSlotPositionXOffset),
                                         kPlayerDamageEffectSpawnCount, kPlayerDamageEffectColor);
                            *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotPositionZOffset) =
                                kPlayerDamageCollisionEffectZBits;
                        }
                        *reinterpret_cast<u16 *>(shotSlot + kPlayerShotSlotStateOffset) =
                            static_cast<u16>(kPlayerShotSlotCollidedState);
                        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) != 3)
                        {
                            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityXOffset) =
                                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityXOffset) /
                                *reinterpret_cast<f32 *>(kPlayerDamageShotVelocityDivisorAddress);
                            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityYOffset) =
                                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityYOffset) /
                                *reinterpret_cast<f32 *>(kPlayerDamageShotVelocityDivisorAddress);
                        }
                    }
                }
            }
        }
        shotSlot += kPlayerShotSlotStride;
    }

    for (idx = 0; idx < static_cast<i32>(kPlayerDamageBombRegionCount); idx = idx + 1)
    {
        const u32 regionOffset = idx * kPlayerDamageBombRegionStride;
        f32 *regionSize =
            reinterpret_cast<f32 *>(base + kPlayerDamageBombRegionSizeOffset + regionOffset);
        f32 *regionPosition =
            reinterpret_cast<f32 *>(base + kPlayerDamageBombRegionPositionOffset + regionOffset);
        if (regionSize[0] <= *reinterpret_cast<f32 *>(kPlayerDamageZeroConstantAddress))
        {
            continue;
        }

        f32 halfScale = *reinterpret_cast<f32 *>(kPlayerDamageOneFrameConstantAddress) /
                        *reinterpret_cast<f32 *>(kPlayerDamageTwoConstantAddress);
        f32 regionLeft = regionPosition[0] - halfScale * regionSize[0];
        f32 regionTop = regionPosition[1] - halfScale * regionSize[1];
        f32 regionRight = halfScale * regionSize[0] + regionPosition[0];
        f32 regionBottom = halfScale * regionSize[1] + regionPosition[1];
        if ((regionLeft <= enemyRight) && (enemyLeft <= regionRight) &&
            (regionTop <= enemyBottom) && (enemyTop <= regionBottom))
        {
            damage += *reinterpret_cast<i32 *>(
                base + kPlayerDamageBombRegionDamageOffset + regionOffset);
            *reinterpret_cast<i32 *>(base + kPlayerDamageBombRegionAccumulatedDamageOffset +
                                     regionOffset) =
                *reinterpret_cast<i32 *>(base + kPlayerDamageBombRegionAccumulatedDamageOffset +
                                         regionOffset) +
                *reinterpret_cast<i32 *>(base + kPlayerDamageBombRegionDamageOffset + regionOffset);
            *reinterpret_cast<u8 *>(base + kPlayerOptionShotEffectCounterOffset) =
                *reinterpret_cast<u8 *>(base + kPlayerOptionShotEffectCounterOffset) + 1;
            if ((*reinterpret_cast<u8 *>(base + kPlayerOptionShotEffectCounterOffset) &
                 kPlayerDamageBombEffectCadenceMask) == 0)
            {
                if (idx < static_cast<i32>(kPlayerDamageBombRegionLowEffectCount))
                {
                    FUN_0041c1c0(reinterpret_cast<EffectManager *>(kTh07EffectManagerAddress),
                                 kPlayerDamageBombEffectLowId, enemyPos,
                                 kPlayerDamageEffectSpawnCount, kPlayerDamageEffectColor);
                }
                else
                {
                    FUN_0041c1c0(reinterpret_cast<EffectManager *>(kTh07EffectManagerAddress),
                                 kPlayerDamageBombEffectHighId, enemyPos,
                                 kPlayerDamageEffectSpawnCount, kPlayerDamageEffectColor);
                }
            }
            if ((*reinterpret_cast<i32 *>(base + kPlayerDamageBombActiveFlagOffset) != 0) &&
                (hitWithLazerDuringBomb != nullptr))
            {
                *hitWithLazerDuringBomb = 1;
            }
        }
    }
    return damage;
#else
    ZunVec3 bulletTopLeft;
    i32 damage;
    ZunVec3 enemyTopLeft;
    i32 idx;
    PlayerBullet *bullet;

    ZunVec3 bulletBottomRight;
    ZunVec3 enemyBottomRight;

    damage = 0;

    ZunVec3::SetVecCorners(&enemyTopLeft, &enemyBottomRight, enemyPos, enemyHitboxSize);
    bullet = &this->bullets[0];
    if (hitWithLazerDuringBomb)
    {
        *hitWithLazerDuringBomb = false;
    }
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(this->bullets); idx++, bullet++)
    {
        if (bullet->bulletState == BULLET_STATE_UNUSED ||
            bullet->bulletState != BULLET_STATE_FIRED && bullet->bulletType != BULLET_TYPE_2)
        {
            continue;
        }

        ZunVec3::SetVecCorners(&bulletTopLeft, &bulletBottomRight, &bullet->position, &bullet->size);

        if (bulletTopLeft.y > enemyBottomRight.y || bulletTopLeft.x > enemyBottomRight.x ||
            bulletBottomRight.y < enemyTopLeft.y || bulletBottomRight.x < enemyTopLeft.x)
        {
            continue;
        }
        /* Bullet is hitting the enemy */
        if (!this->bombInfo.isInUse)
        {
            damage += bullet->damage;
        }
        else
        {
            damage += bullet->damage / 3 != 0 ? bullet->damage / 3 : 1;
        }

        if (bullet->bulletType == BULLET_TYPE_2)
        {
            bullet->damage = bullet->damage / 4;
            if (bullet->damage == 0)
            {
                bullet->damage = 1;
            }
            switch (bullet->sprite.anmFileIndex)
            {
            case ANM_SCRIPT_PLAYER_MARISA_A_ORB_BULLET_1:
                bullet->size.x = 32.0f;
                bullet->size.y = 32.0f;
                break;
            case ANM_SCRIPT_PLAYER_MARISA_A_ORB_BULLET_2:
                bullet->size.x = 42.0f;
                bullet->size.y = 42.0f;
                break;
            case ANM_SCRIPT_PLAYER_MARISA_A_ORB_BULLET_3:
                bullet->size.x = 48.0f;
                bullet->size.y = 48.0f;
                break;
            case ANM_SCRIPT_PLAYER_MARISA_A_ORB_BULLET_4:
                bullet->size.x = 48.0f;
                bullet->size.y = 48.0f;
            }
            if (bullet->unk_140.AsFrames() % 6 == 0)
            {
                g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_5, &bullet->position, 1, COLOR_WHITE);
            }
        }

        if (bullet->bulletType != BULLET_TYPE_LASER)
        {
            if (bullet->bulletState == BULLET_STATE_FIRED)
            {
                g_AnmManager->SetAndExecuteScriptIdx(&bullet->sprite, bullet->sprite.anmFileIndex + 0x20);
                g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_5, &bullet->position, 1, COLOR_WHITE);
                bullet->position.z = 0.1;
            }
            bullet->bulletState = BULLET_STATE_COLLIDED;
            bullet->velocity.x /= 8.0f;
            bullet->velocity.y /= 8.0f;
        }
        else
        {
            this->unk_9e4++;
            if (this->unk_9e4 % 8 == 0)
            {
                *bulletTopLeft.AsD3dXVec() = *enemyPos;
                bulletTopLeft.x = bullet->position.x;

                g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_5, bulletTopLeft.AsD3dXVec(), 1, COLOR_WHITE);
            }
        }
    }
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(this->bombRegionSizes); idx++)
    {
        if (this->bombRegionSizes[idx].x <= 0.0f)
        {
            continue;
        }

        *bulletTopLeft.AsD3dXVec() = this->bombRegionPositions[idx] - this->bombRegionSizes[idx] / 2.0f;
        *bulletBottomRight.AsD3dXVec() = this->bombRegionPositions[idx] + this->bombRegionSizes[idx] / 2.0f;
        if (bulletTopLeft.x > enemyBottomRight.x || bulletBottomRight.x < enemyTopLeft.x ||
            bulletTopLeft.y > enemyBottomRight.y || bulletBottomRight.y < enemyTopLeft.y)
        {
            continue;
        }
        damage += this->bombRegionDamages[idx];
        this->unk_838[idx] += this->bombRegionDamages[idx];
        this->unk_9e4++;
        if (this->unk_9e4 % 4 == 0)
        {
            g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_3, enemyPos, 1, COLOR_WHITE);
        }
        if (this->bombInfo.isInUse && hitWithLazerDuringBomb)
        {
            *hitWithLazerDuringBomb = true;
        }
    }
    return damage;
#endif
}

#pragma var_order(vector, idx, vecLength, bullet)
void TH07_PLAYER_FASTCALL Player::UpdatePlayerBullets(Player *player)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *base = reinterpret_cast<u8 *>(player);
    u8 *shotSlot;
    i32 idx;

    if (*reinterpret_cast<char *>(base + kPlayerOptionStateOffset) != '\x03' &&
        *reinterpret_cast<u32 *>(base + kPlayerUpdateTrackedShotEffectFocusedPointerOffset) != 0)
    {
        *reinterpret_cast<i16 *>(
            *reinterpret_cast<u8 **>(base + kPlayerUpdateTrackedShotEffectFocusedPointerOffset) +
            kPlayerShotSlotStateOffset) = 0;
        *reinterpret_cast<u32 *>(base + kPlayerUpdateTrackedShotEffectFocusedPointerOffset) = 0;
    }
    if (*reinterpret_cast<char *>(base + kPlayerOptionStateOffset) != '\x01')
    {
        if (*reinterpret_cast<u32 *>(base + kPlayerUpdateTrackedShotEffectPointerArrayOffset) != 0)
        {
            *reinterpret_cast<i16 *>(
                *reinterpret_cast<u8 **>(base + kPlayerUpdateTrackedShotEffectPointerArrayOffset) +
                kPlayerShotSlotPendingInterruptOffset) =
                kPlayerUpdateTrackedShotEffectPendingInterrupt;
            *reinterpret_cast<u32 *>(base + kPlayerUpdateTrackedShotEffectPointerArrayOffset) = 0;
        }
        if (*reinterpret_cast<u32 *>(
                base + kPlayerUpdateTrackedShotEffectPointerArrayOffset +
                kPlayerUpdateTrackedShotEffectPointerStride) != 0)
        {
            *reinterpret_cast<i16 *>(
                *reinterpret_cast<u8 **>(base + kPlayerUpdateTrackedShotEffectPointerArrayOffset +
                                         kPlayerUpdateTrackedShotEffectPointerStride) +
                kPlayerShotSlotPendingInterruptOffset) =
                kPlayerUpdateTrackedShotEffectPendingInterrupt;
            *reinterpret_cast<u32 *>(
                base + kPlayerUpdateTrackedShotEffectPointerArrayOffset +
                kPlayerUpdateTrackedShotEffectPointerStride) = 0;
        }
    }
    if (*reinterpret_cast<char *>(base + kPlayerModeStateOffset) == '\x02')
    {
        for (idx = 0; idx < static_cast<i32>(kPlayerUpdateTrackedShotEffectPointerCount); idx = idx + 1)
        {
            u8 *trackedPointerSlot = base + kPlayerUpdateTrackedShotEffectPointerArrayOffset +
                                     idx * kPlayerUpdateTrackedShotEffectPointerStride;
            if (*reinterpret_cast<u32 *>(trackedPointerSlot) != 0)
            {
                *reinterpret_cast<i16 *>(
                    *reinterpret_cast<u8 **>(trackedPointerSlot) + kPlayerShotSlotStateOffset) = 0;
                *reinterpret_cast<u32 *>(trackedPointerSlot) = 0;
            }
        }
    }
    for (idx = 0; idx < static_cast<i32>(kPlayerUpdateTrackedShotEffectPointerCount); idx = idx + 1)
    {
        u8 *trackedTimer =
            base + kPlayerUpdateTrackedShotEffectTimerArrayOffset +
            idx * kPlayerUpdateTrackedShotEffectPointerStride;
        u8 *trackedPointerSlot =
            base + kPlayerUpdateTrackedShotEffectPointerArrayOffset +
            idx * kPlayerUpdateTrackedShotEffectPointerStride;
        if (*reinterpret_cast<u32 *>(trackedPointerSlot) != 0)
        {
            if (0 < *reinterpret_cast<i32 *>(trackedTimer + kPlayerUpdateTrackedShotEffectCurrentOffset) &&
                *reinterpret_cast<i32 *>(trackedTimer + kPlayerUpdateTrackedShotEffectCurrentOffset) <
                    kPlayerUpdateTrackedShotEffectMaxDuration)
            {
                FUN_004394c7(trackedTimer, 1);
            }
            if (*reinterpret_cast<i32 *>(base + kPlayerUpdateTrackedShotEffectMode2GateOffset) < 0 &&
                kPlayerUpdateTrackedShotEffectClampDuration <
                    *reinterpret_cast<i32 *>(trackedTimer + kPlayerUpdateTrackedShotEffectCurrentOffset))
            {
                *reinterpret_cast<u32 *>(trackedTimer + 8) = kPlayerUpdateTrackedShotEffectClampDuration;
                *reinterpret_cast<u32 *>(trackedTimer + 4) = kPlayerUpdateTrackedShotEffectZeroBits;
                *reinterpret_cast<u32 *>(trackedTimer) =
                    kPlayerUpdateTrackedShotEffectPreviousSentinelBits;
            }
            if (*reinterpret_cast<i32 *>(trackedTimer + kPlayerUpdateTrackedShotEffectCurrentOffset) == 0)
            {
                *reinterpret_cast<u32 *>(trackedPointerSlot) = 0;
            }
        }
    }

    shotSlot = base + kPlayerShotSlotArrayOffset;
    for (idx = 0; idx < static_cast<i32>(kPlayerShotSlotCount); idx = idx + 1)
    {
        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) != 0)
        {
            Th07ShtUpdateCallback updateCallback =
                *reinterpret_cast<Th07ShtUpdateCallback *>(shotSlot + kPlayerShotSlotUpdateCallbackOffset);
            if (updateCallback == nullptr || (*updateCallback)(player, shotSlot) == 0)
            {
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset) =
                    *reinterpret_cast<f32 *>(kPlayerMovementTimerMultiplierGlobalAddress) *
                        *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityXOffset) +
                    *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset);
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset) =
                    *reinterpret_cast<f32 *>(kPlayerMovementTimerMultiplierGlobalAddress) *
                        *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotVelocityYOffset) +
                    *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset);
                if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) !=
                        kPlayerUpdateLaserTypeFirst &&
                    *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotTypeOffset) !=
                        kPlayerUpdateLaserTypeSecond &&
                    FUN_0042d6d8(reinterpret_cast<void *>(kPlayerUpdateBoundsOwnerAddress),
                                 *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotPositionXOffset),
                                 *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotPositionYOffset),
                                 *reinterpret_cast<u32 *>(
                                     *reinterpret_cast<u8 **>(shotSlot +
                                                              kPlayerShotSlotAnmSpritePointerOffset) +
                                     kPlayerUpdateLoadedSpriteHeightOffset),
                                 *reinterpret_cast<u32 *>(
                                     *reinterpret_cast<u8 **>(shotSlot +
                                                              kPlayerShotSlotAnmSpritePointerOffset) +
                                     kPlayerUpdateLoadedSpriteWidthOffset)) == 0)
                {
                    *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) = 0;
                }
                if (FUN_00450d60(
                        *reinterpret_cast<void **>(kPlayerUpdateAnmManagerPointerAddress),
                        shotSlot) != 0)
                {
                    *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) = 0;
                }
                *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotTimerPreviousOffset) =
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotTimerCurrentOffset);
                FUN_0043958d(reinterpret_cast<void *>(kPlayerUpdateTimerHelperOwnerAddress),
                             shotSlot + kPlayerShotSlotTimerCurrentOffset,
                             shotSlot + kPlayerShotSlotTimerSubframeOffset);
            }
            else
            {
                *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) = 0;
            }
        }
        shotSlot += kPlayerShotSlotStride;
    }
#else
    ZunVec2 vector;
    PlayerBullet *bullet;
    f32 vecLength;
    i32 idx;

    for (idx = 0; idx < ARRAY_SIZE_SIGNED(player->laserTimer); idx++)
    {
        if (player->laserTimer[idx].AsFrames() != 0)
        {
            player->laserTimer[idx].Decrement(1);
        }
    }
    bullet = &player->bullets[0];
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(player->bullets); idx++, bullet++)
    {
        if (bullet->bulletState == BULLET_STATE_UNUSED)
        {
            continue;
        }

        switch (bullet->bulletType)
        {
        case BULLET_TYPE_1:
            if (bullet->bulletState == BULLET_STATE_FIRED)
            {
                if (player->positionOfLastEnemyHit.x > -100.0f && bullet->unk_140.AsFrames() < 40 &&
                    bullet->unk_140.HasTicked())
                {
                    vector.x = player->positionOfLastEnemyHit.x - bullet->position.x;
                    vector.y = player->positionOfLastEnemyHit.y - bullet->position.y;

                    vecLength = vector.VectorLength() / (bullet->unk_134.y / 4.0f);
                    if (vecLength < 1.0f)
                    {
                        vecLength = 1.0f;
                    }

                    vector.x = vector.x / vecLength + bullet->velocity.x;
                    vector.y = vector.y / vecLength + bullet->velocity.y;

                    vecLength = vector.VectorLengthF64();

                    bullet->unk_134.y = ZUN_MIN(vecLength, 10.0f);

                    if (bullet->unk_134.y < 1.0f)
                    {
                        bullet->unk_134.y = 1.0f;
                    }

                    bullet->velocity.x = (vector.x * bullet->unk_134.y) / vecLength;
                    bullet->velocity.y = (vector.y * bullet->unk_134.y) / vecLength;
                }
                else
                {
                    if (bullet->unk_134.y < 10.0f)
                    {
                        bullet->unk_134.y += 0.33333333f;
                        vector.x = bullet->velocity.x;
                        vector.y = bullet->velocity.y;
                        vecLength = vector.VectorLengthF64();
                        bullet->velocity.x = vector.x * bullet->unk_134.y / vecLength;
                        bullet->velocity.y = vector.y * bullet->unk_134.y / vecLength;
                    }
                }
            }

            break;

        case BULLET_TYPE_2:
            if (bullet->bulletState == BULLET_STATE_FIRED)
            {
                bullet->velocity.y -= 0.3f;
            }
            break;
        case BULLET_TYPE_LASER:

            if (player->laserTimer[bullet->unk_152] == 70)
            {
                bullet->sprite.pendingInterrupt = 1;
            }
            else if (player->laserTimer[bullet->unk_152] == 1)
            {
                bullet->sprite.pendingInterrupt = 1;
            }

            bullet->position = player->orbsPosition[bullet->spawnPositionIdx - 1];

            bullet->position.x += bullet->sidewaysMotion;
            bullet->position.y /= 2.0f;
            bullet->position.z = 0.44f;

            bullet->sprite.scaleY = (bullet->position.y * 2) / 14.0f;

            bullet->size.y = bullet->position.y * 2;
            break;
        }

        bullet->sprite.pos.x = bullet->position[0] += bullet->velocity.x * g_Supervisor.effectiveFramerateMultiplier;

        bullet->sprite.pos.y = bullet->position[1] += bullet->velocity.y * g_Supervisor.effectiveFramerateMultiplier;

        bullet->sprite.pos.z = bullet->position.z;
        if (bullet->bulletType != BULLET_TYPE_LASER &&
            !g_GameManager.IsInBounds(bullet->position.x, bullet->position.y, bullet->sprite.sprite->widthPx,
                                      bullet->sprite.sprite->heightPx))
        {
            bullet->bulletState = BULLET_STATE_UNUSED;
        }

        if (g_AnmManager->ExecuteScript(&bullet->sprite))
        {
            bullet->bulletState = BULLET_STATE_UNUSED;
        }
        bullet->unk_140.Tick();
    }
#endif
}

ChainCallbackResult TH07_PLAYER_FASTCALL Player::OnDrawHighPrio(Player *p)
{
    Player::DrawBullets(p);
    if (p->bombInfo.isInUse != 0 && p->bombInfo.draw != NULL)
    {
        p->bombInfo.draw(p);
    }
    p->playerSprite.pos.x = g_GameManager.arcadeRegionTopLeftPos.x + p->positionCenter.x;
    p->playerSprite.pos.y = g_GameManager.arcadeRegionTopLeftPos.y + p->positionCenter.y;
    p->playerSprite.pos.z = 0.49;
    if (!g_GameManager.isInRetryMenu)
    {
        g_AnmManager->DrawNoRotation(&p->playerSprite);
        if (p->orbState != ORB_HIDDEN &&
            (p->playerState == PLAYER_STATE_ALIVE || p->playerState == PLAYER_STATE_INVULNERABLE))
        {
            p->orbsSprite[0].pos = p->orbsPosition[0];
            p->orbsSprite[1].pos = p->orbsPosition[1];
            p->orbsSprite[0].pos[0] += g_GameManager.arcadeRegionTopLeftPos.x;
            p->orbsSprite[0].pos[1] += g_GameManager.arcadeRegionTopLeftPos.y;
            p->orbsSprite[1].pos[0] += g_GameManager.arcadeRegionTopLeftPos.x;
            p->orbsSprite[1].pos[1] += g_GameManager.arcadeRegionTopLeftPos.y;
            p->orbsSprite[0].pos.z = 0.491;
            p->orbsSprite[1].pos.z = 0.491;
            g_AnmManager->Draw(&p->orbsSprite[0]);
            g_AnmManager->Draw(&p->orbsSprite[1]);
        }
    }
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult TH07_PLAYER_FASTCALL Player::OnDrawLowPrio(Player *p)
{
    Player::DrawBulletExplosions(p);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

#pragma var_order(verticalOrbOffset, horizontalOrbOffset, intermediateFloat)
ZunResult TH07_PLAYER_THISCALL Player::HandlePlayerInputs()
{
    float intermediateFloat;

    float horizontalOrbOffset;
    float verticalOrbOffset;
    ZunTimer &optionTimer = this->Th07OptionInterpolationTimer();

    ApplyTh07PlayerMovementCore(this, g_CurFrameInput);

    verticalOrbOffset = 0.0;
    horizontalOrbOffset = verticalOrbOffset;

    switch (this->Th07OptionState())
    {
    case kPlayerOptionStateHidden:
        optionTimer.InitializeForPopup();
        break;

    case kPlayerOptionStateUnfocused:
        horizontalOrbOffset = 24.0;
        optionTimer.InitializeForPopup();
        if (this->Th07FocusHeld() != 0)
        {
            this->Th07OptionState() = kPlayerOptionStateFocusing;
        }
        else
        {
            break;
        }

    CASE_ORB_FOCUSING:
    case kPlayerOptionStateFocusing:
        optionTimer.Tick();

        intermediateFloat = optionTimer.AsFramesFloat() / 8.0f;
        verticalOrbOffset = (1.0f - intermediateFloat) * 32.0f + -32.0f;
        intermediateFloat *= intermediateFloat;
        horizontalOrbOffset = -16.0f * intermediateFloat + 24.0f;

        if ((ZunBool)(optionTimer.current >= 8))
        {
            this->Th07OptionState() = kPlayerOptionStateFocused;
        }
        if (this->Th07FocusHeld() == 0)
        {

            this->Th07OptionState() = kPlayerOptionStateUnfocusing;
            optionTimer.SetCurrent(8 - optionTimer.AsFrames());

            goto CASE_ORB_UNFOCUSING;
        }
        else
        {
            break;
        }

    case kPlayerOptionStateFocused:
        horizontalOrbOffset = 8.0;
        verticalOrbOffset = -32.0;
        optionTimer.InitializeForPopup();
        if (this->Th07FocusHeld() == 0)
        {
            this->Th07OptionState() = kPlayerOptionStateUnfocusing;
        }
        else
        {
            break;
        }

    CASE_ORB_UNFOCUSING:
    case kPlayerOptionStateUnfocusing:
        optionTimer.Tick();

        intermediateFloat = optionTimer.AsFramesFloat() / 8.0f;
        verticalOrbOffset = (32.0f * intermediateFloat) + -32.0f;
        intermediateFloat *= intermediateFloat;
        intermediateFloat = 1.0f - intermediateFloat;
        horizontalOrbOffset = -16.0f * intermediateFloat + 24.0f;
        if ((ZunBool)(optionTimer.current >= 8))
        {
            this->Th07OptionState() = kPlayerOptionStateUnfocused;
        }
        if (this->Th07FocusHeld() != 0)
        {
            this->Th07OptionState() = kPlayerOptionStateFocusing;
            optionTimer.SetCurrent(8 - optionTimer.AsFrames());
            goto CASE_ORB_FOCUSING;
        }
    }

    this->orbsPosition[0].x -= horizontalOrbOffset;
    this->orbsPosition[1].x += horizontalOrbOffset;
    this->orbsPosition[0].y += verticalOrbOffset;
    this->orbsPosition[1].y += verticalOrbOffset;
    SyncLegacyPlayerMovementBridge(this);
    if ((g_CurFrameInput & kPlayerMovementInputMaskShoot) != 0 && !g_Gui.HasCurrentMsgIdx())
    {
        this->StartFireBulletTimer(this);
    }
    this->previousFrameInput = g_CurFrameInput;
    return ZUN_SUCCESS;
}

#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC push_options
#pragma GCC optimize("Og")
#endif
#pragma var_order(bulletIdx, bullets)
void TH07_PLAYER_FASTCALL Player::DrawBullets(Player *p)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *shotSlot;
    i32 bulletIdx;

    shotSlot = reinterpret_cast<u8 *>(p) + kPlayerShotSlotArrayOffset;
    for (bulletIdx = 0; bulletIdx < static_cast<i32>(kPlayerShotSlotCount); bulletIdx = bulletIdx + 1)
    {
        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) == kPlayerShotSlotActiveState)
        {
            if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotAutoRotateOffset) != 0)
            {
                float rotationZ;
                const u32 angleBits = *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotAngleOffset);
                __asm__ __volatile__(
                    "pushl $0x3fc90fdb\n\t"
                    "pushl %1\n\t"
                    "call FUN_00431930\n\t"
                    "addl $8, %%esp\n\t"
                    "fstps %0"
                    : "=m"(rotationZ)
                    : "r"(angleBits)
                    : "eax", "ecx", "edx", "memory");
                *reinterpret_cast<f32 *>(shotSlot + kTh07AnmVmRotationZOffset) = rotationZ;
                *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) =
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) |
                    kPlayerShotSlotRenderFlagsRotationDirtyBit;
            }
            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotDrawPositionXOffset) =
                *reinterpret_cast<f32 *>(kPlayerShotDrawOffsetXAddress) +
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset);
            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotDrawPositionYOffset) =
                *reinterpret_cast<f32 *>(kPlayerShotDrawOffsetYAddress) +
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset);
            *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotDrawPositionZOffset) =
                kPlayerShotDrawZBits;
            FUN_0044f9a0(*reinterpret_cast<void **>(kPlayerShotDrawAnmManagerPointerAddress), shotSlot);
            if (*reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotDrawCallbackOffset) != 0)
            {
                Th07ShtDrawCallback drawCallback =
                    *reinterpret_cast<Th07ShtDrawCallback *>(shotSlot + kPlayerShotSlotDrawCallbackOffset);
                (*drawCallback)(p, shotSlot);
            }
        }
        shotSlot += kPlayerShotSlotStride;
    }
#else
    i32 bulletIdx;
    PlayerBullet *bullets;

    bullets = p->bullets;
    for (bulletIdx = 0; bulletIdx < ARRAY_SIZE_SIGNED(p->bullets); bulletIdx++, bullets++)
    {
        if (bullets->bulletState != BULLET_STATE_FIRED)
        {
            continue;
        }
        if (bullets->sprite.autoRotate)
        {
            bullets->sprite.rotation.z = ZUN_PI / 2 - utils::AddNormalizeAngle(bullets->unk_134.z, ZUN_PI);
        }
        g_AnmManager->Draw2(&bullets->sprite);
    }
#endif
}
#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC pop_options
#endif

#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC push_options
#pragma GCC optimize("Og")
#endif
#pragma var_order(bulletIdx, bullets)
void TH07_PLAYER_FASTCALL Player::DrawBulletExplosions(Player *p)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *shotSlot;
    i32 bulletIdx;

    shotSlot = reinterpret_cast<u8 *>(p) + kPlayerShotSlotArrayOffset;
    for (bulletIdx = 0; bulletIdx < static_cast<i32>(kPlayerShotSlotCount); bulletIdx = bulletIdx + 1)
    {
        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) ==
            kPlayerShotSlotCollidedState)
        {
            if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotAutoRotateOffset) != 0)
            {
                float rotationZ;
                const u32 angleBits = *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotAngleOffset);
                __asm__ __volatile__(
                    "pushl $0x3fc90fdb\n\t"
                    "pushl %1\n\t"
                    "call FUN_00431930\n\t"
                    "addl $8, %%esp\n\t"
                    "fstps %0"
                    : "=m"(rotationZ)
                    : "r"(angleBits)
                    : "eax", "ecx", "edx", "memory");
                *reinterpret_cast<f32 *>(shotSlot + kTh07AnmVmRotationZOffset) = rotationZ;
                *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) =
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) |
                    kPlayerShotSlotRenderFlagsRotationDirtyBit;
            }
            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotDrawPositionXOffset) =
                *reinterpret_cast<f32 *>(kPlayerShotDrawOffsetXAddress) +
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionXOffset);
            *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotDrawPositionYOffset) =
                *reinterpret_cast<f32 *>(kPlayerShotDrawOffsetYAddress) +
                *reinterpret_cast<f32 *>(shotSlot + kPlayerShotSlotPositionYOffset);
            *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotDrawPositionZOffset) =
                kPlayerShotDrawZBits;
            FUN_0044f9a0(*reinterpret_cast<void **>(kPlayerShotDrawAnmManagerPointerAddress), shotSlot);
        }
        shotSlot += kPlayerShotSlotStride;
    }
#else
    i32 bulletIdx;
    PlayerBullet *bullets;

    bullets = p->bullets;
    for (bulletIdx = 0; bulletIdx < ARRAY_SIZE_SIGNED(p->bullets); bulletIdx++, bullets++)
    {
        if (bullets->bulletState != BULLET_STATE_COLLIDED)
        {
            continue;
        }
        if (bullets->sprite.autoRotate)
        {
            bullets->sprite.rotation.z = ZUN_PI / 2 - utils::AddNormalizeAngle(bullets->unk_134.z, ZUN_PI);
        }
        bullets->sprite.pos.z = 0.4f;
        g_AnmManager->Draw2(&bullets->sprite);
    }
#endif
}
#if defined(TH07_PLAYER_OBJDIFF) && defined(__GNUC__)
#pragma GCC pop_options
#endif

void TH07_PLAYER_FASTCALL Player::StartFireBulletTimer(Player *p)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *base = reinterpret_cast<u8 *>(p);
    if (*reinterpret_cast<i32 *>(base + kPlayerFireTimerCurrentOffset) < 0)
    {
        u8 *timer = base + kPlayerFireTimerPreviousOffset;
        *reinterpret_cast<i32 *>(timer + 8) = kPlayerFireTimerStartValue;
        *reinterpret_cast<u32 *>(timer + 4) = kPlayerFireTimerSubframeZeroBits;
        *reinterpret_cast<u32 *>(timer) = kPlayerFireTimerPreviousSentinelBits;
    }
#else
    if (p->fireBulletTimer.AsFrames() < 0)
    {
        p->fireBulletTimer.InitializeForPopup();
    }
#endif
}

ZunResult TH07_PLAYER_FASTCALL Player::UpdateFireBulletsTimer(Player *p)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *base = reinterpret_cast<u8 *>(p);
    u8 *timer;

    if (*reinterpret_cast<i32 *>(base + kPlayerFireTimerCurrentOffset) < 0)
    {
        return ZUN_SUCCESS;
    }

    timer = base + kPlayerFireTimerPreviousOffset;
    if (*reinterpret_cast<i32 *>(timer + 8) != *reinterpret_cast<i32 *>(timer) &&
        (*reinterpret_cast<u32 *>(kPlayerFireTimerReplayGateAddress) == 0 ||
         *reinterpret_cast<u8 *>(kPlayerFireTimerReplayGateByte0Address) != '\x01' ||
         *reinterpret_cast<u8 *>(kPlayerFireTimerReplayGateByte1Address) != '\x01'))
    {
        Player::SpawnBullets(p, *reinterpret_cast<u32 *>(base + kPlayerFireTimerCurrentOffset));
    }

    *reinterpret_cast<u32 *>(timer) = *reinterpret_cast<u32 *>(timer + 8);
    FUN_0043958d(reinterpret_cast<void *>(kPlayerUpdateTimerHelperOwnerAddress),
                 timer + 8, timer + 4);

    if (kPlayerFireTimerStopFrame <= *reinterpret_cast<i32 *>(base + kPlayerFireTimerCurrentOffset) ||
        *reinterpret_cast<char *>(base + kPlayerModeStateOffset) == '\x02' ||
        *reinterpret_cast<char *>(base + kPlayerModeStateOffset) == '\x01')
    {
        *reinterpret_cast<i32 *>(timer + 8) = kPlayerFireTimerStopValue;
        *reinterpret_cast<u32 *>(timer + 4) = kPlayerFireTimerSubframeZeroBits;
        *reinterpret_cast<u32 *>(timer) = kPlayerFireTimerPreviousSentinelBits;
    }
    return ZUN_SUCCESS;
#else
    if (p->fireBulletTimer.AsFrames() < 0)
    {
        return ZUN_SUCCESS;
    }

    if (p->fireBulletTimer.HasTicked() && (!g_Player.bombInfo.isInUse || g_GameManager.character != CHARA_MARISA ||
                                           g_GameManager.shotType != SHOT_TYPE_B))
    {
        p->SpawnBullets(p, p->fireBulletTimer.AsFrames());
    }

    p->fireBulletTimer.Tick();

    if (p->fireBulletTimer.AsFrames() >= 30 || p->playerState == PLAYER_STATE_DEAD ||
        p->playerState == PLAYER_STATE_SPAWNING)
    {
        p->fireBulletTimer.SetCurrent(-1);
    }
    return ZUN_SUCCESS;
#endif
}

#pragma var_order(relY, relX)
f32 TH07_PLAYER_THISCALL Player::AngleFromPlayer(D3DXVECTOR3 *pos)
{
    f32 relX;
    f32 relY;

    relX = pos->x - this->positionCenter.x;
    relY = pos->y - this->positionCenter.y;
    if (relY == 0.0f && relX == 0.0f)
    {
        return ZUN_PI / 2;
    }
    return atan2f(relY, relX);
}

#pragma var_order(relY, relX)
f32 TH07_PLAYER_THISCALL Player::AngleToPlayer(D3DXVECTOR3 *pos)
{
    f32 relX;
    f32 relY;

    relX = this->positionCenter.x - pos->x;
    relY = this->positionCenter.y - pos->y;
    if (relY == 0.0f && relX == 0.0f)
    {
        // Shoot down. An angle of 0 means to the right, and the angle goes
        // clockwise.
        return RADIANS(90.0f);
    }
    return atan2f(relY, relX);
}

#pragma var_order(idx, curBulletIdx, curBullet, bulletResult)
void TH07_PLAYER_FASTCALL Player::SpawnBullets(Player *p, u32 timer)
{
#if defined(TH07_PLAYER_OBJDIFF) && defined(__i386__) && defined(__GNUC__)
    u8 *base = reinterpret_cast<u8 *>(p);
    u32 *powerEntry;
    u8 *shotRecord;
    u8 *shotSlot;
    i32 slotIdx;
    i32 spawnResult;

    if (*reinterpret_cast<char *>(base + kPlayerFocusHeldOffset) == '\0')
    {
        powerEntry = reinterpret_cast<u32 *>(
            *reinterpret_cast<u8 **>(base + kPlayerShotDataPointerOffset) +
            kPlayerSpawnBulletsShtPowerTableOffset);
    }
    else
    {
        powerEntry = reinterpret_cast<u32 *>(
            *reinterpret_cast<u8 **>(base + kPlayerFocusedShotDataPointerOffset) +
            kPlayerSpawnBulletsShtPowerTableOffset);
    }
    while (ReadTh07PlayerSpawnPowerForObjdiff() >=
           *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(powerEntry) +
                                    kPlayerSpawnBulletsShtPowerThresholdOffset))
    {
        powerEntry = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(powerEntry) +
                                             kPlayerSpawnBulletsShtPowerEntryStride);
    }

    shotRecord = reinterpret_cast<u8 *>(*reinterpret_cast<u32 *>(
        reinterpret_cast<u8 *>(powerEntry) + kPlayerSpawnBulletsShtPowerRecordPointerOffset));
    shotSlot = base + kPlayerShotSlotArrayOffset;
    slotIdx = 0;
    do
    {
        if (static_cast<i32>(kPlayerShotSlotCount) <= slotIdx)
        {
            return;
        }
        if (*reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) == 0)
        {
            do
            {
                Th07ShtSpawnCallback spawnCallback =
                    *reinterpret_cast<Th07ShtSpawnCallback *>(
                        shotRecord + kPlayerSpawnBulletsShtSpawnCallbackOffset);
                if (spawnCallback == nullptr)
                {
                    spawnResult = FUN_0043bdc0(p, shotSlot, timer, shotRecord);
                }
                else
                {
                    spawnResult = (*spawnCallback)(p, shotSlot, timer, shotRecord);
                }
                if (spawnResult == 1)
                {
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) =
                        *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotRenderFlagsOffset) |
                        kPlayerShotSlotRenderFlagsActiveBit;
                    *reinterpret_cast<i16 *>(shotSlot + kPlayerShotSlotStateOffset) = 1;
                    *reinterpret_cast<u8 **>(shotSlot + kPlayerShotSlotShtRecordPointerOffset) = shotRecord;
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotUpdateCallbackOffset) =
                        *reinterpret_cast<u32 *>(
                            *reinterpret_cast<u8 **>(shotSlot + kPlayerShotSlotShtRecordPointerOffset) +
                            kPlayerSpawnBulletsShtUpdateCallbackOffset);
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotDrawCallbackOffset) =
                        *reinterpret_cast<u32 *>(
                            *reinterpret_cast<u8 **>(shotSlot + kPlayerShotSlotShtRecordPointerOffset) +
                            kPlayerSpawnBulletsShtDrawCallbackOffset);
                    *reinterpret_cast<u32 *>(shotSlot + kPlayerShotSlotCollisionCallbackOffset) =
                        *reinterpret_cast<u32 *>(
                            *reinterpret_cast<u8 **>(shotSlot + kPlayerShotSlotShtRecordPointerOffset) +
                            kPlayerSpawnBulletsShtCollisionCallbackOffset);
                }
                shotRecord += kPlayerSpawnBulletsShtShotRecordStride;
                if (*reinterpret_cast<i16 *>(shotRecord + kPlayerSpawnBulletsShtShotRecordTerminatorOffset) <
                    0)
                {
                    return;
                }
            } while (spawnResult == 0);
        }
        slotIdx = slotIdx + 1;
        shotSlot += kPlayerShotSlotStride;
    } while (true);
#else
    FireBulletResult bulletResult;
    PlayerBullet *curBullet;
    i32 curBulletIdx;
    u32 idx;

    idx = 0;
    curBullet = p->bullets;

    for (curBulletIdx = 0; curBulletIdx < ARRAY_SIZE_SIGNED(p->bullets); curBulletIdx++, curBullet++)
    {
        if (curBullet->bulletState != BULLET_STATE_UNUSED)
        {
            continue;
        }
    WHILE_LOOP:
        if (!p->isFocus)
        {
            bulletResult = (*p->fireBulletCallback)(p, curBullet, idx, timer);
        }
        else
        {
            bulletResult = (*p->fireBulletFocusCallback)(p, curBullet, idx, timer);
        }
        if (bulletResult >= 0)
        {
            curBullet->sprite.pos.x = curBullet->position.x;
            curBullet->sprite.pos.y = curBullet->position.y;
            curBullet->sprite.pos.z = 0.495;
            curBullet->bulletState = BULLET_STATE_FIRED;
        }
        if (bulletResult == FBR_STOP_SPAWNING)
        {
            return;
        }
        if (bulletResult > 0)
        {
            return;
        }
        idx++;
        if (bulletResult == FBR_SPAWN_MORE)
        {
            goto WHILE_LOOP;
        }
    }
#endif
}

#pragma var_order(bulletData, bulletFrame, unused3, unused, unused2)
FireBulletResult Player::FireSingleBullet(Player *player, PlayerBullet *bullet, i32 bulletIdx,
                                          i32 framesSinceLastBullet, CharacterPowerData *powerData)
{
    CharacterPowerBulletData *bulletData;
    i32 bulletFrame;
    i32 unused;
    i32 unused2;
    i32 unused3;

    while (g_GameManager.currentPower >= powerData->power)
    {
        powerData++;
    }

    bulletData = powerData->bullets + bulletIdx;

    if (bulletData->bulletType == BULLET_TYPE_LASER)
    {
        bulletFrame = bulletData->bulletFrame;
        if (!player->laserTimer[bulletFrame].AsFrames())
        {
            player->laserTimer[bulletFrame].SetCurrent(bulletData->waitBetweenBullets);

            bullet->unk_152 = bulletFrame;
            bullet->spawnPositionIdx = bulletData->spawnPositionIdx;
            bullet->sidewaysMotion = bulletData->motion.x;
            bullet->unk_134.x = bulletData->motion.y;
            goto SHOOT_BULLET;
        }
    }
    else if (framesSinceLastBullet % bulletData->waitBetweenBullets == bulletData->bulletFrame)
    {
    SHOOT_BULLET:

        g_AnmManager->SetAndExecuteScriptIdx(&bullet->sprite, bulletData->anmFileIdx);
        if (!bulletData->spawnPositionIdx)
        {
            bullet->position = player->positionCenter;
        }
        else
        {
            bullet->position = player->orbsPosition[bulletData->spawnPositionIdx - 1];
        }
        bullet->position[0] += bulletData->motion.x;
        bullet->position[1] += bulletData->motion.y;

        bullet->position.z = 0.495f;

        bullet->size.x = bulletData->size.x;
        bullet->size.y = bulletData->size.y;
        bullet->size.z = 1.0f;
        bullet->unk_134.z = bulletData->direction;
        bullet->unk_134.y = bulletData->velocity;

        bullet->velocity.x = cosf(bulletData->direction) * bulletData->velocity;

        bullet->velocity.y = sinf(bulletData->direction) * bulletData->velocity;

        bullet->unk_140.InitializeForPopup();

        bullet->bulletType = bulletData->bulletType;
        bullet->damage = bulletData->damage;
        if (bulletData->bulletSoundIdx >= 0)
        {
            g_SoundPlayer.PlaySoundByIdx((SoundIdx)bulletData->bulletSoundIdx, 0);
        }

        return bulletIdx >= powerData->numBullets - 1;
    }

    if (bulletIdx >= powerData->numBullets - 1)
    {
        return FBR_STOP_SPAWNING;
    }
    else
    {
        return FBR_SPAWN_MORE;
    }
}

FireBulletResult Player::FireBulletReimuA(Player *player, PlayerBullet *bullet, u32 bulletIdx,
                                          u32 framesSinceLastBullet)
{
    return player->FireSingleBullet(player, bullet, bulletIdx, framesSinceLastBullet, g_CharacterPowerDataReimuA);
}

FireBulletResult Player::FireBulletReimuB(Player *player, PlayerBullet *bullet, u32 bulletIdx,
                                          u32 framesSinceLastBullet)
{
    return player->FireSingleBullet(player, bullet, bulletIdx, framesSinceLastBullet, g_CharacterPowerDataReimuB);
}

FireBulletResult Player::FireBulletMarisaA(Player *player, PlayerBullet *bullet, u32 bulletIdx,
                                           u32 framesSinceLastBullet)
{
    return player->FireSingleBullet(player, bullet, bulletIdx, framesSinceLastBullet, g_CharacterPowerDataMarisaA);
}

FireBulletResult Player::FireBulletMarisaB(Player *player, PlayerBullet *bullet, u32 bulletIdx,
                                           u32 framesSinceLastBullet)
{
    return player->FireSingleBullet(player, bullet, bulletIdx, framesSinceLastBullet, g_CharacterPowerDataMarisaB);
}

FireBulletResult Player::FireBulletSakuyaA(Player *, PlayerBullet *, u32, u32)
{
    // TH07 Sakuya .sht records are parsed, but the original callback runtime is not wired yet.
    return FBR_STOP_SPAWNING;
}

FireBulletResult Player::FireBulletSakuyaB(Player *, PlayerBullet *, u32, u32)
{
    // Keep this explicit rather than borrowing another shot type's behavior.
    return FBR_STOP_SPAWNING;
}

#pragma var_order(i, slot, bulletBottom, bulletRight, bulletTop, bulletLeft)
i32 TH07_PLAYER_THISCALL Player::CalcBombCollision(D3DXVECTOR3 *center, D3DXVECTOR3 *size)
{
    f32 bulletLeft;
    f32 bulletTop;
    f32 bulletRight;
    f32 bulletBottom;
    f32 *slot;
    i32 i;

    bulletLeft = center->x - size->x / 2.0f;
    bulletTop = center->y - size->y / 2.0f;
    bulletRight = size->x / 2.0f + center->x;
    bulletBottom = size->y / 2.0f + center->y;
    slot = reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(this) + kPlayerBombCollisionSlotArrayOffset);
    for (i = 0; i < static_cast<i32>(kPlayerBombCollisionSlotCount); i++, slot += 8)
    {
        if (slot[2] == 0.0f)
        {
            const f32 deltaX = center->x - slot[0];
            const f32 deltaY = center->y - slot[1];
            if (slot[4] != 0.0f && deltaY * deltaY + deltaX * deltaX < slot[4] * slot[4])
            {
                *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(this) + kPlayerCollisionPayloadOffset) =
                    reinterpret_cast<u32 *>(slot)[7];
                return 2;
            }
        }
        else if (slot[0] - slot[2] / 2.0f <= bulletRight &&
                 bulletLeft <= slot[2] / 2.0f + slot[0] &&
                 slot[1] - slot[3] / 2.0f <= bulletBottom &&
                 bulletTop <= slot[3] / 2.0f + slot[1])
        {
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(this) + kPlayerCollisionPayloadOffset) =
                reinterpret_cast<u32 *>(slot)[7];
            return 2;
        }
    }

    return 0;
}

#pragma var_order(slot, i)
void TH07_PLAYER_THISCALL Player::UpdateBombCollisionSlots()
{
    Th07PlayerBombCollisionSlot *slot;
    i32 i;

    for (i = 0; i < static_cast<i32>(kPlayerBombCollisionBulletClearCount); i++)
    {
        *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(this) + kPlayerBombCollisionBulletClearOffset +
                                 i * kPlayerBombCollisionBulletClearStride) = 0;
    }

    slot = this->th07BombCollisionSlots;
    for (i = 0; i < static_cast<i32>(kPlayerBombCollisionSlotCount); i++, slot++)
    {
        if (slot->framesLeft < 1)
        {
            slot->radius = 0.0f;
            slot->rectSizeX = 0.0f;
        }
        else
        {
            slot->framesLeft--;
            slot->radius += slot->radiusVelocity;
        }
    }
}

#pragma var_order(slot, i)
Th07PlayerBombCollisionSlot *TH07_PLAYER_THISCALL Player::RegisterBombCollisionRect(
    D3DXVECTOR3 *center, f32 sizeX, f32 sizeY, u32 payloadBits)
{
    Th07PlayerBombCollisionSlot *slot;
    i32 i;

    for (i = 0, slot = this->th07BombCollisionSlots;
         i < static_cast<i32>(kPlayerBombCollisionSlotCount - 1) &&
         (slot->rectSizeX != 0.0f || slot->radius != 0.0f);
         i++, slot++)
    {
    }

    slot->x = center->x;
    slot->y = center->y;
    slot->rectSizeX = sizeX;
    slot->rectSizeY = sizeY;
    slot->framesLeft = 0;
    slot->payloadBits = payloadBits;
    return slot;
}

#pragma var_order(slot, i)
Th07PlayerBombCollisionSlot *TH07_PLAYER_THISCALL Player::RegisterBombCollisionCircle(
    D3DXVECTOR3 *center, f32 radius, f32 radiusVelocity, i32 framesLeft, u32 payloadBits)
{
    Th07PlayerBombCollisionSlot *slot;
    i32 i;

    for (i = 0, slot = this->th07BombCollisionSlots;
         i < static_cast<i32>(kPlayerBombCollisionSlotCount - 1) &&
         (slot->rectSizeX != 0.0f || slot->radius != 0.0f);
         i++, slot++)
    {
    }

    slot->x = center->x;
    slot->y = center->y;
    slot->radius = radius;
    slot->radiusVelocity = radiusVelocity;
    slot->framesLeft = framesLeft;
    slot->payloadBits = payloadBits;
    return slot;
}

#pragma var_order(bombTopLeft, i, bulletBottomRight, bulletTopLeft, bombProjectile, bombBottomRight)
i32 TH07_PLAYER_THISCALL Player::CheckGraze(D3DXVECTOR3 *center, D3DXVECTOR3 *size)
{
    D3DXVECTOR3 bombBottomRight;
    PlayerRect *bombProjectile;
    D3DXVECTOR3 bombTopLeft;
    D3DXVECTOR3 bulletBottomRight;
    D3DXVECTOR3 bulletTopLeft;
    i32 i;

    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(this) + kPlayerCollisionPayloadOffset) =
        kPlayerCollisionDefaultPayloadBits;
    if (this->CalcBombCollision(center, size) != 0)
    {
        return 2;
    }

    bulletTopLeft.x = center->x - size->x / 2.0f - 20.0f;
    bulletTopLeft.y = center->y - size->y / 2.0f - 20.0f;
    bulletBottomRight.x = center->x + size->x / 2.0f + 20.0f;
    bulletBottomRight.y = center->y + size->y / 2.0f + 20.0f;
    bombProjectile = this->bombProjectiles;

    for (i = 0; i < ARRAY_SIZE_SIGNED(this->bombProjectiles); i++, bombProjectile++)
    {
        if (bombProjectile->sizeX == 0.0f)
        {
            continue;
        }

        bombTopLeft.x = bombProjectile->posX - bombProjectile->sizeX / 2.0f;
        bombTopLeft.y = bombProjectile->posY - bombProjectile->sizeY / 2.0f;
        bombBottomRight.x = bombProjectile->sizeX / 2.0f + bombProjectile->posX;
        bombBottomRight.y = bombProjectile->sizeY / 2.0f + bombProjectile->posY;

        // Bomb clips bullet's hitbox, destroys bullet upon return
        if (!(bombTopLeft.x > bulletBottomRight.x || bombBottomRight.x < bulletTopLeft.x ||
              bombTopLeft.y > bulletBottomRight.y || bombBottomRight.y < bulletTopLeft.y))
        {
            return 2;
        }
    }

    if (this->playerState == PLAYER_STATE_DEAD || this->playerState == PLAYER_STATE_SPAWNING)
    {
        return 0;
    }
    if (this->grazeTopLeft.x > bulletBottomRight.x || this->grazeBottomRight.x < bulletTopLeft.x ||
        this->grazeTopLeft.y > bulletBottomRight.y || this->grazeBottomRight.y < bulletTopLeft.y)
    {
        return 0;
    }

    // Bullet clips player's graze hitbox, add score and check for death upon return
    this->ScoreGraze(center);
    return 1;
}

#pragma var_order(padding1, bombProjectileTop, bombProjectileLeft, curBombIdx, padding2, bulletBottom, bulletRight,    \
                  padding3, bulletTop, bulletLeft, curBombProjectile, padding4, bombProjectileBottom,                  \
                  bombProjectileRight)
i32 TH07_PLAYER_THISCALL Player::CalcKillBoxCollision(D3DXVECTOR3 *bulletCenter, D3DXVECTOR3 *bulletSize)
{
    PlayerRect *curBombProjectile;
    f32 bulletLeft, bulletTop, bulletRight, bulletBottom;
    f32 bombProjectileLeft, bombProjectileTop, bombProjectileRight, bombProjectileBottom;
    i32 curBombIdx;
    i32 padding1, padding2, padding3, padding4;

    *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(this) + kPlayerCollisionPayloadOffset) =
        kPlayerCollisionDefaultPayloadBits;
    if (this->CalcBombCollision(bulletCenter, bulletSize) != 0)
    {
        return 2;
    }

    curBombProjectile = this->bombProjectiles;
    bulletLeft = bulletCenter->x - bulletSize->x / 2.0f;
    bulletTop = bulletCenter->y - bulletSize->y / 2.0f;
    bulletRight = bulletCenter->x + bulletSize->x / 2.0f;
    bulletBottom = bulletCenter->y + bulletSize->y / 2.0f;
    for (curBombIdx = 0; curBombIdx < ARRAY_SIZE_SIGNED(this->bombProjectiles); curBombIdx++, curBombProjectile++)
    {
        if (curBombProjectile->sizeX == 0.0f)
        {
            continue;
        }
        bombProjectileLeft = curBombProjectile->posX - curBombProjectile->sizeX / 2.0f;
        bombProjectileTop = curBombProjectile->posY - curBombProjectile->sizeY / 2.0f;
        bombProjectileRight = curBombProjectile->posX + curBombProjectile->sizeX / 2.0f;
        bombProjectileBottom = curBombProjectile->posY + curBombProjectile->sizeY / 2.0f;
        if (!(bombProjectileLeft > bulletRight || bombProjectileRight < bulletLeft ||
              bombProjectileTop > bulletBottom || bombProjectileBottom < bulletTop))
        {
            return 2;
        }
    }
    if (this->hitboxTopLeft.x > bulletRight || this->hitboxTopLeft.y > bulletBottom ||
        this->hitboxBottomRight.x < bulletLeft || this->hitboxBottomRight.y < bulletTop)
    {
        return 0;
    }
    else if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 1;
    }
    else
    {
        this->Die();
        return 1;
    }
}

#pragma var_order(playerRelativeTopLeft, laserBottomRight, laserTopLeft, playerRelativeBottomRight)
i32 TH07_PLAYER_THISCALL Player::CalcLaserHitbox(D3DXVECTOR3 *laserCenter, D3DXVECTOR3 *laserSize,
                                                 D3DXVECTOR3 *rotation, f32 angle, i32 canGraze)
{
    D3DXVECTOR3 laserTopLeft;
    D3DXVECTOR3 laserBottomRight;
    D3DXVECTOR3 playerRelativeTopLeft;
    D3DXVECTOR3 playerRelativeBottomRight;

    laserTopLeft = this->positionCenter - *rotation;
    utils::Rotate(&laserBottomRight, &laserTopLeft, angle);
    laserBottomRight.z = 0;
    laserTopLeft = laserBottomRight + *rotation;
    playerRelativeTopLeft = laserTopLeft - this->hitboxSize;
    playerRelativeBottomRight = laserTopLeft + this->hitboxSize;

    laserTopLeft = *laserCenter - *laserSize / 2.0f;
    laserBottomRight = *laserCenter + *laserSize / 2.0f;

    if (!(playerRelativeTopLeft.x > laserBottomRight.x || playerRelativeBottomRight.x < laserTopLeft.x ||
          playerRelativeTopLeft.y > laserBottomRight.y || playerRelativeBottomRight.y < laserTopLeft.y))
    {
        goto LASER_COLLISION;
    }
    if (canGraze == 0)
    {
        return 0;
    }

    laserTopLeft.x -= 48.0f;
    laserTopLeft.y -= 48.0f;
    laserBottomRight.x += 48.0f;
    laserBottomRight.y += 48.0f;

    if (playerRelativeTopLeft.x > laserBottomRight.x || playerRelativeBottomRight.x < laserTopLeft.x ||
        playerRelativeTopLeft.y > laserBottomRight.y || playerRelativeBottomRight.y < laserTopLeft.y)
    {
        return 0;
    }
    if (this->playerState == PLAYER_STATE_DEAD || this->playerState == PLAYER_STATE_SPAWNING)
    {
        return 0;
    }

    this->ScoreGraze(&this->positionCenter);
    return 2;

LASER_COLLISION:
    if (this->playerState != PLAYER_STATE_ALIVE)
    {
        return 0;
    }

    this->Die();
    return 1;
}

#pragma var_order(itemBottomRight, itemTopLeft)
i32 TH07_PLAYER_THISCALL Player::CalcItemBoxCollision(D3DXVECTOR3 *itemCenter, D3DXVECTOR3 *itemSize)
{
    if (*reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 0 &&
        *reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 3 &&
        *reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 4)
    {
        return 0;
    }

    const f32 halfItemWidth = itemSize->x * (1.0f / 2.0f);
    const f32 halfItemHeight = itemSize->y * (1.0f / 2.0f);
    if (itemCenter->x + halfItemWidth < this->grabItemTopLeft.x ||
        this->grabItemBottomRight.x < itemCenter->x - halfItemWidth ||
        itemCenter->y + halfItemHeight < this->grabItemTopLeft.y ||
        this->grabItemBottomRight.y < itemCenter->y - halfItemHeight)
    {
        return 0;
    }
    return 1;
}

void TH07_PLAYER_THISCALL Player::ScoreGraze(D3DXVECTOR3 *center)
{
    D3DXVECTOR3 particlePosition;

    if (g_Player.bombInfo.isInUse == 0)
    {
        if (g_GameManager.grazeInStage < 9999)
        {
            g_GameManager.grazeInStage++;
        }
        if (g_GameManager.grazeInTotal < 999999)
        {
            g_GameManager.grazeInTotal++;
        }
    }

    particlePosition = (this->positionCenter + *center) / 2.0f;
    g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_8, &particlePosition, 1, COLOR_WHITE);
    g_GameManager.AddScore(500);
    g_GameManager.IncreaseSubrank(6);
    g_Gui.flags.flag3 = 2;
    g_SoundPlayer.PlaySoundByIdx(SOUND_GRAZE, 0);
}

#pragma var_order(curLaserTimerIdx)
void TH07_PLAYER_THISCALL Player::Die()
{
    int curLaserTimerIdx;

    g_EnemyManager.spellcardInfo.isCapturing = 0;
    g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_12, &this->positionCenter, 1, COLOR_NEONBLUE);
    g_EffectManager.SpawnParticles(PARTICLE_EFFECT_UNK_6, &this->positionCenter, 16, COLOR_WHITE);
    this->playerState = PLAYER_STATE_DEAD;
    this->invulnerabilityTimer.InitializeForPopup();
    g_SoundPlayer.PlaySoundByIdx(SOUND_PICHUN, 0);
    g_GameManager.deaths++;
    for (curLaserTimerIdx = 0; curLaserTimerIdx < ARRAY_SIZE_SIGNED(this->laserTimer); curLaserTimerIdx++)
    {
        this->laserTimer[curLaserTimerIdx].SetCurrent(2);
    }
    return;
}
}; // namespace th07
