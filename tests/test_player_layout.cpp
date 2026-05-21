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
    static_assert(th07::kPlayerModeSoundTimerOffset == 0x2400);
    static_assert(th07::kPlayerModeStateOffset == 0x2408);
    static_assert(th07::kPlayerOptionStateOffset == 0x240a);
    static_assert(th07::kPlayerModeDrawEnabledOffset == th07::kPlayerOptionStateOffset);
    static_assert(th07::kPlayerFocusHeldOffset == 0x240b);
    static_assert(th07::kPlayerModeTransitionRequestOffset == 0x240d);
    static_assert(th07::kPlayerOptionInterpolationPreviousFrameOffset == 0x2410);
    static_assert(th07::kPlayerOptionInterpolationSubframeOffset == 0x2414);
    static_assert(th07::kPlayerOptionInterpolationFrameOffset == 0x2418);
    static_assert(th07::kPlayerBombCommonEffectXOffset == 0x16a00);
    static_assert(th07::kPlayerBombCommonEffectYOffset == 0x16a04);
    static_assert(th07::kPlayerBombCommonEffectDurationOffset == 0x16a08);
    static_assert(th07::kPlayerMode4EffectStartXOffset == 0x16a0c);
    static_assert(th07::kPlayerMode4EffectStartYOffset == 0x16a10);
    static_assert(th07::kPlayerMode4EffectStartDurationOffset == 0x16a14);
    static_assert(th07::kPlayerBombCommonEffectActivePointerOffset == 0x0b7e68);
    static_assert(th07::kPlayerModeTransitionEffectActivePointerOffset == 0x0b7e6c);
    static_assert(th07::kPlayerBombCommonEffectDurationGlobalAddress == 0x004d44e0);
    static_assert(th07::kPlayerModeTransitionEffectActivePointerOffset + 0x0c ==
                  th07::kPlayerObjectClearSize);
    static_assert(th07::kPlayerOptionStateHidden == 0);
    static_assert(th07::kPlayerOptionStateUnfocused == 1);
    static_assert(th07::kPlayerOptionStateFocusing == 2);
    static_assert(th07::kPlayerOptionStateFocused == 3);
    static_assert(th07::kPlayerOptionStateUnfocusing == 4);

    assert(th07::kPlayerObjectClearSize == 0x0b7e78);
    return 0;
}
