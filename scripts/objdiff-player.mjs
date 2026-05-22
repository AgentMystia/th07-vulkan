#!/usr/bin/env node

import { copyFileSync, existsSync, mkdirSync, readFileSync, rmSync, statSync, writeFileSync } from 'node:fs';
import { dirname, join, resolve } from 'node:path';
import { fileURLToPath } from 'node:url';
import { spawnSync } from 'node:child_process';

const scriptDir = dirname(fileURLToPath(import.meta.url));
const root = resolve(scriptDir, '..');

const playerFileFunctions = [
  {
    address: '0043d0e0',
    name: 'FUN_0043d0e0',
    role: 'Player::OptionShotEffectCallback',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player24OptionShotEffectCallbackEPS0_PvP11D3DXVECTOR3',
  },
  {
    address: '0043d160',
    name: 'FUN_0043d160',
    role: 'Player::SpawnBullets',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player12SpawnBulletsEPS0_j',
  },
  {
    address: '0043d2f0',
    name: 'FUN_0043d2f0',
    role: 'Player::UpdatePlayerBullets',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player19UpdatePlayerBulletsEPS0_',
  },
  {
    address: '0043d690',
    name: 'FUN_0043d690',
    role: 'Player::DrawBullets',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player11DrawBulletsEPS0_',
  },
  {
    address: '0043d790',
    name: 'FUN_0043d790',
    role: 'Player::DrawBulletExplosions',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player20DrawBulletExplosionsEPS0_',
  },
  {
    address: '0043d880',
    name: 'FUN_0043d880',
    role: 'Player::UpdateFireBulletsTimer',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player22UpdateFireBulletsTimerEPS0_',
  },
  {
    address: '0043d990',
    name: 'FUN_0043d990',
    role: 'Player::StartFireBulletTimer',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player20StartFireBulletTimerEPS0_',
  },
  {
    address: '0043d9e0',
    name: 'FUN_0043d9e0',
    role: 'Player::CalcDamageToEnemy',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player17CalcDamageToEnemyEP11D3DXVECTOR3S2_Pi',
  },
  {
    address: '0043e0a0',
    name: 'FUN_0043e0a0',
    role: 'Player::CalcBombCollision',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player17CalcBombCollisionEP11D3DXVECTOR3S2_',
  },
  {
    address: '0043e260',
    name: 'FUN_0043e260',
    role: 'Player::CalcKillBoxCollision',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player20CalcKillBoxCollisionEP11D3DXVECTOR3S2_',
  },
  {
    address: '0043e3b0',
    name: 'FUN_0043e3b0',
    role: 'Player::CheckGraze',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player10CheckGrazeEP11D3DXVECTOR3S2_',
  },
  {
    address: '0043e4e0',
    name: 'FUN_0043e4e0',
    role: 'Player::CalcItemBoxCollision',
    status: 'behavior-matched',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player20CalcItemBoxCollisionEP11D3DXVECTOR3S2_',
  },
  {
    address: '0043e6b0',
    name: 'FUN_0043e6b0',
    role: 'Player::CalcLaserHitbox',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player15CalcLaserHitboxEP11D3DXVECTOR3S2_S2_fi',
  },
  {
    address: '0043eb90',
    name: 'FUN_0043eb90',
    role: 'Player::ScoreGraze',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player10ScoreGrazeEP11D3DXVECTOR3',
  },
  {
    address: '0043edc0',
    name: 'FUN_0043edc0',
    role: 'Player::Die',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player3DieEv',
  },
  {
    address: '0043ee50',
    name: 'FUN_0043ee50',
    role: 'Player::HandlePlayerInputs',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player18HandlePlayerInputsEv',
  },
  {
    address: '00440940',
    name: 'FUN_00440940',
    role: 'Player::UpdateBombCollisionSlots',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player24UpdateBombCollisionSlotsEv',
  },
  {
    address: '004409f0',
    name: 'FUN_004409f0',
    role: 'bomb update dispatch',
    status: 'unmapped',
  },
  {
    address: '00440cf0',
    name: 'FUN_00440cf0',
    role: 'mode 2 respawn update',
    status: 'unmapped',
  },
  {
    address: '004411c0',
    name: 'FUN_004411c0',
    role: 'Player::UpdateMode1Invulnerable',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player23UpdateMode1InvulnerableEPS0_',
  },
  {
    address: '00441330',
    name: 'FUN_00441330',
    role: 'mode update branch',
    status: 'mapped',
  },
  {
    address: '00441670',
    name: 'FUN_00441670',
    role: 'mode cleanup',
    status: 'mapped',
  },
  {
    address: '00441800',
    name: 'FUN_00441800',
    role: 'Player::RegisterBombCollisionRect',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player25RegisterBombCollisionRectEP11D3DXVECTOR3ffj',
  },
  {
    address: '004418b0',
    name: 'FUN_004418b0',
    role: 'Player::RegisterBombCollisionCircle',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player27RegisterBombCollisionCircleEP11D3DXVECTOR3ffij',
  },
  {
    address: '00441960',
    name: 'FUN_00441960',
    role: 'mode 4 entry',
    status: 'mapped',
  },
  {
    address: '00441bd0',
    name: 'FUN_00441bd0',
    role: 'mode 3 entry',
    status: 'mapped',
  },
  {
    address: '00441e80',
    name: 'FUN_00441e80',
    role: 'Player::ResetOptionFields',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player17ResetOptionFieldsEPS0_',
  },
  {
    address: '00441fb0',
    name: 'FUN_00441fb0',
    role: 'Player::OnUpdate',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player8OnUpdateEPS0_',
  },
  {
    address: '004420b0',
    name: 'FUN_004420b0',
    role: 'Player::OnDrawHighPrio',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player14OnDrawHighPrioEPS0_',
  },
  {
    address: '00442350',
    name: 'FUN_00442350',
    role: 'Player::OnDrawLowPrio',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player13OnDrawLowPrioEPS0_',
  },
  {
    address: '00442370',
    name: 'FUN_00442370',
    role: 'Player::AngleFromPlayer',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player15AngleFromPlayerEP11D3DXVECTOR3',
  },
  {
    address: '004423e0',
    name: 'FUN_004423e0',
    role: 'Player::AddedCallback',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player13AddedCallbackEPS0_',
  },
  {
    address: '004428e0',
    name: 'FUN_004428e0',
    role: 'Player::DeletedCallback',
    status: 'comparable',
    currentUnit: 'player',
    currentSymbol: '_ZN4th076Player15DeletedCallbackEPS0_',
  },
  {
    address: '004429d0',
    name: 'FUN_004429d0',
    role: 'ComputePlayerMode4StageColor',
    status: 'comparable',
    currentUnit: 'playerBombTables',
    currentSymbol: '_ZN4th0728ComputePlayerMode4StageColorEi',
  },
];

