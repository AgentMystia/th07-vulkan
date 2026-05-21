import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';

const doc = readFileSync(fileURLToPath(new URL('../docs/TH07_DIFF_FROM_TH06.md', import.meta.url)), 'utf8');
const restorationLedger = readFileSync(fileURLToPath(new URL('../config/subsystems.csv', import.meta.url)), 'utf8');
const required = [
  'ANM parsing',
  'STD parsing',
  'MSG parsing',
  'ECL instruction core',
  'Player roster',
  'Player shot data',
  'Cherry / Cherry+',
  'Supernatural Border',
  'Point of Collection',
  'Hitbox display',
  'BGM streaming/loop'
];

for (const label of required) assert.match(doc, new RegExp(`\\| ${label.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')} \\| (Same|Changed|New|Pending) \\|`));

const statuses = [...doc.matchAll(/\| ([^|\n]+) \| (Same|Changed|New|Pending) \| ([^|\n]+) \|/g)]
  .map((match) => ({ subsystem: match[1].trim(), status: match[2], rule: match[3].trim() }));

assert.ok(statuses.length >= 18, 'Expected a substantive TH07/TH06 diff table');
assert.ok(statuses.some((entry) => entry.status === 'New' && entry.subsystem.includes('Cherry')), 'Cherry system must be marked as TH07-new');
assert.ok(statuses.some((entry) => entry.subsystem === 'Point of Collection' && entry.status === 'Changed'), 'PoC must be marked changed from TH06-derived behavior');

const allowedRestorationStatuses = new Set([
  'exact',
  'source-derived',
  'transitional',
  'metadata-only',
  'pending',
  'blocked',
  'obsolete-th06-carryover'
]);
const allowedDependencyPhases = new Set([
  'data-format',
  'data-table',
  'object-layout',
  'pure-helper',
  'manager-layout',
  'manager-runtime',
  'interpreter-runtime',
  'gameplay-runtime',
  'gameplay-state',
  'ui-runtime',
  'platform-boundary'
]);
const ledgerRows = parseCsv(restorationLedger);
assert.deepEqual(Object.keys(ledgerRows[0] ?? {}), [
  'subsystem',
  'restoration_status',
  'dependency_phase',
  'evidence',
  'source_surface',
  'tracker',
  'next_slice',
  'blocker'
], 'Restoration subsystem ledger header changed unexpectedly');
assert.ok(ledgerRows.length >= 24, 'Expected a substantive restoration subsystem ledger');
for (const row of ledgerRows) {
  assert.ok(allowedRestorationStatuses.has(row.restoration_status),
    `Invalid restoration status for ${row.subsystem}: ${row.restoration_status}`);
  assert.ok(allowedDependencyPhases.has(row.dependency_phase),
    `Invalid dependency phase for ${row.subsystem}: ${row.dependency_phase}`);
  assert.notEqual(row.evidence, '', `Missing evidence field for ${row.subsystem}`);
  assert.notEqual(row.source_surface, '', `Missing source surface for ${row.subsystem}`);
  assert.notEqual(row.next_slice, '', `Missing next slice for ${row.subsystem}`);
}
for (const label of [
  'ResourceManifest',
  'SHT',
  'PlayerLayout',
  'PlayerRuntime',
  'BombRuntime',
  'StageObject',
  'StageAnmVm',
  'EffectTables',
  'EffectRuntime',
  'AnmManager',
  'ECL',
  'BulletLaser',
  'GameManager',
  'CherryBorder',
  'ReplayInput'
]) {
  assert.ok(ledgerRows.some((row) => row.subsystem === label), `Restoration ledger missing ${label}`);
}
assert.ok(ledgerRows.some((row) => row.restoration_status === 'metadata-only'), 'Restoration ledger should track metadata-only slices');
assert.ok(ledgerRows.some((row) => row.restoration_status === 'transitional'), 'Restoration ledger should track transitional carryover');
assert.ok(ledgerRows.some((row) => row.restoration_status === 'pending'), 'Restoration ledger should track pending slices');
assert.ok(!restorationLedger.includes('Modernized'), 'Restoration ledger still uses old Modernized status');
assert.ok(!restorationLedger.includes('src/app'), 'Restoration ledger still references removed nested app source root');

console.log(JSON.stringify({
  subsystems: statuses.length,
  same: statuses.filter((entry) => entry.status === 'Same').length,
  changed: statuses.filter((entry) => entry.status === 'Changed').length,
  new: statuses.filter((entry) => entry.status === 'New').length,
  pending: statuses.filter((entry) => entry.status === 'Pending').length,
  restorationLedgerRows: ledgerRows.length,
  restorationPending: ledgerRows.filter((entry) => entry.restoration_status === 'pending').length,
  restorationTransitional: ledgerRows.filter((entry) => entry.restoration_status === 'transitional').length,
  restorationMetadataOnly: ledgerRows.filter((entry) => entry.restoration_status === 'metadata-only').length
}, null, 2));

function parseCsv(text) {
  const lines = text.trim().split(/\r?\n/);
  const header = splitCsvLine(lines.shift() ?? '');
  return lines.filter((line) => line.trim() !== '').map((line) => {
    const values = splitCsvLine(line);
    assert.equal(values.length, header.length, `CSV row has ${values.length} fields instead of ${header.length}: ${line}`);
    return Object.fromEntries(header.map((key, index) => [key, values[index]]));
  });
}

function splitCsvLine(line) {
  const out = [];
  let field = '';
  let inQuote = false;
  for (let i = 0; i < line.length; i++) {
    const char = line[i];
    if (char === '"') {
      if (inQuote && line[i + 1] === '"') {
        field += '"';
        i++;
      }
      else {
        inQuote = !inQuote;
      }
    }
    else if (char === ',' && !inQuote) {
      out.push(field);
      field = '';
    }
    else {
      field += char;
    }
  }
  out.push(field);
  return out;
}
