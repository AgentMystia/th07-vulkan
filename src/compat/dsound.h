#pragma once

#include "Windows.h"

struct IDirectSound8;
struct IDirectSoundBuffer;
struct IDirectSoundNotify;

typedef IDirectSound8 *LPDIRECTSOUND;
typedef IDirectSound8 *LPDIRECTSOUND8;
typedef IDirectSoundBuffer *LPDIRECTSOUNDBUFFER;
typedef IDirectSoundNotify *LPDIRECTSOUNDNOTIFY;

#define DSBPLAY_LOOPING 0x1