const playerFunctions = playerFileFunctions.filter((fn) => fn.currentUnit && fn.currentSymbol);

const args = process.argv.slice(2);
const currentArgIndex = args.indexOf('--current');
const customCurrentObject = currentArgIndex === -1
  ? ''
  : resolve(root, args[currentArgIndex + 1] || '');

const objdiff = findExecutable('objdiff-cli');
const clang = findExecutable('clang');
const cxx = findExecutable(process.env.CXX || 'c++');
const objcopy = findExecutable('llvm-objcopy') || findExecutable('objcopy');
const th07Exe = join(root, 'reference/original/th07.exe');
const functionTable = join(root, 'reference/ghidra/functions.tsv');
const outDir = join(root, 'build/objdiff-player');
const asmPath = join(outDir, 'player-original.s');
const targetObject = join(outDir, 'player-original.o');
const currentPlayerObject = join(outDir, 'player-current.o');
const currentPlayerNamedObject = join(outDir, 'player-current-renamed.o');
const currentPlayerBombTablesObject = join(outDir, 'player-bombtables-current.o');
const currentPlayerBombTablesNamedObject = join(outDir, 'player-bombtables-current-renamed.o');

const result = {
  status: 'unchecked',
  reason: '',
  targetObject,
  currentObjects: {},
  playerFileFunctions,
  checkedFunctions: playerFunctions,
  unmappedFunctions: playerFileFunctions.filter((fn) => !fn.currentUnit || !fn.currentSymbol),
  objdiffSmoke: null,
  diffs: [],
};

