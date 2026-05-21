# TH07 Reconstruction Status

Current verdict: not 100% synchronized yet.

This file tracks the gap between the current C++ reconstruction and the requested DONE condition: behavior matches TH07 static decompilation and original data.

## Current Checkpoint Labels

- Exact / build-validated: flat `src/` project wiring, local reference-path discovery, resource manifest lookup shape, SHT binary parser coverage, and platform-independent Player layout constants covered by CMake tests.
- Source-derived / anchored: SHT path table/order, SHT callback addresses and high-level semantics, Player/Bomb routine tables, Player ANM VM prefix and collision field offsets, Player raw mode/option/common-effect owner fields, Player active-effect pointer tail slots, bomb score-drain/speed/common-effect metadata, ANM/STD/MSG resource-format smoke coverage, ECL declaration opcode coverage, Effect table/layout metadata, Stage camera/ANM VM opcode slices, clear-bonus constants, and Cherry/Border notification labels.
- Transitional: `src/Player.cpp` now initializes the source-derived low-byte Player mode/option/request state and `FUN_004423e0` common-effect defaults, while `src/BombData.*`, `src/EffectManager.*`, `src/Stage.*`, and other flat TH06-style runtime files still carry mixed TH07 evidence slices and need function-level replacement before gameplay parity.
- Pending: full Player shot runtime, full bomb calc/draw/update runtime, Item/Cherry/Border state transitions, GameManager/result/replay/input/window audits, ECL opcode replacement, Bullet/Laser tables, and remaining renderer batch-buffer/x87 edge parity.

## Confirmed Anchors

