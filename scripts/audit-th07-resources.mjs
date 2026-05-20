import assert from 'node:assert/strict';
import { existsSync, readdirSync } from 'node:fs';
import { join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));

const expectedOriginal = [
  'th07.exe',
  'th07.dat',
  'thbgm.dat',
];

const expectedExtracted = [
  ...Array.from({ length: 8 }, (_, i) => `ecldata${i + 1}.ecl`),
  ...Array.from({ length: 8 }, (_, i) => `stage${i + 1}.std`),
  ...Array.from({ length: 8 }, (_, i) => `msg${i + 1}.dat`),
  ...Array.from({ length: 8 }, (_, i) => `std${i + 1}txt.anm`),
  ...Array.from({ length: 8 }, (_, i) => `stg${i + 1}bg.anm`),
  ...Array.from({ length: 8 }, (_, i) => `stg${i + 1}enm.anm`),
  'ascii.anm', 'capture.anm', 'front.anm', 'etama.anm',
  'player00.anm', 'player01.anm', 'player02.anm',
  'ply00a.sht', 'ply00as.sht', 'ply00b.sht', 'ply00bs.sht',
  'ply01a.sht', 'ply01as.sht', 'ply01b.sht', 'ply01bs.sht',
  'ply02a.sht', 'ply02as.sht', 'ply02b.sht', 'ply02bs.sht',
  'thbgm.fmt'
];

function requireFile(path) {
  assert.ok(existsSync(path), `Missing ${path}`);
}

for (const file of expectedOriginal) {
  requireFile(join(root, 'reference/original', file));
}

for (const file of expectedExtracted) {
  requireFile(join(root, 'reference/extracted', file));
}

for (let stage = 1; stage <= 8; stage++) {
  requireFile(join(root, 'reference/ecl', `ecldata${stage}.decl`));
  requireFile(join(root, 'reference/std', `stage${stage}.dstd`));
  requireFile(join(root, 'reference/msg', `msg${stage}.txt`));
}

const sfxDir = join(root, 'reference/generated/sfx');
const sfx = existsSync(sfxDir)
  ? readdirSync(sfxDir).filter((name) => /^se_.*\.wav$/i.test(name))
  : [];
assert.ok(sfx.length >= 29, `Expected TH07 SFX files, found ${sfx.length}`);

const bgmLoop = join(root, 'reference/generated/audio/th07-bgm-loop.json');
requireFile(bgmLoop);

console.log(JSON.stringify({
  originalFiles: expectedOriginal.length,
  extractedFiles: expectedExtracted.length,
  eclDumps: 8,
  stdDumps: 8,
  msgDumps: 8,
  sfx: sfx.length,
}, null, 2));
