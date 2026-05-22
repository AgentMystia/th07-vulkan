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

const expectedShtFiles = [
  'ply00a.sht', 'ply00as.sht', 'ply00b.sht', 'ply00bs.sht',
  'ply01a.sht', 'ply01as.sht', 'ply01b.sht', 'ply01bs.sht',
  'ply02a.sht', 'ply02as.sht', 'ply02b.sht', 'ply02bs.sht',
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
  ...expectedShtFiles,
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
const anmVmInstructionSizeRows = mergeInstructionSizeCounts(anmSummaries);
const anmVmOpcodeUsageRows = mergeOpcodeUsageCounts(anmSummaries);
const anmVmOpcodeArgShapeRows = mergeOpcodeArgShapes(anmSummaries);
const anmVmOpcodes = uniqueSorted(anmSummaries.flatMap((summary) => summary.opcodes));
const resourceFormatTables = parseResourceFormatTables();
const playerShotTables = parsePlayerShotTables();
const bulletTables = parseBulletTables();
const runtimeAnmManifest = parseRuntimeAnmManifest();
const runtimeAnmStringAnchors = parseRuntimeAnmStringAnchors(anmFiles);
const runtimeAnmLoadContracts = parseRuntimeAnmLoadContracts(runtimeAnmManifest);
const liveRuntimeAnmLoadCoverage = auditLiveRuntimeAnmLoads(runtimeAnmManifest);
assert.ok(anmEntries >= anmFiles.length, 'Each ANM file should expose at least one entry header');
assert.ok(anmSprites > 100, 'Expected substantive ANM sprite coverage');
assert.ok(anmScripts > 100, 'Expected substantive ANM script coverage');
assert.deepEqual(resourceFormatTables.anm, {
  files: anmFiles.length,
  entries: anmEntries,
  sprites: anmSprites,
  scripts: anmScripts,
  rows: anmSummaries.map((summary) => ({
    fileName: summary.file,
    sprites: summary.sprites,
    scripts: summary.scripts,
    textureWidth: summary.textureWidth,
    textureHeight: summary.textureHeight,
    version: summary.version,
    hasTextureData: summary.hasTextureData,
  })),
}, 'C++ ANM resource constants drifted from parsed original data');
assert.deepEqual(runtimeAnmManifest, runtimeAnmStringAnchors,
  'C++ runtime ANM manifest drifted from Ghidra string anchors or extracted ANM files');
assert.deepEqual(countBy(runtimeAnmLoadContracts, 'evidenceFunction'), {
  '00401d70': 2,
  '00407000': 12,
  '0041e590': 1,
  '00426f60': 1,
  '00428b19': 23,
  '00438986': 1,
  '0043ae20': 1,
  '004423e0': 3,
  '00449b05': 1,
  '0045bf15': 1,
}, 'C++ runtime ANM load contract evidence-function coverage changed');
assert.deepEqual(uniqueSorted(runtimeAnmLoadContracts.map((row) => row.anmFileSlot)), [
  0x00, 0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x15, 0x17, 0x18, 0x19,
  0x1c, 0x20, 0x2a, 0x2e, 0x31,
], 'C++ runtime ANM load contract slot coverage changed');
assert.equal(liveRuntimeAnmLoadCoverage.totalLoads, 62, 'Live C++ ANM load call count changed');
assert.equal(liveRuntimeAnmLoadCoverage.sourceDerivedLoads, 40,
  'Live C++ ANM load calls backed by TH07 string anchors changed');
assert.deepEqual(liveRuntimeAnmLoadCoverage.obsoleteLoads, [
  {sourceFile: 'Ending.cpp', archivePath: 'data/staff02.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'Ending.cpp', archivePath: 'data/staff03.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/title02.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/title03.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/title04.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/title01s.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/title04s.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/select01.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/select02.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/select03.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/select04.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/select05.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/slpl00a.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/slpl00b.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/slpl01a.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/slpl01b.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MainMenu.cpp', archivePath: 'data/replay00.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MusicRoom.cpp', archivePath: 'data/music01.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'MusicRoom.cpp', archivePath: 'data/music02.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'ResultScreen.cpp', archivePath: 'data/result01.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'ResultScreen.cpp', archivePath: 'data/result02.anm', status: 'obsolete-th06-carryover'},
  {sourceFile: 'ResultScreen.cpp', archivePath: 'data/result03.anm', status: 'obsolete-th06-carryover'},
], 'Live C++ ANM loads with no TH07 string anchor changed');
assert.deepEqual(resourceFormatTables.anmVm, {
  instructions: sumBy(anmSummaries, 'scriptInstructions'),
  terminators: sumBy(anmSummaries, 'scriptTerminators'),
  opcodeCount: anmVmOpcodes.length,
  maxOpcode: Math.max(...anmVmOpcodes),
  opcodes: anmVmOpcodes,
  sizeRows: anmVmInstructionSizeRows,
}, 'C++ ANM VM instruction metadata drifted from parsed original data');
assert.deepEqual(resourceFormatTables.anmVmOpcodeUsage, {
  count: anmVmOpcodeUsageRows.length,
  rows: anmVmOpcodeUsageRows,
}, 'C++ ANM VM opcode usage table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.anmVmOpcodeArgShapes, {
  count: anmVmOpcodeArgShapeRows.length,
  totalArgBytes: sumBy(anmVmOpcodeArgShapeRows, 'totalArgBytes'),
  rows: anmVmOpcodeArgShapeRows,
}, 'C++ ANM VM opcode argument-shape table drifted from parsed original data');
const etamaAnmSummary = anmSummaries.find((summary) => summary.file === 'etama.anm');
assert.ok(etamaAnmSummary, 'Missing extracted etama.anm summary');
const etamaTemplateEvidence = auditEtamaTemplateEvidence();
assert.deepEqual(bulletTables.resource, {
  archivePath: 'data/etama.anm',
  ghidraStringAddress: '00498524',
  sprites: etamaAnmSummary.sprites,
  scripts: etamaAnmSummary.scripts,
  textureWidth: etamaAnmSummary.textureWidth,
  textureHeight: etamaAnmSummary.textureHeight,
  version: etamaAnmSummary.version,
  hasTextureData: etamaAnmSummary.hasTextureData,
  status: 'source-derived',
}, 'C++ TH07 bullet ANM metadata drifted from parsed original data');
assert.deepEqual(bulletTables.templateConstants, {
  loadFunctionAddress: 0x00426f60,
  tableAddress: 0x00495160,
  tableFileOffset: 0x00094360,
  entrySize: 0x14,
  count: 11,
  resolvedCount: 10,
  managerSlot: 0x0b,
  scriptOffset: 0x0200,
  resolvedScriptLast: 0x0218,
  stride: 0x0b8c,
}, 'C++ TH07 enemy bullet template constants drifted');
assert.deepEqual(bulletTables.templates, etamaTemplateEvidence.rows,
  'C++ TH07 enemy bullet template script/graze table drifted from original binary or etama.anm');
