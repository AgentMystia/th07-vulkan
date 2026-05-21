import assert from 'node:assert/strict';
import { existsSync, readdirSync, readFileSync } from 'node:fs';
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

const anmFiles = readdirSync(join(root, 'reference/extracted'))
  .filter((name) => name.endsWith('.anm'))
  .sort((a, b) => a.localeCompare(b));
assert.ok(anmFiles.length >= 50, `Expected substantive ANM coverage, found ${anmFiles.length}`);

const anmSummaries = anmFiles.map((file) => auditAnmFile(join(root, 'reference/extracted', file), file));
const anmEntries = sumBy(anmSummaries, 'entries');
const anmSprites = sumBy(anmSummaries, 'sprites');
const anmScripts = sumBy(anmSummaries, 'scripts');
assert.ok(anmEntries >= anmFiles.length, 'Each ANM file should expose at least one entry header');
assert.ok(anmSprites > 100, 'Expected substantive ANM sprite coverage');
assert.ok(anmScripts > 100, 'Expected substantive ANM script coverage');

const stdSummaries = [];
for (let stage = 1; stage <= 8; stage++) {
  const file = `stage${stage}.dstd`;
  const text = readFileSync(join(root, 'reference/std', file), 'utf8');
  stdSummaries.push({
    file,
    lines: countLines(text),
    entries: countMatches(text, /^ENTRY:/gm),
    quads: countMatches(text, /^\s+QUAD:/gm),
    faces: countMatches(text, /^\s+FACE:/gm),
  });
}
const emptyStdDumps = stdSummaries
  .filter((summary) => summary.lines === 0)
  .map((summary) => summary.file);
assert.deepEqual(emptyStdDumps, ['stage6.dstd'],
  'Readable STD dump coverage changed; update the reconstruction status before relying on it');

const stdBinarySummaries = [];
for (let stage = 1; stage <= 8; stage++) {
  const file = `stage${stage}.std`;
  stdBinarySummaries.push(auditStdBinary(join(root, 'reference/extracted', file), file));
}
assert.equal(sumBy(stdBinarySummaries, 'objects'), 45, 'STD binary object coverage changed');
assert.equal(sumBy(stdBinarySummaries, 'quads'), 162, 'STD binary quad coverage changed');
assert.equal(sumBy(stdBinarySummaries, 'instances'), 906, 'STD binary object-instance coverage changed');
assert.equal(sumBy(stdBinarySummaries, 'scriptInstructions'), 378, 'STD binary script coverage changed');
const stage6StdBinary = stdBinarySummaries.find((summary) => summary.file === 'stage6.std');
assert.deepEqual(stage6StdBinary, {
  file: 'stage6.std',
  objects: 2,
  declaredQuads: 4,
  quads: 4,
  instances: 36,
  scriptInstructions: 58,
  opcodes: [0, 1, 3, 4, 5, 6, 7, 9, 11, 29, 30, 31],
}, 'Stage 6 STD binary shape changed; update the readable-dump blocker and parser notes');

const msgSummaries = [];
for (let stage = 1; stage <= 8; stage++) {
  const file = `msg${stage}.txt`;
  const text = readFileSync(join(root, 'reference/msg', file), 'utf8');
  const entries = countMatches(text, /^entry\s+\d+/gm);
  const timelineLabels = countMatches(text, /^@\d+/gm);
  assert.ok(entries > 0, `${file} should contain dialogue entries`);
  assert.ok(timelineLabels > 0, `${file} should contain timeline labels`);
  msgSummaries.push({
    file,
    lines: countLines(text),
    entries,
    timelineLabels,
  });
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
  anmFiles: anmFiles.length,
  anmEntries,
  anmSprites,
  anmScripts,
  eclDumps: 8,
  stdDumps: 8,
  stdReadableDumps: stdSummaries.length - emptyStdDumps.length,
  stdEmptyDumps: emptyStdDumps,
  stdBinaryObjects: sumBy(stdBinarySummaries, 'objects'),
  stdBinaryQuads: sumBy(stdBinarySummaries, 'quads'),
  stdBinaryInstances: sumBy(stdBinarySummaries, 'instances'),
  stdBinaryScriptInstructions: sumBy(stdBinarySummaries, 'scriptInstructions'),
  stdBinaryOpcodes: uniqueSorted(stdBinarySummaries.flatMap((summary) => summary.opcodes)),
  stage6StdBinary,
  stdEntries: sumBy(stdSummaries, 'entries'),
  stdQuads: sumBy(stdSummaries, 'quads'),
  stdFaces: sumBy(stdSummaries, 'faces'),
  msgDumps: 8,
  msgEntries: sumBy(msgSummaries, 'entries'),
  msgTimelineLabels: sumBy(msgSummaries, 'timelineLabels'),
  sfx: sfx.length,
}, null, 2));

