#include "MCredit.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MCredit_Draw)(void* obj) = (void(*)(void*))(0);
void MCredit_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MCredit_Draw);
}

void MCredit_Init()
{
	MCredit_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x4A4824, 8, &MCredit_Draw_Hook));
}
