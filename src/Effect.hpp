#pragma once

#include "AnmVm.hpp"
#include "ZunTimer.hpp"
#include "inttypes.hpp"

namespace th07
{
enum EffectCallbackResult
{
    EFFECT_CALLBACK_RESULT_STOP = 0,
    EFFECT_CALLBACK_RESULT_DONE = 1
};

struct Effect;

typedef i32 (*EffectUpdateCallback)(Effect *);
typedef i32 (*EffectInitCallback)(Effect *);

struct Effect
{
    AnmVm vm;
    u8 th07VmTail_110_1b8[0xa8];
    ZunColor color;
    u32 unk_1bc;
    u32 renderFlags;
    u32 unk_1c4;
    D3DXVECTOR3 drawPosition;
    u32 unk_1d4;
    i16 anmScript;
    i16 unk_1da;
    u8 unk_1dc_24c[0x70];
    D3DXVECTOR3 pos1;
    D3DXVECTOR3 auxVector;
    D3DXVECTOR3 unk_11c;
    D3DXVECTOR3 unk_128;
    D3DXVECTOR3 targetPosition;
    D3DXVECTOR3 position;
    D3DXVECTOR3 pos2;
    D3DXQUATERNION quaternion;
    f32 unk_15c;
    f32 angleRelated;
    ZunTimer timer;
    u32 unk_2c4;
    u32 updateCallbackAddress;
    i8 inUseFlag;
    i8 effectId;
    i8 unk_17a;
    i8 unk_17b;
    i8 renderMode;
    u8 unk_2d1_2d3[3];
    Effect *nextDraw;
};
ZUN_ASSERT_SIZE(Effect, 0x2d8);

struct EffectInfo
{
    i32 anmIdx;
    u32 updateCallbackAddress;
    u32 initCallbackAddress;
};
ZUN_ASSERT_SIZE(EffectInfo, 0xc);
}; // namespace th07
