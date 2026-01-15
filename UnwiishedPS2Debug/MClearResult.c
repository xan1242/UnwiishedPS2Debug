#include "MClearResult.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MClearResult_Draw)(void* obj) = (void(*)(void*))(0);
void MClearResult_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MClearResult_Draw);
}

void MClearResult_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_4AD274 = 0x4AD274;
	uintptr_t loc_lobytes = loc_4AD274 + 8;
	uintptr_t* MClearResult_vftbl = (uintptr_t*)minj_GetPtr(loc_4AD274, loc_lobytes);

	MClearResult_Draw = (void(*)(void*))(MClearResult_vftbl[vtidx_Draw]);
	MClearResult_vftbl[vtidx_Draw] = (uintptr_t)&MClearResult_Draw_Hook;
}
