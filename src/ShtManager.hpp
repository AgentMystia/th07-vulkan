#pragma once

#include "Th07ResourceManifest.hpp"

namespace th07
{
enum class Th07Character
{
    Reimu,
    Marisa,
    Sakuya,
};

enum class Th07ShotLetter
{
    A,
    B,
};

extern const std::array<ShtFileBinding, 12> &kShtFileBindings;
Th07ShotType ToShotType(Th07Character character, Th07ShotLetter shot);
const ShtFileBinding *FindShtBinding(Th07Character character, Th07ShotLetter shot, bool focused);
} // namespace th07
