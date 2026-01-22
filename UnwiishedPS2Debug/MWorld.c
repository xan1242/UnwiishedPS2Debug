#include "MWorld.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MWorld_Draw)(void* obj) = (void(*)(void*))(0);
void MWorld_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MWorld_Draw);
}

void MWorld_Init()
{
	MWorld_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x275AF4, 8, &MWorld_Draw_Hook));
}
