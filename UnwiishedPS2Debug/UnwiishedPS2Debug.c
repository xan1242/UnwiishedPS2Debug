#include "UnwiishedPS2Debug.h"
#include "LowLevelPS2.h"
#include "bWare.h"
#include "nnRenderStuff.h"
#include "nnPrint.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"
#include "MDebugSelectEx.h"
#include "MActionSonic.h"
#include "MAdventure.h"
#include "MItemGet.h"
#include "MOption.h"
#include "MTitle.h"
#include "MLoad.h"
#include "MSave.h"
#include "MWorld.h"
#include "MAttract.h"
#include "MMovie.h"
#include "MScenario.h"
#include "MCredit.h"
#include "MSaveMain.h"
#include "MAutoSave.h"
#include "MGalleryTop.h"
#include "MClearResult.h"
#include "MLastResult.h"
#include "MEvilResult3.h"
#include "MNextStageSelect.h"

// functions
void(*UnwiishedPS2Debug_uMainLoopFunc)(void* obj) = (void(*)(void*))(0);
void* (*UnwiishedPS2Debug_uGetModeManagerThingy)() = (void* (*)())(0);
void(*UnwiishedPS2Debug_uSetNextGameModeThingy)(void* manager, int mode) = (void(*)(void*, int))(0);
void(*UnwiishedPS2Debug_HeapManager_DispInfo)(int posX, int posY) = (void(*)(int, int))(0);
int(*UnwiishedPS2Debug_sprintf)(char* buf, const char* fmt, ...) = (int(*)(char*,const char*, ...))(0);
int(*UnwiishedPS2Debug_VblCallback)(int) = (int(*)(int))(0);

const uintptr_t p_gp = 0x877FF0;
//uintptr_t p_ButtonMaskThisFrame = p_gp - 0x331C;
uintptr_t p_ButtonMaskCurrent = p_gp - 0x3324;
//uintptr_t p_CurrentGameMode = p_gp - 0x75D4;
uintptr_t p_ModeUpdateTrigger;
bool bModeTriggerOldState = false;

int DisplayFPSMode = DISPLAYFPS_MODE_ON;
bool bDisplayFPSModeTriggerOldState = false;

int DisplayFPSType = DISPLAYFPS_TYPE_ALL;
bool bDisplayFPSTypeTriggerOldState = false;

bool bDisplayHeap = false;
bool bDisplayHeapTriggerOldState = false;

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

	FPSDisplay_ClearDrawTask();

	HandleInputs();
}

void UnwiishedPS2Debug_hkHeapManager_Dump(HeapAlloc* pHeap)
{
	LOG_NOHEADER("UnwiishedPS2Debug_hkHeapManager_Dump: STUB!\nHeap: 0x%x\nPtr: 0x%x\nSize: %u bytes\nNum allocations: %u\n", pHeap, pHeap->unkptr1, pHeap->size1, pHeap->numAlloc);
	LOG_NOHEADER("Size2: %u bytes\nunkptr2: 0x%x\nunkptr3: 0x%x\nunkptr4: 0x%x\nunkptr5: 0x%x\n", pHeap->size2, pHeap->unkptr2, pHeap->unkptr3, pHeap->unkptr4, pHeap->unkptr5);
}

int UnwiishedPS2Debug_VblCallback_Hook(int unk)
{
	FPSDisplay_MeasureVSyncTime();
	return UnwiishedPS2Debug_VblCallback(unk);
}

//// SYNC STUFF -- do not use this, it has been ported to a pnach instead...
//
////const float targetFrameTime = (1.0f / 59.939998f) * 1000.0f;
//float targetFrameTime = (1.0f / 60.0f) * 1000.0f;
////const float targetFrameTime = (1.0f / 30.0f) * 1000.0f;
//unsigned int lastTicks = 0;
//
//
//bool UnwiishedPS2Debug_SyncRT()
//{
//	float ft = bGetTickerDifference(lastTicks, bGetTicker());
//	return ft >= targetFrameTime;
//}
//
//void(*UnwiishedPS2Debug_PS2SkeletonSync)() = (void(*)())(0);
//void(*UnwiishedPS2Debug_ADXM_WaitVsync)() = (void(*)())(0x6A64C0);
//void(*UnwiishedPS2Debug_EnableInterrupts)() = (void(*)())(0x662570);

//void WaitUntilVSync();
//#ifndef __INTELLISENSE__
//asm
//(
//	".global WaitUntilVSync\n"
//	"WaitUntilVSync:\n"
//	"addiu   $sp, -0x10\n"
//	"sd      $ra, 0($sp)\n"
//	"nop\n"
//	"jal     0x662570\n"
//	"lui     $a0, 0x87\n"
//	"lw      $v1, 0x3C50($a0)\n"
//	"lw      $v0, 0x3C50($a0)\n"
//	"ld      $ra, 0($sp)\n"
//	"bne     $v1, $v0, locret_20B534\n"
//	"nop\n"
//	"loc_20B4F8:\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"lw      $v0, 0x3C50($a0)\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"nop\n"
//	"beq     $v1, $v0, loc_20B4F8\n"
//	"ld      $ra, 0($sp)\n"
//	"locret_20B534:\n"
//	"addiu   $sp, 0x10\n"
//	"jr      $ra\n"
//);
//#endif

