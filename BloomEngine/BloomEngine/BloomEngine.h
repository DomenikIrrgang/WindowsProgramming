#pragma once
#ifndef BLOOMENGINE_H

#define BLOOMENGINE_H

#include <windows.h>
#include "Definitions.h"
#include "WindowSize.h"
#include "BERender.h"
#include "BEInput.h"
#include "BESound.h"

global bool running;

LRESULT CALLBACK MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
WNDCLASS RegisterWindowClass(const wchar_t className[], HINSTANCE instanceHandle);
HWND CreateBloomEngineWindow(WNDCLASS windowClass, LPCTSTR windowTitle, HWND parent, HINSTANCE instanceHandle);
void RunMessageLoop(HWND window);

#endif 