try {
  if (!objdiff) fail('objdiff-cli was not found in PATH');
  if (!clang) fail('clang was not found in PATH');
  if (!cxx) fail(`${process.env.CXX || 'c++'} was not found in PATH`);
  if (!objcopy) fail('llvm-objcopy/objcopy was not found in PATH');
  if (!existsSync(th07Exe)) fail(`missing original executable: ${relative(th07Exe)}`);
  if (!existsSync(functionTable)) fail(`missing Ghidra function table: ${relative(functionTable)}`);

  mkdirSync(outDir, { recursive: true });
  writeFileSync(asmPath, buildOriginalAssembly(parseFunctionTable(functionTable)), 'utf8');
  runChecked(clang, ['-m32', '-x', 'assembler', '-c', asmPath, '-o', targetObject], 'assemble original Player object');

  result.objdiffSmoke = runObjdiff(targetObject, targetObject, 'FUN_0043ee50');
  if (result.objdiffSmoke.status !== 'ok') {
    fail(`objdiff could not diff the generated original object: ${result.objdiffSmoke.error}`);
  }

  const currentObjects = customCurrentObject
    ? { player: customCurrentObject, playerBombTables: customCurrentObject }
    : buildCurrentObjects();
  result.currentObjects = currentObjects;

  for (const fn of playerFunctions) {
    const currentObject = currentObjects[fn.currentUnit];
    result.diffs.push({
      function: fn,
      currentObject,
      diff: runObjdiff(targetObject, currentObject, fn.name),
    });
  }

  const failed = result.diffs.filter((entry) => entry.diff.status !== 'ok');
  result.status = failed.length === 0 ? 'checked' : 'blocked';
  if (failed.length !== 0) {
    result.reason = 'objdiff ran, but one or more Player function symbols could not be compared';
    process.exitCode = 1;
  }
} catch (error) {
  result.status = 'blocked';
  result.reason = error instanceof Error ? error.message : String(error);
  process.exitCode = 1;
} finally {
  console.log(JSON.stringify(result, null, 2));
}

function buildCurrentObjects() {
  compileCurrent('Player.cpp', currentPlayerObject);
  copyFileSync(currentPlayerObject, currentPlayerNamedObject);
  redefineSymbols(currentPlayerNamedObject, playerFunctions.filter((fn) => fn.currentUnit === 'player'));

  compileCurrent('PlayerBombTables.cpp', currentPlayerBombTablesObject);
  copyFileSync(currentPlayerBombTablesObject, currentPlayerBombTablesNamedObject);
  redefineSymbols(currentPlayerBombTablesNamedObject, playerFunctions.filter((fn) => fn.currentUnit === 'playerBombTables'));

  return {
    player: currentPlayerNamedObject,
    playerBombTables: currentPlayerBombTablesNamedObject,
  };
}

function compileCurrent(sourceFile, outputObject) {
  runChecked(cxx, [
    '-m32',
    '-march=i486',
    '-std=c++20',
    '-fpermissive',
    '-fno-pic',
    '-fno-pie',
    '-fno-asynchronous-unwind-tables',
    '-fno-exceptions',
    '-fno-stack-protector',
    '-DTH07_PLAYER_OBJDIFF',
    '-Isrc/compat',
    '-Isrc',
    '-c',
    `src/${sourceFile}`,
    '-o',
    outputObject,
  ], `compile current ${sourceFile} objdiff object`);
}

