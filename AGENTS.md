# AGENTS.md

Rules for AI agents and contributors working on this repository.

## Current Goal

Reconstruct Touhou Youyoumu ~ Perfect Cherry Blossom / TH07 as a source-level C++ project by converging every original TH07 runtime function to behavior-level parity, one comparable function at a time.

The current project phase is not the SDL3/Vulkan port. The current phase is function recovery and convergence. SDL3/Vulkan is a later portability phase that begins only after the live gameplay/runtime functions have TH07 behavior parity or an explicitly accepted portability-boundary exception.

The finished reconstruction should operate like the GensokyoClub/th06 project: build directly from the repository, produce a directly launchable executable, load original TH07 resources from the user's local resource path, and play with TH07 behavior. That operational goal does not mean the modern build must be binary-identical to the original TH07 executable.

Binary matching is best effort. Use objdiff wherever a current object can be made comparable, and keep improving the match when doing so does not harm source clarity or behavior. Do not make whole-project `97%` or `100%` binary identity a completion gate. The hard gate is TH07 behavior-level parity, backed by original evidence and tracked per function.

TH06 may guide file shape, include style, naming style, and known shared infrastructure only after the matching TH07 evidence is identified. TH06 must never approve TH07 gameplay, timing, scoring, resources, scripts, object layouts, RNG/timer behavior, or dispatch behavior.

Current uncommitted reconstruction work is intentional. Do not revert, clean, discard, or rewrite unrelated work unless the user explicitly asks.

## Authority Order

When sources conflict, use this priority order:

1. Current user instruction.
2. Original TH07 corpus under `reference/original/`: `th07.exe`, `th07.dat`, `thbgm.dat`, config, manuals, and related original files.
3. TH07 reverse-engineering evidence under `reference/ghidra/`, especially decompiler output, functions tables, xrefs, anchors, and PE-byte checks.
4. Extracted original data under `reference/extracted/`, `reference/ecl/`, `reference/std/`, and `reference/msg/`.
5. Current source under `src/`, only for already-converged TH07 behavior or explicitly marked transition code.
6. TH06 / GensokyoClub `th06`, for structure/style only after TH07 evidence is known.
7. External documentation, for cross-validation only.

`reference/` is local evidence. Do not commit original game files, extracted proprietary resources, full Ghidra projects, generated proprietary assets, secrets, credentials, caches, build directories, or throwaway debug outputs.

## Function Status

Track live runtime replacement in `config/runtime_replacement_manifest.csv`. This ledger is stricter than subsystem docs. A subsystem note such as `source-derived` does not mean its live functions are complete.

Use these function-level states going forward:

- `unmapped`: the original function or behavior surface is not mapped.
- `mapped`: original address, owner fields, inputs/outputs, and side effects are identified, but current code is not comparable or wired.
- `comparable`: original and current objects can be compared with objdiff for the stated function.
- `converging`: current code is being changed toward the original and has a recorded objdiff/behavior baseline.
- `behavior-matched`: behavior, side effects, data flow, timer/RNG use, and replay-relevant state match TH07 evidence for the stated scope.
- `objdiff-close`: behavior is matched and objdiff is meaningfully close; use this only for functions where the object comparison supports it.
- `objdiff-high`: behavior is matched and objdiff is very close; this is a stretch quality label, not a global requirement.
- `binary-exact`: current comparable output is effectively identical for that function. Expect this only for small helpers.
- `legacy-th06`: live behavior is still TH06 carryover.
- `portable-boundary`: code intentionally differs from TH07's Win32/DirectX/DirectSound implementation only at an accepted backend boundary.
- `blocked`: convergence cannot proceed safely until named evidence, ownership, lifetime, or validation is found.

Existing rows may still contain older labels such as `live-th07`, `function-mapped`, or `pending` while the ledger migrates. Do not add new rows with those older labels unless you are preserving a nearby row during a narrow edit.

## Behavior Parity

Behavior-level parity means the current implementation matches the original TH07 function for:

- function boundary, dispatch path, caller-visible return values, and error/fallback behavior
- object/global field reads and writes, including aliasing and clear/copy spans
- allocation, free, ownership, list/slot lifetime, and callback registration
- frame timing, subframe timer behavior, force-step flags, and scheduler effects
- RNG calls, seed transitions, angle normalization, and float/int conversion semantics
- input sampling, movement priority, bounds, collision, graze, item collection, damage, death, and invulnerability
- SHT, ANM, STD, MSG, ECL, effect, item, bullet, laser, boss, score, Cherry/Border, replay, and result side effects
- resource load/free order, sound timing, dialogue state, GUI state, and stage transitions

Tests and objdiff are evidence. They are not a substitute for identifying the original function's full behavior surface. Passing tests does not by itself make a function `behavior-matched`.

## Objdiff Policy

For every function being replaced:

