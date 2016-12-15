#pragma once

#ifndef BEINPUT_H
#define BEINPUT_H

#include <Windows.h>
#include <Xinput.h>
#include "Definitions.h"

#define BE_SHIFT 16
#define BE_ALT 18
#define BE_CTRL 17

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pState)

typedef X_INPUT_GET_STATE(Get_Controller_State);
typedef X_INPUT_SET_STATE(Set_Controller_State);

typedef void(*KeyboardInputCallback)(int32, int32);

typedef struct
{
	bool32 alt;
	bool32 ctrl;
	bool32 shift;
	int32 key;
} KeyboardInput;

typedef struct
{
	KeyboardInput keyboardInput;
	KeyboardInputCallback callback;
} Keybind;

void InitInput(void);
void RegisterKeyboardInput(KeyboardInput keyboardInput, KeyboardInputCallback callback);
void LoadXInput(void);
void HandleKeyEvent(uint32 event, WPARAM wParam, LPARAM lParam);
internal void PullAllControllers(void);

#endif


