#pragma once

#include "Windows.h"

typedef DWORD D3DCOLOR;
typedef int D3DFORMAT;

#define D3DCOLOR_ARGB(a, r, g, b)                                                                                     \
    ((D3DCOLOR)((((DWORD)(a) & 0xff) << 24) | (((DWORD)(r) & 0xff) << 16) | (((DWORD)(g) & 0xff) << 8) |              \
                ((DWORD)(b) & 0xff)))
#define D3DCOLOR_RGBA(r, g, b, a) D3DCOLOR_ARGB(a, r, g, b)

struct D3DVIEWPORT8
{
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    float MinZ;
    float MaxZ;
};

struct D3DPRESENT_PARAMETERS
{
    UINT BackBufferWidth;
    UINT BackBufferHeight;
    D3DFORMAT BackBufferFormat;
    UINT BackBufferCount;
    int MultiSampleType;
    int SwapEffect;
    HWND hDeviceWindow;
    BOOL Windowed;
    BOOL EnableAutoDepthStencil;
    D3DFORMAT AutoDepthStencilFormat;
    DWORD Flags;
    UINT FullScreen_RefreshRateInHz;
    UINT FullScreen_PresentationInterval;
};

struct D3DCAPS8
{
    BYTE reserved[0x13c];
};

struct D3DXIMAGE_INFO
{
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    D3DFORMAT Format;
    int ResourceType;
    int ImageFileFormat;
};
