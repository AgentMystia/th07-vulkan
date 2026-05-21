#include "EffectManager.hpp"

#include "AnmManager.hpp"
#include "Chain.hpp"
#include "ChainPriorities.hpp"
#include "GameManager.hpp"
#include "Player.hpp"
#include "Rng.hpp"
#include "Stage.hpp"
#include "Supervisor.hpp"
#include "ZunResult.hpp"
#include "utils.hpp"

#include <cstring>

namespace th07
{
DIFFABLE_STATIC(EffectManager, g_EffectManager);

DIFFABLE_STATIC(ChainElem, g_EffectManagerCalcChain);
DIFFABLE_STATIC(ChainElem, g_EffectManagerDrawChain);

DIFFABLE_STATIC_ARRAY_ASSIGN(EffectInfo, 34, g_Effects) = {
    {0x02ab, 0, 0},
    {0x02ac, 0, 0},
    {0x02ad, 0, 0},
    {0x02ae, 0x0041a4f0, 0x0041a5a0},
    {0x02b3, 0x0041a4f0, 0x0041a370},
    {0x02b4, 0x0041a4f0, 0x0041a370},
    {0x02b5, 0x0041a4f0, 0x0041a370},
    {0x02b6, 0x0041a4f0, 0x0041a370},
    {0x02b7, 0x0041a4f0, 0x0041a370},
    {0x02b8, 0x0041a4f0, 0x0041a370},
    {0x02b9, 0x0041a4f0, 0x0041a370},
    {0x02ba, 0x0041a4f0, 0x0041a370},
    {0x02bb, 0, 0},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02bc, 0x0041a750, 0x0041a730},
    {0x02dc, 0, 0},
    {0x02af, 0x0041aaf0, 0x0041aa60},
    {0x02b0, 0x0041ac30, 0x0041aa60},
    {0x02bd, 0x0041c1b0, 0},
    {0x02bf, 0x0041aef0, 0x0041b0b0},
    {0x02c3, 0, 0},
    {0x02c0, 0x0041bfd0, 0x0041bec0},
    {0x0304, 0x0041c100, 0},
    {0x02c2, 0x0041abe0, 0},
    {0x02da, 0x0041c1b0, 0},
    {0x02bf, 0x0041aef0, 0x0041b4a0},
    {0x02bf, 0x0041aef0, 0x0041b770},
    {0x02db, 0x0041c1b0, 0},
    {0x02b2, 0x0041ad10, 0x0041aa60},
    {0x02bf, 0x0041aef0, 0x0041b9f0},
    {0x02bf, 0x0041aef0, 0x0041bc20},
    {0x02c1, 0x0041bfd0, 0x0041bec0},
    {0x02b1, 0x0041aaf0, 0x0041aa60},
};

namespace {

const EffectInfo *GetEffectInfo(i32 effectIdx)
{
    if (effectIdx < 0 || effectIdx >= ARRAY_SIZE_SIGNED(g_Effects))
    {
        return NULL;
    }
    return &g_Effects[effectIdx];
}

void InitializeEffectSlot(Effect *effect, i32 effectIdx, D3DXVECTOR3 *pos, ZunColor color, ZunBool setInitialRenderFlag,
                          D3DXVECTOR3 *auxVector)
{
    const EffectInfo *effectInfo = GetEffectInfo(effectIdx);

    std::memset(effect, 0, sizeof(*effect));
    effect->inUseFlag = 1;
    effect->effectId = effectIdx;
    effect->pos1 = *pos;
    effect->anmScript = effectInfo != NULL ? static_cast<i16>(effectInfo->anmIdx) : 0;

    if (effectInfo != NULL)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&effect->vm, effectInfo->anmIdx);
        effect->updateCallbackAddress = effectInfo->updateCallbackAddress;
    }

    if (setInitialRenderFlag)
    {
        effect->renderFlags |= kTh07EffectSlotInitialRenderFlagBits;
    }
    effect->color = color;
    effect->timer.InitializeForPopup();
    if (auxVector != NULL)
    {
        effect->auxVector = *auxVector;
    }

    if (effectInfo != NULL && effectInfo->initCallbackAddress != 0 &&
        EffectManager::DispatchInitCallback(effect, effectInfo->initCallbackAddress) != 0)
    {
        effect->inUseFlag = 0;
    }
}

