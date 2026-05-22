#pragma once

#include "diffbuild.hpp"
#include "inttypes.hpp"

#include <cstddef>

namespace th07
{
inline constexpr u32 kTh07RngNextU16FunctionAddress = 0x00431870;
inline constexpr u32 kTh07RngNextU32FunctionAddress = 0x004318d0;
inline constexpr u32 kTh07RngFloatZeroToOneFunctionAddress = 0x00431900;
inline constexpr u32 kTh07NormalizeAngleFunctionAddress = 0x00431930;

inline constexpr u32 kTh07RngFloatDivisorAddress = 0x00498b28;
inline constexpr u32 kTh07NormalizeAngleLowerBoundAddress = 0x00498a5c;
inline constexpr u32 kTh07NormalizeAnglePeriodAddress = 0x00498a60;
inline constexpr u32 kTh07NormalizeAngleUpperBoundAddress = 0x00498a64;

inline constexpr u16 kTh07RngXorConstant = 0x9630;
inline constexpr u16 kTh07RngAddConstant = 0x9aad;
inline constexpr u16 kTh07RngCarryMask = 0xc000;
inline constexpr u32 kTh07RngCarryShift = 14;
inline constexpr u32 kTh07RngSeedMultiplier = 4;

inline constexpr u32 kTh07RngSeedOffset = 0x00;
inline constexpr u32 kTh07RngGenerationCountOffset = 0x04;
inline constexpr u32 kTh07RngStateSize = 0x08;

inline constexpr u32 kTh07RngFloatDivisorBits = 0x4f800000;
inline constexpr long double kTh07RngFloatDivisor = 4294967296.0L;
inline constexpr u32 kTh07NormalizeAngleLowerBoundBits = 0xc0490fdb;
inline constexpr u32 kTh07NormalizeAnglePeriodBits = 0x40c90fdb;
inline constexpr u32 kTh07NormalizeAngleUpperBoundBits = 0x40490fdb;
inline constexpr f32 kTh07NormalizeAngleLowerBound = -3.1415927f;
inline constexpr f32 kTh07NormalizeAnglePeriod = 6.2831855f;
inline constexpr f32 kTh07NormalizeAngleUpperBound = 3.1415927f;
inline constexpr i32 kTh07NormalizeAngleLoopGuard = 0x11;

u16 NextTh07RngSeed(u16 seed);
u32 BuildTh07RngU32(u16 high, u16 low);
f32 ScaleTh07RngU32ToUnit(u32 value);
f32 NormalizeTh07AngleWithAddend(f32 value, f32 addend);

struct Rng
{
    u16 seed;
    u32 generationCount;

    u16 GetRandomU16();
    u32 GetRandomU32();
    f32 GetRandomF32ZeroToOne();

    void Initialize(u16 seed)
    {
        this->seed = seed;
        this->generationCount = 0;
    }

    u16 GetRandomU16InRange(u16 range)
    {
        return range != 0 ? this->GetRandomU16() % range : 0;
    }

    u32 GetRandomU32InRange(u32 range)
    {
        return range != 0 ? this->GetRandomU32() % range : 0;
    }

    f32 GetRandomF32InRange(f32 range)
    {
        return this->GetRandomF32ZeroToOne() * range;
    }
};

static_assert(offsetof(Rng, seed) == kTh07RngSeedOffset);
static_assert(offsetof(Rng, generationCount) == kTh07RngGenerationCountOffset);
static_assert(sizeof(Rng) == kTh07RngStateSize);

DIFFABLE_EXTERN(Rng, g_Rng);
}; // namespace th07
