#pragma once

#ifndef BESOUND_H
#define BESOUND_H

#include <Windows.h>
#include <dsound.h>
#include "Definitions.h"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

void InitSound(HWND window, int32 samplesPerSecond, int32 bufferSize);

#endif