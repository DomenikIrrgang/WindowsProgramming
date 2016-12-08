#pragma once

#ifndef BEINPUT_H
#define BEINPUT_H

#include <Windows.h>
#include <Xinput.h>
#include "Definitions.h"


#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pState)

typedef X_INPUT_GET_STATE(Get_Controller_State);
typedef X_INPUT_SET_STATE(Set_Controller_State);


void LoadXInput(void);
void HandleKeyEvent(uint32 event, uint32 keycode, bool32 isDown, bool32 wasDown);
internal void PullAllControllers(void);

#endif


