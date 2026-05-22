#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace th07 {

enum class Th07BulletEvidenceStatus {
    SourceDerived,
    ObsoleteTh06Carryover,
    Pending,
    Blocked,
};

struct Th07EnemyBulletAnmResource {
    std::string_view archivePath;
    std::string_view ghidraStringAddress;
    std::uint16_t sprites;
    std::uint16_t scripts;
    std::uint16_t textureWidth;
    std::uint16_t textureHeight;
    std::uint8_t version;
    bool hasTextureData;
    Th07BulletEvidenceStatus status;
};

struct Th07ObsoleteBulletCarryover {
    std::string_view sourceToken;
    std::string_view reason;
    Th07BulletEvidenceStatus status;
};

struct Th07EnemyBulletTemplateScripts {
    std::uint8_t slotIndex;
    std::uint32_t bulletScript;
    std::uint32_t spawnFastScript;
    std::uint32_t spawnNormalScript;
    std::uint32_t spawnSlowScript;
    std::uint32_t spawnDonutScript;
    std::uint16_t activeSprite;
    std::uint8_t spriteWidth;
    std::uint8_t spriteHeight;
    float grazeSize;
    std::uint8_t spriteOffsetCategory;
    Th07BulletEvidenceStatus status;
};

inline constexpr std::uint32_t kTh07EnemyBulletLoadFunctionAddress = 0x00426f60;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateTableAddress = 0x00495160;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateTableFileOffset = 0x00094360;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateEntrySize = 0x14;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateCount = 11;
inline constexpr std::uint32_t kTh07EnemyBulletResolvedTemplateCount = 10;
inline constexpr std::uint32_t kTh07EnemyBulletAnmManagerSlot = 0x0b;
inline constexpr std::uint32_t kTh07EnemyBulletAnmScriptOffset = 0x0200;
inline constexpr std::uint32_t kTh07EnemyBulletResolvedScriptLast = 0x0218;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateStride = 0x0b8c;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateBulletVmOffset = 0x0000;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpawnFastVmOffset = 0x024c;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpawnNormalVmOffset = 0x0498;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpawnSlowVmOffset = 0x06e4;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpawnDonutVmOffset = 0x0930;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateGrazeXOffset = 0x0b7c;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateGrazeYOffset = 0x0b80;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpriteHeightOffset = 0x0b89;
inline constexpr std::uint32_t kTh07EnemyBulletTemplateSpriteOffsetCategoryOffset = 0x0b8a;
inline constexpr std::uint16_t kTh07EnemyBulletTemplateMissingSprite = 0xffff;
inline constexpr std::uint32_t kTh07EnemyBulletGrazeThreshold8Bits = 0x41000000;
inline constexpr std::uint32_t kTh07EnemyBulletGrazeThreshold16Bits = 0x41800000;
inline constexpr std::uint32_t kTh07EnemyBulletGrazeThreshold32Bits = 0x42000000;

inline constexpr Th07EnemyBulletAnmResource kTh07EnemyBulletAnmResource = {
    "data/etama.anm",
    "00498524",
    168,
    25,
    256,
    256,
    2,
    true,
    Th07BulletEvidenceStatus::SourceDerived,
};