function auditAnmFile(path, label) {
  const data = readFileSync(path);
  const entrySize = 0xb8;
  assert.ok(data.length >= entrySize, `${label} is too small for an ANM entry header`);

  const numSprites = data.readInt32LE(0);
  const numScripts = data.readInt32LE(4);
  const width = data.readInt32LE(12);
  const height = data.readInt32LE(16);
  const nameOffset = data.readUInt32LE(28);
  const mipmapNameOffset = data.readUInt32LE(36);
  const version = data.readUInt32LE(40);
  const textureOffset = data.readUInt32LE(48);
  const hasData = data.readUInt32LE(52);

  assert.ok(numSprites >= 0 && numSprites <= 2048, `${label} ANM sprite count out of range`);
  assert.ok(numScripts >= 0 && numScripts <= 2048, `${label} ANM script count out of range`);
  assert.ok(width >= 0 && width <= 4096, `${label} ANM texture width out of range`);
  assert.ok(height >= 0 && height <= 4096, `${label} ANM texture height out of range`);
  assert.ok(version === 0 || version === 2, `${label} ANM version ${version} is unexpected`);
  if (nameOffset !== 0) {
    assert.ok(nameOffset < data.length, `${label} ANM name offset out of range`);
  }
  if (mipmapNameOffset !== 0) {
    assert.ok(mipmapNameOffset < data.length, `${label} ANM mipmap name offset out of range`);
  }
  if (hasData !== 0) {
    assert.ok(textureOffset < data.length, `${label} ANM texture data offset out of range`);
  }

  const spriteOffsetBase = 0x40;
  const scriptBase = spriteOffsetBase + numSprites * 4;
  assert.ok(scriptBase + numScripts * 8 <= data.length, `${label} ANM sprite/script table exceeds file bounds`);
  for (let i = 0; i < numSprites; i++) {
    const spriteOffset = data.readUInt32LE(spriteOffsetBase + i * 4);
    assert.ok(spriteOffset + 20 <= data.length, `${label} ANM sprite offset out of range`);
  }

  for (let i = 0; i < numScripts; i++) {
    const scriptOffset = data.readUInt32LE(scriptBase + i * 8 + 4);
    assert.ok(scriptOffset + 4 <= data.length, `${label} ANM script offset out of range`);
  }

  return { file: label, entries: 1, sprites: numSprites, scripts: numScripts };
}

function auditStdBinary(path, label) {
  const data = readFileSync(path);
  const headerSize = 0x490;
  const objectHeaderSize = 0x1c;
  const objectInstanceSize = 0x10;
  const scriptInstructionSize = 0x14;
  assert.ok(data.length >= headerSize, `${label} is too small for a TH07 STD header`);

  const objects = data.readInt16LE(0);
  const declaredQuads = data.readInt16LE(2);
  const instancesOffset = data.readInt32LE(4);
  const scriptOffset = data.readInt32LE(8);

  assert.ok(objects > 0 && objects <= 64, `${label} STD object count out of range`);
  assert.ok(declaredQuads >= 0 && declaredQuads <= 512, `${label} STD quad count out of range`);
  assert.ok(instancesOffset >= headerSize && instancesOffset < data.length,
    `${label} STD instance offset out of range`);
  assert.ok(scriptOffset >= headerSize && scriptOffset < data.length,
    `${label} STD script offset out of range`);
  assert.ok(headerSize + objects * 4 <= data.length, `${label} STD object offset table exceeds file bounds`);

  let quads = 0;
  for (let index = 0; index < objects; index++) {
    const objectOffset = data.readUInt32LE(headerSize + index * 4);
    assert.ok(objectOffset >= headerSize && objectOffset + objectHeaderSize <= data.length,
      `${label} STD object offset out of range`);
    quads += countStdObjectQuads(data, objectOffset, label);
  }
  assert.equal(quads, declaredQuads, `${label} STD declared quad count does not match parsed quads`);

  let instances = 0;
  for (let offset = instancesOffset; offset + objectInstanceSize <= data.length; offset += objectInstanceSize) {
    const objectId = data.readInt16LE(offset);
    if (objectId < 0) {
      break;
    }
    assert.ok(objectId < objects, `${label} STD instance references object ${objectId}`);
    instances++;
  }

  let scriptInstructions = 0;
  const opcodes = new Set();
  for (let offset = scriptOffset; offset + scriptInstructionSize <= data.length; offset += scriptInstructionSize) {
    const frame = data.readInt32LE(offset);
    if (frame < 0) {
      break;
    }
    const opcode = data.readInt16LE(offset + 4);
    const argsByteSize = data.readInt16LE(offset + 6);
    assert.equal(argsByteSize, 0xc, `${label} STD script argument byte size changed`);
    assert.ok(opcode >= 0 && opcode <= 0xff, `${label} STD script opcode out of range`);
    opcodes.add(opcode);
    scriptInstructions++;
  }

  return {
    file: label,
    objects,
    declaredQuads,
    quads,
    instances,
    scriptInstructions,
    opcodes: uniqueSorted([...opcodes]),
  };
}

function countStdObjectQuads(data, objectOffset, label) {
  const firstQuadOffset = objectOffset + 0x1c;
  let quads = 0;
  for (let offset = firstQuadOffset; offset + 4 <= data.length;) {
    const type = data.readInt16LE(offset);
    if (type < 0) {
      return quads;
    }
    const size = data.readInt16LE(offset + 2);
    assert.ok(size >= 0x1c, `${label} STD quad size out of range`);
    assert.ok(offset + size <= data.length, `${label} STD quad exceeds file bounds`);
    quads++;
    offset += size;
  }
  throw new Error(`${label} STD object quad list is missing its sentinel`);
}

function countMatches(text, pattern) {
  return [...text.matchAll(pattern)].length;
}

function countLines(text) {
  if (text.length === 0) {
    return 0;
  }
  return text.split(/\r?\n/).filter((line) => line.length > 0).length;
}

function sumBy(rows, key) {
  return rows.reduce((sum, row) => sum + row[key], 0);
}

function uniqueSorted(values) {
  return [...new Set(values)].sort((a, b) => a - b);
}
