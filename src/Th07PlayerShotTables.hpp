#pragma once

#include "PlayerLayout.hpp"
#include "Th07Constants.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace th07 {

inline constexpr std::size_t kTh07PlayerShotFileCount = 12;
inline constexpr std::size_t kTh07PlayerShtPowerLevelCount = 9;
inline constexpr std::uint16_t kTh07PlayerShtShotRecordCount = 582;
inline constexpr std::array<std::int32_t, kTh07PlayerShtPowerLevelCount> kTh07PlayerShtPowerThresholds = {
    8, 16, 32, 48, 64, 80, 96, 128, 999,
};

inline constexpr std::array<int, 6> kTh07ShtSpawnCallbackUsage = {386, 9, 18, 9, 51, 109};
inline constexpr std::array<int, 6> kTh07ShtUpdateCallbackUsage = {461, 36, 17, 41, 18, 9};
inline constexpr std::array<int, 2> kTh07ShtDrawCallbackUsage = {573, 9};
inline constexpr std::array<int, 3> kTh07ShtCollisionCallbackUsage = {514, 41, 27};
inline constexpr std::uint32_t kTh07ShtLoadFunctionAddress = 0x00442b70;
inline constexpr std::uint32_t kTh07CurrentShotTypeGlobalAddress = 0x0062f647;
inline constexpr std::uint32_t kTh07ShtUnfocusedPathTableAddress = 0x0049f530;
inline constexpr std::uint32_t kTh07ShtFocusedPathTableAddress = 0x0049f548;
inline constexpr std::size_t kTh07ShtPathTableEntryCount = 6;
inline constexpr std::size_t kTh07ShtPathTableEntryStride = 4;
inline constexpr std::uint32_t kTh07PlayerInitUnfocusedShotTypeLoadAddress = 0x004423e9;
inline constexpr std::uint32_t kTh07PlayerInitUnfocusedShotDataPointerAddAddress = 0x004423f3;
inline constexpr std::uint32_t kTh07PlayerInitUnfocusedShtPathLoadAddress = 0x004423f9;
inline constexpr std::uint32_t kTh07PlayerInitUnfocusedShtLoadCallAddress = 0x00442400;
inline constexpr std::uint32_t kTh07PlayerInitFocusedShotTypeLoadAddress = 0x00442411;
inline constexpr std::uint32_t kTh07PlayerInitFocusedShotDataPointerAddAddress = 0x0044241b;
inline constexpr std::uint32_t kTh07PlayerInitFocusedShtPathLoadAddress = 0x00442420;
inline constexpr std::uint32_t kTh07PlayerInitFocusedShtLoadCallAddress = 0x00442429;

inline constexpr std::array<std::uint32_t, kTh07PlayerShotFileCount> kTh07ShtPathStringAddresses = {
    0x00496bb4, 0x00496ba4, 0x00496b94, 0x00496b84, 0x00496b74, 0x00496b64,
    0x00496b50, 0x00496b3c, 0x00496b28, 0x00496b14, 0x00496b00, 0x00496aec,
};

enum class Th07PlayerShotDataLoadKind {
    Unfocused,
    Focused,
};

struct Th07PlayerShotDataLoadContract {
    Th07PlayerShotDataLoadKind kind;
    std::uint32_t pathTableAddress;
    std::uint32_t ownerPointerOffset;
    std::uint32_t shotTypeLoadAddress;
    std::uint32_t ownerPointerAddAddress;
    std::uint32_t pathTableLoadAddress;
    std::uint32_t loaderCallAddress;
    std::string_view evidence;
};

inline constexpr std::array<Th07PlayerShotDataLoadContract, 2> kTh07PlayerShotDataLoadContracts = {{
    {Th07PlayerShotDataLoadKind::Unfocused,
     kTh07ShtUnfocusedPathTableAddress,
     kPlayerShotDataPointerOffset,
     kTh07PlayerInitUnfocusedShotTypeLoadAddress,
     kTh07PlayerInitUnfocusedShotDataPointerAddAddress,
     kTh07PlayerInitUnfocusedShtPathLoadAddress,
     kTh07PlayerInitUnfocusedShtLoadCallAddress,
     "FUN_004423e0 entry; th07.exe@004423e9..00442400"},
    {Th07PlayerShotDataLoadKind::Focused,
     kTh07ShtFocusedPathTableAddress,
     kPlayerFocusedShotDataPointerOffset,
     kTh07PlayerInitFocusedShotTypeLoadAddress,
     kTh07PlayerInitFocusedShotDataPointerAddAddress,
     kTh07PlayerInitFocusedShtPathLoadAddress,
     kTh07PlayerInitFocusedShtLoadCallAddress,
     "FUN_004423e0 entry; th07.exe@00442411..00442429"},
}};

