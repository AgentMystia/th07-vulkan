# TH07 Diff From TH06

This document is the working map for source-first TH07 restoration. Every reused TH06 behavior must be checked here before it becomes gameplay authority.

## Status Legend

- `Same`: TH07 is confirmed to match TH06 closely enough for direct reuse.
- `Changed`: TH07 shares the subsystem shape but has different constants, data, state, or edge cases.
- `New`: TH07 adds behavior that TH06 does not have.
- `Pending`: not audited yet.

## Subsystem Map

| Subsystem | Status | Implementation Rule |
| --- | --- | --- |
| Window, fixed timestep, input edge tracking | Changed | Platform layer is modern SDL3, but fixed-frame input semantics must be derived from TH07. |
| Archive/resource loading | Changed | TH07 uses `th07.dat`, `thbgm.dat`, and TH07-specific archive/resource manifests. |
| ANM parsing | Same | thcrap marks TH07 as `anm06`; `scripts/audit-th07-resources.mjs` now smoke-checks 63 extracted ANM files through the raw header/sprite/script tables, covering 1187 sprite records and 727 script records before deeper opcode parity work. |
| STD parsing | Changed | TH07 has `stage1-8.std`; `scripts/audit-th07-resources.mjs` now parses all eight STD binaries through the `RawStageHeader`/object/instance/script layout used by `FUN_00407610`, covering 45 objects, 162 quads, 906 instances, 378 script instructions, and script opcodes `0..31` with gaps. The readable dump coverage is still 7/8 because `reference/std/stage6.dstd` is empty, so Stage 6 work should rely on `stage6.std` binary evidence until that dump is refreshed. |
| MSG parsing | Same | thcrap marks TH07 as `msg06`; the resource audit now verifies all eight readable MSG dumps with 60 entries and 1098 timeline labels. |
| ECL instruction core | Changed | TH07 `ecldata1-8.ecl` manifest is anchored, and `scripts/audit-ecl-opcodes.mjs` now audits all eight extracted `.decl` dumps: 141 unique script opcodes appear, with max opcode `161`. The current copied enum/dispatch is missing 25 TH07-used opcodes (`136..146`, `148..161`). TH07 decompiler function `00410520` now maps 23 of those to high switch cases (`136..139`, `141..160`), and `config/ecl_high_opcode_cases.csv` records their direct side-effect surfaces by offset/global/helper call. Opcodes `140` and `161` remain blocked because the current decompiler switch has no `0x8c` or `0xa1` case. Runtime replacement must resolve owner fields/lifetimes and those two blockers before wiring. |
| Enemy and boss lifecycle | Changed | Extra/Phantasm, boss UI, and spell handling require TH07 function replacement. |
| Bullet/effect manager | Changed | `etama.anm`, TH07 bullet tables, and the 34-entry TH07 effect table/layout must replace TH06 assumptions; effect slots, spawn/update manager layout, stage effect ANM loads, bomb common effect id `0x19`, first TH07 random-splash/render-mode-2 orbit/radial/player-anchored/perspective/camera-snap/eased-radial callbacks, the stage camera opcode/interpolation model, core non-camera Stage position/fog/pause/jump opcodes, stage ANM VM opcodes `0x1d`/`0x1e`, ANM VM render-helper contracts, the TH07 `FUN_00450d60` 16-bit instruction header/operand readers, `FUN_00450a50`/`FUN_00450b20` value operand-reference variable slots, `FUN_00450c10`/`FUN_00450ca0` write-target resolver behavior for the simple variable/math opcode slice, execute-opcode slices through raw opcode `0x51`, and the `FUN_00450d60` post-switch action contract are anchored. `Stage` now exposes the TH07 `0x52b4` camera block, dispatches the camera opcode subset and core non-camera stage opcodes, and executes the source-derived stage ANM VM script slots through a TH07-header-driven opcode slice with raw `0x04` jump, raw `0x05` loop, value operand references, raw `0x0e..0x51` follow-up/interpolation/variable/math/control opcodes, source-derived screen-quad draw helper routing, and pending-interrupt label dispatch; `EffectManager` routes draw lists through the source-derived TH07 screen-quad, camera-projected, and transformed camera-projected helpers. The copied TH06 20-entry runtime skeleton, the Stage ANM VM call to TH06 `AnmManager::ExecuteScript`, the TH06 `DrawInner` bridge for these TH07 draw routes, and the TH06 rectangle-darken bomb backdrop have been removed. TH07 screen shake now feeds ANM manager draw offsets and uses the mapped GameManager frame-counter gate, Stage has the source-derived color-multiplier write/consume/reset path, the `004083f0` reset-guard write at `0134cdb0` is mapped to stage offset `+0x52b0`, and the player/bomb producers that feed that path are mapped in `PlayerBombTables`. Remaining renderer gaps are original batch-buffer flushing and runtime replacement for the mode-4 `FUN_00406930` producers. |
| Laser behavior | Pending | Audit TH07 ECL and executable before reuse. |
| Item manager | Changed | Cherry, Cherry+, Border Bonus, and TH07 point item behavior are authoritative. |
| Player roster | New | TH07 has Reimu, Marisa, and Sakuya, each with A/B shot types. |
| Player shot data | New | 12 `.sht` files and high-level SHT callback semantics are Ghidra/binary anchored; behavior replacement pending. |
| Bomb behavior | Changed | Focused/unfocused bomb routine slots, player object base/clear span, original `0x24c` player ANM VM prefix layout, player position/movement/hitbox/graze/grab-item field offsets, raw mode state `+0x2408`, option/focus state `+0x240a/+0x240b`, transition request `+0x240d`, common-effect durations, common start-effect metadata, score-drain factors/minimums, score-drain-step math, common backdrop color timing/runtime color-write wiring, mode-4 stage-color producers, mode transition effect metadata, the `DAT_004d44e0 == DAT_004bdad8 + 0x16a08` alias, and movement speed multipliers are binary/decompiler anchored. `Player.hpp` now structurally carries the leading TH07 VM/position block, `+0x23f0/+0x23f4` speed fields, `+0x16a00..+0x16a14` common-effect tail, and active pointers at `+0xb7e68/+0xb7e6c`; `Player.cpp` seeds the initial low-byte mode/option/request state plus `FUN_004423e0` common-effect defaults. Runtime producer wiring remains pending. |
| Point of Collection | Changed | Use original TH07 full-power/border rules; no Web modernization. |
| Hitbox display | Changed | Debug-only overlay may exist; release gameplay must match original visibility. |
| Cherry / Cherry+ | New | String/constant anchors exist; state transitions still require decompilation. |
| Supernatural Border | New | HUD/penalty/rank anchors exist; start/break/end transitions still require decompilation. |
| Spell card score/result | Changed | TH07 has 141 spell ids; score/result structures still need mapping. |
| Stage list | Changed | Main stages 1-6 plus Extra stage 7 and Phantasm stage 8. |
| BGM streaming/loop | Changed | Use `thbgm.fmt` + `thbgm.dat`; modern audio backend must preserve original loop metadata. |
| SFX table | Changed | TH07 adds `se_bonus`, `se_bonus2`, `se_border`, and `se_pause`. |
| C++ source reference | New | `src/` is the TH06-derived baseline being replaced. |

## Immediate Audit Queue

1. Map TH07 ANM manager draw-offset/global-color producers and decide the exact boundary for original batch-buffer flushing versus the portable renderer.
2. Promote the 23 high ECL side-effect surfaces into named owner fields only after owners/lifetimes are proven, and resolve the missing `140/161` dispatch evidence before replacing interpreter dispatch.
3. Use the STD binary audit for Stage 6 and move ANM/STD/MSG from smoke coverage toward parser/opcode parity; refresh the Stage 6 readable dump only when needed.
4. Use the expanded Player owner tail to wire the mapped mode/common-effect producers before replacing player-shot callbacks and full bomb runtime.
5. Source Cherry, Cherry+, and Border state transitions from Ghidra before gameplay implementation.
