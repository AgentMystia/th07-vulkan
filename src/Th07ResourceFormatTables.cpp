#include "Th07ResourceFormatTables.hpp"

#include <initializer_list>

namespace th07 {
namespace {

std::uint64_t OpcodeMask(std::initializer_list<std::uint8_t> opcodes)
{
    std::uint64_t mask = 0;
    for (std::uint8_t opcode : opcodes) {
        mask |= 1ull << opcode;
    }
    return mask;
}

std::uint32_t MsgOpcodeMask(std::initializer_list<std::uint8_t> opcodes)
{
    std::uint32_t mask = 0;
    for (std::uint8_t opcode : opcodes) {
        mask |= 1u << opcode;
    }
    return mask;
}

} // namespace

const std::array<Th07AnmFileSummary, kTh07AnmFileCount> kTh07AnmFileSummaries = {{
    {"ascii.anm", 146, 8, 256, 256, 2, true},
    {"capture.anm", 3, 4, 512, 512, 2, false},
    {"eff01.anm", 1, 1, 256, 256, 2, true},
    {"eff02.anm", 1, 1, 256, 256, 2, true},
    {"eff03.anm", 1, 1, 256, 256, 2, true},
    {"eff04.anm", 1, 1, 256, 256, 2, true},
    {"eff04b.anm", 1, 1, 256, 256, 2, true},
    {"eff05.anm", 1, 1, 256, 256, 2, true},
    {"eff06.anm", 1, 1, 256, 256, 2, true},
    {"eff07.anm", 1, 1, 256, 256, 2, true},
    {"eff08.anm", 1, 1, 256, 256, 2, true},
    {"etama.anm", 168, 25, 256, 256, 2, true},
    {"face_01_00.anm", 5, 0, 512, 512, 2, true},
    {"face_02_00.anm", 5, 0, 512, 512, 2, true},
    {"face_03_00.anm", 5, 0, 512, 512, 2, true},
    {"face_04_00.anm", 4, 0, 512, 512, 2, true},
    {"face_05_00.anm", 2, 0, 512, 512, 2, true},
    {"face_06_00.anm", 2, 0, 512, 512, 2, true},
    {"face_07_00.anm", 4, 0, 512, 512, 2, true},
    {"face_08_00.anm", 2, 0, 512, 512, 2, true},
    {"face_mr00.anm", 5, 8, 512, 512, 2, true},
    {"face_rm00.anm", 5, 8, 512, 512, 2, true},
    {"face_sk00.anm", 5, 8, 512, 512, 2, true},
    {"front.anm", 14, 14, 256, 256, 2, true},
    {"loading.anm", 1, 1, 512, 512, 2, true},
    {"loading2.anm", 1, 1, 512, 512, 2, true},
    {"loading3.anm", 1, 1, 512, 512, 2, true},
    {"music00.anm", 1, 1, 256, 256, 2, true},
    {"player00.anm", 31, 35, 256, 256, 2, true},
    {"player01.anm", 29, 41, 256, 256, 2, true},
    {"player02.anm", 17, 21, 256, 256, 2, true},
    {"result00.anm", 9, 9, 256, 256, 2, true},
    {"staff01.anm", 15, 9, 512, 512, 2, true},
    {"std1txt.anm", 6, 5, 512, 256, 2, true},
    {"std2txt.anm", 6, 5, 512, 256, 2, true},
    {"std3txt.anm", 6, 5, 512, 256, 2, true},
    {"std4txt.anm", 6, 5, 512, 256, 2, true},
    {"std5txt.anm", 6, 5, 512, 256, 2, true},
    {"std6txt.anm", 6, 6, 512, 256, 2, true},
    {"std7txt.anm", 6, 5, 512, 256, 2, true},
    {"std8txt.anm", 6, 5, 512, 256, 2, true},
    {"stg1bg.anm", 3, 3, 512, 512, 2, true},
    {"stg1enm.anm", 48, 24, 512, 512, 2, true},
    {"stg2bg.anm", 7, 9, 512, 512, 2, true},
    {"stg2enm.anm", 40, 20, 512, 512, 2, true},
    {"stg3bg.anm", 6, 6, 512, 512, 2, true},
    {"stg3enm.anm", 52, 31, 512, 512, 2, true},
    {"stg4bg.anm", 2, 2, 512, 256, 2, true},
    {"stg4bg2.anm", 1, 1, 512, 256, 2, true},
    {"stg4bg3.anm", 1, 1, 256, 256, 2, true},
    {"stg4bg4.anm", 3, 5, 512, 512, 2, true},
    {"stg4bg5.anm", 1, 1, 256, 256, 2, true},
    {"stg4enm.anm", 87, 68, 512, 512, 2, true},
    {"stg5bg.anm", 15, 26, 512, 512, 2, true},
    {"stg5enm.anm", 80, 53, 512, 512, 2, true},
    {"stg6bg.anm", 4, 5, 512, 512, 2, true},
    {"stg6enm.anm", 79, 51, 512, 512, 2, true},
    {"stg7bg.anm", 15, 26, 512, 512, 2, true},
    {"stg7enm.anm", 79, 46, 512, 512, 2, true},
    {"stg8bg.anm", 15, 26, 512, 512, 2, true},
    {"stg8enm.anm", 75, 48, 512, 512, 2, true},
    {"text.anm", 36, 30, 512, 256, 2, false},
    {"title01.anm", 1, 1, 512, 256, 2, true},
}};

const std::array<std::uint8_t, kTh07AnmVmOpcodeCount> kTh07AnmVmOpcodes = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 50, 52, 55, 59, 60, 69, 79, 80,
};

