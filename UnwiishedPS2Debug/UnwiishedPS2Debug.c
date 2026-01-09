#include <stdint.h>
#include "UnwiishedPS2Debug.h"
#include "nnRenderStuff.h"
#include "nnPrint.h"
#include "includes/minjector.h"

// functions
void(*UnwiishedPS2Debug_uMainLoopFunc)(void* obj) = (void(*)(void*))(0);
void* (*UnwiishedPS2Debug_uGetModeManagerThingy)() = (void* (*)())(0);
void(*UnwiishedPS2Debug_uSetNextGameModeThingy)(void* manager, int mode) = (void(*)(void*, int))(0);

const uintptr_t p_gp = 0x877FF0;
//uintptr_t p_ButtonMaskThisFrame = p_gp - 0x331C;
uintptr_t p_ButtonMaskCurrent = p_gp - 0x3324;
uintptr_t p_ModeUpdateTrigger;
bool bModeTriggerOldState;

void (*_FlushCache)(int op) = (void(*)(int))(0);
void FlushCache(int op)
{
	if (_FlushCache)
		return _FlushCache(op);
}

void GotoDebugSelectMode()
{
	void* manager = UnwiishedPS2Debug_uGetModeManagerThingy();
	if (!manager)
		return;
	UnwiishedPS2Debug_uSetNextGameModeThingy(manager, 1);
	*(int*)p_ModeUpdateTrigger = 1;
}

void HandleInputs()
{
	uint32_t buttonMask = *(uint32_t*)(p_ButtonMaskCurrent);
	bool bButtonState = (buttonMask & 0x0100) && (buttonMask & 0x0800); // select & start
	if (bModeTriggerOldState != bButtonState)
	{
		if (!bButtonState) // on release
		{
			//LOG("released start+select!\n");
			GotoDebugSelectMode();
		}
	}
	bModeTriggerOldState = bButtonState;
}

void UnwiishedPS2Debug_MainLoopHook(void* obj)
{
	UnwiishedPS2Debug_uMainLoopFunc(obj);
	HandleInputs();
}


void UnwiishedPS2Debug_Init()
{
	LOG("UnwiishedPS2Debug_Init...\n");

	uintptr_t loc_D01F0 = 0xD01F0;
	_FlushCache = (void(*)(int))(minj_GetBranchDestination(loc_D01F0));

	nnRenderStuff_Init();
	nnPrint_Init();

	uintptr_t loc_4FF7B0 = 0x4FF7B0;
	UnwiishedPS2Debug_uMainLoopFunc = (void(*)(void*))(minj_GetBranchDestination(loc_4FF7B0));
	minj_MakeCALL(loc_4FF7B0, (uintptr_t)&UnwiishedPS2Debug_MainLoopHook);

	uintptr_t loc_482854 = 0x482854;
	UnwiishedPS2Debug_uGetModeManagerThingy = (void*(*)())(minj_GetBranchDestination(loc_482854));

	uintptr_t loc_482860 = 0x482860;
	UnwiishedPS2Debug_uSetNextGameModeThingy = (void(*)(void*, int))(minj_GetBranchDestination(loc_482860));

	uintptr_t loc_D098C = 0xD098C;
	p_ModeUpdateTrigger = minj_GetPtr(loc_D098C, loc_D098C + 4);

	// replace attract mode with DebugSelect mode
	uintptr_t loc_40E98C = 0x0040E98C;
	*(int16_t*)(loc_40E98C) = 1;

    //PrintActionIdEnum();
}

void UnwiishedPS2Debug_PostInit()
{
    LOG("UnwiishedPS2Debug_PostInit...\n");

	// this needs to happen after nnConfigureSystem...
	nnPrint_InitDebugPrint(10, 13);
}
