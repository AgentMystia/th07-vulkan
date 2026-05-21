# Module Map

| TH07 Module | Current Source | TH06 Template | Status |
| --- | --- | --- | --- |
| `Rng` | `src/Rng.*` | TH06 `Rng.*` | Pending TH07 audit |
| `ZunTimer` | `src/ZunTimer.*` | TH06 `ZunTimer.*` | Pending TH07 audit |
| `FileSystem` | `src/FileSystem.*` | TH06 `FileSystem.*` | Changed for TH07 archive layout |
| `AnmManager` | `src/AnmManager.*` | TH06 `AnmManager.*` | Same parser family, pending opcode audit |
| `Stage` | `src/Stage.*` | TH06 `Stage.*` | Changed: TH07 stage1-8 manifest applied |
| `EclManager` | `src/EclManager.*` | TH06 `EclManager.*` | Changed: opcode audit pending |
| `EnemyManager` | `src/EnemyManager.*` | TH06 `EnemyManager.*` | Changed |
| `BulletManager` / `EffectManager` | `src/BulletManager.*`, `src/EffectManager.*`, `src/Th07EffectTables.*` | TH06 `BulletManager.*`, `EffectManager.*` | Changed: TH07 effect table, slot layout, spawn paths, draw-list offsets, stage effect ANM loads, first random-splash/render-mode-2 orbit/radial/player-anchored/perspective/camera-snap/eased-radial callbacks, stage-owned camera field mapping, core non-camera Stage position/fog/pause/jump opcodes, Stage ANM VM opcodes `0x1d`/`0x1e`, ANM VM render-helper contracts and source-derived screen/camera/transformed draw helper routing plus `FUN_0044efb0` submit slice, TH07 shake draw offsets plus GameManager frame-counter gate, `FUN_00406930`/`004069d0`/`00406de0` color-multiplier path with common bomb backdrop runtime wiring and `0134cdb0` reset guard, TH07 `FUN_00450d60` instruction header/operand readers, `FUN_00450a50`/`FUN_00450b20` value operand references, `FUN_00450c10`/`FUN_00450ca0` write-target references for simple variable/math/control opcodes through raw `0x51`, Stage ANM VM opcode runtime slice with raw `0x04` jump, raw `0x05` loop, raw `0x0e..0x51` follow-up/interpolation/variable/math/control opcodes, pending-interrupt dispatch, execute-opcode slices through raw `0x51`, and the post-switch VM action contract anchored; callback bodies, original batch-buffer flushing, mode-4 player color producers, and bullet tables pending |
| `ItemManager` | `src/ItemManager.*` | TH06 `ItemManager.*` | Changed for Cherry/Border |
| `Player` | `src/Player.*`, `src/PlayerLayout.hpp` | TH06 `Player.*` | Changed: Sakuya added; `.sht` behavior and exact runtime replacement pending; ANM VM prefix plus position/collision block are now structurally bridged in `Player.hpp` |
| `BombData` | `src/BombData.*`, `src/PlayerBombTables.*`, `src/PlayerLayout.hpp` | TH06 `BombData.*` | Changed: TH07 focused/unfocused slots, player object base/clear span, original player ANM VM prefix layout, position/collision fields, effect durations, common start-effect metadata, mode transition effect metadata, score-drain factors/minimums, score-drain-step math, backdrop color timing, mode-4 color-frame alias, and speed multipliers anchored |
| `ShtManager` | `src/ShtManager.*` | none | Compatibility wrapper over `Th07ResourceManifest`; SHT table data has a single source of truth |
| `CherrySystem` | `src/CherrySystem.*` | none | New: HUD/constant anchors added |
| `BorderSystem` | `src/BorderSystem.*` | none | New: HUD/penalty/rank anchors added |
| `Th07Constants` | `src/Th07Constants.*` | none | Portable source-derived constants |
| `Th07ResourceManifest` | `src/Th07ResourceManifest.*` | `Th07ResourceManifest.*`, `ShtManager.*` | Portable Ghidra-anchored resource routes, including the authoritative 12-entry SHT path table |
| `ReferencePaths` | `src/ReferencePaths.*` | none | Modern local reference locator |
| `SDL3/Vulkan app` | `src/Th07App.cpp` | none | Modern platform shell |