//void WaitUntilVSync()
//{
//	uint32_t* VBlankCounter = (uint32_t*)0x873C50;
//
//	UnwiishedPS2Debug_EnableInterrupts();
//	uint32_t initVal = *VBlankCounter;
//	uint32_t counter = *VBlankCounter;
//
//	while (counter == initVal)
//	{
//		counter = *VBlankCounter;
//	}
//}

//void UnwiishedPS2Debug_CustomSync()
//{
//	// #TODO:
//	// - 30/60fps switch
//	// - pnach version...
//	//
//	const int Mode_Attract = 12;
//	const int Mode_Movie = 13;
//	const int Mode_Credit = 16;
//
//	int currGameMode = *(int*)(p_CurrentGameMode);
//
//	if ((currGameMode == Mode_Attract) || (currGameMode == Mode_Movie) || (currGameMode == Mode_Credit))
//		return UnwiishedPS2Debug_PS2SkeletonSync();
//
//	while (!UnwiishedPS2Debug_SyncRT());
//	lastTicks = bGetTicker();
//}
//
//void Call_SetRealtimeScale(uintptr_t, float);
//#ifndef __INTELLISENSE__
//asm
//(
//	".global Call_SetRealtimeScale\n"
//	"Call_SetRealtimeScale:\n"
//	"lw $t9, 0($a0)\n"
//	"lw $t9, 0x34($t9)\n"
//	"mov.s $f12, $f13\n"
//	"jr $t9\n"
//);
//#endif
//
//void SetFrameTimeThing_Hook(uintptr_t mode_obj, uint32_t fps)
//{
//	uint32_t videoModeFPS = *(uint32_t*)0x87C3B0;
//	uint32_t* fpsLimit = (uint32_t*)0x87C3B4;
//	if (fps == 30)
//	{
//		*fpsLimit = videoModeFPS / 2;
//	}
//	else
//	{
//		*fpsLimit = videoModeFPS;
//	}
//
//	targetFrameTime = (1.0f / (float)*fpsLimit) * 1000.0f;
//	return Call_SetRealtimeScale(mode_obj, 60.0f / (float)*fpsLimit);
//}

void UnwiishedPS2Debug_Init()
{
	LOG("UnwiishedPS2Debug_Init...\n");

	LowLevelPS2_Init();

	uintptr_t loc_FD348 = 0xFD348;
	UnwiishedPS2Debug_sprintf = (int(*)(char*, const char*, ...))(minj_GetBranchDestination(loc_FD348));

	nnRenderStuff_Init();
	nnPrint_Init();
	MDebugSelectEx_Init();

	// There isn't an easy, global way to measure drawing as it's chain called from a parent function (that has nothing to do with drawing) in each class' vftable, so we have to hook each mode individually for profiling...
	MActionSonic_Init();
	MAdventure_Init();
	MItemGet_Init();
	MOption_Init();
	MTitle_Init();
	MLoad_Init();
	MSave_Init();
	MWorld_Init();
	MAttract_Init();
	MMovie_Init();
	MScenario_Init();
	MCredit_Init();
	MSaveMain_Init();
	MAutoSave_Init();
	MGalleryTop_Init();
	MClearResult_Init();
	MLastResult_Init();
	MEvilResult3_Init();
	MNextStageSelect_Init();

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

	// patch heap display inputs to use global inputs instead... -- removed, it's actually read on port 2...
	//minj_WriteMemory32(0x000D6B18, 0x03801021); // move v0, gp
	//minj_WriteMemory32(0x000D6B1C, 0x2442CCDC); // addiu v0, v0, -0x3324

	// hook the dump function
	minj_MakeJMP(0xD5560, (uintptr_t)&UnwiishedPS2Debug_hkHeapManager_Dump);

	// fix "Sonic Action" entry in DebugSelect
	minj_WriteMemory32(0x754740, 4);

	uintptr_t loc_4FE9D8 = 0x4FE9D8;
	UnwiishedPS2Debug_VblCallback = (int(*)(int))(minj_GetPtr(loc_4FE9D8, loc_4FE9D8 + 8));
	minj_WriteLUI_ADDIU(loc_4FE9D8, (uint32_t)&UnwiishedPS2Debug_VblCallback_Hook, MIPSR_a0);

	//uintptr_t loc_500E10 = 0x500E10;
	//UnwiishedPS2Debug_PS2SkeletonSync = (void(*)())(minj_GetBranchDestination(loc_500E10));
	//
	//minj_MakeCALL(loc_500E10, (uintptr_t)&UnwiishedPS2Debug_CustomSync);
	//minj_MakeJMPwNOP(0xD91A0, (uintptr_t)&SetFrameTimeThing_Hook);
}

void UnwiishedPS2Debug_PostInit()
{
    LOG("UnwiishedPS2Debug_PostInit...\n");

	// this needs to happen after nnConfigureSystem...
	nnPrint_InitDebugPrint(10, 13);
	//lastTicks = bGetTicker();
}
