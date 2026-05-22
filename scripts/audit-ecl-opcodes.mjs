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
const th07Exe = readFileSync(join(root, 'reference/original/th07.exe'));
const eclManagerHpp = readFileSync(join(root, 'src/EclManager.hpp'), 'utf8');
const eclManagerCpp = readFileSync(join(root, 'src/EclManager.cpp'), 'utf8');
const eclOpcodeTablesHpp = readFileSync(join(root, 'src/EclOpcodeTables.hpp'), 'utf8');
const eclOpcodeTablesCpp = readFileSync(join(root, 'src/EclOpcodeTables.cpp'), 'utf8');
const th07EclOpcodeJumpTableAddress = 0x00417838;
const th07EclOpcodeJumpTableFileOffset = 0x00016c38;
const th07EclOpcodeJumpTableEntryCount = 161;
const th07EclDispatchAdvanceAddress = 0x0041677b;

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
      const callMatch = line.slice(match.index).match(/^ins_(\d+)\s*\(([^)]*)\)/);
      assert.ok(callMatch, `Could not parse ECL call on ${file}:${lineIndex + 1}`);
      const usage = opcodeUsage.get(opcode) ?? { count: 0, stages: new Set(), firstUse: null, signatures: new Map() };
      usage.count++;
      usage.stages.add(stage);
      usage.firstUse ??= {
        file: basename(file),
        line: lineIndex + 1,
        text: line.trim(),
      };
      const signature = splitEclArguments(callMatch[2]).map(classifyEclArgument).join(';');
      const signatureUsage = usage.signatures.get(signature) ?? {
        count: 0,
        firstUse: `${basename(file)}:${lineIndex + 1}`,
        sample: callMatch[0],
      };
      signatureUsage.count++;
      usage.signatures.set(signature, signatureUsage);
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
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07DeclOpcodeCount'), declOpcodes.length,
  'C++ ECL declaration opcode count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07CurrentEclEnumOpcodeCount'), enumOpcodes.size,
  'C++ ECL enum opcode count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07CurrentEclHandledOpcodeCount'), handledOpcodeValues.size,
  'C++ ECL handled opcode count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07MissingEnumOpcodeCount'), missingEnumOpcodes.length,
  'C++ ECL missing-enum opcode count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07MissingDispatchOpcodeCount'), missingDispatchOpcodes.length,
  'C++ ECL missing-dispatch opcode count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07MissingNonNopDispatchOpcodeCount'),
  missingNonNopDispatchOpcodes.length,
  'C++ ECL non-NOP missing-dispatch opcode count drifted');
assert.deepEqual(parseCppIntegerArray(eclOpcodeTablesCpp, 'kTh07MissingEnumOpcodes'), missingEnumOpcodes,
  'C++ ECL missing-enum opcode table drifted');
assert.deepEqual(parseCppIntegerArray(eclOpcodeTablesCpp, 'kTh07MissingDispatchOpcodes'), missingDispatchOpcodes,
  'C++ ECL missing-dispatch opcode table drifted');

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
const dispatchCasesByOpcode = new Map([...dispatchCases.entries()]
  .map(([caseValue, dispatchCase]) => [caseValue + 1, dispatchCase]));
const jumpTableTargets = parseJumpTableTargets(th07Exe, th07EclOpcodeJumpTableFileOffset,
  th07EclOpcodeJumpTableAddress, th07EclOpcodeJumpTableEntryCount);
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcodeJumpTableAddress'),
  th07EclOpcodeJumpTableAddress, 'C++ ECL jump-table address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclDispatchAdvanceAddress'),
  th07EclDispatchAdvanceAddress, 'C++ ECL common advance address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcodeJumpTableEntryCount'),
  th07EclOpcodeJumpTableEntryCount, 'C++ ECL jump-table entry count drifted');
const highDispatchCases = [...dispatchCasesByOpcode.keys()]
  .filter((opcode) => opcode >= 136)
  .sort((a, b) => a - b);
