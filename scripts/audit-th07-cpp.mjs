import { existsSync, readFileSync, readdirSync, statSync } from 'node:fs';
import { join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));
const src = join(root, 'src');
const runtimeManifestPath = join(root, 'config/runtime_replacement_manifest.csv');
const playerFunctionManifestPath = join(root, 'config/player_function_manifest.csv');
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
  'Effect.hpp',
  'EffectManager.hpp',
  'EffectManager.cpp',
  'Th07ResourceManifest.hpp',
  'Th07ResourceManifest.cpp',
  'Th07ResourceFormatTables.hpp',
  'Th07ResourceFormatTables.cpp',
  'Th07EclFile.hpp',
  'Th07EclFile.cpp',
  'Th07EclManagerTables.hpp',
  'Th07AnmFile.hpp',
  'Th07AnmFile.cpp',
  'Th07AnmManagerLoadTables.hpp',
  'Th07MsgFile.hpp',
  'Th07MsgFile.cpp',
  'Th07GuiMsgLoaderTables.hpp',
  'Th07StdFile.hpp',
  'Th07StdFile.cpp',
  'Th07StageLoaderTables.hpp',
  'Rng.hpp',
  'Rng.cpp',
  'Th07Timer.hpp',
  'Th07ArchiveReader.hpp',
  'Th07ArchiveReader.cpp',
  'Th07FileSystemRuntime.hpp',
  'Th07FileSystemRuntime.cpp',
  'Th07FileSystemTables.hpp',
  'Th07ScoreTables.hpp',
  'Th07ScoreTables.cpp',
  'Th07BulletTables.hpp',
  'Th07PlayerShotTables.hpp',
  'Th07EffectTables.hpp',
  'Th07EffectTables.cpp',
  'ShtManager.hpp',
  'ShtManager.cpp',
  'ShtFile.hpp',
  'ShtFile.cpp',
  'EclOpcodeTables.hpp',
  'EclOpcodeTables.cpp',
  'PlayerBombTables.hpp',
  'PlayerBombTables.cpp',
  'PlayerLayout.hpp',
  'CherrySystem.hpp',
  'CherrySystem.cpp',
  'BorderSystem.hpp',
  'BorderSystem.cpp',
  'ClearBonusSystem.hpp',
  'ClearBonusSystem.cpp',
  'GuiLayout.hpp',
  'compat/D3D8.h',
  'compat/Windows.h',
  'compat/d3d8.h',
  'compat/d3d8types.h',
  'compat/d3dx8math.h',
  'compat/dinput.h',
  'compat/dsound.h',
  'compat/i18n.hpp',
  'compat/mmreg.h',
  'compat/mmsystem.h',
  'compat/windows.h',
];

const problems = [];
let runtimeReplacementManifestRows = 0;
let playerFunctionManifestRows = 0;
for (const file of requiredFiles) {
  if (!existsSync(join(src, file))) problems.push(`missing reconstructed file: ${file}`);
}

if (!existsSync(join(root, 'scripts/objdiff-player.mjs'))) {
  problems.push('missing Player objdiff readiness script: scripts/objdiff-player.mjs');
}
const objdiffPlayerScript = existsSync(join(root, 'scripts/objdiff-player.mjs'))
  ? readFileSync(join(root, 'scripts/objdiff-player.mjs'), 'utf8')
  : '';
if (!existsSync(join(src, 'PROVENANCE.md'))) problems.push('missing reconstructed provenance file');

const allCppText = allFiles(src)
  .filter((file) => /\.(cpp|hpp|rc|tpl)$/.test(file))
  .map((file) => readFileSync(file, 'utf8'))
  .join('\n');

mustContain(allCppText, 'namespace th07', 'copied source namespace was not rewritten to th07');
mustNotContain(allCppText, 'namespace th06', 'copied source still contains namespace th06');

const packageJson = readFileSync(join(root, 'package.json'), 'utf8');
mustContain(packageJson, '"objdiff-player": "node scripts/objdiff-player.mjs"',
  'Player objdiff readiness script should be exposed through npm');
for (const value of [
  'compile current ${sourceFile} objdiff object',
  'player-current-renamed.o',
  'llvm-objcopy',
  'currentSymbol',
  'playerFileFunctions',
  'unmappedFunctions',
  'FUN_00441fb0',
  'FUN_0043e0a0',
  'FUN_00440940',
  'FUN_00441800',
  'FUN_004418b0',
  'FUN_0043e4e0',
  '-DTH07_PLAYER_OBJDIFF',
  '-Isrc/compat',
]) {
  mustContain(objdiffPlayerScript, value, `Player objdiff source-object comparison support missing ${value}`);
}

const agentsMd = readFileSync(join(root, 'AGENTS.md'), 'utf8');
for (const value of [
  'behavior-level parity',
  'Binary matching is best effort',
  'objdiff-close',
  'objdiff-high',
  'binary-exact',
  'The current project phase is not the SDL3/Vulkan port',
  'src/compat/` is for diff-only compilation',
]) {
  mustContain(agentsMd, value, `AGENTS.md convergence policy missing ${value}`);
}

const gmH = read('GameManager.hpp');
mustContain(gmH, '#define PSCR_NUM_CHARS_SHOTTYPES 6', 'TH07 score table should cover 6 shot types');
mustContain(gmH, '#define PSCR_NUM_STAGES 8', 'TH07 score table should cover 8 stage routes');
mustContain(gmH, '#define CLRD_NUM_CHARACTERS 6', 'TH07 clear table should cover 6 shot types');
mustContain(gmH, '#define CATK_NUM_CAPTURES 141', 'TH07 spell/capture table should cover 141 names');
mustContain(gmH, 'PHANTASM_STAGE', 'TH07 stage enum should include Phantasm');

const playerH = read('Player.hpp');
mustContain(playerH, 'CHARA_SAKUYA', 'TH07 player enum should include Sakuya');
mustContain(playerH, 'Th07PlayerOriginalAnmVmSlot playerSprite', 'TH07 Player prefix should use original-size VM slots');
mustContain(playerH, 'offsetof(Player, positionCenter) == kPlayerPositionOffset',
  'TH07 Player position offset assert missing');
mustContain(playerH, 'offsetof(Player, movementDelta) == kPlayerMovementDeltaOffset',
  'TH07 Player movement delta offset assert missing');
mustContain(playerH, 'offsetof(Player, th07BombCollisionSlots) == kPlayerBombCollisionSlotArrayOffset',
  'TH07 Player bomb collision slot offset assert missing');
mustContain(playerH, 'offsetof(Player, th07BombCommonEffectXBits) == kPlayerBombCommonEffectXOffset',
  'TH07 Player common-effect X tail offset assert missing');
mustContain(playerH, 'offsetof(Player, th07BombCommonEffectActivePointer)',
  'TH07 Player common-effect active pointer tail offset assert missing');
mustContain(playerH, 'ZUN_ASSERT_SIZE(Player, kPlayerObjectClearSize)',
  'TH07 Player clear-span size assert missing');

const rngTables = `${read('Rng.hpp')}\n${read('Rng.cpp')}`;
for (const value of [
  'kTh07RngNextU16FunctionAddress = 0x00431870',
  'kTh07RngNextU32FunctionAddress = 0x004318d0',
  'kTh07RngFloatZeroToOneFunctionAddress = 0x00431900',
  'kTh07NormalizeAngleFunctionAddress = 0x00431930',
  'kTh07RngFloatDivisorAddress = 0x00498b28',
  'kTh07NormalizeAngleLowerBoundAddress = 0x00498a5c',
  'kTh07NormalizeAnglePeriodAddress = 0x00498a60',
  'kTh07NormalizeAngleUpperBoundAddress = 0x00498a64',
  'kTh07RngXorConstant = 0x9630',
  'kTh07RngAddConstant = 0x9aad',
  'kTh07RngCarryMask = 0xc000',
  'kTh07RngCarryShift = 14',
  'kTh07RngSeedMultiplier = 4',
  'kTh07RngFloatDivisorBits = 0x4f800000',
  'kTh07NormalizeAngleLowerBoundBits = 0xc0490fdb',
  'kTh07NormalizeAnglePeriodBits = 0x40c90fdb',
  'kTh07NormalizeAngleUpperBoundBits = 0x40490fdb',
  'NextTh07RngSeed',
  'BuildTh07RngU32',
  'ScaleTh07RngU32ToUnit',
  'NormalizeTh07AngleWithAddend',
  'sizeof(Rng) == kTh07RngStateSize',
]) {
  mustContain(rngTables, value, `TH07 RNG/math helper metadata missing ${value}`);
}

const timerTables = read('Th07Timer.hpp');
for (const value of [
  'kTh07TimerIncrementFunctionAddress = 0x00439401',
  'kTh07TimerDecrementFunctionAddress = 0x004394c7',
  'kTh07TickTimerFunctionAddress = 0x0043958d',
  'kTh07TimerForceStepProducerFunctionAddress = 0x00418fc0',
  'kTh07SupervisorGlobalAddress = 0x00575950',
  'kTh07SupervisorTimerMultiplierOffset = 0x178',
  'kTh07SupervisorTimerMultiplierGlobalAddress = 0x00575ac8',
  'kTh07SupervisorTimerForceStepFlagsGlobalAddress = 0x00575adc',
  'kTh07SupervisorTimerForceStepFlagsOffset = 0x18c',
  'kTh07TimerForceStepBit = 5',
  'kTh07TimerForceStepMask',
  'kTh07TimerZeroConstantAddress = 0x00498a4c',
  'kTh07TimerOneFrameConstantAddress = 0x00498a54',
  'kTh07TimerSubframeThresholdAddress = 0x00498a58',
  'kTh07TimerZeroBits = 0x00000000',
  'kTh07TimerOneFrameBits = 0x3f800000',
  'kTh07TimerSubframeThresholdBits = 0x3f7d70a4',
  'kTh07TimerPreviousSentinel = -999',
  'kTh07TimerOneFrame = 1.0f',
  'kTh07TimerSubframeThreshold = 0.99f',
  'kTh07TimerForceStepProducerMultiplierStoreAddress = 0x004190a2',
  'kTh07TimerForceStepProducerFlagReadAddress = 0x004190bb',
  'kTh07TimerForceStepProducerFlagOrAddress = 0x004190c1',
  'kTh07TimerForceStepProducerFlagWriteAddress = 0x004190c4',
  'kTh07TimerForceStepFlagAccessCount = 3',
  'Th07TimerForceStepFlagAccessKind',
  'Th07TimerForceStepFlagAccess',
  'kTh07TimerForceStepFlagAccesses',
  'FindTh07TimerForceStepFlagAccess',
  'Th07TimerState',
  'HasTh07TimerForceStep',
  'UsesTh07TimerSubframePath',
  'IncrementTh07Timer',
  'DecrementTh07Timer',
  'TickTh07FrameSubtimer',
]) {
  mustContain(timerTables, value, `TH07 timer helper metadata missing ${value}`);
}
mustEqual(countMatches(timerTables, /Th07TimerForceStepFlagAccessKind::/g), 3,
  'TH07 timer force-step flag access contract count');
const effectTimerUse = read('Th07EffectTables.cpp');
mustContain(effectTimerUse, '#include "Th07Timer.hpp"', 'TH07 effect tables should use the shared timer helper');
mustContain(effectTimerUse, 'TickTh07FrameSubtimer(&timer->currentFrame',
  'TH07 stage/effect timers should route through source-derived timer helper');
mustContain(effectTimerUse, 'effectiveFramerateMultiplier);',
  'TH07 stage/effect timers should use the original DAT_00575ac8-equivalent multiplier');

const fileSystemTables = read('Th07FileSystemTables.hpp');
const fileSystem = read('FileSystem.cpp');
const archiveReader = `${read('Th07ArchiveReader.hpp')}\n${read('Th07ArchiveReader.cpp')}`;
const fileSystemRuntime = `${read('Th07FileSystemRuntime.hpp')}\n${read('Th07FileSystemRuntime.cpp')}`;
for (const value of [
  'kTh07OpenPathFunctionAddress = 0x00431330',
  'kTh07ArchiveLookupObjectAddress = 0x00626258',
  'kTh07ArchiveOwnerOpenFunctionAddress = 0x00438668',
  'kTh07ArchiveOwnerOpenThisMovAddress = 0x00438676',
  'kTh07ArchiveOwnerOpenCallAddress = 0x0043867b',
  'kTh07ArchiveOwnerCloseHostFunctionAddress = 0x00438de2',
  'kTh07ArchiveOwnerCloseThisMovAddress = 0x00438f8b',
  'kTh07ArchiveOwnerCloseCallAddress = 0x00438f90',
  'kTh07OpenPathLastFileSizeGlobalAddress = 0x004b9e64',
  'kTh07ArchiveOpenFunctionAddress = 0x0045f710',
  'kTh07ArchiveCloseFunctionAddress = 0x0045f800',
  'kTh07ArchiveMagicPbg4 = 0x34474250',
  'kTh07ArchiveHeaderMagicOffset = 0x00',
  'kTh07ArchiveHeaderEntryCountOffset = 0x04',
  'kTh07ArchiveHeaderPackedIndexOffsetOffset = 0x08',
  'kTh07ArchiveHeaderDecodedIndexSizeOffset = 0x0c',
  'kTh07ArchiveHeaderSize = 0x10',
  'kTh07ArchiveOwnerEntryTableOffset = 0x00',
  'kTh07ArchiveOwnerEntryCountOffset = 0x04',
  'kTh07ArchiveOwnerPathCopyOffset = 0x08',
  'kTh07ArchiveOwnerStreamOffset = 0x0c',
  'kTh07ArchiveOwnerFieldSpan = 0x10',
  'kTh07ArchiveEntryStride = 0x10',
  'kTh07ArchiveEntryNamePointerOffset = 0x00',
  'kTh07ArchiveEntryPackedOffsetOffset = 0x04',
  'kTh07ArchiveEntryDecodedSizeOffset = 0x08',
  'kTh07ArchiveEntryMetadataOffset = 0x0c',
  'kTh07ArchiveLoadIndexFunctionAddress = 0x0045fb50',
  'kTh07ArchiveBuildEntryTableFunctionAddress = 0x0045fde0',
  'kTh07ArchiveCopyStringFunctionAddress = 0x0045ffc0',
  'kTh07ArchiveFindEntryFunctionAddress = 0x0045fae0',
  'kTh07ArchiveEntrySizeFunctionAddress = 0x0045fab0',
  'kTh07ArchiveReadEntryFunctionAddress = 0x0045f960',
  'kTh07ArchiveMissLogBufferAddress = 0x00624210',
  'kTh07ArchivePathStringAddress = 0x00497150',
  'kTh07ArchiveVersionProbeFormatStringAddress = 0x00497140',
  'kTh07ArchivePath[] = "th07.dat"',
  'kTh07ArchiveVersionProbePath[] = "th07_0100b.ver"',
  'kTh07ArchiveOpenLogStringAddress = 0x00495100',
  'kTh07ArchiveCloseLogStringAddress = 0x004950cc',
  'kTh07ArchiveOpenMissingLogStringAddress = 0x004950e8',
  'kTh07ArchiveOpenErrorLogStringAddress = 0x00495084',
  'kTh07ArchiveMissStringAddress = 0x00497d38',
  'kTh07ArchiveDecodeStringAddress = 0x00497d24',
  'kTh07ExternalLoadStringAddress = 0x00497d14',
  'kTh07ExternalMissingStringAddress = 0x00497cf8',
  'ComputeTh07ArchiveLookupName',
]) {
  mustContain(fileSystemTables, value, `TH07 FileSystem archive lookup contract missing ${value}`);
}
for (const value of [
  '#include "Th07FileSystemTables.hpp"',
  'ComputeTh07ArchiveLookupName(filepath)',
  'error : %s is not found in arcfile.',
]) {
  mustContain(fileSystem, value, `TH07 FileSystem live source missing ${value}`);
}
for (const value of [
  'class Th07ArchiveReader',
  'DecodeTh07ArchiveLzss',
  'kTh07ArchiveMagicPbg4',
  'kTh07ArchiveHeaderPackedIndexOffsetOffset',
  'kTh07ArchiveHeaderDecodedIndexSizeOffset',
  'EqualsIgnoreAsciiCase',
  'matchLengthBits + 2',
]) {
  mustContain(archiveReader, value, `TH07 archive reader missing ${value}`);
}
for (const value of [
  'class Th07FileSystemRuntime',
  'OpenDefaultArchive',
  'OpenArchive(const std::filesystem::path &path)',
  'CloseArchive',
  'VersionProbeData',
  'OpenPath(std::string_view filepath, bool isExternalResource)',
  'ComputeTh07ArchiveLookupName(path.c_str())',
  'kTh07ArchiveVersionProbePath',
  'ArchiveNotOpen',
  'EntryNotFound',
  'ExternalOpenFailed',
]) {
  mustContain(fileSystemRuntime, value, `TH07 FileSystem runtime missing ${value}`);
}

const scoreTables = `${read('Th07ScoreTables.hpp')}\n${read('Th07ScoreTables.cpp')}`;
for (const value of [
  'kTh07GuiScoreUpdateFunctionAddress = 0x0042d8d5',
  'kTh07GuiScoreMaxClampInstructionAddress = 0x0042deb5',
  'kTh07GuiScoreMaxImmediateAddress = 0x0042deb8',
  'kTh07GuiScoreStepShiftInstructionAddress = 0x0042df08',
  'kTh07GuiScoreStepMaxCompareInstructionAddress = 0x0042df0e',
  'kTh07GuiScoreStepMaxCompareImmediateAddress = 0x0042df11',
  'kTh07GuiScoreStepMaxWriteInstructionAddress = 0x0042df17',
  'kTh07GuiScoreStepMaxWriteImmediateAddress = 0x0042df1a',
  'kTh07GuiScoreStepMinWriteInstructionAddress = 0x0042df26',
  'kTh07GuiScoreStepMinWriteImmediateAddress = 0x0042df29',
  'kTh07HighScoreCompareInstructionAddress = 0x0042dfd0',
  'kTh07HighScoreWriteInstructionAddress = 0x0042dfe5',
  'kTh07HighScoreShotCopyReadInstructionAddress = 0x0042dff4',
  'kTh07HighScoreShotCopyWriteInstructionAddress = 0x0042dff7',
  'kTh07ExtendAwardFunctionAddress = 0x0042d83a',
  'kTh07AddLifeFunctionAddress = 0x0042d5cd',
  'kTh07AddBombFunctionAddress = 0x0042d612',
  'kTh07ExtendAwardLifeStockReadAddress = 0x0042d847',
  'kTh07ExtendAwardBombStockReadAddress = 0x0042d891',
  'kTh07ItemCollectionFunctionAddress = 0x00432990',
  'kTh07PointItemExtendDifficultyCompareAddress = 0x004332e0',
  'kTh07PointItemExtendAwardCallAddress = 0x004333ca',
  'kTh07LifeItemExtendAwardCallAddress = 0x004335ba',
  'kTh07GuiScoreFieldOffset = 0x00',
  'kTh07ScoreFieldOffset = 0x04',
  'kTh07NextScoreIncrementFieldOffset = 0x08',
  'kTh07HighScoreFieldOffset = 0x0c',
  'kTh07HighScoreShotTypeFieldOffset = 0x10',
  'kTh07CurrentShotTypeFieldOffset = 0x20',
  'kTh07PointItemExtendProgressFieldOffset = 0x28',
  'kTh07PointItemExtendCountFieldOffset = 0x2c',
  'kTh07PointItemExtendNextThresholdFieldOffset = 0x30',
  'kTh07ExtendLifeStockFieldOffset = 0x5c',
  'kTh07ExtendBombStockFieldOffset = 0x68',
  'kTh07GuiScoreMax = 999999999',
  'kTh07GuiScoreStepShift = 5',
  'kTh07GuiScoreStepMax = 0x0008d55e',
  'kTh07GuiScoreStepMin = 1',
  'kTh07ExtendStockMaxExclusive = 8',
  'kTh07ExtendSoundIndex = 0x1c',
  'kTh07ExtendNotificationDurationFrames = 200',
  'kTh07ExtendLifeNotificationFlagMask = 0xfffffffc',
  'kTh07ExtendBombNotificationFlagMask = 0xfffffff3',
  'kTh07PointItemExtendNormalEarlyStep = 75',
  'kTh07PointItemExtendNormalMidStep = 150',
  'kTh07PointItemExtendNormalLateStep = 200',
  'kTh07PointItemExtendExtraLateStep = 500',
  'Th07GuiScoreStepState',
  'StepTh07GuiScore',
  'Th07HighScoreStepState',
  'StepTh07HighScore',
  'Th07ExtendAwardState',
  'ApplyTh07ExtendAward',
  'ComputeTh07PointItemExtendThreshold',
]) {
  mustContain(scoreTables, value, `TH07 GUI score helper metadata missing ${value}`);
}
mustNotContain(scoreTables, '78910', 'TH07 GUI score helper still uses copied TH06 step cap');
mustNotContain(scoreTables, 'scoreIncrement % 10', 'TH07 GUI score helper still rounds increments like TH06');

const manifestH = read('Th07ResourceManifest.hpp');
const manifest = read('Th07ResourceManifest.cpp');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/ecldata\d\.ecl"/g), 8, 'ECL manifest entry count');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/stage\d\.std"/g), 8, 'STD manifest entry count');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/msg\d\.dat"/g), 8, 'MSG manifest entry count');
mustContain(manifest, 'kTh07EnemyBulletAnmFile = {"data/etama.anm", "00498524"}',
  'TH07 enemy bullet ANM manifest entry missing');
