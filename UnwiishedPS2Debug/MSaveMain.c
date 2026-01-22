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
	MSaveMain_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x480CA0, 8, &MSaveMain_Draw_Hook));
}
