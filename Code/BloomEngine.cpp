#include "BloomEngine.h"

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS windowClass = registerWindowClass(L"BLOOM ENGINE WINDOW CLASS", hInstance);
	HWND window = createBloomEngineWindow(windowClass, L"BloomEngine", NULL, hInstance);
	if (window)
	{
		ShowWindow(window, nCmdShow);
		runMessageLoop(window);
	}
	return 0;
}

void runMessageLoop(HWND window)
{
	MSG message = { };
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

WNDCLASS registerWindowClass(const wchar_t className[], HINSTANCE instanceHandle)
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = className;
	RegisterClass(&windowClass);
	return windowClass;
}

HWND createBloomEngineWindow(WNDCLASS windowClass, LPCTSTR windowTitle, HWND parent, HINSTANCE instanceHandle)
{
	WindowSize size;
	size.height = 300;
	size.width = 300;
	size.x = 100;
	size.y = 100;
	HWND window = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		windowClass.lpszClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		size.x, size.y, size.width, size.height,
		parent,  
		NULL,
		instanceHandle,
		NULL
	);
	return window;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
			return 0;			
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