const expectedHighDecompilerDispatchOpcodes = [
  136, 137, 138, 139, 140, 142, 143, 144, 145, 146,
  147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
  157, 158, 159, 160, 161
];
assert.deepEqual(highDispatchCases, expectedHighDecompilerDispatchOpcodes,
  'TH07 ECL high decompiler cases changed; update config/ecl_opcode_dispatch.csv');
const missingEnumOpcodesWithJumpTarget =
  missingEnumOpcodes.filter((opcode) => jumpTableTargets.has(opcode));
const missingEnumOpcodesWithoutJumpTarget =
  missingEnumOpcodes.filter((opcode) => !jumpTableTargets.has(opcode));
const missingEnumOpcodesWithDecompilerCase =
  missingEnumOpcodes.filter((opcode) => dispatchCasesByOpcode.has(opcode));
const missingEnumOpcodesWithoutDecompilerCase =
  missingEnumOpcodes.filter((opcode) => !dispatchCasesByOpcode.has(opcode));
assert.deepEqual(missingEnumOpcodesWithoutJumpTarget, [],
  'TH07 ECL missing-enum opcode jump-table blockers changed');
assert.deepEqual(missingEnumOpcodesWithoutDecompilerCase, [141],
  'TH07 ECL missing-enum opcode decompiler no-op cases changed');

