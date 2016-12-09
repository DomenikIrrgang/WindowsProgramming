#include "BloomEngine.h"

INT CALLBACK wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR commandLine, int showCode)
{
	LoadXInput();
	WNDCLASS windowClass = RegisterWindowClass(L"BLOOM ENGINE WINDOW CLASS", instance);
	HWND window = CreateBloomEngineWindow(windowClass, L"BloomEngine", NULL, instance);
	InitRender(&defaultRender);
	ResizeDIBSection(&defaultRender, 1280, 720);
	if (window)
	{
		InitSound(window, &primarySoundBuffer, &secondarySoundBuffer, 48000);
		LockAndWriteBuffer(&secondarySoundBuffer, 0, secondarySoundBuffer.size);
		secondarySoundBuffer.buffer->Play(0, 0, DSBPLAY_LOOPING);
		ShowWindow(window, showCode);
		RunMessageLoop(window);
	}
	return 0;
}

void RunMessageLoop(HWND window)
{
	MSG message = { };
	running = true;
	int32 xOffset = 0;
	int32 yOffset = 0;
	while (running)
	{
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				running = false;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}


		// TODO: Just test sound
		WriteSoundToBuffer(&secondarySoundBuffer);
		// TODO: Just testing redering
		HDC context = GetDC(window);
		WindowSize windowSize = GetWindowSize(window);
		RenderGradient(&defaultRender, xOffset, yOffset);
		RenderClientArea(&defaultRender, windowSize, context);
		ReleaseDC(window, context);
		xOffset++;
		yOffset++;
	}
}

WNDCLASS RegisterWindowClass(const wchar_t className[], HINSTANCE instanceHandle)
{
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindowCallback;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = className;
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&windowClass);
	return windowClass;
}

HWND CreateBloomEngineWindow(WNDCLASS windowClass, LPCTSTR windowTitle, HWND parent, HINSTANCE instanceHandle)
{
	WindowSize size;
	size.height = 200;
	size.width = 200;
	size.x = 100;
	size.y = 100;
	HWND window = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		windowClass.lpszClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		size.x, size.y, size.width, size.height,
		parent,  
		NULL,
		instanceHandle,
		NULL
	);
	return window;
}

LRESULT CALLBACK MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
		case WM_SIZE:
		{
		} break;

		case WM_DESTROY:
		{
			running = false;
		} break;
			
		case WM_ACTIVATEAPP:
		{

		} break;
		
		case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC context = BeginPaint(window, &paintStruct);
			WindowSize windowSize = GetWindowSize(window);
			RenderClientArea(&defaultRender, windowSize, context);
			EndPaint(window, &paintStruct);

		} break;

		case WM_CLOSE:
		{
			running = false;
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32 vkCode = wParam;
			bool32 wasDown = (lParam & (1 << 30));
			bool32 isDown = (lParam & (1 << 31));
			HandleKeyEvent(message, vkCode, isDown, wasDown);

			bool32 altKeyDown = (lParam & (1 << 29));
			if (vkCode == VK_F4 && altKeyDown)
			{
				running = false;
			}
		} break;

		default:
		{
			result = DefWindowProc(window, message, wParam, lParam);
		} break;
		
	}
	return result;
}