mustContain(manifestH, 'kTh07RuntimeAnmFileCount = 63', 'TH07 runtime ANM manifest count missing');
mustContain(manifestH, 'kTh07RuntimeAnmLoadContractCount = 46', 'TH07 runtime ANM load contract count missing');
mustContain(manifestH, 'RuntimeAnmLoadContract', 'TH07 runtime ANM load contract type missing');
mustContain(manifest, 'kTh07RuntimeAnmFiles', 'TH07 runtime ANM manifest table missing');
const runtimeAnmManifest = extractCppArrayBlock(manifest, 'kTh07RuntimeAnmFiles');
mustEqual(countMatches(runtimeAnmManifest, /"data\/[^"]+\.anm"/g), 63, 'TH07 runtime ANM manifest entry count');
for (const value of [
  '{"data/title01.anm", "004954a8"}',
  '{"data/text.anm", "00497018"}',
  '{"data/front.anm", "00498378"}',
  '{"data/etama.anm", "00498524"}',
  '{"data/stg4bg5.anm", "00498904"}',
  '{"data/ascii.anm", "00498a28"}',
]) {
  mustContain(runtimeAnmManifest, value, `TH07 runtime ANM manifest missing ${value}`);
}
mustContain(manifest, 'kTh07RuntimeAnmLoadContracts', 'TH07 runtime ANM load contract table missing');
const runtimeAnmLoadContracts = extractCppArrayBlock(manifest, 'kTh07RuntimeAnmLoadContracts');
mustEqual(countMatches(runtimeAnmLoadContracts, /\{\{"data\/[^"]+\.anm",\s*"[^"]+"\},\s*"00[0-9a-f]+",\s*0x[0-9a-f]+,\s*0x[0-9a-f]+\},/gi),
  46, 'TH07 runtime ANM load contract entry count');
for (const value of [
  '{{"data/ascii.anm", "00498a28"}, "00401d70", 0x01, 0x0000}',
  '{{"data/front.anm", "00498378"}, "00428b19", 0x15, 0x0600}',
  '{{"data/face_08_00.anm", "00498164"}, "00428b19", 0x1c, 0x04ad}',
  '{{"data/stg4bg3.anm", "0049892c"}, "00407000", 0x07, 0x0320}',
  '{{"data/etama.anm", "00498524"}, "00426f60", 0x0b, 0x0200}',
  '{{"data/player02.anm", "00496ab0"}, "004423e0", 0x0a, 0x0400}',
  '{{"data/staff01.anm", "004985c4"}, "0041e590", 0x31, 0x0600}',
  '{{"data/text.anm", "00497018"}, "00438986", 0x00, 0x0700}',
  '{{"data/music00.anm", "00496bd8"}, "0043ae20", 0x2e, 0x0900}',
  '{{"data/result00.anm", "00496380"}, "00449b05", 0x2a, 0x0900}',
  '{{"data/title01.anm", "004954a8"}, "0045bf15", 0x20, 0x0900}',
]) {
  mustContain(runtimeAnmLoadContracts, value, `TH07 runtime ANM load contract missing ${value}`);
}
mustContain(manifest, 'FindRuntimeAnmLoadContract', 'TH07 runtime ANM load contract lookup missing');

const anmManagerLoadTables = read('Th07AnmManagerLoadTables.hpp');
for (const value of [
  'kTh07AnmManagerLoadAnmFunctionAddress = 0x0044df90',
  'kTh07AnmManagerLoadAnmSegmentFunctionAddress = 0x0044e070',
  'kTh07AnmManagerReleaseAnmFunctionAddress = 0x0044e4e0',
  'kTh07AnmManagerOpenPathFunctionAddress = 0x00431330',
  'kTh07AnmManagerLogFunctionAddress = 0x00431730',
  'kTh07AnmManagerLoadFailureStringAddress = 0x00495c7c',
  'kTh07AnmManagerInvalidSlotStringAddress = 0x00495c5c',
  'kTh07AnmManagerInvalidVersionStringAddress = 0x00495c3c',
  'kTh07AnmManagerTextureFailureStringAddress = 0x00495bf8',
  'kTh07AnmManagerEmbeddedTextureFailureStringAddress = 0x00495bb8',
  'kTh07AnmManagerSpriteRangeFailureStringAddress = 0x00495b80',
  'kTh07AnmManagerScriptRangeFailureStringAddress = 0x00495b4c',
  'kTh07AnmManagerLoadAnmInitialOwnedFlag = 1',
  'kTh07AnmManagerLoadAnmChainedOwnedFlag = 0',
  'kTh07AnmManagerMaxLoadedSlots = 0x32',
  'kTh07AnmManagerIndexedSpriteOrScriptLimit = 0x0a00',
  'kTh07AnmManagerLoadedSpritesOffset = 0x000060',
  'kTh07AnmManagerTextureNamePointersOffset = 0x28acc',
  'kTh07AnmManagerScriptsOffset = 0x28ef0',
  'kTh07AnmManagerSpriteIndicesOffset = 0x2b6f0',
  'kTh07AnmManagerSlotInfoOffset = 0x2def0',
  'kTh07AnmManagerSlotInfoStride = 0x0c',
  'kTh07AnmHeaderVersionOffset = 0x28',
  'kTh07AnmHeaderOwnedFileFlagOffset = 0x35',
  'kTh07AnmHeaderNextEntryOffsetOffset = 0x38',
  'Th07AnmManagerLoadContract',
  'kTh07AnmManagerLoadContract',
  'Th07AnmManagerSlotLoadedCountOffset',
  'Th07AnmManagerScriptPointerOffset',
  'Th07AnmManagerSpriteIndexOffset',
  'Th07AnmManagerTextureNamePointerOffset',
  'IsTh07AnmManagerSlotInRange',
  'IsTh07AnmManagerGlobalSpriteScriptIdInRange',
  'Th07AnmManagerAdvanceSpriteScriptOffset',
]) {
  mustContain(anmManagerLoadTables, value, `TH07 ANM manager load contract missing ${value}`);
}

const resourceFormatTables = `${read('Th07ResourceFormatTables.hpp')}\n${read('Th07ResourceFormatTables.cpp')}`;
for (const value of [
  'kTh07AnmFileCount = 63',
  'kTh07AnmSpriteCount = 1187',
  'kTh07AnmScriptCount = 727',
  'kTh07AnmVmScriptInstructionCount = 6214',
  'kTh07AnmVmScriptTerminatorCount = 727',
  'kTh07AnmVmOpcodeCount = 44',
  'kTh07AnmVmMaxOpcode = 80',
  'kTh07AnmVmOpcodeUsageCount = 44',
  'kTh07AnmVmOpcodeArgShapeCount = 44',
  'kTh07AnmVmTotalArgByteCount = 38724',
  'kTh07AnmVmInstructionSizeClassCount = 6',
  'kTh07StdBinaryObjectCount = 45',
  'kTh07StdBinaryQuadCount = 162',
  'kTh07StdBinaryInstanceCount = 906',
  'kTh07StdBinaryScriptInstructionCount = 378',
  'kTh07StdBinaryOpcodeUsageCount = 25',
  'kTh07StdBinaryOpcodeArgShapeCount = 25',
  'kTh07StdBinaryTotalArgByteCount = 4536',
  'kTh07ReadableStdDumpCount = 7',
  'kTh07EmptyReadableStdDumpStage = 6',
  'kTh07MsgEntryCount = 60',
  'kTh07MsgTimelineLabelCount = 1098',
  'kTh07MsgBinaryEntryPointerCount = 180',
  'kTh07MsgBinaryUniqueEntryCount = 60',
  'kTh07MsgBinaryInstructionCount = 4883',
  'kTh07MsgBinaryTextByteCount = 27832',
  'kTh07MsgBinaryWaitTotalFrames = 313330',
  'kTh07MsgBinaryMaxOpcode = 14',
  'kTh07MsgBinaryOpcodeUsageCount = 15',
  'kTh07MsgBinaryOpcodeArgShapeCount = 15',
  'kTh07MsgBinaryTotalArgByteCount = 46572',
  'Th07AnmFileSummary',
  'Th07AnmVmInstructionSizeSummary',
  'Th07AnmVmOpcodeUsage',
  'Th07AnmVmOpcodeArgShape',
  'kTh07AnmFileSummaries',
  'kTh07AnmVmOpcodes',
  'kTh07AnmVmOpcodeUsage',
  'kTh07AnmVmOpcodeArgShapes',
  'kTh07AnmVmInstructionSizeSummaries',
  'FindTh07AnmFileSummary',
  'FindTh07AnmVmInstructionSizeSummary',
  'FindTh07AnmVmOpcodeUsage',
  'FindTh07AnmVmOpcodeArgShape',
  'IsTh07AnmVmOpcodeUsed',
  'CountTh07AnmFilesWithTextureData',
  'CountTh07AnmVmScriptInstructionsBySize',
  'CountTh07AnmVmOpcodeUsageInstructions',
  'CountTh07AnmVmOpcodeArgBytes',
  'Th07StdBinarySummary',
  'Th07StdBinaryOpcodeUsage',
  'Th07StdBinaryOpcodeArgShape',
  'Th07MsgDumpSummary',
  'Th07MsgBinarySummary',
  'Th07MsgBinaryOpcodeUsage',
  'Th07MsgBinaryOpcodeArgShape',
  'FindTh07StdBinarySummary',
  'FindTh07StdBinaryOpcodeUsage',
  'FindTh07StdBinaryOpcodeArgShape',
  'FindTh07MsgDumpSummary',
  'FindTh07MsgBinarySummary',
  'FindTh07MsgBinaryOpcodeUsage',
  'FindTh07MsgBinaryOpcodeArgShape',
  'BuildTh07StdBinaryOpcodeMask',
  'CountTh07StdBinaryOpcodeUsageInstructions',
  'CountTh07StdBinaryOpcodeArgBytes',
  'CountTh07MsgTimelineLabels',
  'BuildTh07MsgBinaryOpcodeMask',
  'CountTh07MsgBinaryOpcodeUsageInstructions',
  'CountTh07MsgBinaryOpcodeArgBytes',
  '{"player02.anm", 17, 21, 256, 256, 2, true}',
  '{"capture.anm", 3, 4, 512, 512, 2, false}',
  '{"text.anm", 36, 30, 512, 256, 2, false}',
  '{3, 1769}',
  '{34, 331}',
  '{80, 1}',
  '{3, 1769, 4, 4, 7076}',
  '{32, 221, 20, 20, 4420}',
  '{80, 1, 4, 4, 4}',
  '{12, 2991}',
  '{28, 294}',
  '{6, 2, 4, 36, 58, OpcodeMask({0, 1, 3, 4, 5, 6, 7, 9, 11, 29, 30, 31}), false}',
  '{5, 103}',
  '{31, 5}',
  '{5, 103, 12, 12, 1236}',
  '{31, 5, 12, 12, 60}',
  '{6, 10516, 23, 9, 759, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}), 4468, 42720}',
  '{5, 1684}',
  '{14, 3}',
  '{3, 1049, 8, 48, 30384}',
  '{8, 60, 24, 40, 1884}',
  '{13, 124, 4, 4, 496}',
  '{8, 9, 215}',
]) {
  mustContain(resourceFormatTables, value, `TH07 resource format metadata missing ${value}`);
}
mustEqual(countMatches(resourceFormatTables, /^\s+\{"[^"]+\.anm", \d+, \d+, \d+, \d+, \d+, (?:true|false)\},$/gm),
  63, 'TH07 ANM file summary count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07AnmVmInstructionSizeSummaries'),
  /^\s*\{\d+, \d+\},/gm), 6, 'TH07 ANM VM instruction size summary count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07AnmVmOpcodeUsage'),
  /^\s*\{\d+, \d+\},/gm), 44, 'TH07 ANM VM opcode usage count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07AnmVmOpcodeArgShapes'),
  /^\s*\{\d+, \d+, \d+, \d+, \d+\},/gm), 44, 'TH07 ANM VM opcode argument-shape count');
mustEqual(countMatches(resourceFormatTables, /\bOpcodeMask\(\{/g), 8, 'TH07 STD binary summary count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07StdBinaryOpcodeUsage'),
  /^\s*\{\d+, \d+\},/gm), 25, 'TH07 STD binary opcode usage count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07StdBinaryOpcodeArgShapes'),
  /^\s*\{\d+, \d+, \d+, \d+, \d+\},/gm), 25, 'TH07 STD binary opcode argument-shape count');
mustEqual(countMatches(resourceFormatTables, /^\s+\{\d, \d, \d+\},$/gm), 8, 'TH07 MSG dump summary count');
mustEqual(countMatches(resourceFormatTables, /MsgOpcodeMask\(\{/g), 8, 'TH07 MSG binary summary count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07MsgBinaryOpcodeUsage'),
  /^\s*\{\d+, \d+\},/gm), 15, 'TH07 MSG binary opcode usage count');
mustEqual(countMatches(extractCppArrayBlock(resourceFormatTables, 'kTh07MsgBinaryOpcodeArgShapes'),
  /^\s*\{\d+, \d+, \d+, \d+, \d+\},/gm), 15, 'TH07 MSG binary opcode argument-shape count');

const stdParser = `${read('Th07StdFile.hpp')}\n${read('Th07StdFile.cpp')}`;
for (const value of [
  'kTh07StdHeaderSize = 0x0490',
  'kTh07StdObjectOffsetTableOffset = kTh07StdHeaderSize',
  'kTh07StdObjectHeaderSize = 0x001c',
  'kTh07StdObjectFirstQuadOffset = 0x001c',
  'kTh07StdQuadMinimumSize = 0x001c',
  'kTh07StdObjectInstanceSize = 0x0010',
  'kTh07StdScriptInstructionSize = 0x0014',
  'kTh07StdScriptArgumentSize = 0x000c',
  'kTh07StdFixedStringSize = 0x0080',
  'kTh07StdSongCount = 4',
  'Th07StdHeader',
  'Th07StdQuad',
  'Th07StdObject',
  'Th07StdObjectInstance',
  'Th07StdScriptInstruction',
  'Th07StdFile',
  'ParseTh07StdFile',
  'CountTh07StdQuads',
  'BuildTh07StdScriptOpcodeMask',
  'CountTh07StdScriptOpcode',
  'FindTh07StdObject',
  'STD script instruction argument size is not 0x0c',
  'STD object quad list is missing its sentinel',
  'STD object-instance list is missing its sentinel',
  'STD script is missing its sentinel',
]) {
  mustContain(stdParser, value, `TH07 STD parser missing ${value}`);
}

const stageLoaderTables = read('Th07StageLoaderTables.hpp');
for (const value of [
  'kTh07StageLoadStdFunctionAddress = 0x00407610',
  'kTh07StageLoadStdOpenPathCallAddress = 0x0040761e',
  'kTh07StageLoadStdErrorStringPushAddress = 0x00407638',
  'kTh07StageLoadStdErrorBufferPushAddress = 0x0040763d',
  'kTh07StageLoadStdErrorLogCallAddress = 0x00407642',
  'kTh07StageLoadStdMallocCallAddress = 0x00407730',
  'kTh07StageLoadStdSetScriptCallAddress = 0x004077b3',
  'kTh07StageLoadStdOpenPathFunctionAddress = 0x00431330',
  'kTh07StageLoadStdLogFunctionAddress = 0x004315f0',
  'kTh07StageLoadStdMallocFunctionAddress = 0x0047d39d',
  'kTh07StageLoadStdSetScriptFunctionAddress = kTh07StageSetAnmVmScriptFunctionAddress',
  'kTh07StageLoadStdAnmManagerPointerAddress = 0x004b9e44',
  'kTh07StageLoadStdLogBufferAddress = 0x00624210',
  'kTh07StageLoadStdFailureStringAddress = 0x0049888c',
  'kTh07StageLoadStdOwnerVmArrayOffset = 0x0000',
  'kTh07StageLoadStdOwnerStdDataOffset = kTh07StageStdDataOffset',
  'kTh07StageLoadStdOwnerQuadCountOffset = kTh07StageQuadCountOffset',
  'kTh07StageLoadStdOwnerObjectCountOffset = kTh07StageObjectCountOffset',
  'kTh07StageLoadStdOwnerObjectTableOffset = kTh07StageObjectTableOffset',
  'kTh07StageLoadStdOwnerObjectInstancesOffset = kTh07StageObjectInstancesOffset',
  'kTh07StageLoadStdOwnerScriptOffset = kTh07StageScriptOffset',
  'kTh07StageLoadStdHeaderObjectCountOffset = 0x0000',
  'kTh07StageLoadStdHeaderQuadCountOffset = 0x0002',
  'kTh07StageLoadStdHeaderInstancesOffsetOffset = 0x0004',
  'kTh07StageLoadStdHeaderScriptOffsetOffset = 0x0008',
  'kTh07StageLoadStdObjectOffsetTableFileOffset = kTh07StdObjectOffsetTableOffset',
  'kTh07StageLoadStdObjectFlagsOffset = 0x0003',
  'kTh07StageLoadStdObjectActiveFlag = 1',
  'kTh07StageLoadStdObjectFirstQuadOffset = kTh07StdObjectFirstQuadOffset',
  'kTh07StageLoadStdQuadByteSizeOffset = 0x0002',
  'kTh07StageLoadStdQuadAnmScriptOffset = 0x0004',
  'kTh07StageLoadStdQuadVmIndexOffset = 0x0006',
  'kTh07StageLoadStdQuadVmSlotSize = kTh07StageAnmVmSlotSize',
  'kTh07StageLoadStdAnmScriptBase = kTh07StageAnmVmScriptBase',
  'Th07StageStdLoaderContract',
  'kTh07StageStdLoaderContract',
  'Th07StageStdObjectOffsetTableEntryFileOffset',
  'Th07StageStdVmAllocationSize',
]) {
  mustContain(stageLoaderTables, value, `TH07 Stage STD loader metadata missing ${value}`);
}

const anmParser = `${read('Th07AnmFile.hpp')}\n${read('Th07AnmFile.cpp')}`;
for (const value of [
  'kTh07AnmHeaderMinimumSize = 0x00b8',
  'kTh07AnmSpriteOffsetTableOffset = 0x0040',
  'kTh07AnmSpriteRecordMinimumSize = 0x0014',
  'kTh07AnmScriptTableEntrySize = 0x0008',
  'kTh07AnmVmInstructionHeaderSize = 0x0008',
  'kTh07AnmVmTerminatorOpcode = -1',
  'kTh07AnmVmTerminatorSize = 0',
  'kTh07AnmVmParserMaxOpcode = 0x0050',
  'Th07AnmHeader',
  'Th07AnmSprite',
  'Th07AnmScriptEntry',
  'Th07AnmVmInstruction',
  'Th07AnmScript',
  'Th07AnmFile',
  'ParseTh07AnmFile',
  'FindTh07AnmScript',
  'FindTh07AnmScriptByOffset',
  'FindTh07AnmSprite',
  'CountTh07AnmVmInstructions',
  'CountTh07AnmVmTerminators',
  'CountTh07AnmVmOpcode',
  'CountTh07AnmVmInstructionSize',
  'CountTh07AnmVmArgumentBytes',
  'BuildTh07AnmVmOpcodeMask',
  'ANM file is too small for a TH07 entry header',
  'ANM sprite count out of range',
  'ANM script count out of range',
  'ANM sprite/script table exceeds file bounds',
  'ANM script terminator opcode changed',
  'ANM script terminator size changed',
  'ANM instruction header exceeds script bounds',
  'ANM instruction size out of range',
  'ANM instruction exceeds script bounds',
  'ANM opcode ',
  'ANM script is missing its terminator',
]) {
  mustContain(anmParser, value, `TH07 ANM parser missing ${value}`);
}

const msgParser = `${read('Th07MsgFile.hpp')}\n${read('Th07MsgFile.cpp')}`;
for (const value of [
  'kTh07MsgHeaderSize = 0x0004',
  'kTh07MsgEntryPointerSize = 0x0004',
  'kTh07MsgInstructionHeaderSize = 0x0004',
  'kTh07MsgMaxEntryPointers = 64',
  'kTh07MsgMaxOpcode = 0x0e',
  'kTh07MsgOpcodeScreenFade = 0x0e',
  'Th07MsgInstruction',
  'Th07MsgEntry',
  'Th07MsgFile',
  'ParseTh07MsgFile',
  'FindTh07MsgEntry',
  'FindTh07MsgEntryByOffset',
  'CountTh07MsgInstructions',
  'BuildTh07MsgOpcodeMask',
  'CountTh07MsgOpcode',
  'CountTh07MsgTextBytes',
  'CountTh07MsgWaitTotalFrames',
  'MSG entry pointer count out of range',
  'MSG pointer table exceeds file bounds',
  'MSG entry offset out of range',
  'MSG instruction header exceeds entry bounds',
  'MSG instruction args exceed entry bounds',
  'MSG opcode ',
  'MSG text opcode is missing text metadata',
  'MSG wait opcode argument size changed',
]) {
  mustContain(msgParser, value, `TH07 MSG parser missing ${value}`);
}

const guiMsgLoaderTables = read('Th07GuiMsgLoaderTables.hpp');
for (const value of [
  'kTh07GuiLoadMsgFunctionAddress = 0x00429935',
  'kTh07GuiFreeMsgFunctionAddress = 0x004299f8',
  'kTh07GuiLoadMsgFreeMsgCallAddress = 0x00429940',
  'kTh07GuiLoadMsgOpenPathCallAddress = 0x0042994a',
  'kTh07GuiLoadMsgFileStoreAddress = 0x00429955',
  'kTh07GuiLoadMsgNullCompareAddress = 0x00429961',
  'kTh07GuiLoadMsgErrorStringPushAddress = 0x0042996d',
  'kTh07GuiLoadMsgErrorBufferPushAddress = 0x00429972',
  'kTh07GuiLoadMsgErrorLogCallAddress = 0x00429977',
  'kTh07GuiLoadMsgCurrentIndexInitAddress = 0x0042998a',
  'kTh07GuiLoadMsgCurrentInstructionInitAddress = 0x00429997',
  'kTh07GuiLoadMsgEntryCountCompareAddress = 0x004299ba',
  'kTh07GuiLoadMsgEntryPointerReadAddress = 0x004299d3',
  'kTh07GuiLoadMsgEntryPointerRelocateAddress = 0x004299d7',
  'kTh07GuiLoadMsgEntryPointerWriteAddress = 0x004299ec',
  'kTh07GuiFreeMsgNullCompareAddress = 0x00429a06',
  'kTh07GuiFreeMsgFreeCallAddress = 0x00429a21',
  'kTh07GuiFreeMsgFileClearAddress = 0x00429a2d',
  'kTh07GuiLoadMsgOpenPathFunctionAddress = 0x00431330',
  'kTh07GuiLoadMsgLogFunctionAddress = 0x004315f0',
  'kTh07GuiFreeMsgFreeFunctionAddress = 0x0047d285',
  'kTh07GuiLoadMsgLogBufferAddress = 0x00624210',
  'kTh07GuiLoadMsgFailureStringAddress = 0x00498108',
  'kTh07GuiLoadMsgOpenPathExternalFlag = 0',
  'kTh07GuiLoadMsgMsgFileOffset = 0x1fba4',
  'kTh07GuiLoadMsgCurrentInstructionOffset = kTh07GuiMsgCurrentInstructionOffset',
  'kTh07GuiLoadMsgCurrentIndexOffset = kTh07GuiMsgCurrentIndexOffset',
  'kTh07GuiLoadMsgInvalidCurrentIndex = -1',
  'kTh07GuiLoadMsgEntryPointerTableOffset = kTh07MsgHeaderSize',
  'kTh07GuiLoadMsgEntryPointerSize = kTh07MsgEntryPointerSize',
  'Th07GuiMsgLoaderContract',
  'kTh07GuiMsgLoaderContract',
  'Th07GuiMsgEntryPointerFileOffset',
  'Th07GuiMsgPointerTableByteSize',
  'Th07GuiMsgRelocatedEntryPointer',
]) {
  mustContain(guiMsgLoaderTables, value, `TH07 GUI MSG loader metadata missing ${value}`);
}

const eclParser = `${read('Th07EclFile.hpp')}\n${read('Th07EclFile.cpp')}`;
for (const value of [
  'kTh07EclHeaderSize = 0x0040',
  'kTh07EclSubOffsetTableOffset = 0x0040',
  'kTh07EclSubOffsetTableSentinelEntries = 1',
  'kTh07EclSubHeaderSize = 0x0004',
  'kTh07EclRawInstructionHeaderSize = 0x0008',
  'kTh07EclTimelineInstructionHeaderSize = 0x0008',
  'kTh07EclTimelineInstructionMaxSize = 0x0020',
  'kTh07EclTimelineArgumentMaxSize = 0x0018',
  'kTh07EclRawInstructionTerminatorOpcode = -1',
  'kTh07EclRawInstructionTerminatorSize = 0x000c',
  'kTh07EclTimelineTerminatorTime = -1',
  'kTh07EclMaxOpcode = 0x00a1',
  'kTh07EclMaxTimelineOpcode = 0x000c',
  'Th07EclHeader',
  'Th07EclRawInstruction',
  'Th07EclSub',
  'Th07EclTimelineInstruction',
  'Th07EclTimeline',
  'Th07EclFile',
  'ParseTh07EclFile',
  'FindTh07EclSub',
  'FindTh07EclTimeline',
  'CountTh07EclSubInstructions',
  'CountTh07EclSubTerminators',
  'CountTh07EclSubOpcode',
  'CountTh07EclSubArgumentBytes',
  'BuildTh07EclSubOpcodeMask',
  'CountTh07EclTimelineInstructions',
  'CountTh07EclTimelineTerminators',
  'CountTh07EclTimelineOpcode',
  'CountTh07EclTimelineArgumentBytes',
  'BuildTh07EclTimelineOpcodeMask',
  'ECL sub offset table sentinel is not zero',
  'ECL first sub offset does not begin after the offset table',
  'ECL sub terminator size is not 0x0c',
  'ECL sub opcode is outside the current TH07 evidence range',
  'ECL sub is missing its terminator',
  'ECL timeline instruction size is invalid',
]) {
  mustContain(eclParser, value, `TH07 ECL parser missing ${value}`);
}

const eclManagerTables = read('Th07EclManagerTables.hpp');
for (const value of [
  'kTh07EclManagerLoadFunctionAddress = 0x0040e420',
  'kTh07EclManagerUnloadFunctionAddress = 0x0040e4f0',
  'kTh07EclManagerCallSubFunctionAddress = 0x0040e530',
  'kTh07EclManagerLoadOpenPathCallAddress = 0x0040e42e',
  'kTh07EclManagerLoadErrorStringPushAddress = 0x0040e440',
  'kTh07EclManagerLoadErrorBufferPushAddress = 0x0040e445',
  'kTh07EclManagerLoadErrorLogCallAddress = 0x0040e44a',
  'kTh07EclManagerUnloadFreeCallAddress = 0x0040e50d',
  'kTh07EclManagerLogBufferAddress = 0x00624210',
  'kTh07EclManagerLoadFailureStringAddress = 0x00498700',
  'kTh07EclManagerLogFunctionAddress = 0x004315f0',
  'kTh07EclManagerOpenPathFunctionAddress = 0x00431330',
  'kTh07EclManagerFreeFunctionAddress = 0x0047d285',
  'kTh07EclManagerOwnerFileOffset = 0x0000',
  'kTh07EclManagerOwnerSubOffsetTableOffset = 0x0004',
  'kTh07EclManagerLoadHeaderRelocationBaseOffset = 0x0004',
  'kTh07EclManagerLoadHeaderRelocationEntryCount = 0x0010',
  'kTh07EclManagerSubOffsetSentinelFileOffset = kTh07EclSubOffsetTableOffset',
  'kTh07EclManagerRuntimeSubOffsetTableFileOffset = 0x0044',
  'kTh07EclManagerRuntimeSubOffsetTableSkipsSentinel = true',
  'kTh07EclManagerContextCurrentInstrOffset = 0x0000',
  'kTh07EclManagerContextPrimaryTimerPreviousOffset = 0x0004',
  'kTh07EclManagerContextPrimaryTimerSubframeOffset = 0x0008',
  'kTh07EclManagerContextPrimaryTimerCurrentOffset = 0x000c',
  'kTh07EclManagerContextInterruptTimerPreviousOffset = 0x0080',
  'kTh07EclManagerContextInterruptTimerSubframeOffset = 0x0084',
  'kTh07EclManagerContextInterruptTimerCurrentOffset = 0x0088',
  'kTh07EclManagerContextSubIdOffset = 0x0214',
  'kTh07EclManagerContextMinimumKnownSpan = 0x0216',
  'kTh07EclManagerTimerPreviousSentinel = -999',
  'Th07EclManagerLoadContract',
  'Th07EclManagerCallSubContract',
  'kTh07EclManagerLoadContract',
  'kTh07EclManagerCallSubContract',
  'Th07EclManagerHeaderRelocationEntryFileOffset',
  'Th07EclManagerRuntimeSubOffsetEntryFileOffset',
]) {
  mustContain(eclManagerTables, value, `TH07 ECL manager metadata missing ${value}`);
}

