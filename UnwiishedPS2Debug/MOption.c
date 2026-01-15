#include "MOption.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MOption_Draw)(void* obj) = (void(*)(void*))(0);
void MOption_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MOption_Draw);
}

void MOption_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_450D44 = 0x450D44;
	uintptr_t loc_lobytes = loc_450D44 + 8;
	uintptr_t* MOption_vftbl = (uintptr_t*)minj_GetPtr(loc_450D44, loc_lobytes);

	MOption_Draw = (void(*)(void*))(MOption_vftbl[vtidx_Draw]);
	MOption_vftbl[vtidx_Draw] = (uintptr_t)&MOption_Draw_Hook;
}
