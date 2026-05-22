#include "ReferencePaths.hpp"
#include "Th07AnmFile.hpp"
#include "Th07ResourceFormatTables.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>

namespace {

struct OpcodeShape {
    std::uint32_t instructions = 0;
    std::uint16_t minArgSize = std::numeric_limits<std::uint16_t>::max();
    std::uint16_t maxArgSize = 0;
    std::uint32_t totalArgBytes = 0;
};

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
    static_assert(th07::kTh07AnmHeaderMinimumSize == 0x00b8);
    static_assert(th07::kTh07AnmSpriteOffsetTableOffset == 0x0040);
    static_assert(th07::kTh07AnmSpriteRecordMinimumSize == 0x0014);
    static_assert(th07::kTh07AnmScriptTableEntrySize == 0x0008);
    static_assert(th07::kTh07AnmVmInstructionHeaderSize == 0x0008);
    static_assert(th07::kTh07AnmVmTerminatorOpcode == -1);
    static_assert(th07::kTh07AnmVmTerminatorSize == 0);
    static_assert(th07::kTh07AnmVmParserMaxOpcode == 0x0050);

    std::uint32_t totalSprites = 0;
    std::uint32_t totalScripts = 0;
    std::uint32_t totalInstructions = 0;
    std::uint32_t totalTerminators = 0;
    std::uint32_t totalArgBytes = 0;
    std::uint32_t filesWithTexture = 0;
    std::array<std::uint32_t, th07::kTh07AnmVmParserMaxOpcode + 1> opcodeCounts{};
    std::array<OpcodeShape, th07::kTh07AnmVmParserMaxOpcode + 1> opcodeShapes{};
    std::map<std::uint16_t, std::uint32_t> instructionSizeCounts;
    th07::Th07AnmVmOpcodeMask aggregateOpcodeMask;

    for (const th07::Th07AnmFileSummary &summary : th07::kTh07AnmFileSummaries) {
        const std::filesystem::path path =
            th07::ExtractedReferenceRoot() / std::string(summary.fileName);
        const std::vector<u8> bytes = ReadWholeFile(path);

        th07::Th07AnmFile file;
        std::string error;
        assert(th07::ParseTh07AnmFile(bytes, &file, &error));
        assert(error.empty());
        assert(file.header.spriteCount == summary.sprites);
        assert(file.header.scriptCount == summary.scripts);
        assert(file.header.textureWidth == summary.textureWidth);
        assert(file.header.textureHeight == summary.textureHeight);
        assert(file.header.version == summary.version);
        assert(file.header.hasTextureData == summary.hasTextureData);
        assert(file.sprites.size() == summary.sprites);
        assert(file.scriptEntries.size() == summary.scripts);
        assert(file.uniqueScripts.size() <= file.scriptEntries.size());

        totalSprites += file.sprites.size();
        totalScripts += file.scriptEntries.size();
        totalInstructions += th07::CountTh07AnmVmInstructions(file);
        totalTerminators += th07::CountTh07AnmVmTerminators(file);
        totalArgBytes += th07::CountTh07AnmVmArgumentBytes(file);
        if (file.header.hasTextureData) {
            filesWithTexture++;
        }

        aggregateOpcodeMask |= th07::BuildTh07AnmVmOpcodeMask(file);
        for (const th07::Th07AnmScript &script : file.uniqueScripts) {
            for (const th07::Th07AnmVmInstruction &instruction : script.instructions) {
                const std::uint16_t opcode = static_cast<std::uint16_t>(instruction.opcode);
                const std::uint16_t argSize =
                    instruction.byteSize - th07::kTh07AnmVmInstructionHeaderSize;
                opcodeCounts[opcode]++;
                OpcodeShape &shape = opcodeShapes[opcode];
                shape.instructions++;
                if (argSize < shape.minArgSize) {
                    shape.minArgSize = argSize;
                }
                if (argSize > shape.maxArgSize) {
                    shape.maxArgSize = argSize;
                }
                shape.totalArgBytes += argSize;
                instructionSizeCounts[instruction.byteSize]++;
            }
        }
    }

