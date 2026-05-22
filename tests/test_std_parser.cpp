#include "ReferencePaths.hpp"
#include "Th07ResourceFormatTables.hpp"
#include "Th07StdFile.hpp"

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
    static_assert(th07::kTh07StdHeaderSize == 0x0490);
    static_assert(th07::kTh07StdObjectOffsetTableOffset == 0x0490);
    static_assert(th07::kTh07StdObjectHeaderSize == 0x001c);
    static_assert(th07::kTh07StdObjectFirstQuadOffset == 0x001c);
    static_assert(th07::kTh07StdQuadMinimumSize == 0x001c);
    static_assert(th07::kTh07StdObjectInstanceSize == 0x0010);
    static_assert(th07::kTh07StdScriptInstructionSize == 0x0014);
    static_assert(th07::kTh07StdScriptArgumentSize == 0x000c);
    static_assert(th07::kTh07StdFixedStringSize == 0x0080);
    static_assert(th07::kTh07StdSongCount == 4);

    std::size_t totalObjects = 0;
    std::size_t totalQuads = 0;
    std::size_t totalInstances = 0;
    std::size_t totalInstructions = 0;

    for (std::uint8_t stage = 1; stage <= th07::kTh07StdFileCount; stage++) {
        const th07::Th07StdBinarySummary *summary = th07::FindTh07StdBinarySummary(stage);
        assert(summary != nullptr);

        const std::filesystem::path path =
            th07::ExtractedReferenceRoot() / ("stage" + std::to_string(stage) + ".std");
        const std::vector<u8> bytes = ReadWholeFile(path);

        th07::Th07StdFile file;
        std::string error;
        assert(th07::ParseTh07StdFile(bytes, &file, &error));
        assert(error.empty());
        assert(file.header.objectCount == summary->objects);
        assert(file.header.declaredQuadCount == summary->quads);
        assert(file.objects.size() == summary->objects);
        assert(th07::CountTh07StdQuads(file) == summary->quads);
        assert(file.instances.size() == summary->instances);
        assert(file.script.size() == summary->scriptInstructions);
        assert(th07::BuildTh07StdScriptOpcodeMask(file) == summary->opcodeMask);
        assert(file.header.instancesOffset >= th07::kTh07StdHeaderSize);
        assert(file.header.scriptOffset >= file.header.instancesOffset);

        totalObjects += file.objects.size();
        totalQuads += th07::CountTh07StdQuads(file);
        totalInstances += file.instances.size();
        totalInstructions += file.script.size();
    }

    assert(totalObjects == th07::kTh07StdBinaryObjectCount);
    assert(totalQuads == th07::kTh07StdBinaryQuadCount);
    assert(totalInstances == th07::kTh07StdBinaryInstanceCount);
    assert(totalInstructions == th07::kTh07StdBinaryScriptInstructionCount);

    const std::vector<u8> stage1Bytes =
        ReadWholeFile(th07::ExtractedReferenceRoot() / "stage1.std");
    th07::Th07StdFile stage1;
    assert(th07::ParseTh07StdFile(stage1Bytes, &stage1));
    assert(stage1.header.stageName.find("Pseudo Winter") != std::string::npos);
    assert(stage1.header.songNames[0].find("Deep Mountain") != std::string::npos);
    assert(stage1.header.songPaths[0] == "bgm/th07_02.mid");
    assert(stage1.header.songPaths[1] == "bgm/th07_03.mid");
    assert(th07::FindTh07StdObject(stage1, 0) != nullptr);
    assert(th07::CountTh07StdScriptOpcode(stage1, 5) == 13);

    const std::vector<u8> stage6Bytes =
        ReadWholeFile(th07::ExtractedReferenceRoot() / "stage6.std");
    th07::Th07StdFile stage6;
    assert(th07::ParseTh07StdFile(stage6Bytes, &stage6));
    assert(stage6.header.objectCount == 2);
    assert(stage6.header.declaredQuadCount == 4);
    assert(stage6.header.instancesOffset == 0x0548);
    assert(stage6.header.scriptOffset == 0x0798);
    assert(stage6.instances.size() == 36);
    assert(stage6.script.size() == 58);
    assert(stage6.header.songNames[0].find("Chinese Tea") != std::string::npos);
    assert(stage6.header.songPaths[0] == "bgm/th07_12.mid");
    assert(stage6.header.songPaths[1] == "bgm/th07_13.mid");
    assert(th07::CountTh07StdScriptOpcode(stage6, 5) == 20);
    assert(th07::CountTh07StdScriptOpcode(stage6, 29) == 5);
    assert(th07::CountTh07StdScriptOpcode(stage6, 30) == 1);
    assert(th07::CountTh07StdScriptOpcode(stage6, 31) == 5);

    return 0;
}
