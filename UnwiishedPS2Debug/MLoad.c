#include "MLoad.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MLoad_Draw)(void* obj) = (void(*)(void*))(0);
void MLoad_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MLoad_Draw);
}

void MLoad_Init()
{
	MLoad_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x40CFF0, 8, &MLoad_Draw_Hook));
}
