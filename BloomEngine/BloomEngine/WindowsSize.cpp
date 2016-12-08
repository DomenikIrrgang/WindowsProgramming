#include "WindowSize.h"

WindowSize GetWindowSize(HWND window)
{
	RECT clientRectangle;
	WindowSize windowSize;
	GetClientRect(window, &clientRectangle);
	windowSize.width = clientRectangle.right - clientRectangle.left;
	windowSize.height = clientRectangle.bottom - clientRectangle.top;
	windowSize.x = clientRectangle.left;
	windowSize.y = clientRectangle.top;
	return windowSize;
}