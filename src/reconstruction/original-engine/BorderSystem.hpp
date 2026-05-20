#pragma once

#include <array>

namespace th07
{
struct BorderHudStringAnchor
{
    const char *text;
    const char *ghidraStringAddress;
};

constexpr int kBorderDurationFrames = 540;
constexpr int kBorderBreakPenalty = 100000;

extern const std::array<BorderHudStringAnchor, 5> kBorderHudStringAnchors;
} // namespace th07