function redefineSymbols(objectPath, functions) {
  const args = [];
  for (const fn of functions) {
    args.push('--redefine-sym', `${fn.currentSymbol}=${fn.name}`);
  }
  args.push(objectPath);
  runChecked(objcopy, args, `rename current symbols in ${relative(objectPath)}`);
}

function buildOriginalAssembly(entries) {
  const lines = [
    '.intel_syntax noprefix',
    '.section .text,"ax",@progbits',
  ];
  for (const fn of playerFileFunctions) {
    const address = Number.parseInt(fn.address, 16);
    const next = nextFunctionAddress(entries, address);
    const size = next - address;
    const fileOffset = address - 0x00401000 + 0x00000400;
    lines.push(
      `.globl ${fn.name}`,
      `.type ${fn.name}, @function`,
      `${fn.name}:`,
      `.incbin "${escapeAsmPath(th07Exe)}", ${fileOffset}, ${size}`,
      `.size ${fn.name}, . - ${fn.name}`,
    );
  }
  lines.push('');
  return lines.join('\n');
}

function parseFunctionTable(path) {
  return readFileSync(path, 'utf8')
    .trim()
    .split(/\r?\n/)
    .slice(1)
    .map((line) => {
      const [name, address] = line.split('\t');
      return { name, address: Number.parseInt(address, 16) };
    })
    .filter((entry) => Number.isFinite(entry.address))
    .sort((a, b) => a.address - b.address);
}

function nextFunctionAddress(entries, address) {
  const index = entries.findIndex((entry) => entry.address === address);
  if (index === -1) fail(`function address not found in reference/ghidra/functions.tsv: ${address.toString(16)}`);
  if (index + 1 >= entries.length) fail(`no following function address for ${address.toString(16)}`);
  return entries[index + 1].address;
}

function runObjdiff(target, base, symbol) {
  const outputPath = join(outDir, `${symbol}.json`);
  rmSync(outputPath, { force: true });
  const proc = spawnSync(objdiff, [
    'diff',
    '-1', target,
    '-2', base,
    '-o', outputPath,
    '--format', 'json',
    symbol,
  ], {
    cwd: root,
    encoding: 'utf8',
  });
  if (proc.status !== 0) {
    return {
      status: 'error',
      error: proc.stderr.trim() || proc.stdout.trim() || `exit ${proc.status}`,
    };
  }
  const summary = summarizeObjdiff(outputPath, symbol);
  return {
    status: 'ok',
    output: outputPath,
    ...summary,
  };
}

function summarizeObjdiff(outputPath, symbol) {
  const parsed = JSON.parse(readFileSync(outputPath, 'utf8'));
  const leftSymbol = parsed.left?.symbols?.find((entry) => entry.name === symbol);
  const rightSymbol = parsed.right?.symbols?.find((entry) => entry.name === symbol);
  return {
    matchPercent: leftSymbol?.match_percent ?? rightSymbol?.match_percent ?? null,
    originalSize: leftSymbol?.size ?? null,
    currentSize: rightSymbol?.size ?? null,
    originalInstructions: leftSymbol?.instructions?.length ?? null,
    currentInstructions: rightSymbol?.instructions?.length ?? null,
  };
}

function runChecked(command, args, label) {
  const proc = spawnSync(command, args, { cwd: root, encoding: 'utf8' });
  if (proc.status !== 0) {
    fail(`${label} failed: ${proc.stderr.trim() || proc.stdout.trim() || `exit ${proc.status}`}`);
  }
}

function findExecutable(name) {
  for (const dir of (process.env.PATH || '').split(':')) {
    const candidate = join(dir, name);
    if (!existsSync(candidate)) continue;
    const mode = statSync(candidate).mode;
    if ((mode & 0o111) !== 0) return candidate;
  }
  return '';
}

function fail(message) {
  throw new Error(message);
}

function relative(path) {
  return path.startsWith(root) ? path.slice(root.length + 1) : path;
}

function escapeAsmPath(path) {
  return path.replace(/\\/g, '\\\\').replace(/"/g, '\\"');
}