const std::array<Th07AnmVmOpcodeUsage, kTh07AnmVmOpcodeUsageCount> kTh07AnmVmOpcodeUsage = {{
    {0, 89},
    {1, 250},
    {2, 233},
    {3, 1769},
    {4, 329},
    {5, 36},
    {6, 224},
    {7, 157},
    {8, 325},
    {9, 74},
    {10, 68},
    {12, 155},
    {13, 169},
    {14, 31},
    {15, 201},
    {16, 78},
    {17, 20},
    {18, 22},
    {19, 36},
    {20, 115},
    {21, 309},
    {22, 163},
    {23, 76},
    {24, 110},
    {25, 55},
    {26, 1},
    {27, 28},
    {29, 4},
    {30, 115},
    {31, 108},
    {32, 221},
    {33, 19},
    {34, 331},
    {35, 73},
    {36, 93},
    {37, 24},
    {50, 32},
    {52, 20},
    {55, 12},
    {59, 1},
    {60, 16},
    {69, 3},
    {79, 18},
    {80, 1},
}};

const std::array<Th07AnmVmOpcodeArgShape, kTh07AnmVmOpcodeArgShapeCount> kTh07AnmVmOpcodeArgShapes = {{
    {0, 89, 0, 0, 0},
    {1, 250, 0, 0, 0},
    {2, 233, 0, 0, 0},
    {3, 1769, 4, 4, 7076},
    {4, 329, 8, 8, 2632},
    {5, 36, 12, 12, 432},
    {6, 224, 12, 12, 2688},
    {7, 157, 8, 8, 1256},
    {8, 325, 4, 4, 1300},
    {9, 74, 4, 4, 296},
    {10, 68, 0, 0, 0},
    {12, 155, 12, 12, 1860},
    {13, 169, 12, 12, 2028},
    {14, 31, 8, 8, 248},
    {15, 201, 8, 8, 1608},
    {16, 78, 4, 4, 312},
    {17, 20, 16, 16, 320},
    {18, 22, 16, 16, 352},
    {19, 36, 16, 16, 576},
    {20, 115, 0, 0, 0},
    {21, 309, 4, 4, 1236},
    {22, 163, 0, 0, 0},
    {23, 76, 0, 0, 0},
    {24, 110, 4, 4, 440},
    {25, 55, 4, 4, 220},
    {26, 1, 4, 4, 4},
    {27, 28, 4, 4, 112},
    {29, 4, 12, 12, 48},
    {30, 115, 4, 4, 460},
    {31, 108, 4, 4, 432},
    {32, 221, 20, 20, 4420},
    {33, 19, 12, 12, 228},
    {34, 331, 12, 12, 3972},
    {35, 73, 20, 20, 1460},
    {36, 93, 16, 16, 1488},
    {37, 24, 8, 8, 192},
    {50, 32, 12, 12, 384},
    {52, 20, 12, 12, 240},
    {55, 12, 12, 12, 144},
    {59, 1, 8, 8, 8},
    {60, 16, 8, 8, 128},
    {69, 3, 16, 16, 48},
    {79, 18, 4, 4, 72},
    {80, 1, 4, 4, 4},
}};

