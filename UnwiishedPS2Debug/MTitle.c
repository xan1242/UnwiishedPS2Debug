#include "MTitle.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MTitle_Draw)(void* obj) = (void(*)(void*))(0);
void MTitle_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MTitle_Draw);
}

void MTitle_Init()
{
	MTitle_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x215910, 8, &MTitle_Draw_Hook));
}
