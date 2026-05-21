#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

namespace th07 {

inline constexpr std::size_t kShtHeaderSize = 0x34;
inline constexpr std::size_t kShtPowerLevelEntrySize = 0x8;
inline constexpr std::size_t kShtShotRecordSize = 0x34;

struct ShtHeader {
    std::uint16_t formatId;
    std::uint16_t powerLevelCount;
    float unknownFloat04;
    std::int32_t deathbombWindowFrames;
    std::array<float, 10> tuningFloats;

    float hitboxDiagonalExtent() const;
    float grazeDiagonalExtent() const;
    float orthogonalMovementSpeed() const;
    float focusedOrthogonalMovementSpeed() const;
    float diagonalMovementSpeed() const;
    float focusedDiagonalMovementSpeed() const;
};

struct ShtCallbackIds {
    std::int32_t onSpawn;
    std::int32_t onUpdate;
    std::int32_t onDraw;
    std::int32_t onCollision;
};

struct ShtShotRecord {
    std::int16_t waitBetweenBullets;
    std::int16_t bulletFrame;
    float motionX;
    float motionY;
    float sizeX;
    float sizeY;
    float direction;
    float velocity;
    std::uint16_t damage;
    std::uint8_t spawnPositionIndex;
    std::uint8_t bulletType;
    std::int16_t anmScriptIndex;
    std::int16_t soundIndex;
    ShtCallbackIds callbacks;
};

struct ShtPowerLevel {
    std::uint32_t recordOffset;
    std::int32_t powerThreshold;
    std::uint32_t terminatorOffset;
    std::vector<ShtShotRecord> shots;
};

struct ShtFile {
    ShtHeader header;
    std::vector<ShtPowerLevel> powerLevels;
};

enum class ShtCallbackRole {
    OnSpawn,
    OnUpdate,
    OnDraw,
    OnCollision,
};

enum class ShtCallbackSemantic {
    None,
    PeriodicSpawn,
    Mode1OptionSpawn,
    Mode3TrailSpawn,
    RandomizedAnglePeriodicSpawn,
    FixedAnglePeriodicSpawn,
    HomingUpdateSpeed10,
    HomingUpdateSpeed18,
    VerticalAngleDriftUpdate,
    Mode1OptionUpdate,
    Mode3TrailUpdate,
    TrailDraw,
    ImpactScaleAndSound,
    CollisionCounterSound,
};

struct ShtCallbackTarget {
    std::int32_t id;
    std::uint32_t originalAddress;
    std::string_view ghidraName;
    ShtCallbackSemantic semantic;
};

struct ResolvedShtCallbacks {
    const ShtCallbackTarget *spawn;
    const ShtCallbackTarget *update;
    const ShtCallbackTarget *draw;
    const ShtCallbackTarget *collision;
};

inline constexpr std::uint32_t kShtOnSpawnCallbackTableAddress = 0x0049ecb0;
inline constexpr std::uint32_t kShtOnUpdateCallbackTableAddress = 0x0049ecc8;
inline constexpr std::uint32_t kShtOnDrawCallbackTableAddress = 0x0049ece0;
inline constexpr std::uint32_t kShtOnCollisionCallbackTableAddress = 0x0049ece8;

extern const std::array<ShtCallbackTarget, 6> kShtOnSpawnCallbacks;
extern const std::array<ShtCallbackTarget, 6> kShtOnUpdateCallbacks;
extern const std::array<ShtCallbackTarget, 2> kShtOnDrawCallbacks;
extern const std::array<ShtCallbackTarget, 3> kShtOnCollisionCallbacks;

ShtFile ParseShtFile(std::span<const std::uint8_t> bytes);
ShtFile LoadShtFile(const std::filesystem::path &path);
std::size_t CountShtShotRecords(const ShtFile &file);
const ShtPowerLevel *FindShtPowerLevelForPower(const ShtFile &file, int power);
const ShtCallbackTarget *FindShtCallback(ShtCallbackRole role, std::int32_t id);
ResolvedShtCallbacks ResolveShtCallbacks(const ShtShotRecord &shot);
std::string_view ShtCallbackSemanticName(ShtCallbackSemantic semantic);

} // namespace th07
