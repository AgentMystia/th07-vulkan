#include "ShtManager.hpp"

namespace th07
{
const std::array<ShtFileBinding, 12> &kShtFileBindings = kTh07ShtFiles;

Th07ShotType ToShotType(Th07Character character, Th07ShotLetter shot)
{
    switch (character)
    {
    case Th07Character::Reimu:
        return shot == Th07ShotLetter::A ? Th07ShotType::ReimuA : Th07ShotType::ReimuB;
    case Th07Character::Marisa:
        return shot == Th07ShotLetter::A ? Th07ShotType::MarisaA : Th07ShotType::MarisaB;
    case Th07Character::Sakuya:
        return shot == Th07ShotLetter::A ? Th07ShotType::SakuyaA : Th07ShotType::SakuyaB;
    }
    return Th07ShotType::ReimuA;
}

const ShtFileBinding *FindShtBinding(Th07Character character, Th07ShotLetter shot, bool focused)
{
    return FindShtFile(ToShotType(character, shot), focused);
}
} // namespace th07
