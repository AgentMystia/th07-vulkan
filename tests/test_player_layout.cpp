#include "PlayerLayout.hpp"

#include <cassert>

int main()
{
    static_assert(th07::kPlayerAnmVmSlotSize == 0x024c);
    static_assert(th07::kPlayerOriginalAnmVmPrefixSize == th07::kPlayerPositionOffset);
    static_assert(th07::kPlayerFourthAnmVmOffset + th07::kPlayerAnmVmSlotSize == th07::kPlayerPositionOffset);
    static_assert(th07::kPlayerHitboxMinOffset + th07::kPlayerVectorSize == th07::kPlayerHitboxMaxOffset);
    static_assert(th07::kPlayerGrazeBoxMinOffset + th07::kPlayerVectorSize == th07::kPlayerGrazeBoxMaxOffset);
    static_assert(th07::kPlayerGrabItemBoxMinOffset + th07::kPlayerVectorSize == th07::kPlayerGrabItemBoxMaxOffset);
    static_assert(th07::kPlayerHitboxSizeOffset + th07::kPlayerVectorSize == th07::kPlayerGrazeBoxSizeOffset);
    static_assert(th07::kPlayerGrazeBoxSizeOffset + th07::kPlayerVectorSize == th07::kPlayerGrabItemSizeOffset);
    static_assert(th07::kPlayerLeftOptionPositionOffset == th07::kPlayerGrabItemSizeOffset + th07::kPlayerVectorSize);
    static_assert(th07::kPlayerRightOptionPositionOffset == th07::kPlayerLeftOptionPositionOffset +
                  th07::kPlayerVectorSize);
    static_assert(th07::kPlayerMovementDeltaYOffset == 0x09d0);
    static_assert(th07::kPlayerBombVerticalSpeedMultiplierOffset ==
                  th07::kPlayerBombHorizontalSpeedMultiplierOffset + 4);

    assert(th07::kPlayerObjectClearSize == 0x0b7e78);
    return 0;
}
