#include "MLoad.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MLoad_Draw)(void* obj) = (void(*)(void*))(0);
void MLoad_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MLoad_Draw);
}

void MLoad_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_40CFF0 = 0x40CFF0;
	uintptr_t loc_lobytes = loc_40CFF0 + 8;
	uintptr_t* MLoad_vftbl = (uintptr_t*)minj_GetPtr(loc_40CFF0, loc_lobytes);

	MLoad_Draw = (void(*)(void*))(MLoad_vftbl[vtidx_Draw]);
	MLoad_vftbl[vtidx_Draw] = (uintptr_t)&MLoad_Draw_Hook;
}
