#include "MNextStageSelect.h"
#include "includes/minjector.h"
#include "FPSDisplay.h"

void(*MNextStageSelect_Draw)(void* obj) = (void(*)(void*))(0);
void MNextStageSelect_Draw_Hook(void* obj)
{
	FPSDisplay_ProfileDrawTask(obj, MNextStageSelect_Draw);
}

void MNextStageSelect_Init()
{
	MNextStageSelect_Draw = (void(*)(void*))(FPSDisplay_InstallDrawHook(0x4C0684, 8, &MNextStageSelect_Draw_Hook));
}
