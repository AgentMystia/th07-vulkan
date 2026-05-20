# Module Map

| TH07 Module | Current Source | TH06 Template | Status |
| --- | --- | --- | --- |
| `Rng` | `src/reconstruction/original-engine/Rng.*` | TH06 `Rng.*` | Pending TH07 audit |
| `ZunTimer` | `src/reconstruction/original-engine/ZunTimer.*` | TH06 `ZunTimer.*` | Pending TH07 audit |
| `FileSystem` | `src/reconstruction/original-engine/FileSystem.*` | TH06 `FileSystem.*` | Changed for TH07 archive layout |
| `AnmManager` | `src/reconstruction/original-engine/AnmManager.*` | TH06 `AnmManager.*` | Same parser family, pending opcode audit |
| `Stage` | `src/reconstruction/original-engine/Stage.*` | TH06 `Stage.*` | Changed: TH07 stage1-8 manifest applied |
| `EclManager` | `src/reconstruction/original-engine/EclManager.*` | TH06 `EclManager.*` | Changed: opcode audit pending |
| `EnemyManager` | `src/reconstruction/original-engine/EnemyManager.*` | TH06 `EnemyManager.*` | Changed |
| `BulletManager` | `src/reconstruction/original-engine/BulletManager.*` | TH06 `BulletManager.*` | Changed |
| `ItemManager` | `src/reconstruction/original-engine/ItemManager.*` | TH06 `ItemManager.*` | Changed for Cherry/Border |
| `Player` | `src/reconstruction/original-engine/Player.*` | TH06 `Player.*` | Changed: Sakuya added; `.sht` behavior pending |
| `BombData` | `src/reconstruction/original-engine/BombData.*` | TH06 `BombData.*` | Changed for focused/unfocused TH07 bombs |
| `ShtManager` | `src/reconstruction/original-engine/ShtManager.*` | none | New: 12 Ghidra-anchored shot data files |
| `CherrySystem` | `src/reconstruction/original-engine/CherrySystem.*` | none | New: HUD/constant anchors added |
| `BorderSystem` | `src/reconstruction/original-engine/BorderSystem.*` | none | New: HUD/penalty/rank anchors added |
| `Th07Constants` | `src/game/Th07Constants.*` | none | Portable source-derived constants |
| `Th07ResourceManifest` | `src/game/Th07ResourceManifest.*` | `Th07ResourceManifest.*`, `ShtManager.*` | Portable Ghidra-anchored resource routes |
| `ReferencePaths` | `src/game/ReferencePaths.*` | none | Modern local reference locator |
| `SDL3/Vulkan app` | `src/app/main.cpp` | none | Modern platform shell |
