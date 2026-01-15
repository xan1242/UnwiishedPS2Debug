#include "MScenario.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MScenario_Draw)(void* obj) = (void(*)(void*))(0);
void MScenario_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MScenario_Draw);
}

void MScenario_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_3D4434 = 0x3D4434;
	uintptr_t loc_lobytes = loc_3D4434 + 4;
	uintptr_t* MScenario_vftbl = (uintptr_t*)minj_GetPtr(loc_3D4434, loc_lobytes);

	MScenario_Draw = (void(*)(void*))(MScenario_vftbl[vtidx_Draw]);
	MScenario_vftbl[vtidx_Draw] = (uintptr_t)&MScenario_Draw_Hook;
}