const std::array<Th07AnmVmInstructionSizeSummary, kTh07AnmVmInstructionSizeClassCount>
    kTh07AnmVmInstructionSizeSummaries = {{
        {8, 994},
        {12, 2991},
        {16, 759},
        {20, 1002},
        {24, 174},
        {28, 294},
    }};

const std::array<Th07StdBinarySummary, kTh07StdFileCount> kTh07StdBinarySummaries = {{
    {1, 2, 19, 59, 39, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 8, 9, 11}), true},
    {2, 8, 35, 76, 34, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 9, 11}), true},
    {3, 5, 5, 92, 41, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 8, 9, 11}), true},
    {4, 4, 10, 157, 88, OpcodeMask({0, 1, 2, 5, 6, 7, 8, 9, 11, 13, 24, 25, 26, 27, 28, 29}), true},
    {5, 8, 29, 162, 38, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 8, 9, 11, 14, 15, 16, 17, 18}), true},
    {6, 2, 4, 36, 58, OpcodeMask({0, 1, 3, 4, 5, 6, 7, 9, 11, 29, 30, 31}), false},
    {7, 8, 30, 162, 40, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 8, 9, 11}), true},
    {8, 8, 30, 162, 40, OpcodeMask({0, 1, 2, 4, 5, 6, 7, 8, 9, 11}), true},
}};

const std::array<Th07StdBinaryOpcodeUsage, kTh07StdBinaryOpcodeUsageCount> kTh07StdBinaryOpcodeUsage = {{
    {0, 15},
    {1, 29},
    {2, 21},
    {3, 2},
    {4, 10},
    {5, 103},
    {6, 66},
    {7, 22},
    {8, 14},
    {9, 8},
    {11, 8},
    {13, 1},
    {14, 3},
    {15, 3},
    {16, 3},
    {17, 3},
    {18, 3},
    {24, 10},
    {25, 10},
    {26, 10},
    {27, 10},
    {28, 10},
    {29, 8},
    {30, 1},
    {31, 5},
}};

const std::array<Th07StdBinaryOpcodeArgShape, kTh07StdBinaryOpcodeArgShapeCount>
    kTh07StdBinaryOpcodeArgShapes = {{
        {0, 15, 12, 12, 180},
        {1, 29, 12, 12, 348},
        {2, 21, 12, 12, 252},
        {3, 2, 12, 12, 24},
        {4, 10, 12, 12, 120},
        {5, 103, 12, 12, 1236},
        {6, 66, 12, 12, 792},
        {7, 22, 12, 12, 264},
        {8, 14, 12, 12, 168},
        {9, 8, 12, 12, 96},
        {11, 8, 12, 12, 96},
        {13, 1, 12, 12, 12},
        {14, 3, 12, 12, 36},
        {15, 3, 12, 12, 36},
        {16, 3, 12, 12, 36},
        {17, 3, 12, 12, 36},
        {18, 3, 12, 12, 36},
        {24, 10, 12, 12, 120},
        {25, 10, 12, 12, 120},
        {26, 10, 12, 12, 120},
        {27, 10, 12, 12, 120},
        {28, 10, 12, 12, 120},
        {29, 8, 12, 12, 96},
        {30, 1, 12, 12, 12},
        {31, 5, 12, 12, 60},
    }};