mustEqual(countMatches(manifest, /\{Th07ShotType::/g), 12, 'SHT binding count');
mustContain(manifest, 'data/ply02bs.sht', 'SakuyaB focused SHT binding missing');
mustOrdered(manifest, [
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
const sht = read('ShtManager.cpp');
mustContain(sht, 'kShtFileBindings = kTh07ShtFiles', 'ShtManager should alias the portable TH07 SHT manifest');
mustContain(sht, 'FindShtFile(ToShotType(character, shot), focused)', 'ShtManager should delegate lookups to Th07ResourceManifest');

const playerShotTables = read('Th07PlayerShotTables.hpp');
for (const value of [
  'kTh07PlayerShotFileCount = 12',
  'kTh07PlayerShtPowerLevelCount = 9',
  'kTh07PlayerShtShotRecordCount = 582',
  'kTh07PlayerShtPowerThresholds',
  'kTh07ShtSpawnCallbackUsage = {386, 9, 18, 9, 51, 109}',
  'kTh07ShtUpdateCallbackUsage = {461, 36, 17, 41, 18, 9}',
  'kTh07ShtLoadFunctionAddress = 0x00442b70',
  'kTh07CurrentShotTypeGlobalAddress = 0x0062f647',
  'kTh07ShtUnfocusedPathTableAddress = 0x0049f530',
  'kTh07ShtFocusedPathTableAddress = 0x0049f548',
  'kTh07ShtPathTableEntryCount = 6',
  'kTh07ShtPathStringAddresses',
  'kTh07PlayerInitUnfocusedShotDataPointerAddAddress = 0x004423f3',
  'kTh07PlayerInitFocusedShotDataPointerAddAddress = 0x0044241b',
  'Th07PlayerShotSummary',
  'Th07PlayerShotDataLoadKind',
  'Th07PlayerShotDataLoadContract',
  'kTh07PlayerShotDataLoadContracts',
  'FindTh07PlayerShotSummary',
  'FindTh07PlayerShotDataLoadContract',
  'GetTh07UnfocusedPlayerShotSummary',
  'ply02bs.sht',
  '0x00496bb4',
  '0x00496aec',
  'kPlayerShotDataPointerOffset',
  'kPlayerFocusedShotDataPointerOffset',
  '4268, 79, 6',
  '3436, 63, 6',
]) {
  mustContain(playerShotTables, value, `TH07 player SHT metadata missing ${value}`);
}
mustEqual(countMatches(playerShotTables, /"ply\d\d[ab]s?\.sht"/g), 12, 'TH07 player SHT metadata row count');
mustEqual(countMatches(playerShotTables, /Th07ShotType::/g), 12, 'TH07 player SHT shot-type row count');
mustEqual(countMatches(playerShotTables, /Th07PlayerShotDataLoadKind::/g), 4,
  'TH07 player SHT load contract kind reference count');

const bulletTables = read('Th07BulletTables.hpp');
const bulletManager = read('BulletManager.cpp');
for (const value of [
  'Th07BulletEvidenceStatus::SourceDerived',
  'Th07BulletEvidenceStatus::ObsoleteTh06Carryover',
  'Th07BulletEvidenceStatus::Blocked',
  'kTh07EnemyBulletAnmResource',
  '"data/etama.anm"',
  '"00498524"',
  '168',
  '25',
  'kTh07EnemyBulletLoadFunctionAddress = 0x00426f60',
  'kTh07EnemyBulletTemplateTableAddress = 0x00495160',
  'kTh07EnemyBulletTemplateTableFileOffset = 0x00094360',
  'kTh07EnemyBulletTemplateEntrySize = 0x14',
  'kTh07EnemyBulletTemplateCount = 11',
  'kTh07EnemyBulletResolvedTemplateCount = 10',
  'kTh07EnemyBulletAnmManagerSlot = 0x0b',
  'kTh07EnemyBulletAnmScriptOffset = 0x0200',
  'kTh07EnemyBulletResolvedScriptLast = 0x0218',
  'kTh07EnemyBulletTemplateStride = 0x0b8c',
  'kTh07EnemyBulletTemplateBulletVmOffset = 0x0000',
  'kTh07EnemyBulletTemplateSpawnFastVmOffset = 0x024c',
  'kTh07EnemyBulletTemplateSpawnNormalVmOffset = 0x0498',
  'kTh07EnemyBulletTemplateSpawnSlowVmOffset = 0x06e4',
  'kTh07EnemyBulletTemplateSpawnDonutVmOffset = 0x0930',
  'kTh07EnemyBulletTemplateGrazeXOffset = 0x0b7c',
  'kTh07EnemyBulletTemplateSpriteHeightOffset = 0x0b89',
  'kTh07EnemyBulletTemplateSpriteOffsetCategoryOffset = 0x0b8a',
  'kTh07EnemyBulletTemplateMissingSprite = 0xffff',
  'kTh07EnemyBulletGrazeThreshold8Bits = 0x41000000',
  'kTh07EnemyBulletGrazeThreshold16Bits = 0x41800000',
  'kTh07EnemyBulletGrazeThreshold32Bits = 0x42000000',
  'kTh07EnemyBulletTemplateScripts',
  'FindTh07EnemyBulletTemplate',
  'CountTh07ResolvedEnemyBulletTemplates',
  'IsTh07EnemyBulletTemplateScriptResolved',
  '{0, 0x0200, 0x0212, 0x0213, 0x0214, 0x020f, 0, 8, 8, 4.0f, 5,',
  '{7, 0x0207, 0x0218, 0x0218, 0x0218, 0x0211, 112, 32, 32, 10.0f, 2,',
  '{8, 0x0208, 0x0218, 0x0218, 0x0218, 0x0211, 120, 32, 32, 5.0f, 1,',
  '{9, 0x0209, 0x0218, 0x0218, 0x0218, 0x0211, 128, 32, 32, 8.0f, 2,',
  '{10, 0x02a8, 0x02aa, 0x02aa, 0x02aa, 0x02a9, kTh07EnemyBulletTemplateMissingSprite',
  '"blocked"',
  'kTh07ObsoleteBulletResourceCarryovers',
  '"data/etama3.anm"',
  '"data/etama4.anm"',
  '"ANM_FILE_BULLET3"',
  '"ANM_FILE_BULLET4"',
  '"obsolete-th06-carryover"',
]) {
  mustContain(bulletTables, value, `TH07 bullet metadata missing ${value}`);
}
mustEqual(countMatches(bulletTables, /Th07BulletEvidenceStatus::SourceDerived\},/g), 10,
  'TH07 resolved enemy bullet template count');
mustEqual(countMatches(bulletTables, /Th07BulletEvidenceStatus::Blocked\},/g), 1,
  'TH07 blocked enemy bullet template count');
for (const value of [
  '#include "Th07BulletTables.hpp"',
  'LoadAnm(kTh07EnemyBulletAnmManagerSlot, "data/etama.anm"',
  'kTh07EnemyBulletAnmScriptOffset',
  'kTh07EnemyBulletTemplateScripts[9].bulletScript',
  'templateEvidence.grazeSize',
  'templateEvidence.spriteOffsetCategory',
  'ReleaseAnm(kTh07EnemyBulletAnmManagerSlot)',
]) {
  mustContain(bulletManager, value, `TH07 live bullet runtime missing ${value}`);
}
for (const value of [
  '"data/etama3.anm"',
  '"data/etama4.anm"',
  'ASB4(',
]) {
  mustNotContain(bulletManager, value, `TH07 live bullet runtime still uses obsolete ${value}`);
}

const shtFile = read('ShtFile.cpp');
for (const value of [
  'ShtCallbackSemantic::PeriodicSpawn',
  'ShtCallbackSemantic::Mode1OptionSpawn',
  'ShtCallbackSemantic::Mode3TrailSpawn',
  'ShtCallbackSemantic::RandomizedAnglePeriodicSpawn',
  'ShtCallbackSemantic::FixedAnglePeriodicSpawn',
  'ShtCallbackSemantic::HomingUpdateSpeed10',
  'ShtCallbackSemantic::HomingUpdateSpeed18',
  'ShtCallbackSemantic::VerticalAngleDriftUpdate',
  'ShtCallbackSemantic::Mode1OptionUpdate',
  'ShtCallbackSemantic::Mode3TrailUpdate',
  'ShtCallbackSemantic::TrailDraw',
  'ShtCallbackSemantic::ImpactScaleAndSound',
  'ShtCallbackSemantic::CollisionCounterSound',
]) {
  mustContain(shtFile, value, `SHT callback semantic missing ${value}`);
}

const eclOpcodeTables = `${read('EclOpcodeTables.hpp')}\n${read('EclOpcodeTables.cpp')}`;
for (const value of [
  'kTh07EclDispatchFunctionAddress = 0x00410520',
  'kTh07DeclOpcodeCount = 141',
  'kTh07CurrentEclEnumOpcodeCount = 136',
  'kTh07CurrentEclHandledOpcodeCount = 134',
  'kTh07MissingEnumOpcodeCount = 25',
  'kTh07MissingDispatchOpcodeCount = 26',
  'kTh07MissingNonNopDispatchOpcodeCount = 25',
  'kTh07EclOpcodeJumpTableAddress = 0x00417838',
  'kTh07EclDispatchAdvanceAddress = 0x0041677b',
  'kTh07EclOpcodeJumpTableEntryCount = 161',
  'kTh07HighEclOpcodeCaseCount = 25',
  'kTh07HighEclOpcodeSignatureCount = 31',
  'kTh07HighEclDirectEffectContractCount = 41',
  'kTh07HighEclCallContractCount = 17',
  'kTh07EclResolveFloatTargetFunctionAddress = 0x0040f3c0',
  'kTh07EclResolveFloatTargetJumpTableAddress = 0x0040f5d0',
  'kTh07EclResolveFloatTargetClassTableAddress = 0x0040f668',
  'kTh07EclResolveFloatTargetContractCount = 37',
  'kTh07EclIntReadResolverFunctionAddress = 0x0040e5b0',
  'kTh07EclIntReadResolverJumpTableAddress = 0x0040eace',
  'kTh07EclIntReadResolverContractCount = 70',
  'kTh07EclFloatReadResolverFunctionAddress = 0x0040edf0',
  'kTh07EclFloatReadResolverJumpTableAddress = 0x0040f28a',
  'kTh07EclFloatReadResolverContractCount = 74',
  'kTh07EclReadResolverCaseBase = 0x2710',
  'kTh07EclReadResolverMaxRelativeId = 0x49',
  'kTh07EclReadResolverFloatToIntBridgeAddress = 0x0048b8a0',
  'kTh07EclReadResolverRandomIntFunctionAddress = 0x004318d0',
  'kTh07EclReadResolverRandomFloatFunctionAddress = 0x00431900',
  'kTh07EclReadResolverVectorLengthFunctionAddress = 0x00442370',
  'kTh07EclReadResolverDistanceFunctionAddress = 0x00404e70',
  'kTh07EclClearHelperFunctionAddress = 0x00424740',
  'kTh07EclClearHelperObjectBaseOffset = 0x366628',
  'kTh07EclClearHelperObjectCount = 0x40',
  'kTh07EclClearHelperObjectStrideBytes = 0x04ec',
  'kTh07EclClearHelperObjectActiveOffset = 0x04d4',
  'kTh07EclClearHelperObjectFlagsOffset = 0x04e4',
  'kTh07EclClearHelperObjectStateOffset = 0x04e8',
  'kTh07EclClearHelperObjectTimerSentinel = 0xfffffc19',
  'kTh07EclClearHelperManagerEffectKindOffset = 0x37a160',
  'kTh07EclClearHelperFinalCooldownOffset = 0x37a12c',
  'kTh07EclClearHelperEffectSpawnFunctionAddress = 0x004326f0',
  'kTh07EclClearHelperLineDirectionFunctionAddress = 0x00417af0',
  'kTh07EclClearHelperLineStepConstantAddress = 0x00498a84',
  'kTh07EclItemClearRadiusHelperFunctionAddress = 0x00424c00',
  'kTh07EclItemClearRadiusItemArrayAddress = 0x0063b218',
  'kTh07EclItemClearRadiusItemCount = 0x400',
  'kTh07EclItemClearRadiusItemStrideBytes = 0x0d68',
  'kTh07EclItemClearRadiusItemStrideDwords = 0x35a',
  'kTh07EclItemClearRadiusPositionOffset = 0x0b8c',
  'kTh07EclItemClearRadiusStateOffset = 0x0bfc',
  'kTh07EclItemClearRadiusSkippedState = 5',
  'kTh07EclItemClearRadiusEffectKind = 6',
  'kTh07EclItemClearRadiusEffectArg = 1',
  'kTh07EclVisualEffectSpawnFunctionAddress = 0x004326f0',
  'kTh07EclVisualEffectManagerAddress = 0x00575c70',
  'kTh07EclVisualEffectSlotCount = 0x44c',
  'kTh07EclVisualEffectSlotSizeBytes = 0x0288',
  'kTh07EclVisualEffectFallbackSlotOffset = 0x0ae060',
  'kTh07EclVisualEffectNextIndexOffset = 0x0ae2e8',
  'kTh07EclVisualEffectPositionOffset = 0x024c',
  'kTh07EclVisualEffectTimerPreviousOffset = 0x0270',
  'kTh07EclVisualEffectModeOffset = 0x027c',
  'kTh07EclVisualEffectActiveFlagOffset = 0x027d',
  'kTh07EclVisualEffectInitializedFlagOffset = 0x027e',
  'kTh07EclVisualEffectVariantOffset = 0x027f',
  'kTh07EclVisualEffectAnmScriptBase = 0x02c4',
  'kTh07EclVisualEffectModeGatePointerAddress = 0x00626278',
  'kTh07EclVisualEffectModeGateThreshold = 0x80',
  'kTh07EclVisualEffectFloatToIntFunctionAddress = 0x0048b8a0',
  'kTh07EclVisualEffectRandomFloatFunctionAddress = 0x00431900',
  'kTh07EclVisualEffectAnmManagerPointerAddress = 0x004b9e44',
  'kTh07EclVisualEffectAnmScriptTableOffset = 0x28ef0',
  'kTh07EclVisualEffectSetScriptFunctionAddress = 0x0044ea20',
  'kTh07EclProjectionOpcode = 151',
  'kTh07EclProjectionHelperContractCount = 2',
  'kTh07EclProjectionSinWrapperFunctionAddress = 0x0048bb40',
  'kTh07EclProjectionSinCoreFunctionAddress = 0x0048bb5d',
  'kTh07EclProjectionCosWrapperFunctionAddress = 0x0048bbf0',
  'kTh07EclProjectionCosCoreFunctionAddress = 0x0048bc0d',
  'kTh07EclProjectionFiniteCheckFunctionAddress = 0x00483d38',
  'kTh07EclProjectionSinOpcodeCallInstructionAddress = 0x0041653f',
  'kTh07EclProjectionCosOpcodeCallInstructionAddress = 0x004165de',
  'kTh07EclProjectionRangeReductionConstantAddress = 0x004911ba',
  'kTh07EclProjectionMathErrorFlagAddress = 0x0049f77c',
  'kTh07EclOpcode138RecordHelperFunctionAddress = 0x00455170',
  'kTh07EclOpcode138RecordHelperCallInstructionAddress = 0x004160c5',
  'kTh07EclOpcode138RecordHelperCallThisPointerAddress = 0x004b9e44',
  'kTh07EclOpcode138RecordHelperOutputBaseOffset = 0x39f8',
  'kTh07EclOpcode138RecordHelperFlagFieldOffset = 0x4f30',
  'kTh07EclOpcode138RecordHelperEnabledBitMask = 0x08',
  'kTh07EclOpcode138RecordHelperSegmentNumeratorOffset = 0x4f32',
  'kTh07EclOpcode138RecordHelperSegmentDenominatorOffset = 0x4f36',
  'kTh07EclOpcode138RecordHelperMetadataPointerOffset = 0x01e4',
  'kTh07EclOpcode138RecordHelperRecordStrideBytes = 0x0038',
  'kTh07EclOpcode138RecordHelperRecordScaleBits = 0x3f800000',
  'kTh07MissingEnumOpcodes',
  'kTh07MissingDispatchOpcodes',
  'Th07EclOperandKind',
  'Th07EclEffectOwner',
  'Th07EclEffectWriteKind',
  'Th07EclCallContractKind',
  'Th07EclFloatTargetOwner',
  'Th07EclReadSourceKind',
  'Th07EclProjectionHelperKind',
  'Th07EclOpcodeStatus::SourceDerived',
  'hasDecompilerCase',
  'targetsCommonAdvance',
  'Th07EclHighOpcodeSignature',
  'Th07EclHighOpcodeEffectContract',
  'Th07EclHighOpcodeCallContract',
  'Th07EclFloatTargetResolverContract',
  'Th07EclReadResolverContract',
  'Th07EclClearHelperContract',
  'Th07EclItemClearRadiusHelperContract',
  'Th07EclVisualEffectSpawnContract',
  'Th07EclProjectionHelperContract',
  'Th07EclOpcode138RecordHelperContract',
  'kTh07HighEclOpcodeSignatures',
  'kTh07HighEclDirectEffectContracts',
  'kTh07HighEclCallContracts',
  'kTh07EclFloatTargetResolverContracts',
  'kTh07EclIntReadResolverContracts',
  'kTh07EclFloatReadResolverContracts',
  'kTh07EclClearHelperContract',
  'kTh07EclItemClearRadiusHelperContract',
  'kTh07EclVisualEffectSpawnContract',
  'kTh07EclProjectionHelperContracts',
  'kTh07EclOpcode138RecordHelperContract',
  'FindTh07HighEclOpcodeCase',
  'FindTh07HighEclOpcodeEffectContract',
  'FindTh07HighEclOpcodeCallContract',
  'FindTh07EclFloatTargetResolverContract',
  'FindTh07EclIntReadResolverContract',
  'FindTh07EclFloatReadResolverContract',
  'FindTh07EclProjectionHelperContract',
  'CountTh07HighEclOpcodeCases',
  'CountTh07HighEclOpcodeSignatures',
  'CountTh07HighEclOpcodeEffectContracts',
  'CountTh07HighEclOpcodeCallContracts',
  'CountTh07EclFloatTargetResolverContracts',
  'CountTh07EclIntReadResolverContracts',
  'CountTh07EclFloatReadResolverContracts',
  'Th07HighEclOpcodeUsesOperandKind',
  'IsTh07HighEclOpcodeBlocked',
  'IsTh07MissingEnumOpcode',
  'IsTh07MissingDispatchOpcode',
  'ecldata1.decl:1162',
  'ecldata8.decl:3484',
  'writes enemy+0x2e29 bit5 from arg0 and sets enemy+0x2e2f to 2',
  'writes enemy+0x2e2a bit7 from arg0',
  'writes enemy+0x2e2b bit3 from arg0',
  'jump-table entry targets the common instruction-advance path with no side effects',
  'Th07EclEffectOwner::GlobalIndexedTable',
  'Th07EclEffectOwner::GlobalPointerTableEntry',
  'Th07EclEffectWriteKind::CopyDwords',
  '0x0049fc44',
  '0x012fe098',
  '0x012fe25c',
  '0x2d8c, 0x04a8',
  '0x2eb0, 0x0254',
  '0x00417a54',
  '0x00417ab8',
  '0x004160cf',
  '0x00416729',
  '0x004160c5, 0x00455170',
  '0x0041653f, 0x0048bb40',
  '0x00416569, 0x0040f3c0',
  '0x004165de, 0x0048bbf0',
  '0x004155a5, 0x004326f0',
  '0x00416696, 0x0040f3c0',
  '0x004116db, 0x0040f3c0',
  '0x00416722, 0x0042f5a2',
  '0x2714, Th07EclFloatTargetOwner::Enemy, 0x070c, 0x0040f422',
  '0x2725, Th07EclFloatTargetOwner::Global, 0x004be408, 0x0040f4e5',
  '0x2739, Th07EclFloatTargetOwner::Global, 0x01347ab0, 0x0040f51d',
  '0x2746, Th07EclFloatTargetOwner::Enemy, 0x2b60, 0x0040f5bd',
  '0x2759, Th07EclFloatTargetOwner::Enemy, 0x0740, 0x0040f510',
  '0x2710, Th07EclReadSourceKind::EnemyI32, 0x06fc',
  '0x2714, Th07EclReadSourceKind::EnemyF32, 0x070c',
  '0x272a, Th07EclReadSourceKind::DistanceToGlobals, 0x2b0c, 0x004be408',
  '0x2748, Th07EclReadSourceKind::RandomIntModuloEnemyI32, 0x0744, 0x0748',
  '0x2748, Th07EclReadSourceKind::RandomFloatScaledEnemyF32, 0x0754, 0x0758',
  '0x274c, Th07EclReadSourceKind::RandomFloatScaledGlobalF32, 0x00498a60, 0x00498a64',
  'kTh07EclClearHelperFunctionAddress',
  'kTh07EclClearHelperObjectBaseOffset',
  'kTh07EclClearHelperObjectStrideBytes',
  'kTh07EclClearHelperObjectTimerSentinel',
  'kTh07EclClearHelperFinalCooldownValue',
  'kTh07EclClearHelperProtectedBypassMode',
  '0x00424cf6',
  '0x004326f0',
  '004326f0:15-75',
  '0048bb40:8-10',
  '0048bb5d:15-40',
  '0048bbf0:8-10',
  '0048bc0d:15-40',
  '00410520:3083-3096',
  '00410520:3097-3110',
  '00455170:15-48',
  '00410520:2833-2860',
  '00410520:2969-2974',
  '00410520:3184-3186',
  '0040f3c0:11-13',
  '0040f3c0:121-122',
  '0040e5b0:10-12',
  '0040e5b0:180-190',
  '0040edf0:196-199',
  '00424740:20-72',
  '00424c00:11-25',
  'ins_140(1.0f, [10005.0f], [10006.0f], [10004.0f])',
  'ins_146()',
  'ins_159([10010.0f], [10033.0f], [10035.0f], 0.5f)',
  'helper-call-contract-compiled-owner-lifetime-pending',
  'turn-case-surface-into-named-owner-fields',
  'treat-as-no-op-until-runtime-naming-is-proven',
]) {
  mustContain(eclOpcodeTables, value, `TH07 ECL high opcode metadata missing ${value}`);
}
mustEqual(countMatches(eclOpcodeTables, /, Th07EclOpcodeStatus::SourceDerived,/g), 25,
  'TH07 ECL source-derived high opcode case count');
mustEqual(countMatches(eclOpcodeTables, /, Th07EclOpcodeStatus::Blocked,/g), 0,
  'TH07 ECL blocked high opcode case count');
mustEqual(countMatches(eclOpcodeTables, /^\s+\{1(?:3[6-9]|4[0-6]|4[8-9]|5[0-9]|60|61), \d+, \d+, \{/gm), 31,
  'TH07 ECL high opcode signature count');
mustEqual(countMatches(eclOpcodeTables,
  /^\s+\{1(?:3[6-9]|4[0-5]|4[8-9]|5[0-3]|56|57|58|61), 0x[0-9a-f]+, Th07EclEffectOwner::/gm), 41,
  'TH07 ECL direct effect contract count');
mustEqual(countMatches(eclOpcodeTables, /Th07EclCallContractKind::/g), 17,
  'TH07 ECL helper-call/resolved-target contract count');
mustEqual(countMatches(eclOpcodeTables, /Th07EclFloatTargetOwner::Enemy/g), 30,
  'TH07 ECL resolved float target enemy contract count');
mustEqual(countMatches(eclOpcodeTables, /Th07EclFloatTargetOwner::Global/g), 7,
  'TH07 ECL resolved float target global contract count');
mustEqual(countMatches(eclOpcodeTables, /Th07EclReadSourceKind::/g), 144,
  'TH07 ECL int/float read resolver contract count');
mustEqual(countMatches(eclOpcodeTables, /Th07EclProjectionHelperKind::/g), 2,
  'TH07 ECL projection helper contract count');

const playerBombTables = `${read('PlayerBombTables.hpp')}\n${read('PlayerBombTables.cpp')}`;
const playerEvidenceTables = `${read('PlayerLayout.hpp')}\n${playerBombTables}`;
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::UnfocusedCalc/g), 6, 'unfocused bomb calc routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::UnfocusedDraw/g), 6, 'unfocused bomb draw routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::FocusedCalc/g), 6, 'focused bomb calc routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::FocusedDraw/g), 6, 'focused bomb draw routine count');
mustEqual(countMatches(playerBombTables, /PlayerModeEffectLifecycleKind::/g), 4,
  'player mode active-effect lifecycle contract count');
mustEqual(countMatches(playerBombTables, /PlayerModeRuntimeBranchKind::/g), 4,
  'player mode runtime branch contract count');
for (const value of [
  'kPlayerBombUpdateUnfocusedCallAddress',
  'kPlayerBombUpdateFocusedCallAddress',
  'kPlayerBombStartupUnfocusedCallAddress',
  'kPlayerBombStartupFocusedCallAddress',
  'kPlayerBombDrawUnfocusedCallAddress',
  'kPlayerBombDrawFocusedCallAddress',
  'kPlayerObjectAddress',
  'kPlayerObjectClearDwordCount',
  'kPlayerObjectClearSize',
  'kPlayerBombActiveOffset',
  'kPlayerBombCapturedFocusOffset',
  'kPlayerBombDurationOffset',
  'kPlayerBombScoreDrainStepOffset',
  'kPlayerBombLastProcessedFrameOffset',
  'kPlayerBombTimerSubframeOffset',
  'kPlayerBombTimerFrameOffset',
  'kPlayerBombCommonEffectXOffset',
  'kPlayerBombCommonEffectYOffset',
  'kPlayerBombCommonEffectDurationOffset',
  'kPlayerBombCommonEffectXGlobalAddress',
  'kPlayerBombCommonEffectYGlobalAddress',
  'kPlayerBombCommonEffectDurationGlobalAddress',
  'kPlayerBombCommonEffectActivePointerOffset',
  'kPlayerPositionOffset',
  'kPlayerVectorSize',
  'kPlayerPositionXOffset',
  'kPlayerPositionYOffset',
  'kPlayerPositionZOffset',
  'kPlayerHitboxMinOffset',
  'kPlayerHitboxMaxOffset',
  'kPlayerGrazeBoxMinOffset',
  'kPlayerGrazeBoxMaxOffset',
  'kPlayerGrabItemBoxMinOffset',
  'kPlayerGrabItemBoxMaxOffset',
  'kPlayerHitboxSizeOffset',
  'kPlayerGrazeBoxSizeOffset',
  'kPlayerGrabItemSizeOffset',
  'kPlayerAnmVmSlotSize',
  'kPlayerPrimaryAnmVmOffset',
  'kPlayerLeftOptionAnmVmOffset',
  'kPlayerRightOptionAnmVmOffset',
  'kPlayerFourthAnmVmOffset',
  'kPlayerOriginalAnmVmPrefixSize',
  'kPlayerPrimaryAnmVmScriptIndexOffset',
  'kPlayerLeftOptionAnmVmScriptIndexOffset',
  'kPlayerRightOptionAnmVmScriptIndexOffset',
  'kPlayerPrimaryAnmVmDrawPositionOffset',
  'kPlayerLeftOptionAnmVmDrawPositionOffset',
  'kPlayerRightOptionAnmVmDrawPositionOffset',
  'kPlayerLeftOptionPositionOffset',
  'kPlayerRightOptionPositionOffset',
  'kPlayerMovementDeltaOffset',
  'kPlayerMovementDeltaXOffset',
  'kPlayerMovementDeltaYOffset',
  'kPlayerMovementDirectionStateOffset',
  'kPlayerMovementHorizontalSpeedOffset',
  'kPlayerMovementVerticalSpeedOffset',
  'kPlayerOptionResetLeftXOffset',
  'kPlayerOptionResetLeftYOffset',
  'kPlayerOptionResetLeftZOffset',
  'kPlayerOptionResetRightXOffset',
  'kPlayerOptionResetRightYOffset',
  'kPlayerOptionResetRightZOffset',
  'kPlayerOptionResetAuxOffset',
  'kPlayerPrimaryAnmScript',
  'kPlayerLeftOptionAnmScript',
  'kPlayerRightOptionAnmScript',
  'kPlayerMovementLeanLeftAnmScript',
  'kPlayerMovementCenterFromLeftAnmScript',
  'kPlayerMovementLeanRightAnmScript',
  'kPlayerMovementCenterFromRightAnmScript',
  'kPlayerMovementLeanLeftAnmTableOffset',
  'kPlayerMovementCenterFromLeftAnmTableOffset',
  'kPlayerMovementLeanRightAnmTableOffset',
  'kPlayerMovementCenterFromRightAnmTableOffset',
  'kPlayerBombHorizontalSpeedMultiplierOffset',
  'kPlayerBombVerticalSpeedMultiplierOffset',
  'kPlayerCurrentPowerOffset',
  'kPlayerMode3LockoutTimerOffset',
  'kPlayerModeSoundTimerOffset',
  'kPlayerModeStateOffset',
  'kPlayerOptionStateOffset',
  'kPlayerModeDrawEnabledOffset',
  'kPlayerFocusHeldOffset',
  'kPlayerModeTransitionRequestOffset',
  'kPlayerOptionInterpolationPreviousFrameOffset',
  'kPlayerOptionInterpolationSubframeOffset',
  'kPlayerOptionInterpolationFrameOffset',
  'kPlayerModeTransitionEffectActivePointerOffset',
  'kPlayerShotDataPointerOffset',
  'kPlayerFocusedShotDataPointerOffset',
  'kPlayerMovementInputWordAddress',
  'kPlayerMovementAnmManagerAddress',
  'kPlayerMovementZeroFloatAddress',
  'kPlayerMovementTimerMultiplierGlobalAddress',
  'kPlayerMovementPlayfieldMinXAddress',
  'kPlayerMovementPlayfieldMinYAddress',
  'kPlayerMovementPlayfieldSpanXAddress',
  'kPlayerMovementPlayfieldSpanYAddress',
  'kPlayerShotUnfocusedOrthogonalSpeedOffset',
  'kPlayerShotFocusedOrthogonalSpeedOffset',
  'kPlayerShotUnfocusedDiagonalSpeedOffset',
  'kPlayerShotFocusedDiagonalSpeedOffset',
  'kPlayerMovementInputMaskShoot',
  'kPlayerMovementInputMaskFocus',
  'kPlayerMovementInputMaskUp',
  'kPlayerMovementInputMaskDown',
  'kPlayerMovementInputMaskLeft',
  'kPlayerMovementInputMaskRight',
  'kPlayerDirectionStateNone',
  'kPlayerDirectionStateUp',
  'kPlayerDirectionStateDown',
  'kPlayerDirectionStateLeft',
  'kPlayerDirectionStateRight',
  'kPlayerDirectionStateUpLeft',
  'kPlayerDirectionStateUpRight',
  'kPlayerDirectionStateDownLeft',
  'kPlayerDirectionStateDownRight',
  'kPlayerMovementDirectionStateClearAddress',
  'kPlayerMovementInputUpReadAddress',
  'kPlayerMovementInputDownReadAddress',
  'kPlayerMovementInputLeftReadAddress',
  'kPlayerMovementInputRightReadAddress',
  'kPlayerMovementInputFocusReadAddress',
  'kPlayerMovementFocusHeldWriteAddress',
  'kPlayerMovementFocusNotHeldWriteAddress',
  'kPlayerMovementFocusedOrthogonalSpeedFirstLoadAddress',
  'kPlayerMovementFocusedDiagonalSpeedFirstLoadAddress',
  'kPlayerMovementUnfocusedOrthogonalSpeedFirstLoadAddress',
  'kPlayerMovementUnfocusedDiagonalSpeedFirstLoadAddress',
  'kPlayerMovementLeanLeftScriptWriteAddress',
  'kPlayerMovementCenterFromLeftScriptWriteAddress',
  'kPlayerMovementLeanRightScriptWriteAddress',
  'kPlayerMovementCenterFromRightScriptWriteAddress',
  'kPlayerMovementLeanLeftApplyScriptCallAddress',
  'kPlayerMovementCenterFromLeftApplyScriptCallAddress',
  'kPlayerMovementLeanRightApplyScriptCallAddress',
  'kPlayerMovementCenterFromRightApplyScriptCallAddress',
  'kPlayerMovementHorizontalSpeedStoreAddress',
  'kPlayerMovementVerticalSpeedStoreAddress',
  'kPlayerMovementHorizontalMultiplierReadAddress',
  'kPlayerMovementVerticalMultiplierReadAddress',
  'kPlayerMovementHorizontalTimerMultiplierReadAddress',
  'kPlayerMovementVerticalTimerMultiplierReadAddress',
  'kPlayerMovementHorizontalDeltaStoreAddress',
  'kPlayerMovementVerticalDeltaStoreAddress',
  'kPlayerMovementPlayfieldMinXCompareAddress',
  'kPlayerMovementPlayfieldMaxXStoreAddress',
  'kPlayerMovementPlayfieldMinYCompareAddress',
  'kPlayerMovementPlayfieldMaxYStoreAddress',
  'kPlayerModeEffectActiveFlagOffset',
  'kPlayerModeEffectLifecycleNoInstruction',
  'kPlayerModeEffectLifecycleNoEffectId',
  'kPlayerModeEffectLifecycleContractCount',
  'kPlayerModeRuntimeNoInstruction',
  'kPlayerModeRuntimeNoOwnerOffset',
  'kPlayerModeRuntimeDefaultBranchState',
  'kPlayerModeRuntimeBranchContractCount',
  'kPlayerInitFunctionAddress',
  'kPlayerMovementFunctionAddress',
  'kPlayerKillBoxCollisionFunctionAddress',
  'kPlayerGrazeCollisionFunctionAddress',
  'kPlayerItemBoxCollisionFunctionAddress',
  'kPlayerLaserHitboxFunctionAddress',
  'kPlayerBombCollisionFunctionAddress',
  'kPlayerBombCollisionUpdateFunctionAddress',
  'kPlayerBombCollisionRegisterRectFunctionAddress',
  'kPlayerBombCollisionRegisterCircleFunctionAddress',
  'kPlayerMode1InvulnerableUpdateFunctionAddress',
  'kPlayerOptionFieldResetFunctionAddress',
  'kPlayerOptionResetPendingInterruptMirrorAddress',
  'kPlayerOptionResetPendingInterruptStateAddress',
  'kPlayerOptionResetSentinelBits',
  'kPlayerOptionResetZeroBits',
  'kPlayerOptionResetTopYThresholdAddress',
  'kPlayerOptionResetRightXThresholdAddress',
  'kPlayerOptionResetTopYThreshold',
  'kPlayerOptionResetRightXThreshold',
  'kPlayerOptionResetStateEntering',
  'kPlayerOptionResetStateInterrupted',
  'kPlayerModeConfigPointerAddress',
  'kPlayerModeConfigCurrentPowerCapOffset',
  'kPlayerMode1SoundTimerValue',
  'kPlayerMode1BlendFlag',
  'kPlayerMode1BlendFlagClearMask',
  'kPlayerMode1WhiteRgbBits',
  'kPlayerMode1WhiteArgbBits',
  'kPlayerMode1SpeedMultiplierBits',
  'kPlayerMode1FadeScaleYMultiplierBits',
  'kPlayerMode1FadeScaleYMultiplier',
  'kPlayerMode1FadeFrames',
  'kPlayerMode1FadeFramesFloat',
  'kPlayerMode1FadeFramesConstantAddress',
  'kPlayerMode1OneFrameConstantAddress',
  'kPlayerMode1ScaleYMultiplierAddress',
  'kPlayerMode1CompleteModeState',
  'kPlayerMode1CompleteCommonEffectDuration',
  'kPlayerBombCollisionBulletClearOffset',
  'kPlayerBombCollisionBulletClearStride',
  'kPlayerBombCollisionBulletClearCount',
  'kPlayerBombCollisionSlotArrayOffset',
  'kPlayerBombCollisionSlotStride',
  'kPlayerBombCollisionSlotCount',
  'kPlayerBombCollisionSlotRectSizeXOffset',
  'kPlayerBombCollisionSlotRadiusOffset',
  'kPlayerBombCollisionSlotFramesLeftOffset',
  'kPlayerBombCollisionSlotPayloadOffset',
  'kPlayerCollisionPayloadOffset',
  'kPlayerCollisionDefaultPayloadBits',
  'kPlayerCollisionHalfScale',
  'kPlayerGrazeCollisionMargin',
  'kPlayerBombCollisionHalfScale',
  'kPlayerItemBoxCollisionModeStateOffset',
  'kPlayerItemBoxCollisionGrabMinOffset',
  'kPlayerItemBoxCollisionGrabMaxOffset',
  'kPlayerItemBoxCollisionHalfScale',
  'CheckTh07PlayerKillBoxCollision',
  'CheckTh07PlayerGrazeBoxCollision',
  'IsTh07PlayerGrazeSuppressedMode',
  'kPlayerInitialZBits',
  'kPlayerHitboxDepthBits',
  'kPlayerGrazeBoxDepthBits',
  'kPlayerGrabItemSizeXBits',
  'kPlayerGrabItemSizeYBits',
  'kPlayerGrabItemSizeZBits',
  'kPlayerBombDefaultSpeedMultiplierBits',
  'kPlayerBombBackdropFadeFrames',
  'kPlayerBombBackdropFadeDelta',
  'kPlayerBombBackdropAlpha',
  'kPlayerBombBackdropPlateauChannel',
  'kPlayerBombBackdropBaseColor',
  'kPlayerBombBackdropPlateauColor',
  'kPlayerStageColorWriteCommonBackdropCallAddress',
  'kPlayerStageColorWriteMode4UpdateCallAddress',
  'kPlayerStageColorWriteMode4DrawCallAddress',
  'kPlayerModeUpdateFunctionAddress',
  'kPlayerModeDrawFunctionAddress',
  'kPlayerModeCleanupFunctionAddress',
  'kPlayerModeEnterMode4FunctionAddress',
  'kPlayerModeEnterMode3FunctionAddress',
  'kPlayerMode4StateOffset',
  'kPlayerMode4DrawEnabledOffset',
  'kPlayerModeStateIdle',
  'kPlayerModeState1',
  'kPlayerModeState2',
  'kPlayerModeState3',
  'kPlayerModeState4',
  'kPlayerModeTransitionRequestIdle',
  'kPlayerModeTransitionRequestStartMode4',
  'kPlayerModeTransitionRequestCleanup',
  'kPlayerOptionStateHidden',
  'kPlayerOptionStateUnfocused',
  'kPlayerOptionStateFocusing',
  'kPlayerOptionStateFocused',
  'kPlayerOptionStateUnfocusing',
  'kPlayerMode4StageColorFrameGlobalAddress',
  'kPlayerMode4CommonEffectDurationFrames',
  'kPlayerMode3CommonEffectDurationFrames',
  'kPlayerMode3EffectInterpolationDurationFrames',
  'kPlayerMode1CleanupCommonEffectDurationFrames',
  'kPlayerInitialCommonEffectDurationFrames',
  'kPlayerMode4StageColorFadeInFrames',
  'kPlayerMode4StageColorFadeOutStartFrame',
  'kPlayerMode4StageColorTotalFrames',
  'kPlayerMode4StageColorFadeDelta',
  'kPlayerMode4StageColorPlateauChannel',
  'kPlayerMode4StageColorPlateauColor',
  'kPlayerBombCommonEffectSpawnFunctionAddress',
  'kPlayerBombCommonEffectManagerAddress',
  'kPlayerBombCommonEffectTableAddress',
  'kPlayerBombCommonEffectId',
  'kPlayerBombCommonEffectAnmScript',
  'kPlayerBombCommonEffectUpdateCallbackAddress',
  'kPlayerBombCommonEffectInitCallbackAddress',
  'kPlayerBombCommonEffectSpawnSlotArgument',
  'kPlayerBombCommonEffectSpawnFlagArgument',
  'kPlayerBombCommonEffectInitialColor',
  'kPlayerBombCommonEffectTintColor',
  'kPlayerBombCommonEffectResetXBits',
  'kPlayerMode4EffectStartXOffset',
  'kPlayerMode4EffectStartYOffset',
  'kPlayerMode4EffectStartDurationOffset',
  'kPlayerBombCommonEffectInterpScaleBits',
  'kPlayerBombCommonEffectPostSpawnMultiplierBits',
  'kPlayerModeTransitionEffectId',
  'kPlayerModeTransitionEffectSpawnSlotArgument',
  'kPlayerModeTransitionEffectSpawnFlagArgument',
  'kPlayerModeTransitionEffectInitialColor',
  'kPlayerMode4EffectInitialScaleBits',
  'kPlayerMode4EffectFinalScaleBits',
  'kPlayerMode3EffectInitialScaleBits',
  'kPlayerMode3EffectFinalScaleBits',
  'PlayerBombCommonEffectConfig',
  'PlayerMovementInputContract',
  'PlayerHorizontalAnimationTransitionKind',
  'PlayerHorizontalAnimationTransitionContract',
  'PlayerItemBoxCollisionInput',
  'Th07PlayerBombCollisionSlot',
  'PlayerBombCollisionInput',
  'PlayerBombCollisionResult',
  'PlayerOptionFieldResetState',
  'PlayerMode1InvulnerableInput',
  'PlayerMode1InvulnerableState',
  'PlayerOptionShotEffectInput',
  'PlayerOptionShotEffectState',
  'PlayerMovementSpeedInput',
  'PlayerMovementVector',
  'kPlayerMovementInputContract',
  'kPlayerHorizontalAnimationTransitionContracts',
  'IsTh07PlayerItemCollectionMode',
  'CheckTh07PlayerItemBoxCollision',
  'IsTh07PlayerBombCollisionSlotFree',
  'BuildTh07PlayerBombCollisionRectSlot',
  'BuildTh07PlayerBombCollisionCircleSlot',
  'CheckTh07PlayerBombCollisionSlot',
  'TickTh07PlayerBombCollisionSlot',
  'BuildTh07PlayerOptionFieldResetState',
  'BuildTh07PlayerMode1InvulnerableUpdateState',
  'BuildTh07PlayerOptionShotEffectState',
  'BuildTh07PlayerDirectionState',
  'BuildTh07PlayerMovementVector',
  'FindPlayerHorizontalAnimationTransitionContract',
  'PlayerModeTransitionEffectConfig',
  'PlayerInitialModeOwnerState',
  'PlayerMode4EntryOwnerState',
  'PlayerMode3EntryOwnerState',
  'PlayerModeEffectPointerSlot',
  'PlayerModeEffectLifecycleKind',
  'PlayerModeEffectLifecycleContract',
  'PlayerModeRuntimeBranchKind',
  'PlayerModeRuntimeBranchContract',
  'PlayerStageColorWriteProducer',
  'kPlayerModeEffectLifecycleContracts',
  'kPlayerModeRuntimeBranchContracts',
  'BuildPlayerBombCommonEffectConfig',
  'BuildPlayerMode4TransitionEffectConfig',
  'BuildPlayerMode3TransitionEffectConfig',
  'BuildPlayerInitialModeOwnerState',
  'BuildPlayerMode4EntryOwnerState',
  'BuildPlayerMode3EntryOwnerState',
  'FindPlayerModeEffectLifecycleContract',
  'FindPlayerModeRuntimeBranchContract',
  'FindPlayerStageColorWriteProducer',
  'kPlayerBombStartupFallbackDurationFrames',
  'kPlayerBombPowerBonusOnStart',
  'durationWriteAddress',
  'commonEffectDurationWriteAddress',
  'minimumScoreDrainTotal',
  'minimumScoreDrainTotalLoadAddress',
  'scoreDrainFactorPushAddress',
  'scoreDrainFactorBits',
  'ComputePlayerBombBackdropColor',
  'ComputePlayerMode4StageColor',
  'ApplyPlayerBombScoreDrainDifficultyScale',
  'ComputePlayerBombScoreDrainStep',
  'speedMultiplierBits',
  'verticalSpeedMultiplierWriteAddress',
  'horizontalSpeedMultiplierWriteAddress',
  '0x004bdad8',
  '0x2df9e',
  '0x0b7e78',
  '0x004d44d8',
  '0x004d44dc',
  '0x024c',
  '0x0498',
  '0x06e4',
  '0x0930',
  '0x0934',
  '0x0938',
  '0x0948',
  '0x0954',
  '0x0960',
  '0x096c',
  '0x0978',
  '0x0984',
  '0x0990',
  '0x099c',
  '0x09a8',
  '0x01d8',
  '0x0424',
  '0x0670',
  '0x01c8',
  '0x0414',
  '0x0660',
  '0x09b4',
  '0x09c0',
  '0x09cc',
  '0x09d0',
  '0x241c',
  '0x2420',
  '0x2424',
  '0x0400',
  '0x0480',
  '0x0481',
  '0x0401',
  '0x0402',
  '0x0403',
  '0x0404',
  '0x004423e0',
  '0x0043ee50',
  '0x004b9e50',
  '0x004b9e44',
  '0x00575ac8',
  '0x0062f874',
  '0x0062f878',
  '0x0062f87c',
  '0x0062f880',
  '0x0b7e70',
  '0x0024',
  '0x0028',
  '0x002c',
  '0x0030',
  '0x0043ee82',
  '0x0043f1fd',
  '0x0043f34f',
  '0x0043e260',
  '0x0043e3b0',
  '0x0043e4e0',
  '0x0043e6b0',
  '0x0043d0e0',
  '0x0043e0a0',
  '0x00440940',
  '0x004411c0',
  '0x00441800',
  '0x004418b0',
  '0x17dc',
  '0x23dc',
  '0x2404',
  '0x3efae148',
  '0x40a00000',
  '0x41400000',
  '0x00408710',
  '0x004091b0',
  '0x00409dd0',
  '0x0040a3a0',
  '0x0040a7c0',
  '0x0040af10',
  '0x0040b7d0',
  '0x0040be20',
  '0x0040c2e0',
  '0x0040ca50',
  '0x0040d4c0',
  '0x0040da80',
  '0x0040933c',
  '0x0040a533',
  '0x0040b073',
  '0x0040b97d',
  '0x0040bf96',
  '0x0040cbc5',
  '0x0040d62c',
  '0x0040dd2c',
  '0x004087a7',
  '0x0040927f',
  '0x00409326',
  '0x00409fc3',
  '0x0040a997',
  '0x0040b058',
  '0x0040dd54',
  '0x004084d0',
  '0x0044162f',
  '0x0044233b',
  '0x00441330',
  '0x00441370',
  '0x004413b7',
  '0x004413fa',
  '0x00441484',
  '0x0044151f',
  '0x00441546',
  '0x00441660',
  '0x00441670',
  '0x00441960',
  '0x00441bd0',
  '0x004420b0',
  '0x0044223d',
  '0x00441a5e',
  '0x00441a7a',
  '0x00441a84',
  '0x00441aa4',
  '0x00441b74',
  '0x00441bee',
  '0x00441bf8',
  '0x00441c19',
  '0x00441d35',
  '0x004417e1',
  '0x004417eb',
  '0x00575948',
  '0x00498b00',
  '0x00498a70',
  '0x004d44e0',
  '0x1fe',
  '0x21c',
  '0x0b7e6c',
  '0x240d',
  '0x23fc',
  '0x2400',
  '0x240a',
  '0x240b',
  '0x240c',
  '0x1c',
  '0x28',
  '0x1e',
  '0xf0',
  '0x3fa66666',
  '0x16a0c',
  '0x16a10',
  '0x16a14',
  '0x2410',
  '0x2414',
  '0x2418',
]) {
  mustContain(playerEvidenceTables, value, `TH07 bomb calc routine missing ${value}`);
}
mustEqual(countMatches(playerEvidenceTables, /PlayerHorizontalAnimationTransitionKind::/g), 4,
  'TH07 player horizontal animation transition contract count');
mustContain(playerEvidenceTables, 'FUN_0043ee50:23-163',
  'TH07 player movement input contract evidence missing');
mustEqual(countMatches(playerBombTables, /PlayerStageColorWriteKind::/g), 3, 'TH07 player stage-color producer kind reference count');

const playerRuntime = `${read('Player.hpp')}\n${read('Player.cpp')}`;
mustContain(playerRuntime, 'DIFFABLE_STATIC_ARRAY_ASSIGN(CharacterData, 6, g_CharData)',
  'TH07 Player character data must cover all 6 shot types');
mustContain(playerRuntime, 'FireBulletSakuyaA', 'TH07 Player SakuyaA shot entry missing');
mustContain(playerRuntime, 'FireBulletSakuyaB', 'TH07 Player SakuyaB shot entry missing');
mustContain(playerRuntime, '#include "Th07PlayerShotTables.hpp"', 'TH07 Player runtime should consume source-derived SHT metadata');
mustContain(playerRuntime, 'Player::CalcBombCollision', 'TH07 Player bomb collision helper should have a current source surface');
mustContain(playerRuntime, 'Player::UpdateBombCollisionSlots', 'TH07 Player bomb collision slot update should have a current source surface');
mustContain(playerRuntime, 'Player::RegisterBombCollisionRect', 'TH07 Player bomb collision rect register should have a current source surface');
mustContain(playerRuntime, 'Player::RegisterBombCollisionCircle', 'TH07 Player bomb collision circle register should have a current source surface');
mustContain(playerRuntime, 'Player::OptionShotEffectCallback', 'TH07 Player option shot effect callback should have a current source surface');
mustContain(playerRuntime, 'Player::UpdateMode1Invulnerable', 'TH07 Player mode 1 invulnerable update should have a current source surface');
mustContain(playerRuntime, 'Player::ResetOptionFields', 'TH07 Player option field reset should have a current source surface');
mustContain(playerRuntime, 'g_Th07PlayerModeConfigOwner',
  'TH07 Player mode 1 update should expose the unresolved DAT_00575948 owner pointer');
mustContain(playerRuntime, 'g_Th07PlayerOptionResetPendingInterruptState',
  'TH07 Player option reset pending interrupt state mirror should be explicit');
mustContain(playerRuntime, 'GetTh07UnfocusedPlayerShotSummary(shotType).orthogonalMovementSpeed()',
  'TH07 Player runtime should derive unfocused speed from SHT metadata');
mustContain(playerRuntime, 'GetTh07UnfocusedPlayerShotSummary(shotType).focusedOrthogonalMovementSpeed()',
  'TH07 Player runtime should derive focused speed from SHT metadata');
for (const value of [
  'Th07OrthogonalSpeed(Th07ShotType::ReimuA)',
  'Th07FocusedOrthogonalSpeed(Th07ShotType::ReimuA)',
  'Th07DiagonalSpeed(Th07ShotType::ReimuA)',
  'Th07FocusedDiagonalSpeed(Th07ShotType::ReimuA)',
  'Th07OrthogonalSpeed(Th07ShotType::MarisaA)',
  'Th07FocusedOrthogonalSpeed(Th07ShotType::MarisaA)',
  'Th07OrthogonalSpeed(Th07ShotType::SakuyaA)',
  'Th07FocusedOrthogonalSpeed(Th07ShotType::SakuyaB)',
  'Th07DiagonalSpeed(Th07ShotType::SakuyaA)',
  'Th07FocusedDiagonalSpeed(Th07ShotType::SakuyaB)',
]) {
  mustContain(playerRuntime, value, `TH07 Player live speed table missing ${value}`);
}
for (const value of [
  'Th07ModeState()',
  'Th07OptionState()',
  'Th07FocusHeld()',
  'Th07ModeTransitionRequest()',
  'Th07BombCommonEffectXBits()',
  'Th07BombCommonEffectYBits()',
  'Th07BombCommonEffectDuration()',
  'ApplyTh07InitialPlayerModeOwnerState',
  'BuildPlayerInitialModeOwnerState()',
  'Th07OptionInterpolationTimer()',
  'Th07MovementDirectionState()',
  'Th07MovementHorizontalSpeed()',
  'Th07MovementVerticalSpeed()',
  'ApplyTh07PlayerCollisionSizes(p, shotSummary)',
  'shotSummary.hitboxDiagonalExtent() / 2.0f',
  'shotSummary.grazeDiagonalExtent() / 2.0f',
  'ApplyTh07PlayerMovementCore(this, g_CurFrameInput)',
  'BuildTh07PlayerMovementVector(inputMask',
  'BuildTh07PlayerMovementSpeedInput(Th07ShotTypeFromGameManager())',
  'kPlayerMovementInputMaskShoot',
  'SyncLegacyPlayerMovementBridge',
  'TH07_PLAYER_THISCALL',
  '*reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 0',
  '*reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 3',
  '*reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(this) + kPlayerModeStateOffset) != 4',
  'halfItemWidth',
  'grabItemTopLeft',
  'grazeTopLeft',
]) {
  mustContain(playerRuntime, value, `TH07 Player mode-owner runtime bridge missing ${value}`);
}

const bombRuntime = read('BombData.cpp');
mustContain(read('BombData.hpp'), 'DIFFABLE_EXTERN_ARRAY(BombData, 6, g_BombData)',
  'TH07 BombData declaration must cover all 6 shot types');
mustContain(bombRuntime, 'DIFFABLE_STATIC_ARRAY_ASSIGN(BombData, 6, g_BombData)',
  'TH07 BombData runtime table must cover all 6 shot types');
mustContain(bombRuntime, '/* SakuyaA */ {nullptr, nullptr}', 'TH07 SakuyaA bomb slot must be an explicit pending placeholder');
mustContain(bombRuntime, '/* SakuyaB */ {nullptr, nullptr}', 'TH07 SakuyaB bomb slot must be an explicit pending placeholder');
for (const value of [
  'ComputePlayerBombBackdropColor(player->bombInfo.timer.AsFrames(), player->bombInfo.duration)',
  'ResetTh07AnmManagerColorMultiplier()',
  'ApplyTh07StageAnmManagerColorWrite(&g_Stage, backdropColor)',
  'GuardTh07StageAnmManagerColorReset(&g_Stage)',
]) {
  mustContain(bombRuntime, value, `TH07 common bomb backdrop runtime missing ${value}`);
}
mustNotContain(bombRuntime, 'ScreenEffect::DrawSquare(&viewport, darknessLevel << 24)', 'TH07 common bomb backdrop still uses TH06 rectangle darken path');

const effectTables = `${read('Th07EffectTables.hpp')}\n${read('Th07EffectTables.cpp')}`;
mustContain(effectTables, 'kTh07EffectTableAddress', 'TH07 effect table address missing');
mustContain(effectTables, 'kTh07EffectTableCount = 34', 'TH07 effect table count missing');
mustContain(effectTables, 'kTh07BombCommonEffectId', 'TH07 bomb common effect id missing');
mustContain(effectTables, 'kTh07EffectManagerAddress', 'TH07 effect manager address missing');
mustContain(effectTables, 'kTh07EffectSpawnFunctionAddress', 'TH07 effect spawn function address missing');
mustContain(effectTables, 'kTh07EffectSpawnWithAuxVectorFunctionAddress', 'TH07 effect aux-vector spawn function address missing');
mustContain(effectTables, 'kTh07EffectSpawnDedicatedFunctionAddress', 'TH07 effect dedicated spawn function address missing');
mustContain(effectTables, 'kTh07EffectUpdateFunctionAddress', 'TH07 effect update function address missing');
mustContain(effectTables, 'kTh07EffectLoadStageAnmFunctionAddress', 'TH07 effect stage ANM load function address missing');
mustContain(effectTables, 'kTh07EffectSlotSize = 0x2d8', 'TH07 effect slot size missing');
mustContain(effectTables, 'kTh07EffectArrayOffset = 0x1c', 'TH07 effect array offset missing');
mustContain(effectTables, 'kTh07EffectPrimarySlotCount = 400', 'TH07 effect primary slot count missing');
mustContain(effectTables, 'kTh07EffectDedicatedSlotBaseIndex = 400', 'TH07 effect dedicated slot base missing');
mustContain(effectTables, 'kTh07EffectUpdateSlotCount = 0x198', 'TH07 effect update slot count missing');
mustContain(effectTables, 'kTh07EffectFallbackSlotOffset = 0x4885c', 'TH07 effect fallback slot offset missing');
mustContain(effectTables, 'kTh07EffectSlotActiveFlagOffset = 0x2cc', 'TH07 effect active flag offset missing');
mustContain(effectTables, 'kTh07EffectSlotUpdateCallbackOffset = 0x2c8', 'TH07 effect update callback offset missing');
mustContain(effectTables, 'kTh07EffectDrawListScrollHeadOffset = 0x48e08', 'TH07 effect scroll draw list head missing');
mustContain(effectTables, 'kTh07EffectDrawListIndexedHeadOffset = 0x490e0', 'TH07 effect indexed draw list head missing');
mustContain(effectTables, 'kTh07EffectFrameCounterOffset = 0x496a4', 'TH07 effect frame counter offset missing');
mustContain(effectTables, 'Th07EffectTableEntry', 'TH07 effect table entry type missing');
mustContain(effectTables, 'Th07EffectDrawListLayout', 'TH07 effect draw list layout missing');
mustContain(effectTables, 'Th07EffectSpawnContract', 'TH07 effect spawn contract missing');
mustContain(effectTables, 'Th07EffectStageAnmLoad', 'TH07 effect stage ANM load table missing');
mustContain(effectTables, 'updateCallbackAddress', 'TH07 effect update callback evidence missing');
mustContain(effectTables, 'initCallbackAddress', 'TH07 effect init callback evidence missing');
mustContain(effectTables, 'FindTh07EffectTableEntry', 'TH07 effect table lookup missing');
mustContain(effectTables, 'FindTh07EffectStageAnmLoad', 'TH07 effect stage load lookup missing');
mustContain(effectTables, 'Th07EffectSlotOffset', 'TH07 effect slot offset helper missing');
mustContain(effectTables, 'BuildTh07DedicatedEffectSpawnContract', 'TH07 dedicated effect spawn contract helper missing');
mustContain(effectTables, 'BuildTh07EffectRandomSplashMotion', 'TH07 small random splash motion helper missing');
mustContain(effectTables, 'BuildTh07EffectRandomSplashBigMotion', 'TH07 big random splash motion helper missing');
mustContain(effectTables, 'BuildTh07EffectOrbitingRenderMode2State', 'TH07 render-mode-2 orbit update helper missing');
mustContain(effectTables, 'BuildTh07EffectOrbitFadeStep', 'TH07 render-mode-2 orbit fade helper missing');
mustContain(effectTables, 'BuildTh07EffectRadialState', 'TH07 radial effect init helper missing');
mustContain(effectTables, 'BuildTh07EffectRadialInwardPosition', 'TH07 radial inward update helper missing');
mustContain(effectTables, 'BuildTh07EffectRadialOutwardPosition', 'TH07 radial outward update helper missing');
mustContain(effectTables, 'BuildTh07EffectPlayerAnchoredPosition', 'TH07 player-anchored effect update helper missing');
mustContain(effectTables, 'BuildTh07EffectPerspectiveMotionState', 'TH07 perspective motion update helper missing');
mustContain(effectTables, 'BuildTh07EffectPerspectiveInitState', 'TH07 perspective init helper missing');
mustContain(effectTables, 'BuildTh07EffectPerspectiveRgbWhiteGate', 'TH07 perspective white-gate helper missing');
mustContain(effectTables, 'BuildTh07EffectCameraSnapState', 'TH07 camera snap update helper missing');
mustContain(effectTables, 'Th07StageCameraState', 'TH07 stage camera state helper missing');
mustContain(effectTables, 'kTh07AnmVmTickTimerFunctionAddress', 'TH07 ANM VM timer tick helper missing');
mustContain(effectTables, 'kTh07AnmVmSetScriptFunctionAddress', 'TH07 ANM VM script setter evidence missing');
mustContain(effectTables, 'kTh07AnmVmSetActiveSpriteFunctionAddress', 'TH07 ANM VM active sprite setter evidence missing');
mustContain(effectTables, 'kTh07AnmVmResolveFloatOperandFunctionAddress', 'TH07 ANM VM float operand resolver missing');
mustContain(effectTables, 'kTh07AnmVmResolveIntOperandFunctionAddress', 'TH07 ANM VM int operand resolver missing');
mustContain(effectTables, 'kTh07AnmVmResolveFloatPointerFunctionAddress', 'TH07 ANM VM float pointer resolver missing');
mustContain(effectTables, 'kTh07AnmVmResolveIntPointerFunctionAddress', 'TH07 ANM VM int pointer resolver missing');
mustContain(effectTables, 'kTh07AnmVmDrawScreenQuadFunctionAddress', 'TH07 ANM VM screen draw helper evidence missing');
mustContain(effectTables, 'kTh07AnmVmDrawCameraProjectedFunctionAddress', 'TH07 ANM VM projected draw helper evidence missing');
mustContain(effectTables, 'kTh07AnmVmDrawTransformedCameraProjectedFunctionAddress', 'TH07 ANM VM transformed draw helper evidence missing');
mustContain(effectTables, 'kTh07AnmVmSubmitQuadFunctionAddress', 'TH07 ANM VM quad submit helper evidence missing');
mustContain(effectTables, 'kTh07AnmVmRotationOffset', 'TH07 ANM VM rotation offset missing');
mustContain(effectTables, 'kTh07AnmVmAngularVelocityOffset', 'TH07 ANM VM angular velocity offset missing');
mustContain(effectTables, 'kTh07AnmVmRotationXOffset', 'TH07 ANM VM rotation X offset missing');
mustContain(effectTables, 'kTh07AnmVmRotationYOffset', 'TH07 ANM VM rotation Y offset missing');
mustContain(effectTables, 'kTh07AnmVmRotationZOffset', 'TH07 ANM VM rotation Z offset missing');
mustContain(effectTables, 'kTh07AnmVmAngularVelocityXOffset', 'TH07 ANM VM angular velocity X offset missing');
mustContain(effectTables, 'kTh07AnmVmAngularVelocityYOffset', 'TH07 ANM VM angular velocity Y offset missing');
mustContain(effectTables, 'kTh07AnmVmAngularVelocityZOffset', 'TH07 ANM VM angular velocity Z offset missing');
mustContain(effectTables, 'kTh07AnmVmScaleXOffset', 'TH07 ANM VM scale-X offset missing');
mustContain(effectTables, 'kTh07AnmVmScaleYOffset', 'TH07 ANM VM scale-Y offset missing');
mustContain(effectTables, 'kTh07AnmVmScaleVelocityXOffset', 'TH07 ANM VM scale velocity-X offset missing');
mustContain(effectTables, 'kTh07AnmVmScaleVelocityYOffset', 'TH07 ANM VM scale velocity-Y offset missing');
mustContain(effectTables, 'kTh07AnmVmAngleField28Offset', 'TH07 ANM VM +0x28 angle field missing');
mustContain(effectTables, 'kTh07AnmVmAngleField2cOffset', 'TH07 ANM VM +0x2c angle field missing');
mustContain(effectTables, 'kTh07AnmVmScriptTimerCurrentOffset', 'TH07 ANM VM script timer current offset missing');
mustContain(effectTables, 'kTh07AnmVmWaitTimerPreviousOffset', 'TH07 ANM VM wait timer previous offset missing');
mustContain(effectTables, 'kTh07AnmVmWaitTimerSubframeOffset', 'TH07 ANM VM wait timer subframe offset missing');
mustContain(effectTables, 'kTh07AnmVmWaitTimerCurrentOffset', 'TH07 ANM VM wait timer current offset missing');
mustContain(effectTables, 'kTh07AnmVmPositionInterpolationModeOffset', 'TH07 ANM VM position interpolation mode missing');
mustContain(effectTables, 'kTh07AnmVmColorInterpolationModeOffset', 'TH07 ANM VM color interpolation mode missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityInterpolationModeOffset', 'TH07 ANM VM visibility interpolation mode missing');
mustContain(effectTables, 'kTh07AnmVmRotationInterpolationModeOffset', 'TH07 ANM VM rotation interpolation mode missing');
mustContain(effectTables, 'kTh07AnmVmScaleInterpolationModeOffset', 'TH07 ANM VM scale interpolation mode missing');
mustContain(effectTables, 'kTh07AnmVmAngleField28VelocityOffset', 'TH07 ANM VM +0x28 angle velocity offset missing');
mustContain(effectTables, 'kTh07AnmVmAngleField2cVelocityOffset', 'TH07 ANM VM +0x2c angle velocity offset missing');
mustContain(effectTables, 'kTh07AnmVmPositionInterpolationTimerOffset', 'TH07 ANM VM position interpolation timer missing');
mustContain(effectTables, 'kTh07AnmVmColorInterpolationTimerOffset', 'TH07 ANM VM color interpolation timer missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityInterpolationTimerOffset', 'TH07 ANM VM visibility interpolation timer missing');
mustContain(effectTables, 'kTh07AnmVmRotationInterpolationTimerOffset', 'TH07 ANM VM rotation interpolation timer missing');
mustContain(effectTables, 'kTh07AnmVmScaleInterpolationTimerOffset', 'TH07 ANM VM scale interpolation timer missing');
mustContain(effectTables, 'kTh07AnmVmPositionInterpolationDurationOffset', 'TH07 ANM VM position interpolation duration missing');
mustContain(effectTables, 'kTh07AnmVmColorInterpolationDurationOffset', 'TH07 ANM VM color interpolation duration missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityInterpolationDurationOffset', 'TH07 ANM VM visibility interpolation duration missing');
mustContain(effectTables, 'kTh07AnmVmRotationInterpolationDurationOffset', 'TH07 ANM VM rotation interpolation duration missing');
mustContain(effectTables, 'kTh07AnmVmScaleInterpolationDurationOffset', 'TH07 ANM VM scale interpolation duration missing');
mustContain(effectTables, 'kTh07AnmVmColorOffset', 'TH07 ANM VM color offset missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityByteOffset', 'TH07 ANM VM visibility byte offset missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagsOffset', 'TH07 ANM VM render flags offset missing');
mustContain(effectTables, 'kTh07AnmVmField1c4Offset', 'TH07 ANM VM +0x1c4 field missing');
mustContain(effectTables, 'kTh07AnmVmPendingInterruptOffset', 'TH07 ANM VM pending interrupt offset missing');
mustContain(effectTables, 'kTh07AnmVmBeginningOfScriptOffset', 'TH07 ANM VM script begin offset missing');
mustContain(effectTables, 'kTh07AnmVmCurrentInstructionOffset', 'TH07 ANM VM current instruction offset missing');
mustContain(effectTables, 'kTh07AnmVmLoadedSpriteOffset', 'TH07 ANM VM loaded sprite offset missing');
mustContain(effectTables, 'kTh07AnmVmPositionInterpolationInitialOffset', 'TH07 ANM VM position interpolation initial vector missing');
mustContain(effectTables, 'kTh07AnmVmPositionInterpolationTargetOffset', 'TH07 ANM VM position interpolation target vector missing');
mustContain(effectTables, 'kTh07AnmVmRotationInterpolationInitialOffset', 'TH07 ANM VM rotation interpolation initial vector missing');
mustContain(effectTables, 'kTh07AnmVmRotationInterpolationTargetOffset', 'TH07 ANM VM rotation interpolation target vector missing');
mustContain(effectTables, 'kTh07AnmVmScaleInterpolationInitialXOffset', 'TH07 ANM VM scale interpolation initial X missing');
mustContain(effectTables, 'kTh07AnmVmScaleInterpolationFinalXOffset', 'TH07 ANM VM scale interpolation final X missing');
mustContain(effectTables, 'kTh07AnmVmColorInterpolationInitialOffset', 'TH07 ANM VM color interpolation initial bytes missing');
mustContain(effectTables, 'kTh07AnmVmColorInterpolationTargetOffset', 'TH07 ANM VM color interpolation target bytes missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityInterpolationInitialOffset', 'TH07 ANM VM visibility interpolation initial byte missing');
mustContain(effectTables, 'kTh07AnmVmVisibilityInterpolationTargetOffset', 'TH07 ANM VM visibility interpolation target byte missing');
mustContain(effectTables, 'kTh07AnmVmLastSpriteSetFrameOffset', 'TH07 ANM VM active-sprite frame offset missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagActive', 'TH07 ANM VM active flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagDrawable', 'TH07 ANM VM drawable flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagRotationDirty', 'TH07 ANM VM rotation dirty flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagScaleDirty', 'TH07 ANM VM scale dirty flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagBit4', 'TH07 ANM VM controlled bit-4 flag missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagPositionTargetsAux', 'TH07 ANM VM aux target flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagAnchorX', 'TH07 ANM VM anchor-X flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagAnchorY', 'TH07 ANM VM anchor-Y flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagAnchorTopLeft', 'TH07 ANM VM top-left anchor flag pair missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagBit12', 'TH07 ANM VM controlled bit-12 flag missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagInterruptPending', 'TH07 ANM VM interrupt pending flag bit missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagBit14', 'TH07 ANM VM controlled bit-14 flag missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagSkipTransformRebuild', 'TH07 ANM VM transform rebuild skip flag missing');
mustContain(effectTables, 'kTh07AnmVmRenderFlagUseAlternateColor', 'TH07 ANM VM alternate color flag bit missing');
mustContain(effectTables, 'kTh07AnmVmInterruptLabelOpcode', 'TH07 ANM VM interrupt label opcode missing');
mustContain(effectTables, 'kTh07AnmVmFallbackInterruptLabel', 'TH07 ANM VM fallback interrupt label missing');
mustContain(effectTables, 'kTh07AnmVmInterpolationModeFromInstruction', 'TH07 ANM VM dynamic interpolation mode sentinel missing');
mustContain(effectTables, 'kTh07AnmVmOperandFlagArg0IsReference', 'TH07 ANM VM arg0 reference flag missing');
mustContain(effectTables, 'kTh07AnmVmOperandFlagArg1IsReference', 'TH07 ANM VM arg1 reference flag missing');
mustContain(effectTables, 'kTh07AnmVmOperandFlagArg2IsReference', 'TH07 ANM VM arg2 reference flag missing');
mustContain(effectTables, 'kTh07AnmVmInstructionOpcodeOffset = 0x00', 'TH07 ANM VM instruction opcode offset missing');
mustContain(effectTables, 'kTh07AnmVmInstructionSizeOffset = 0x02', 'TH07 ANM VM instruction size offset missing');
mustContain(effectTables, 'kTh07AnmVmInstructionTimeOffset = 0x04', 'TH07 ANM VM instruction time offset missing');
mustContain(effectTables, 'kTh07AnmVmInstructionOperandFlagsOffset = 0x06', 'TH07 ANM VM operand-flags offset missing');
mustContain(effectTables, 'kTh07AnmVmInstructionOperandsOffset = 0x08', 'TH07 ANM VM operands offset missing');
mustContain(effectTables, 'kTh07AnmVmInstructionHeaderSize = 0x08', 'TH07 ANM VM instruction header size missing');
mustContain(effectTables, 'kTh07AnmVmVariableIdBase = 10000', 'TH07 ANM VM variable-id base missing');
mustContain(effectTables, 'kTh07AnmVmIntVariable0Id = 10000', 'TH07 ANM VM int variable 0 id missing');
mustContain(effectTables, 'kTh07AnmVmFloatVariable0Id = 0x2714', 'TH07 ANM VM float variable 0 id missing');
mustContain(effectTables, 'kTh07AnmVmIntVariable5Id = 0x2719', 'TH07 ANM VM int variable 5 id missing');
mustContain(effectTables, 'kTh07AnmVmIntVariable0Offset = 0x00c8', 'TH07 ANM VM int variable 0 offset missing');
mustContain(effectTables, 'kTh07AnmVmFloatVariable0Offset = 0x00d8', 'TH07 ANM VM float variable 0 offset missing');
mustContain(effectTables, 'kTh07AnmVmIntVariable5Offset = 0x00ec', 'TH07 ANM VM int variable 5 offset missing');
mustContain(effectTables, 'kTh07AnmVmBaseTransformMatrixOffset = 0x00f8', 'TH07 ANM VM base transform matrix offset missing');
mustContain(effectTables, 'kTh07AnmVmActiveTransformMatrixOffset = 0x0138', 'TH07 ANM VM active transform matrix offset missing');
mustContain(effectTables, 'kTh07AnmVmVariableSlotCount = 10', 'TH07 ANM VM variable slot count missing');
mustContain(effectTables, 'Th07AnmVmVariableSlot', 'TH07 ANM VM variable slot metadata missing');
mustContain(effectTables, 'kTh07AnmVmVariableSlots', 'TH07 ANM VM variable slot table missing');
mustContain(effectTables, 'FindTh07AnmVmVariableSlot', 'TH07 ANM VM variable slot lookup missing');
mustContain(effectTables, 'Th07AnmVmInstructionHeader', 'TH07 ANM VM instruction header type missing');
mustContain(effectTables, 'ReadTh07AnmVmInstructionHeader', 'TH07 ANM VM instruction header reader missing');
mustContain(effectTables, 'Th07AnmVmInstructionNextOffset', 'TH07 ANM VM instruction advance helper missing');
mustContain(effectTables, 'Th07AnmVmInstructionOperandOffset', 'TH07 ANM VM instruction operand offset helper missing');
mustContain(effectTables, 'TruncateTh07AnmVmFloatToI32', 'TH07 ANM VM float-to-int helper missing');
mustContain(effectTables, 'ReadTh07AnmVmInstructionI32Operand', 'TH07 ANM VM integer operand reader missing');
mustContain(effectTables, 'ReadTh07AnmVmInstructionF32Operand', 'TH07 ANM VM float operand reader missing');
mustContain(effectTables, 'kTh07AnmVmRandomU32FunctionAddress', 'TH07 ANM VM random U32 helper missing');
mustContain(effectTables, 'kTh07AnmVmRandomFloatZeroToOneFunctionAddress', 'TH07 ANM VM random float helper missing');
mustContain(effectTables, 'kTh07AnmVmNormalizeAngleFunctionAddress', 'TH07 ANM VM normalize angle helper missing');
mustContain(effectTables, 'kTh07AnmVmFloatToIntFunctionAddress', 'TH07 ANM VM float-to-int address missing');
mustContain(effectTables, 'kTh07AnmVmRoundFloatToByteFunctionAddress', 'TH07 ANM VM float-to-byte helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0048bb0aAddress', 'TH07 ANM VM 0048bb0a math helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0048bb40Address', 'TH07 ANM VM 0048bb40 math helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0048bbf0Address', 'TH07 ANM VM 0048bbf0 math helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0048b920Address', 'TH07 ANM VM 0048b920 math helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0047eca0Address', 'TH07 ANM VM 0047eca0 math helper missing');
mustContain(effectTables, 'kTh07AnmVmMathHelper0048ba20Address', 'TH07 ANM VM 0048ba20 math helper missing');
mustContain(effectTables, 'Th07AnmVmRenderHelperContract', 'TH07 ANM VM render helper contract missing');
mustContain(effectTables, 'FindTh07AnmVmRenderHelper', 'TH07 ANM VM render helper lookup missing');
mustContain(effectTables, 'Th07AnmVmExecuteOpcodeContract', 'TH07 ANM VM execute opcode contract missing');
mustContain(effectTables, 'FindTh07AnmVmExecuteOpcode', 'TH07 ANM VM execute opcode lookup missing');
mustContain(effectTables, 'Th07AnmVmExecuteOpcodeSemantic::Loop', 'TH07 ANM VM loop opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmFollowupOpcodeContract', 'TH07 ANM VM followup opcode contract missing');
mustContain(effectTables, 'kTh07AnmVmFollowupExecuteOpcodes', 'TH07 ANM VM followup opcode table missing');
mustContain(effectTables, 'FindTh07AnmVmFollowupExecuteOpcode', 'TH07 ANM VM followup opcode lookup missing');
mustContain(effectTables, 'StartPositionInterpolation', 'TH07 ANM VM position interpolation opcode semantic missing');
mustContain(effectTables, 'StartVisibilityInterpolation', 'TH07 ANM VM visibility interpolation opcode semantic missing');
mustContain(effectTables, 'StartColorInterpolation', 'TH07 ANM VM color interpolation opcode semantic missing');
mustContain(effectTables, 'StartRotationInterpolation', 'TH07 ANM VM rotation interpolation opcode semantic missing');
mustContain(effectTables, 'StartScaleInterpolation', 'TH07 ANM VM scale interpolation opcode semantic missing');
mustContain(effectTables, 'StopAndClearActiveUntilInterrupt', 'TH07 ANM VM stop-clear opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeContract', 'TH07 ANM VM variable opcode contract missing');
mustContain(effectTables, 'kTh07AnmVmVariableExecuteOpcodes', 'TH07 ANM VM variable opcode table missing');
mustContain(effectTables, 'FindTh07AnmVmVariableExecuteOpcode', 'TH07 ANM VM variable opcode lookup missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeSemantic::AddAssign', 'TH07 ANM VM add-assign opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeSemantic::AddOperands', 'TH07 ANM VM add-operands opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeSemantic::RandomIntModulo', 'TH07 ANM VM random-int opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeSemantic::RandomFloatScaled', 'TH07 ANM VM random-float opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmVariableOpcodeSemantic::NormalizeAngle', 'TH07 ANM VM normalize-angle opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmConditionalJumpOpcodeContract', 'TH07 ANM VM conditional jump opcode contract missing');
mustContain(effectTables, 'kTh07AnmVmConditionalJumpExecuteOpcodes', 'TH07 ANM VM conditional jump opcode table missing');
mustContain(effectTables, 'FindTh07AnmVmConditionalJumpExecuteOpcode', 'TH07 ANM VM conditional jump opcode lookup missing');
mustContain(effectTables, 'Th07AnmVmConditionalJumpComparison::Equal', 'TH07 ANM VM conditional equal semantic missing');
mustContain(effectTables, 'Th07AnmVmConditionalJumpComparison::LessOrEqual', 'TH07 ANM VM conditional less-or-equal semantic missing');
mustContain(effectTables, 'Th07AnmVmConditionalJumpComparison::GreaterOrEqual', 'TH07 ANM VM conditional greater-or-equal semantic missing');
mustContain(effectTables, 'Th07AnmVmControlOpcodeContract', 'TH07 ANM VM control opcode contract missing');
mustContain(effectTables, 'kTh07AnmVmControlExecuteOpcodes', 'TH07 ANM VM control opcode table missing');
mustContain(effectTables, 'FindTh07AnmVmControlExecuteOpcode', 'TH07 ANM VM control opcode lookup missing');
mustContain(effectTables, 'Th07AnmVmControlOpcodeSemantic::WaitTimer', 'TH07 ANM VM wait timer opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmControlOpcodeSemantic::SetAngleField28Velocity', 'TH07 ANM VM +0x28 velocity opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmControlOpcodeSemantic::SetAngleField2cVelocity', 'TH07 ANM VM +0x2c velocity opcode semantic missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionContract', 'TH07 ANM VM post-step action contract missing');
mustContain(effectTables, 'timerHelperFunctionAddress', 'TH07 ANM VM post-step timer helper field missing');
mustContain(effectTables, 'valueHelperFunctionAddress', 'TH07 ANM VM post-step value helper field missing');
mustContain(effectTables, 'kTh07AnmVmPostStepActions', 'TH07 ANM VM post-step action table missing');
mustContain(effectTables, 'FindTh07AnmVmPostStepAction', 'TH07 ANM VM post-step action lookup missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionSemantic::ApplyAngularVelocity', 'TH07 ANM VM angular velocity post-step missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane', 'TH07 ANM VM interpolation post-step missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionSemantic::ApplyScaleVelocity', 'TH07 ANM VM scale velocity post-step missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionSemantic::ApplyAngleFieldVelocity', 'TH07 ANM VM angle-field velocity post-step missing');
mustContain(effectTables, 'Th07AnmVmPostStepActionSemantic::AdvanceScriptTimer', 'TH07 ANM VM script timer post-step missing');
mustContain(effectTables, 'Th07StageCameraRuntimeState', 'TH07 stage camera runtime state missing');
mustContain(effectTables, 'Th07StageCameraInstruction', 'TH07 stage camera instruction wrapper missing');
mustContain(effectTables, 'Th07StageScriptRuntimeState', 'TH07 stage script runtime state missing');
mustContain(effectTables, 'Th07StageScriptInstruction', 'TH07 stage script instruction wrapper missing');
mustContain(effectTables, 'Th07StageFogState', 'TH07 stage fog state missing');
mustContain(effectTables, 'Th07StageScriptStepResult', 'TH07 stage script step result missing');
mustContain(effectTables, 'Th07StageAnmVmOpcodeEffect', 'TH07 stage ANM VM opcode effect missing');
mustContain(effectTables, 'BuildTh07StageAnmVmOpcodeEffect', 'TH07 stage ANM VM opcode helper missing');
mustContain(effectTables, 'BuildTh07InitialStageCameraState', 'TH07 initial stage camera helper missing');
mustContain(effectTables, 'BuildTh07InitialStageCameraRuntimeState', 'TH07 initial stage camera runtime helper missing');
mustContain(effectTables, 'BuildTh07StageCameraSetupState', 'TH07 stage camera setup helper missing');
mustContain(effectTables, 'BuildTh07EffectCameraBasis', 'TH07 effect camera basis helper missing');
mustContain(effectTables, 'ApplyTh07StageCameraOpcode', 'TH07 stage camera opcode apply helper missing');
mustContain(effectTables, 'ApplyTh07StageScriptOpcode', 'TH07 non-camera stage script opcode helper missing');
mustContain(effectTables, 'AdvanceTh07StageFogInterpolation', 'TH07 stage fog interpolation helper missing');
mustContain(effectTables, 'IsTh07StageNonCameraOpcode', 'TH07 non-camera stage opcode classifier missing');
mustContain(effectTables, 'IsTh07StageScriptInstructionDue', 'TH07 stage script due helper missing');
mustContain(effectTables, 'AdvanceTh07StageCameraRuntimeState', 'TH07 stage camera runtime advance helper missing');
mustContain(effectTables, 'Th07StageCameraVectorChannelLayout', 'TH07 stage camera vector-channel layout missing');
mustContain(effectTables, 'Th07StageCameraOpcodeLayout', 'TH07 stage camera opcode layout missing');
mustContain(effectTables, 'FindTh07StageCameraVectorChannel', 'TH07 stage camera vector-channel lookup missing');
mustContain(effectTables, 'FindTh07StageCameraOpcode', 'TH07 stage camera opcode lookup missing');
mustContain(effectTables, 'ApplyTh07StageCameraInterpolationCurve', 'TH07 stage camera interpolation curve helper missing');
mustContain(effectTables, 'BuildTh07StageCameraCurveComponent', 'TH07 stage camera curve helper missing');
mustContain(effectTables, 'BuildTh07StageCameraInterpolatedVector', 'TH07 stage camera vector interpolation helper missing');
mustContain(effectTables, 'BuildTh07StageCameraInterpolatedScalar', 'TH07 stage camera scalar interpolation helper missing');
mustContain(effectTables, 'kTh07StageCameraOpcodes', 'TH07 stage camera opcode table missing');
mustContain(effectTables, 'kTh07CameraPositionGlobalAddress', 'TH07 camera position global evidence missing');
mustContain(effectTables, 'kTh07CameraOffsetGlobalAddress', 'TH07 camera offset global evidence missing');
mustContain(effectTables, 'kTh07CameraForwardGlobalAddress', 'TH07 camera forward global evidence missing');
mustContain(effectTables, 'kTh07StageObjectAddress', 'TH07 stage object global evidence missing');
mustContain(effectTables, 'kTh07StageObjectClearDwordCount', 'TH07 stage object clear size evidence missing');
mustContain(effectTables, 'kTh07StageSetAnmVmScriptFunctionAddress', 'TH07 stage ANM VM script setter evidence missing');
mustContain(effectTables, 'kTh07StageAnmManagerColorWriteFunctionAddress', 'TH07 stage ANM manager color writer evidence missing');
mustContain(effectTables, 'kTh07StageAnmManagerColorMultiplierOffset', 'TH07 stage ANM manager color multiplier offset missing');
mustContain(effectTables, 'kTh07StageAnmManagerColorResetGuardOffset', 'TH07 stage ANM manager color reset guard offset missing');
mustContain(effectTables, 'kTh07StageAnmManagerColorMultiplierIdle', 'TH07 stage ANM manager idle color missing');
mustContain(effectTables, 'kTh07GameManagerObjectAddress', 'TH07 GameManager object address evidence missing');
mustContain(effectTables, 'kTh07GameManagerFrameCounterOffset', 'TH07 GameManager frame counter offset evidence missing');
mustContain(effectTables, 'kTh07GameManagerFrameCounterGlobalAddress', 'TH07 GameManager frame counter global evidence missing');
mustContain(effectTables, 'kTh07ScreenEffectShakeFunctionAddress', 'TH07 screen shake function address missing');
mustContain(effectTables, 'kTh07ScreenShakeRequiredGameFrameCount', 'TH07 screen shake frame gate missing');
mustContain(effectTables, 'kTh07StageFirstAnmVmOffset', 'TH07 first stage ANM VM offset missing');
mustContain(effectTables, 'kTh07StageSecondAnmVmOffset', 'TH07 second stage ANM VM offset missing');
mustContain(effectTables, 'kTh07StageAnmVmSlotSize', 'TH07 stage ANM VM slot size missing');
mustContain(effectTables, 'kTh07StageAnmVmActiveSpriteIndexOffset', 'TH07 stage ANM VM active-sprite offset missing');
mustContain(effectTables, 'kTh07StageAnmVmScriptIndexOffset', 'TH07 stage ANM VM script index offset missing');
mustContain(effectTables, 'kTh07StageCameraPositionOffset', 'TH07 stage camera position offset evidence missing');
mustContain(effectTables, 'kTh07StageCameraForwardOffset', 'TH07 stage camera forward offset evidence missing');
mustContain(effectTables, 'kTh07StageCameraSideVectorOffset', 'TH07 stage camera side-vector offset evidence missing');
mustContain(effectTables, 'kTh07StageAnmManagerColorResetGuardGlobalAddress', 'TH07 stage color reset guard global address missing');
mustContain(effectTables, 'Th07EffectPerspectiveInitKind::Init0041b0b0', 'TH07 0041b0b0 perspective init variant missing');
mustContain(effectTables, 'Th07EffectPerspectiveInitKind::Init0041b4a0', 'TH07 0041b4a0 perspective init variant missing');
mustContain(effectTables, 'Th07EffectPerspectiveInitKind::Init0041b770', 'TH07 0041b770 perspective init variant missing');
mustContain(effectTables, 'Th07EffectPerspectiveInitKind::Init0041b9f0', 'TH07 0041b9f0 perspective init variant missing');
mustContain(effectTables, 'Th07EffectPerspectiveInitKind::Init0041bc20', 'TH07 0041bc20 perspective init variant missing');
mustContain(effectTables, 'NormalizeTh07EffectAngle', 'TH07 eased radial angle normalization helper missing');
mustContain(effectTables, 'BuildTh07EffectEasedRadialStateFromAngle', 'TH07 eased radial provided-angle init helper missing');
mustContain(effectTables, 'BuildTh07EffectEasedRadialStateFromRandomAngle', 'TH07 eased radial random-angle init helper missing');
mustContain(effectTables, 'BuildTh07EffectEasedRadialPosition', 'TH07 eased radial update helper missing');
mustEqual(countMatches(effectTables, /\{0x02[a-f0-9]{2},/g), 33, 'TH07 effect table script entry count');
for (const value of [
  '0x0049efc0',
  '0x012fe250',
  '0x0041c1c0',
  '0x0041c400',
  '0x0041c610',
  '0x0041c790',
  '0x0041ca10',
  '0x0041cb80',
  '0x0041cde0',
  '0x0041d050',
  '0x0041a370',
  '0x0041a4f0',
  '0x0041a5a0',
  '0x0041a730',
  '0x0041a750',
  '0x0041aa60',
  '0x0041aaf0',
  '0x0041abe0',
  '0x0041ac30',
  '0x0041ad10',
  '0x0041aef0',
  '0x0041b0b0',
  '0x0041b4a0',
  '0x0041b770',
  '0x0041b9f0',
  '0x0041bc20',
  '256.0f',
  '128.0f',
  '12.0f',
  '19.0f',
  '33.0f',
  '20.0f',
  '6.2831855f',
  '3.1415927f',
  '60.0f',
  '240.0f',
  '30.0f',
  '0.00001f',
  '6.0f',
  '16.0f',
  '255.0f',
  '0.94f',
  '0x4',
  '0x02d8',
  '100',
  '0.5f',
  '160.0f',
  '80.0f',
  '120.0f',
  '0.06f',
  '0.03f',
  '0.09f',
  '0.2f',
  '0.02f',
  '0.0002f',
  '0.0001f',
  '-0.015f',
  '0.062831856f',
  '0.031415928f',
  '0x1e0',
  '-990.0f',
  '1.5f',
  '90.0f',
  '0x0041bec0',
  '0x0041bfd0',
  '0x0041c100',
  '0x00408180',
  '0x004082b0',
  '0x0044e8e0',
  '0x0044ea20',
  '0x00450a50',
  '0x00450b20',
  '0x00450c10',
  '0x00450ca0',
  '0x0044efb0',
  '0x0044f770',
  '0x0044f9a0',
  '0x0044fe00',
  '0x00450130',
  '0x004501a0',
  '0x004504b0',
  '0x00406930',
  '0x004318d0',
  '0x00431900',
  '0x00431930',
  '0x0047eca0',
  '0x0048b920',
  '0x0048ba20',
  '0x0048bb0a',
  '0x0048bb40',
  '0x0048bbf0',
  'EndScript',
  'SetActiveSprite',
  'SetDrawPositionOrAux',
  'SetAngularVelocity',
  '0x01347b00',
  '0x14ad',
  '0x52b4',
  '0x0134ccf8',
  '0x0134cd04',
  '0x0134cd1c',
  '0x0134cd28',
  '0x0134cdb0',
  '0x51f8',
  '0x5204',
  '0x5210',
  '0x521c',
  '0x5228',
  '0x5234',
  '0x50f8',
  '0x5104',
  '0x5110',
  '0x5134',
  '0x5138',
  '0x5144',
  '0x5150',
  '0x5174',
  '0x5178',
  '0x5184',
  '0x5190',
  '0x51b8',
  '0x51c4',
  '0x51d0',
  '0x5238',
  '0x523c',
  '0x5240',
  '0x5244',
  '0x5248',
  '0x5254',
  '0x5260',
  '0x526c',
  '0x5278',
  '0x527c',
  '0x5280',
  '0x5284',
  '0x49c',
  '0x4a0',
  '0x4a4',
  '0x4a8',
  '0x4ac',
  '0x4b0',
  '0x4b4',
  '0x4c0',
  '0x4c4',
  '0x4cc',
  '0x4d8',
  '0x4dc',
  '0x4e0',
  '0x4e4',
  '0x4e8',
  '0x4f4',
  '0x500',
  '0x504',
  '0x5288',
  '0x5294',
  '0x5298',
  '0x52a4',
  '0x52a8',
  '0x52ac',
  '0x52b0',
  '0x00808080',
  '0x447a0000',
  '0x3f800000',
  '0x3f060a92',
  '0x0041c1b0',
  '0x0304',
  '0x02da',
  '0x01b8',
  '0x01bb',
  '0x01bc',
  '0x01c0',
  '0x01c4',
  '0x01c6',
  '0x01dc',
  '0x01e0',
  '0x01e4',
  '0x01e8',
  '0x01f4',
  '0x0200',
  '0x020c',
  '0x0218',
  '0x0220',
  '0x0228',
  '0x022b',
  '0x022c',
  '0x022f',
  '0x00c0',
  '0x00c1',
  '0x00c2',
  '0x00c3',
  '0x00c4',
  '0x0054',
  '0x006c',
  '0x008c',
  '0x0098',
  '0x00a4',
  '0x00b0',
  '0x00bc',
  '0xc00',
  '0x1000',
  '0x2000',
  '0x4000',
  'data/eff01.anm',
  'data/eff02.anm',
  'data/eff03.anm',
  'data/eff04.anm',
  'data/eff04b.anm',
  'data/eff05.anm',
  'data/eff06.anm',
  'data/eff07.anm',
  'data/eff08.anm',
  '0x004986a4',
  '0x00498624',
]) {
  mustContain(effectTables, value, `TH07 effect table missing ${value}`);
}

const effectRuntime = `${read('Effect.hpp')}\n${read('EffectManager.hpp')}\n${read('EffectManager.cpp')}`;
for (const value of [
  'ZUN_ASSERT_SIZE(Effect, 0x2d8)',
  'ZUN_ASSERT_SIZE(EffectInfo, 0xc)',
  'ZUN_ASSERT_SIZE(EffectManager, 0x496a8)',
  'PARTICLE_EFFECT_BOMB_COMMON',
  'Effect effects[408]',
  'Effect fallbackEffect',
  'drawListScrollSentinel',
  'drawListIndexedSentinel',
  'drawListNoScrollSentinel',
  'drawListScrollFlaggedSentinel',
  'static_assert(offsetof(EffectManager, effects) == 0x1c)',
  'static_assert(offsetof(EffectManager, fallbackEffect) == 0x4885c)',
  'static_assert(offsetof(EffectManager, drawListScrollTail) == 0x49694)',
  'static_assert(offsetof(EffectManager, frameCounter) == 0x496a4)',
  'DIFFABLE_STATIC_ARRAY_ASSIGN(EffectInfo, 34, g_Effects)',
  'SpawnParticlesWithAuxVector',
  'SpawnDedicatedParticle',
  'kTh07EffectPrimarySlotCount',
  'kTh07EffectDedicatedSlotBaseIndex',
  'kTh07EffectUpdateSlotCount',
  'EffectInitRandomSplash',
  'EffectInitRandomSplashBig',
  'EffectInitRenderMode2',
  'EffectCallbackOrbitingRenderMode2',
  'EffectInitRadialDirection',
  'EffectCallbackRadialInwardShort',
  'EffectCallbackRadialInwardSlow',
  'EffectCallbackRadialOutward',
  'EffectCallbackPlayerPositionWhileAnmActive',
  'EffectCallbackPerspectiveMotion',
  'EffectInitPerspective',
  'EffectCallbackCameraSnap',
  'EffectInitEasedRadial',
  'EffectCallbackEasedRadial',
  'BuildTh07EffectRandomSplashMotion',
  'BuildTh07EffectRandomSplashBigMotion',
  'BuildTh07EffectOrbitingRenderMode2State',
  'BuildTh07EffectOrbitFadeStep',
  'BuildTh07EffectRadialState',
  'BuildTh07EffectRadialInwardPosition',
  'BuildTh07EffectRadialOutwardPosition',
  'BuildTh07EffectPlayerAnchoredPosition',
  'BuildTh07EffectPerspectiveMotionState',
  'BuildTh07EffectPerspectiveInitState',
  'BuildTh07EffectCameraSnapState',
  'BuildTh07EffectEasedRadialStateFromAngle',
  'BuildTh07EffectEasedRadialPosition',
  'BuildCurrentPerspectiveCameraBasis',
  'g_Stage.th07CameraCurrentBasis',
  'g_AnmManager->DrawTh07ScreenQuad(effect)',
  'g_AnmManager->DrawTh07CameraProjected(effect)',
  'g_AnmManager->DrawTh07TransformedCameraProjected(effect)',
  'Th07EffectPerspectiveInitKind::Init0041b0b0',
  'Th07EffectPerspectiveInitKind::Init0041b4a0',
  'Th07EffectPerspectiveInitKind::Init0041b770',
  'Th07EffectPerspectiveInitKind::Init0041b9f0',
  'Th07EffectPerspectiveInitKind::Init0041bc20',
  'kTh07EffectPerspectiveWhiteAnmScript',
  'vm.scaleX',
  'FindTh07EffectStageAnmLoad',
  'ReleaseAnm(0x11)',
  'ReleaseAnm(0x12)',
  'ReleaseAnm(0x13)',
  'ReleaseAnm(0x14)',
]) {
  mustContain(effectRuntime, value, `TH07 effect runtime skeleton missing ${value}`);
}
mustNotContain(effectRuntime, 'DIFFABLE_STATIC_ARRAY_ASSIGN(EffectInfo, 20, g_Effects)', 'TH06 20-entry effect table still present');
mustNotContain(effectRuntime, 'ANM_SCRIPT_BULLET4_SPAWN_BUBBLE_EXPLOSION_SMALL', 'TH06 effect script macro still drives EffectManager table');
mustNotContain(effectRuntime, 'ZUN_ASSERT_SIZE(Effect, 0x17c)', 'TH06 effect slot size still present');
mustNotContain(effectRuntime, 'ZUN_ASSERT_SIZE(EffectManager, 0x2f984)', 'TH06 effect manager size still present');

const cherryHeader = read('CherrySystem.hpp');
const cherry = read('CherrySystem.cpp');
mustContain(cherry, 'CherryPoint Max!', 'Cherry HUD anchor missing');
for (const value of [
  'kTh07CherryPointMaxStringAddress = 0x004983b4',
  'kTh07CherryValueStringAddress = 0x004984cc',
  'kTh07CherryHudStringAnchorCount = 2',
  'kTh07CherryManagerAddress = 0x00626270',
  'kTh07CherryPrimaryGainFunctionAddress = 0x0042f5a2',
  'kTh07CherrySecondaryGainFunctionAddress = 0x0042f69f',
  'kTh07CherryEclOpcode160CallAddress = 0x00416722',
  'kTh07CherryCapOffset = 0x9618',
  'kTh07CherryCurrentOffset = 0x961c',
  'kTh07CherryPlusOffset = 0x9620',
  'kTh07CherryPlayerThresholdOffset = 0x0088',
  'kTh07CherryDisableFlagAddress = 0x004bfee5',
  'kTh07CherryBorderStartFunctionAddress = 0x00441960',
  'kTh07CherryToastRegisterFunctionAddress = 0x00427c81',
  'kTh07CherryGainHelperCount = 2',
  'CherryGainHelperContract',
]) {
  mustContain(cherryHeader, value, `TH07 Cherry metadata missing ${value}`);
}
mustContain(cherry, 'kTh07CherryPointMaxStringAddress', 'Cherry HUD anchor should use typed string address');
mustContain(cherry, 'kTh07CherryGainHelperContracts', 'Cherry gain helper contracts missing');
mustContain(cherry, 'CherryGainHelperKind::Primary', 'Primary Cherry gain helper missing');
mustContain(cherry, 'CherryGainHelperKind::Secondary', 'Secondary Cherry gain helper missing');
mustContain(cherry, '"0042f5a2:8-23"', 'Primary Cherry gain helper evidence missing');
mustContain(cherry, '"0042f69f:8-17"', 'Secondary Cherry gain helper evidence missing');
mustNotContain(cherry, '"004983b4"', 'Cherry HUD anchor still uses stringly typed address');

const borderHeader = read('BorderSystem.hpp');
const border = read('BorderSystem.cpp');
mustContain(border, 'Supernatural Border!!', 'Border HUD anchor missing');
mustContain(border, 'Phantasm Rank*2.0', 'Phantasm rank HUD anchor missing');
for (const value of [
  'kTh07BorderBonusStringAddress = 0x004983a0',
  'kTh07SupernaturalBorderStringAddress = 0x004983c8',
  'kTh07PlayerPenaltyLowStringAddress = 0x00498410',
  'kTh07PlayerPenaltyHalfStringAddress = 0x00498424',
  'kTh07PhantasmRankStringAddress = 0x00498438',
  'kTh07BorderHudStringAnchorCount = 5',
]) {
  mustContain(borderHeader, value, `TH07 Border HUD anchor metadata missing ${value}`);
}
mustContain(border, 'kTh07SupernaturalBorderStringAddress', 'Border HUD anchor should use typed string address');
mustNotContain(border, '"004983c8"', 'Border HUD anchor still uses stringly typed address');

const clearBonus = read('ClearBonusSystem.cpp');
for (const value of ['Easy Rank    *0.5', 'Normal Rank  *1.0', 'Hard Rank    *1.2', 'Lunatic Rank *1.5', 'Extra Rank   *2.0', 'Phantasm Rank*2.0']) {
  mustContain(clearBonus, value, `TH07 clear bonus rank rule missing ${value}`);
}
for (const value of ['Full Power Mode!', 'Supernatural Border!!', 'CherryPoint Max!', 'Border Bonus %7d']) {
  mustContain(clearBonus, value, `TH07 bonus toast rule missing ${value}`);
}
const clearBonusHeader = read('ClearBonusSystem.hpp');
for (const value of [
  'kTh07ClearBonusFunctionAddress = 0x00427f22',
  'kTh07StageClearBonusInstructionAddress = 0x00427fc2',
  'kTh07PointItemClearBonusInstructionAddress = 0x00428001',
  'kTh07GrazeClearBonusInstructionAddress = 0x00428040',
  'kTh07CherryClearBonusInstructionAddress = 0x0042807f',
  'kTh07LifeClearBonusInstructionAddress = 0x00428103',
  'kTh07BombClearBonusInstructionAddress = 0x00428143',
  'kTh07ClearBonusSummaryVisibleCheckAddress = 0x00427f45',
  'kTh07ClearBonusPointItemsLoadAddress = 0x00427ffb',
  'kTh07ClearBonusGrazeLoadAddress = 0x0042803a',
  'kTh07ClearBonusCherryLoadAddress = 0x00428079',
  'kTh07ClearBonusTotalScorePushAddress = 0x0042831a',
  'kTh07BonusPopupVisibleCheckAddress = 0x00428368',
  'kTh07BonusPopupScorePushAddress = 0x00428381',
  'kTh07BonusPopupPositionAddAddress = 0x00428392',
  'kTh07BonusToastKindLoadAddress = 0x004283b2',
  'kTh07BonusToastRegisterFunctionAddress = 0x00427c81',
  'kTh07BonusToastRegisterPositionXLoadAddress = 0x00427c8c',
  'kTh07BonusToastRegisterPositionYLoadAddress = 0x00427c95',
  'kTh07BonusToastRegisterPositionAddAddress = 0x00427ca9',
  'kTh07BonusToastRegisterKindWriteAddress = 0x00427cbe',
  'kTh07BonusToastRegisterTimerAddAddress = 0x00427cca',
  'kTh07BonusToastRegisterTimerCurrentClearAddress = 0x00427cd5',
  'kTh07BonusToastRegisterTimerSubframeClearAddress = 0x00427cdc',
  'kTh07BonusToastRegisterTimerPreviousWriteAddress = 0x00427ce4',
  'kTh07BonusToastRegisterScoreWriteAddress = 0x00427cf3',
  'kTh07BonusToastRegisterGlobalStateWriteAddress = 0x00427cf9',
  'kTh07BonusToastCherryPointMaxPrimaryProducerFunctionAddress = 0x0042f5a2',
  'kTh07BonusToastCherryPointMaxPrimaryKindPushAddress = 0x0042f679',
  'kTh07BonusToastCherryPointMaxPrimaryScorePushAddress = 0x0042f690',
  'kTh07BonusToastCherryPointMaxPrimaryCallAddress = 0x0042f696',
  'kTh07BonusToastCherryPointMaxSecondaryProducerFunctionAddress = 0x0042f69f',
  'kTh07BonusToastCherryPointMaxSecondaryKindPushAddress = 0x0042f710',
  'kTh07BonusToastCherryPointMaxSecondaryScorePushAddress = 0x0042f727',
  'kTh07BonusToastCherryPointMaxSecondaryCallAddress = 0x0042f72d',
  'kTh07BonusToastFullPowerItemProducerFunctionAddress = 0x00432990',
  'kTh07BonusToastFullPowerItemCase0KindPushAddress = 0x00433006',
  'kTh07BonusToastFullPowerItemCase0ScorePushAddress = 0x00433008',
  'kTh07BonusToastFullPowerItemCase0CallAddress = 0x0043300f',
  'kTh07BonusToastFullPowerItemCase0AltKindPushAddress = 0x004334b2',
  'kTh07BonusToastFullPowerItemCase0AltScorePushAddress = 0x004334b4',
  'kTh07BonusToastFullPowerItemCase0AltCallAddress = 0x004334bb',
  'kTh07BonusToastFullPowerItemCase4KindPushAddress = 0x004335e5',
  'kTh07BonusToastFullPowerItemCase4ScorePushAddress = 0x004335e7',
  'kTh07BonusToastFullPowerItemCase4CallAddress = 0x004335ee',
  'kTh07BonusToastBorderBonusProducerFunctionAddress = 0x00441670',
  'kTh07BonusToastBorderBonusKindPushAddress = 0x004416d2',
  'kTh07BonusToastBorderBonusScorePushAddress = 0x004416d7',
  'kTh07BonusToastBorderBonusCallAddress = 0x004416dd',
  'kTh07BonusToastSupernaturalBorderProducerFunctionAddress = 0x00441960',
  'kTh07BonusToastSupernaturalBorderKindPushAddress = 0x00441b7a',
  'kTh07BonusToastSupernaturalBorderScorePushAddress = 0x00441b7c',
  'kTh07BonusToastSupernaturalBorderCallAddress = 0x00441b83',
  'kTh07BonusToastUpdateFunctionAddress = 0x0042adab',
  'kTh07BonusToastUpdateVisibleCheckAddress = 0x0042b255',
  'kTh07BonusToastUpdateTimerCurrentCompareAddress = 0x0042b26a',
  'kTh07BonusToastUpdateTimerAddAddress = 0x0042b27e',
  'kTh07BonusToastUpdatePositionXWriteAddress = 0x0042b2aa',
  'kTh07BonusToastUpdateFinalPositionXLoadAddress = 0x0042b2b8',
  'kTh07BonusToastUpdateFinalPositionXWriteAddress = 0x0042b2be',
  'kTh07BonusToastUpdateExpireCompareAddress = 0x0042b2cc',
  'kTh07BonusToastUpdateKindClearAddress = 0x0042b2e3',
  'kTh07BonusToastUpdateTimerCallAddress = 0x0042b2fb',
  'kTh07BonusToastUpdateTimerCallTargetAddress = 0x004010b0',
  'kTh07BonusToastInitKindClearAddress = 0x0042986f',
  'kTh07BonusToastPositionAddAddress = 0x004283f5',
  'kTh07BonusToastFullPowerCompareAddress = 0x004283bb',
  'kTh07BonusToastSupernaturalBorderCompareAddress = 0x004283c1',
  'kTh07BonusToastCherryPointMaxCompareAddress = 0x004283c7',
  'kTh07BonusToastBorderBonusCompareAddress = 0x004283d1',
  'kTh07BonusToastFullPowerColorWriteAddress = 0x004283e0',
  'kTh07BonusToastFullPowerStringPushAddress = 0x004283ea',
  'kTh07BonusToastFullPowerPositionAddAddress = 0x004283f5',
  'kTh07BonusToastSupernaturalScaleLoadAddress = 0x00428414',
  'kTh07BonusToastSupernaturalRenderModeWriteAddress = 0x00428428',
  'kTh07BonusToastSupernaturalColorWriteAddress = 0x00428432',
  'kTh07BonusToastSupernaturalStringPushAddress = 0x0042843c',
  'kTh07BonusToastSupernaturalPositionAddAddress = 0x00428447',
  'kTh07BonusToastSupernaturalRenderModeResetAddress = 0x00428471',
  'kTh07BonusToastCherryPointMaxColorWriteAddress = 0x00428480',
  'kTh07BonusToastCherryPointMaxStringPushAddress = 0x0042848a',
  'kTh07BonusToastCherryPointMaxPositionAddAddress = 0x00428495',
  'kTh07BonusToastBorderBonusScaleLoadAddress = 0x004284b1',
  'kTh07BonusToastBorderBonusRenderModeWriteAddress = 0x004284c5',
  'kTh07BonusToastBorderBonusColorWriteAddress = 0x004284cf',
  'kTh07BorderBonusScorePushAddress = 0x004284df',
  'kTh07BonusToastBorderBonusStringPushAddress = 0x004284e5',
  'kTh07BonusToastBorderBonusPositionAddAddress = 0x004284f0',
  'kTh07BonusToastBorderBonusRenderModeResetAddress = 0x0042851a',
  'kTh07SpellCardBonusVisibleCheckAddress = 0x0042852a',
  'kTh07SpellCardBonusPositionWriteAddress = 0x00428565',
  'kTh07SpellCardBonusLineYWriteAddress = 0x00428577',
  'kTh07SpellCardBonusCaptureIdPushAddress = 0x004285bf',
  'kTh07StageClearStringAddress = 0x00498518',
  'kTh07AllClearStringAddress = 0x0049850c',
  'kTh07ClearBonusClearStringAddress = 0x004984fc',
  'kTh07ClearBonusPointStringAddress = 0x004984ec',
  'kTh07ClearBonusGrazeStringAddress = 0x004984dc',
  'kTh07ClearBonusCherryStringAddress = 0x004984cc',
  'kTh07ClearBonusPlayerStringAddress = 0x004984c0',
  'kTh07ClearBonusBombStringAddress = 0x004984b0',
  'kTh07ClearBonusTotalStringAddress = 0x00498400',
  'kTh07ClearBonusSpellCardStringAddress = 0x0049838c',
  'kTh07FullPowerModeStringAddress = 0x004983e0',
  'kTh07BonusToastPositionXStartConstantAddress = 0x00498a74',
  'kTh07BonusToastPositionYStartConstantAddress = 0x00498b30',
  'kTh07BonusToastSlideDeltaConstantAddress = 0x00498cb8',
  'kTh07BonusToastSlideDivisorConstantAddress = 0x00498b00',
  'kTh07BonusToastPositionXFinalConstantAddress = 0x00498bb8',
  'kTh07RankBonusRuleCount = 6',
  'kTh07BonusToastRuleCount = 4',
  'kTh07BonusToastProducerCallCount = 7',
  'kTh07ClearBonusOwnerFieldCount = 15',
  'kTh07ClearBonusSummaryVisibleOffset = 0x209b4',
  'kTh07ClearBonusTotalScoreOffset = 0x209b8',
  'kTh07BonusPopupPositionOffset = 0x209c0',
  'kTh07BonusPopupScoreOffset = 0x209cc',
  'kTh07BonusPopupVisibleOffset = 0x209d0',
  'kTh07BonusToastPositionOffset = 0x209e0',
  'kTh07BonusToastPositionYOffset = 0x209e4',
  'kTh07BonusToastPositionZOffset = 0x209e8',
  'kTh07BorderBonusScoreOffset = 0x209ec',
  'kTh07BonusToastKindOffset = 0x209f0',
  'kTh07BonusToastTimerOffset = 0x209f4',
  'kTh07BonusToastTimerPreviousOffset = 0x209f4',
  'kTh07BonusToastTimerSubframeOffset = 0x209f8',
  'kTh07BonusToastTimerCurrentOffset = 0x209fc',
  'kTh07SpellCardBonusPositionOffset = 0x20a00',
  'kTh07SpellCardBonusLineYOffset = 0x20a04',
  'kTh07SpellCardBonusCaptureIdOffset = 0x20a0c',
  'kTh07SpellCardBonusVisibleOffset = 0x20a10',
  'kTh07ClearBonusPointItemsOffset = 0x20a24',
  'kTh07ClearBonusCherryOffset = 0x20a28',
  'kTh07ClearBonusGrazeOffset = 0x20a2c',
  'kStageClearBonusPerStage = 1000000',
  'kPointItemClearBonus = 50000',
  'kGrazeClearBonus = 500',
  'kCherryClearBonus = 10',
  'kLifeClearBonus = 20000000',
  'kBombClearBonus = 4000000',
  'kTh07BonusToastColorWarmBits = 0xffc0b0ff',
  'kTh07BonusToastColorBorderBits = 0xffe0b0ff',
  'kTh07BonusToastScaledRenderMode = 0x0b',
  'kTh07BonusToastDefaultRenderMode = 0x0e',
  'kTh07BonusToastTimerInitialPrevious = -999',
  'kTh07BonusToastSlideFrames = 30',
  'kTh07BonusToastLifetimeFrames = 180',
  'kTh07BonusToastRegisterGlobalState = 2',
  'StageClearBonus',
  'PointItemClearBonus',
  'GrazeClearBonus',
  'CherryClearBonus',
  'LifeClearBonus',
  'BombClearBonus',
  'BonusToastRule',
  'kTh07BonusToastRules',
  'BonusToastLifecycleContract',
  'kTh07BonusToastLifecycleContract',
  'BonusToastProducerCall',
  'kTh07BonusToastProducerCalls',
  'UsesTh07BonusToastSlidePath',
  'HasTh07BonusToastExpired',
  'CountTh07BonusToastProducerCalls',
  'FindTh07BonusToastRule',
  'ClearBonusOwnerFieldKind',
  'ClearBonusOwnerField',
  'kTh07ClearBonusOwnerFields',
  'FindTh07ClearBonusOwnerField',
]) {
  mustContain(clearBonusHeader, value, `TH07 clear bonus metadata missing ${value}`);
}
mustContain(clearBonus, 'Th07Difficulty::Phantasm', 'TH07 clear bonus rank table should use portable difficulty enum');
mustEqual(countMatches(clearBonus, /ClearBonusOwnerFieldKind::/g), 15,
  'TH07 clear bonus owner field table count');
mustContain(clearBonus, 'kTh07BonusToastKindLoadAddress', 'TH07 clear bonus owner fields should include toast-kind evidence');
mustContain(clearBonus, 'kTh07SpellCardBonusCaptureIdPushAddress',
  'TH07 clear bonus owner fields should include spell-card capture-id evidence');
mustNotContain(clearBonusHeader, '#include "GameManager.hpp"', 'ClearBonusSystem should not depend on GameManager runtime headers');
mustNotContain(clearBonus, '"0049849c"', 'TH07 clear bonus string anchors should be typed addresses');

const guiLayout = read('GuiLayout.hpp');
for (const value of [
  'kTh07GuiSpellcardSetupFunctionAddress = 0x0042868d',
  'kTh07GuiSpellcardOwnerPointerOffset = 0x08',
  'kTh07GuiSpellcardTextWidthOffset = 0x0c',
  'kTh07GuiSpellcardPlayerPortraitVmOffset = 0x574c',
  'kTh07GuiSpellcardEnemyNameVmOffset = 0x5be4',
  'kTh07GuiSpellcardEnemyNameCopyVmOffset = 0x607c',
  'kTh07GuiSpellcardTextVmOffset = 0x6514',
  'kTh07GuiSpellcardInterruptVmOffset = 0x69ac',
  'kTh07GuiAnmVmScriptIndexOffset = 0x01d8',
  'kTh07GuiAnmVmPendingInterruptOffset = 0x01c6',
  'kTh07GuiSpellcardPlayerPortraitScript = 0x04a1',
  'kTh07GuiSpellcardEnemyNameScript = 0x04a4',
  'kTh07GuiSpellcardEnemyNameCopyScript = 0x04a6',
  'kTh07GuiSpellcardTextScript = 0x0704',
  'kTh07GuiSpellcardDefaultNameSprite = 0x04ac',
  'kTh07GuiSpellcardPendingInterrupt = 0x0001',
  'kTh07GuiSpellcardTextColor = 0x00f0f0ff',
  'kTh07GuiSpellcardTextGlyphWidth = 0x0f',
  'kTh07GuiSpellcardSoundId = 0x0e',
  'kTh07GuiSpellcardGlobalStateAddress = 0x00575ab4',
  'kTh07GuiSpellcardGlobalStateValue = 0x02',
  'kTh07GuiRunMsgFunctionAddress = 0x00429c42',
  'kTh07GuiMsgOpcodeMaxCompareAddress = 0x00429d2e',
  'kTh07GuiMsgOpcodeSwitchTableAddress = 0x0042a83a',
  'kTh07GuiMsgOpcode00CaseAddress = 0x00429d42',
  'kTh07GuiMsgOpcode01CaseAddress = 0x00429d54',
  'kTh07GuiMsgOpcode02CaseAddress = 0x00429e29',
  'kTh07GuiMsgPortraitVmBaseOffset = 0x1fbc0',
  'kTh07GuiMsgPortraitVmStride = 0x024c',
  'kTh07GuiMsgPortraitScriptBase = 0x04a0',
  'kTh07GuiMsgPortraitScriptEnemyOffset = 0x02',
  'kTh07GuiMsgPortraitSpriteEnemyOffset = 0x0d',
  'kTh07GuiMsgPortraitScriptEnemyOffsetAndAddress = 0x00429d6d',
  'kTh07GuiMsgPortraitScriptBaseAddAddress = 0x00429d70',
  'kTh07GuiMsgPortraitSpriteEnemyOffsetAndAddress = 0x00429e49',
  'kTh07GuiMsgPortraitSpriteBaseAddAddress = 0x00429e4c',
  'kTh07GuiMsgPortraitAnmScriptSetCallAddress = 0x00429dc3',
  'kTh07GuiMsgPortraitAnmSpriteSetCallAddress = 0x00429e72',
  'kTh07GuiMsgOpcode03CaseAddress = 0x00429f38',
  'kTh07GuiMsgOpcode04CaseAddress = 0x0042a09d',
  'kTh07GuiMsgOpcode05CaseAddress = 0x0042a126',
  'kTh07GuiMsgOpcode06CaseAddress = 0x0042a187',
  'kTh07GuiMsgOpcode07CaseAddress = 0x0042a19f',
  'kTh07GuiMsgOpcode08CaseAddress = 0x0042a28e',
  'kTh07GuiMsgOpcode09CaseAddress = 0x0042a350',
  'kTh07GuiMsgOpcode10CaseAddress = 0x0042a51d',
  'kTh07GuiMsgOpcode11CaseAddress = 0x0042a560',
  'kTh07GuiMsgOpcode12CaseAddress = 0x0042a522',
  'kTh07GuiMsgOpcode12DurationConstantAddress = 0x00498aa8',
  'kTh07GuiMsgOpcode12DurationBits = 0x40800000',
  'kTh07GuiMsgOpcode12SupervisorThisMovAddress = 0x0042a52c',
  'kTh07GuiMsgOpcode12FadeOutCallAddress = 0x0042a531',
  'kTh07SupervisorFadeOutMusicFunctionAddress = 0x0043a0d6',
  'kTh07GuiMsgOpcode13CaseAddress = 0x0042a71b',
  'kTh07GuiMsgOpcode13ArgReadAddress = 0x0042a727',
  'kTh07GuiMsgOpcode13SkippableWriteAddress = 0x0042a72a',
  'kTh07GuiMsgDialogueSkippableOwnerOffset = 0x209b0',
  'kTh07GuiMsgOpcode14CaseAddress = 0x0042a53b',
  'kTh07GuiMsgOpcode14ScreenEffectColorPushAddress = 0x0042a53f',
  'kTh07GuiMsgOpcode14ScreenEffectDurationMovAddress = 0x0042a544',
  'kTh07GuiMsgOpcode14ScreenEffectKindAddress = 0x0042a549',
  'kTh07GuiMsgOpcode14ScreenEffectCallAddress = 0x0042a54c',
  'kTh07GuiMsgOpcode14GlobalStateWriteAddress = 0x0042a551',
  'kTh07GuiMsgOpcodeMusicFadeout = 0x0c',
  'kTh07GuiMsgOpcodeWaitSkippable = 0x0d',
  'kTh07GuiMsgOpcodeMusicFadeoutSeconds = 4.0f',
  'kTh07GuiMsgOpcodeScreenFade = 0x0e',
  'kTh07GuiMsgOpcodeScreenFadeEffectKind = 4',
  'kTh07GuiMsgOpcodeScreenFadeDurationFrames = 0x192',
  'kTh07GuiMsgOpcodeScreenFadeColor = 0x00ffffff',
  'kTh07GuiMsgOpcodeScreenFadeGlobalStateValue = 0x192',
  'kTh07GuiMsgCurrentInstructionOffset = 0x1fba8',
  'kTh07GuiMsgCurrentIndexOffset = 0x1fbac',
  'kTh07GuiMsgTimerPreviousOffset = 0x1fbb0',
  'kTh07GuiMsgTimerSubframeOffset = 0x1fbb4',
  'kTh07GuiMsgTimerCurrentOffset = 0x1fbb8',
  'kTh07GuiMsgWaitFrameCounterOffset = 0x1fbbc',
  'kTh07GuiMsgIgnoreWaitCounterOffset = 0x209ac',
  'kTh07GuiMsgAdvanceInstructionAddress = 0x0042a730',
  'kTh07GuiMsgTimerTickPathAddress = 0x0042a758',
  'kTh07GuiMsgTimerTickCallAddress = 0x0042a761',
  'kTh07GuiMsgTimerTickFunctionAddress = 0x004010b0',
  'kTh07GuiMsgPostTickPathAddress = 0x0042a766',
  'kTh07GuiMsgAnmVmTickFirstCallAddress = 0x0042a775',
  'kTh07GuiMsgAnmVmTickFunctionAddress = 0x00450d60',
  'Th07GuiMsgOpcodeSemantic',
  'Th07GuiMsgAdvanceMode',
  'Th07GuiMsgOpcodeCase',
  'kTh07GuiMsgOpcodeCaseCount = 15',
  'kTh07GuiMsgOpcodeCases',
  'FindTh07GuiMsgOpcodeCase',
  'kTh07GuiSpellcardPlayerPortraitVmAddAddress = 0x0042869c',
  'kTh07GuiSpellcardPlayerPortraitScriptWriteAddress = 0x004286af',
  'kTh07GuiSpellcardPlayerPortraitActiveSpriteAddAddress = 0x004286de',
  'kTh07GuiSpellcardEnemyNameVmAddAddress = 0x004286f5',
  'kTh07GuiSpellcardEnemyNameScriptWriteAddress = 0x00428708',
  'kTh07GuiSpellcardEnemyNameActiveSpritePushAddress = 0x0042872e',
  'kTh07GuiSpellcardEnemyNameCopyVmAddAddress = 0x00428750',
  'kTh07GuiSpellcardEnemyNameCopyScriptWriteAddress = 0x00428763',
  'kTh07GuiSpellcardEnemyNameCopyActiveSpritePushAddress = 0x00428789',
  'kTh07GuiSpellcardTextVmAddAddress = 0x004287ab',
  'kTh07GuiSpellcardTextScriptWriteAddress = 0x004287be',
  'kTh07GuiSpellcardTextColorPushAddress = 0x004287e9',
  'kTh07GuiSpellcardTextInitVmAddAddress = 0x004287f4',
  'kTh07GuiSpellcardTextWidthWriteAddress = 0x0042884e',
  'kTh07GuiSpellcardInterruptVmAddAddress = 0x00428857',
  'kTh07GuiSpellcardInterruptWriteAddress = 0x00428862',
  'kTh07GuiSpellcardSoundPushAddress = 0x0042886d',
  'kTh07GuiSpellcardGlobalStateWriteAddress = 0x00428879',
  'Th07GuiSpellcardVmKind',
  'Th07GuiSpellcardVmSlot',
  'kTh07GuiSpellcardVmSlotCount = 4',
  'kTh07GuiSpellcardVmSlots',
  'FindTh07GuiSpellcardVmSlot',
]) {
  mustContain(guiLayout, value, `TH07 GUI spell-card layout metadata missing ${value}`);
}
mustEqual(countMatches(guiLayout, /Th07GuiSpellcardVmKind::/g), 4,
  'TH07 GUI spell-card VM slot count');
mustEqual(countMatches(guiLayout, /Th07GuiMsgOpcodeSemantic::/g), 15,
  'TH07 GUI MSG opcode case semantic count');
mustEqual(countMatches(guiLayout, /Th07GuiMsgAdvanceMode::/g), 15,
  'TH07 GUI MSG opcode advance-mode count');

const guiHeader = read('Gui.hpp');
for (const value of [
  'MsgRawInstrArgDialogueSkippable',
  'u8 isSkippable',
  'MsgRawInstrArgDialogueSkippable dialogueSkippable',
]) {
  mustContain(guiHeader, value, `TH07 MSG wait-skippable argument shape missing ${value}`);
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
for (const value of [
  'MSG_OPCODE_MUSICFADEOUT',
  'kTh07GuiMsgOpcodeMusicFadeoutSeconds',
  'MSG_OPCODE_WAITSKIPPABLE',
  'dialogueSkippable.isSkippable',
  'MSG_OPCODE_SCREENFADE',
  'ScreenEffect::RegisterChain(SCREEN_EFFECT_FULLSCREEN_FADE_OUT',
  'kTh07GuiMsgOpcodeScreenFadeDurationFrames',
  'kTh07GuiMsgOpcodeScreenFadeGlobalStateValue',
  'g_GuiGlobalState',
  'kTh07GuiMsgPortraitScriptBase',
  'kTh07GuiMsgPortraitScriptEnemyOffset',
  'kTh07GuiMsgPortraitSpriteEnemyOffset',
]) {
  mustContain(gui, value, `TH07 Gui MSG runtime missing ${value}`);
}
mustNotContain(gui, 'ANM_SCRIPT_FACE_START + 8', 'TH07 Gui portrait sprite opcode still uses copied TH06 enemy sprite offset');
for (const stale of ['data/face00a.anm', 'data/face01a.anm', 'data/face03a.anm', 'data/face05a.anm', 'data/face06a.anm', 'data/face08a.anm', 'data/face09a.anm', 'data/face10a.anm', 'data/face12a.anm', 'data/face12b.anm', 'data/face12c.anm']) {
  mustNotContain(gui, stale, `TH06 Gui face path still present: ${stale}`);
}

const stageCpp = read('Stage.cpp');
const stageH = read('Stage.hpp');
const anmDrawRuntime = `${read('AnmManager.hpp')}\n${read('AnmManager.cpp')}`;
const screenEffect = read('ScreenEffect.cpp');
const screenEffectHeader = read('ScreenEffect.hpp');
mustContain(effectTables, 'kTh07ScreenEffectRegisterChainFunctionAddress = 0x0044b310',
  'TH07 screen effect register function address missing');
mustContain(screenEffectHeader, 'SCREEN_EFFECT_FULLSCREEN_FADE_OUT = 4',
  'TH07 fullscreen fade-out screen effect enum missing');
mustContain(screenEffect, 'case SCREEN_EFFECT_FULLSCREEN_FADE_OUT:',
  'TH07 fullscreen fade-out screen effect runtime missing');
mustContain(screenEffect, 'drawChainElem = g_Chain.CreateElem((ChainCallback)ScreenEffect::DrawFadeIn)',
  'TH07 fullscreen fade-out should use original full-screen draw callback');
for (const value of [
  'DrawTh07ScreenQuad(void *th07Vm)',
  'DrawTh07CameraProjected(void *th07Vm)',
  'DrawTh07TransformedCameraProjected(void *th07Vm)',
  'ReadTh07AnmVmDrawMatrix(th07Vm, kTh07AnmVmBaseTransformMatrixOffset)',
  'ReadTh07AnmVmDrawMatrix(th07Vm, kTh07AnmVmActiveTransformMatrixOffset)',
  'BuildTh07AnmVmNoRotationQuad',
  'BuildTh07AnmVmRotatedQuad',
  'Th07AnmVmDrawQuadIntersectsViewport',
  'TranslateTh07AnmVmDrawVertex',
  'BuildTh07AnmVmDrawBackendVm',
  'D3DXVec3Project(&projectedCenter',
  'D3DXVec3Project(&projectedSide, &g_Stage.th07CameraCurrentBasis.sideVector',
  'activeMatrix.m[3][0] = backend.pos.x',
  'fabsf((backend.sprite->widthPx * backend.scaleX) / 2.0f)',
  'WriteTh07AnmVmDrawMatrix(th07Vm, kTh07AnmVmActiveTransformMatrixOffset, activeMatrix)',
  'WriteTh07AnmVmDrawU32(th07Vm, kTh07AnmVmRenderFlagsOffset, renderFlags)',
  'SubmitTh07AnmVmDrawQuad',
  'SubmitTh07AnmVmDrawQuad(this, &backend, snapToPixels)',
  'SubmitTh07AnmVmDrawQuad(this, &backend, 0)',
  'OffsetTh07AnmVmDrawQuadByManager',
  'SnapTh07AnmVmDrawQuadToPixels',
  'nearbyintf',
  'SetTh07AnmVmDrawQuadUv',
  'ApplyTh07AnmManagerColorMultiplier',
  'MultiplyTh07AnmVmColorComponent',
  'SetTh07AnmManagerDrawOffset',
  'ResetTh07AnmManagerDrawOffset',
  'SetTh07AnmManagerColorMultiplier',
  'ResetTh07AnmManagerColorMultiplier',
  'g_Th07AnmManagerColorMultiplierEnabled',
  'g_Th07AnmManagerDrawOffsetX',
  'g_Th07AnmManagerDrawOffsetY',
  'g_Th07AnmManagerSubmittedQuadCount',
  'CopyTh07AnmVmDrawQuadToDiffuseVertices',
  'ApplyTh07AnmVmSubmitRenderState',
  'D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_XYZRHW',
  'DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_PrimitivesToDrawNoVertexBuf, 0x1c)',
]) {
  mustContain(anmDrawRuntime, value, `TH07 ANM draw runtime missing ${value}`);
}
mustNotContain(anmDrawRuntime, 'this->DrawInner(&backend', 'TH07 ANM draw helpers still submit through DrawInner bridge');
for (const value of [
  'StageCameraBasisBlock',
  'ZUN_ASSERT_SIZE(StageCameraBasisBlock, 0x40)',
  'StageCameraInterpolationTimer',
  'ZUN_ASSERT_SIZE(StageCameraInterpolationTimer, 0xc)',
  'Th07StageOriginalAnmVmSlot',
  'ZUN_ASSERT_SIZE(Th07StageOriginalAnmVmSlot, kTh07StageAnmVmSlotSize)',
  'ZUN_ASSERT_SIZE(Stage, 0x52b4)',
  'th07StageAnmVms[2]',
  'originalColor',
  'originalAlternateColor',
  'originalRenderFlags',
  'originalPendingInterrupt',
  'originalBeginningOfScript',
  'originalCurrentInstruction',
  'originalLoadedSprite',
  'originalLastSpriteSetFrame',
  'th07StdData',
  'th07BeginningOfScript',
  'th07ScriptTime',
  'th07InstructionIndex',
  'th07FrameCounter',
  'th07StageRoute',
  'th07StagePosition',
  'th07StageField4d8',
  'th07Fog',
  'th07FogInterpInitial',
  'th07FogInterpFinal',
  'th07FogInterpDuration',
  'th07FogInterpTimer',
  'th07CameraResumeInstruction',
  'th07CameraTargetBasis',
  'th07CameraInterpStartBasis',
  'th07CameraCurvePoint3Basis',
  'th07CameraCurvePoint2Basis',
  'th07CameraCurrentBasis',
  'th07CameraInterpDurations[4]',
  'th07CameraInterpTimers[4]',
  'th07CameraInterpModes[4]',
  'th07StagePositionInterpFinal',
  'th07StagePositionInterpEndFrame',
  'th07StagePositionInterpInitial',
  'th07StagePositionInterpStartFrame',
  'th07CameraSetupPendingAfterJump',
  'th07AnmManagerColorMultiplier',
  'th07AnmManagerColorResetGuard',
  'offsetof(Stage, th07StageAnmVms[0]) == kTh07StageFirstAnmVmOffset',
  'offsetof(Stage, th07StageAnmVms[1]) == kTh07StageSecondAnmVmOffset',
  'offsetof(Th07StageOriginalAnmVmSlot, originalRenderFlags) == kTh07AnmVmRenderFlagsOffset',
  'offsetof(Th07StageOriginalAnmVmSlot, originalBeginningOfScript) ==',
  'offsetof(Th07StageOriginalAnmVmSlot, originalCurrentInstruction) ==',
  'offsetof(Th07StageOriginalAnmVmSlot, originalLoadedSprite) ==',
  'offsetof(Th07StageOriginalAnmVmSlot, originalLastSpriteSetFrame) ==',
  'offsetof(Stage, th07StdData) == kTh07StageStdDataOffset',
  'offsetof(Stage, th07BeginningOfScript) == kTh07StageScriptOffset',
  'offsetof(Stage, th07ScriptTime) == kTh07StageScriptTimerOffset',
  'offsetof(Stage, th07StageRoute) == kTh07StageRouteOffset',
  'offsetof(Stage, th07StagePosition) == kTh07StagePositionOffset',
  'offsetof(Stage, th07Fog) == kTh07StageFogNearOffset',
  'offsetof(Stage, th07CameraCurrentBasis) == kTh07StageCameraPositionOffset',
  'offsetof(Stage, th07CameraInterpDurations) == kTh07StageCameraPositionInterpDurationOffset',
  'offsetof(Stage, th07CameraInterpTimers) == kTh07StageCameraPositionInterpTimerOffset',
  'offsetof(Stage, th07CameraInterpModes) == kTh07StageCameraPositionInterpModeOffset',
  'offsetof(Stage, th07StagePositionInterpFinal) == kTh07StagePositionInterpFinalOffset',
  'offsetof(Stage, th07CameraSetupPendingAfterJump) == kTh07StageCameraSetupPendingOffset',
  'offsetof(Stage, th07AnmManagerColorMultiplier) == kTh07StageAnmManagerColorMultiplierOffset',
  'offsetof(Stage, th07AnmManagerColorResetGuard) == kTh07StageAnmManagerColorResetGuardOffset',
]) {
  mustContain(stageH, value, `TH07 Stage camera layout missing ${value}`);
}
mustNotContain(stageH, 'ZUN_ASSERT_SIZE(Stage, 0x2f4)', 'TH06-sized Stage layout still asserted');
for (const value of [
  'InitializeTh07StageCamera',
  'BuildTh07InitialStageCameraState',
  'BuildStageCameraRuntimeState',
  'ApplyStageCameraRuntimeState',
  'BuildStageScriptRuntimeState',
  'ApplyStageScriptRuntimeState',
  'FindNextTh07StagePositionKey',
  'ResolvePendingTh07StageResume',
  'ApplyDueTh07StageAnmVmInstruction',
  'Th07StageAnmVmInstructionBytes',
  'SetTh07StageAnmVmCurrentInstruction',
  'IsTh07AnmVmOperandReference',
  'ReadTh07StageAnmVmI32Operand',
  'ReadTh07StageAnmVmF32Operand',
  'ResolveTh07StageAnmVmI32WriteTarget',
  'ResolveTh07StageAnmVmF32WriteTarget',
  'WriteTh07StageAnmVmI32Operand',
  'WriteTh07StageAnmVmF32Operand',
  'ApplyTh07StageAnmVmVariableInstruction',
  'FindTh07AnmVmVariableSlot',
  'FindTh07AnmVmVariableExecuteOpcode(header.opcode)',
  'TruncateTh07AnmVmFloatToI32',
  'Th07StageAnmVmI32FromU32',
  'CanDivideTh07StageAnmVmI32',
  'ApplyTh07StageAnmVmFmod',
  'ApplyTh07StageAnmVmUnaryMathInstruction',
  'ApplyTh07StageAnmVmSin',
  'ApplyTh07StageAnmVmCos',
  'ApplyTh07StageAnmVmTan',
  'ApplyTh07StageAnmVmAsin',
  'ApplyTh07StageAnmVmAtan',
  'CompareTh07StageAnmVmI32Condition',
  'CompareTh07StageAnmVmF32Condition',
  'ApplyTh07StageAnmVmConditionalJumpInstruction',
  'FindTh07AnmVmConditionalJumpExecuteOpcode(header.opcode)',
  'ReadTh07AnmVmInstructionI32Operand(instruction, contract->jumpOffsetOperandIndex',
  'ReadTh07AnmVmInstructionI32Operand(instruction, contract->jumpFrameOperandIndex',
  'SetTh07StageAnmVmTimer',
  'DecrementTh07StageAnmVmTimer',
  'ResetTh07StageAnmVmTimerAt',
  'ResetTh07StageAnmVmDurationTimer',
  'TickTh07StageAnmVmTimerAt',
  'BuildTh07StageAnmVmInterpolationRatio',
  'ApplyTh07StageAnmVmInterpolationCurve',
  'StartTh07StageAnmVmPositionInterpolation',
  'StartTh07StageAnmVmVisibilityInterpolation',
  'StartTh07StageAnmVmColorInterpolation',
  'StartTh07StageAnmVmRotationInterpolation',
  'StartTh07StageAnmVmScaleInterpolation',
  'ApplyTh07StageAnmVmFollowupInstruction',
  'FindTh07AnmVmFollowupExecuteOpcode(header.opcode)',
  'ApplyTh07StageAnmVmStopUntilInterruptInstruction',
  'AdvanceTh07StageAnmVmInterpolationLane',
  'Th07AnmVmPostStepActionSemantic::AdvanceInterpolationLane',
  'ApplyTh07StageAnmVmPendingInterrupt',
  'kTh07AnmVmInterruptLabelOpcode',
  'kTh07AnmVmFallbackInterruptLabel',
  'kTh07AnmVmRenderFlagInterruptPending',
  'ApplyTh07StageAnmVmWaitTimerInstruction',
  'FindTh07AnmVmControlExecuteOpcode(header.opcode)',
  'Th07AnmVmControlOpcodeSemantic::WaitTimer',
  'slot->vm.currentTimeInScript.Decrement(1)',
  'g_Rng.GetRandomU32()',
  'g_Rng.GetRandomF32ZeroToOne()',
  'NormalizeTh07EffectAngle(floatSource)',
  'ApplyTh07StageAnmVmInstruction',
  'case 5:',
  'ResolveTh07StageAnmVmI32WriteTarget(slot, instruction, header, 0, &counterTarget)',
  'SubtractTh07StageAnmVmI32(ReadTh07StageAnmVmI32Memory(counterTarget), 1)',
  'ReadTh07StageAnmVmI32Operand(slot, instruction, header, 0, &remaining)',
  'ReadTh07AnmVmInstructionI32Operand(instruction, 1, &relativeOffset)',
  'ReadTh07AnmVmInstructionI32Operand(instruction, 2, &frame)',
  'AdvanceTh07StageAnmVmPostStep',
  'ReadTh07AnmVmInstructionHeader',
  'ReadTh07AnmVmInstructionI32Operand',
  'ReadTh07AnmVmInstructionF32Operand',
  'g_AnmManager->SetActiveSprite(&slot->vm',
  'originalLastSpriteSetFrame = slot->vm.currentTimeInScript.AsFrames()',
  'SetTh07StageAnmVmScript',
  'originalBeginningOfScript = slot->vm.beginingOfScript',
  'originalCurrentInstruction = slot->vm.currentInstruction',
  'originalLoadedSprite = slot->vm.sprite',
  'ExecuteTh07StageAnmVm(&stage->th07StageAnmVms[0])',
  'DrawTh07StageAnmVm(&stage->th07StageAnmVms[0])',
  'g_AnmManager->DrawTh07ScreenQuad(slot)',
  'ApplyDueTh07StageScriptInstruction',
  'ApplyDueTh07StageCameraInstruction',
  'AdvanceTh07StageCameraRuntime',
  'FindTh07StageCameraOpcode(instruction->opcode)',
  'ApplyTh07StageCameraOpcode(&state, cameraInstruction)',
  'BuildTh07StageAnmVmOpcodeEffect',
  'ApplyTh07StageScriptOpcode(&state',
  'AdvanceTh07StageFogInterpolation(&state',
  'AdvanceTh07StageCameraRuntime(stage)',
  'TickTh07StageTimer(&stage->th07ScriptTime)',
  'th07CameraTargetBasis = stage->th07CameraCurrentBasis',
  'th07CameraInterpStartBasis = stage->th07CameraCurrentBasis',
  'th07CameraInterpTimers[idx].previousFrame = -999',
  'ApplyTh07StageAnmManagerColorWrite',
  'GuardTh07StageAnmManagerColorReset',
  'ConsumeTh07StageAnmManagerColorWrite',
  'ResetTh07StageAnmManagerColorAfterLowPrio',
  'SetTh07AnmManagerColorMultiplier(stage->th07AnmManagerColorMultiplier)',
  'kTh07StageAnmManagerColorMultiplierIdle',
  'ResetTh07AnmManagerDrawOffset()',
  'ResetTh07AnmManagerColorMultiplier()',
]) {
  mustContain(stageCpp, value, `TH07 Stage camera initialization missing ${value}`);
}
for (const value of [
  'ResetTh07AnmManagerDrawOffset()',
  'SetTh07AnmManagerDrawOffset(drawOffsetX, drawOffsetY)',
  'g_GameManager.gameFrames < kTh07ScreenShakeRequiredGameFrameCount',
  'g_Rng.GetRandomU32InRange(3)',
  'drawOffsetX = screenOffset',
  'drawOffsetX = -screenOffset',
  'drawOffsetY = screenOffset',
  'drawOffsetY = -screenOffset',
]) {
  mustContain(screenEffect, value, `TH07 screen shake draw-offset runtime missing ${value}`);
}
mustNotContain(screenEffect, 'g_GameManager.arcadeRegionSize.x = 384.0f - screenOffset', 'TH07 shake still shrinks the arcade region width');
mustNotContain(screenEffect, 'g_GameManager.arcadeRegionSize.y = 448.0f - screenOffset', 'TH07 shake still shrinks the arcade region height');
mustNotContain(stageCpp, 'g_AnmManager->ExecuteScript(&slot->vm)', 'TH07 stage ANM VM still falls back to TH06 ExecuteScript');
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

auditRuntimeReplacementManifest();
auditPlayerFunctionManifest();

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
  runtimeReplacementRows: runtimeReplacementManifestRows,
  playerFunctionManifestRows,
  fullDecompFunctions: existsSync(decompManifestPath) ? readFileSync(decompManifestPath, 'utf8').trim().split(/\r?\n/).length - 1 : 'not-exported',
}, null, 2));

function auditPlayerFunctionManifest() {
  if (!existsSync(playerFunctionManifestPath)) {
    problems.push('missing Player function manifest: config/player_function_manifest.csv');
    return;
  }

  const text = readFileSync(playerFunctionManifestPath, 'utf8').trim();
  if (!text) {
    problems.push('Player function manifest is empty');
    return;
  }

  const lines = text.split(/\r?\n/);
  const expectedHeader = [
    'address',
    'original_name',
    'role',
    'current_surface',
    'status',
    'evidence',
    'next_action',
  ];
  const header = lines[0].split(',');
  mustEqual(header.join(','), expectedHeader.join(','), 'Player function manifest header');

  const validStatuses = new Set([
    'unmapped',
    'mapped',
    'comparable',
    'converging',
    'behavior-matched',
    'objdiff-close',
    'objdiff-high',
    'binary-exact',
  ]);
  const rows = [];
  for (let i = 1; i < lines.length; i++) {
    const raw = lines[i];
    if (!raw.trim()) continue;
    const parts = raw.split(',');
    if (parts.length !== expectedHeader.length) {
      problems.push(`Player function manifest row ${i + 1} has ${parts.length} columns`);
      continue;
    }
    const row = Object.fromEntries(expectedHeader.map((name, idx) => [name, parts[idx].trim()]));
    rows.push(row);
    for (const field of expectedHeader) {
      if (!row[field]) problems.push(`Player function manifest row ${i + 1} missing ${field}`);
    }
    if (!validStatuses.has(row.status)) {
      problems.push(`Player function manifest row ${i + 1} has invalid status ${row.status}`);
    }
    if (row.status !== 'unmapped' && row.evidence === 'reference/ghidra/decomp/functions') {
      problems.push(`Player function manifest row ${i + 1} needs function-specific evidence`);
    }
  }

  playerFunctionManifestRows = rows.length;
  mustEqual(rows.length, 34, 'Player original function manifest row count');
  const byAddress = new Map(rows.map((row) => [row.address, row]));
  for (const address of [
    '0x0043d0e0',
    '0x0043e0a0',
    '0x0043e4e0',
    '0x00440940',
    '0x004411c0',
    '0x00441800',
    '0x004418b0',
    '0x00441e80',
    '0x00441fb0',
    '0x004423e0',
    '0x004428e0',
    '0x004429d0',
  ]) {
    if (!byAddress.has(address)) problems.push(`Player function manifest missing ${address}`);
  }
  const itemBox = byAddress.get('0x0043e4e0');
  if (itemBox) {
    mustEqual(itemBox.status, 'behavior-matched', 'Player item-box function status');
    mustContain(itemBox.evidence, 'tests/test_player_layout.cpp', 'Player item-box manifest evidence');
  }
  const bombSlots = byAddress.get('0x0043e0a0');
  if (bombSlots) {
    mustEqual(bombSlots.status, 'comparable', 'Player bomb collision helper status');
    mustContain(bombSlots.current_surface, 'Player::CalcBombCollision', 'Player bomb collision manifest surface');
    mustContain(bombSlots.evidence, 'src/PlayerLayout.hpp', 'Player bomb collision manifest layout evidence');
  }
  for (const [address, surface] of [
    ['0x0043d0e0', 'Player::OptionShotEffectCallback'],
    ['0x00440940', 'Player::UpdateBombCollisionSlots'],
    ['0x004411c0', 'Player::UpdateMode1Invulnerable'],
    ['0x00441800', 'Player::RegisterBombCollisionRect'],
    ['0x004418b0', 'Player::RegisterBombCollisionCircle'],
    ['0x00441e80', 'Player::ResetOptionFields'],
  ]) {
    const row = byAddress.get(address);
    if (row) {
      mustEqual(row.status, 'comparable', `Player ${surface} manifest status`);
      mustContain(row.current_surface, surface, `Player ${surface} manifest surface`);
      mustContain(row.evidence, 'src/Player.cpp', `Player ${surface} manifest source evidence`);
    }
  }
}

function auditRuntimeReplacementManifest() {
  if (!existsSync(runtimeManifestPath)) {
    problems.push('missing runtime replacement manifest: config/runtime_replacement_manifest.csv');
    return;
  }

  const text = readFileSync(runtimeManifestPath, 'utf8').trim();
  if (!text) {
    problems.push('runtime replacement manifest is empty');
    return;
  }

  const lines = text.split(/\r?\n/);
  const expectedHeader = [
    'subsystem',
    'original_address',
    'original_name',
    'live_surface',
    'status',
    'evidence',
    'blocker',
    'next_action',
    'validation',
  ];
  const header = lines[0].split(',');
  mustEqual(header.join(','), expectedHeader.join(','), 'runtime replacement manifest header');

  const validStatuses = new Set([
    'unmapped',
    'mapped',
    'comparable',
    'converging',
    'behavior-matched',
    'objdiff-close',
    'objdiff-high',
    'binary-exact',
    'live-th07',
    'function-mapped',
    'legacy-th06',
    'portable-boundary',
    'blocked',
    'pending',
  ]);
  const rows = [];
  for (let i = 1; i < lines.length; i++) {
    const raw = lines[i];
    if (!raw.trim()) continue;
    const parts = raw.split(',');
    if (parts.length !== expectedHeader.length) {
      problems.push(`runtime replacement manifest row ${i + 1} has ${parts.length} columns`);
      continue;
    }
    const row = Object.fromEntries(expectedHeader.map((name, idx) => [name, parts[idx].trim()]));
    rows.push(row);
    for (const field of ['subsystem', 'original_address', 'original_name', 'live_surface', 'status', 'evidence', 'next_action', 'validation']) {
      if (!row[field]) problems.push(`runtime replacement manifest row ${i + 1} missing ${field}`);
    }
    if (!validStatuses.has(row.status)) {
      problems.push(`runtime replacement manifest row ${i + 1} has invalid status ${row.status}`);
    }
    if (['legacy-th06', 'blocked', 'pending', 'unmapped'].includes(row.status) && (!row.blocker || row.blocker === 'none')) {
      problems.push(`runtime replacement manifest row ${i + 1} status ${row.status} must name a blocker`);
    }
    if ([
      'live-th07',
      'function-mapped',
      'mapped',
      'comparable',
      'converging',
      'behavior-matched',
      'objdiff-close',
      'objdiff-high',
      'binary-exact',
    ].includes(row.status) && row.evidence === 'reference/ghidra/decomp/functions') {
      problems.push(`runtime replacement manifest row ${i + 1} status ${row.status} needs specific evidence`);
    }
  }

  runtimeReplacementManifestRows = rows.length;
  if (rows.length < 40) {
    problems.push(`runtime replacement manifest should cover the first runtime wave broadly: got ${rows.length} rows`);
  }

  const bySubsystem = new Map(rows.map((row) => [row.subsystem, row]));
  for (const subsystem of [
    'PlayableExecutable',
    'FileSystemOpenPath',
    'ArchivePbg4Reader',
    'AnmManagerLoad',
    'StageStdLoad',
    'GuiMsgDispatch',
    'EclDispatch',
    'ShtParser',
    'PlayerUpdateDraw',
    'BombRoutineTable',
    'BulletLaserRuntime',
    'EffectSpawn',
    'CherryBorderRuntime',
    'GameManagerLayout',
    'ReplayInput',
    'SdlVulkanShell',
  ]) {
    if (!bySubsystem.has(subsystem)) problems.push(`runtime replacement manifest missing subsystem ${subsystem}`);
  }

  const playable = bySubsystem.get('PlayableExecutable');
  if (playable) {
    mustContain(playable.next_action, 'launchable executable', 'playable executable row must require launchable executable work');
    mustContain(playable.next_action, 'loads original TH7 resources', 'playable executable row must require original TH7 resource loading');
    mustContain(playable.validation, 'cmake --build --preset linux-debug', 'playable executable validation must include app build');
    mustContain(playable.validation, '--check-resources', 'playable executable validation must include resource check');
    mustContain(playable.validation, '--headless-smoke', 'playable executable validation must include headless smoke');
  }

  const legacyRows = rows.filter((row) => row.status === 'legacy-th06');
  if (!legacyRows.some((row) => row.subsystem === 'PlayerUpdateDraw')) {
    problems.push('runtime replacement manifest should explicitly mark PlayerUpdateDraw as legacy-th06 until replaced');
  }
  if (!rows.some((row) => row.status === 'portable-boundary')) {
    problems.push('runtime replacement manifest should include at least one portable-boundary row');
  }
  if (!rows.some((row) => ['live-th07', 'behavior-matched', 'objdiff-close', 'objdiff-high', 'binary-exact'].includes(row.status))) {
    problems.push('runtime replacement manifest should include already behavior-matched TH07 helper rows');
  }
}

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

function extractCppArrayBlock(text, name) {
  const match = text.match(new RegExp(`${name}\\s*=\\s*\\{\\{([\\s\\S]*?)\\}\\};`));
  if (!match) {
    problems.push(`Missing C++ array ${name}`);
    return '';
  }
  return match[1];
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
