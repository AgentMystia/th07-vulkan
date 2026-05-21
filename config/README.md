# Reconstruction Config

Structured reconstruction metadata lives here.

Use this directory for address maps, structure-field notes, subsystem synchronization ledgers, and generated CSV/TOML files that are safe to commit. Raw Ghidra projects and proprietary original data remain under ignored `reference/`.

`subsystems.csv` is the compact dependency-ordered restoration ledger. Its `restoration_status` values must use the repository vocabulary from `AGENTS.md`: `exact`, `source-derived`, `transitional`, `metadata-only`, `pending`, `blocked`, or `obsolete-th06-carryover`.