const std::array<Th07MsgDumpSummary, kTh07MsgDumpCount> kTh07MsgDumpSummaries = {{
    {1, 6, 60},
    {2, 6, 67},
    {3, 6, 96},
    {4, 6, 118},
    {5, 9, 150},
    {6, 9, 178},
    {7, 9, 214},
    {8, 9, 215},
}};

const std::array<Th07MsgBinarySummary, kTh07MsgDumpCount> kTh07MsgBinarySummaries = {{
    {1, 3564, 22, 6, 249, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 1564, 17070},
    {2, 3948, 22, 6, 301, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 1532, 20460},
    {3, 6000, 22, 6, 447, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 2416, 25770},
    {4, 7768, 22, 6, 581, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 3124, 32490},
    {5, 8880, 23, 9, 638, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 3788, 34260},
    {6, 10516, 23, 9, 759, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}), 4468, 42720},
    {7, 13156, 23, 9, 955, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 5528, 69880},
    {8, 13024, 23, 9, 953, MsgOpcodeMask({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}), 5412, 70680},
}};

const std::array<Th07MsgBinaryOpcodeUsage, kTh07MsgBinaryOpcodeUsageCount> kTh07MsgBinaryOpcodeUsage = {{
    {0, 96},
    {1, 105},
    {2, 758},
    {3, 1049},
    {4, 857},
    {5, 1684},
    {6, 27},
    {7, 24},
    {8, 60},
    {9, 24},
    {10, 24},
    {11, 24},
    {12, 24},
    {13, 124},
    {14, 3},
}};

const std::array<Th07MsgBinaryOpcodeArgShape, kTh07MsgBinaryOpcodeArgShapeCount>
    kTh07MsgBinaryOpcodeArgShapes = {{
        {0, 96, 0, 0, 0},
        {1, 105, 4, 4, 420},
        {2, 758, 4, 4, 3032},
        {3, 1049, 8, 48, 30384},
        {4, 857, 4, 4, 3428},
        {5, 1684, 4, 4, 6736},
        {6, 27, 0, 0, 0},
        {7, 24, 4, 4, 96},
        {8, 60, 24, 40, 1884},
        {9, 24, 4, 4, 96},
        {10, 24, 0, 0, 0},
        {11, 24, 0, 0, 0},
        {12, 24, 0, 0, 0},
        {13, 124, 4, 4, 496},
        {14, 3, 0, 0, 0},
    }};

const Th07AnmFileSummary *FindTh07AnmFileSummary(std::string_view fileName)
{
    for (const Th07AnmFileSummary &summary : kTh07AnmFileSummaries) {
        if (summary.fileName == fileName) {
            return &summary;
        }
    }
    return nullptr;
}

const Th07AnmVmInstructionSizeSummary *FindTh07AnmVmInstructionSizeSummary(std::uint16_t size)
{
    for (const Th07AnmVmInstructionSizeSummary &summary : kTh07AnmVmInstructionSizeSummaries) {
        if (summary.size == size) {
            return &summary;
        }
    }
    return nullptr;
}

const Th07AnmVmOpcodeUsage *FindTh07AnmVmOpcodeUsage(std::uint8_t opcode)
{
    for (const Th07AnmVmOpcodeUsage &usage : kTh07AnmVmOpcodeUsage) {
        if (usage.opcode == opcode) {
            return &usage;
        }
    }
    return nullptr;
}

const Th07AnmVmOpcodeArgShape *FindTh07AnmVmOpcodeArgShape(std::uint8_t opcode)
{
    for (const Th07AnmVmOpcodeArgShape &shape : kTh07AnmVmOpcodeArgShapes) {
        if (shape.opcode == opcode) {
            return &shape;
        }
    }
    return nullptr;
}

