import assert from 'node:assert/strict';
import { readFileSync, readdirSync } from 'node:fs';
import { basename, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));
const eclDir = join(root, 'reference/ecl');
const eclOpcodeGapManifest = readFileSync(join(root, 'config/ecl_opcode_gaps.csv'), 'utf8');
const eclOpcodeDispatchManifest = readFileSync(join(root, 'config/ecl_opcode_dispatch.csv'), 'utf8');
const eclHighOpcodeCaseManifest = readFileSync(join(root, 'config/ecl_high_opcode_cases.csv'), 'utf8');
const th07EclDispatchFunctionName = '00410520_FUN_00410520.c';
const th07EclDispatchFunction = readFileSync(
  join(root, 'reference/ghidra/decomp/functions', th07EclDispatchFunctionName),
  'utf8'
);
const eclManagerHpp = readFileSync(join(root, 'src/EclManager.hpp'), 'utf8');
const eclManagerCpp = readFileSync(join(root, 'src/EclManager.cpp'), 'utf8');

const declFiles = readdirSync(eclDir)
  .filter((file) => /^ecldata\d\.decl$/.test(file))
  .sort((a, b) => a.localeCompare(b));
assert.equal(declFiles.length, 8, 'Expected all eight TH07 ECL declaration dumps');

const opcodeUsage = new Map();
for (const file of declFiles) {
  const stage = Number(file.match(/\d+/)?.[0]);
  const text = readFileSync(join(eclDir, file), 'utf8');
  const lines = text.split(/\r?\n/);
  for (let lineIndex = 0; lineIndex < lines.length; lineIndex++) {
    const line = lines[lineIndex];
    for (const match of line.matchAll(/\bins_(\d+)\s*\(/g)) {
      const opcode = Number(match[1]);
      const usage = opcodeUsage.get(opcode) ?? { count: 0, stages: new Set(), firstUse: null };
      usage.count++;
      usage.stages.add(stage);
      usage.firstUse ??= {
        file: basename(file),
        line: lineIndex + 1,
        text: line.trim(),
      };
      opcodeUsage.set(opcode, usage);
    }
  }
}

const declOpcodes = [...opcodeUsage.keys()].sort((a, b) => a - b);
assert.ok(declOpcodes.length > 100, 'Expected substantive ECL opcode coverage from declaration dumps');
assert.equal(declOpcodes[0], 0, 'TH07 declaration dumps should include opcode 0');
assert.ok(declOpcodes.includes(154), 'TH07 declaration dumps should include opcode 154');
assert.equal(declOpcodes.at(-1), 161, 'TH07 declaration dumps should include high opcode 161');

const enumOpcodes = parseEnumOpcodes(eclManagerHpp);
assert.ok(enumOpcodes.has('ECL_OPCODE_NOP'), 'ECL opcode enum missing NOP');
assert.ok(enumOpcodes.has('ECL_OPCODE_SPELLCARDFLAGTIMEOUT'), 'ECL opcode enum missing last known TH06-style opcode');

const enumOpcodeValues = new Map([...enumOpcodes.entries()].map(([name, value]) => [value, name]));
const handledOpcodeNames = new Set([...eclManagerCpp.matchAll(/case\s+(ECL_OPCODE_[A-Z0-9_]+)\s*:/g)]
  .map((match) => match[1]));
assert.ok(handledOpcodeNames.has('ECL_OPCODE_CALL'), 'ECL dispatch should still handle CALL');

const handledOpcodeValues = new Set(
  [...handledOpcodeNames]
    .map((name) => enumOpcodes.get(name))
    .filter((value) => value !== undefined)
);

const missingEnumOpcodes = declOpcodes.filter((opcode) => !enumOpcodeValues.has(opcode));
const missingDispatchOpcodes = declOpcodes.filter((opcode) => !handledOpcodeValues.has(opcode));
const missingNonNopDispatchOpcodes = missingDispatchOpcodes.filter((opcode) => opcode !== 0);
const declaredButUnusedEnumOpcodes = [...enumOpcodeValues.keys()]
  .filter((opcode) => !opcodeUsage.has(opcode))
  .sort((a, b) => a - b);
const expectedUnmappedTh07HighOpcodes = [
  136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146,
  148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161
];
assert.deepEqual(missingEnumOpcodes, expectedUnmappedTh07HighOpcodes,
  'Current ECL enum gap changed; update the restoration ledger and audit expectations');

const gapRows = parseCsv(eclOpcodeGapManifest);
assert.deepEqual(Object.keys(gapRows[0] ?? {}), [
  'opcode',
  'count',
  'stages',
  'first_use',
  'status',
  'next_evidence',
], 'ECL opcode gap manifest header changed unexpectedly');
assert.deepEqual(gapRows.map((row) => Number(row.opcode)), missingEnumOpcodes,
  'ECL opcode gap manifest no longer matches the missing enum opcodes');
for (const row of gapRows) {
  const opcode = Number(row.opcode);
  const usage = opcodeUsage.get(opcode);
  assert.ok(usage, `ECL opcode gap manifest references unused opcode ${opcode}`);
  assert.equal(Number(row.count), usage.count, `ECL opcode ${opcode} count changed`);
  assert.equal(row.stages, [...usage.stages].sort((a, b) => a - b).join(';'),
    `ECL opcode ${opcode} stage set changed`);
  assert.equal(row.first_use, `${usage.firstUse.file}:${usage.firstUse.line}`,
    `ECL opcode ${opcode} first-use site changed`);
  assert.equal(row.status, 'metadata-only', `ECL opcode ${opcode} status should remain metadata-only`);
  assert.equal(row.next_evidence, 'map-th07-dispatch',
    `ECL opcode ${opcode} next evidence step changed`);
}

const dispatchCases = parseSwitchCases(th07EclDispatchFunction);
const highDispatchCases = [...dispatchCases.keys()]
  .filter((opcode) => opcode >= 136)
  .sort((a, b) => a - b);
const expectedMappedHighDispatchOpcodes = [
  136, 137, 138, 139, 141, 142, 143, 144, 145, 146,
  147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
  157, 158, 159, 160
];
assert.deepEqual(highDispatchCases, expectedMappedHighDispatchOpcodes,
  'TH07 ECL high dispatch cases changed; update config/ecl_opcode_dispatch.csv');
const missingEnumOpcodesWithDispatchCase =
  missingEnumOpcodes.filter((opcode) => dispatchCases.has(opcode));
const missingEnumOpcodesWithoutDispatchCase =
  missingEnumOpcodes.filter((opcode) => !dispatchCases.has(opcode));
assert.deepEqual(missingEnumOpcodesWithoutDispatchCase, [140, 161],
  'TH07 ECL missing-enum opcode dispatch blockers changed');

const dispatchRows = parseCsv(eclOpcodeDispatchManifest);
assert.deepEqual(Object.keys(dispatchRows[0] ?? {}), [
  'opcode',
  'decl_count',
  'first_use',
  'dispatch_function',
  'case_label',
  'case_line',
  'status',
  'next_evidence',
], 'ECL opcode dispatch manifest header changed unexpectedly');
assert.deepEqual(dispatchRows.map((row) => Number(row.opcode)), missingEnumOpcodes,
  'ECL opcode dispatch manifest no longer matches the missing enum opcodes');
for (const row of dispatchRows) {
  const opcode = Number(row.opcode);
  const usage = opcodeUsage.get(opcode);
  assert.ok(usage, `ECL opcode dispatch manifest references unused opcode ${opcode}`);
  assert.equal(Number(row.decl_count), usage.count, `ECL opcode ${opcode} dispatch count changed`);
  assert.equal(row.first_use, `${usage.firstUse.file}:${usage.firstUse.line}`,
    `ECL opcode ${opcode} dispatch first-use site changed`);
  assert.equal(row.dispatch_function, th07EclDispatchFunctionName,
    `ECL opcode ${opcode} dispatch function changed`);
  const dispatchCase = dispatchCases.get(opcode);
  if (dispatchCase === undefined) {
    assert.equal(row.case_label, '', `ECL opcode ${opcode} should not have a mapped case label yet`);
    assert.equal(row.case_line, '', `ECL opcode ${opcode} should not have a mapped case line yet`);
    assert.equal(row.status, 'blocked', `ECL opcode ${opcode} should remain blocked`);
    assert.equal(row.next_evidence, 'inspect-binary-disassembly-or-loader-translator',
      `ECL opcode ${opcode} blocked next evidence step changed`);
  }
  else {
    assert.equal(row.case_label, dispatchCase.label, `ECL opcode ${opcode} dispatch case label changed`);
    assert.equal(Number(row.case_line), dispatchCase.line, `ECL opcode ${opcode} dispatch case line changed`);
    assert.equal(row.status, 'source-derived', `ECL opcode ${opcode} dispatch status changed`);
    assert.equal(row.next_evidence, 'map-case-semantics-before-runtime-wiring',
      `ECL opcode ${opcode} dispatch next evidence step changed`);
  }
}

const highCaseRows = parseCsv(eclHighOpcodeCaseManifest);
assert.deepEqual(Object.keys(highCaseRows[0] ?? {}), [
  'opcode',
  'case_label',
  'evidence_lines',
  'effect_surface',
  'status',
  'next_evidence',
], 'ECL high opcode case manifest header changed unexpectedly');
assert.deepEqual(highCaseRows.map((row) => Number(row.opcode)), missingEnumOpcodes,
  'ECL high opcode case manifest no longer matches the missing enum opcodes');
for (const row of highCaseRows) {
  const opcode = Number(row.opcode);
  const dispatchCase = dispatchCases.get(opcode);
  assert.notEqual(row.effect_surface, '', `ECL high opcode ${opcode} needs an effect surface or blocker note`);
  if (dispatchCase === undefined) {
    assert.equal(row.case_label, '', `ECL high opcode ${opcode} should not have a case label`);
    assert.equal(row.evidence_lines, '', `ECL high opcode ${opcode} should not have case evidence lines`);
    assert.equal(row.status, 'blocked', `ECL high opcode ${opcode} should remain blocked`);
    assert.equal(row.next_evidence, 'inspect-binary-disassembly-or-loader-translator',
      `ECL high opcode ${opcode} blocked next evidence step changed`);
  }
  else {
    assert.equal(row.case_label, dispatchCase.label, `ECL high opcode ${opcode} case label changed`);
    const lineRange = parseLineRange(row.evidence_lines);
    assert.equal(lineRange.start, dispatchCase.line, `ECL high opcode ${opcode} evidence start line changed`);
    assert.ok(lineRange.end >= lineRange.start, `ECL high opcode ${opcode} evidence line range is invalid`);
    assert.equal(row.status, 'source-derived', `ECL high opcode ${opcode} case status changed`);
    assert.equal(row.next_evidence, 'turn-case-surface-into-named-owner-fields',
      `ECL high opcode ${opcode} next evidence step changed`);
  }
}

console.log(JSON.stringify({
  declFiles: declFiles.map((file) => basename(file)),
  declOpcodeCount: declOpcodes.length,
  declOpcodeMin: declOpcodes[0],
  declOpcodeMax: declOpcodes.at(-1),
  currentEnumOpcodeCount: enumOpcodes.size,
  currentHandledOpcodeCount: handledOpcodeValues.size,
  missingEnumOpcodeCount: missingEnumOpcodes.length,
  missingDispatchOpcodeCount: missingDispatchOpcodes.length,
  missingNonNopDispatchOpcodeCount: missingNonNopDispatchOpcodes.length,
  gapManifestRows: gapRows.length,
  dispatchManifestRows: dispatchRows.length,
  highCaseSurfaceRows: highCaseRows.length,
  th07DecompilerDispatchFunction: th07EclDispatchFunctionName,
  highDecompilerDispatchCases: highDispatchCases,
  missingEnumOpcodesWithDispatchCase,
  missingEnumOpcodesWithoutDispatchCase,
  missingEnumOpcodes,
  missingDispatchOpcodes,
  missingNonNopDispatchOpcodes,
  declaredButUnusedEnumOpcodes: declaredButUnusedEnumOpcodes.slice(0, 32),
  missingEnumOpcodeFirstUses: summarizeOpcodeFirstUses(opcodeUsage, missingEnumOpcodes),
  highOpcodeUsage: summarizeOpcodeRange(opcodeUsage, 136),
}, null, 2));

function parseEnumOpcodes(text) {
  const match = text.match(/enum\s+EclRawInstrOpcode\s*\{([\s\S]*?)\n\};/);
  assert.ok(match, 'Could not locate EclRawInstrOpcode enum');

  const opcodes = new Map();
  let nextValue = 0;
  for (const rawLine of match[1].split('\n')) {
    const line = rawLine.replace(/\/\/.*$/, '').trim();
    if (line === '') {
      continue;
    }

    const item = line.replace(/,$/, '').trim();
    if (item === '') {
      continue;
    }

    const valueMatch = item.match(/^(ECL_OPCODE_[A-Z0-9_]+)(?:\s*=\s*(-?\d+))?$/);
    assert.ok(valueMatch, `Unsupported ECL opcode enum line: ${rawLine}`);
    const value = valueMatch[2] === undefined ? nextValue : Number(valueMatch[2]);
    opcodes.set(valueMatch[1], value);
    nextValue = value + 1;
  }
  return opcodes;
}

function summarizeOpcodeRange(usage, firstOpcode) {
  return [...usage.entries()]
    .filter(([opcode]) => opcode >= firstOpcode)
    .sort(([a], [b]) => a - b)
    .map(([opcode, value]) => ({
      opcode,
      count: value.count,
      stages: [...value.stages].sort((a, b) => a - b),
      firstUse: value.firstUse,
    }));
}

function summarizeOpcodeFirstUses(usage, opcodes) {
  return opcodes.map((opcode) => ({
    opcode,
    firstUse: usage.get(opcode)?.firstUse,
  }));
}

function parseSwitchCases(text) {
  const cases = new Map();
  const lines = text.split(/\r?\n/);
  for (let index = 0; index < lines.length; index++) {
    const match = lines[index].match(/^\s*case\s+(0x[0-9a-f]+|\d+):/i);
    if (match === null) {
      continue;
    }
    const value = match[1].startsWith('0x') || match[1].startsWith('0X')
      ? Number.parseInt(match[1], 16)
      : Number(match[1]);
    cases.set(value, {
      label: match[1].toLowerCase(),
      line: index + 1,
    });
  }
  return cases;
}

function parseLineRange(value) {
  const match = value.match(/^(\d+)-(\d+)$/);
  assert.ok(match, `Unsupported line range: ${value}`);
  return {
    start: Number(match[1]),
    end: Number(match[2]),
  };
}

function parseCsv(text) {
  const lines = text.trim().split(/\r?\n/);
  const header = splitCsvLine(lines.shift() ?? '');
  return lines.filter((line) => line.trim() !== '').map((line) => {
    const values = splitCsvLine(line);
    assert.equal(values.length, header.length,
      `CSV row has ${values.length} fields instead of ${header.length}: ${line}`);
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
