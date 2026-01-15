#include "MItemGet.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MItemGet_Draw)(void* obj) = (void(*)(void*))(0);
void MItemGet_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MItemGet_Draw);
}

void MItemGet_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_44D584 = 0x44D584;
	uintptr_t loc_lobytes = loc_44D584 + 8;
	uintptr_t* MItemGet_vftbl = (uintptr_t*)minj_GetPtr(loc_44D584, loc_lobytes);

	MItemGet_Draw = (void(*)(void*))(MItemGet_vftbl[vtidx_Draw]);
	MItemGet_vftbl[vtidx_Draw] = (uintptr_t)&MItemGet_Draw_Hook;
}
