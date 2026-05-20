#include "ShtManager.hpp"

namespace th07
{
const std::array<ShtFileBinding, 12> kShtFileBindings = {
    ShtFileBinding{Th07Character::Reimu, Th07ShotLetter::A, false, "data/ply00a.sht", "00496bb4"},
    ShtFileBinding{Th07Character::Reimu, Th07ShotLetter::B, false, "data/ply00b.sht", "00496ba4"},
    ShtFileBinding{Th07Character::Marisa, Th07ShotLetter::A, false, "data/ply01a.sht", "00496b94"},
    ShtFileBinding{Th07Character::Marisa, Th07ShotLetter::B, false, "data/ply01b.sht", "00496b84"},
    ShtFileBinding{Th07Character::Sakuya, Th07ShotLetter::A, false, "data/ply02a.sht", "00496b74"},
    ShtFileBinding{Th07Character::Sakuya, Th07ShotLetter::B, false, "data/ply02b.sht", "00496b64"},
    ShtFileBinding{Th07Character::Reimu, Th07ShotLetter::A, true, "data/ply00as.sht", "00496b50"},
    ShtFileBinding{Th07Character::Reimu, Th07ShotLetter::B, true, "data/ply00bs.sht", "00496b3c"},
    ShtFileBinding{Th07Character::Marisa, Th07ShotLetter::A, true, "data/ply01as.sht", "00496b28"},
    ShtFileBinding{Th07Character::Marisa, Th07ShotLetter::B, true, "data/ply01bs.sht", "00496b14"},
    ShtFileBinding{Th07Character::Sakuya, Th07ShotLetter::A, true, "data/ply02as.sht", "00496b00"},
    ShtFileBinding{Th07Character::Sakuya, Th07ShotLetter::B, true, "data/ply02bs.sht", "00496aec"},
};

const ShtFileBinding *FindShtBinding(Th07Character character, Th07ShotLetter shot, bool focused)
{
    for (const ShtFileBinding &binding : kShtFileBindings)
    {
        if (binding.character == character && binding.shot == shot && binding.focused == focused)
        {
            return &binding;
        }
    }
    return nullptr;
}
} // namespace th07
