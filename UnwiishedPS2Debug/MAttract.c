#include "MAttract.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MAttract_Draw)(void* obj) = (void(*)(void*))(0);
void MAttract_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAttract_Draw);
}

void MAttract_Init()
{
	MAttract_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x3C7B24, 8, &MAttract_Draw_Hook));
}
