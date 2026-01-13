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
	int adventureMissionIdx;
	int capitalID; 
	int stageID;
	int startArea;
	int endArea;
	int isEvil;
	int specialMode;
	char unk1[0x6C];
	// mission settings
	int16_t UniqId;
	int16_t StageType;
	int16_t StageNo;
	int16_t MissionId;
	int16_t MissionType;
	int16_t padd1_unk;
	float LimitTime;
	char unk2[0x18]; // AreaLimitTime?
	int mission_StartArea;
	int mission_EndArea;
	int16_t ClearNum;
	int16_t StartPoint;
	int16_t StartCamera;
	int16_t LoadType;
} SonicActionSettings;

typedef struct
{
	int StageNo;
	int CapitalNo;
} StageToCapitalIdMap;


#define MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS 7
#define MDEBUGSELECTEX_POS_MISSIONSETTINGS MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS
#define MDEBUGSELECTEX_NUM_MISSIONSETTINGS 9
#define MDEBUGSELECTEX_NUM_SETTINGS (MDEBUGSELECTEX_NUM_MISSIONSETTINGS + MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS)

#define SONICACTIONSETTINGS_NUM_CAPITAL 7
#define SONICACTIONSETTINGS_NUM_AREAS 7
#define SONICACTIONSETTINGS_NUM_SPECIALMODE 6
#define SONICACTIONSETTINGS_NUM_STAGETYPE 2

typedef enum
{
	SAS_MissionIdx,
	SAS_Capital,
	SAS_Stage,
	SAS_StartArea,
	SAS_EndArea,
	SAS_IsEvil,
	SAS_SpecialMode,
	// mission settings
	SAS_UniqId,
	SAS_StageType,
	SAS_MissionId,
	SAS_MissionType,
	SAS_LimitTime,
	SAS_ClearNum,
	SAS_StartPoint,
	SAS_StartCamera,
	SAS_LoadType,
} SonicActionSettingEnum;

typedef enum
{
	SASM_Normal,
	SASM_TimeAttack,
	SASM_Survival,
	SASM_DragoonBoss,
	SASM_Boss,
	SASM_Entrance,
} SonicActionSettingModeEnum;

typedef enum
{
	SAMT_None = -1,
	SAMT_CollectRings,
	SAMT_TimeAttack,
	SAMT_NoDamageChallenge,
	SAMT_NoBreakChallenge,
	SAMT_Tutorial_Sonic_FirstJump = 10,
	SAMT_Tutorial_Sonic_Drift,
	SAMT_Tutorial_Sonic_HomingAttack,
	SAMT_Tutorial_Sonic_Boost,
	SAMT_Tutorial_Sonic_QuickStep,
	SAMT_Tutorial_Sonic_Slide,
	SAMT_Tutorial_Sonic_Stomp,
	SAMT_Tutorial_Sonic_LightspeedDash,
	SAMT_Tutorial_Sonic_Jump,
	SAMT_Evil_ClearEnemies = 100,
	SAMT_Evil_NoDamageChallenge,
	SAMT_Evil_NoBreakChallenge,
	SAMT_Evil_CollectItems,
	SAMT_Tutorial_Evil_FirstJump = 110,
	SAMT_Tutorial_Evil_Climb,
	SAMT_Tutorial_Evil_ComboAttack,
	SAMT_Tutorial_Evil_Throw1,
	SAMT_Tutorial_Evil_SpecialMove1,
	SAMT_Tutorial_Evil_SpecialMove2,
	SAMT_Tutorial_Evil_SpecialMove3,
	SAMT_Tutorial_Evil_SpecialMove4,
	SAMT_Tutorial_Evil_SpecialMove5,
	SAMT_Tutorial_Evil_Throw2,
	SAMT_Tutorial_Evil_Throw3,
} SonicActionMissionType;

void MDebugSelectEx_Init();

#endif