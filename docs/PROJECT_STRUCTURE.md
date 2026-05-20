# Project Structure

This project is now a C++ reconstruction project, not a Web/PWA runtime.

## Tracked Project Surface

- `src/game/`: portable TH07 behavior constants and deterministic game logic.
- `src/app/`: SDL3/Vulkan application entrypoint and platform wiring.
- `src/reconstruction/original-engine/`: TH06-derived C++ source baseline being replaced with TH07 evidence.
- `config/`: structured maps and synchronization ledgers safe to commit.
- `resources/`: redistributable placeholders and resource-loading notes.
- `scripts/`: Ghidra export, resource extraction, C++ import, and audit tooling.
- `tests/`: C++ tests.
- `docs/`: reconstruction status, subsystem differences, and project notes.

## Local Reference Surface

- `reference/original/`: original TH07 files such as `th07.exe`, `th07.dat`, `thbgm.dat`, manuals, and local config.
- `reference/extracted/`: unpacked `th07.dat` entries.
- `reference/ecl/`: readable ECL dumps.
- `reference/std/`: readable STD dumps.
- `reference/msg/`: readable MSG dumps.
- `reference/ghidra/`: Ghidra metadata, strings, function lists, and decompiler exports.
- `reference/generated/`: optional generated audio/images/SFX used for inspection.

Only placeholders and `reference/README.md` are tracked from `reference/`.

## Build Surface

- `CMakeLists.txt`: modern C++ build.
- `CMakePresets.json`: first preset is `linux-debug`.
- `th07_game`: portable C++ library target.
- `th07-vulkan`: SDL3/Vulkan desktop shell target.
- `th07-tests`: C++ test executable.

## Scripts

- `scripts/extract-th07-resources.mjs`: unpack resources and generate ignored local reference material.
- `scripts/import-th06-baseline.mjs`: one-time TH06 baseline import into `src/reconstruction/original-engine/`.
- `scripts/audit-th07-cpp.mjs`: verify known TH07 C++ reconstruction anchors.
- `scripts/audit-th07-resources.mjs`: verify local original/extracted/reference data.
- `scripts/audit-th07-vs-th06.mjs`: enforce the TH07-vs-TH06 diff map.
- `scripts/export-th07-ghidra.mjs`: run Ghidra headless metadata export.
- `scripts/export-th07-anchor-xrefs.mjs`: export Ghidra xrefs and decompiled callers for TH07 anchors.
- `scripts/export-th07-decomp.mjs`: export full-function decompiler output to ignored `reference/ghidra/decomp/`.