assert.deepEqual(bulletTables.carryovers.map((row) => row.token), [
  'data/etama3.anm',
  'data/etama4.anm',
  'ANM_FILE_BULLET3',
  'ANM_FILE_BULLET4',
], 'C++ obsolete bullet resource carryover list changed');
assert.deepEqual([...new Set(bulletTables.carryovers.map((row) => row.status))],
  ['obsolete-th06-carryover'], 'C++ obsolete bullet carryover status changed');

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
const stdBinaryOpcodeUsageRows = mergeOpcodeUsageCounts(stdBinarySummaries);
const stdBinaryOpcodeArgShapeRows = mergeOpcodeArgShapes(stdBinarySummaries);
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
  opcodeUsage: [
    {opcode: 0, instructions: 5},
    {opcode: 1, instructions: 1},
    {opcode: 3, instructions: 2},
    {opcode: 4, instructions: 4},
    {opcode: 5, instructions: 20},
    {opcode: 6, instructions: 12},
    {opcode: 7, instructions: 1},
    {opcode: 9, instructions: 1},
    {opcode: 11, instructions: 1},
    {opcode: 29, instructions: 5},
    {opcode: 30, instructions: 1},
    {opcode: 31, instructions: 5},
  ],
  opcodeArgShapes: [
    {opcode: 0, instructions: 5, minArgSize: 12, maxArgSize: 12, totalArgBytes: 60},
    {opcode: 1, instructions: 1, minArgSize: 12, maxArgSize: 12, totalArgBytes: 12},
    {opcode: 3, instructions: 2, minArgSize: 12, maxArgSize: 12, totalArgBytes: 24},
    {opcode: 4, instructions: 4, minArgSize: 12, maxArgSize: 12, totalArgBytes: 48},
    {opcode: 5, instructions: 20, minArgSize: 12, maxArgSize: 12, totalArgBytes: 240},
    {opcode: 6, instructions: 12, minArgSize: 12, maxArgSize: 12, totalArgBytes: 144},
    {opcode: 7, instructions: 1, minArgSize: 12, maxArgSize: 12, totalArgBytes: 12},
    {opcode: 9, instructions: 1, minArgSize: 12, maxArgSize: 12, totalArgBytes: 12},
    {opcode: 11, instructions: 1, minArgSize: 12, maxArgSize: 12, totalArgBytes: 12},
    {opcode: 29, instructions: 5, minArgSize: 12, maxArgSize: 12, totalArgBytes: 60},
    {opcode: 30, instructions: 1, minArgSize: 12, maxArgSize: 12, totalArgBytes: 12},
    {opcode: 31, instructions: 5, minArgSize: 12, maxArgSize: 12, totalArgBytes: 60},
  ],
}, 'Stage 6 STD binary shape changed; update the readable-dump blocker and parser notes');
assert.deepEqual(resourceFormatTables.std, {
  files: 8,
  objects: sumBy(stdBinarySummaries, 'objects'),
  quads: sumBy(stdBinarySummaries, 'quads'),
  instances: sumBy(stdBinarySummaries, 'instances'),
  scriptInstructions: sumBy(stdBinarySummaries, 'scriptInstructions'),
  readableDumps: stdSummaries.length - emptyStdDumps.length,
  emptyReadableDumpStage: 6,
  rows: stdBinarySummaries.map((summary, index) => ({
    stage: index + 1,
    objects: summary.objects,
    quads: summary.quads,
    instances: summary.instances,
    scriptInstructions: summary.scriptInstructions,
    opcodes: summary.opcodes,
    readableDumpAvailable: !emptyStdDumps.includes(`stage${index + 1}.dstd`),
  })),
}, 'C++ STD resource table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.stdOpcodeUsage, {
  count: stdBinaryOpcodeUsageRows.length,
  rows: stdBinaryOpcodeUsageRows,
}, 'C++ STD binary opcode usage table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.stdOpcodeArgShapes, {
  count: stdBinaryOpcodeArgShapeRows.length,
  totalArgBytes: sumBy(stdBinaryOpcodeArgShapeRows, 'totalArgBytes'),
  rows: stdBinaryOpcodeArgShapeRows,
}, 'C++ STD binary opcode argument-shape table drifted from parsed original data');

const msgSummaries = [];
const msgBinarySummaries = [];
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

  const binaryFile = `msg${stage}.dat`;
  msgBinarySummaries.push(auditMsgBinary(join(root, 'reference/extracted', binaryFile), binaryFile, stage));
}
const msgBinaryOpcodeUsageRows = mergeOpcodeUsageCounts(msgBinarySummaries);
const msgBinaryOpcodeArgShapeRows = mergeOpcodeArgShapes(msgBinarySummaries);
assert.deepEqual(resourceFormatTables.msg, {
  dumps: 8,
  entries: sumBy(msgSummaries, 'entries'),
  timelineLabels: sumBy(msgSummaries, 'timelineLabels'),
  rows: msgSummaries.map((summary, index) => ({
    stage: index + 1,
    entries: summary.entries,
    timelineLabels: summary.timelineLabels,
  })),
}, 'C++ MSG resource table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.msgBinary, {
  entryPointers: sumBy(msgBinarySummaries, 'entryPointers'),
  uniqueEntries: sumBy(msgBinarySummaries, 'uniqueEntries'),
  instructions: sumBy(msgBinarySummaries, 'instructions'),
  textBytes: sumBy(msgBinarySummaries, 'textBytes'),
  waitTotalFrames: sumBy(msgBinarySummaries, 'waitTotalFrames'),
  maxOpcode: Math.max(...msgBinarySummaries.flatMap((summary) => summary.opcodes)),
  rows: msgBinarySummaries.map((summary) => ({
    stage: summary.stage,
    fileSize: summary.fileSize,
    entryPointers: summary.entryPointers,
    uniqueEntries: summary.uniqueEntries,
    instructions: summary.instructions,
    opcodes: summary.opcodes,
    textBytes: summary.textBytes,
    waitTotalFrames: summary.waitTotalFrames,
  })),
}, 'C++ MSG binary table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.msgBinaryOpcodeUsage, {
  count: msgBinaryOpcodeUsageRows.length,
  rows: msgBinaryOpcodeUsageRows,
}, 'C++ MSG binary opcode usage table drifted from parsed original data');
assert.deepEqual(resourceFormatTables.msgBinaryOpcodeArgShapes, {
  count: msgBinaryOpcodeArgShapeRows.length,
  totalArgBytes: sumBy(msgBinaryOpcodeArgShapeRows, 'totalArgBytes'),
  rows: msgBinaryOpcodeArgShapeRows,
}, 'C++ MSG binary opcode argument-shape table drifted from parsed original data');

const shtSummaries = expectedShtFiles.map((file) => auditShtFile(join(root, 'reference/extracted', file), file));
const shtSummaryByFile = new Map(shtSummaries.map((summary) => [summary.fileName, summary]));
const shtCallbackUsage = {
  spawn: [0, 0, 0, 0, 0, 0],
  update: [0, 0, 0, 0, 0, 0],
  draw: [0, 0],
  collision: [0, 0, 0],
};
for (const summary of shtSummaries) {
  accumulateUsage(shtCallbackUsage.spawn, summary.callbackUsage.spawn, `${summary.fileName} spawn`);
  accumulateUsage(shtCallbackUsage.update, summary.callbackUsage.update, `${summary.fileName} update`);
  accumulateUsage(shtCallbackUsage.draw, summary.callbackUsage.draw, `${summary.fileName} draw`);
  accumulateUsage(shtCallbackUsage.collision, summary.callbackUsage.collision, `${summary.fileName} collision`);
}
assert.equal(playerShotTables.files, 12, 'C++ player SHT file count changed');
assert.equal(playerShotTables.powerLevels, 9, 'C++ player SHT power-level count changed');
assert.equal(playerShotTables.shotRecords, sumBy(shtSummaries, 'shotRecordCount'),
  'C++ player SHT shot-record count drifted from parsed original data');
assert.deepEqual(playerShotTables.callbackUsage, shtCallbackUsage,
  'C++ player SHT callback usage drifted from parsed original data');
