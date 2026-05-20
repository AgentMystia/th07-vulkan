import { copyFileSync, createReadStream, createWriteStream, existsSync, mkdirSync, readdirSync, readFileSync, statSync, writeFileSync } from 'node:fs';
import { rm } from 'node:fs/promises';
import { basename, dirname, extname, join } from 'node:path';
import { spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { tmpdir } from 'node:os';

const root = fileURLToPath(new URL('../', import.meta.url));
const gameDir = join(root, 'reference/original');
const thtkZip = '/home/mystia/下载/thtk-bin-12.zip';
const toolRoot = join(tmpdir(), 'thtk-th07');
const toolDir = join(toolRoot, 'thtk-bin-12');
const workDir = join(tmpdir(), 'th07-resource-work');
const originalDir = join(root, 'reference/extracted');
const eclOutDir = join(root, 'reference/ecl');
const stdOutDir = join(root, 'reference/std');
const msgOutDir = join(root, 'reference/msg');
const imgOutDir = join(root, 'reference/generated/th07-img');
const sfxOutDir = join(root, 'reference/generated/sfx');
const audioOutDir = join(root, 'reference/generated/audio');

const runtimeAnmFiles = [
  'ascii.anm', 'capture.anm', 'front.anm', 'etama.anm',
  'player00.anm', 'player01.anm', 'player02.anm',
  'title01.anm', 'result00.anm', 'music00.anm', 'staff01.anm',
  'loading.anm', 'loading2.anm', 'loading3.anm',
  ...Array.from({ length: 8 }, (_, i) => `std${i + 1}txt.anm`),
  ...Array.from({ length: 8 }, (_, i) => `stg${i + 1}bg.anm`),
  'stg4bg2.anm', 'stg4bg3.anm', 'stg4bg4.anm', 'stg4bg5.anm',
  ...Array.from({ length: 8 }, (_, i) => `stg${i + 1}enm.anm`),
  ...Array.from({ length: 8 }, (_, i) => `eff0${i + 1}.anm`),
  'eff04b.anm',
  ...Array.from({ length: 8 }, (_, i) => `face_0${i + 1}_00.anm`),
  'face_rm00.anm', 'face_mr00.anm', 'face_sk00.anm'
];

const directJpgFiles = [
  'title00.jpg', 'select00.jpg', 'th07logo.jpg', 'phantasm.jpg',
  'music.jpg', 'result.jpg', 'staff00.jpg',
  ...Array.from({ length: 22 }, (_, i) => `end${String(i).padStart(2, '0')}.jpg`),
  'end00b.jpg', 'end02b.jpg', 'end10b.jpg', 'end20b.jpg'
];

function run(command, args, options = {}) {
  const result = spawnSync(command, args, {
    cwd: options.cwd || root,
    encoding: options.encoding ?? 'utf8',
    stdio: options.capture ? ['ignore', 'pipe', 'pipe'] : 'inherit',
    env: {
      ...process.env,
      OMP_NUM_THREADS: '1',
      WINEDEBUG: process.env.WINEDEBUG || '-all'
    }
  });
  if (result.status !== 0 && !options.optional) {
    const stderr = result.stderr ? `\n${result.stderr}` : '';
    throw new Error(`${command} ${args.join(' ')} failed with ${result.status}${stderr}`);
  }
  return result;
}

function ensureDir(path) {
  mkdirSync(path, { recursive: true });
}

function ensureFile(path, label = path) {
  if (!existsSync(path)) throw new Error(`${label} is missing: ${path}`);
}

function tool(name) {
  return join(toolDir, name);
}

function prepareTools() {
  ensureFile(thtkZip, 'thtk zip');
  ensureDir(toolRoot);
  if (!existsSync(tool('thdat.exe'))) run('unzip', ['-qo', thtkZip, '-d', toolRoot]);
  for (const name of ['thdat.exe', 'thanm.exe', 'thecl.exe', 'thstd.exe', 'thmsg.exe']) ensureFile(tool(name), name);
}

function copyGameArchives() {
  ensureDir(workDir);
  const dat = join(workDir, 'th07.dat');
  const bgm = join(workDir, 'thbgm.dat');
  copyFileSync(join(gameDir, 'th07.dat'), dat);
  copyFileSync(join(gameDir, 'thbgm.dat'), bgm);
  return { dat, bgm };
}

function archiveList(dat) {
  const result = run('wine', [tool('thdat.exe'), '-l', 'd', dat], { capture: true });
  const files = [];
  for (const line of result.stdout.split(/\r?\n/)) {
    const match = line.match(/^(\S+)\s+\d+\s+\d+\s*$/);
    if (match && match[1] !== 'Name') files.push(match[1]);
  }
  if (files.length < 120) throw new Error(`Unexpectedly short th07.dat listing: ${files.length} files`);
  return files;
}

function extractArchive(dat) {
  ensureDir(originalDir);
  const files = archiveList(dat);
  for (let i = 0; i < files.length; i += 24) {
    const chunk = files.slice(i, i + 24);
    run('wine', [tool('thdat.exe'), '-x', 'd', dat, ...chunk], { cwd: originalDir });
  }
  return files;
}

function decompileReferences() {
  ensureDir(eclOutDir);
  ensureDir(stdOutDir);
  ensureDir(msgOutDir);
  const decompileWork = join(workDir, 'decompile');
  ensureDir(decompileWork);
  const decompileOne = (toolName, args, input, output) => {
    const tempInput = join(decompileWork, basename(input));
    const tempOutput = join(decompileWork, basename(output));
    copyFileSync(input, tempInput);
    run('wine', [tool(toolName), ...args, tempInput, tempOutput], { optional: true });
    if (existsSync(tempOutput)) copyFileSync(tempOutput, output);
  };
  for (let stage = 1; stage <= 8; stage++) {
    const ecl = join(originalDir, `ecldata${stage}.ecl`);
    const std = join(originalDir, `stage${stage}.std`);
    const msg = join(originalDir, `msg${stage}.dat`);
    if (existsSync(ecl)) decompileOne('thecl.exe', ['-d7'], ecl, join(eclOutDir, `ecldata${stage}.decl`));
    if (existsSync(std)) decompileOne('thstd.exe', ['-d7'], std, join(stdOutDir, `stage${stage}.dstd`));
    if (existsSync(msg)) decompileOne('thmsg.exe', ['-d7'], msg, join(msgOutDir, `msg${stage}.txt`));
  }
}

function walkFiles(dir) {
  const out = [];
  if (!existsSync(dir)) return out;
  for (const entry of readdirSync(dir)) {
    const path = join(dir, entry);
    if (statSync(path).isDirectory()) out.push(...walkFiles(path));
    else out.push(path);
  }
  return out;
}

async function resetDir(path) {
  await rm(path, { recursive: true, force: true });
  ensureDir(path);
}

async function extractAnmImages() {
  ensureDir(join(imgOutDir, 'png'));
  ensureDir(join(imgOutDir, 'jpg'));
  const thanmWork = join(workDir, 'thanm');
  await resetDir(thanmWork);
  for (const anm of runtimeAnmFiles) {
    const source = join(originalDir, anm);
    if (!existsSync(source)) continue;
    await resetDir(thanmWork);
    copyFileSync(source, join(thanmWork, anm));
    run('wine', [tool('thanm.exe'), '-x', '-f', anm], { cwd: thanmWork, optional: true });
    for (const image of walkFiles(join(thanmWork, 'data'))) {
      const ext = extname(image).toLowerCase();
      if (ext !== '.png' && ext !== '.jpg' && ext !== '.jpeg') continue;
      const targetDir = ext === '.png' ? join(imgOutDir, 'png') : join(imgOutDir, 'jpg');
      copyFileSync(image, join(targetDir, basename(image)));
    }
  }
  for (const jpg of directJpgFiles) {
    const source = join(originalDir, jpg);
    if (existsSync(source)) copyFileSync(source, join(imgOutDir, 'jpg', jpg));
  }
}

function copySfx() {
  ensureDir(sfxOutDir);
  for (const file of readdirSync(originalDir)) {
    if (/^se_.*\.wav$/i.test(file)) copyFileSync(join(originalDir, file), join(sfxOutDir, file));
  }
}

function parseBgmFmt() {
  const fmt = readFileSync(join(originalDir, 'thbgm.fmt'));
  const entries = [];
  for (let off = 0; off + 52 <= fmt.length; off += 52) {
    const rawName = fmt.subarray(off, off + 16);
    const nul = rawName.indexOf(0);
    const name = rawName.subarray(0, nul >= 0 ? nul : rawName.length).toString('ascii');
    if (!name) break;
    entries.push({
      name,
      ogg: name.replace(/\.wav$/i, '.ogg'),
      startByte: fmt.readUInt32LE(off + 16),
      introField: fmt.readUInt32LE(off + 20),
      loopStartByte: fmt.readUInt32LE(off + 24),
      byteLength: fmt.readUInt32LE(off + 28),
      channels: fmt.readUInt16LE(off + 34),
      sampleRate: fmt.readUInt32LE(off + 36),
      avgBytesPerSecond: fmt.readUInt32LE(off + 40),
      blockAlign: fmt.readUInt16LE(off + 44),
      bitsPerSample: fmt.readUInt16LE(off + 46)
    });
  }
  return entries;
}

function extractBgm(bgmDat) {
  if (process.argv.includes('--no-bgm')) return;
  ensureDir(audioOutDir);
  const entries = parseBgmFmt();
  const fd = readFileSync(bgmDat);
  const loopMeta = [];
  for (const entry of entries) {
    const target = join(audioOutDir, entry.ogg);
    const raw = join(workDir, entry.name);
    if (!existsSync(target)) {
      writeFileSync(raw, fd.subarray(entry.startByte, entry.startByte + entry.byteLength));
      run('ffmpeg', [
        '-hide_banner', '-loglevel', 'error', '-y',
        '-f', 's16le',
        '-ar', String(entry.sampleRate || 44100),
        '-ac', String(entry.channels || 2),
        '-i', raw,
        '-c:a', 'libopus',
        '-b:a', '112k',
        target
      ]);
    }
    loopMeta.push(entry);
  }
  writeFileSync(join(audioOutDir, 'th07-bgm-loop.json'), `${JSON.stringify(loopMeta, null, 2)}\n`);
}

async function main() {
  prepareTools();
  const { dat, bgm } = copyGameArchives();
  const files = extractArchive(dat);
  decompileReferences();
  await extractAnmImages();
  copySfx();
  extractBgm(bgm);
  console.log(`Extracted ${files.length} TH07 archive entries into reference/extracted.`);
}

main().catch((error) => {
  console.error(error);
  process.exitCode = 1;
});
