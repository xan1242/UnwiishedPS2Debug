#include "MAutoSave.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MAutoSave_Draw)(void* obj) = (void(*)(void*))(0);
void MAutoSave_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MAutoSave_Draw);
}

void MAutoSave_Init()
{
	MAutoSave_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x481D10, 8, &MAutoSave_Draw_Hook));
}
