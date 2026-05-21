import { existsSync, readFileSync, readdirSync, statSync } from 'node:fs';
import { join } from 'node:path';
import { fileURLToPath } from 'node:url';

const root = fileURLToPath(new URL('../', import.meta.url));
const src = join(root, 'src');
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
  'Th07EffectTables.hpp',
  'Th07EffectTables.cpp',
  'ShtManager.hpp',
  'ShtManager.cpp',
  'ShtFile.hpp',
  'ShtFile.cpp',
  'PlayerBombTables.hpp',
  'PlayerBombTables.cpp',
  'PlayerLayout.hpp',
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

if (!existsSync(join(src, 'PROVENANCE.md'))) problems.push('missing reconstructed provenance file');

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
mustContain(playerH, 'Th07PlayerOriginalAnmVmSlot playerSprite', 'TH07 Player prefix should use original-size VM slots');
mustContain(playerH, 'offsetof(Player, positionCenter) == kPlayerPositionOffset',
  'TH07 Player position offset assert missing');
mustContain(playerH, 'offsetof(Player, movementDelta) == kPlayerMovementDeltaOffset',
  'TH07 Player movement delta offset assert missing');

const manifest = read('Th07ResourceManifest.cpp');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/ecldata\d\.ecl"/g), 8, 'ECL manifest entry count');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/stage\d\.std"/g), 8, 'STD manifest entry count');
mustEqual(countMatches(manifest, /(?:ResourceStringAnchor)?\{"data\/msg\d\.dat"/g), 8, 'MSG manifest entry count');

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

const playerBombTables = `${read('PlayerBombTables.hpp')}\n${read('PlayerBombTables.cpp')}`;
const playerEvidenceTables = `${read('PlayerLayout.hpp')}\n${playerBombTables}`;
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::UnfocusedCalc/g), 6, 'unfocused bomb calc routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::UnfocusedDraw/g), 6, 'unfocused bomb draw routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::FocusedCalc/g), 6, 'focused bomb calc routine count');
mustEqual(countMatches(playerBombTables, /\{PlayerBombRoutineSlot::FocusedDraw/g), 6, 'focused bomb draw routine count');
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
  'kPlayerPrimaryAnmScript',
  'kPlayerLeftOptionAnmScript',
  'kPlayerRightOptionAnmScript',
  'kPlayerBombHorizontalSpeedMultiplierOffset',
  'kPlayerBombVerticalSpeedMultiplierOffset',
  'kPlayerCurrentPowerOffset',
  'kPlayerMode3LockoutTimerOffset',
  'kPlayerModeStateOffset',
  'kPlayerModeDrawEnabledOffset',
  'kPlayerModeTransitionRequestOffset',
  'kPlayerModeTransitionEffectActivePointerOffset',
  'kPlayerInitFunctionAddress',
  'kPlayerMovementFunctionAddress',
  'kPlayerKillBoxCollisionFunctionAddress',
  'kPlayerGrazeCollisionFunctionAddress',
  'kPlayerItemBoxCollisionFunctionAddress',
  'kPlayerLaserHitboxFunctionAddress',
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
  'kPlayerMode4StageColorFrameGlobalAddress',
  'kPlayerMode4CommonEffectDurationFrames',
  'kPlayerMode3CommonEffectDurationFrames',
  'kPlayerMode3EffectInterpolationDurationFrames',
  'kPlayerMode1CleanupCommonEffectDurationFrames',
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
  'PlayerModeTransitionEffectConfig',
  'PlayerStageColorWriteProducer',
  'BuildPlayerBombCommonEffectConfig',
  'BuildPlayerMode4TransitionEffectConfig',
  'BuildPlayerMode3TransitionEffectConfig',
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
  '0x0400',
  '0x0480',
  '0x0481',
  '0x004423e0',
  '0x0043ee50',
  '0x0043e260',
  '0x0043e3b0',
  '0x0043e4e0',
  '0x0043e6b0',
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
  '0x00441670',
  '0x00441960',
  '0x00441bd0',
  '0x004420b0',
  '0x004d44e0',
  '0x1fe',
  '0x21c',
  '0x0b7e6c',
  '0x240d',
  '0x23fc',
  '0x1c',
  '0x28',
  '0x1e',
  '0xf0',
  '0x3fa66666',
]) {
  mustContain(playerEvidenceTables, value, `TH07 bomb calc routine missing ${value}`);
}
mustEqual(countMatches(playerBombTables, /PlayerStageColorWriteKind::/g), 3, 'TH07 player stage-color producer kind reference count');

const bombRuntime = read('BombData.cpp');
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
const stageH = read('Stage.hpp');
const anmDrawRuntime = `${read('AnmManager.hpp')}\n${read('AnmManager.cpp')}`;
const screenEffect = read('ScreenEffect.cpp');
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
