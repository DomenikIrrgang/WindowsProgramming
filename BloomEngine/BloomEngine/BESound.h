#pragma once

#ifndef BESOUND_H
#define BESOUND_H

#include <Windows.h>
#include <dsound.h>
#include "Definitions.h"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

typedef struct SoundBuffer
{
	LPDIRECTSOUNDBUFFER buffer;
	DSBUFFERDESC description;
	int32 bytesPerSample;
	int32 size;
	int32 tone;
	int16 volume;
	uint32 runningSampleIndex;
	int32 wavePeriod;
	int32 samplesPerSecond;
	real32 tSine;
	int32 latencySampleCount;
} SoundBuffer;

global SoundBuffer primarySoundBuffer;
global SoundBuffer secondarySoundBuffer;
global WAVEFORMATEX waveFormat;

WAVEFORMATEX* GetWaveFormat(int32 samplesPerSecond);
void CreatePrimarySoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat);
void CreateSecondarySoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat, int32 bufferSize);
void InitSound(HWND window, SoundBuffer* primarySoundBuffer, SoundBuffer* secondarySoundBuffer, int32 samplesPerSecond);
void WriteSoundToBuffer(SoundBuffer* soundBuffer);
void FillSoundBuffer(SoundBuffer* soundBuffer, VOID* region, DWORD regionSize);
void CreateSoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat);
void LockAndWriteBuffer(SoundBuffer* soundBuffer, DWORD byteToLock, DWORD bytesToWrite);

#endif