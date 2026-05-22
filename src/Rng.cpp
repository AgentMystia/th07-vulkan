#include "Rng.hpp"

namespace th07
{
DIFFABLE_STATIC(Rng, g_Rng);

u16 NextTh07RngSeed(u16 seed)
{
    const u16 mixed = static_cast<u16>((seed ^ kTh07RngXorConstant) + kTh07RngAddConstant);
    return static_cast<u16>(((mixed & kTh07RngCarryMask) >> kTh07RngCarryShift) +
                            mixed * kTh07RngSeedMultiplier);
}

u32 BuildTh07RngU32(u16 high, u16 low)
{
    return (static_cast<u32>(high) << 16) | low;
}

f32 ScaleTh07RngU32ToUnit(u32 value)
{
    return static_cast<f32>(static_cast<long double>(value) / kTh07RngFloatDivisor);
}

f32 NormalizeTh07AngleWithAddend(f32 value, f32 addend)
{
    i32 iterations = 0;
    value += addend;

    do {
        if (value <= kTh07NormalizeAngleUpperBound) {
            break;
        }
        value -= kTh07NormalizeAnglePeriod;
        const bool keepGoing = iterations < kTh07NormalizeAngleLoopGuard;
        iterations++;
        if (!keepGoing) {
            break;
        }
    } while (true);

    do {
        if (kTh07NormalizeAngleLowerBound <= value) {
            break;
        }
        value += kTh07NormalizeAnglePeriod;
        const bool keepGoing = iterations < kTh07NormalizeAngleLoopGuard;
        iterations++;
        if (!keepGoing) {
            break;
        }
    } while (true);

    return value;
}

u16 Rng::GetRandomU16(void)
{
    this->seed = NextTh07RngSeed(this->seed);
    this->generationCount++;
    return this->seed;
}

u32 Rng::GetRandomU32(void)
{
    const u16 high = GetRandomU16();
    const u16 low = GetRandomU16();
    return BuildTh07RngU32(high, low);
}

f32 Rng::GetRandomF32ZeroToOne(void)
{
    return ScaleTh07RngU32ToUnit(GetRandomU32());
}
}; // namespace th07