    assert(totalSprites == th07::kTh07AnmSpriteCount);
    assert(totalScripts == th07::kTh07AnmScriptCount);
    assert(totalInstructions == th07::kTh07AnmVmScriptInstructionCount);
    assert(totalTerminators == th07::kTh07AnmVmScriptTerminatorCount);
    assert(totalArgBytes == th07::kTh07AnmVmTotalArgByteCount);
    assert(filesWithTexture == th07::CountTh07AnmFilesWithTextureData());
    assert(aggregateOpcodeMask.count() == th07::kTh07AnmVmOpcodeCount);

    for (const th07::Th07AnmVmInstructionSizeSummary &sizeSummary :
         th07::kTh07AnmVmInstructionSizeSummaries) {
        assert(instructionSizeCounts[sizeSummary.size] == sizeSummary.instructions);
    }
    for (const th07::Th07AnmVmOpcodeUsage &usage : th07::kTh07AnmVmOpcodeUsage) {
        assert(opcodeCounts[usage.opcode] == usage.instructions);
        assert(aggregateOpcodeMask.test(usage.opcode));
    }
    for (const th07::Th07AnmVmOpcodeArgShape &shapeSummary : th07::kTh07AnmVmOpcodeArgShapes) {
        const OpcodeShape &shape = opcodeShapes[shapeSummary.opcode];
        assert(shape.instructions == shapeSummary.instructions);
        assert(shape.minArgSize == shapeSummary.minArgSize);
        assert(shape.maxArgSize == shapeSummary.maxArgSize);
        assert(shape.totalArgBytes == shapeSummary.totalArgBytes);
    }

    const std::vector<u8> etamaBytes = ReadWholeFile(th07::ExtractedReferenceRoot() / "etama.anm");
    th07::Th07AnmFile etama;
    assert(th07::ParseTh07AnmFile(etamaBytes, &etama));
    assert(etama.header.spriteCount == 168);
    assert(etama.header.scriptCount == 25);
    assert(etama.header.textureOffset == 0x1634);
    const th07::Th07AnmSprite *etamaSprite0 = th07::FindTh07AnmSprite(etama, 0);
    assert(etamaSprite0 != nullptr);
    assert(static_cast<int>(etamaSprite0->width) == 8);
    assert(static_cast<int>(etamaSprite0->height) == 8);
    const th07::Th07AnmScript *etamaScript0 = th07::FindTh07AnmScript(etama, 0);
    assert(etamaScript0 != nullptr);
    assert(etamaScript0->offset == 0x10e8);
    assert(etamaScript0->instructions.size() == 2);
    assert(etamaScript0->instructions[0].opcode == 3);
    assert(etamaScript0->instructions[0].byteSize == 12);
    assert(etamaScript0->instructions[1].opcode == 2);

    const std::vector<u8> playerBytes = ReadWholeFile(th07::ExtractedReferenceRoot() / "player02.anm");
    th07::Th07AnmFile player;
    assert(th07::ParseTh07AnmFile(playerBytes, &player));
    assert(player.header.spriteCount == 17);
    assert(player.header.scriptCount == 21);
    assert(player.header.textureOffset == 0x0bec);
    assert(th07::CountTh07AnmVmOpcode(player, 3) == 56);
    assert(th07::CountTh07AnmVmInstructionSize(player, 12) == 101);
    const th07::Th07AnmScript *playerScript0 = th07::FindTh07AnmScript(player, 0);
    assert(playerScript0 != nullptr);
    assert(playerScript0->instructions[4].opcode == 4);
    assert(playerScript0->instructions[4].byteSize == 16);

    const std::vector<u8> captureBytes = ReadWholeFile(th07::ExtractedReferenceRoot() / "capture.anm");
    th07::Th07AnmFile capture;
    assert(th07::ParseTh07AnmFile(captureBytes, &capture));
    assert(!capture.header.hasTextureData);
    assert(capture.header.textureOffset == 0);
    assert(capture.uniqueScripts.size() == 4);

    return 0;
}
