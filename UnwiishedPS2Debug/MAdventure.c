#include "MAdventure.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MAdventure_Draw)(void* obj) = (void(*)(void*))(0);
void(*MAdventure_Draw_Inner)(void* obj) = (void(*)(void*))(0);

void MAdventure_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAdventure_Draw);
}

void MAdventure_Draw_Inner_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAdventure_Draw_Inner);
}

void MAdventure_Init()
{
	MAdventure_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x33E550, 8, &MAdventure_Draw_Hook));

	uintptr_t loc_33D45C = 0x33D45C;
	MAdventure_Draw_Inner = (void(*)(void*))(minj_GetBranchDestination(loc_33D45C));
	minj_MakeCALL(loc_33D45C, (uintptr_t)&MAdventure_Draw_Inner_Hook);
}
