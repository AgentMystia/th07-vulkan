#pragma once

#include "Chain.hpp"
#include "Effect.hpp"
#include "Th07EffectTables.hpp"
#include "ZunColor.hpp"
#include "ZunResult.hpp"
#include "inttypes.hpp"

#include <cstddef>

namespace th07
{

enum ParticleEffects
{
    PARTICLE_EFFECT_UNK_0,
    PARTICLE_EFFECT_UNK_1,
    PARTICLE_EFFECT_UNK_2,
    PARTICLE_EFFECT_UNK_3,
    PARTICLE_EFFECT_UNK_4,
    PARTICLE_EFFECT_UNK_5,
    PARTICLE_EFFECT_UNK_6,
    PARTICLE_EFFECT_UNK_7,
    PARTICLE_EFFECT_UNK_8,
    PARTICLE_EFFECT_UNK_9,
    PARTICLE_EFFECT_UNK_10,
    PARTICLE_EFFECT_UNK_11,
    PARTICLE_EFFECT_UNK_12,
    PARTICLE_EFFECT_UNK_13,
    PARTICLE_EFFECT_UNK_14,
    PARTICLE_EFFECT_UNK_15,
    PARTICLE_EFFECT_UNK_16,
    PARTICLE_EFFECT_UNK_17,
    PARTICLE_EFFECT_UNK_18,
    PARTICLE_EFFECT_UNK_19,
    PARTICLE_EFFECT_UNK_20,
    PARTICLE_EFFECT_UNK_21,
    PARTICLE_EFFECT_UNK_22,
    PARTICLE_EFFECT_UNK_23,
    PARTICLE_EFFECT_UNK_24,
    PARTICLE_EFFECT_BOMB_COMMON,
    PARTICLE_EFFECT_UNK_26,
    PARTICLE_EFFECT_UNK_27,
    PARTICLE_EFFECT_UNK_28,
    PARTICLE_EFFECT_UNK_29,
    PARTICLE_EFFECT_UNK_30,
    PARTICLE_EFFECT_UNK_31,
    PARTICLE_EFFECT_UNK_32,
    PARTICLE_EFFECT_UNK_33,
};

struct EffectManager
{
    i32 nextIndex;
    i32 unk_4;
    i32 activeEffects;
    u8 unk_c_1c[0x10];
    Effect effects[408];
    Effect fallbackEffect;
    Effect drawListScrollSentinel;
    Effect drawListIndexedSentinel;
    Effect drawListNoScrollSentinel;
    Effect drawListScrollFlaggedSentinel;
    Effect *drawListScrollTail;
    Effect *drawListIndexedTail;
    Effect *drawListNoScrollTail;
    Effect *drawListScrollFlaggedTail;
    i32 frameCounter;

    EffectManager();

    static ZunResult RegisterChain();
    static void CutChain();
    static ChainCallbackResult OnUpdate(EffectManager *mgr);
    static ZunResult AddedCallback(EffectManager *mgr);
    static ZunResult DeletedCallback(EffectManager *mgr);

    static i32 EffectCallbackDone(Effect *);
    static i32 EffectCallbackLinear(Effect *);
    static i32 EffectInitRandomSplash(Effect *);
    static i32 EffectInitRandomSplashBig(Effect *);
    static i32 EffectInitRenderMode2(Effect *);
    static i32 EffectCallbackOrbitingRenderMode2(Effect *);
    static i32 EffectInitRadialDirection(Effect *);
    static i32 EffectCallbackRadialInwardShort(Effect *);
    static i32 EffectCallbackRadialInwardSlow(Effect *);
    static i32 EffectCallbackRadialOutward(Effect *);
    static i32 EffectCallbackPlayerPositionWhileAnmActive(Effect *);
    static i32 EffectCallbackPerspectiveMotion(Effect *);
    static i32 EffectInitPerspective(Effect *, Th07EffectPerspectiveInitKind kind);
    static i32 EffectCallbackCameraSnap(Effect *);
    static i32 EffectInitEasedRadial(Effect *);
    static i32 EffectCallbackEasedRadial(Effect *);
    static i32 DispatchUpdateCallback(Effect *effect);
    static i32 DispatchInitCallback(Effect *effect, u32 callbackAddress);

    static ChainCallbackResult OnDraw(EffectManager *mgr);
    void Reset();
    void ResetDrawLists();
    void LinkEffectForDraw(Effect *effect);
    Effect *SpawnParticles(i32 effectIdx, D3DXVECTOR3 *pos, i32 count, ZunColor color);
    Effect *SpawnParticlesWithAuxVector(i32 effectIdx, D3DXVECTOR3 *pos, D3DXVECTOR3 *auxVector, i32 count,
                                        ZunColor color);
    Effect *SpawnDedicatedParticle(i32 effectIdx, D3DXVECTOR3 *pos, i32 dedicatedSlot, i32 spawnFlag,
                                   ZunColor color);
};
ZUN_ASSERT_SIZE(EffectManager, 0x496a8);

static_assert(offsetof(EffectManager, effects) == 0x1c);
static_assert(offsetof(EffectManager, fallbackEffect) == 0x4885c);
static_assert(offsetof(EffectManager, drawListScrollSentinel) == 0x48b34);
static_assert(offsetof(EffectManager, drawListIndexedSentinel) == 0x48e0c);
static_assert(offsetof(EffectManager, drawListNoScrollSentinel) == 0x490e4);
static_assert(offsetof(EffectManager, drawListScrollFlaggedSentinel) == 0x493bc);
static_assert(offsetof(EffectManager, drawListScrollTail) == 0x49694);
static_assert(offsetof(EffectManager, frameCounter) == 0x496a4);

DIFFABLE_EXTERN(EffectManager, g_EffectManager);
}; // namespace th07