const dispatchRows = parseCsv(eclOpcodeDispatchManifest);
assert.deepEqual(Object.keys(dispatchRows[0] ?? {}), [
  'opcode',
  'decl_count',
  'first_use',
  'dispatch_function',
  'jump_table_entry',
  'target_address',
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
  const jumpTarget = jumpTableTargets.get(opcode);
  assert.ok(jumpTarget, `ECL opcode ${opcode} has no jump-table entry`);
  assert.equal(row.jump_table_entry, formatHex(jumpTarget.entryAddress),
    `ECL opcode ${opcode} jump-table entry changed`);
  assert.equal(row.target_address, formatHex(jumpTarget.targetAddress),
    `ECL opcode ${opcode} jump-table target changed`);
  const dispatchCase = dispatchCasesByOpcode.get(opcode);
  if (dispatchCase === undefined) {
    assert.equal(opcode, 141, `Unexpected high ECL opcode ${opcode} without decompiler case`);
    assert.equal(row.case_label, '', `ECL opcode ${opcode} should not have a decompiler case label`);
    assert.equal(row.case_line, '', `ECL opcode ${opcode} should not have a decompiler case line`);
    assert.equal(row.status, 'source-derived', `ECL opcode ${opcode} no-op status changed`);
    assert.equal(row.next_evidence, 'treat-as-no-op-until-runtime-naming-is-proven',
      `ECL opcode ${opcode} no-op next evidence step changed`);
    assert.equal(formatHex(jumpTarget.targetAddress), formatHex(th07EclDispatchAdvanceAddress),
      `ECL opcode ${opcode} should target the common instruction-advance path`);
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
const highOpcodeHelperCallContractOpcodes = new Set([138, 143, 146, 151, 154, 155, 159, 160]);
assert.deepEqual(Object.keys(highCaseRows[0] ?? {}), [
  'opcode',
  'jump_table_entry',
  'target_address',
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
  const jumpTarget = jumpTableTargets.get(opcode);
  const dispatchCase = dispatchCasesByOpcode.get(opcode);
  const usage = opcodeUsage.get(opcode);
  assert.ok(usage, `ECL high opcode ${opcode} has no declaration usage`);
  assert.ok(jumpTarget, `ECL high opcode ${opcode} has no jump-table entry`);
  assert.equal(row.jump_table_entry, formatHex(jumpTarget.entryAddress),
    `ECL high opcode ${opcode} jump-table entry changed`);
  assert.equal(row.target_address, formatHex(jumpTarget.targetAddress),
    `ECL high opcode ${opcode} jump-table target changed`);
  const firstUse = `${usage.firstUse.file}:${usage.firstUse.line}`;
  const cppCaseLabel = row.case_label === '' ? '0' : row.case_label;
  const cppLineRange = parseLineRange(row.evidence_lines);
  assert.notEqual(row.effect_surface, '', `ECL high opcode ${opcode} needs an effect surface or blocker note`);
  assert.match(eclOpcodeTablesCpp,
    new RegExp(`\\{${opcode},\\s+${usage.count},\\s+"${escapeRegex(firstUse)}",\\s+${escapeRegex(cppCaseLabel)},\\s+${escapeRegex(row.jump_table_entry)},\\s+${escapeRegex(row.target_address)},\\s+${cppLineRange.start},\\s+${cppLineRange.end},`),
    `ECL high opcode ${opcode} is missing or stale in src/EclOpcodeTables.cpp`);
  if (dispatchCase === undefined) {
    assert.equal(opcode, 141, `Unexpected high ECL opcode ${opcode} without decompiler case`);
    assert.equal(row.case_label, '', `ECL high opcode ${opcode} should not have a decompiler case label`);
    assert.equal(row.status, 'source-derived', `ECL high opcode ${opcode} no-op status changed`);
    assert.equal(row.next_evidence, 'treat-as-no-op-until-runtime-naming-is-proven',
      `ECL high opcode ${opcode} no-op next evidence step changed`);
  }
  else {
    assert.equal(row.case_label, dispatchCase.label, `ECL high opcode ${opcode} case label changed`);
    const lineRange = parseLineRange(row.evidence_lines);
    assert.equal(lineRange.start, dispatchCase.line, `ECL high opcode ${opcode} evidence start line changed`);
    assert.ok(lineRange.end >= lineRange.start, `ECL high opcode ${opcode} evidence line range is invalid`);
    assert.equal(row.status, 'source-derived', `ECL high opcode ${opcode} case status changed`);
    const nextEvidence = highOpcodeHelperCallContractOpcodes.has(opcode)
      ? 'helper-call-contract-compiled-owner-lifetime-pending'
      : 'turn-case-surface-into-named-owner-fields';
    assert.equal(row.next_evidence, nextEvidence,
      `ECL high opcode ${opcode} next evidence step changed`);
  }
}
assert.equal(countMatches(eclOpcodeTablesCpp, /, Th07EclOpcodeStatus::SourceDerived,/g), 25,
  'src/EclOpcodeTables.cpp source-derived high opcode count changed');
assert.equal(countMatches(eclOpcodeTablesCpp, /, Th07EclOpcodeStatus::Blocked,/g), 0,
  'src/EclOpcodeTables.cpp blocked high opcode count changed');
const highOpcodeSignatureRows = summarizeHighOpcodeSignatures(opcodeUsage, missingEnumOpcodes);
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07HighEclOpcodeSignatureCount'),
  highOpcodeSignatureRows.length, 'C++ high ECL opcode signature count drifted');
assert.deepEqual(parseHighOpcodeSignatureRows(eclOpcodeTablesCpp), highOpcodeSignatureRows,
  'C++ high ECL opcode signature table drifted from declaration dumps');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclResolveFloatTargetFunctionAddress'),
  0x0040f3c0, 'C++ ECL resolved float-target function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclResolveFloatTargetJumpTableAddress'),
  0x0040f5d0, 'C++ ECL resolved float-target jump table address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclResolveFloatTargetClassTableAddress'),
  0x0040f668, 'C++ ECL resolved float-target class table address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclResolveFloatTargetContractCount'),
  37, 'C++ ECL resolved float-target contract count drifted');
assert.equal(countMatches(eclOpcodeTablesCpp, /Th07EclFloatTargetOwner::Enemy/g), 30,
  'C++ ECL resolved float-target enemy contract count drifted');
