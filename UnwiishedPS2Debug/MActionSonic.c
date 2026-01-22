#include "MActionSonic.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MActionSonic_Draw)(void* obj) = (void(*)(void*))(0);
void MActionSonic_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MActionSonic_Draw);
}

void MActionSonic_Init()
{
	MActionSonic_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0xE74B8, 8, &MActionSonic_Draw_Hook));
}
