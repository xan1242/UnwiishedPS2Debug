#include "MMovie.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MMovie_Draw)(void* obj) = (void(*)(void*))(0);
void MMovie_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MMovie_Draw);
}

void MMovie_Init()
{
	MMovie_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x3FB864, 8, &MMovie_Draw_Hook));
}