assert.equal(countMatches(eclOpcodeTablesCpp, /Th07EclFloatTargetOwner::Global/g), 7,
  'C++ ECL resolved float-target global contract count drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2714, Th07EclFloatTargetOwner::Enemy, 0x070c, 0x0040f422, "0040f3c0:11-13"\}/,
  'C++ ECL resolved float-target local0 contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2725, Th07EclFloatTargetOwner::Global, 0x004be408, 0x0040f4e5, "0040f3c0:46-48"\}/,
  'C++ ECL resolved float-target global contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2759, Th07EclFloatTargetOwner::Enemy, 0x0740, 0x0040f510, "0040f3c0:121-122"\}/,
  'C++ ECL resolved float-target tail contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclIntReadResolverFunctionAddress'),
  0x0040e5b0, 'C++ ECL int read resolver function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclIntReadResolverJumpTableAddress'),
  0x0040eace, 'C++ ECL int read resolver jump table address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclIntReadResolverContractCount'),
  70, 'C++ ECL int read resolver contract count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclFloatReadResolverFunctionAddress'),
  0x0040edf0, 'C++ ECL float read resolver function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclFloatReadResolverJumpTableAddress'),
  0x0040f28a, 'C++ ECL float read resolver jump table address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclFloatReadResolverContractCount'),
  74, 'C++ ECL float read resolver contract count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclReadResolverCaseBase'),
  0x2710, 'C++ ECL read resolver case base drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclReadResolverMaxRelativeId'),
  0x49, 'C++ ECL read resolver max relative id drifted');
assert.equal(countMatches(eclOpcodeTablesCpp, /Th07EclReadSourceKind::/g), 144,
  'C++ ECL int/float read resolver contract count drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2710, Th07EclReadSourceKind::EnemyI32, 0x06fc, kTh07EclReadResolverNoField,[\s\S]*?"0040e5b0:10-12"\}/,
  'C++ ECL int read local0 contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x272a, Th07EclReadSourceKind::DistanceToGlobals, 0x2b0c, 0x004be408,[\s\S]*?kTh07EclReadResolverDistanceFunctionAddress, "0040e5b0:89-92"\}/,
  'C++ ECL int read distance contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2748, Th07EclReadSourceKind::RandomIntModuloEnemyI32, 0x0744, 0x0748,[\s\S]*?kTh07EclReadResolverRandomIntFunctionAddress, "0040e5b0:180-190"\}/,
  'C++ ECL int read random range contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x2748, Th07EclReadSourceKind::RandomFloatScaledEnemyF32, 0x0754, 0x0758,[\s\S]*?kTh07EclReadResolverRandomFloatFunctionAddress, "0040edf0:182-186"\}/,
  'C++ ECL float read random enemy range contract drifted');
assert.match(eclOpcodeTablesCpp,
  /\{0x274c, Th07EclReadSourceKind::RandomFloatScaledGlobalF32, 0x00498a60, 0x00498a64,[\s\S]*?kTh07EclReadResolverRandomFloatFunctionAddress, "0040edf0:196-199"\}/,
  'C++ ECL float read random global range contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperFunctionAddress'),
  0x00424740, 'C++ ECL clear helper function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperObjectBaseOffset'),
  0x366628, 'C++ ECL clear helper object base offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperObjectCount'),
  0x40, 'C++ ECL clear helper object count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperObjectStrideBytes'),
  0x04ec, 'C++ ECL clear helper object stride drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperManagerEffectKindOffset'),
  0x37a160, 'C++ ECL clear helper manager effect-kind offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclClearHelperFinalCooldownOffset'),
  0x37a12c, 'C++ ECL clear helper final cooldown offset drifted');
