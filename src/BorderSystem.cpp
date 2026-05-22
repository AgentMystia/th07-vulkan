#include "BorderSystem.hpp"

namespace th07
{
const std::array<BorderHudStringAnchor, kTh07BorderHudStringAnchorCount> kBorderHudStringAnchors = {
    BorderHudStringAnchor{"Border Bonus %7d", kTh07BorderBonusStringAddress},
    BorderHudStringAnchor{"Supernatural Border!!", kTh07SupernaturalBorderStringAddress},
    BorderHudStringAnchor{"Player Penalty*0.2", kTh07PlayerPenaltyLowStringAddress},
    BorderHudStringAnchor{"Player Penalty*0.5", kTh07PlayerPenaltyHalfStringAddress},
    BorderHudStringAnchor{"Phantasm Rank*2.0", kTh07PhantasmRankStringAddress},
};
} // namespace th07
