#include "MAttract.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MAttract_Draw)(void* obj) = (void(*)(void*))(0);
void MAttract_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAttract_Draw);
}

void MAttract_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_3C7B24 = 0x3C7B24;
	uintptr_t loc_lobytes = loc_3C7B24 + 8;
	uintptr_t* MAttract_vftbl = (uintptr_t*)minj_GetPtr(loc_3C7B24, loc_lobytes);

	MAttract_Draw = (void(*)(void*))(MAttract_vftbl[vtidx_Draw]);
	MAttract_vftbl[vtidx_Draw] = (uintptr_t)&MAttract_Draw_Hook;
}