| Area | Evidence | Current Source |
| --- | --- | --- |
| Ghidra anchor xrefs | `reference/ghidra/anchors/anchor-xrefs.tsv` | `npm run ghidra-anchors` |
| Character GUI face/loading loads | `FUN_00428b19` | `src/Gui.cpp` |
| Stage face/stdtxt/msg loads | `FUN_00428b19` | `src/Gui.cpp` |
| ASCII/capture ANM load | decompile sample around ASCII init | `src/AsciiManager.cpp` |
| Stage primary/Stage 4 extra background loads | `FUN_00407000` | `src/Stage.cpp` |
| SHT path table order | xrefs through `0049f530..0049f55c` | `src/Th07ResourceManifest.*`; `src/ShtManager.*` is only a legacy compatibility wrapper |
| SHT binary container layout | `FUN_00442b70`, `reference/extracted/ply*.sht` | `src/ShtFile.*`, `tests/test_sht_parser.cpp` |
| SHT callback table addresses | `DAT_0049ecb0`, `DAT_0049ecc8`, `DAT_0049ece0`, `DAT_0049ece8` in `th07.exe` | `src/ShtFile.*`, `tests/test_sht_parser.cpp` |
| SHT callback high-level semantics | `FUN_0043bdc0..0043d0e0` decompiler output | `ShtCallbackSemantic` in `src/ShtFile.*` |
| ANM/STD/MSG extracted format smoke coverage | `reference/extracted/*.anm`, `reference/extracted/stage*.std`, `reference/std/stage*.dstd`, `reference/msg/msg*.txt` | `scripts/audit-th07-resources.mjs`: 63 ANM raw headers with 1187 sprites and 727 scripts; 8 STD binaries with 45 objects, 162 quads, 906 instances, 378 script instructions, and Stage 6 binary coverage despite `stage6.dstd` being empty; 8 MSG dumps with 60 entries and 1098 timeline labels |
| ECL declaration and dispatch coverage | `reference/ecl/ecldata1-8.decl`, `reference/ghidra/decomp/functions/00410520_FUN_00410520.c` | `scripts/audit-ecl-opcodes.mjs`: 141 unique script opcodes, max opcode `161`, 25 extracted TH07 opcodes not yet in the current `EclRawInstrOpcode` enum, 23 of those mapped to high TH07 decompiler switch cases, direct side-effect surfaces recorded in `config/ecl_high_opcode_cases.csv`, and opcodes `140`/`161` still blocked by missing switch cases in the current decompile |
| Player bomb routine table addresses | `PTR_FUN_0049ec50` in `FUN_004423e0` and `th07.exe` | `src/PlayerBombTables.*`, `tests/test_sht_parser.cpp` |
| Player bomb routine slot roles | call sites at `00440b18`, `00440b26`, `004420df`, `004420ed` | `PlayerBombRoutineSlot` in `src/PlayerBombTables.*` |
| Player bomb lifecycle fields | `FUN_004409f0`, `FUN_004420b0`, `FUN_004429d0`, update dispatch around `00440b18` | player object base/clear span and bomb offsets/constants in `src/PlayerBombTables.*` |
| Player ANM VM prefix layout | `FUN_00441fb0`, `FUN_004420b0`, `FUN_004423e0`, and PE instruction checks in `th07.exe` | original `0x24c` player VM slot size, VM offsets, draw-position offsets, option-position offsets, and script indices in `src/PlayerLayout.hpp`, `src/PlayerBombTables.*`, and `Player.hpp` offset asserts |
| Player position and collision boxes | `FUN_004423e0`, `FUN_0043ee50`, `FUN_0043e260`, `FUN_0043e3b0`, `FUN_0043e4e0`, `FUN_0043e6b0`, and PE instruction checks in `th07.exe` | original position, movement delta, hitbox, graze box, grab-item box, size-vector, option-position offsets, and speed multiplier offsets in `src/PlayerLayout.hpp`, `Player.hpp`, and tests |
| Player bomb start constants | `FUN_004083f0`, `FUN_004084f0`, `FUN_00408610`, `FUN_00408710..0040e280`, plus the `004083f0`/`00441330`/`004420b0` calls into `FUN_00406930`, decompiler output and PE instruction checks in `th07.exe` | duration/common-effect/score-drain/speed/backdrop/stage-color fields and helpers in `src/PlayerBombTables.*`; `Player.hpp` carries `+0x16a00..+0x16a14`; `DAT_004d44e0` is mapped as `DAT_004bdad8 + 0x16a08` |
| Player mode transition effect metadata | `FUN_00441960`, `FUN_00441bd0`, `FUN_00441670`, and PE instruction checks in `th07.exe` | raw mode/request values, active effect pointer offsets, `0x1c` effect spawn metadata, mode-4/mode-3 durations, and scale constants in `src/PlayerBombTables.*` |
| Effect table and manager layout | `FUN_0041c1c0`, `FUN_0041c400`, `FUN_0041c610`, `FUN_0041c790`, `FUN_0041cde0`, `0x0049efc0..0x0049f158` in `th07.exe` | 34-entry metadata, slot/layout constants, dedicated-spawn contract, draw-list offsets, stage effect ANM load table, and TH07-shaped `src/Effect.hpp`/`src/EffectManager.*` skeleton |
| TH07 draw/shake/color path | `FUN_0044f9a0`, `FUN_00450130`, `FUN_004504b0`, `FUN_0044efb0`, `FUN_0044b0e0`, `FUN_00406930`, `FUN_004069d0`, `FUN_00406de0`, and PE checks around `DAT_0062f858` | `src/AnmManager.*`, `src/ScreenEffect.cpp`, `src/Stage.*`, `src/Th07EffectTables.*` |
| Clear bonus constants/rank labels | `FUN_00427f22` | `src/ClearBonusSystem.*` |
| Cherry/Border notification labels | `FUN_00427f22` | `src/CherrySystem.*`, `BorderSystem.*` |
| TH06-style source layout | GensokyoClub/th06 source tree shape | flat `src/` plus `src/pbg3/` |
| Modern project shell | local CMake/SDL3/Vulkan scaffold | `src/Th07App.cpp` |
| Full decompiler corpus | `reference/ghidra/decomp/summary.txt`: 1574/1574 functions | `npm run ghidra-decomp` |
| Portable resource manifest | Ghidra string addresses and TH07 route tables | `src/Th07ResourceManifest.*` |

## Not Yet 100% Synchronized

