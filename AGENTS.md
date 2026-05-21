# AGENTS.md

Rules for AI agents and contributors working on this repository.

## Current Goal

Reconstruct Touhou Youyoumu ~ Perfect Cherry Blossom / TH07 as a C++ source-level, playable, cross-platform project from the original TH07 binary, decompilation, and extracted data.

Work should advance through an evidence-first, dependency-ordered restoration loop, not by repeatedly polishing one familiar module. Prefer slices that unlock many downstream functions, replace obsolete TH06 carryover, or convert transitional code into source-derived code.

The source of truth is original TH07 behavior and data: binary behavior, data formats, fixed-frame timing, RNG, replay behavior, hitboxes, bullet patterns, scoring, stages, assets, scripts, and resource timing. The target is behavior parity, not binary parity.

Use the flat GensokyoClub/th06-style `src/` tree as the editable source workspace and as the decompilation/coding-style reference. TH06 may guide file shape, include style, naming style, and shared infrastructure only after the matching TH07 evidence is identified. TH06 must not be used to invent or approve TH07 gameplay behavior.

The port target is SDL3 + Vulkan. Replace Win32, DirectX, platform, rendering, input, and audio layers only at the boundary where portability requires it. Deterministic gameplay and simulation code must remain faithful to original TH07 behavior.

Do not guess. If evidence is missing or incomplete, keep the behavior pending or blocked, record the address/offset/script/data evidence already found, and leave the smallest practical placeholder instead of filling in a compatible approximation.

A slice is only ready for handoff when the current implemented module builds or its build gap is documented, the strongest practical validation has run, and behavior is labeled with the repository status vocabulary below.

## Authority Order

When sources conflict, use this priority order:

1. Current user instruction.
2. Original TH07 corpus under `reference/original/`: `th07.exe`, `th07.dat`, `thbgm.dat`, manuals, config, and related original files.
3. TH07 static reverse-engineering evidence under `reference/ghidra/`.
4. Extracted original data under `reference/extracted/`, `reference/ecl/`, `reference/std/`, and `reference/msg/`.
5. Current C++ source under `src/`.
6. TH06 / GensokyoClub `th06`, for decompilation reference, project shape, coding style, naming style, and verified shared behavior only.
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

No gameplay modernization is approved by default. Do not substitute a compatible approximation when original behavior can be inspected. Debug overlays, including hitbox display, may exist only behind explicit development flags and must never change gameplay state.

## Architecture

- `src/`: flat GensokyoClub/th06-style C++ source tree. TH06-derived code is replaced by TH07 evidence in place.
- `src/pbg3/`: archive reader sources kept in the same subdirectory shape as GensokyoClub/th06.
- `src/main.cpp` and `src/Th07App.cpp`: application entrypoint and SDL3/Vulkan shell.
- `docs/`: reconstruction status, subsystem differences, and project notes.
- `config/`: mapping tables and structured reverse-engineering notes.
- `scripts/`: extraction, Ghidra export, import, and audit tooling.
- `tests/`: C++ validation and evidence checks.
- `resources/`: redistributable placeholders and instructions only.
- `reference/`: ignored local original/evidence material.

Do not reintroduce `src/game/`, `src/app/`, or `src/reconstruction/original-engine/` as live source roots unless the user explicitly changes the structure goal again. Historical provenance notes may remain as normal files under the flat tree.

Keep deterministic gameplay logic independent from SDL, Vulkan, miniaudio, filesystem dialogs, OS timing, and window APIs. Platform backends may call into gameplay logic, but gameplay logic must not call into platform backends.

## Restoration Strategy

Use a dependency-ordered loop for reconstruction work:

1. Data formats, constants, tables, object layouts, and pure helpers.
2. Manager ownership, live object layout, and dispatch contracts.
3. Interpreters, script VMs, callback runtimes, and state machines.
4. Gameplay systems: resource/archive, ANM, Stage, Effect, Player, Bomb, SHT, Bullet/Laser, ECL, Enemy/Boss, Item, Cherry/Border, GameManager.
5. GUI, ResultScreen, Replay, Input, Audio, and platform/render parity boundaries.

At the start of a substantial iteration, inspect the current worktree, existing manifests/audits, and current source coverage. Choose the highest-value safe unblocked slice by dependency order. Do not keep working on one module after it is evidence-blocked while another unblocked dependency slice exists.

