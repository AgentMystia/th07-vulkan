# Reconstruction Config

Structured reconstruction metadata lives here.

Use this directory for address maps, structure-field notes, subsystem synchronization ledgers, and generated CSV/TOML files that are safe to commit. Raw Ghidra projects and proprietary original data remain under ignored `reference/`.

`subsystems.csv` is the compact dependency-ordered restoration ledger. Its `restoration_status` values must use the repository vocabulary from `AGENTS.md`: `exact`, `source-derived`, `transitional`, `metadata-only`, `pending`, `blocked`, or `obsolete-th06-carryover`.

`scripts/audit-th07-resources.mjs` is the core extracted-resource smoke audit for original/extracted coverage, ANM raw header tables, STD binary tables/scripts, readable STD dumps, MSG dumps, SFX, and BGM loop metadata. It intentionally reports `stage6.dstd` as the current readable-STD dump gap while still validating `stage6.std` through the binary parser.

`ecl_opcode_gaps.csv` tracks the TH07-used ECL opcodes present in extracted `.decl` scripts but still absent from the current copied enum/dispatch. `scripts/audit-ecl-opcodes.mjs` validates the counts, stage coverage, and first-use sites against `reference/ecl/ecldata*.decl`.

`ecl_opcode_dispatch.csv` tracks those same missing enum opcodes against the TH07 `FUN_00410520` decompiler switch. It records the source-derived high-opcode dispatch cases and keeps opcodes `140` and `161` blocked until binary disassembly or loader/decompiler evidence explains the missing cases.

`ecl_high_opcode_cases.csv` records the direct side-effect surface of the mapped high ECL cases from `FUN_00410520`: field offsets, globals, and helper calls only. It is deliberately metadata-only and must not be treated as runtime wiring or final gameplay semantics.
