#include "MNextStageSelect.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MNextStageSelect_Draw)(void* obj) = (void(*)(void*))(0);
void MNextStageSelect_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MNextStageSelect_Draw);
}

void MNextStageSelect_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_4C0684 = 0x4C0684;
	uintptr_t loc_lobytes = loc_4C0684 + 8;
	uintptr_t* MNextStageSelect_vftbl = (uintptr_t*)minj_GetPtr(loc_4C0684, loc_lobytes);

	MNextStageSelect_Draw = (void(*)(void*))(MNextStageSelect_vftbl[vtidx_Draw]);
	MNextStageSelect_vftbl[vtidx_Draw] = (uintptr_t)&MNextStageSelect_Draw_Hook;
}
