#include "MMovie.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MMovie_Draw)(void* obj) = (void(*)(void*))(0);
void MMovie_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MMovie_Draw);
}

void MMovie_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_3FB864 = 0x3FB864;
	uintptr_t loc_lobytes = loc_3FB864 + 8;
	uintptr_t* MMovie_vftbl = (uintptr_t*)minj_GetPtr(loc_3FB864, loc_lobytes);

	MMovie_Draw = (void(*)(void*))(MMovie_vftbl[vtidx_Draw]);
	MMovie_vftbl[vtidx_Draw] = (uintptr_t)&MMovie_Draw_Hook;
}
