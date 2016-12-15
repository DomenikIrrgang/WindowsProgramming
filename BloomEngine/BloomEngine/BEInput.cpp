#include "BEInput.h"
#include "BESound.h"
#include <stdio.h>

bool32 altKeyDown = 0;
bool32 ctrlKeyDown = 0;
bool32 shiftKeyDown = 0;
Keybind keybinds[20];
int32 keybindCount;

X_INPUT_GET_STATE(Get_Controller_State_Stub)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

X_INPUT_SET_STATE(Set_Controller_State_Stub)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}

global Get_Controller_State* GetControllerState = Get_Controller_State_Stub;
global Set_Controller_State* SetControllerState = Set_Controller_State_Stub;

void InitInput(void)
{
	keybindCount = 0;
	LoadXInput();
}

void RegisterKeyboardInput(KeyboardInput keyboardInput, KeyboardInputCallback callback)
{
	Keybind keybind;
	keybind.callback = callback;
	keybind.keyboardInput = keyboardInput;
	keybinds[keybindCount++] = keybind;
}

void LoadXInput(void)
{
	HMODULE xInputLibary = LoadLibraryA("xinput9_1_0.dll");
	if (xInputLibary)
	{
		GetControllerState = (Get_Controller_State*) GetProcAddress(xInputLibary, "XInputGetState");
		SetControllerState = (Set_Controller_State*) GetProcAddress(xInputLibary, "XInputSetState");
	}
}

void HandleKeyEvent(uint32 event, WPARAM wParam, LPARAM lParam)
{
	uint32 vkCode = wParam;
	bool32 keydown = 0;
	bool32 wasDown = (lParam & (1 << 30));
	bool32 isDown = (lParam & (1 << 31));

	if (event == WM_KEYDOWN || event == WM_SYSKEYDOWN)
	{
		keydown = 1;
	}

	if (vkCode == BE_SHIFT)
	{
		shiftKeyDown = keydown;
		return;
	}

	if (vkCode == BE_ALT)
	{
		altKeyDown = keydown;
		return;
	}

	if (vkCode == BE_CTRL)
	{
		ctrlKeyDown = keydown;
		return;
	}

	for (int32 index = 0; index < keybindCount; index++)
	{
		KeyboardInput keyboardInput = keybinds[index].keyboardInput;
		if (keyboardInput.alt == altKeyDown && keyboardInput.ctrl == ctrlKeyDown && keyboardInput.shift == shiftKeyDown && keyboardInput.key == vkCode)
		{
			keybinds[index].callback(isDown, wasDown);

		}
	}
	char str[256];
	sprintf_s(str, "Key: %d \n", vkCode);
	OutputDebugStringA(str);
}

internal void PullAllControllers(void)
{
	for (DWORD controllerIndex = 0; controllerIndex < XUSER_MAX_COUNT; controllerIndex++)
	{
		XINPUT_STATE controllerState;
		if (XInputGetState(controllerIndex, &controllerState) == ERROR_SUCCESS)
		{
			bool up = (controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		}
		else
		{
			//TODO: Handle Controller unplug
		}
	}
}