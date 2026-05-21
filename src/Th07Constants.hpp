#pragma once

#include <array>
#include <cstdint>

namespace th07 {

enum class Th07Difficulty : std::uint8_t {
    Easy = 0,
    Normal = 1,
    Hard = 2,
    Lunatic = 3,
    Extra = 4,
    Phantasm = 5,
};

enum class Th07PlayableCharacter : std::uint8_t {
    Reimu = 0,
    Marisa = 1,
    Sakuya = 2,
};

enum class Th07ShotType : std::uint8_t {
    ReimuA = 0,
    ReimuB = 1,
    MarisaA = 2,
    MarisaB = 3,
    SakuyaA = 4,
    SakuyaB = 5,
};

struct Vec2 {
    float x;
    float y;
};

struct PlayerSystemConstants {
    Vec2 hitboxHalf;
    Vec2 itemGrabHalf;
    float grazePadding;
    int maxPower;
};

struct CherrySystemConstants {
    int maxCherry;
    int borderStart;
    int borderDurationFrames;
    int borderBreakPenalty;
};

extern const PlayerSystemConstants kPlayerSystem;
extern const CherrySystemConstants kCherrySystem;
extern const std::array<const char*, 8> kStageEclFiles;
extern const std::array<const char*, 8> kStageStdFiles;
extern const std::array<const char*, 12> kShotFiles;

bool IsPointOfCollectionAvailable(int power, bool borderActive);

} // namespace th07

