#include "ShtFile.hpp"

#include <bit>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace th07 {
namespace {

void RequireRange(std::span<const std::uint8_t> bytes, std::size_t offset, std::size_t size)
{
    if (offset > bytes.size() || size > bytes.size() - offset) {
        throw std::runtime_error("SHT parse error: unexpected end of file");
    }
}

std::uint16_t ReadU16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    RequireRange(bytes, offset, 2);
    return static_cast<std::uint16_t>(bytes[offset]) |
           static_cast<std::uint16_t>(bytes[offset + 1] << 8);
}

std::int16_t ReadI16(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    const std::uint16_t raw = ReadU16(bytes, offset);
    return std::bit_cast<std::int16_t>(raw);
}

std::uint32_t ReadU32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    RequireRange(bytes, offset, 4);
    return static_cast<std::uint32_t>(bytes[offset]) |
           (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

std::int32_t ReadI32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    const std::uint32_t raw = ReadU32(bytes, offset);
    return std::bit_cast<std::int32_t>(raw);
}

float ReadF32(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    const std::uint32_t raw = ReadU32(bytes, offset);
    return std::bit_cast<float>(raw);
}

ShtShotRecord ParseShotRecord(std::span<const std::uint8_t> bytes, std::size_t offset)
{
    RequireRange(bytes, offset, kShtShotRecordSize);

    return {
        ReadI16(bytes, offset + 0x00),
        ReadI16(bytes, offset + 0x02),
        ReadF32(bytes, offset + 0x04),
        ReadF32(bytes, offset + 0x08),
        ReadF32(bytes, offset + 0x0c),
        ReadF32(bytes, offset + 0x10),
        ReadF32(bytes, offset + 0x14),
        ReadF32(bytes, offset + 0x18),
        ReadU16(bytes, offset + 0x1c),
        bytes[offset + 0x1e],
        bytes[offset + 0x1f],
        ReadI16(bytes, offset + 0x20),
        ReadI16(bytes, offset + 0x22),
        {
            ReadI32(bytes, offset + 0x24),
            ReadI32(bytes, offset + 0x28),
            ReadI32(bytes, offset + 0x2c),
            ReadI32(bytes, offset + 0x30),
        },
    };
}

} // namespace

const std::array<ShtCallbackTarget, 6> kShtOnSpawnCallbacks = {{
    {0, 0x00000000, "", ShtCallbackSemantic::None},
    {1, 0x0043bdc0, "FUN_0043bdc0", ShtCallbackSemantic::PeriodicSpawn},
    {2, 0x0043be10, "FUN_0043be10", ShtCallbackSemantic::Mode1OptionSpawn},
    {3, 0x0043bf50, "FUN_0043bf50", ShtCallbackSemantic::Mode3TrailSpawn},
    {4, 0x0043c0d0, "FUN_0043c0d0", ShtCallbackSemantic::RandomizedAnglePeriodicSpawn},
    {5, 0x0043c1c0, "FUN_0043c1c0", ShtCallbackSemantic::FixedAnglePeriodicSpawn},
}};

const std::array<ShtCallbackTarget, 6> kShtOnUpdateCallbacks = {{
    {0, 0x00000000, "", ShtCallbackSemantic::None},
    {1, 0x0043c250, "FUN_0043c250", ShtCallbackSemantic::HomingUpdateSpeed10},
    {2, 0x0043c480, "FUN_0043c480", ShtCallbackSemantic::HomingUpdateSpeed18},
    {3, 0x0043c6b0, "FUN_0043c6b0", ShtCallbackSemantic::VerticalAngleDriftUpdate},
    {4, 0x0043c700, "FUN_0043c700", ShtCallbackSemantic::Mode1OptionUpdate},
    {5, 0x0043c940, "FUN_0043c940", ShtCallbackSemantic::Mode3TrailUpdate},
}};

const std::array<ShtCallbackTarget, 2> kShtOnDrawCallbacks = {{
    {0, 0x00000000, "", ShtCallbackSemantic::None},
    {1, 0x0043ccb0, "FUN_0043ccb0", ShtCallbackSemantic::TrailDraw},
}};

const std::array<ShtCallbackTarget, 3> kShtOnCollisionCallbacks = {{
    {0, 0x00000000, "", ShtCallbackSemantic::None},
    {1, 0x0043cde0, "FUN_0043cde0", ShtCallbackSemantic::ImpactScaleAndSound},
    {2, 0x0043d0e0, "FUN_0043d0e0", ShtCallbackSemantic::CollisionCounterSound},
}};

float ShtHeader::hitboxDiagonalExtent() const
{
    return tuningFloats[0];
}

float ShtHeader::grazeDiagonalExtent() const
{
    return tuningFloats[1];
}

float ShtHeader::orthogonalMovementSpeed() const
{
    return tuningFloats[6];
}

float ShtHeader::focusedOrthogonalMovementSpeed() const
{
    return tuningFloats[7];
}

float ShtHeader::diagonalMovementSpeed() const
{
    return tuningFloats[8];
}

float ShtHeader::focusedDiagonalMovementSpeed() const
{
    return tuningFloats[9];
}

