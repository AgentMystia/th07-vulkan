#pragma once

#include <array>

namespace th07
{
struct CherryHudStringAnchor
{
    const char *text;
    const char *ghidraStringAddress;
};

constexpr int kCherryMax = 500000;
constexpr int kCherryBorderStart = 50000;
constexpr int kCherryDisplayScale = 10;

extern const std::array<CherryHudStringAnchor, 2> kCherryHudStringAnchors;
} // namespace th07