assert.match(eclOpcodeTablesCpp,
  /kTh07EclClearHelperFunctionAddress,[\s\S]*?kTh07EclClearHelperObjectBaseOffset,[\s\S]*?kTh07EclClearHelperEffectSpawnFunctionAddress,[\s\S]*?kTh07EclClearHelperLineDirectionFunctionAddress,[\s\S]*?"00424740:20-72"/,
  'C++ ECL clear helper contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclItemClearRadiusHelperFunctionAddress'),
  0x00424c00, 'C++ ECL item-clear helper function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclItemClearRadiusItemArrayAddress'),
  0x0063b218, 'C++ ECL item-clear helper item array address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclItemClearRadiusItemCount'),
  0x400, 'C++ ECL item-clear helper item count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclItemClearRadiusItemStrideBytes'),
  0x0d68, 'C++ ECL item-clear helper item stride drifted');
assert.match(eclOpcodeTablesCpp,
  /kTh07EclItemClearRadiusHelperFunctionAddress,[\s\S]*?0x00424cf6,[\s\S]*?kTh07EclItemClearRadiusEffectSpawnFunctionAddress,[\s\S]*?"00424c00:11-25"/,
  'C++ ECL item-clear helper contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectSpawnFunctionAddress'),
  0x004326f0, 'C++ ECL visual-effect spawn function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectManagerAddress'),
  0x00575c70, 'C++ ECL visual-effect manager address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectSlotCount'),
  0x44c, 'C++ ECL visual-effect slot count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectSlotSizeBytes'),
  0x0288, 'C++ ECL visual-effect slot size drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectNextIndexOffset'),
  0x0ae2e8, 'C++ ECL visual-effect next-index offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclVisualEffectAnmScriptBase'),
  0x02c4, 'C++ ECL visual-effect ANM script base drifted');
assert.match(eclOpcodeTablesCpp,
  /kTh07EclVisualEffectSpawnFunctionAddress,[\s\S]*?kTh07EclVisualEffectSlotCount,[\s\S]*?kTh07EclVisualEffectNextIndexOffset,[\s\S]*?kTh07EclVisualEffectModeGatePointerAddress,[\s\S]*?kTh07EclVisualEffectSetScriptFunctionAddress,[\s\S]*?"004326f0:15-75"/,
  'C++ ECL visual-effect spawn contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionOpcode'),
  151, 'C++ ECL projection helper opcode drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionHelperContractCount'),
  2, 'C++ ECL projection helper contract count drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionSinWrapperFunctionAddress'),
  0x0048bb40, 'C++ ECL projection sin wrapper drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionSinCoreFunctionAddress'),
  0x0048bb5d, 'C++ ECL projection sin core drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionCosWrapperFunctionAddress'),
  0x0048bbf0, 'C++ ECL projection cos wrapper drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionCosCoreFunctionAddress'),
  0x0048bc0d, 'C++ ECL projection cos core drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionFiniteCheckFunctionAddress'),
  0x00483d38, 'C++ ECL projection finite-check helper drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionRangeReductionConstantAddress'),
  0x004911ba, 'C++ ECL projection range-reduction constant drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclProjectionMathErrorFlagAddress'),
  0x0049f77c, 'C++ ECL projection math-error flag drifted');
assert.equal(countMatches(eclOpcodeTablesCpp, /Th07EclProjectionHelperKind::/g),
  2, 'C++ ECL projection helper contract rows drifted');
assert.match(eclOpcodeTablesCpp,
  /Th07EclProjectionHelperKind::Sin,[\s\S]*?kTh07EclProjectionSinWrapperFunctionAddress,[\s\S]*?kTh07EclProjectionSinCoreFunctionAddress,[\s\S]*?kTh07EclProjectionSinOpcodeCallInstructionAddress,[\s\S]*?kTh07EclProjectionSinPrimaryInstructionAddress,[\s\S]*?"0048bb40:8-10",[\s\S]*?"0048bb5d:15-40",[\s\S]*?"00410520:3083-3096"/,
  'C++ ECL projection sin contract drifted');
