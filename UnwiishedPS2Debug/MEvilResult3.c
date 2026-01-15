#include "MEvilResult3.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MEvilResult3_Draw)(void* obj) = (void(*)(void*))(0);
void MEvilResult3_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MEvilResult3_Draw);
}

void MEvilResult3_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_456B54 = 0x456B54;
	uintptr_t loc_lobytes = loc_456B54 + 8;
	uintptr_t* MEvilResult3_vftbl = (uintptr_t*)minj_GetPtr(loc_456B54, loc_lobytes);

	MEvilResult3_Draw = (void(*)(void*))(MEvilResult3_vftbl[vtidx_Draw]);
	MEvilResult3_vftbl[vtidx_Draw] = (uintptr_t)&MEvilResult3_Draw_Hook;
}
