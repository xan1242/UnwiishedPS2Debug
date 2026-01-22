#include "MClearResult.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MClearResult_Draw)(void* obj) = (void(*)(void*))(0);
void MClearResult_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MClearResult_Draw);
}

void MClearResult_Init()
{
	MClearResult_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x4AD274, 8, &MClearResult_Draw_Hook));
}