for (const tableRow of playerShotTables.rows) {
  const summary = shtSummaryByFile.get(tableRow.fileName);
  assert.ok(summary, `C++ player SHT row references missing file ${tableRow.fileName}`);
  assert.equal(tableRow.fileSize, summary.fileSize, `${tableRow.fileName} SHT file size drifted`);
  assert.equal(tableRow.shotRecordCount, summary.shotRecordCount, `${tableRow.fileName} SHT shot count drifted`);
  assert.equal(tableRow.deathbombWindowFrames, summary.deathbombWindowFrames,
    `${tableRow.fileName} SHT deathbomb window drifted`);
  assert.deepEqual(summary.powerThresholds, playerShotTables.powerThresholds,
    `${tableRow.fileName} SHT power thresholds drifted`);
  assertFloatArrayNearlyEqual(tableRow.tuningFloats, summary.tuningFloats,
    `${tableRow.fileName} SHT tuning floats drifted`);
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
  anmVmScriptInstructions: sumBy(anmSummaries, 'scriptInstructions'),
  anmVmScriptTerminators: sumBy(anmSummaries, 'scriptTerminators'),
  anmVmOpcodes,
  anmVmOpcodeUsage: anmVmOpcodeUsageRows,
  anmVmOpcodeArgShapes: anmVmOpcodeArgShapeRows,
  anmVmInstructionSizeRows,
  eclDumps: 8,
  stdDumps: 8,
  stdReadableDumps: stdSummaries.length - emptyStdDumps.length,
  stdEmptyDumps: emptyStdDumps,
  stdBinaryObjects: sumBy(stdBinarySummaries, 'objects'),
  stdBinaryQuads: sumBy(stdBinarySummaries, 'quads'),
  stdBinaryInstances: sumBy(stdBinarySummaries, 'instances'),
  stdBinaryScriptInstructions: sumBy(stdBinarySummaries, 'scriptInstructions'),
  stdBinaryOpcodes: uniqueSorted(stdBinarySummaries.flatMap((summary) => summary.opcodes)),
  stdBinaryOpcodeUsage: stdBinaryOpcodeUsageRows,
  stdBinaryOpcodeArgShapes: stdBinaryOpcodeArgShapeRows,
  stage6StdBinary,
  stdEntries: sumBy(stdSummaries, 'entries'),
  stdQuads: sumBy(stdSummaries, 'quads'),
  stdFaces: sumBy(stdSummaries, 'faces'),
  msgDumps: 8,
  msgEntries: sumBy(msgSummaries, 'entries'),
  msgTimelineLabels: sumBy(msgSummaries, 'timelineLabels'),
  msgBinaryEntryPointers: sumBy(msgBinarySummaries, 'entryPointers'),
  msgBinaryUniqueEntries: sumBy(msgBinarySummaries, 'uniqueEntries'),
  msgBinaryInstructions: sumBy(msgBinarySummaries, 'instructions'),
  msgBinaryTextBytes: sumBy(msgBinarySummaries, 'textBytes'),
  msgBinaryWaitTotalFrames: sumBy(msgBinarySummaries, 'waitTotalFrames'),
  msgBinaryOpcodes: uniqueSorted(msgBinarySummaries.flatMap((summary) => summary.opcodes)),
  msgBinaryOpcodeUsage: msgBinaryOpcodeUsageRows,
  msgBinaryOpcodeArgShapes: msgBinaryOpcodeArgShapeRows,
  shtFiles: shtSummaries.length,
  shtShotRecords: sumBy(shtSummaries, 'shotRecordCount'),
  shtCallbackUsage,
  enemyBulletAnm: bulletTables.resource,
  runtimeAnmLoadContracts: {
    rows: runtimeAnmLoadContracts.length,
    evidenceFunctions: countBy(runtimeAnmLoadContracts, 'evidenceFunction'),
    slots: uniqueSorted(runtimeAnmLoadContracts.map((row) => row.anmFileSlot)),
  },
  liveRuntimeAnmLoads: liveRuntimeAnmLoadCoverage,
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

  const scriptOffsets = [];
  for (let i = 0; i < numScripts; i++) {
    const scriptOffset = data.readUInt32LE(scriptBase + i * 8 + 4);
    assert.ok(scriptOffset + 4 <= data.length, `${label} ANM script offset out of range`);
    scriptOffsets.push(scriptOffset);
  }

  const scriptAudit = auditAnmScripts(data, label, scriptOffsets, hasData !== 0 ? textureOffset : data.length);
  return {
    file: label,
    entries: 1,
    sprites: numSprites,
    scripts: numScripts,
    textureWidth: width,
    textureHeight: height,
    version,
    hasTextureData: hasData !== 0,
    scriptInstructions: scriptAudit.instructions,
    scriptTerminators: scriptAudit.terminators,
    opcodes: scriptAudit.opcodes,
    opcodeUsage: scriptAudit.opcodeUsage,
    opcodeArgShapes: scriptAudit.opcodeArgShapes,
    instructionSizeCounts: scriptAudit.instructionSizeCounts,
  };
}

function auditAnmScripts(data, label, scriptOffsets, scriptEndBoundary) {
  const uniqueOffsets = uniqueSorted(scriptOffsets);
  let instructions = 0;
  let terminators = 0;
  const opcodes = new Set();
  const instructionSizeCounts = new Map();
  const opcodeUsage = new Map();
  const opcodeArgShapes = new Map();

  for (let index = 0; index < uniqueOffsets.length; index++) {
    const end = index + 1 < uniqueOffsets.length ? uniqueOffsets[index + 1] : scriptEndBoundary;
    let offset = uniqueOffsets[index];
    let sawTerminator = false;
    while (offset + 2 <= end) {
      const opcode = data.readInt16LE(offset);
      if (opcode < 0) {
        assert.equal(opcode, -1, `${label} ANM script terminator opcode changed`);
        if (offset + 4 <= end) {
          assert.equal(data.readUInt16LE(offset + 2), 0, `${label} ANM script terminator size changed`);
        }
        terminators++;
        sawTerminator = true;
        break;
      }

      assert.ok(offset + 8 <= end, `${label} ANM instruction header exceeds script bounds`);
      const size = data.readUInt16LE(offset + 2);
      assert.ok(size >= 8 && size % 4 === 0, `${label} ANM instruction size out of range`);
      assert.ok(offset + size <= end, `${label} ANM instruction exceeds script bounds`);
      assert.ok(opcode <= 0x50, `${label} ANM opcode ${opcode} exceeds current TH07 evidence`);
      instructions++;
      opcodes.add(opcode);
      opcodeUsage.set(opcode, (opcodeUsage.get(opcode) ?? 0) + 1);
      instructionSizeCounts.set(size, (instructionSizeCounts.get(size) ?? 0) + 1);
      const argSize = size - 8;
      const argShape = opcodeArgShapes.get(opcode) ?? {
        opcode,
        instructions: 0,
        minArgSize: argSize,
        maxArgSize: argSize,
        totalArgBytes: 0,
      };
      argShape.instructions++;
      argShape.minArgSize = Math.min(argShape.minArgSize, argSize);
      argShape.maxArgSize = Math.max(argShape.maxArgSize, argSize);
      argShape.totalArgBytes += argSize;
      opcodeArgShapes.set(opcode, argShape);
      offset += size;
    }
    assert.ok(sawTerminator, `${label} ANM script at offset ${uniqueOffsets[index]} is missing terminator`);
  }

  return {
    instructions,
    terminators,
    opcodes: uniqueSorted([...opcodes]),
    opcodeUsage: [...opcodeUsage.entries()]
      .sort((a, b) => a[0] - b[0])
      .map(([opcode, instructions]) => ({opcode, instructions})),
    opcodeArgShapes: [...opcodeArgShapes.values()].sort((a, b) => a.opcode - b.opcode),
    instructionSizeCounts: [...instructionSizeCounts.entries()]
      .sort((a, b) => a[0] - b[0])
      .map(([size, count]) => ({size, instructions: count})),
  };
}

