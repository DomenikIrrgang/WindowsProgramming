#pragma once

#ifndef SCREENBUFFER_H
#define SCREENBUFFER_H

#include "Definitions.h"
#include <Windows.h>

typedef struct ScreenBuffer
{
	BITMAPINFO info;
	int32 bytesPerPixel;
	int32 width;
	int32 height;
	int32 pitch;
	void* memory;
} ScreenBuffer;

#endif 
