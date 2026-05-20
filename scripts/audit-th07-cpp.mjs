import { existsSync, readFileSync, readdirSync, statSync } from 'node:fs';
import { join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));
const recon = join(root, 'src/reconstruction');
const src = join(recon, 'original-engine');
const stringsPath = join(root, 'reference/ghidra/strings.tsv');
const decompManifestPath = join(root, 'reference/ghidra/decomp/manifest.tsv');

const requiredFiles = [
  'GameManager.hpp',
  'GameManager.cpp',
  'Player.hpp',
  'Player.cpp',
  'Stage.cpp',
  'MainMenu.cpp',
  'ResultScreen.cpp',
  'th07.hpp',
  'Th07ResourceManifest.hpp',
  'Th07ResourceManifest.cpp',
  'ShtManager.hpp',
  'ShtManager.cpp',
  'CherrySystem.hpp',
  'CherrySystem.cpp',
  'BorderSystem.hpp',
  'BorderSystem.cpp',
  'ClearBonusSystem.hpp',
  'ClearBonusSystem.cpp',
];

const problems = [];
for (const file of requiredFiles) {
  if (!existsSync(join(src, file))) problems.push(`missing reconstructed file: ${file}`);
}

if (!existsSync(join(recon, 'PROVENANCE.md'))) problems.push('missing reconstructed provenance file');

const allCppText = allFiles(src)
  .filter((file) => /\.(cpp|hpp|rc|tpl)$/.test(file))
  .map((file) => readFileSync(file, 'utf8'))
  .join('\n');

mustContain(allCppText, 'namespace th07', 'copied source namespace was not rewritten to th07');
mustNotContain(allCppText, 'namespace th06', 'copied source still contains namespace th06');

const gmH = read('GameManager.hpp');
mustContain(gmH, '#define PSCR_NUM_CHARS_SHOTTYPES 6', 'TH07 score table should cover 6 shot types');
mustContain(gmH, '#define PSCR_NUM_STAGES 8', 'TH07 score table should cover 8 stage routes');
mustContain(gmH, '#define CLRD_NUM_CHARACTERS 6', 'TH07 clear table should cover 6 shot types');
mustContain(gmH, '#define CATK_NUM_CAPTURES 141', 'TH07 spell/capture table should cover 141 names');
mustContain(gmH, 'PHANTASM_STAGE', 'TH07 stage enum should include Phantasm');

const playerH = read('Player.hpp');
mustContain(playerH, 'CHARA_SAKUYA', 'TH07 player enum should include Sakuya');

