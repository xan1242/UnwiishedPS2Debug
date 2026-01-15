#include "MCredit.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MCredit_Draw)(void* obj) = (void(*)(void*))(0);
void MCredit_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MCredit_Draw);
}

void MCredit_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_4A4824 = 0x4A4824;
	uintptr_t loc_lobytes = loc_4A4824 + 8;
	uintptr_t* MCredit_vftbl = (uintptr_t*)minj_GetPtr(loc_4A4824, loc_lobytes);

	MCredit_Draw = (void(*)(void*))(MCredit_vftbl[vtidx_Draw]);
	MCredit_vftbl[vtidx_Draw] = (uintptr_t)&MCredit_Draw_Hook;
}
