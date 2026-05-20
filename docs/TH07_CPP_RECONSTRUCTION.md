# TH07 C++ Reconstruction Notes

`src/reconstruction/original-engine/` is the TH06-derived baseline. It is not a binary-perfect target and it is not the final modern engine; it is the source-level workspace where TH06 code is replaced by TH07 evidence.

Portable, modernized gameplay code moves into `src/game/` once it is detached from Win32/D3D8 assumptions.

Current evidence roots:

- Ghidra string export: `reference/ghidra/strings.tsv`
- TH07 extracted archive: `reference/extracted/`
- readable ECL/STD/MSG: `reference/ecl/`, `reference/std/`, `reference/msg/`
- original executable SHA256:
  `35467eaf8dc7fc85f024f16fb2037255f151cefda33cf4867bc9122aaa2e80ca`
- targeted Ghidra anchor export:
  `reference/ghidra/anchors/anchor-xrefs.tsv`

## Applied TH07 Deltas

| Area | Status | Evidence |
| --- | --- | --- |
| Namespace/project identity | Changed | copied source rewritten to `th07`; TH06 title/mutex/placeholder names replaced |
| Stage routes | Changed | `stage1.std` through `stage8.std` at Ghidra strings |
| ECL scripts | Changed | `ecldata1.ecl` through `ecldata8.ecl` at Ghidra strings |
| MSG scripts | Changed | `msg1.dat` through `msg8.dat` present in Ghidra strings and extracted archive |
| Player roster | Changed | Reimu, Marisa, Sakuya; 6 shot entries |
| `.sht` shot data | New | `ply00a/b/as/bs`, `ply01a/b/as/bs`, `ply02a/b/as/bs` |
| GUI character/loading ANM | Changed | `FUN_00428b19`: `face_rm00/mr00/sk00` plus `loading/loading2/loading3` |
| GUI stage face/text/msg load | Changed | `FUN_00428b19`: `face_01_00` through `face_08_00`, `std1txt` through `std8txt`, `msg1` through `msg8` |
| ASCII resources | Changed | TH07 loads `ascii.anm` and `capture.anm`; copied TH06 `asciis.anm` dependency removed |
| Stage background ANM load | Changed | `FUN_00407000`: primary stage backgrounds use slot 5; Stage 4 additionally loads `stg4bg2-5` |
| Cherry HUD anchors | New | `CherryPoint Max!`, `Cherry = %8d` |
| Border HUD anchors | New | `Border Bonus %7d`, `Supernatural Border!!`, penalty/rank strings |
| Clear bonus constants | Changed | `FUN_00427f22`: stage, point item, graze, cherry, life, bomb, rank multipliers |
| Capture/spell table count | Changed | 141 spell names |

## Next C++ Passes

1. Export/decompile callers around SHT loading and player callback tables.
2. Replace `Player`, `BombData`, and shot callbacks with TH07 `.sht` driven behavior.
3. Map TH07 `GameManager`, `ResultScreen`, and score-file structures.
4. Audit ECL opcode differences against `reference/ecl/ecldata*.decl`.
5. Move verified deterministic logic from `src/reconstruction/original-engine/` into portable `src/game/`.
