#include "BESound.h"
#include <math.h>

//TEST

WAVEFORMATEX* GetWaveFormat(int32 samplesPerSecond)
{
	waveFormat = {};
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = samplesPerSecond;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	return &waveFormat;
}

void CreatePrimarySoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat)
{
	buffer->description = {};
	buffer->description.dwSize = sizeof(buffer->description);
	buffer->description.dwFlags = DSBCAPS_PRIMARYBUFFER;
	buffer->description.guid3DAlgorithm = DS3DALG_DEFAULT;
	if (SUCCEEDED(directSound->CreateSoundBuffer(&(buffer->description), &(buffer->buffer), 0)))
	{
		OutputDebugStringA("pbuffer created\n");
		if (SUCCEEDED(buffer->buffer->SetFormat(waveFormat)))
		{
			CreateSoundBuffer(directSound, buffer, waveFormat);
			OutputDebugStringA("pbuffer format\n");
		}
	}
	
}

void CreateSecondarySoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat, int32 bufferSize)
{
	buffer->description = {};
	buffer->description.dwSize = sizeof(buffer->description);
	buffer->description.dwFlags = 0;
	buffer->description.dwBufferBytes = bufferSize;
	buffer->description.lpwfxFormat = waveFormat;
	if (SUCCEEDED(directSound->CreateSoundBuffer(&(buffer->description), &(buffer->buffer), 0)))
	{
		CreateSoundBuffer(directSound, buffer, waveFormat);
		OutputDebugStringA("sbuffer created\n");
	}
}

void CreateSoundBuffer(LPDIRECTSOUND directSound, SoundBuffer* buffer, WAVEFORMATEX* waveFormat)
{
	buffer->samplesPerSecond = waveFormat->nSamplesPerSec;
	buffer->bytesPerSample = (waveFormat->wBitsPerSample / 8) * waveFormat->nChannels;
	buffer->size = buffer->bytesPerSample * buffer->samplesPerSecond;
	buffer->volume = 3000;
	buffer->tone = 256;
	buffer->runningSampleIndex = 0;
	buffer->wavePeriod = buffer->samplesPerSecond / buffer->tone;
	buffer->tSine = 0;
	buffer->latencySampleCount = buffer->samplesPerSecond / 15;
}


void InitSound(HWND window, SoundBuffer* primarySoundBuffer, SoundBuffer* secondarySoundBuffer, int32 samplesPerSecond)
{
	HMODULE soundLibrary = LoadLibraryA("dsound.dll");
	if (soundLibrary)
	{
		direct_sound_create* directSoundCreate = (direct_sound_create*) GetProcAddress(soundLibrary, "DirectSoundCreate");
		LPDIRECTSOUND directSound;
		if (directSoundCreate && SUCCEEDED(directSoundCreate(0, &directSound, 0)))
		{
			WAVEFORMATEX* waveFormat = GetWaveFormat(samplesPerSecond);
			if (SUCCEEDED(directSound->SetCooperativeLevel(window, DSSCL_PRIORITY)))
			{
				CreatePrimarySoundBuffer(directSound, primarySoundBuffer, waveFormat);
				CreateSecondarySoundBuffer(directSound, secondarySoundBuffer, waveFormat, samplesPerSecond * (waveFormat->wBitsPerSample / 8) * waveFormat->nChannels);
			}
		
		}
	}
}

void WriteSoundToBuffer(SoundBuffer* soundBuffer)
{
	DWORD playCursor;
	DWORD writeCursor;
	if (SUCCEEDED(soundBuffer->buffer->GetCurrentPosition(&playCursor, &writeCursor)))
	{
		DWORD byteToLock = (soundBuffer->runningSampleIndex * soundBuffer->bytesPerSample) % soundBuffer->size;
		DWORD targetCursor = (playCursor + (soundBuffer->latencySampleCount * soundBuffer->bytesPerSample)) % soundBuffer->size;
		DWORD bytesToWrite = 0;

		if (byteToLock == targetCursor)
		{
//			bytesToWrite = soundBuffer->size;
		}
		else if (byteToLock > targetCursor)
		{
			bytesToWrite = soundBuffer->size - byteToLock;
			bytesToWrite += targetCursor;
		}
		else
		{
			bytesToWrite = targetCursor - byteToLock;
		}
		LockAndWriteBuffer(soundBuffer, byteToLock, bytesToWrite);
	}
}

void LockAndWriteBuffer(SoundBuffer* soundBuffer, DWORD byteToLock, DWORD bytesToWrite)
{
	VOID* region1;
	DWORD region1Size;
	VOID* region2;
	DWORD region2Size;

	if (SUCCEEDED(soundBuffer->buffer->Lock(byteToLock, bytesToWrite, &region1, &region1Size, &region2, &region2Size, 0)))
	{
		DWORD region1SampleCount = (region1Size / soundBuffer->bytesPerSample);
		DWORD region2SampleCount = region2Size / soundBuffer->bytesPerSample;
		FillSoundBuffer(soundBuffer, region1, region1SampleCount);
		FillSoundBuffer(soundBuffer, region2, region2SampleCount);
		soundBuffer->buffer->Unlock(region1, region1Size, region2, region2Size);
	}
}

void FillSoundBuffer(SoundBuffer* soundBuffer, VOID* region, DWORD sampleCount)
{
	int16* sampleOut = (int16*) region;
	for (DWORD sampleIndex = 0; sampleIndex < sampleCount / 2; sampleIndex++)
	{
		real32 sineValue = sinf(soundBuffer->tSine);
		int16 sampleValue = (int16)(sineValue * soundBuffer->volume);
		*sampleOut++ += sampleValue;
		*sampleOut++ += sampleValue;
		soundBuffer->tSine += 2.0f * pi32 * 1.0f / (real32)soundBuffer->wavePeriod;
		soundBuffer->runningSampleIndex++;
		if (sampleIndex == sampleCount - 1)
		OutputDebugStringA("REACHED END OF BUFFER");
	}
}