struct Th07PlayerShotSummary {
    Th07ShotType shotType;
    bool focused;
    std::string_view fileName;
    std::uint32_t fileSize;
    std::uint16_t shotRecordCount;
    std::int32_t deathbombWindowFrames;
    std::array<float, 10> tuningFloats;

    constexpr float hitboxDiagonalExtent() const { return tuningFloats[0]; }
    constexpr float grazeDiagonalExtent() const { return tuningFloats[1]; }
    constexpr float orthogonalMovementSpeed() const { return tuningFloats[6]; }
    constexpr float focusedOrthogonalMovementSpeed() const { return tuningFloats[7]; }
    constexpr float diagonalMovementSpeed() const { return tuningFloats[8]; }
    constexpr float focusedDiagonalMovementSpeed() const { return tuningFloats[9]; }
};

inline constexpr std::array<Th07PlayerShotSummary, kTh07PlayerShotFileCount> kTh07PlayerShotSummaries = {{
    {Th07ShotType::ReimuA, false, "ply00a.sht", 3280, 60, 15,
     {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
    {Th07ShotType::ReimuB, false, "ply00b.sht", 3436, 63, 15,
     {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
    {Th07ShotType::MarisaA, false, "ply01a.sht", 2604, 47, 8,
     {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
    {Th07ShotType::MarisaB, false, "ply01b.sht", 2136, 38, 8,
     {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
    {Th07ShotType::SakuyaA, false, "ply02a.sht", 4268, 79, 6,
     {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
    {Th07ShotType::SakuyaB, false, "ply02b.sht", 3748, 69, 6,
     {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
    {Th07ShotType::ReimuA, true, "ply00as.sht", 1928, 34, 15,
     {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
    {Th07ShotType::ReimuB, true, "ply00bs.sht", 1564, 27, 15,
     {1.65f, 2.8f, 8.0f, 20.0f, 0.5f, 128.0f, 4.0f, 1.6f, 2.8284271f, 1.1313709f}},
    {Th07ShotType::MarisaA, true, "ply01as.sht", 1408, 24, 8,
     {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
    {Th07ShotType::MarisaB, true, "ply01bs.sht", 1564, 27, 8,
     {2.2f, 2.5f, 10.0f, 28.0f, 0.5f, 156.0f, 5.0f, 2.0f, 3.5355339f, 1.4142135f}},
    {Th07ShotType::SakuyaA, true, "ply02as.sht", 2812, 51, 6,
     {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
    {Th07ShotType::SakuyaB, true, "ply02bs.sht", 3436, 63, 6,
     {2.2f, 6.0f, 8.0f, 20.0f, 0.33f, 128.0f, 4.0f, 2.2f, 2.8284271f, 1.555635f}},
}};

constexpr const Th07PlayerShotSummary *FindTh07PlayerShotSummary(Th07ShotType shotType, bool focused)
{
    for (const Th07PlayerShotSummary &summary : kTh07PlayerShotSummaries) {
        if (summary.shotType == shotType && summary.focused == focused) {
            return &summary;
        }
    }
    return nullptr;
}

constexpr const Th07PlayerShotDataLoadContract *
FindTh07PlayerShotDataLoadContract(Th07PlayerShotDataLoadKind kind)
{
    for (const Th07PlayerShotDataLoadContract &contract : kTh07PlayerShotDataLoadContracts) {
        if (contract.kind == kind) {
            return &contract;
        }
    }
    return nullptr;
}

constexpr const Th07PlayerShotSummary &GetTh07UnfocusedPlayerShotSummary(Th07ShotType shotType)
{
    return kTh07PlayerShotSummaries[static_cast<std::size_t>(shotType)];
}

static_assert(kTh07ShtFocusedPathTableAddress ==
              kTh07ShtUnfocusedPathTableAddress +
                  kTh07ShtPathTableEntryCount * kTh07ShtPathTableEntryStride);
static_assert(kTh07ShtPathStringAddresses.size() == kTh07PlayerShotFileCount);
static_assert(kTh07ShtPathStringAddresses[0] == 0x00496bb4);
static_assert(kTh07ShtPathStringAddresses[5] == 0x00496b64);
static_assert(kTh07ShtPathStringAddresses[6] == 0x00496b50);
static_assert(kTh07ShtPathStringAddresses[11] == 0x00496aec);
static_assert(FindTh07PlayerShotDataLoadContract(Th07PlayerShotDataLoadKind::Unfocused)
                  ->ownerPointerOffset == kPlayerShotDataPointerOffset);
static_assert(FindTh07PlayerShotDataLoadContract(Th07PlayerShotDataLoadKind::Focused)
                  ->ownerPointerOffset == kPlayerFocusedShotDataPointerOffset);

} // namespace th07
