#include "MEvilResult3.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MEvilResult3_Draw)(void* obj) = (void(*)(void*))(0);
void MEvilResult3_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MEvilResult3_Draw);
}

void MEvilResult3_Init()
{
	MEvilResult3_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x456B54, 8, &MEvilResult3_Draw_Hook));
}
