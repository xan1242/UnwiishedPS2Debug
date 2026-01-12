#include <stdint.h>
#include "UnwiishedPS2Debug.h"
#include "nnRenderStuff.h"
#include "nnPrint.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

// functions
void(*UnwiishedPS2Debug_uMainLoopFunc)(void* obj) = (void(*)(void*))(0);
void* (*UnwiishedPS2Debug_uGetModeManagerThingy)() = (void* (*)())(0);
void(*UnwiishedPS2Debug_uSetNextGameModeThingy)(void* manager, int mode) = (void(*)(void*, int))(0);
void(*UnwiishedPS2Debug_HeapManager_DispInfo)(int posX, int posY) = (void(*)(int, int))(0);

const uintptr_t p_gp = 0x877FF0;
//uintptr_t p_ButtonMaskThisFrame = p_gp - 0x331C;
uintptr_t p_ButtonMaskCurrent = p_gp - 0x3324;
uintptr_t p_ModeUpdateTrigger;
bool bModeTriggerOldState;

int DisplayFPSMode = DISPLAYFPS_MODE_ON;
bool bDisplayFPSModeTriggerOldState;

int DisplayFPSType = DISPLAYFPS_TYPE_ALL;
bool bDisplayFPSTypeTriggerOldState;

bool bDisplayHeap = false;
bool bDisplayHeapTriggerOldState;

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
	bool bModeButtonState = (buttonMask & 0x0100) && (buttonMask & 0x0800); // select & start
	if (bModeTriggerOldState != bModeButtonState)
	{
		if (!bModeButtonState) // on release
		{
			//LOG("released start+select!\n");
			GotoDebugSelectMode();
		}
	}
	bModeTriggerOldState = bModeButtonState;


	bool bDisplayFPSModeButtonState = (buttonMask & 0x0100) && (buttonMask & 0004); // select & L1
	if (bDisplayFPSModeTriggerOldState != bDisplayFPSModeButtonState)
	{
		if (bDisplayFPSModeButtonState) // on press
		{
			DisplayFPSMode++;
			DisplayFPSMode %= DISPLAYFPS_MODE_COUNT;

			unsigned int fpsmode = FPSDisplay_GetMode();

			switch (DisplayFPSMode)
			{
				case DISPLAYFPS_MODE_ON_CONSTANT:
					fpsmode |= FPSDISPLAY_MODE_FLAG_ONOFF | FPSDISPLAY_MODE_FLAG_CONSTANT;
					nnSetPrintColor(0xFFFF00FF);
					nnPrint(0, 3, "FPSDisplay ON CONSTANT");
					break;
				case DISPLAYFPS_MODE_ON:
					fpsmode &= ~FPSDISPLAY_MODE_FLAG_CONSTANT;
					fpsmode |= FPSDISPLAY_MODE_FLAG_ONOFF;
					nnSetPrintColor(0x00FF00FF);
					nnPrint(0, 3, "FPSDisplay ON");
					break;
				default:
					fpsmode &= ~FPSDISPLAY_MODE_FLAG_ONOFF;
					nnSetPrintColor(0xFF0000FF);
					nnPrint(0, 3, "FPSDisplay OFF");
					break;
			}

			FPSDisplay_SetMode(fpsmode);
		}
	}
	bDisplayFPSModeTriggerOldState = bDisplayFPSModeButtonState;

	bool bDisplayFPSTypeButtonState = (buttonMask & 0x0100) && (buttonMask & 0001); // select & L2
	if (bDisplayFPSTypeTriggerOldState != bDisplayFPSTypeButtonState)
	{
		if (bDisplayFPSTypeButtonState)
		{
			DisplayFPSType++;
			DisplayFPSType %= DISPLAYFPS_TYPE_COUNT;

			unsigned int fpsmode = FPSDisplay_GetMode();

			switch (DisplayFPSType)
			{
				case DISPLAYFPS_TYPE_FPS:
					fpsmode &= ~FPSDISPLAY_MODE_FLAG_FRAMETIME;
					fpsmode |= FPSDISPLAY_MODE_FLAG_FRAMERATE;
					nnSetPrintColor(0xFFFFFFFF);
					nnPrint(0, 3, "FPSDisplay Type FPS");
					break;
				case DISPLAYFPS_TYPE_FT:
					fpsmode &= ~FPSDISPLAY_MODE_FLAG_FRAMERATE;
					fpsmode |= FPSDISPLAY_MODE_FLAG_FRAMETIME;
					nnSetPrintColor(0xFFFFFFFF);
					nnPrint(0, 3, "FPSDisplay Type FT");
					break;
				default:
					fpsmode |= FPSDISPLAY_MODE_FLAG_FRAMETIME | FPSDISPLAY_MODE_FLAG_FRAMERATE;
					nnSetPrintColor(0xFFFFFFFF);
					nnPrint(0, 3, "FPSDisplay Type ALL");
					break;
			}

			FPSDisplay_SetMode(fpsmode);
		}
	}
	bDisplayFPSTypeTriggerOldState = bDisplayFPSTypeButtonState;

	bool bDisplayHeapButtonState = (buttonMask & 0x0100) && (buttonMask & 0x0400); // select & R3
	if (bDisplayHeapTriggerOldState != bDisplayHeapButtonState)
	{
		if (bDisplayHeapButtonState) // on press
		{
			bDisplayHeap = !bDisplayHeap;
		}
	}
	bDisplayHeapTriggerOldState = bDisplayHeapButtonState;
}

void UnwiishedPS2Debug_MainLoopHook(void* obj)
{
	FPSDisplay_StartFrame();
	UnwiishedPS2Debug_uMainLoopFunc(obj);
	FPSDisplay_EndFrame();

	if (bDisplayHeap)
		UnwiishedPS2Debug_HeapManager_DispInfo(32, 3);
	
	if (DisplayFPSMode != 0)
		FPSDisplay_Draw();

	HandleInputs();
}

void UnwiishedPS2Debug_hkHeapManager_Dump(HeapAlloc* pHeap)
{
	LOG_NOHEADER("UnwiishedPS2Debug_hkHeapManager_Dump: STUB!\nHeap: 0x%x\nPtr: 0x%x\nSize: %u bytes\nNum allocations: %u\n", pHeap, pHeap->unkptr1, pHeap->size1, pHeap->numAlloc);
	LOG_NOHEADER("Size2: %u bytes\nunkptr2: 0x%x\nunkptr3: 0x%x\nunkptr4: 0x%x\nunkptr5: 0x%x\n", pHeap->size2, pHeap->unkptr2, pHeap->unkptr3, pHeap->unkptr4, pHeap->unkptr5);
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

	uintptr_t loc_35A908 = 0x35A908;
	uintptr_t loc_35A910 = loc_35A908 + 8;
	UnwiishedPS2Debug_HeapManager_DispInfo = (void(*)(int, int))(minj_GetPtr(loc_35A908, loc_35A910));

	// patch heap display inputs to use global inputs instead...
	minj_WriteMemory32(0x000D6B18, 0x03801021); // move v0, gp
	minj_WriteMemory32(0x000D6B1C, 0x2442CCDC); // addiu v0, v0, -0x3324

	// hook the dump function
	minj_MakeJMP(0xD5560, (uintptr_t)&UnwiishedPS2Debug_hkHeapManager_Dump);
}

void UnwiishedPS2Debug_PostInit()
{
    LOG("UnwiishedPS2Debug_PostInit...\n");

	// this needs to happen after nnConfigureSystem...
	nnPrint_InitDebugPrint(10, 13);
}