function auditEtamaTemplateEvidence() {
  const exe = readFileSync(join(root, 'reference/original/th07.exe'));
  const etama = readFileSync(join(root, 'reference/extracted/etama.anm'));
  const scriptOffset = 0x200;
  const tableFileOffset = 0x94360;
  const tableRows = 11;
  const tableEntrySize = 0x14;
  const numSprites = etama.readInt32LE(0);
  const numScripts = etama.readInt32LE(4);
  const textureOffset = etama.readUInt32LE(48);
  const hasTextureData = etama.readUInt32LE(52) !== 0;
  const scriptBase = 0x40 + numSprites * 4;
  const scriptEntries = [];
  for (let index = 0; index < numScripts; index++) {
    scriptEntries.push({
      localScriptId: etama.readUInt32LE(scriptBase + index * 8),
      scriptDataOffset: etama.readUInt32LE(scriptBase + index * 8 + 4),
    });
  }
  const scriptEndsByOffset = new Map();
  const sortedScriptOffsets = uniqueSorted(scriptEntries.map((entry) => entry.scriptDataOffset));
  for (let index = 0; index < sortedScriptOffsets.length; index++) {
    scriptEndsByOffset.set(sortedScriptOffsets[index],
      index + 1 < sortedScriptOffsets.length
        ? sortedScriptOffsets[index + 1]
        : hasTextureData ? textureOffset : etama.length);
  }
  const scriptFirstSpriteByGlobalId = new Map();
  for (const {localScriptId, scriptDataOffset} of scriptEntries) {
    const firstSprite = findFirstAnmActiveSprite(etama, scriptDataOffset, scriptEndsByOffset.get(scriptDataOffset),
      `etama.anm script ${localScriptId}`);
    if (firstSprite !== undefined) {
      scriptFirstSpriteByGlobalId.set(scriptOffset + localScriptId, firstSprite);
    }
  }

  const spriteShapeByLocalId = new Map();
  for (let index = 0; index < numSprites; index++) {
    const spriteOffsetInFile = etama.readUInt32LE(0x40 + index * 4);
    const localSpriteId = etama.readUInt32LE(spriteOffsetInFile);
    spriteShapeByLocalId.set(localSpriteId, {
      width: Math.trunc(etama.readFloatLE(spriteOffsetInFile + 12)),
      height: Math.trunc(etama.readFloatLE(spriteOffsetInFile + 16)),
    });
  }

  const rows = [];
  for (let slot = 0; slot < tableRows; slot++) {
    const offset = tableFileOffset + slot * tableEntrySize;
    const bulletScript = exe.readUInt32LE(offset);
    const spawnFastScript = exe.readUInt32LE(offset + 4);
    const spawnNormalScript = exe.readUInt32LE(offset + 8);
    const spawnSlowScript = exe.readUInt32LE(offset + 12);
    const spawnDonutScript = exe.readUInt32LE(offset + 16);
    const localFirstSprite = scriptFirstSpriteByGlobalId.get(bulletScript);
    const rowScripts = [bulletScript, spawnFastScript, spawnNormalScript, spawnSlowScript, spawnDonutScript];
    if (rowScripts.some((script) => !scriptFirstSpriteByGlobalId.has(script))) {
      rows.push({
        slot,
        bulletScript,
        spawnFastScript,
        spawnNormalScript,
        spawnSlowScript,
        spawnDonutScript,
        activeSprite: 0xffff,
        spriteWidth: 0,
        spriteHeight: 0,
        grazeSize: '0.0f',
        spriteOffsetCategory: 0,
        status: 'blocked',
      });
      continue;
    }

    const spriteShape = spriteShapeByLocalId.get(localFirstSprite);
    assert.ok(spriteShape, `etama.anm script ${bulletScript.toString(16)} references missing sprite ${localFirstSprite}`);
    const graze = deriveBulletGraze(bulletScript, spriteShape.height);
    rows.push({
      slot,
      bulletScript,
      spawnFastScript,
      spawnNormalScript,
      spawnSlowScript,
      spawnDonutScript,
      activeSprite: localFirstSprite,
      spriteWidth: spriteShape.width,
      spriteHeight: spriteShape.height,
      grazeSize: `${graze.grazeSize}.0f`,
      spriteOffsetCategory: graze.spriteOffsetCategory,
      status: 'source-derived',
    });
  }
  return { rows };
}

function findFirstAnmActiveSprite(data, scriptOffset, scriptEnd, label) {
  assert.ok(scriptEnd !== undefined, `${label} end offset missing`);
  for (let offset = scriptOffset; offset + 2 <= scriptEnd;) {
    const opcode = data.readInt16LE(offset);
    if (opcode < 0) {
      assert.equal(opcode, -1, `${label} terminator opcode changed`);
      return undefined;
    }
    assert.ok(offset + 8 <= scriptEnd, `${label} ANM instruction header exceeds script bounds`);
    const size = data.readUInt16LE(offset + 2);
    assert.ok(size >= 8 && size % 4 === 0, `${label} ANM instruction size out of range`);
    assert.ok(offset + size <= scriptEnd, `${label} ANM instruction exceeds script bounds`);
    if (opcode === 3) {
      assert.ok(size >= 12, `${label} active-sprite instruction too short`);
      return data.readUInt32LE(offset + 8);
    }
    offset += size;
  }
  return undefined;
}

function deriveBulletGraze(script, spriteHeight) {
  if (spriteHeight <= 8) return { grazeSize: 4, spriteOffsetCategory: 5 };
  if (spriteHeight <= 16) {
    switch (script) {
    case 0x202:
    case 0x204:
    case 0x205:
    case 0x206:
      return { grazeSize: 4, spriteOffsetCategory: 4 };
    default:
      return { grazeSize: 6, spriteOffsetCategory: 3 };
    }
  }
  if (spriteHeight <= 32) {
    if (script === 0x208) return { grazeSize: 5, spriteOffsetCategory: 1 };
    if (script === 0x209) return { grazeSize: 8, spriteOffsetCategory: 2 };
    return { grazeSize: 10, spriteOffsetCategory: 2 };
  }
  return { grazeSize: 24, spriteOffsetCategory: 0 };
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
  const opcodeUsage = new Map();
  const opcodeArgShapes = new Map();
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
    opcodeUsage.set(opcode, (opcodeUsage.get(opcode) ?? 0) + 1);
    const argShape = opcodeArgShapes.get(opcode) ?? {
      opcode,
      instructions: 0,
      minArgSize: argsByteSize,
      maxArgSize: argsByteSize,
      totalArgBytes: 0,
    };
    argShape.instructions++;
    argShape.minArgSize = Math.min(argShape.minArgSize, argsByteSize);
    argShape.maxArgSize = Math.max(argShape.maxArgSize, argsByteSize);
    argShape.totalArgBytes += argsByteSize;
    opcodeArgShapes.set(opcode, argShape);
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
    opcodeUsage: [...opcodeUsage.entries()]
      .sort((a, b) => a[0] - b[0])
      .map(([opcode, instructions]) => ({opcode, instructions})),
    opcodeArgShapes: [...opcodeArgShapes.values()].sort((a, b) => a.opcode - b.opcode),
  };
}

