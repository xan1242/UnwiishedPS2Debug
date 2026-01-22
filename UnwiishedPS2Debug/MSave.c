#include "MSave.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MSave_Draw)(void* obj) = (void(*)(void*))(0);
void MSave_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MSave_Draw);
}

void MSave_Init()
{
	MSave_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x40DE24, 4, &MSave_Draw_Hook));
}