assert.match(eclOpcodeTablesCpp,
  /Th07EclProjectionHelperKind::Cos,[\s\S]*?kTh07EclProjectionCosWrapperFunctionAddress,[\s\S]*?kTh07EclProjectionCosCoreFunctionAddress,[\s\S]*?kTh07EclProjectionCosOpcodeCallInstructionAddress,[\s\S]*?kTh07EclProjectionCosPrimaryInstructionAddress,[\s\S]*?"0048bbf0:8-10",[\s\S]*?"0048bc0d:15-40",[\s\S]*?"00410520:3097-3110"/,
  'C++ ECL projection cos contract drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperFunctionAddress'),
  0x00455170, 'C++ ECL opcode 138 record helper function address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperCallInstructionAddress'),
  0x004160c5, 'C++ ECL opcode 138 record helper call address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperCallThisPointerAddress'),
  0x004b9e44, 'C++ ECL opcode 138 record helper this-pointer address drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperOutputBaseOffset'),
  0x39f8, 'C++ ECL opcode 138 record output base offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperFlagFieldOffset'),
  0x4f30, 'C++ ECL opcode 138 record flag field offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperSegmentNumeratorOffset'),
  0x4f32, 'C++ ECL opcode 138 record numerator offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperSegmentDenominatorOffset'),
  0x4f36, 'C++ ECL opcode 138 record denominator offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperMetadataPointerOffset'),
  0x01e4, 'C++ ECL opcode 138 record metadata pointer offset drifted');
assert.equal(parseCppIntegerConstant(eclOpcodeTablesHpp, 'kTh07EclOpcode138RecordHelperRecordStrideBytes'),
  0x0038, 'C++ ECL opcode 138 record stride drifted');
assert.match(eclOpcodeTablesCpp,
  /kTh07EclOpcode138RecordHelperFunctionAddress,[\s\S]*?kTh07EclOpcode138RecordHelperCallInstructionAddress,[\s\S]*?kTh07EclOpcode138RecordHelperOutputBaseOffset,[\s\S]*?kTh07EclOpcode138RecordHelperRecordScaleBits,[\s\S]*?"00455170:15-48",[\s\S]*?"00410520:2833-2860"/,
  'C++ ECL opcode 138 record helper contract drifted');

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
  th07EclOpcodeJumpTableAddress: formatHex(th07EclOpcodeJumpTableAddress),
  th07EclDispatchAdvanceAddress: formatHex(th07EclDispatchAdvanceAddress),
  highDecompilerDispatchOpcodes: highDispatchCases,
  missingEnumOpcodesWithJumpTarget,
  missingEnumOpcodesWithoutJumpTarget,
  missingEnumOpcodesWithDecompilerCase,
  missingEnumOpcodesWithoutDecompilerCase,
  missingEnumOpcodes,
  missingDispatchOpcodes,
  missingNonNopDispatchOpcodes,
  declaredButUnusedEnumOpcodes: declaredButUnusedEnumOpcodes.slice(0, 32),
  missingEnumOpcodeFirstUses: summarizeOpcodeFirstUses(opcodeUsage, missingEnumOpcodes),
  highOpcodeUsage: summarizeOpcodeRange(opcodeUsage, 136),
  highOpcodeSignatureRows,
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

function parseJumpTableTargets(buffer, fileOffset, tableAddress, entryCount) {
  const entries = new Map();
  for (let opcode = 1; opcode <= entryCount; opcode++) {
    const entryAddress = tableAddress + (opcode - 1) * 4;
    const targetAddress = buffer.readUInt32LE(fileOffset + (opcode - 1) * 4);
    entries.set(opcode, { entryAddress, targetAddress });
  }
  return entries;
}

function formatHex(value) {
  return `0x${value.toString(16).padStart(8, '0')}`;
}