inline constexpr std::array<Th07EnemyBulletTemplateScripts, kTh07EnemyBulletTemplateCount>
    kTh07EnemyBulletTemplateScripts = {{
        {0, 0x0200, 0x0212, 0x0213, 0x0214, 0x020f, 0, 8, 8, 4.0f, 5,
         Th07BulletEvidenceStatus::SourceDerived},
        {1, 0x0201, 0x0215, 0x0216, 0x0217, 0x0210, 16, 16, 16, 6.0f, 3,
         Th07BulletEvidenceStatus::SourceDerived},
        {2, 0x0202, 0x0215, 0x0216, 0x0217, 0x0210, 32, 14, 16, 4.0f, 4,
         Th07BulletEvidenceStatus::SourceDerived},
        {3, 0x0203, 0x0215, 0x0216, 0x0217, 0x0210, 48, 16, 16, 6.0f, 3,
         Th07BulletEvidenceStatus::SourceDerived},
        {4, 0x0204, 0x0215, 0x0216, 0x0217, 0x0210, 64, 14, 16, 4.0f, 4,
         Th07BulletEvidenceStatus::SourceDerived},
        {5, 0x0205, 0x0215, 0x0216, 0x0217, 0x0210, 80, 14, 16, 4.0f, 4,
         Th07BulletEvidenceStatus::SourceDerived},
        {6, 0x0206, 0x0215, 0x0216, 0x0217, 0x0210, 96, 14, 16, 4.0f, 4,
         Th07BulletEvidenceStatus::SourceDerived},
        {7, 0x0207, 0x0218, 0x0218, 0x0218, 0x0211, 112, 32, 32, 10.0f, 2,
         Th07BulletEvidenceStatus::SourceDerived},
        {8, 0x0208, 0x0218, 0x0218, 0x0218, 0x0211, 120, 32, 32, 5.0f, 1,
         Th07BulletEvidenceStatus::SourceDerived},
        {9, 0x0209, 0x0218, 0x0218, 0x0218, 0x0211, 128, 32, 32, 8.0f, 2,
         Th07BulletEvidenceStatus::SourceDerived},
        {10, 0x02a8, 0x02aa, 0x02aa, 0x02aa, 0x02a9, kTh07EnemyBulletTemplateMissingSprite, 0, 0, 0.0f, 0,
         Th07BulletEvidenceStatus::Blocked},
    }};

inline constexpr std::array<Th07ObsoleteBulletCarryover, 4> kTh07ObsoleteBulletResourceCarryovers = {{
    {"data/etama3.anm", "TH07 live runtime now uses the proven single data/etama.anm load",
     Th07BulletEvidenceStatus::ObsoleteTh06Carryover},
    {"data/etama4.anm", "TH07 live runtime now uses the proven single data/etama.anm load",
     Th07BulletEvidenceStatus::ObsoleteTh06Carryover},
    {"ANM_FILE_BULLET3", "copied TH06 split bullet-bank alias superseded by slot 0x0b",
     Th07BulletEvidenceStatus::ObsoleteTh06Carryover},
    {"ANM_FILE_BULLET4", "copied TH06 split bullet-bank alias superseded by slot 0x0b",
     Th07BulletEvidenceStatus::ObsoleteTh06Carryover},
}};

constexpr std::string_view Th07BulletEvidenceStatusName(Th07BulletEvidenceStatus status)
{
    switch (status) {
    case Th07BulletEvidenceStatus::SourceDerived:
        return "source-derived";
    case Th07BulletEvidenceStatus::ObsoleteTh06Carryover:
        return "obsolete-th06-carryover";
    case Th07BulletEvidenceStatus::Pending:
        return "pending";
    case Th07BulletEvidenceStatus::Blocked:
        return "blocked";
    }
    return "pending";
}

constexpr bool IsTh07EnemyBulletTemplateScriptResolved(std::uint32_t scriptIndex)
{
    return scriptIndex >= kTh07EnemyBulletAnmScriptOffset && scriptIndex <= kTh07EnemyBulletResolvedScriptLast;
}

constexpr const Th07EnemyBulletTemplateScripts *FindTh07EnemyBulletTemplate(std::uint8_t slotIndex)
{
    for (const Th07EnemyBulletTemplateScripts &entry : kTh07EnemyBulletTemplateScripts) {
        if (entry.slotIndex == slotIndex) {
            return &entry;
        }
    }
    return nullptr;
}

constexpr std::uint32_t CountTh07ResolvedEnemyBulletTemplates()
{
    std::uint32_t count = 0;
    for (const Th07EnemyBulletTemplateScripts &entry : kTh07EnemyBulletTemplateScripts) {
        if (entry.status == Th07BulletEvidenceStatus::SourceDerived) {
            count++;
        }
    }
    return count;
}

} // namespace th07