ShtFile ParseShtFile(std::span<const std::uint8_t> bytes)
{
    RequireRange(bytes, 0, kShtHeaderSize);

    ShtFile file;
    file.header.formatId = ReadU16(bytes, 0x00);
    file.header.powerLevelCount = ReadU16(bytes, 0x02);
    file.header.unknownFloat04 = ReadF32(bytes, 0x04);
    file.header.deathbombWindowFrames = ReadI32(bytes, 0x08);
    for (std::size_t i = 0; i < file.header.tuningFloats.size(); ++i) {
        file.header.tuningFloats[i] = ReadF32(bytes, 0x0c + i * sizeof(float));
    }

    const std::size_t tableSize =
        kShtHeaderSize + static_cast<std::size_t>(file.header.powerLevelCount) * kShtPowerLevelEntrySize;
    RequireRange(bytes, 0, tableSize);

    file.powerLevels.reserve(file.header.powerLevelCount);
    for (std::size_t i = 0; i < file.header.powerLevelCount; ++i) {
        const std::size_t entryOffset = kShtHeaderSize + i * kShtPowerLevelEntrySize;
        ShtPowerLevel level;
        level.recordOffset = ReadU32(bytes, entryOffset + 0x00);
        level.powerThreshold = ReadI32(bytes, entryOffset + 0x04);

        std::size_t cursor = level.recordOffset;
        for (;;) {
            const std::int16_t wait = ReadI16(bytes, cursor);
            if (wait < 0) {
                level.terminatorOffset = static_cast<std::uint32_t>(cursor);
                break;
            }

            level.shots.push_back(ParseShotRecord(bytes, cursor));
            cursor += kShtShotRecordSize;
        }

        file.powerLevels.push_back(std::move(level));
    }

    return file;
}

ShtFile LoadShtFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("SHT parse error: unable to open " + path.string());
    }

    file.seekg(0, std::ios::end);
    const std::streamoff size = file.tellg();
    if (size < 0) {
        throw std::runtime_error("SHT parse error: unable to determine size for " + path.string());
    }
    file.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    if (!bytes.empty()) {
        file.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (!file) {
            throw std::runtime_error("SHT parse error: unable to read " + path.string());
        }
    }

    return ParseShtFile(bytes);
}

std::size_t CountShtShotRecords(const ShtFile &file)
{
    std::size_t count = 0;
    for (const ShtPowerLevel &level : file.powerLevels) {
        count += level.shots.size();
    }
    return count;
}

const ShtPowerLevel *FindShtPowerLevelForPower(const ShtFile &file, int power)
{
    for (const ShtPowerLevel &level : file.powerLevels) {
        if (power < level.powerThreshold) {
            return &level;
        }
    }
    return file.powerLevels.empty() ? nullptr : &file.powerLevels.back();
}

const ShtCallbackTarget *FindShtCallback(ShtCallbackRole role, std::int32_t id)
{
    const auto findInTable = [id](const auto &table) -> const ShtCallbackTarget * {
        for (const ShtCallbackTarget &target : table) {
            if (target.id == id) {
                return &target;
            }
        }
        return nullptr;
    };

    switch (role) {
    case ShtCallbackRole::OnSpawn:
        return findInTable(kShtOnSpawnCallbacks);
    case ShtCallbackRole::OnUpdate:
        return findInTable(kShtOnUpdateCallbacks);
    case ShtCallbackRole::OnDraw:
        return findInTable(kShtOnDrawCallbacks);
    case ShtCallbackRole::OnCollision:
        return findInTable(kShtOnCollisionCallbacks);
    }

    return nullptr;
}

ResolvedShtCallbacks ResolveShtCallbacks(const ShtShotRecord &shot)
{
    return {
        FindShtCallback(ShtCallbackRole::OnSpawn, shot.callbacks.onSpawn),
        FindShtCallback(ShtCallbackRole::OnUpdate, shot.callbacks.onUpdate),
        FindShtCallback(ShtCallbackRole::OnDraw, shot.callbacks.onDraw),
        FindShtCallback(ShtCallbackRole::OnCollision, shot.callbacks.onCollision),
    };
}

std::string_view ShtCallbackSemanticName(ShtCallbackSemantic semantic)
{
    switch (semantic) {
    case ShtCallbackSemantic::None:
        return "none";
    case ShtCallbackSemantic::PeriodicSpawn:
        return "periodic spawn";
    case ShtCallbackSemantic::Mode1OptionSpawn:
        return "mode-1 option spawn";
    case ShtCallbackSemantic::Mode3TrailSpawn:
        return "mode-3 trail spawn";
    case ShtCallbackSemantic::RandomizedAnglePeriodicSpawn:
        return "randomized-angle periodic spawn";
    case ShtCallbackSemantic::FixedAnglePeriodicSpawn:
        return "fixed-angle periodic spawn";
    case ShtCallbackSemantic::HomingUpdateSpeed10:
        return "homing update speed 10";
    case ShtCallbackSemantic::HomingUpdateSpeed18:
        return "homing update speed 18";
    case ShtCallbackSemantic::VerticalAngleDriftUpdate:
        return "vertical angle-drift update";
    case ShtCallbackSemantic::Mode1OptionUpdate:
        return "mode-1 option update";
    case ShtCallbackSemantic::Mode3TrailUpdate:
        return "mode-3 trail update";
    case ShtCallbackSemantic::TrailDraw:
        return "trail draw";
    case ShtCallbackSemantic::ImpactScaleAndSound:
        return "impact scale and sound";
    case ShtCallbackSemantic::CollisionCounterSound:
        return "collision counter sound";
    }

    return "unknown";
}

} // namespace th07
