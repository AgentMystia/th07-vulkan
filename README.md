# th07-vulkan

Source-level C++ reconstruction of **Touhou Youyoumu ~ Perfect Cherry Blossom / TH07** targeting a modern SDL3 + Vulkan runtime.

This project is modeled after the structure of [GensokyoClub/th06](https://github.com/GensokyoClub/th06), but the standard is different: the goal is gameplay behavior parity, not binary matching.

Original game files are required locally and are not part of this repository.

## Goals

- Reconstruct TH07 gameplay behavior from the original executable, scripts, and data.
- Keep deterministic game logic portable and separate from platform/render/audio backends.
- Build a playable cross-platform version for Linux, Windows, and Android.
- Use Linux desktop as the first validation target.

## Repository Layout

- `src/game/`: portable TH07 logic and source-derived constants.
- `src/app/`: SDL3/Vulkan application entrypoint.
- `src/reconstruction/original-engine/`: TH06-derived reconstruction baseline being replaced by TH07 evidence.
- `config/`: reverse-engineering maps and synchronization ledgers.
- `docs/`: reconstruction notes and TH06/TH07 subsystem differences.
- `scripts/`: Ghidra export, resource extraction, and audit tooling.
- `tests/`: C++ behavior/tooling tests.
- `resources/`: redistributable placeholders only.
- `reference/`: ignored local original files, extracted data, and Ghidra output.

## Local Reference Files

Place original TH07 files under `reference/original/`:

```text
reference/original/th07.exe
reference/original/th07.dat
reference/original/thbgm.dat
```

The current local workspace may also keep manuals, config files, thcrap/vpatch material, and generated extraction output under `reference/`. Those files are intentionally ignored by Git.

Useful generated evidence paths:

```text
reference/extracted/   unpacked th07.dat files
reference/ecl/         readable ECL dumps
reference/std/         readable STD dumps
reference/msg/         readable MSG dumps
reference/ghidra/      Ghidra metadata and decompiler exports
```

## Build

Linux desktop is the first supported target.

Required packages:

- CMake
- a C++20 compiler
- pkg-config
- SDL3 development files
- Vulkan loader/runtime support

```sh
cmake --preset linux-debug
cmake --build --preset linux-debug
ctest --preset linux-debug
```

The first executable is a Vulkan-capable SDL3 shell that checks the local reference-resource layout. Full gameplay is implemented subsystem by subsystem from TH07 evidence.

## Tooling

Node scripts use only built-in Node.js modules.

```sh
npm run check
npm run audit-cpp
npm run audit-resources
npm run audit-diff
npm run ghidra-export
npm run ghidra-anchors
npm run ghidra-decomp
```

Resource extraction expects `/home/mystia/下载/thtk-bin-12.zip`, `wine`, and `ffmpeg` when generating optional audio/image references:

```sh
npm run extract-resources
```

## Legal

The source code in this repository is released under CC0, following the TH06 reconstruction project's license style.

Touhou Youyoumu / TH07 and its original assets are by Team Shanghai Alice. Original game files, extracted resources, music, manuals, and Ghidra project data must not be committed to this repository.
