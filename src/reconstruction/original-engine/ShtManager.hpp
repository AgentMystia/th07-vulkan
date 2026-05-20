#pragma once

#include <array>

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

struct ShtFileBinding
{
    Th07Character character;
    Th07ShotLetter shot;
    bool focused;
    const char *archivePath;
    const char *ghidraStringAddress;
};

extern const std::array<ShtFileBinding, 12> kShtFileBindings;
const ShtFileBinding *FindShtBinding(Th07Character character, Th07ShotLetter shot, bool focused);
} // namespace th07
