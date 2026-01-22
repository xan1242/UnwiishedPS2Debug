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
	MGalleryTop_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x4AE038, 8, &MGalleryTop_Draw_Hook));
}