function auditShtFile(path, label) {
  const data = readFileSync(path);
  const headerSize = 0x34;
  const powerLevelEntrySize = 0x8;
  const shotRecordSize = 0x34;
  assert.ok(data.length >= headerSize, `${label} is too small for a TH07 SHT header`);

  const formatId = data.readUInt16LE(0);
  const powerLevelCount = data.readUInt16LE(2);
  assert.equal(formatId, 0, `${label} SHT format id changed`);
  assert.equal(powerLevelCount, 9, `${label} SHT power-level count changed`);
  assert.ok(headerSize + powerLevelCount * powerLevelEntrySize <= data.length,
    `${label} SHT power table exceeds file bounds`);

  const tuningFloats = [];
  for (let index = 0; index < 10; index++) {
    tuningFloats.push(data.readFloatLE(0x0c + index * 4));
  }

  const callbackUsage = {
    spawn: [0, 0, 0, 0, 0, 0],
    update: [0, 0, 0, 0, 0, 0],
    draw: [0, 0],
    collision: [0, 0, 0],
  };
  const powerThresholds = [];
  let shotRecordCount = 0;
  for (let level = 0; level < powerLevelCount; level++) {
    const tableOffset = headerSize + level * powerLevelEntrySize;
    const recordOffset = data.readUInt32LE(tableOffset);
    const threshold = data.readInt32LE(tableOffset + 4);
    assert.ok(recordOffset >= headerSize + powerLevelCount * powerLevelEntrySize && recordOffset < data.length,
      `${label} SHT shot record offset out of range`);
    powerThresholds.push(threshold);

    for (let cursor = recordOffset; cursor + 2 <= data.length; cursor += shotRecordSize) {
      const wait = data.readInt16LE(cursor);
      if (wait < 0) {
        break;
      }
      assert.ok(cursor + shotRecordSize <= data.length, `${label} SHT shot record exceeds file bounds`);
      incrementUsage(callbackUsage.spawn, data.readInt32LE(cursor + 0x24), `${label} spawn`);
      incrementUsage(callbackUsage.update, data.readInt32LE(cursor + 0x28), `${label} update`);
      incrementUsage(callbackUsage.draw, data.readInt32LE(cursor + 0x2c), `${label} draw`);
      incrementUsage(callbackUsage.collision, data.readInt32LE(cursor + 0x30), `${label} collision`);
      shotRecordCount++;
    }
  }

  return {
    fileName: label,
    fileSize: data.length,
    shotRecordCount,
    deathbombWindowFrames: data.readInt32LE(0x08),
    tuningFloats,
    powerThresholds,
    callbackUsage,
  };
}

