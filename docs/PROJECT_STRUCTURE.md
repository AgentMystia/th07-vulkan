# Project Structure

This project is now a C++ reconstruction project, not a Web/PWA runtime.

## Tracked Project Surface

- `src/`: flat TH06/GensokyoClub-style C++ source tree, with TH07 evidence replacing the TH06-derived baseline in place.
- `src/pbg3/`: archive reader sources kept in the same subdirectory shape as GensokyoClub/th06.
- `src/Th07App.cpp`: SDL3/Vulkan application shell used by the modern CMake target.
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
- `th07-sht-tests`: `.sht` parser and callback-table evidence test executable.
- `th07-player-layout-tests`: platform-independent Player layout constant evidence test executable.

Current CMake coverage is intentionally narrower than the full flat `src/` tree: it builds the portable evidence library, SDL3/Vulkan shell, and tests. The TH06-style runtime files that still include Win32/D3D8 headers, including `src/Stage.cpp` and `src/Player.cpp`, are checked by audits and evidence tests but are not compiled by the Linux CMake target until their platform dependencies are replaced or shimmed. `src/PlayerLayout.hpp` is kept platform-independent so layout constants can stay in CMake coverage while `Player.hpp` still carries legacy D3D-era dependencies.

## Scripts

- `scripts/extract-th07-resources.mjs`: unpack resources and generate ignored local reference material.
- `scripts/import-th06-baseline.mjs`: one-time TH06 baseline import into `src/`.
- `scripts/audit-th07-cpp.mjs`: verify known TH07 C++ reconstruction anchors.
- `scripts/audit-th07-resources.mjs`: verify local original/extracted/reference data.
- `scripts/audit-th07-vs-th06.mjs`: enforce the TH07-vs-TH06 diff map.
- `scripts/export-th07-ghidra.mjs`: run Ghidra headless metadata export.
- `scripts/export-th07-anchor-xrefs.mjs`: export Ghidra xrefs and decompiled callers for TH07 anchors.
- `scripts/export-th07-decomp.mjs`: export full-function decompiler output to ignored `reference/ghidra/decomp/`.
