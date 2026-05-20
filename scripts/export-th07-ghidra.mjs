import { existsSync, mkdirSync } from 'node:fs';
import { join } from 'node:path';
import { spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { tmpdir } from 'node:os';

const root = fileURLToPath(new URL('../', import.meta.url));
const ghidraHome = process.env.GHIDRA_HOME || '/opt/ghidra';
const analyzeHeadless = join(ghidraHome, 'support/analyzeHeadless');
const exe = join(root, 'reference/original/th07.exe');
const projectDir = join(root, 'reference/ghidra/project');
const outDir = join(root, 'reference/ghidra');
const scriptDir = join(root, 'scripts/ghidra');
const writableHome = join(tmpdir(), 'ghidra-th07-home');
const writableConfig = join(tmpdir(), 'ghidra-th07-config');
const writableCache = join(tmpdir(), 'ghidra-th07-cache');

if (!existsSync(analyzeHeadless)) throw new Error(`Ghidra analyzeHeadless not found: ${analyzeHeadless}`);
if (!existsSync(exe)) throw new Error(`TH07 executable not found: ${exe}`);

mkdirSync(projectDir, { recursive: true });
mkdirSync(outDir, { recursive: true });
mkdirSync(writableHome, { recursive: true });
mkdirSync(writableConfig, { recursive: true });
mkdirSync(writableCache, { recursive: true });

const result = spawnSync(analyzeHeadless, [
  projectDir,
  'th07',
  '-import', exe,
  '-overwrite',
  '-scriptPath', scriptDir,
  '-postScript', 'ExportTh07Metadata.java', outDir
], {
  cwd: root,
  stdio: 'inherit',
  env: {
    ...process.env,
    HOME: writableHome,
    XDG_CONFIG_HOME: writableConfig,
    XDG_CACHE_HOME: writableCache
  }
});

if (result.status !== 0) {
  throw new Error(`Ghidra export failed with status ${result.status}. If this is a JDK issue, set GHIDRA_HOME or configure /opt/ghidra/support/launch.properties with a supported writable JDK setup.`);
}