void CopyEffectPositionToDrawPosition(Effect *effect, ZunBool applyScroll)
{
    effect->drawPosition = effect->pos1;
    if (applyScroll)
    {
        effect->drawPosition.x += g_GameManager.arcadeRegionTopLeftPos.x;
        effect->drawPosition.y += g_GameManager.arcadeRegionTopLeftPos.y;
    }
    effect->vm.pos = effect->drawPosition;
}

void DrawScrollList(Effect *first)
{
    for (Effect *effect = first; effect != NULL; effect = effect->nextDraw)
    {
        CopyEffectPositionToDrawPosition(effect, true);
        g_AnmManager->DrawTh07ScreenQuad(effect);
    }
}

void DrawNoScrollList(Effect *first)
{
    for (Effect *effect = first; effect != NULL; effect = effect->nextDraw)
    {
        CopyEffectPositionToDrawPosition(effect, false);
        g_AnmManager->DrawTh07CameraProjected(effect);
    }
}

void DrawIndexedList(Effect *first)
{
    for (Effect *effect = first; effect != NULL; effect = effect->nextDraw)
    {
        CopyEffectPositionToDrawPosition(effect, false);
        if (effect->renderMode == 1)
        {
            g_AnmManager->DrawTh07CameraProjected(effect);
        }
        else
        {
            g_AnmManager->DrawTh07TransformedCameraProjected(effect);
        }
    }
}

void ApplyEffectLinearMotion(Effect *effect, const Th07EffectLinearMotion &motion)
{
    effect->unk_11c.x = motion.velocity.x;
    effect->unk_11c.y = motion.velocity.y;
    effect->unk_11c.z = motion.velocity.z;
    effect->unk_128.x = motion.acceleration.x;
    effect->unk_128.y = motion.acceleration.y;
    effect->unk_128.z = motion.acceleration.z;
}

Th07EffectVector3 ToTh07EffectVector3(const D3DXVECTOR3 &value)
{
    return {value.x, value.y, value.z};
}

void ApplyTh07EffectVector3(D3DXVECTOR3 *out, const Th07EffectVector3 &value)
{
    out->x = value.x;
    out->y = value.y;
    out->z = value.z;
}

Th07EffectPerspectiveCameraBasis BuildCurrentPerspectiveCameraBasis()
{
    return {
        ToTh07EffectVector3(g_Stage.th07CameraCurrentBasis.position),
        ToTh07EffectVector3(g_Stage.th07CameraCurrentBasis.offset),
    };
}

Th07EffectPerspectiveInitRandoms BuildPerspectiveInitRandoms()
{
    return {
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
        g_Rng.GetRandomF32ZeroToOne(),
    };
}

void ApplyPerspectiveInitState(Effect *effect, const Th07EffectPerspectiveInitState &state)
{
    ApplyTh07EffectVector3(&effect->targetPosition, state.targetPosition);
    ApplyTh07EffectVector3(&effect->unk_11c, state.velocity);
    ApplyTh07EffectVector3(&effect->unk_128, state.acceleration);
    effect->renderMode = static_cast<i8>(state.renderMode);
    effect->vm.rotation.x = state.rotationX;
    effect->vm.rotation.z = state.rotationZ;

    if (state.forceRgbWhite)
    {
        g_AnmManager->SetAndExecuteScriptIdx(&effect->vm, kTh07EffectPerspectiveWhiteAnmScript);
        effect->anmScript = kTh07EffectPerspectiveWhiteAnmScript;
        effect->color = COLOR_COMBINE_ALPHA(COLOR_WHITE, effect->color);
    }
    if (state.doubleAngleVelocityZ)
    {
        effect->vm.angleVel.z += effect->vm.angleVel.z;
    }
}

} // namespace

EffectManager::EffectManager()
{
    this->Reset();
}

void EffectManager::Reset()
{
    std::memset(this, 0, sizeof(*this));
}