function parseLineRange(value) {
  const match = value.match(/^(\d+)-(\d+)$/);
  assert.ok(match, `Unsupported line range: ${value}`);
  return {
    start: Number(match[1]),
    end: Number(match[2]),
  };
}

function parseCppIntegerConstant(text, name) {
  const match = text.match(new RegExp(`\\b${name}\\s*=\\s*(0x[0-9a-fA-F]+|\\d+)`));
  assert.ok(match, `Missing C++ ECL constant ${name}`);
  return match[1].startsWith('0x') || match[1].startsWith('0X')
    ? Number.parseInt(match[1], 16)
    : Number(match[1]);
}

function parseCppIntegerArray(text, name) {
  const match = text.match(new RegExp(`${name}\\s*=\\s*\\{\\{([\\s\\S]*?)\\}\\};`));
  assert.ok(match, `Missing C++ ECL array ${name}`);
  return match[1].split(',')
    .map((value) => value.trim())
    .filter((value) => value.length > 0)
    .map((value) => Number(value));
}

function splitEclArguments(args) {
  if (args.trim().length === 0) {
    return [];
  }
  const result = [];
  let current = '';
  let referenceDepth = 0;
  for (const char of args) {
    if (char === '[') {
      referenceDepth++;
    } else if (char === ']') {
      referenceDepth--;
    }
    if (char === ',' && referenceDepth === 0) {
      result.push(current.trim());
      current = '';
    } else {
      current += char;
    }
  }
  if (current.trim().length > 0) {
    result.push(current.trim());
  }
  return result;
}

function classifyEclArgument(argument) {
  const reference = argument.startsWith('[') && argument.endsWith(']');
  const value = reference ? argument.slice(1, -1) : argument;
  const isFloat = /\b\d+\.\d+f\b|f\b|\./.test(value);
  if (reference && isFloat) {
    return 'FloatReference';
  }
  if (reference) {
    return 'IntReference';
  }
  if (isFloat) {
    return 'Float';
  }
  return 'Int';
}

function summarizeHighOpcodeSignatures(opcodeUsage, opcodes) {
  const rows = [];
  for (const opcode of opcodes) {
    const usage = opcodeUsage.get(opcode);
    assert.ok(usage, `Missing usage for high ECL opcode ${opcode}`);
    for (const [signature, signatureUsage] of usage.signatures.entries()) {
      rows.push({
        opcode,
        count: signatureUsage.count,
        operandKinds: signature.length === 0 ? [] : signature.split(';'),
        firstUse: signatureUsage.firstUse,
        sample: signatureUsage.sample,
      });
    }
  }
  return rows;
}

function parseHighOpcodeSignatureRows(text) {
  const block = extractCppArrayBlock(text, 'kTh07HighEclOpcodeSignatures');
  const rowPattern =
    /^\s*\{(\d+),\s*(\d+),\s*(\d+),\s*\{([^}]*)\},\s*"([^"]*)",\s*"([^"]*)"\},/gm;
  return [...block.matchAll(rowPattern)].map((match) => {
    const operandCount = Number(match[3]);
    const operandKinds = [...match[4].matchAll(/Th07EclOperandKind::(\w+)/g)]
      .map((kindMatch) => kindMatch[1])
      .slice(0, operandCount);
    assert.equal(operandKinds.length, operandCount, `ECL opcode ${match[1]} signature operand count mismatch`);
    return {
      opcode: Number(match[1]),
      count: Number(match[2]),
      operandKinds,
      firstUse: match[5],
      sample: match[6],
    };
  });
}

function extractCppArrayBlock(text, name) {
  const match = text.match(new RegExp(`${name}\\s*=\\s*\\{\\{([\\s\\S]*?)\\}\\};`));
  assert.ok(match, `Missing C++ array ${name}`);
  return match[1];
}

function countMatches(text, pattern) {
  return [...text.matchAll(pattern)].length;
}

function escapeRegex(value) {
  return String(value).replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
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
