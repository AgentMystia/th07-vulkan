# AGENTS.md

Rules for AI agents and contributors working on this repository.

## Mission

Reconstruct Touhou Youyoumu ~ Perfect Cherry Blossom / TH07 as a C++ source-level, playable, cross-platform project.

The target is game behavior parity, not binary parity. The rebuilt game should match original TH07 gameplay for fixed-frame logic, scripts, RNG, collision, scoring, player state, Cherry/Border systems, replay-relevant state, and resource timing. Rendering, audio, input, and platform layers may be modernized to support Linux, Windows, and Android through SDL3, Vulkan, and miniaudio-compatible abstractions.

Web/PWA work is out of scope until the C++ reconstruction is behavior-complete.

## Authority Order

When sources conflict, use this priority order:

1. Current user instruction.
2. Original TH07 corpus under `reference/original/`: `th07.exe`, `th07.dat`, `thbgm.dat`, manuals, config, and related original files.
3. TH07 static reverse-engineering evidence under `reference/ghidra/`.
4. Extracted original data under `reference/extracted/`, `reference/ecl/`, `reference/std/`, and `reference/msg/`.
5. Current C++ source under `src/`.
6. TH06 / GensokyoClub `th06`, for project shape and verified shared behavior only.
7. External documentation, for cross-validation only.

`reference/` is local evidence. Do not commit original game files, extracted resources, Ghidra project data, or generated proprietary assets. Only `reference/README.md` and placeholder files may be tracked.

## Fidelity Rules

Match original TH07 gameplay for:

- frame timing, RNG, coordinates, angles, speed, acceleration
- input sampling, movement, collision, hitboxes, damage, invulnerability
- enemy scripts, boss phases, spell behavior, drops, items, scoring
- player movement, shots, bombs, death, power, lives, extends
- Cherry, Cherry+, Supernatural Border, Border Bonus, and related item/scoring behavior
- dialogue triggers, UI state, stage transitions, replay-relevant state, and sound timing

No gameplay modernization is approved by default. Debug overlays, including hitbox display, may exist only behind explicit development flags and must never change gameplay state.

## Architecture

- `src/game/`: portable TH07 behavior and deterministic data structures.
- `src/app/`: application entrypoints and platform wiring.
- `src/reconstruction/original-engine/`: TH06-derived C++ reconstruction being replaced by TH07 evidence.
- `config/`: mapping tables, reconstruction status, and structured reverse-engineering notes.
- `resources/`: redistributable placeholders and instructions only.
- `reference/`: ignored local original/evidence material.

Keep deterministic gameplay logic independent from SDL, Vulkan, miniaudio, filesystem dialogs, OS timing, and window APIs. Platform backends may call into gameplay logic, but gameplay logic must not call into platform backends.

## Work Process

Before editing behavior:

- Inspect the relevant TH07 executable/decompiler evidence or extracted original script/data.
- Compare against the matching TH06 subsystem only after TH07 evidence is identified.
- Mark the subsystem as Same, Changed, New, or Pending in `docs/TH07_DIFF_FROM_TH06.md`.
- Prefer parsed original data over hand-written recreations.

While editing:

- Keep original resources and generated evidence under `reference/`.
- Keep source changes in tracked project files outside `reference/`.
- Do not overwrite `src/reconstruction/original-engine/` from TH06 unless explicitly running the one-time import tool with intent.
- Avoid unrelated refactors; every behavior change should point to evidence.

After editing:

- Run the strongest practical validation.
- Update `docs/TH07_RECONSTRUCTION_STATUS.md` when synchronization state changes.
- State whether behavior is exact, source-derived, or still a placeholder.

## Validation

Use the strongest practical subset available:

- `cmake --preset linux-debug`
- `cmake --build --preset linux-debug`
- `ctest --preset linux-debug`
- `npm run check`
- `npm run audit-cpp`
- `npm run audit-resources`
- `npm run audit-diff`
- `npm run ghidra-anchors` when replacing behavior from static decompilation anchors

For documentation-only changes, minimal validation is acceptable.

## Repo Hygiene

Never commit or ship:

- original game archives, executables, BGM data, manuals, extracted resources, or full Ghidra projects
- secrets, tokens, credentials, browser profiles
- caches, generated logs, screenshots, build directories, or test outputs
- throwaway debug scripts

New tests, audit tools, extraction tools, CMake files, reconstruction notes, and source scaffolding may be tracked when intentional.
