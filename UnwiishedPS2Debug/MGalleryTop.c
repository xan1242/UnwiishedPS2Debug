#include "MGalleryTop.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MGalleryTop_Draw)(void* obj) = (void(*)(void*))(0);
void MGalleryTop_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MGalleryTop_Draw);
}

void MGalleryTop_Init()
{
	const int idx_vftbl_start = 2;
	const int vtidx_Draw = idx_vftbl_start + 4;

	uintptr_t loc_4AE038 = 0x4AE038;
	uintptr_t loc_lobytes = loc_4AE038 + 8;
	uintptr_t* MGalleryTop_vftbl = (uintptr_t*)minj_GetPtr(loc_4AE038, loc_lobytes);

	MGalleryTop_Draw = (void(*)(void*))(MGalleryTop_vftbl[vtidx_Draw]);
	MGalleryTop_vftbl[vtidx_Draw] = (uintptr_t)&MGalleryTop_Draw_Hook;
}