function auditMsgBinary(path, label, stage) {
  const data = readFileSync(path);
  assert.ok(data.length >= 4, `${label} is too small for a TH07 MSG header`);
  const entryPointers = data.readInt32LE(0);
  assert.ok(entryPointers > 0 && entryPointers <= 64, `${label} MSG entry pointer count out of range`);
  const pointerTableEnd = 4 + entryPointers * 4;
  assert.ok(pointerTableEnd <= data.length, `${label} MSG pointer table exceeds file bounds`);

  const offsets = [];
  for (let index = 0; index < entryPointers; index++) {
    const offset = data.readUInt32LE(4 + index * 4);
    assert.ok(offset >= pointerTableEnd && offset < data.length, `${label} MSG entry offset out of range`);
    offsets.push(offset);
  }

  const uniqueOffsets = uniqueSorted(offsets);
  let instructions = 0;
  let textBytes = 0;
  let waitTotalFrames = 0;
  const opcodes = new Set();
  const opcodeUsage = new Map();
  const opcodeArgShapes = new Map();
  for (let index = 0; index < uniqueOffsets.length; index++) {
    const end = index + 1 < uniqueOffsets.length ? uniqueOffsets[index + 1] : data.length;
    for (let offset = uniqueOffsets[index]; offset < end;) {
      assert.ok(offset + 4 <= end, `${label} MSG instruction header exceeds entry bounds`);
      const opcode = data.readUInt8(offset + 2);
      const argSize = data.readUInt8(offset + 3);
      assert.ok(offset + 4 + argSize <= end, `${label} MSG instruction args exceed entry bounds`);
      assert.ok(opcode <= 0x0e, `${label} MSG opcode ${opcode} exceeds current TH07 evidence`);

      instructions++;
      opcodes.add(opcode);
      opcodeUsage.set(opcode, (opcodeUsage.get(opcode) ?? 0) + 1);
      const argShape = opcodeArgShapes.get(opcode) ?? {
        opcode,
        instructions: 0,
        minArgSize: argSize,
        maxArgSize: argSize,
        totalArgBytes: 0,
      };
      argShape.instructions++;
      argShape.minArgSize = Math.min(argShape.minArgSize, argSize);
      argShape.maxArgSize = Math.max(argShape.maxArgSize, argSize);
      argShape.totalArgBytes += argSize;
      opcodeArgShapes.set(opcode, argShape);
      if (opcode === 3 || opcode === 8) {
        assert.ok(argSize >= 4, `${label} MSG text opcode ${opcode} missing text metadata`);
        textBytes += argSize - 4;
      } else if (opcode === 4) {
        assert.equal(argSize, 4, `${label} MSG wait opcode argument size changed`);
        waitTotalFrames += data.readInt32LE(offset + 4);
      }
      offset += 4 + argSize;
    }
  }

  return {
    stage,
    fileSize: data.length,
    entryPointers,
    uniqueEntries: uniqueOffsets.length,
    instructions,
    opcodes: uniqueSorted([...opcodes]),
    opcodeUsage: [...opcodeUsage.entries()]
      .sort((a, b) => a[0] - b[0])
      .map(([opcode, instructions]) => ({opcode, instructions})),
    opcodeArgShapes: [...opcodeArgShapes.values()].sort((a, b) => a.opcode - b.opcode),
    textBytes,
    waitTotalFrames,
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

function mergeInstructionSizeCounts(rows) {
  const counts = new Map();
  for (const row of rows) {
    for (const sizeRow of row.instructionSizeCounts) {
      counts.set(sizeRow.size, (counts.get(sizeRow.size) ?? 0) + sizeRow.instructions);
    }
  }
  return [...counts.entries()]
    .sort((a, b) => a[0] - b[0])
    .map(([size, instructions]) => ({size, instructions}));
}

function mergeOpcodeUsageCounts(rows) {
  const counts = new Map();
  for (const row of rows) {
    for (const opcodeRow of row.opcodeUsage) {
      counts.set(opcodeRow.opcode, (counts.get(opcodeRow.opcode) ?? 0) + opcodeRow.instructions);
    }
  }
  return [...counts.entries()]
    .sort((a, b) => a[0] - b[0])
    .map(([opcode, instructions]) => ({opcode, instructions}));
}

function mergeOpcodeArgShapes(rows) {
  const shapes = new Map();
  for (const row of rows) {
    for (const argShape of row.opcodeArgShapes) {
      const current = shapes.get(argShape.opcode) ?? {
        opcode: argShape.opcode,
        instructions: 0,
        minArgSize: argShape.minArgSize,
        maxArgSize: argShape.maxArgSize,
        totalArgBytes: 0,
      };
      current.instructions += argShape.instructions;
      current.minArgSize = Math.min(current.minArgSize, argShape.minArgSize);
      current.maxArgSize = Math.max(current.maxArgSize, argShape.maxArgSize);
      current.totalArgBytes += argShape.totalArgBytes;
      shapes.set(argShape.opcode, current);
    }
  }
  return [...shapes.values()].sort((a, b) => a.opcode - b.opcode);
}

function parseResourceFormatTables() {
  const header = readFileSync(join(root, 'src/Th07ResourceFormatTables.hpp'), 'utf8');
  const cpp = readFileSync(join(root, 'src/Th07ResourceFormatTables.cpp'), 'utf8');

  return {
    anm: {
      files: parseCppIntegerConstant(header, 'kTh07AnmFileCount'),
      entries: parseCppIntegerConstant(header, 'kTh07AnmEntryCount'),
      sprites: parseCppIntegerConstant(header, 'kTh07AnmSpriteCount'),
      scripts: parseCppIntegerConstant(header, 'kTh07AnmScriptCount'),
      rows: parseAnmResourceRows(cpp),
    },
    anmVm: {
      instructions: parseCppIntegerConstant(header, 'kTh07AnmVmScriptInstructionCount'),
      terminators: parseCppIntegerConstant(header, 'kTh07AnmVmScriptTerminatorCount'),
      opcodeCount: parseCppIntegerConstant(header, 'kTh07AnmVmOpcodeCount'),
      maxOpcode: parseCppIntegerConstant(header, 'kTh07AnmVmMaxOpcode'),
      opcodes: parseCppIntegerList(extractCppInitializerList(cpp, 'kTh07AnmVmOpcodes')),
      sizeRows: parseAnmVmInstructionSizeRows(cpp),
    },
    anmVmOpcodeUsage: {
      count: parseCppIntegerConstant(header, 'kTh07AnmVmOpcodeUsageCount'),
      rows: parseAnmVmOpcodeUsageRows(cpp),
    },
    anmVmOpcodeArgShapes: {
      count: parseCppIntegerConstant(header, 'kTh07AnmVmOpcodeArgShapeCount'),
      totalArgBytes: parseCppIntegerConstant(header, 'kTh07AnmVmTotalArgByteCount'),
      rows: parseAnmVmOpcodeArgShapeRows(cpp),
    },
    std: {
      files: parseCppIntegerConstant(header, 'kTh07StdFileCount'),
      objects: parseCppIntegerConstant(header, 'kTh07StdBinaryObjectCount'),
      quads: parseCppIntegerConstant(header, 'kTh07StdBinaryQuadCount'),
      instances: parseCppIntegerConstant(header, 'kTh07StdBinaryInstanceCount'),
      scriptInstructions: parseCppIntegerConstant(header, 'kTh07StdBinaryScriptInstructionCount'),
      readableDumps: parseCppIntegerConstant(header, 'kTh07ReadableStdDumpCount'),
      emptyReadableDumpStage: parseCppIntegerConstant(header, 'kTh07EmptyReadableStdDumpStage'),
      rows: parseStdResourceRows(cpp),
    },
    stdOpcodeUsage: {
      count: parseCppIntegerConstant(header, 'kTh07StdBinaryOpcodeUsageCount'),
      rows: parseStdOpcodeUsageRows(cpp),
    },
    stdOpcodeArgShapes: {
      count: parseCppIntegerConstant(header, 'kTh07StdBinaryOpcodeArgShapeCount'),
      totalArgBytes: parseCppIntegerConstant(header, 'kTh07StdBinaryTotalArgByteCount'),
      rows: parseStdOpcodeArgShapeRows(cpp),
    },
    msg: {
      dumps: parseCppIntegerConstant(header, 'kTh07MsgDumpCount'),
      entries: parseCppIntegerConstant(header, 'kTh07MsgEntryCount'),
      timelineLabels: parseCppIntegerConstant(header, 'kTh07MsgTimelineLabelCount'),
      rows: parseMsgResourceRows(cpp),
    },
    msgBinary: {
      entryPointers: parseCppIntegerConstant(header, 'kTh07MsgBinaryEntryPointerCount'),
      uniqueEntries: parseCppIntegerConstant(header, 'kTh07MsgBinaryUniqueEntryCount'),
      instructions: parseCppIntegerConstant(header, 'kTh07MsgBinaryInstructionCount'),
      textBytes: parseCppIntegerConstant(header, 'kTh07MsgBinaryTextByteCount'),
      waitTotalFrames: parseCppIntegerConstant(header, 'kTh07MsgBinaryWaitTotalFrames'),
      maxOpcode: parseCppIntegerConstant(header, 'kTh07MsgBinaryMaxOpcode'),
      rows: parseMsgBinaryResourceRows(cpp),
    },
    msgBinaryOpcodeUsage: {
      count: parseCppIntegerConstant(header, 'kTh07MsgBinaryOpcodeUsageCount'),
      rows: parseMsgBinaryOpcodeUsageRows(cpp),
    },
    msgBinaryOpcodeArgShapes: {
      count: parseCppIntegerConstant(header, 'kTh07MsgBinaryOpcodeArgShapeCount'),
      totalArgBytes: parseCppIntegerConstant(header, 'kTh07MsgBinaryTotalArgByteCount'),
      rows: parseMsgBinaryOpcodeArgShapeRows(cpp),
    },
  };
}

function parseAnmResourceRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07AnmFileSummaries');
  const rowPattern = /^\s*\{"([^"]+)",\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(true|false)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    fileName: match[1],
    sprites: Number(match[2]),
    scripts: Number(match[3]),
    textureWidth: Number(match[4]),
    textureHeight: Number(match[5]),
    version: Number(match[6]),
    hasTextureData: match[7] === 'true',
  }));
  assert.equal(rows.length, 63, 'Expected 63 C++ ANM resource rows');
  return rows;
}

function parseRuntimeAnmManifest() {
  const header = readFileSync(join(root, 'src/Th07ResourceManifest.hpp'), 'utf8');
  const cpp = readFileSync(join(root, 'src/Th07ResourceManifest.cpp'), 'utf8');
  const expectedRows = parseCppIntegerConstant(header, 'kTh07RuntimeAnmFileCount');
  const block = extractCppArrayBlock(cpp, 'kTh07RuntimeAnmFiles');
  const rowPattern = /^\s*\{"([^"]+)",\s*"([0-9a-fA-F]+)"\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    archivePath: match[1],
    ghidraStringAddress: match[2],
  }));
  assert.equal(expectedRows, 63, 'Expected 63 TH07 runtime ANM manifest rows');
  assert.equal(rows.length, expectedRows, 'C++ runtime ANM manifest row count changed');
  return rows;
}

function parseRuntimeAnmLoadContracts(runtimeAnmManifest) {
  const manifestPaths = new Set(runtimeAnmManifest.map((row) => row.archivePath));
  const header = readFileSync(join(root, 'src/Th07ResourceManifest.hpp'), 'utf8');
  const cpp = readFileSync(join(root, 'src/Th07ResourceManifest.cpp'), 'utf8');
  const expectedRows = parseCppIntegerConstant(header, 'kTh07RuntimeAnmLoadContractCount');
  const block = extractCppArrayBlock(cpp, 'kTh07RuntimeAnmLoadContracts');
  const rowPattern =
    /^\s*\{\{"([^"]+)",\s*"([0-9a-fA-F]+)"\},\s*"([0-9a-fA-F]+)",\s*(0x[0-9a-fA-F]+),\s*(0x[0-9a-fA-F]+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    archivePath: match[1],
    ghidraStringAddress: match[2],
    evidenceFunction: match[3],
    anmFileSlot: Number.parseInt(match[4], 16),
    spriteScriptOffset: Number.parseInt(match[5], 16),
  }));
  assert.equal(expectedRows, 46, 'Expected 46 TH07 runtime ANM load contracts');
  assert.equal(rows.length, expectedRows, 'C++ runtime ANM load contract row count changed');
  for (const row of rows) {
    assert.ok(manifestPaths.has(row.archivePath), `${row.archivePath} load contract has no TH07 ANM string anchor`);
  }
  return rows;
}

