#ifndef MDEBUGSELECTEX_H
#define MDEBUGSELECTEX_H

#include <stddef.h>
#include <stdint.h>

typedef struct
{
	uintptr_t unk;
	int selection;
} MDebugSelect;

typedef struct
{
	int missionID;
	int capitalID; 
	int stageID;
	int startArea;
	int currentArea;
	int isEvil;
	int specialMode;
} SonicActionSettings;

#define MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS 6
#define SONICACTIONSETTINGS_NUM_CAPITAL 7
#define SONICACTIONSETTINGS_NUM_AREAS 7
#define SONICACTIONSETTINGS_NUM_SPECIALMODE 6

typedef enum
{
	SAS_Mission,
	SAS_Capital,
	SAS_Stage,
	SAS_StartArea,
	SAS_IsEvil,
	SAS_SpecialMode,
} SonicActionSettingEnum;

typedef enum
{
	SASM_Normal,
	SASM_Unk1,
	SASM_Unk2,
	SASM_DragoonBoss,
	SASM_Boss,
	SASM_Entrance,
} SonicActionSettingModeEnum;

void MDebugSelectEx_Init();

#endif