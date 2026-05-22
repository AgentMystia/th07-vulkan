#pragma once

#include "d3d8types.h"

struct IDirect3D8;
struct IDirect3DDevice8;
struct IDirect3DTexture8;
struct IDirect3DSurface8;
struct IDirect3DVertexBuffer8
{
    ULONG Release();
};

typedef IDirect3D8 *PDIRECT3D8;
typedef IDirect3D8 *LPDIRECT3D8;
typedef IDirect3DDevice8 *PDIRECT3DDEVICE8;
typedef IDirect3DDevice8 *LPDIRECT3DDEVICE8;
typedef IDirect3DTexture8 *LPDIRECT3DTEXTURE8;
typedef IDirect3DSurface8 *LPDIRECT3DSURFACE8;
typedef IDirect3DVertexBuffer8 *LPDIRECT3DVERTEXBUFFER8;

#define D3D_SDK_VERSION 220
#define D3DFVF_DIFFUSE 0x40
#define D3DFVF_XYZ 0x002
#define D3DFVF_XYZRHW 0x004
#define D3DFVF_TEX1 0x100