1. Identify the original TH07 address/name and size from `reference/ghidra/functions.tsv` or stronger evidence.
2. Produce or reuse an original object slice from `reference/original/th07.exe`.
3. Compile a current comparable object whenever practical. Diff-only compatibility shims are allowed when they do not become normal runtime dependencies.
4. Rename current symbols to original `FUN_...` names for comparison when needed.
5. Record match percent, compared object path, blockers, and the strongest behavior validation in the manifest or nearby tracker.
6. Improve objdiff as best effort after behavior is correct. Prefer behavior and source mapping over register-allocation games.

Low objdiff does not automatically fail a function if the remaining difference is explained by compiler, ABI, backend, or accepted portability-boundary differences. High objdiff does not automatically prove behavior if important side effects are untested or unmapped.

## Source Tree Rules

Keep `src/` flat, matching the TH06 project style. Do not introduce `src/game/`, `src/app/`, `src/reconstruction/`, or other live source roots unless the user explicitly changes the structure goal.

Clean `src/` by ownership, not by cosmetic preference:

- Runtime behavior belongs in the module that owns the original function.
- Temporary evidence files such as `Th07*Tables`, `*Layout`, and `*Contract` are allowed only while they help convergence. Merge them into the owning module once the live function is behavior-matched.
- Independent binary-format readers may remain separate when the format boundary is real: ANM, STD, MSG, ECL, SHT, archive, and similar parsers.
- `src/compat/` is for diff-only compilation and short-lived convergence shims. Do not let it become the normal portability layer.
- `src/pbg3/` is historical/archive-reader lineage. Prefer TH07 archive evidence for live resource loading.
- Do not do broad file moves together with behavior changes. Move/merge mechanically first, validate, then change behavior.

When deleting or merging files, first prove they are unused or fully absorbed: includes, CMake, tests, scripts, audits, docs, and manifest rows must move together.

## Port Boundary

SDL3/Vulkan, miniaudio, host filesystem dialogs, OS timers, and window/event handling are port work. During the function-convergence phase:

- Keep deterministic gameplay independent from SDL, Vulkan, audio backend lifetime, window events, host timing, and platform file dialogs.
- Preserve only the minimal application shell needed for build/resource/headless smoke checks.
- Do not expand renderer/input/audio features as a substitute for TH07 function convergence.
- Mark unavoidable backend substitutions as `portable-boundary` and describe the original function they replace.

## Work Loop

Use this loop for each slice:

1. Pick the next unblocked function by dependency order.
2. Read TH07 evidence before editing current code.
3. Add or update the manifest row with address, current surface, status, evidence, blocker, next action, and validation.
4. Make the current function comparable with objdiff if practical.
5. Replace behavior from TH07 evidence, preserving call order and side effects.
6. Run the strongest practical validation.
7. Update manifest/docs only with durable facts: behavior status, objdiff baseline, known gaps, and next function.

Dependency order:

1. File/archive/resource loading, binary parsers, constants, object layouts, and pure helpers.
2. Manager ownership, allocation/free lifetime, dispatch contracts, and live object layout.
3. Script VMs and interpreters: ANM, STD, MSG, ECL, SHT callbacks.
4. Gameplay systems: Player, Bomb, Bullet/Laser, Enemy/Boss, Item, Cherry/Border, Effect, Stage, GameManager, ResultScreen.
5. Replay, input sampling, score-file, audio timing, GUI completion, and accepted backend boundaries.

If the same slice is blocked twice for the same missing proof, mark it `blocked` with the missing evidence and move to the next unblocked function.

## Validation

Use the strongest practical subset available:

- `git diff --check`
- `npm run check`
- `npm run audit-cpp`
- `npm run audit-resources`
- `npm run audit-diff`
- `node scripts/audit-ecl-opcodes.mjs` when ECL tables or interpreter surfaces change
- `npm run objdiff-player` for Player-related comparable-object work
- future module objdiff scripts when added
- `npm run ghidra-anchors` when replacing behavior from static decompilation anchors
- `cmake --preset linux-debug`
- `cmake --build --preset linux-debug`
- `ctest --preset linux-debug`
- `build/linux-debug/th07-vulkan --check-resources` when resource/archive paths change
- `build/linux-debug/th07-vulkan --headless-smoke` when startup/resource behavior changes
- `npm run ghidra-decomp` only when refreshing the local full decompiler corpus is intentional

The playable bar is higher than the audit bar. The repository is not done until the normal build produces a launchable executable that loads original TH07 resources and runs playable TH07 game flow with TH07 behavior replacing TH06 carryover.

## Git And Hygiene

Do not run `git add`, `git commit`, `git push`, PR creation/update, branch rewrites, rebases, or destructive git commands unless the user explicitly asks.

Do not revert or discard existing work you did not make. If unrelated files are dirty, leave them alone. If a dirty file affects your task, understand and work with the existing changes.

Never commit or ship:

- original game archives, executables, BGM data, manuals, extracted resources, or full Ghidra projects
- secrets, tokens, credentials, browser profiles
- caches, generated logs, screenshots, build directories, or test outputs
- throwaway debug scripts

When stopping, leave a concise checkpoint: changed scope, functions/status rows touched, TH07 evidence used, objdiff result if any, validations run, known blockers, and the next function-level step.
