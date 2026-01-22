#include "MItemGet.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MItemGet_Draw)(void* obj) = (void(*)(void*))(0);
void MItemGet_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MItemGet_Draw);
}

void MItemGet_Init()
{
	MItemGet_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x44D584, 8, &MItemGet_Draw_Hook));
}
