#include "MSave.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MSave_Draw)(void* obj) = (void(*)(void*))(0);
void MSave_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MSave_Draw);
}

void MSave_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_40DE24 = 0x40DE24;
	uintptr_t loc_lobytes = loc_40DE24 + 4;
	uintptr_t* MSave_vftbl = (uintptr_t*)minj_GetPtr(loc_40DE24, loc_lobytes);

	MSave_Draw = (void(*)(void*))(MSave_vftbl[vtidx_Draw]);
	MSave_vftbl[vtidx_Draw] = (uintptr_t)&MSave_Draw_Hook;
}