Prefer generated or structured tracking over prose-only planning. Use existing trackers and audits first, especially `docs/TH07_RECONSTRUCTION_STATUS.md`, `docs/TH07_DIFF_FROM_TH06.md`, `docs/TH07_CPP_RECONSTRUCTION.md`, `config/subsystems.csv`, and `scripts/audit-th07-*.mjs`. Do not create scattered new Markdown planning documents. If tracking is missing, add one compact structured manifest under `config/` or generate it from `scripts/`.

Use these status labels when tracking restoration state:

- `exact`: verified against original TH07 behavior/data for the stated scope.
- `source-derived`: directly modeled from TH07 binary, decompiler, xrefs, or extracted data, but not yet exhaustive runtime parity.
- `transitional`: live code still mixes TH06-derived shape with TH07 evidence slices.
- `metadata-only`: constants, tables, owner offsets, or helper contracts are recorded, but runtime wiring is intentionally pending.
- `pending`: not audited or not implemented yet.
- `blocked`: evidence, ownership, lifetime, dispatch, or validation is missing and must be resolved before safe wiring.
- `obsolete-th06-carryover`: copied TH06 behavior or shape that is known not to be authoritative for TH07 and should be replaced.

## Work Process

For flat `src/` structure and style maintenance:

- Treat GensokyoClub/th06 as the layout and coding-style reference.
- Move or rename files mechanically when structure work is required; avoid semantic behavior changes in the same edit.
- Update includes, CMake, scripts, docs, and tests together with any file moves.
- Keep old nested source roots deleted; do not revive them as live source roots.
- Preserve TH07-specific names and evidence-backed constants even when the surrounding style comes from TH06.

Before editing behavior:

- Inspect the relevant TH07 executable/disassembly/decompiler evidence or extracted original script/data.
- Compare against the matching TH06/GensokyoClub subsystem only after TH07 evidence is identified.
- Mark broad TH07-vs-TH06 subsystem differences as Same, Changed, New, or Pending in `docs/TH07_DIFF_FROM_TH06.md`.
- Track implementation status with the restoration labels from this file in the relevant existing tracker, compact manifest, tests, audits, or commit message.
- Prefer parsed original data over hand-written recreations.
- Identify what evidence will prove the current module exact, source-derived, or still placeholder before declaring it done.
- When evidence is incomplete, record the mapped addresses/offsets and leave runtime wiring pending instead of guessing a compatible behavior.
- If a slice fails twice for the same evidence or validation reason, mark it blocked with the missing proof and move to the next safe unblocked dependency slice.

While editing:

- Keep original resources and generated evidence under `reference/`.
- Keep source changes in tracked project files outside `reference/`.
- Do not rerun or overwrite the flat `src/` TH06 baseline import unless explicitly running `scripts/import-th06-baseline.mjs` with intent.
- Avoid unrelated refactors; every behavior change should point to evidence.
- Preserve GensokyoClub/th06 file shape and local style unless TH07 evidence or portability requires a different shape.
- Keep source-derived metadata, tests, and docs close together so the next agent can recover the evidence chain quickly.
- Update only the relevant tests, audits, manifests, and docs for the active slice. Do not polish unrelated modules during an unattended dependency-ordered run.

After editing:

- Run the strongest practical validation.
- Update `docs/TH07_RECONSTRUCTION_STATUS.md` when synchronization state changes.
- State whether behavior is exact, source-derived, or still a placeholder.
- Do not mark a module complete just because tests pass; tests are only evidence for the cases they actually cover.
- Before stopping for a handoff, leave a concise checkpoint with modified scope, validation run, known gaps, and the next evidence-backed step.
- Do not commit or discard work unless the user explicitly asks or the active long-running goal explicitly requests validated checkpoint commits. In unattended restoration loops, commit each validated slice with an evidence-focused message, and push/update a PR only when credentials and tools work non-interactively.

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
- `npm run ghidra-decomp` when refreshing the local full decompiler corpus

For structure-only work, at minimum run the checks that prove the moved paths, includes, build metadata, and audits agree. For documentation-only changes, minimal validation is acceptable.

## Repo Hygiene

Never commit or ship:

- original game archives, executables, BGM data, manuals, extracted resources, or full Ghidra projects
- secrets, tokens, credentials, browser profiles
- caches, generated logs, screenshots, build directories, or test outputs
- throwaway debug scripts

New tests, audit tools, extraction tools, CMake files, reconstruction notes, and source scaffolding may be tracked when intentional.