bool IsTh07AnmVmOpcodeUsed(std::uint8_t opcode)
{
    for (std::uint8_t usedOpcode : kTh07AnmVmOpcodes) {
        if (usedOpcode == opcode) {
            return true;
        }
    }
    return false;
}

bool Th07StdBinarySummary::hasOpcode(std::uint8_t opcode) const
{
    return opcode < 64 && (opcodeMask & (1ull << opcode)) != 0;
}

const Th07StdBinarySummary *FindTh07StdBinarySummary(std::uint8_t stage)
{
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        if (summary.stage == stage) {
            return &summary;
        }
    }
    return nullptr;
}

const Th07StdBinaryOpcodeUsage *FindTh07StdBinaryOpcodeUsage(std::uint8_t opcode)
{
    for (const Th07StdBinaryOpcodeUsage &usage : kTh07StdBinaryOpcodeUsage) {
        if (usage.opcode == opcode) {
            return &usage;
        }
    }
    return nullptr;
}

const Th07StdBinaryOpcodeArgShape *FindTh07StdBinaryOpcodeArgShape(std::uint8_t opcode)
{
    for (const Th07StdBinaryOpcodeArgShape &shape : kTh07StdBinaryOpcodeArgShapes) {
        if (shape.opcode == opcode) {
            return &shape;
        }
    }
    return nullptr;
}

const Th07MsgDumpSummary *FindTh07MsgDumpSummary(std::uint8_t stage)
{
    for (const Th07MsgDumpSummary &summary : kTh07MsgDumpSummaries) {
        if (summary.stage == stage) {
            return &summary;
        }
    }
    return nullptr;
}

bool Th07MsgBinarySummary::hasOpcode(std::uint8_t opcode) const
{
    return opcode < 32 && (opcodeMask & (1u << opcode)) != 0;
}

const Th07MsgBinarySummary *FindTh07MsgBinarySummary(std::uint8_t stage)
{
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        if (summary.stage == stage) {
            return &summary;
        }
    }
    return nullptr;
}

const Th07MsgBinaryOpcodeUsage *FindTh07MsgBinaryOpcodeUsage(std::uint8_t opcode)
{
    for (const Th07MsgBinaryOpcodeUsage &usage : kTh07MsgBinaryOpcodeUsage) {
        if (usage.opcode == opcode) {
            return &usage;
        }
    }
    return nullptr;
}

const Th07MsgBinaryOpcodeArgShape *FindTh07MsgBinaryOpcodeArgShape(std::uint8_t opcode)
{
    for (const Th07MsgBinaryOpcodeArgShape &shape : kTh07MsgBinaryOpcodeArgShapes) {
        if (shape.opcode == opcode) {
            return &shape;
        }
    }
    return nullptr;
}

std::uint16_t CountTh07AnmSprites()
{
    std::uint16_t count = 0;
    for (const Th07AnmFileSummary &summary : kTh07AnmFileSummaries) {
        count += summary.sprites;
    }
    return count;
}

std::uint16_t CountTh07AnmScripts()
{
    std::uint16_t count = 0;
    for (const Th07AnmFileSummary &summary : kTh07AnmFileSummaries) {
        count += summary.scripts;
    }
    return count;
}

std::uint16_t CountTh07AnmFilesWithTextureData()
{
    std::uint16_t count = 0;
    for (const Th07AnmFileSummary &summary : kTh07AnmFileSummaries) {
        if (summary.hasTextureData) {
            count++;
        }
    }
    return count;
}

std::uint16_t CountTh07AnmVmScriptInstructionsBySize()
{
    std::uint16_t count = 0;
    for (const Th07AnmVmInstructionSizeSummary &summary : kTh07AnmVmInstructionSizeSummaries) {
        count += summary.instructions;
    }
    return count;
}

