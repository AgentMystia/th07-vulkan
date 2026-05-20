import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { fileURLToPath } from 'node:url';

const doc = readFileSync(fileURLToPath(new URL('../docs/TH07_DIFF_FROM_TH06.md', import.meta.url)), 'utf8');
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

console.log(JSON.stringify({
  subsystems: statuses.length,
  same: statuses.filter((entry) => entry.status === 'Same').length,
  changed: statuses.filter((entry) => entry.status === 'Changed').length,
  new: statuses.filter((entry) => entry.status === 'New').length,
  pending: statuses.filter((entry) => entry.status === 'Pending').length
}, null, 2));
