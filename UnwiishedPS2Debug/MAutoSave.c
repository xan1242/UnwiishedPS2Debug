#include "MAutoSave.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MAutoSave_Draw)(void* obj) = (void(*)(void*))(0);
void MAutoSave_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAutoSave_Draw);
}

void MAutoSave_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_33E550 = 0x481D10;
	uintptr_t loc_lobytes = loc_33E550 + 8;
	uintptr_t* MAutoSave_vftbl = (uintptr_t*)minj_GetPtr(loc_33E550, loc_lobytes);

	MAutoSave_Draw = (void(*)(void*))(MAutoSave_vftbl[vtidx_Draw]);
	MAutoSave_vftbl[vtidx_Draw] = (uintptr_t)&MAutoSave_Draw_Hook;
}
