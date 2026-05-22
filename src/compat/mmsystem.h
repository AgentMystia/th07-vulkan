#pragma once

#include "Windows.h"
#include "mmreg.h"

typedef struct mmckinfo_tag
{
    DWORD ckid;
    DWORD cksize;
    DWORD fccType;
    DWORD dwDataOffset;
    DWORD dwFlags;
} MMCKINFO;

typedef struct mmioinfo_tag
{
    DWORD dwFlags;
    char *pchNext;
    char *pchEndRead;
    char *pchEndWrite;
    LONG lBufOffset;
    LONG lDiskOffset;
    DWORD adwInfo[3];
    DWORD dwReserved1;
    DWORD dwReserved2;
    HMMIO hmmio;
} MMIOINFO;
