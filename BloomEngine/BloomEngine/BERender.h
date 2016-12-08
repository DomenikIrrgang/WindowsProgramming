#ifndef BERENDER_H

#define BERENDER_H

#include <Windows.h>
#include "Definitions.h"
#include "ScreenBuffer.h"
#include "WindowSize.h"

typedef struct Render
{
	ScreenBuffer screenBuffer;
} Render;

global Render defaultRender;

void InitRender(Render* render);
void ResizeDIBSection(Render* render, int32 width, int32 height);
void RenderClientArea(Render* render, WindowSize windowSize, HDC context);
void RenderGradient(Render* render, int32 xOffset, int32 yOffset);

#endif