i32 EffectManager::EffectCallbackDone(Effect *)
{
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectCallbackLinear(Effect *effect)
{
    effect->pos1 += effect->unk_11c;
    effect->unk_11c += effect->unk_128;
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectInitRandomSplash(Effect *effect)
{
    ApplyEffectLinearMotion(effect,
                            BuildTh07EffectRandomSplashMotion(g_Rng.GetRandomF32ZeroToOne(),
                                                              g_Rng.GetRandomF32ZeroToOne(),
                                                              g_Supervisor.effectiveFramerateMultiplier));
    return 0;
}

i32 EffectManager::EffectInitRandomSplashBig(Effect *effect)
{
    ApplyEffectLinearMotion(effect,
                            BuildTh07EffectRandomSplashBigMotion(g_Rng.GetRandomF32ZeroToOne(),
                                                                 g_Rng.GetRandomF32ZeroToOne(),
                                                                 g_Supervisor.effectiveFramerateMultiplier));
    return 0;
}

i32 EffectManager::EffectInitRenderMode2(Effect *effect)
{
    effect->renderMode = 2;
    return 0;
}

i32 EffectManager::EffectCallbackOrbitingRenderMode2(Effect *effect)
{
    const Th07EffectOrbitState orbitState =
        BuildTh07EffectOrbitingRenderMode2State(ToTh07EffectVector3(effect->position),
                                                ToTh07EffectVector3(effect->pos2),
                                                effect->unk_15c,
                                                effect->angleRelated);
    ApplyTh07EffectVector3(&effect->pos1, orbitState.position);
    effect->quaternion.x = orbitState.quaternion.x;
    effect->quaternion.y = orbitState.quaternion.y;
    effect->quaternion.z = orbitState.quaternion.z;
    effect->quaternion.w = orbitState.quaternion.w;

    if (effect->unk_17a != 0)
    {
        const Th07EffectOrbitFadeStep fadeStep =
            BuildTh07EffectOrbitFadeStep(effect->unk_17b, effect->color);
        effect->unk_17b = static_cast<i8>(fadeStep.nextFrame);
        if (!fadeStep.active)
        {
            return EFFECT_CALLBACK_RESULT_STOP;
        }
        effect->color = fadeStep.color;
        effect->vm.scaleX = fadeStep.scale;
        effect->vm.scaleY = fadeStep.scale;
    }
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectInitRadialDirection(Effect *effect)
{
    const Th07EffectRadialState state =
        BuildTh07EffectRadialState(ToTh07EffectVector3(effect->pos1), g_Rng.GetRandomF32ZeroToOne());
    ApplyTh07EffectVector3(&effect->position, state.origin);
    ApplyTh07EffectVector3(&effect->pos2, state.direction);
    return 0;
}

i32 EffectManager::EffectCallbackRadialInwardShort(Effect *effect)
{
    ApplyTh07EffectVector3(&effect->pos1,
                           BuildTh07EffectRadialInwardPosition(ToTh07EffectVector3(effect->position),
                                                               ToTh07EffectVector3(effect->pos2),
                                                               effect->timer.AsFramesFloat(),
                                                               kTh07EffectRadialInwardShortFrames,
                                                               true));
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectCallbackRadialInwardSlow(Effect *effect)
{
    ApplyTh07EffectVector3(&effect->pos1,
                           BuildTh07EffectRadialInwardPosition(ToTh07EffectVector3(effect->position),
                                                               ToTh07EffectVector3(effect->pos2),
                                                               effect->timer.AsFramesFloat(),
                                                               kTh07EffectRadialInwardSlowFrames,
                                                               false));
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectCallbackRadialOutward(Effect *effect)
{
    ApplyTh07EffectVector3(&effect->pos1,
                           BuildTh07EffectRadialOutwardPosition(ToTh07EffectVector3(effect->position),
                                                                ToTh07EffectVector3(effect->pos2),
                                                                effect->timer.AsFramesFloat()));
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectCallbackPlayerPositionWhileAnmActive(Effect *effect)
{
    const Th07EffectAnchoredPosition anchoredPosition =
        BuildTh07EffectPlayerAnchoredPosition(effect->vm.currentInstruction != NULL,
                                              ToTh07EffectVector3(g_Player.positionCenter));
    if (!anchoredPosition.active)
    {
        return EFFECT_CALLBACK_RESULT_STOP;
    }
    ApplyTh07EffectVector3(&effect->pos1, anchoredPosition.position);
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::EffectCallbackPerspectiveMotion(Effect *effect)
{
    const Th07EffectPerspectiveMotionState state =
        BuildTh07EffectPerspectiveMotionState(ToTh07EffectVector3(effect->unk_11c),
                                              ToTh07EffectVector3(effect->unk_128),
                                              ToTh07EffectVector3(effect->targetPosition),
                                              effect->vm.rotation.x,
                                              effect->vm.rotation.z,
                                              effect->renderFlags,
                                              ToTh07EffectVector3(g_Stage.th07CameraCurrentBasis.position),
                                              ToTh07EffectVector3(g_Stage.th07CameraCurrentBasis.forward));

    ApplyTh07EffectVector3(&effect->unk_11c, state.velocity);
    ApplyTh07EffectVector3(&effect->targetPosition, state.targetPosition);
    ApplyTh07EffectVector3(&effect->pos1, state.position);
    effect->vm.rotation.z = state.rotationZ;
    effect->renderFlags = state.renderFlags;
    return state.active ? EFFECT_CALLBACK_RESULT_DONE : EFFECT_CALLBACK_RESULT_STOP;
}

i32 EffectManager::EffectInitPerspective(Effect *effect, Th07EffectPerspectiveInitKind kind)
{
    // TODO(th07): map the score/cherry globals DAT_00626278/+0x88 and DAT_0062f888/f88c before
    // enabling the conditional white-script gate for 0041b0b0 and 0041b4a0.
    const bool rgbWhiteGatePassed = false;
    ApplyPerspectiveInitState(
        effect,
        BuildTh07EffectPerspectiveInitState(kind,
                                            ToTh07EffectVector3(effect->auxVector),
                                            BuildCurrentPerspectiveCameraBasis(),
                                            BuildPerspectiveInitRandoms(),
                                            g_Supervisor.effectiveFramerateMultiplier,
                                            rgbWhiteGatePassed));
    return 0;
}

i32 EffectManager::EffectCallbackCameraSnap(Effect *effect)
{
    const Th07EffectCameraSnapState state =
        BuildTh07EffectCameraSnapState(BuildCurrentPerspectiveCameraBasis());
    ApplyTh07EffectVector3(&effect->targetPosition, state.targetPosition);
    ApplyTh07EffectVector3(&effect->pos1, state.position);
    effect->renderMode = static_cast<i8>(state.renderMode);
    return state.active ? EFFECT_CALLBACK_RESULT_DONE : EFFECT_CALLBACK_RESULT_STOP;
}

i32 EffectManager::EffectInitEasedRadial(Effect *effect)
{
    Th07EffectRadialState state = {};
    if (effect->auxVector.x <= kTh07EffectEasedRadialAngleSentinel)
    {
        state = BuildTh07EffectEasedRadialStateFromRandomAngle(ToTh07EffectVector3(effect->pos1),
                                                               g_Rng.GetRandomF32ZeroToOne(),
                                                               g_Rng.GetRandomF32ZeroToOne());
    }
    else
    {
        state = BuildTh07EffectEasedRadialStateFromAngle(ToTh07EffectVector3(effect->pos1),
                                                         effect->auxVector.x,
                                                         g_Rng.GetRandomF32ZeroToOne());
    }
    ApplyTh07EffectVector3(&effect->position, state.origin);
    ApplyTh07EffectVector3(&effect->pos2, state.direction);
    return 0;
}

i32 EffectManager::EffectCallbackEasedRadial(Effect *effect)
{
    ApplyTh07EffectVector3(&effect->pos1,
                           BuildTh07EffectEasedRadialPosition(ToTh07EffectVector3(effect->position),
                                                              ToTh07EffectVector3(effect->pos2),
                                                              effect->timer.AsFramesFloat()));
    return EFFECT_CALLBACK_RESULT_DONE;
}

i32 EffectManager::DispatchUpdateCallback(Effect *effect)
{
    switch (effect->updateCallbackAddress)
    {
    case 0:
    case 0x0041c1b0:
        return EffectCallbackDone(effect);
    case 0x0041a4f0:
        return EffectCallbackLinear(effect);
    case 0x0041a750:
        return EffectCallbackOrbitingRenderMode2(effect);
    case 0x0041aaf0:
        return EffectCallbackRadialInwardShort(effect);
    case 0x0041abe0:
        return EffectCallbackPlayerPositionWhileAnmActive(effect);
    case 0x0041ac30:
        return EffectCallbackRadialInwardSlow(effect);
    case 0x0041ad10:
        return EffectCallbackRadialOutward(effect);
    case 0x0041aef0:
        return EffectCallbackPerspectiveMotion(effect);
    case 0x0041bfd0:
        return EffectCallbackEasedRadial(effect);
    case 0x0041c100:
        return EffectCallbackCameraSnap(effect);
    default:
        return EFFECT_CALLBACK_RESULT_DONE;
    }
}

i32 EffectManager::DispatchInitCallback(Effect *effect, u32 callbackAddress)
{
    switch (callbackAddress)
    {
    case 0:
        return 0;
    case 0x0041a370:
        return EffectInitRandomSplash(effect);
    case 0x0041a5a0:
        return EffectInitRandomSplashBig(effect);
    case 0x0041a730:
        return EffectInitRenderMode2(effect);
    case 0x0041aa60:
        return EffectInitRadialDirection(effect);
    case 0x0041b0b0:
        return EffectInitPerspective(effect, Th07EffectPerspectiveInitKind::Init0041b0b0);
    case 0x0041b4a0:
        return EffectInitPerspective(effect, Th07EffectPerspectiveInitKind::Init0041b4a0);
    case 0x0041b770:
        return EffectInitPerspective(effect, Th07EffectPerspectiveInitKind::Init0041b770);
    case 0x0041b9f0:
        return EffectInitPerspective(effect, Th07EffectPerspectiveInitKind::Init0041b9f0);
    case 0x0041bc20:
        return EffectInitPerspective(effect, Th07EffectPerspectiveInitKind::Init0041bc20);
    case 0x0041bec0:
        return EffectInitEasedRadial(effect);
    default:
        (void)effect;
        return 0;
    }
}

#pragma var_order(effect, idx)
Effect *EffectManager::SpawnParticles(i32 effectIdx, D3DXVECTOR3 *pos, i32 count, ZunColor color)
{
    i32 idx;
    Effect *effect;

    effect = &this->effects[this->nextIndex];
    for (idx = 0; idx < kTh07EffectPrimarySlotCount; idx++)
    {
        this->nextIndex++;
        if (this->nextIndex >= kTh07EffectPrimarySlotCount)
        {
            this->nextIndex = 0;
        }
        if (effect->inUseFlag)
        {
            effect = this->nextIndex == 0 ? &this->effects[0] : effect + 1;
            continue;
        }

        InitializeEffectSlot(effect, effectIdx, pos, color, true, NULL);
        count--;

        if (count == 0)
            break;

        effect = this->nextIndex == 0 ? &this->effects[0] : effect + 1;
    }

    return idx >= kTh07EffectPrimarySlotCount ? &this->fallbackEffect : effect;
}

Effect *EffectManager::SpawnParticlesWithAuxVector(i32 effectIdx, D3DXVECTOR3 *pos, D3DXVECTOR3 *auxVector,
                                                   i32 count, ZunColor color)
{
    i32 idx;
    Effect *effect;

    effect = &this->effects[this->nextIndex];
    for (idx = 0; idx < kTh07EffectPrimarySlotCount; idx++)
    {
        this->nextIndex++;
        if (this->nextIndex >= kTh07EffectPrimarySlotCount)
        {
            this->nextIndex = 0;
        }
        if (effect->inUseFlag)
        {
            effect = this->nextIndex == 0 ? &this->effects[0] : effect + 1;
            continue;
        }

        InitializeEffectSlot(effect, effectIdx, pos, color, false, auxVector);
        count--;

        if (count == 0)
            break;

        effect = this->nextIndex == 0 ? &this->effects[0] : effect + 1;
    }

    return idx >= kTh07EffectPrimarySlotCount ? &this->fallbackEffect : effect;
}

Effect *EffectManager::SpawnDedicatedParticle(i32 effectIdx, D3DXVECTOR3 *pos, i32 dedicatedSlot, i32 spawnFlag,
                                              ZunColor color)
{
    (void)spawnFlag;
    if (dedicatedSlot < 0 || dedicatedSlot >= kTh07EffectDedicatedSlotCount)
    {
        return &this->fallbackEffect;
    }

    Effect *effect = &this->effects[kTh07EffectDedicatedSlotBaseIndex + dedicatedSlot];
    InitializeEffectSlot(effect, effectIdx, pos, color, true, NULL);
    effect->renderMode = 0;
    return effect;
}

void EffectManager::ResetDrawLists()
{
    this->activeEffects = 0;
    this->drawListScrollTail = &this->drawListScrollSentinel;
    this->drawListIndexedTail = &this->drawListIndexedSentinel;
    this->drawListNoScrollTail = &this->drawListNoScrollSentinel;
    this->drawListScrollFlaggedTail = &this->drawListScrollFlaggedSentinel;
    this->drawListScrollSentinel.nextDraw = NULL;
    this->drawListIndexedSentinel.nextDraw = NULL;
    this->drawListNoScrollSentinel.nextDraw = NULL;
    this->drawListScrollFlaggedSentinel.nextDraw = NULL;
}

void EffectManager::LinkEffectForDraw(Effect *effect)
{
    effect->nextDraw = NULL;
    if (effect->renderMode == 1 || effect->renderMode == 3)
    {
        this->drawListIndexedTail->nextDraw = effect;
        this->drawListIndexedTail = effect;
    }
    else if (effect->renderMode == 0)
    {
        if (((effect->renderFlags >> 4) & 1) == 0)
        {
            this->drawListScrollTail->nextDraw = effect;
            this->drawListScrollTail = effect;
        }
        else
        {
            this->drawListScrollFlaggedTail->nextDraw = effect;
            this->drawListScrollFlaggedTail = effect;
        }
    }
    else
    {
        this->drawListNoScrollTail->nextDraw = effect;
        this->drawListNoScrollTail = effect;
    }
}

ChainCallbackResult EffectManager::OnUpdate(EffectManager *mgr)
{
    i32 effectIdx;
    Effect *effect;

    mgr->ResetDrawLists();
    for (effect = &mgr->effects[0], effectIdx = 0; effectIdx < kTh07EffectUpdateSlotCount; effectIdx++, effect++)
    {
        if (effect->inUseFlag == 0)
        {
            continue;
        }

        mgr->activeEffects++;
        if ((effect->updateCallbackAddress != 0 && EffectManager::DispatchUpdateCallback(effect) != EFFECT_CALLBACK_RESULT_DONE) ||
            g_AnmManager->ExecuteScript(&effect->vm) != 0)
        {
            effect->inUseFlag = 0;
            continue;
        }

        effect->timer.Tick();
        mgr->LinkEffectForDraw(effect);
    }

    mgr->frameCounter++;
    if ((mgr->frameCounter % 300 == 100) && g_GameManager.guiScore != 0)
    {
        return CHAIN_CALLBACK_RESULT_BREAK;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ChainCallbackResult EffectManager::OnDraw(EffectManager *mgr)
{
    DrawScrollList(mgr->drawListScrollSentinel.nextDraw);
    DrawNoScrollList(mgr->drawListNoScrollSentinel.nextDraw);
    DrawScrollList(mgr->drawListScrollFlaggedSentinel.nextDraw);
    DrawIndexedList(mgr->drawListIndexedSentinel.nextDraw);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

ZunResult EffectManager::AddedCallback(EffectManager *mgr)
{
    const Th07EffectStageAnmLoad *stageLoad;

    mgr->Reset();
    stageLoad = FindTh07EffectStageAnmLoad(g_GameManager.currentStage);
    if (stageLoad == NULL)
    {
        return ZUN_SUCCESS;
    }

    for (const Th07EffectAnmLoad &load : stageLoad->loads)
    {
        if (load.archivePath.empty())
        {
            continue;
        }
        if (g_AnmManager->LoadAnm(load.anmFileSlot, const_cast<char *>(load.archivePath.data()), load.scriptBase) !=
            ZUN_SUCCESS)
        {
            return ZUN_ERROR;
        }
    }
    return ZUN_SUCCESS;
}

ZunResult EffectManager::DeletedCallback(EffectManager *p)
{
    (void)p;
    g_AnmManager->ReleaseAnm(0x11);
    g_AnmManager->ReleaseAnm(0x12);
    g_AnmManager->ReleaseAnm(0x13);
    g_AnmManager->ReleaseAnm(0x14);

    return ZUN_SUCCESS;
}

ZunResult EffectManager::RegisterChain()
{
    EffectManager *mgr = &g_EffectManager;
    mgr->Reset();

    g_EffectManagerCalcChain.callback = (ChainCallback)mgr->OnUpdate;
    g_EffectManagerCalcChain.addedCallback = (ChainAddedCallback)mgr->AddedCallback;
    g_EffectManagerCalcChain.deletedCallback = (ChainAddedCallback)mgr->DeletedCallback;
    g_EffectManagerCalcChain.arg = mgr;

    if (g_Chain.AddToCalcChain(&g_EffectManagerCalcChain, TH_CHAIN_PRIO_CALC_EFFECTMANAGER))
    {
        return ZUN_ERROR;
    }

    g_EffectManagerDrawChain.callback = (ChainCallback)mgr->OnDraw;
    g_EffectManagerDrawChain.addedCallback = NULL;
    g_EffectManagerDrawChain.deletedCallback = NULL;
    g_EffectManagerDrawChain.arg = mgr;
    g_Chain.AddToDrawChain(&g_EffectManagerDrawChain, TH_CHAIN_PRIO_DRAW_EFFECTMANAGER);

    return ZUN_SUCCESS;
}

void EffectManager::CutChain()
{
    g_Chain.Cut(&g_EffectManagerCalcChain);
    g_Chain.Cut(&g_EffectManagerDrawChain);
}
}; // namespace th07
