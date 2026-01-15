#include "MWorld.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MWorld_Draw)(void* obj) = (void(*)(void*))(0);
void MWorld_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MWorld_Draw);
}

void MWorld_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_275AF4 = 0x275AF4;
	uintptr_t loc_lobytes = loc_275AF4 + 8;
	uintptr_t* MWorld_vftbl = (uintptr_t*)minj_GetPtr(loc_275AF4, loc_lobytes);

	MWorld_Draw = (void(*)(void*))(MWorld_vftbl[vtidx_Draw]);
	MWorld_vftbl[vtidx_Draw] = (uintptr_t)&MWorld_Draw_Hook;
}