| Area | Why Not Done |
| --- | --- |
| Player shot behavior | `.sht` binary records plus SHT/player callback table addresses, high-level callback semantics, the original `0x24c` ANM VM prefix layout, the position/collision field slice, and the raw mode/option bytes are decoded. `Player.hpp` now carries the leading TH07 layout bridge and `Player.cpp` seeds the low-byte mode/option/request initial state, but the callback runtime, exact SHT-derived player-size initialization, and firing behavior still need TH07 function-level replacement. |
| Bomb behavior | TH07 focused/unfocused bomb routine slots, lifecycle fields, player object base/clear span, player ANM VM prefix layout, position/collision fields, raw mode/option fields, common-effect durations, common start-effect spawn metadata, score-drain factors/minimums, score-drain-step math, common backdrop color timing/runtime color-write wiring, mode-4 stage-color write producers, mode transition effect metadata, and movement speed multipliers are decoded. The `+0x23f0/+0x23f4` speed multiplier offsets, low-byte mode/option/request initialization, `+0x16a00..+0x16a14` common-effect tail, and `+0xb7e68/+0xb7e6c` active-pointer slots are now structurally placed; mode/common-effect producers still need runtime wiring. |
| Effect manager | TH07's 34-entry effect table, slot size/counts, dedicated-spawn contract, draw-list routing, stage effect ANM loads, first callback slices, stage camera model, Stage ANM VM runtime slice, render-helper routing, `FUN_0044efb0` submit slice, `FUN_0044b0e0` shake draw offsets plus the mapped `DAT_0062f858` GameManager frame-counter gate, and the `FUN_00406930`/`004069d0`/`00406de0` color-multiplier path are anchored in portable metadata and PE/audit checks. The common bomb backdrop producer `004083f0` is wired through the Stage color writer and its `_DAT_0134cdb0 = 1` reset guard is mapped to `DAT_01347b00 + 0x52b0`; the mode-4 player producers, active effect pointer lifetime, and transition effect runtime still need source-derived wiring. Several TH07 callback bodies, exact batched `FUN_0044f690`/`FUN_0044f5c0` buffer flushing, exact x87/CRT edge-case parity for math/float comparisons, and the conditional RGB-white gate for `0041b0b0`/`0041b4a0` still need function-level replacement. |
| Item manager | Cherry, Cherry+, Border start/break/item scoring transitions are not fully reconstructed. |
| GameManager layout | TH06 layout assert is disabled; TH07 field offsets need mapping before restoring asserts. The `DAT_0062f858` frame counter is mapped as `DAT_00626270 + 0x95e8`, but the rest of the structure remains pending. |
| ResultScreen layout | Clear bonus constants are extracted, but the full score-file/result-screen structure still inherits TH06 code. |
| ECL interpreter | Resource manifests are TH07 and extracted script opcode coverage is now audited across all eight `.decl` dumps. Current evidence shows 141 unique script opcodes through opcode `161`; opcodes `136..146`, `148..161` are present in TH07 scripts but missing from the copied enum/dispatch. `config/ecl_opcode_dispatch.csv` now maps 23 of those missing enum opcodes to TH07 `FUN_00410520` switch cases, and `config/ecl_high_opcode_cases.csv` records their direct offset/global/helper-call side effects. These are metadata-only until owner fields and lifetimes are proven. Opcodes `140` and `161` remain blocked pending binary-disassembly or loader/decompiler evidence because the current decompiler switch lacks `0x8c` and `0xa1`. |
| Bullet/laser manager | TH07 bullet sheets and laser behavior need executable-backed tables and hitbox constants. |
| ANM/STD/MSG parsers | Core extracted-resource smoke checks are present, but full parser/opcode parity is not done. The current readable STD dump coverage has a known Stage 6 gap: `reference/std/stage6.dstd` is empty; the binary audit now covers `reference/extracted/stage6.std` directly with 2 objects, 4 quads, 36 instances, 58 script instructions, and opcodes `0,1,3,4,5,6,7,9,11,29,30,31`. |
| ANM index table | Confirmed slots were patched, but remaining TH06 title/select/result/music slots still need TH07 verification. |
| Replay/input/window systems | Not yet audited against TH07 functions. |
| Portable engine migration | Deterministic logic still needs replacement of Win32/D3D8-style reconstruction code with portable TH07 behavior in the flat `src/` tree. |

Next proof step: use the now-carried Player owner tail to wire the mapped mode-4 `FUN_00406930` producers and transition effects without guessing effect lifetime. In parallel, decide whether the remaining renderer migration should reconstruct the original `FUN_0044f690`/`FUN_0044f5c0` batch buffer or keep the current immediate diffuse submission as a portability boundary; `DAT_004d44e0` is known to be the original global-player `+0x16a08` alias.
