#include "MSaveMain.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MSaveMain_Draw)(void* obj) = (void(*)(void*))(0);
void MSaveMain_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MSaveMain_Draw);
}

void MSaveMain_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_480CA0 = 0x480CA0;
	uintptr_t loc_lobytes = loc_480CA0 + 8;
	uintptr_t* MSaveMain_vftbl = (uintptr_t*)minj_GetPtr(loc_480CA0, loc_lobytes);

	MSaveMain_Draw = (void(*)(void*))(MSaveMain_vftbl[vtidx_Draw]);
	MSaveMain_vftbl[vtidx_Draw] = (uintptr_t)&MSaveMain_Draw_Hook;
}
