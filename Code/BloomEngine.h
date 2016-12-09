#pragma once
#ifndef BLOOMENGINE_H

#define BLOOMENGINE_H

#include <windows.h>
#include "Definitions.h"
#include "WindowSize.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS registerWindowClass(const wchar_t className[], HINSTANCE instanceHandle);
HWND createBloomEngineWindow(WNDCLASS windowClass, LPCTSTR windowTitle, HWND parent, HINSTANCE instanceHandle);
void runMessageLoop(HWND window);

#endif 
