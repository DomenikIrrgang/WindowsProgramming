#include "BERender.h"

void InitRender(Render* render)
{
	render->screenBuffer.memory = 0;
	render->screenBuffer.bytesPerPixel = 4;
	render->screenBuffer.info.bmiHeader.biSize = sizeof(render->screenBuffer.info.bmiHeader);
	render->screenBuffer.info.bmiHeader.biPlanes = 1;
	render->screenBuffer.info.bmiHeader.biBitCount = 32;
	render->screenBuffer.info.bmiHeader.biCompression = BI_RGB;
	render->screenBuffer.info.bmiHeader.biSizeImage = 0;
	render->screenBuffer.info.bmiHeader.biXPelsPerMeter = 0;
	render->screenBuffer.info.bmiHeader.biYPelsPerMeter = 0;
}

void ResizeDIBSection(Render* render, int32 width, int32 height)
{
	if (render->screenBuffer.memory)
	{
		VirtualFree(render->screenBuffer.memory, 0, MEM_RELEASE);
	}

	render->screenBuffer.info.bmiHeader.biWidth = width;
	render->screenBuffer.info.bmiHeader.biHeight = height;
	render->screenBuffer.width = width;
	render->screenBuffer.height = height;
	render->screenBuffer.pitch = render->screenBuffer.width * render->screenBuffer.bytesPerPixel;
	render->screenBuffer.memory = VirtualAlloc(0, render->screenBuffer.bytesPerPixel * width * height, MEM_COMMIT, PAGE_READWRITE);
}

void RenderClientArea(Render* render, WindowSize windowSize, HDC context)
{
	StretchDIBits(context, 0, 0, windowSize.width, windowSize.height, 0, 0, render->screenBuffer.width, render->screenBuffer.height, render->screenBuffer.memory, &(render->screenBuffer.info), DIB_RGB_COLORS, SRCCOPY);
}


void RenderGradient(Render* render, int32 xOffset, int32 yOffset)
{
	uint8* row = (uint8*)render->screenBuffer.memory;
	for (int32 y = 0; y < render->screenBuffer.height; y++)
	{
		uint8* pixel = (uint8*)row;
		for (int32 x = 0; x < render->screenBuffer.width; x++)
		{
			// BLUE
			*pixel = x + xOffset;
			++pixel;
			// GREEN
			*pixel = y + yOffset;
			++pixel;
			// RED
			*pixel = 0;
			++pixel;
			*pixel = 0;
			++pixel;
		}
		row += render->screenBuffer.pitch;
	}
}
