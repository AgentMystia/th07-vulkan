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
| ANM parsing | Same | thcrap marks TH07 as `anm06`; reuse parser shape only after file/opcode checks. |
| STD parsing | Changed | TH07 has `stage1-8.std`; parser shape is shared, stage list differs. |
| MSG parsing | Same | thcrap marks TH07 as `msg06`; verify all `msg1-8.dat` counts. |
| ECL instruction core | Changed | TH07 `ecldata1-8.ecl` manifest is anchored; opcode table audit still pending. |
| Enemy and boss lifecycle | Changed | Extra/Phantasm, boss UI, and spell handling require TH07 function replacement. |
| Bullet manager | Changed | `etama.anm` and TH07 bullet tables must replace TH06 assumptions. |
| Laser behavior | Pending | Audit TH07 ECL and executable before reuse. |
| Item manager | Changed | Cherry, Cherry+, Border Bonus, and TH07 point item behavior are authoritative. |
| Player roster | New | TH07 has Reimu, Marisa, and Sakuya, each with A/B shot types. |
| Player shot data | New | 12 `.sht` files are Ghidra-anchored; decoding and behavior replacement pending. |
| Bomb behavior | Changed | Focused/unfocused bomb behavior differs and includes Sakuya. |
| Point of Collection | Changed | Use original TH07 full-power/border rules; no Web modernization. |
| Hitbox display | Changed | Debug-only overlay may exist; release gameplay must match original visibility. |
| Cherry / Cherry+ | New | String/constant anchors exist; state transitions still require decompilation. |
| Supernatural Border | New | HUD/penalty/rank anchors exist; start/break/end transitions still require decompilation. |
| Spell card score/result | Changed | TH07 has 141 spell ids; score/result structures still need mapping. |
| Stage list | Changed | Main stages 1-6 plus Extra stage 7 and Phantasm stage 8. |
| BGM streaming/loop | Changed | Use `thbgm.fmt` + `thbgm.dat`; modern audio backend must preserve original loop metadata. |
| SFX table | Changed | TH07 adds `se_bonus`, `se_bonus2`, `se_border`, and `se_pause`. |
| C++ source reference | New | `src/reconstruction/original-engine/` is the TH06-derived baseline being replaced. |

## Immediate Audit Queue

1. Export Ghidra xrefs/decompilation around SHT, Cherry, Border, ECL, and result-screen anchors.
2. Confirm ANM/STD/MSG parsers against all extracted TH07 files.
3. Decode `.sht` shot tables and replace TH06 player-shot callbacks.
4. Source Cherry, Cherry+, and Border state transitions from Ghidra before gameplay implementation.
5. Keep SDL3/Vulkan backend outside deterministic gameplay logic.
