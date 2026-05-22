#include "Rng.hpp"

#include <cassert>
#include <cmath>
#include <cstddef>

namespace
{
bool NearlyEqual(float lhs, float rhs)
{
    return std::fabs(lhs - rhs) < 0.00001f;
}
} // namespace

int main()
{
    static_assert(th07::kTh07RngNextU16FunctionAddress == 0x00431870);
    static_assert(th07::kTh07RngNextU32FunctionAddress == 0x004318d0);
    static_assert(th07::kTh07RngFloatZeroToOneFunctionAddress == 0x00431900);
    static_assert(th07::kTh07NormalizeAngleFunctionAddress == 0x00431930);
    static_assert(th07::kTh07RngFloatDivisorAddress == 0x00498b28);
    static_assert(th07::kTh07NormalizeAngleLowerBoundAddress == 0x00498a5c);
    static_assert(th07::kTh07NormalizeAnglePeriodAddress == 0x00498a60);
    static_assert(th07::kTh07NormalizeAngleUpperBoundAddress == 0x00498a64);
    static_assert(th07::kTh07RngXorConstant == 0x9630);
    static_assert(th07::kTh07RngAddConstant == 0x9aad);
    static_assert(th07::kTh07RngCarryMask == 0xc000);
    static_assert(th07::kTh07RngCarryShift == 14);
    static_assert(th07::kTh07RngSeedMultiplier == 4);
    static_assert(th07::kTh07RngFloatDivisorBits == 0x4f800000);
    static_assert(th07::kTh07NormalizeAngleLowerBoundBits == 0xc0490fdb);
    static_assert(th07::kTh07NormalizeAnglePeriodBits == 0x40c90fdb);
    static_assert(th07::kTh07NormalizeAngleUpperBoundBits == 0x40490fdb);
    static_assert(offsetof(th07::Rng, seed) == th07::kTh07RngSeedOffset);
    static_assert(offsetof(th07::Rng, generationCount) == th07::kTh07RngGenerationCountOffset);
    static_assert(sizeof(th07::Rng) == th07::kTh07RngStateSize);

    th07::Rng rng {};
    rng.Initialize(0);
    assert(rng.GetRandomU16() == 0xc374);
    assert(rng.generationCount == 1);
    assert(rng.GetRandomU16() == 0xbfc7);
    assert(rng.generationCount == 2);
    assert(th07::NextTh07RngSeed(0xbfc7) == 0x1293);

    rng.Initialize(0);
    assert(rng.GetRandomU32() == 0xc374bfc7);
    assert(rng.generationCount == 2);
    assert(th07::BuildTh07RngU32(0x1234, 0xabcd) == 0x1234abcd);

    assert(th07::ScaleTh07RngU32ToUnit(0) == 0.0f);
    assert(th07::ScaleTh07RngU32ToUnit(0x80000000u) == 0.5f);
    assert(th07::ScaleTh07RngU32ToUnit(0x40000000u) == 0.25f);
    assert(th07::ScaleTh07RngU32ToUnit(0xffffffffu) == 1.0f);

    assert(NearlyEqual(th07::NormalizeTh07AngleWithAddend(4.0f, 0.0f), 4.0f - th07::kTh07NormalizeAnglePeriod));
    assert(NearlyEqual(th07::NormalizeTh07AngleWithAddend(-4.0f, 0.0f), -4.0f + th07::kTh07NormalizeAnglePeriod));
    assert(NearlyEqual(th07::NormalizeTh07AngleWithAddend(1.0f, 2.5f),
                       3.5f - th07::kTh07NormalizeAnglePeriod));

    return 0;
}
