# Local Reference Corpus

This directory is the local evidence root for TH07 reconstruction.

Only this README and placeholder files are tracked. All original game files, extracted resources, generated audio/images, and Ghidra project data are ignored.

Expected local layout:

```text
reference/original/   original TH07 files such as th07.exe, th07.dat, thbgm.dat
reference/extracted/  unpacked th07.dat entries
reference/ecl/        readable ECL dumps
reference/std/        readable STD dumps
reference/msg/        readable MSG dumps
reference/ghidra/     Ghidra project, strings, functions, decompiler exports
reference/generated/  optional generated images/audio/SFX used for inspection
```

Do not commit proprietary original files or generated assets from this tree.