std::uint16_t CountTh07AnmVmOpcodeUsageInstructions()
{
    std::uint16_t count = 0;
    for (const Th07AnmVmOpcodeUsage &usage : kTh07AnmVmOpcodeUsage) {
        count += usage.instructions;
    }
    return count;
}

std::uint16_t CountTh07AnmVmOpcodeArgBytes()
{
    std::uint16_t count = 0;
    for (const Th07AnmVmOpcodeArgShape &shape : kTh07AnmVmOpcodeArgShapes) {
        count += shape.totalArgBytes;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryObjects()
{
    std::uint16_t count = 0;
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        count += summary.objects;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryQuads()
{
    std::uint16_t count = 0;
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        count += summary.quads;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryInstances()
{
    std::uint16_t count = 0;
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        count += summary.instances;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryScriptInstructions()
{
    std::uint16_t count = 0;
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        count += summary.scriptInstructions;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryOpcodeUsageInstructions()
{
    std::uint16_t count = 0;
    for (const Th07StdBinaryOpcodeUsage &usage : kTh07StdBinaryOpcodeUsage) {
        count += usage.instructions;
    }
    return count;
}

std::uint16_t CountTh07StdBinaryOpcodeArgBytes()
{
    std::uint16_t count = 0;
    for (const Th07StdBinaryOpcodeArgShape &shape : kTh07StdBinaryOpcodeArgShapes) {
        count += shape.totalArgBytes;
    }
    return count;
}

std::uint64_t BuildTh07StdBinaryOpcodeMask()
{
    std::uint64_t mask = 0;
    for (const Th07StdBinarySummary &summary : kTh07StdBinarySummaries) {
        mask |= summary.opcodeMask;
    }
    return mask;
}

std::uint16_t CountTh07MsgEntries()
{
    std::uint16_t count = 0;
    for (const Th07MsgDumpSummary &summary : kTh07MsgDumpSummaries) {
        count += summary.entries;
    }
    return count;
}

std::uint16_t CountTh07MsgTimelineLabels()
{
    std::uint16_t count = 0;
    for (const Th07MsgDumpSummary &summary : kTh07MsgDumpSummaries) {
        count += summary.timelineLabels;
    }
    return count;
}

std::uint16_t CountTh07MsgBinaryEntryPointers()
{
    std::uint16_t count = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        count += summary.entryPointers;
    }
    return count;
}

std::uint16_t CountTh07MsgBinaryUniqueEntries()
{
    std::uint16_t count = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        count += summary.uniqueEntries;
    }
    return count;
}

std::uint16_t CountTh07MsgBinaryInstructions()
{
    std::uint16_t count = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        count += summary.instructions;
    }
    return count;
}

std::uint16_t CountTh07MsgBinaryTextBytes()
{
    std::uint16_t count = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        count += summary.textBytes;
    }
    return count;
}

std::uint32_t CountTh07MsgBinaryWaitTotalFrames()
{
    std::uint32_t count = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        count += summary.waitTotalFrames;
    }
    return count;
}

std::uint32_t BuildTh07MsgBinaryOpcodeMask()
{
    std::uint32_t mask = 0;
    for (const Th07MsgBinarySummary &summary : kTh07MsgBinarySummaries) {
        mask |= summary.opcodeMask;
    }
    return mask;
}

std::uint16_t CountTh07MsgBinaryOpcodeUsageInstructions()
{
    std::uint16_t count = 0;
    for (const Th07MsgBinaryOpcodeUsage &usage : kTh07MsgBinaryOpcodeUsage) {
        count += usage.instructions;
    }
    return count;
}

std::uint32_t CountTh07MsgBinaryOpcodeArgBytes()
{
    std::uint32_t count = 0;
    for (const Th07MsgBinaryOpcodeArgShape &shape : kTh07MsgBinaryOpcodeArgShapes) {
        count += shape.totalArgBytes;
    }
    return count;
}

} // namespace th07
