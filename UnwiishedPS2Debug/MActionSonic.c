#include "MActionSonic.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MActionSonic_Draw)(void* obj) = (void(*)(void*))(0);
void MActionSonic_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MActionSonic_Draw);
}

void MActionSonic_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_E74B8 = 0xE74B8;
	uintptr_t loc_lobytes = loc_E74B8 + 8;
	uintptr_t* MActionSonic_vftbl = (uintptr_t*)minj_GetPtr(loc_E74B8, loc_lobytes);

	MActionSonic_Draw = (void(*)(void*))(MActionSonic_vftbl[vtidx_Draw]);
	MActionSonic_vftbl[vtidx_Draw] = (uintptr_t)&MActionSonic_Draw_Hook;
}