function parseRuntimeAnmStringAnchors(anmFiles) {
  const extracted = new Set(anmFiles);
  const strings = readFileSync(join(root, 'reference/ghidra/strings.tsv'), 'utf8');
  const rows = [];
  for (const line of strings.split(/\r?\n/)) {
    const [address, archivePath] = line.split('\t');
    if (!archivePath || !archivePath.startsWith('data/') || !archivePath.endsWith('.anm')) {
      continue;
    }
    const fileName = archivePath.split('/').pop();
    assert.ok(extracted.has(fileName), `${archivePath} string anchor has no extracted ANM file`);
    rows.push({
      archivePath,
      ghidraStringAddress: address,
    });
  }
  assert.equal(rows.length, 63, 'Expected 63 runtime ANM string anchors');
  return rows;
}

function countBy(rows, key) {
  return rows.reduce((counts, row) => {
    counts[row[key]] = (counts[row[key]] ?? 0) + 1;
    return counts;
  }, {});
}

function auditLiveRuntimeAnmLoads(runtimeAnmManifest) {
  const manifestPaths = new Set(runtimeAnmManifest.map((row) => row.archivePath));
  const sourceDir = join(root, 'src');
  let totalLoads = 0;
  let sourceDerivedLoads = 0;
  const obsoleteLoads = [];
  for (const sourceFile of readdirSync(sourceDir).filter((file) => file.endsWith('.cpp')).sort()) {
    const text = readFileSync(join(sourceDir, sourceFile), 'utf8');
    for (const match of text.matchAll(/LoadAnm\s*\([^;]*?"(data\/[^"]+\.anm)"/gs)) {
      totalLoads++;
      const archivePath = match[1];
      if (manifestPaths.has(archivePath)) {
        sourceDerivedLoads++;
      } else {
        obsoleteLoads.push({
          sourceFile,
          archivePath,
          status: 'obsolete-th06-carryover',
        });
      }
    }
  }
  return {
    totalLoads,
    sourceDerivedLoads,
    obsoleteLoadCount: obsoleteLoads.length,
    obsoleteLoads,
  };
}

function parseAnmVmInstructionSizeRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07AnmVmInstructionSizeSummaries');
  const rowPattern = /^\s*\{(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    size: Number(match[1]),
    instructions: Number(match[2]),
  }));
  assert.equal(rows.length, 6, 'Expected 6 C++ ANM VM instruction size rows');
  return rows;
}

function parseAnmVmOpcodeUsageRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07AnmVmOpcodeUsage');
  const rowPattern = /^\s*\{(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
  }));
  assert.equal(rows.length, 44, 'Expected 44 C++ ANM VM opcode usage rows');
  return rows;
}

function parseAnmVmOpcodeArgShapeRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07AnmVmOpcodeArgShapes');
  const rowPattern = /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
    minArgSize: Number(match[3]),
    maxArgSize: Number(match[4]),
    totalArgBytes: Number(match[5]),
  }));
  assert.equal(rows.length, 44, 'Expected 44 C++ ANM VM opcode argument-shape rows');
  return rows;
}

function parsePlayerShotTables() {
  const header = readFileSync(join(root, 'src/Th07PlayerShotTables.hpp'), 'utf8');
  const rows = [];
  const rowPattern =
    /\{Th07ShotType::(\w+),\s*(true|false),\s*"([^"]+)",\s*(\d+),\s*(\d+),\s*(\d+),\s*\{([^}]*)\}\},/g;
  for (const match of header.matchAll(rowPattern)) {
    rows.push({
      shotType: match[1],
      focused: match[2] === 'true',
      fileName: match[3],
      fileSize: Number(match[4]),
      shotRecordCount: Number(match[5]),
      deathbombWindowFrames: Number(match[6]),
      tuningFloats: parseCppFloatList(match[7]),
    });
  }
  assert.equal(rows.length, 12, 'Expected 12 C++ player SHT rows');

  return {
    files: parseCppIntegerConstant(header, 'kTh07PlayerShotFileCount'),
    powerLevels: parseCppIntegerConstant(header, 'kTh07PlayerShtPowerLevelCount'),
    shotRecords: parseCppIntegerConstant(header, 'kTh07PlayerShtShotRecordCount'),
    powerThresholds: parseCppIntegerList(extractCppInitializerList(header, 'kTh07PlayerShtPowerThresholds')),
    callbackUsage: {
      spawn: parseCppIntegerList(extractCppInitializerList(header, 'kTh07ShtSpawnCallbackUsage')),
      update: parseCppIntegerList(extractCppInitializerList(header, 'kTh07ShtUpdateCallbackUsage')),
      draw: parseCppIntegerList(extractCppInitializerList(header, 'kTh07ShtDrawCallbackUsage')),
      collision: parseCppIntegerList(extractCppInitializerList(header, 'kTh07ShtCollisionCallbackUsage')),
    },
    rows,
  };
}

function parseBulletTables() {
  const header = readFileSync(join(root, 'src/Th07BulletTables.hpp'), 'utf8');
  const resourceMatch = header.match(
    /kTh07EnemyBulletAnmResource\s*=\s*\{\s*"([^"]+)",\s*"([^"]+)",\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(true|false),\s*Th07BulletEvidenceStatus::(\w+),\s*\};/m);
  assert.ok(resourceMatch, 'Missing C++ TH07 bullet ANM resource metadata');

  const templates = parseBulletTemplateRows(header);
  assert.equal(templates.length, 11, 'Expected 11 C++ TH07 enemy bullet template rows');

  const carryovers = [...header.matchAll(
    /\{\s*"([^"]+)",\s*"([^"]+)",\s*Th07BulletEvidenceStatus::(\w+)\s*\}/g)]
    .map((match) => ({
      token: match[1],
      reason: match[2],
      status: bulletStatusName(match[3]),
    }));
  assert.equal(carryovers.length, 4, 'Expected 4 obsolete TH06 bullet carryover records');

  return {
    resource: {
      archivePath: resourceMatch[1],
      ghidraStringAddress: resourceMatch[2],
      sprites: Number(resourceMatch[3]),
      scripts: Number(resourceMatch[4]),
      textureWidth: Number(resourceMatch[5]),
      textureHeight: Number(resourceMatch[6]),
      version: Number(resourceMatch[7]),
      hasTextureData: resourceMatch[8] === 'true',
      status: bulletStatusName(resourceMatch[9]),
    },
    templateConstants: {
      loadFunctionAddress: parseCppHexConstant(header, 'kTh07EnemyBulletLoadFunctionAddress'),
      tableAddress: parseCppHexConstant(header, 'kTh07EnemyBulletTemplateTableAddress'),
      tableFileOffset: parseCppHexConstant(header, 'kTh07EnemyBulletTemplateTableFileOffset'),
      entrySize: parseCppHexConstant(header, 'kTh07EnemyBulletTemplateEntrySize'),
      count: parseCppIntegerConstant(header, 'kTh07EnemyBulletTemplateCount'),
      resolvedCount: parseCppIntegerConstant(header, 'kTh07EnemyBulletResolvedTemplateCount'),
      managerSlot: parseCppHexConstant(header, 'kTh07EnemyBulletAnmManagerSlot'),
      scriptOffset: parseCppHexConstant(header, 'kTh07EnemyBulletAnmScriptOffset'),
      resolvedScriptLast: parseCppHexConstant(header, 'kTh07EnemyBulletResolvedScriptLast'),
      stride: parseCppHexConstant(header, 'kTh07EnemyBulletTemplateStride'),
    },
    templates,
    carryovers,
  };
}

