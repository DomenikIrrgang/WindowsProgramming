#include "BESound.h"

void InitSound(HWND window, int32 samplesPerSecond, int32 bufferSize)
{
	HMODULE soundLibrary = LoadLibraryA("dsound.dll");
	if (soundLibrary)
	{
		direct_sound_create* directSoundCreate = (direct_sound_create*) GetProcAddress(soundLibrary, "DirectSoundCreate");
		LPDIRECTSOUND directSound;
		if (directSoundCreate && SUCCEEDED(directSoundCreate(0, &directSound, 0)))
		{
			WAVEFORMATEX waveFormat = {};
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = 2;
			waveFormat.nSamplesPerSec = samplesPerSecond;
			waveFormat.wBitsPerSample = 16;
			waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
			waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;	
			waveFormat.cbSize = 0;

			if (SUCCEEDED(directSound->SetCooperativeLevel(window, DSSCL_PRIORITY)))
			{
				DSBUFFERDESC primaryBufferDescription = {};
				primaryBufferDescription.dwSize = sizeof(primaryBufferDescription);
				primaryBufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
				LPDIRECTSOUNDBUFFER primaryBuffer;
				if (SUCCEEDED(directSound->CreateSoundBuffer(&primaryBufferDescription, &primaryBuffer, 0)))
				{
					OutputDebugStringA("primary buffer created\n");
					if (SUCCEEDED(primaryBuffer->SetFormat(&waveFormat)))
					{
						OutputDebugStringA("primary buffer format set\n");
					}
				}

				WAVEFORMATEX waveTest = {};
				waveTest.wFormatTag = WAVE_FORMAT_PCM;
				waveTest.nChannels = 2;
				waveTest.nSamplesPerSec = samplesPerSecond;
				waveTest.wBitsPerSample = 16;
				waveTest.nBlockAlign = (waveTest.nChannels * waveTest.wBitsPerSample) / 8;
				waveTest.nAvgBytesPerSec = waveTest.nSamplesPerSec * waveTest.nBlockAlign;
				waveTest.cbSize = 0;

				DSBUFFERDESC secondaryBufferDescription = {};
				secondaryBufferDescription.dwSize = sizeof(secondaryBufferDescription);
				secondaryBufferDescription.dwFlags = 0;
				secondaryBufferDescription.dwBufferBytes = bufferSize;
				secondaryBufferDescription.lpwfxFormat = &waveTest;
				LPDIRECTSOUNDBUFFER secondaryBuffer;
				if (SUCCEEDED(directSound->CreateSoundBuffer(&secondaryBufferDescription, &secondaryBuffer, 0)))
				{
					OutputDebugStringA("secondary buffer created\n");
				}
			}
		
		}
	}
}