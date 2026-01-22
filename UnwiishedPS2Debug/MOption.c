#include "MOption.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MOption_Draw)(void* obj) = (void(*)(void*))(0);
void MOption_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MOption_Draw);
}

void MOption_Init()
{
	MOption_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x450D44, 8, &FPSDisplay_ProfileDrawTask));
}
