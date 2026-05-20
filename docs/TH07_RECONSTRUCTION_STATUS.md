# TH07 Reconstruction Status

Current verdict: not 100% synchronized yet.

This file tracks the gap between the current C++ reconstruction and the requested DONE condition: behavior matches TH07 static decompilation and original data.

## Confirmed Anchors

| Area | Evidence | Current Source |
| --- | --- | --- |
| Ghidra anchor xrefs | `reference/ghidra/anchors/anchor-xrefs.tsv` | `npm run ghidra-anchors` |
| Character GUI face/loading loads | `FUN_00428b19` | `src/reconstruction/original-engine/Gui.cpp` |
| Stage face/stdtxt/msg loads | `FUN_00428b19` | `src/reconstruction/original-engine/Gui.cpp` |
| ASCII/capture ANM load | decompile sample around ASCII init | `src/reconstruction/original-engine/AsciiManager.cpp` |
| Stage primary/Stage 4 extra background loads | `FUN_00407000` | `src/reconstruction/original-engine/Stage.cpp` |
| SHT path table order | xrefs through `0049f530..0049f55c` | `src/reconstruction/original-engine/ShtManager.cpp` |
| Clear bonus constants/rank labels | `FUN_00427f22` | `src/reconstruction/original-engine/ClearBonusSystem.*` |
| Cherry/Border notification labels | `FUN_00427f22` | `src/reconstruction/original-engine/CherrySystem.*`, `BorderSystem.*` |
| Modern project shell | local CMake/SDL3/Vulkan scaffold | `src/app/`, `src/game/` |

## Not Yet 100% Synchronized

| Area | Why Not Done |
| --- | --- |
| Player shot behavior | `.sht` binary fields are not decoded into firing callbacks yet; `Player.cpp` still inherits most TH06 shot/bomb control flow. |
| Bomb behavior | TH07 focused/unfocused bombs and Sakuya bombs still need function-level decompilation replacement. |
| Item manager | Cherry, Cherry+, Border start/break/item scoring transitions are not fully reconstructed. |
| GameManager layout | TH06 layout assert is disabled; TH07 field offsets need mapping before restoring asserts. |
| ResultScreen layout | Clear bonus constants are extracted, but the full score-file/result-screen structure still inherits TH06 code. |
| ECL interpreter | Resource manifests are TH07, but opcode semantics still need a TH07 opcode table and function replacement. |
| Bullet/laser manager | TH07 bullet sheets and laser behavior need executable-backed tables and hitbox constants. |
| ANM index table | Confirmed slots were patched, but remaining TH06 title/select/result/music slots still need TH07 verification. |
| Replay/input/window systems | Not yet audited against TH07 functions. |
| Portable engine migration | Deterministic logic still needs to move from Win32/D3D8-style reconstruction into `src/game/`. |

Next proof step: export/decompile the callers around `FUN_00442b70` and the player callback/function tables used by `FUN_004423e0`, then replace `Player.cpp`, `BombData.cpp`, and `.sht` parsing with source-derived code.
