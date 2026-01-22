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
	MScenario_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x3D4434, 4, &MScenario_Draw_Hook));
}
