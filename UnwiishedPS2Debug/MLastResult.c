#include "MLastResult.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MLastResult_Draw)(void* obj) = (void(*)(void*))(0);
void MLastResult_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MLastResult_Draw);
}

void MLastResult_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_330494 = 0x330494;
	uintptr_t loc_lobytes = loc_330494 + 8;
	uintptr_t* MLastResult_vftbl = (uintptr_t*)minj_GetPtr(loc_330494, loc_lobytes);

	MLastResult_Draw = (void(*)(void*))(MLastResult_vftbl[vtidx_Draw]);
	MLastResult_vftbl[vtidx_Draw] = (uintptr_t)&MLastResult_Draw_Hook;
}
