#pragma once

#include "inttypes.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace th07 {

inline constexpr std::uint32_t kTh07MsgHeaderSize = 0x0004;
inline constexpr std::uint32_t kTh07MsgEntryPointerSize = 0x0004;
inline constexpr std::uint32_t kTh07MsgInstructionHeaderSize = 0x0004;
inline constexpr std::uint32_t kTh07MsgMaxEntryPointers = 64;
inline constexpr std::uint8_t kTh07MsgMaxOpcode = 0x0e;

inline constexpr std::uint8_t kTh07MsgOpcodeMsgDelete = 0x00;
inline constexpr std::uint8_t kTh07MsgOpcodePortraitAnmScript = 0x01;
inline constexpr std::uint8_t kTh07MsgOpcodePortraitAnmSprite = 0x02;
inline constexpr std::uint8_t kTh07MsgOpcodeTextDialogue = 0x03;
inline constexpr std::uint8_t kTh07MsgOpcodeWait = 0x04;
inline constexpr std::uint8_t kTh07MsgOpcodeAnmInterrupt = 0x05;
inline constexpr std::uint8_t kTh07MsgOpcodeEclResume = 0x06;
inline constexpr std::uint8_t kTh07MsgOpcodeMusic = 0x07;
inline constexpr std::uint8_t kTh07MsgOpcodeTextIntro = 0x08;
inline constexpr std::uint8_t kTh07MsgOpcodeStageResults = 0x09;
inline constexpr std::uint8_t kTh07MsgOpcodeMsgHalt = 0x0a;
inline constexpr std::uint8_t kTh07MsgOpcodeStageEnd = 0x0b;
inline constexpr std::uint8_t kTh07MsgOpcodeMusicFadeout = 0x0c;
inline constexpr std::uint8_t kTh07MsgOpcodeWaitSkippable = 0x0d;
inline constexpr std::uint8_t kTh07MsgOpcodeScreenFade = 0x0e;

struct Th07MsgInstruction {
    std::uint16_t time = 0;
    std::uint8_t opcode = 0;
    std::uint8_t argumentSize = 0;
    std::vector<u8> arguments;
};

struct Th07MsgEntry {
    std::uint32_t offset = 0;
    std::vector<Th07MsgInstruction> instructions;
};

struct Th07MsgFile {
    std::vector<std::uint32_t> entryOffsets;
    std::vector<Th07MsgEntry> uniqueEntries;
};

bool ParseTh07MsgFile(std::span<const u8> data, Th07MsgFile *outFile, std::string *errorMessage = nullptr);
const Th07MsgEntry *FindTh07MsgEntry(const Th07MsgFile &file, std::uint32_t entryIndex);
const Th07MsgEntry *FindTh07MsgEntryByOffset(const Th07MsgFile &file, std::uint32_t offset);
std::size_t CountTh07MsgInstructions(const Th07MsgFile &file);
std::uint32_t BuildTh07MsgOpcodeMask(const Th07MsgFile &file);
std::size_t CountTh07MsgOpcode(const Th07MsgFile &file, std::uint8_t opcode);
std::uint32_t CountTh07MsgTextBytes(const Th07MsgFile &file);
std::uint32_t CountTh07MsgWaitTotalFrames(const Th07MsgFile &file);

} // namespace th07
