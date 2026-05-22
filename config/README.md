# Reconstruction Config

Structured reconstruction metadata lives here.

Use this directory for address maps, structure-field notes, subsystem synchronization ledgers, and generated CSV/TOML files that are safe to commit. Raw Ghidra projects and proprietary original data remain under ignored `reference/`.

`runtime_replacement_manifest.csv` is the function-level runtime replacement ledger. Its forward status vocabulary is `unmapped`, `mapped`, `comparable`, `converging`, `behavior-matched`, `objdiff-close`, `objdiff-high`, `binary-exact`, `legacy-th06`, `portable-boundary`, and `blocked`. Older rows may still use `live-th07`, `function-mapped`, or `pending` while the ledger migrates; do not use those older labels for newly expanded work. `scripts/audit-th07-cpp.mjs` validates this manifest and requires the playable-executable row to keep the direct-build/direct-launch/original-resource-loading goal visible.

`player_function_manifest.csv` is the file-level ledger for the original Player function range `0x0043d0e0..0x004429d0`. It maps every original function in that range to the current source surface when one exists, and it is the checklist for moving `src/Player.cpp`/`src/Player.hpp` from unmapped or comparable into behavior-matched and objdiff-converged states.

Objdiff is a best-effort convergence instrument, not a whole-project binary-identity gate. A function can be behavior-matched with low objdiff when the remaining difference is explained by compiler, ABI, or accepted portability boundaries; high objdiff does not prove parity unless the TH07 behavior surface is mapped and validated.

`subsystems.csv` is the compact dependency-ordered restoration ledger. Its `restoration_status` values describe broad subsystem state only: `exact`, `source-derived`, `transitional`, `metadata-only`, `pending`, `blocked`, or `obsolete-th06-carryover`. A subsystem-level `source-derived` label does not mean every live runtime function in that subsystem has been replaced.

`scripts/audit-th07-resources.mjs` is the core extracted-resource smoke audit for original/extracted coverage, ANM raw header tables, ANM VM instruction/opcode-usage/argument-size coverage, STD binary tables/scripts/opcode usage and argument-size shapes, readable STD dumps, MSG readable dumps, MSG binary tables/instructions/opcode usage and argument-size shapes, SFX, and BGM loop metadata. It intentionally reports `stage6.dstd` as the current readable-STD dump gap while still validating `stage6.std` through the binary parser.

`ecl_opcode_gaps.csv` tracks the TH07-used ECL opcodes present in extracted `.decl` scripts but still absent from the current copied enum/dispatch. `scripts/audit-ecl-opcodes.mjs` validates the counts, stage coverage, and first-use sites against `reference/ecl/ecldata*.decl`.

`ecl_opcode_dispatch.csv` tracks those same missing enum opcodes against the TH07 `FUN_00410520` `opcode - 1` jump table at `0x00417838` plus the decompiler switch labels. It records jump-table entries and target addresses so decompiler case labels are not mistaken for raw opcode values; opcode `141` currently targets the common instruction-advance path without a decompiler case body.

`ecl_high_opcode_cases.csv` records the direct side-effect surface of the mapped high ECL cases from `FUN_00410520`: jump-table targets, field offsets, globals, and helper calls only. It is deliberately metadata-only and must not be treated as runtime wiring or final gameplay semantics. `src/EclOpcodeTables.*` mirrors this surface plus observed high-opcode operand signatures into compiled metadata; `scripts/audit-ecl-opcodes.mjs` validates that mirror against the CSV, readable `.decl` files, original jump table, and decompiler evidence.
