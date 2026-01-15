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
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_33E550 = 0x33E550;
	uintptr_t loc_lobytes = loc_33E550 + 8;
	uintptr_t* MAdventure_vftbl = (uintptr_t*)minj_GetPtr(loc_33E550, loc_lobytes);

	MAdventure_Draw = (void(*)(void*))(MAdventure_vftbl[vtidx_Draw]);
	MAdventure_vftbl[vtidx_Draw] = (uintptr_t)&MAdventure_Draw_Hook;

	uintptr_t loc_33D45C = 0x33D45C;
	MAdventure_Draw_Inner = (void(*)(void*))(minj_GetBranchDestination(loc_33D45C));
	minj_MakeCALL(loc_33D45C, (uintptr_t)&MAdventure_Draw_Inner_Hook);
}