function parseBulletTemplateRows(header) {
  const block = extractCppArrayBlock(header, 'kTh07EnemyBulletTemplateScripts');
  const rowPattern = /^\s*\{(\d+),\s*(0x[0-9a-f]+),\s*(0x[0-9a-f]+),\s*(0x[0-9a-f]+),\s*(0x[0-9a-f]+),\s*(0x[0-9a-f]+),\s*(kTh07EnemyBulletTemplateMissingSprite|\d+),\s*(\d+),\s*(\d+),\s*([0-9.]+f),\s*(\d+),\s*Th07BulletEvidenceStatus::(\w+)\},/gmi;
  return [...block.matchAll(rowPattern)].map((match) => ({
    slot: Number(match[1]),
    bulletScript: Number.parseInt(match[2], 16),
    spawnFastScript: Number.parseInt(match[3], 16),
    spawnNormalScript: Number.parseInt(match[4], 16),
    spawnSlowScript: Number.parseInt(match[5], 16),
    spawnDonutScript: Number.parseInt(match[6], 16),
    activeSprite: match[7] === 'kTh07EnemyBulletTemplateMissingSprite' ? 0xffff : Number(match[7]),
    spriteWidth: Number(match[8]),
    spriteHeight: Number(match[9]),
    grazeSize: match[10],
    spriteOffsetCategory: Number(match[11]),
    status: bulletStatusName(match[12]),
  }));
}

function bulletStatusName(name) {
  switch (name) {
  case 'SourceDerived':
    return 'source-derived';
  case 'ObsoleteTh06Carryover':
    return 'obsolete-th06-carryover';
  case 'Pending':
    return 'pending';
  case 'Blocked':
    return 'blocked';
  default:
    return name;
  }
}

function parseCppIntegerConstant(text, name) {
  const match = text.match(new RegExp(`\\b${name}\\s*=\\s*(\\d+)`));
  assert.ok(match, `Missing C++ resource constant ${name}`);
  return Number(match[1]);
}

function parseCppHexConstant(text, name) {
  const match = text.match(new RegExp(`\\b${name}\\s*=\\s*(0x[0-9a-fA-F]+)`));
  assert.ok(match, `Missing C++ resource constant ${name}`);
  return Number.parseInt(match[1], 16);
}

function parseStdResourceRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07StdBinarySummaries');
  const rowPattern = /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*OpcodeMask\(\{([^}]*)\}\),\s*(true|false)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    stage: Number(match[1]),
    objects: Number(match[2]),
    quads: Number(match[3]),
    instances: Number(match[4]),
    scriptInstructions: Number(match[5]),
    opcodes: parseCppIntegerList(match[6]),
    readableDumpAvailable: match[7] === 'true',
  }));
  assert.equal(rows.length, 8, 'Expected 8 C++ STD resource rows');
  return rows;
}

function parseStdOpcodeUsageRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07StdBinaryOpcodeUsage');
  const rowPattern = /^\s*\{(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
  }));
  assert.equal(rows.length, 25, 'Expected 25 C++ STD opcode usage rows');
  return rows;
}

function parseStdOpcodeArgShapeRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07StdBinaryOpcodeArgShapes');
  const rowPattern = /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
    minArgSize: Number(match[3]),
    maxArgSize: Number(match[4]),
    totalArgBytes: Number(match[5]),
  }));
  assert.equal(rows.length, 25, 'Expected 25 C++ STD binary opcode argument-shape rows');
  return rows;
}

function parseMsgResourceRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07MsgDumpSummaries');
  const rowPattern = /^\s*\{(\d+),\s*(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    stage: Number(match[1]),
    entries: Number(match[2]),
    timelineLabels: Number(match[3]),
  }));
  assert.equal(rows.length, 8, 'Expected 8 C++ MSG resource rows');
  return rows;
}

function parseMsgBinaryResourceRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07MsgBinarySummaries');
  const rowPattern =
    /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*MsgOpcodeMask\(\{([^}]*)\}\),\s*(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    stage: Number(match[1]),
    fileSize: Number(match[2]),
    entryPointers: Number(match[3]),
    uniqueEntries: Number(match[4]),
    instructions: Number(match[5]),
    opcodes: parseCppIntegerList(match[6]),
    textBytes: Number(match[7]),
    waitTotalFrames: Number(match[8]),
  }));
  assert.equal(rows.length, 8, 'Expected 8 C++ MSG binary rows');
  return rows;
}

function parseMsgBinaryOpcodeUsageRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07MsgBinaryOpcodeUsage');
  const rowPattern = /^\s*\{(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
  }));
  assert.equal(rows.length, 15, 'Expected 15 C++ MSG binary opcode usage rows');
  return rows;
}

function parseMsgBinaryOpcodeArgShapeRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07MsgBinaryOpcodeArgShapes');
  const rowPattern = /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+)\},/gm;
  const rows = [...block.matchAll(rowPattern)].map((match) => ({
    opcode: Number(match[1]),
    instructions: Number(match[2]),
    minArgSize: Number(match[3]),
    maxArgSize: Number(match[4]),
    totalArgBytes: Number(match[5]),
  }));
  assert.equal(rows.length, 15, 'Expected 15 C++ MSG binary opcode argument-shape rows');
  return rows;
}

function extractCppArrayBlock(text, name) {
  const match = text.match(new RegExp(`${name}\\s*=\\s*\\{\\{([\\s\\S]*?)\\}\\};`));
  assert.ok(match, `Missing C++ resource array ${name}`);
  return match[1];
}

function parseCppIntegerList(text) {
  if (text.trim().length === 0) {
    return [];
  }
  return text.split(',')
    .map((value) => value.trim())
    .filter((value) => value.length > 0)
    .map((value) => Number(value));
}

function parseCppFloatList(text) {
  if (text.trim().length === 0) {
    return [];
  }
  return text.split(',')
    .map((value) => value.trim())
    .filter((value) => value.length > 0)
    .map((value) => Number(value.replace(/f$/, '')));
}

function extractCppInitializerList(text, name) {
  const match = text.match(new RegExp(`${name}\\s*=\\s*\\{([^}]*)\\}`));
  assert.ok(match, `Missing C++ initializer list ${name}`);
  return match[1];
}

function incrementUsage(usage, id, label) {
  assert.ok(id >= 0 && id < usage.length, `${label} callback id out of range: ${id}`);
  usage[id]++;
}

function accumulateUsage(total, added, label) {
  assert.equal(total.length, added.length, `${label} callback usage width changed`);
  for (let index = 0; index < total.length; index++) {
    total[index] += added[index];
  }
}

function assertFloatArrayNearlyEqual(actual, expected, label) {
  assert.equal(actual.length, expected.length, `${label}: length changed`);
  for (let index = 0; index < actual.length; index++) {
    assert.ok(Math.abs(actual[index] - expected[index]) < 0.0001,
      `${label}: index ${index} expected ${expected[index]}, found ${actual[index]}`);
  }
}