const manifest = read('Th07ResourceManifest.cpp');
mustEqual(countMatches(manifest, /ResourceStringAnchor\{"data\/ecldata\d\.ecl"/g), 8, 'ECL manifest entry count');
mustEqual(countMatches(manifest, /ResourceStringAnchor\{"data\/stage\d\.std"/g), 8, 'STD manifest entry count');
mustEqual(countMatches(manifest, /ResourceStringAnchor\{"data\/msg\d\.dat"/g), 8, 'MSG manifest entry count');

const sht = read('ShtManager.cpp');
mustEqual(countMatches(sht, /ShtFileBinding\{/g), 12, 'SHT binding count');
mustContain(sht, 'data/ply02bs.sht', 'SakuyaB focused SHT binding missing');
mustOrdered(sht, [
  'data/ply00a.sht',
  'data/ply00b.sht',
  'data/ply01a.sht',
  'data/ply01b.sht',
  'data/ply02a.sht',
  'data/ply02b.sht',
  'data/ply00as.sht',
  'data/ply00bs.sht',
  'data/ply01as.sht',
  'data/ply01bs.sht',
  'data/ply02as.sht',
  'data/ply02bs.sht',
], 'SHT binding order should match TH07 table 0049f530..0049f55c');

const cherry = read('CherrySystem.cpp');
mustContain(cherry, 'CherryPoint Max!', 'Cherry HUD anchor missing');

const border = read('BorderSystem.cpp');
mustContain(border, 'Supernatural Border!!', 'Border HUD anchor missing');
mustContain(border, 'Phantasm Rank*2.0', 'Phantasm rank HUD anchor missing');

const clearBonus = read('ClearBonusSystem.cpp');
for (const value of ['Easy Rank    *0.5', 'Normal Rank  *1.0', 'Hard Rank    *1.2', 'Lunatic Rank *1.5', 'Extra Rank   *2.0', 'Phantasm Rank*2.0']) {
  mustContain(clearBonus, value, `TH07 clear bonus rank rule missing ${value}`);
}
for (const value of ['1000000', '50000', '500', '20000000', '4000000']) {
  mustContain(read('ClearBonusSystem.hpp'), value, `TH07 clear bonus constant missing ${value}`);
}

const ascii = read('AsciiManager.cpp');
mustNotContain(ascii, 'data/asciis.anm', 'TH07 archive has no asciis.anm; TH06 ASCII small-font load must be removed');

const anmIdx = read('AnmIdx.hpp');
mustContain(anmIdx, '#define ANM_FILE_STAGE_TEXT 0x18', 'TH07 stdtxt ANM file slot missing');
mustContain(anmIdx, '#define ANM_FILE_CAPTURE 4', 'TH07 capture ANM file slot should match ASCII decompile sample');
mustContain(anmIdx, '#define ANM_FILE_STAGEBG 5', 'TH07 stage background ANM slot should match FUN_00407000');
mustContain(anmIdx, '#define ANM_FILE_STAGEBG5 9', 'TH07 Stage 4 extra background ANM slots missing');
mustContain(anmIdx, '#define ANM_FILE_PLAYER 10', 'TH07 player ANM file slot should match FUN_004423e0');
mustContain(anmIdx, '#define ANM_OFFSET_STAGE_TEXT 0x800', 'TH07 stdtxt ANM offset missing');
mustContain(anmIdx, '#define ANM_OFFSET_CAPTURE 0x724', 'TH07 capture ANM offset should match ASCII decompile sample');
mustContain(anmIdx, '#define ANM_OFFSET_STAGEBG5 0x340', 'TH07 Stage 4 extra background ANM offsets missing');
mustContain(anmIdx, '#define ANM_OFFSET_LOADING 0x61e', 'TH07 loading ANM offset should match FUN_00428b19');
mustContain(anmIdx, '#define ANM_OFFSET_FACE_STAGE_A 0x4ad', 'TH07 stage face ANM offset should match FUN_00428b19');

const gui = read('Gui.cpp');
for (const value of ['data/face_rm00.anm', 'data/face_mr00.anm', 'data/face_sk00.anm', 'data/loading2.anm', 'data/loading3.anm', 'data/face_08_00.anm', 'data/std8txt.anm', 'data/msg8.dat']) {
  mustContain(gui, value, `TH07 Gui load path missing ${value}`);
}
for (const stale of ['data/face00a.anm', 'data/face01a.anm', 'data/face03a.anm', 'data/face05a.anm', 'data/face06a.anm', 'data/face08a.anm', 'data/face09a.anm', 'data/face10a.anm', 'data/face12a.anm', 'data/face12b.anm', 'data/face12c.anm']) {
  mustNotContain(gui, stale, `TH06 Gui face path still present: ${stale}`);
}

const stageCpp = read('Stage.cpp');
for (const value of ['data/stg4bg2.anm', 'data/stg4bg3.anm', 'data/stg4bg4.anm', 'data/stg4bg5.anm']) {
  mustContain(stageCpp, value, `TH07 Stage 4 extra background load missing ${value}`);
}

const ghidraStrings = readFileSync(stringsPath, 'utf8');
for (const value of [
  'data/ply02bs.sht',
  'data/ecldata8.ecl',
  'data/stage8.std',
  'Supernatural Border!!',
  'Phantasm Rank*2.0',
  'Easy Rank    *0.5',
  'Lunatic Rank *1.5',
  'data/face_sk00.anm',
  'data/std8txt.anm',
]) {
  mustContain(ghidraStrings, value, `Ghidra string export missing ${value}`);
}

if (existsSync(decompManifestPath)) {
  const lines = readFileSync(decompManifestPath, 'utf8').trim().split(/\r?\n/);
  mustContain(lines[0] || '', 'entry\tname\tbody_size\tstatus\tfile', 'Full decompilation manifest header is invalid');
  if (lines.length < 1500) problems.push(`Full decompilation manifest is unexpectedly short: ${lines.length} lines`);
}

if (problems.length) {
  console.error(problems.join('\n'));
  process.exit(1);
}

console.log(JSON.stringify({
  reconstructedFiles: allFiles(src).length,
  eclFiles: 8,
  stdFiles: 8,
  msgFiles: 8,
  shtFiles: 12,
  catkCaptures: 141,
  fullDecompFunctions: existsSync(decompManifestPath) ? readFileSync(decompManifestPath, 'utf8').trim().split(/\r?\n/).length - 1 : 'not-exported',
}, null, 2));

function read(file) {
  const path = join(src, file);
  if (!existsSync(path)) return '';
  return readFileSync(path, 'utf8');
}

function allFiles(dir) {
  const out = [];
  if (!existsSync(dir)) return out;
  for (const entry of readdirSync(dir)) {
    const path = join(dir, entry);
    if (statSync(path).isDirectory()) out.push(...allFiles(path));
    else out.push(path);
  }
  return out;
}

function countMatches(text, pattern) {
  return [...text.matchAll(pattern)].length;
}

function mustContain(text, needle, message) {
  if (!text.includes(needle)) problems.push(message);
}

function mustNotContain(text, needle, message) {
  if (text.includes(needle)) problems.push(message);
}

function mustEqual(actual, expected, message) {
  if (actual !== expected) problems.push(`${message}: expected ${expected}, got ${actual}`);
}

function mustOrdered(text, needles, message) {
  let cursor = -1;
  for (const needle of needles) {
    const idx = text.indexOf(needle);
    if (idx < 0) {
      problems.push(`${message}: missing ${needle}`);
      return;
    }
    if (idx <= cursor) {
      problems.push(`${message}: ${needle} is out of order`);
      return;
    }
    cursor = idx;
  }
}
