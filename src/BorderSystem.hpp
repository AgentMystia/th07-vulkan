#pragma once

#include "inttypes.hpp"

#include <array>

namespace th07
{
inline constexpr u32 kTh07BorderBonusStringAddress = 0x004983a0;
inline constexpr u32 kTh07SupernaturalBorderStringAddress = 0x004983c8;
inline constexpr u32 kTh07PlayerPenaltyLowStringAddress = 0x00498410;
inline constexpr u32 kTh07PlayerPenaltyHalfStringAddress = 0x00498424;
inline constexpr u32 kTh07PhantasmRankStringAddress = 0x00498438;
inline constexpr std::size_t kTh07BorderHudStringAnchorCount = 5;

struct BorderHudStringAnchor
{
    const char *text;
    u32 ghidraStringAddress;
};

constexpr int kBorderDurationFrames = 540;
constexpr int kBorderBreakPenalty = 100000;

extern const std::array<BorderHudStringAnchor, kTh07BorderHudStringAnchorCount> kBorderHudStringAnchors;
} // namespace th07
