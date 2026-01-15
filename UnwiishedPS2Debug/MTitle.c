#include "MTitle.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MTitle_Draw)(void* obj) = (void(*)(void*))(0);
void MTitle_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MTitle_Draw);
}

void MTitle_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_215910 = 0x215910;
	uintptr_t loc_lobytes = loc_215910 + 8;
	uintptr_t* MTitle_vftbl = (uintptr_t*)minj_GetPtr(loc_215910, loc_lobytes);

	MTitle_Draw = (void(*)(void*))(MTitle_vftbl[vtidx_Draw]);
	MTitle_vftbl[vtidx_Draw] = (uintptr_t)&MTitle_Draw_Hook;
}
