#include "Th07ResourceFormatTables.hpp"

#include <cassert>
#include <bit>
#include <cstddef>

int main()
{
    static_assert(th07::kTh07AnmFileCount == 63);
    static_assert(th07::kTh07AnmSpriteCount == 1187);
    static_assert(th07::kTh07AnmScriptCount == 727);
    static_assert(th07::kTh07AnmVmScriptInstructionCount == 6214);
    static_assert(th07::kTh07AnmVmScriptTerminatorCount == 727);
    static_assert(th07::kTh07AnmVmOpcodeCount == 44);
    static_assert(th07::kTh07AnmVmMaxOpcode == 80);
    static_assert(th07::kTh07AnmVmOpcodeUsageCount == 44);
    static_assert(th07::kTh07AnmVmOpcodeArgShapeCount == 44);
    static_assert(th07::kTh07AnmVmTotalArgByteCount == 38724);
    static_assert(th07::kTh07AnmVmInstructionSizeClassCount == 6);
    static_assert(th07::kTh07StdFileCount == 8);
    static_assert(th07::kTh07StdBinaryOpcodeUsageCount == 25);
    static_assert(th07::kTh07StdBinaryOpcodeArgShapeCount == 25);
    static_assert(th07::kTh07StdBinaryTotalArgByteCount == 4536);
    static_assert(th07::kTh07ReadableStdDumpCount == 7);
    static_assert(th07::kTh07EmptyReadableStdDumpStage == 6);
    static_assert(th07::kTh07MsgDumpCount == 8);
    static_assert(th07::kTh07MsgEntryCount == 60);
    static_assert(th07::kTh07MsgTimelineLabelCount == 1098);
    static_assert(th07::kTh07MsgBinaryEntryPointerCount == 180);
    static_assert(th07::kTh07MsgBinaryUniqueEntryCount == 60);
    static_assert(th07::kTh07MsgBinaryInstructionCount == 4883);
    static_assert(th07::kTh07MsgBinaryTextByteCount == 27832);
    static_assert(th07::kTh07MsgBinaryWaitTotalFrames == 313330);
    static_assert(th07::kTh07MsgBinaryMaxOpcode == 14);
    static_assert(th07::kTh07MsgBinaryOpcodeUsageCount == 15);
    static_assert(th07::kTh07MsgBinaryOpcodeArgShapeCount == 15);
    static_assert(th07::kTh07MsgBinaryTotalArgByteCount == 46572);

    assert(th07::kTh07AnmFileSummaries.size() == th07::kTh07AnmFileCount);
    assert(th07::CountTh07AnmSprites() == th07::kTh07AnmSpriteCount);
    assert(th07::CountTh07AnmScripts() == th07::kTh07AnmScriptCount);
    assert(th07::CountTh07AnmFilesWithTextureData() == 61);
    assert(th07::CountTh07AnmVmScriptInstructionsBySize() == th07::kTh07AnmVmScriptInstructionCount);
    assert(th07::CountTh07AnmVmOpcodeUsageInstructions() == th07::kTh07AnmVmScriptInstructionCount);
    assert(th07::CountTh07AnmVmOpcodeArgBytes() == th07::kTh07AnmVmTotalArgByteCount);
    assert(th07::kTh07AnmVmOpcodes.front() == 0);
    assert(th07::kTh07AnmVmOpcodes.back() == th07::kTh07AnmVmMaxOpcode);
    assert(th07::IsTh07AnmVmOpcodeUsed(50));
    assert(th07::IsTh07AnmVmOpcodeUsed(80));
    assert(!th07::IsTh07AnmVmOpcodeUsed(81));
    assert(th07::CountTh07StdBinaryObjects() == th07::kTh07StdBinaryObjectCount);
    assert(th07::CountTh07StdBinaryQuads() == th07::kTh07StdBinaryQuadCount);
    assert(th07::CountTh07StdBinaryInstances() == th07::kTh07StdBinaryInstanceCount);
    assert(th07::CountTh07StdBinaryScriptInstructions() == th07::kTh07StdBinaryScriptInstructionCount);
    assert(th07::CountTh07StdBinaryOpcodeUsageInstructions() == th07::kTh07StdBinaryScriptInstructionCount);
    assert(th07::CountTh07StdBinaryOpcodeArgBytes() == th07::kTh07StdBinaryTotalArgByteCount);
    assert(std::popcount(th07::BuildTh07StdBinaryOpcodeMask()) == 25);
    assert(th07::CountTh07MsgEntries() == th07::kTh07MsgEntryCount);
    assert(th07::CountTh07MsgTimelineLabels() == th07::kTh07MsgTimelineLabelCount);
    assert(th07::CountTh07MsgBinaryEntryPointers() == th07::kTh07MsgBinaryEntryPointerCount);
    assert(th07::CountTh07MsgBinaryUniqueEntries() == th07::kTh07MsgBinaryUniqueEntryCount);
    assert(th07::CountTh07MsgBinaryInstructions() == th07::kTh07MsgBinaryInstructionCount);
    assert(th07::CountTh07MsgBinaryTextBytes() == th07::kTh07MsgBinaryTextByteCount);
    assert(th07::CountTh07MsgBinaryWaitTotalFrames() == th07::kTh07MsgBinaryWaitTotalFrames);
    assert(std::popcount(th07::BuildTh07MsgBinaryOpcodeMask()) == 15);
    assert(th07::CountTh07MsgBinaryOpcodeUsageInstructions() == th07::kTh07MsgBinaryInstructionCount);
    assert(th07::CountTh07MsgBinaryOpcodeArgBytes() == th07::kTh07MsgBinaryTotalArgByteCount);

    for (std::size_t idx = 0; idx < th07::kTh07StdBinarySummaries.size(); ++idx) {
        assert(th07::kTh07StdBinarySummaries[idx].stage == idx + 1);
    }
    for (std::size_t idx = 0; idx < th07::kTh07MsgDumpSummaries.size(); ++idx) {
        assert(th07::kTh07MsgDumpSummaries[idx].stage == idx + 1);
    }
    for (std::size_t idx = 0; idx < th07::kTh07MsgBinarySummaries.size(); ++idx) {
        assert(th07::kTh07MsgBinarySummaries[idx].stage == idx + 1);
    }

    const th07::Th07AnmFileSummary *player02 = th07::FindTh07AnmFileSummary("player02.anm");
    assert(player02 != nullptr);
    assert(player02->sprites == 17);
    assert(player02->scripts == 21);
    assert(player02->textureWidth == 256);
    assert(player02->textureHeight == 256);
    assert(player02->hasTextureData);

    const th07::Th07AnmFileSummary *stage4Enemy = th07::FindTh07AnmFileSummary("stg4enm.anm");
    assert(stage4Enemy != nullptr);
    assert(stage4Enemy->sprites == 87);
    assert(stage4Enemy->scripts == 68);

    const th07::Th07AnmFileSummary *capture = th07::FindTh07AnmFileSummary("capture.anm");
    assert(capture != nullptr);
    assert(!capture->hasTextureData);
    assert(th07::FindTh07AnmFileSummary("missing.anm") == nullptr);

    const th07::Th07AnmVmInstructionSizeSummary *anmSize12 = th07::FindTh07AnmVmInstructionSizeSummary(12);
    assert(anmSize12 != nullptr);
    assert(anmSize12->instructions == 2991);
    const th07::Th07AnmVmInstructionSizeSummary *anmSize28 = th07::FindTh07AnmVmInstructionSizeSummary(28);
    assert(anmSize28 != nullptr);
    assert(anmSize28->instructions == 294);
    assert(th07::FindTh07AnmVmInstructionSizeSummary(32) == nullptr);

    const th07::Th07AnmVmOpcodeUsage *anmOpcode3 = th07::FindTh07AnmVmOpcodeUsage(3);
    assert(anmOpcode3 != nullptr);
    assert(anmOpcode3->instructions == 1769);
    const th07::Th07AnmVmOpcodeUsage *anmOpcode34 = th07::FindTh07AnmVmOpcodeUsage(34);
    assert(anmOpcode34 != nullptr);
    assert(anmOpcode34->instructions == 331);
    const th07::Th07AnmVmOpcodeUsage *anmOpcode80 = th07::FindTh07AnmVmOpcodeUsage(80);
    assert(anmOpcode80 != nullptr);
    assert(anmOpcode80->instructions == 1);
    assert(th07::FindTh07AnmVmOpcodeUsage(11) == nullptr);
    const th07::Th07AnmVmOpcodeArgShape *anmOpcode3Shape =
        th07::FindTh07AnmVmOpcodeArgShape(3);
    assert(anmOpcode3Shape != nullptr);
    assert(anmOpcode3Shape->instructions == 1769);
    assert(anmOpcode3Shape->minArgSize == 4);
    assert(anmOpcode3Shape->maxArgSize == 4);
    assert(anmOpcode3Shape->totalArgBytes == 7076);
    const th07::Th07AnmVmOpcodeArgShape *anmOpcode32Shape =
        th07::FindTh07AnmVmOpcodeArgShape(32);
    assert(anmOpcode32Shape != nullptr);
    assert(anmOpcode32Shape->instructions == 221);
    assert(anmOpcode32Shape->minArgSize == 20);
    assert(anmOpcode32Shape->maxArgSize == 20);
    assert(anmOpcode32Shape->totalArgBytes == 4420);
    const th07::Th07AnmVmOpcodeArgShape *anmOpcode80Shape =
        th07::FindTh07AnmVmOpcodeArgShape(80);
    assert(anmOpcode80Shape != nullptr);
    assert(anmOpcode80Shape->instructions == 1);
    assert(anmOpcode80Shape->minArgSize == 4);
    assert(anmOpcode80Shape->maxArgSize == 4);
    assert(anmOpcode80Shape->totalArgBytes == 4);
    assert(th07::FindTh07AnmVmOpcodeArgShape(11) == nullptr);

    const th07::Th07StdBinarySummary *stage4 = th07::FindTh07StdBinarySummary(4);
    assert(stage4 != nullptr);
    assert(stage4->objects == 4);
    assert(stage4->instances == 157);
    assert(stage4->hasOpcode(13));
    assert(stage4->hasOpcode(29));
    assert(!stage4->hasOpcode(30));

    const th07::Th07StdBinarySummary *stage6 = th07::FindTh07StdBinarySummary(6);
    assert(stage6 != nullptr);
    assert(stage6->objects == 2);
    assert(stage6->quads == 4);
    assert(stage6->instances == 36);
    assert(stage6->scriptInstructions == 58);
    assert(!stage6->readableDumpAvailable);
    assert(stage6->hasOpcode(3));
    assert(stage6->hasOpcode(31));
    assert(!stage6->hasOpcode(2));

    assert(th07::FindTh07StdBinarySummary(0) == nullptr);
    assert(th07::FindTh07StdBinarySummary(9) == nullptr);

    const th07::Th07StdBinaryOpcodeUsage *stdOpcode5 = th07::FindTh07StdBinaryOpcodeUsage(5);
    assert(stdOpcode5 != nullptr);
    assert(stdOpcode5->instructions == 103);
    const th07::Th07StdBinaryOpcodeUsage *stdOpcode31 = th07::FindTh07StdBinaryOpcodeUsage(31);
    assert(stdOpcode31 != nullptr);
    assert(stdOpcode31->instructions == 5);
    assert(th07::FindTh07StdBinaryOpcodeUsage(10) == nullptr);
    const th07::Th07StdBinaryOpcodeArgShape *stdOpcode5Shape =
        th07::FindTh07StdBinaryOpcodeArgShape(5);
    assert(stdOpcode5Shape != nullptr);
    assert(stdOpcode5Shape->instructions == 103);
    assert(stdOpcode5Shape->minArgSize == 12);
    assert(stdOpcode5Shape->maxArgSize == 12);
    assert(stdOpcode5Shape->totalArgBytes == 1236);
    const th07::Th07StdBinaryOpcodeArgShape *stdOpcode31Shape =
        th07::FindTh07StdBinaryOpcodeArgShape(31);
    assert(stdOpcode31Shape != nullptr);
    assert(stdOpcode31Shape->instructions == 5);
    assert(stdOpcode31Shape->minArgSize == 12);
    assert(stdOpcode31Shape->maxArgSize == 12);
    assert(stdOpcode31Shape->totalArgBytes == 60);
    assert(th07::FindTh07StdBinaryOpcodeArgShape(10) == nullptr);

    const th07::Th07MsgDumpSummary *stage6Msg = th07::FindTh07MsgDumpSummary(6);
    assert(stage6Msg != nullptr);
    assert(stage6Msg->entries == 9);
    assert(stage6Msg->timelineLabels == 178);

    const th07::Th07MsgBinarySummary *stage6MsgBinary = th07::FindTh07MsgBinarySummary(6);
    assert(stage6MsgBinary != nullptr);
    assert(stage6MsgBinary->fileSize == 10516);
    assert(stage6MsgBinary->entryPointers == 23);
    assert(stage6MsgBinary->uniqueEntries == 9);
    assert(stage6MsgBinary->instructions == 759);
    assert(stage6MsgBinary->textBytes == 4468);
    assert(stage6MsgBinary->waitTotalFrames == 42720);
    assert(stage6MsgBinary->hasOpcode(14));

    const th07::Th07MsgDumpSummary *stage8Msg = th07::FindTh07MsgDumpSummary(8);
    assert(stage8Msg != nullptr);
    assert(stage8Msg->entries == 9);
    assert(stage8Msg->timelineLabels == 215);
    const th07::Th07MsgBinarySummary *stage8MsgBinary = th07::FindTh07MsgBinarySummary(8);
    assert(stage8MsgBinary != nullptr);
    assert(stage8MsgBinary->fileSize == 13024);
    assert(stage8MsgBinary->instructions == 953);
    assert(!stage8MsgBinary->hasOpcode(14));
    assert(th07::FindTh07MsgDumpSummary(0) == nullptr);
    assert(th07::FindTh07MsgDumpSummary(9) == nullptr);
    assert(th07::FindTh07MsgBinarySummary(0) == nullptr);
    assert(th07::FindTh07MsgBinarySummary(9) == nullptr);

    const th07::Th07MsgBinaryOpcodeUsage *msgOpcode3 = th07::FindTh07MsgBinaryOpcodeUsage(3);
    assert(msgOpcode3 != nullptr);
    assert(msgOpcode3->instructions == 1049);
    const th07::Th07MsgBinaryOpcodeUsage *msgOpcode5 = th07::FindTh07MsgBinaryOpcodeUsage(5);
    assert(msgOpcode5 != nullptr);
    assert(msgOpcode5->instructions == 1684);
    const th07::Th07MsgBinaryOpcodeUsage *msgOpcode14 = th07::FindTh07MsgBinaryOpcodeUsage(14);
    assert(msgOpcode14 != nullptr);
    assert(msgOpcode14->instructions == 3);
    assert(th07::FindTh07MsgBinaryOpcodeUsage(15) == nullptr);
    const th07::Th07MsgBinaryOpcodeArgShape *msgOpcode3Shape =
        th07::FindTh07MsgBinaryOpcodeArgShape(3);
    assert(msgOpcode3Shape != nullptr);
    assert(msgOpcode3Shape->instructions == 1049);
    assert(msgOpcode3Shape->minArgSize == 8);
    assert(msgOpcode3Shape->maxArgSize == 48);
    assert(msgOpcode3Shape->totalArgBytes == 30384);
    const th07::Th07MsgBinaryOpcodeArgShape *msgOpcode8Shape =
        th07::FindTh07MsgBinaryOpcodeArgShape(8);
    assert(msgOpcode8Shape != nullptr);
    assert(msgOpcode8Shape->instructions == 60);
    assert(msgOpcode8Shape->minArgSize == 24);
    assert(msgOpcode8Shape->maxArgSize == 40);
    assert(msgOpcode8Shape->totalArgBytes == 1884);
    const th07::Th07MsgBinaryOpcodeArgShape *msgOpcode12Shape =
        th07::FindTh07MsgBinaryOpcodeArgShape(12);
    assert(msgOpcode12Shape != nullptr);
    assert(msgOpcode12Shape->instructions == 24);
    assert(msgOpcode12Shape->minArgSize == 0);
    assert(msgOpcode12Shape->maxArgSize == 0);
    assert(msgOpcode12Shape->totalArgBytes == 0);
    const th07::Th07MsgBinaryOpcodeArgShape *msgOpcode13Shape =
        th07::FindTh07MsgBinaryOpcodeArgShape(13);
    assert(msgOpcode13Shape != nullptr);
    assert(msgOpcode13Shape->instructions == 124);
    assert(msgOpcode13Shape->minArgSize == 4);
    assert(msgOpcode13Shape->maxArgSize == 4);
    assert(msgOpcode13Shape->totalArgBytes == 496);
    assert(th07::FindTh07MsgBinaryOpcodeArgShape(15) == nullptr);

    return 0;
}
