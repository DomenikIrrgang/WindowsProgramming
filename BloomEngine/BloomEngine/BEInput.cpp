#include "BEInput.h"
#include "BESound.h"

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

void LoadXInput(void)
{
	HMODULE xInputLibary = LoadLibraryA("xinput9_1_0.dll");
	if (xInputLibary)
	{
		GetControllerState = (Get_Controller_State*) GetProcAddress(xInputLibary, "XInputGetState");
		SetControllerState = (Set_Controller_State*) GetProcAddress(xInputLibary, "XInputSetState");
	}
}

void HandleKeyEvent(uint32 event, uint32 keycode, bool32 isDown, bool32 wasDown)
{
	switch (keycode)
	{
		case 'W':
		{

		} break;

		case 'A':
		{

		} break;

		case 'S':
		{
			secondarySoundBuffer.tone -= 100;
			secondarySoundBuffer.wavePeriod = secondarySoundBuffer.samplesPerSecond / secondarySoundBuffer.tone;
		} break;

		case 'D':
		{

		} break;

		default:
		{

		} break;

	}
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