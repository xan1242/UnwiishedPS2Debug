#include "MLastResult.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MLastResult_Draw)(void* obj) = (void(*)(void*))(0);
void MLastResult_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MLastResult_Draw);
}

void MLastResult_Init()
{
	MLastResult_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x330494, 8, &MLastResult_Draw_Hook));
}
