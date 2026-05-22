#pragma once

#include "Windows.h"

struct IDirectInput8A;
struct IDirectInputDevice8A;

typedef IDirectInput8A *LPDIRECTINPUT8;
typedef IDirectInputDevice8A *LPDIRECTINPUTDEVICE8A;

struct DIDEVCAPS
{
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwDevType;
    DWORD dwAxes;
    DWORD dwButtons;
    DWORD dwPOVs;
    DWORD dwFFSamplePeriod;
    DWORD dwFFMinTimeResolution;
    DWORD dwFirmwareRevision;
    DWORD dwHardwareRevision;
    DWORD dwFFDriverVersion;
};

struct DIDEVICEINSTANCEA
{
    DWORD dwSize;
};

struct DIDEVICEOBJECTINSTANCEA
{
    DWORD dwSize;
};

typedef const DIDEVICEINSTANCEA *LPCDIDEVICEINSTANCEA;
typedef const DIDEVICEOBJECTINSTANCEA *LPCDIDEVICEOBJECTINSTANCEA;
