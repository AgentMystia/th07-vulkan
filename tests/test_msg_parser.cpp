#include "ReferencePaths.hpp"
#include "Th07MsgFile.hpp"
#include "Th07ResourceFormatTables.hpp"

#include <cassert>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::vector<u8> ReadWholeFile(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::binary);
    assert(file.good());
    return std::vector<u8>(std::istreambuf_iterator<char>(file),
                           std::istreambuf_iterator<char>());
}

} // namespace

int main()
{
    static_assert(th07::kTh07MsgHeaderSize == 0x0004);
    static_assert(th07::kTh07MsgEntryPointerSize == 0x0004);
    static_assert(th07::kTh07MsgInstructionHeaderSize == 0x0004);
    static_assert(th07::kTh07MsgMaxEntryPointers == 64);
    static_assert(th07::kTh07MsgMaxOpcode == 0x0e);
    static_assert(th07::kTh07MsgOpcodeTextDialogue == 0x03);
    static_assert(th07::kTh07MsgOpcodeTextIntro == 0x08);
    static_assert(th07::kTh07MsgOpcodeScreenFade == 0x0e);

    std::size_t totalEntryPointers = 0;
    std::size_t totalUniqueEntries = 0;
    std::size_t totalInstructions = 0;
    std::uint32_t totalTextBytes = 0;
    std::uint32_t totalWaitFrames = 0;

    for (std::uint8_t stage = 1; stage <= th07::kTh07MsgDumpCount; stage++) {
        const th07::Th07MsgBinarySummary *summary = th07::FindTh07MsgBinarySummary(stage);
        assert(summary != nullptr);

        const std::filesystem::path path =
            th07::ExtractedReferenceRoot() / ("msg" + std::to_string(stage) + ".dat");
        const std::vector<u8> bytes = ReadWholeFile(path);

        th07::Th07MsgFile file;
        std::string error;
        assert(th07::ParseTh07MsgFile(bytes, &file, &error));
        assert(error.empty());
        assert(bytes.size() == summary->fileSize);
        assert(file.entryOffsets.size() == summary->entryPointers);
        assert(file.uniqueEntries.size() == summary->uniqueEntries);
        assert(th07::CountTh07MsgInstructions(file) == summary->instructions);
        assert(th07::BuildTh07MsgOpcodeMask(file) == summary->opcodeMask);
        assert(th07::CountTh07MsgTextBytes(file) == summary->textBytes);
        assert(th07::CountTh07MsgWaitTotalFrames(file) == summary->waitTotalFrames);

        totalEntryPointers += file.entryOffsets.size();
        totalUniqueEntries += file.uniqueEntries.size();
        totalInstructions += th07::CountTh07MsgInstructions(file);
        totalTextBytes += th07::CountTh07MsgTextBytes(file);
        totalWaitFrames += th07::CountTh07MsgWaitTotalFrames(file);
    }

    assert(totalEntryPointers == th07::kTh07MsgBinaryEntryPointerCount);
    assert(totalUniqueEntries == th07::kTh07MsgBinaryUniqueEntryCount);
    assert(totalInstructions == th07::kTh07MsgBinaryInstructionCount);
    assert(totalTextBytes == th07::kTh07MsgBinaryTextByteCount);
    assert(totalWaitFrames == th07::kTh07MsgBinaryWaitTotalFrames);

    const std::vector<u8> stage1Bytes =
        ReadWholeFile(th07::ExtractedReferenceRoot() / "msg1.dat");
    th07::Th07MsgFile stage1;
    assert(th07::ParseTh07MsgFile(stage1Bytes, &stage1));
    assert(stage1.entryOffsets.size() == 22);
    assert(stage1.uniqueEntries.size() == 6);
    assert(stage1.entryOffsets[0] == 0x005c);
    assert(stage1.entryOffsets[1] == 0x0464);
    const th07::Th07MsgEntry *stage1Entry0 = th07::FindTh07MsgEntry(stage1, 0);
    assert(stage1Entry0 != nullptr);
    assert(th07::FindTh07MsgEntry(stage1, 2) == stage1Entry0);
    assert(stage1Entry0->instructions[0].opcode == th07::kTh07MsgOpcodePortraitAnmScript);
    assert(stage1Entry0->instructions[1].opcode == th07::kTh07MsgOpcodePortraitAnmSprite);
    assert(stage1Entry0->instructions[3].time == 60);
    assert(stage1Entry0->instructions[3].opcode == th07::kTh07MsgOpcodeTextDialogue);
    assert(stage1Entry0->instructions[3].argumentSize == 12);

    const std::vector<u8> stage6Bytes =
        ReadWholeFile(th07::ExtractedReferenceRoot() / "msg6.dat");
    th07::Th07MsgFile stage6;
    assert(th07::ParseTh07MsgFile(stage6Bytes, &stage6));
    assert(stage6.entryOffsets.size() == 23);
    assert(stage6.uniqueEntries.size() == 9);
    assert(stage6.entryOffsets[0] == 0x0060);
    assert(stage6.entryOffsets[1] == 0x25c0);
    assert(stage6.uniqueEntries[3].offset == 0x25c0);
    assert(th07::CountTh07MsgOpcode(stage6, th07::kTh07MsgOpcodeScreenFade) == 3);
    assert(th07::FindTh07MsgEntryByOffset(stage6, 0x25c0) == th07::FindTh07MsgEntry(stage6, 1));
    assert(th07::FindTh07MsgEntry(stage6, 23) == nullptr);

    return 0;
}
