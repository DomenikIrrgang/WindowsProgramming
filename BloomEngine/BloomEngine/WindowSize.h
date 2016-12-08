#pragma once

#ifndef WINDOWSIZE_H

#define WINDOWSIZE_H

#include <Windows.h>

typedef struct WindowSize
{
	int x;
	int y;
	int width;
	int height;
} WindowSize;

WindowSize GetWindowSize(HWND window);

#endif