#include "MDebugSelectEx.h"
#include "UnwiishedPS2Debug.h"
#include "nnPrint.h"
#include "includes/minjector.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool bDoingSonicActionSetup = false;
int MDebugSelectEx_SonicAction_Selection = 0;

const char MDebugSelectEx_SonicAction_AreaLetters[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
const char* MDebugSelectEx_SonicAction_EntranceNames[] = 
{
	"Mykonos",
	"Tundra",
	"Eu",
	"China",
	"Petra",
	"Beach",
	"Africa/Eggmanland",
};

const char* MDebugSelectEx_SonicAction_StageNames[] =
{
	"Windmill Isle",
	"Cool Edge",
	"Rooftop Run",
	"Dragon Road",
	"Arid Sands",
	"Jungle Joyride",
	"Eggmanland",
};

const char* MDebugSelectEx_SonicAction_BossNames[] =
{
	"Egg Beetle",
	"Dark Moray",
	"Egg Devil Ray",
	"Dark Gaia Phoenix",
	"Dark Guardian",
	"Egg Lancer",
	"Egg Dragoon",
	"Dark Gaia",
	"Perfect Dark Gaia",
};

const char* MDebugSelectEx_SonicAction_StageTypes[] =
{
	"Normal",
	"Mission",
};

uint32_t controllerDummy[4] = { 0 };

SonicActionSettings* MDebugSelectEx_SonicActionSettings = (SonicActionSettings*)0x87F830;
StageToCapitalIdMap* MDebugSelectEx_StageToCapitalMap = (StageToCapitalIdMap*)0x7A9560;
const size_t MDebugSelectEx_StageToCapitalMap_Size = 0x5A;

void MDebugSelectEx_GotoSonicActionMode()
{
	void* manager = UnwiishedPS2Debug_uGetModeManagerThingy();
	if (!manager)
		return;
	UnwiishedPS2Debug_uSetNextGameModeThingy(manager, 4);
	*(int*)p_ModeUpdateTrigger = 1;
}

int MDebugSelectEx_GetAdventureMissionIdx(int stageID)
{
	for (int i = 0; i < MDebugSelectEx_StageToCapitalMap_Size; i++)
	{
		if (MDebugSelectEx_StageToCapitalMap[i].StageNo == stageID)
			return i;
	}
	return 0;
}

void MDebugSelectEx_GetStageName(int stageID, char* outBuffer)
{
	switch (stageID)
	{
		case 175: // Perfect Dark Gaia
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Sonic", MDebugSelectEx_SonicAction_BossNames[8]);
			return;
		case 174: // Egg Dragoon
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Evil", MDebugSelectEx_SonicAction_BossNames[6]);
			return;
		case 155: // Dark Guardian
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Evil", MDebugSelectEx_SonicAction_BossNames[4]);
			return;
		case 145: // Dark Gaia Phoenix
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Evil", MDebugSelectEx_SonicAction_BossNames[3]);
			return;
		case 125: // Dark Moray
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Evil", MDebugSelectEx_SonicAction_BossNames[1]);
			return;
		case 75: // Dark Gaia
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Sonic", MDebugSelectEx_SonicAction_BossNames[7]);
			return;
		case 65: // Egg Lancer
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Sonic", MDebugSelectEx_SonicAction_BossNames[5]);
			return;
		case 35: // Egg Devil Ray
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Sonic", MDebugSelectEx_SonicAction_BossNames[2]);
			return;
		case 15: // Egg Beetle
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s Boss] %s", "Sonic", MDebugSelectEx_SonicAction_BossNames[0]);
			return;
		default:
			break;
	}

	int lowStageID = 0;
	int midStageID = 0;
	int hiStageID = 0;

	if (MDebugSelectEx_SonicActionSettings->stageID > 0)
	{
		lowStageID = MDebugSelectEx_SonicActionSettings->stageID - (MDebugSelectEx_SonicActionSettings->stageID / 10) * 10;
		hiStageID = MDebugSelectEx_SonicActionSettings->stageID / 100;
	}

	if (MDebugSelectEx_SonicActionSettings->stageID >= 10)
		midStageID = (MDebugSelectEx_SonicActionSettings->stageID / 10) - ((MDebugSelectEx_SonicActionSettings->stageID / 100) * 10);


	if ((hiStageID >= 1) && (lowStageID == 6))
	{
		const char* entName = "Unknown";
		if ((midStageID > 0) && (midStageID <= SONICACTIONSETTINGS_NUM_CAPITAL))
			entName = MDebugSelectEx_SonicAction_EntranceNames[midStageID - 1];
		UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Entrance", entName);
		return;
	}

	const char* hiName = "Sonic";
	if (hiStageID >= 1)
		hiName = "Evil";

	const char* midName = "Unknown";
	if ((midStageID > 0) && (midStageID <= SONICACTIONSETTINGS_NUM_CAPITAL))
		midName = MDebugSelectEx_SonicAction_StageNames[midStageID - 1];

	UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s Act %d", hiName, midName, lowStageID);
	return;
}


void MDebugSelectEx_ChangeSetting(int setting, int inc)
{
	switch (setting)
	{
		case SAS_LoadType:
		{
			MDebugSelectEx_SonicActionSettings->LoadType += inc;
			if (MDebugSelectEx_SonicActionSettings->LoadType < 0)
				MDebugSelectEx_SonicActionSettings->LoadType = 0;
			break;
		}
		case SAS_StartCamera:
		{
			MDebugSelectEx_SonicActionSettings->StartCamera += inc;
			if (MDebugSelectEx_SonicActionSettings->StartCamera < 0)
				MDebugSelectEx_SonicActionSettings->StartCamera = 0;
			break;
		}
		case SAS_StartPoint:
		{
			MDebugSelectEx_SonicActionSettings->StartPoint += inc;
			if (MDebugSelectEx_SonicActionSettings->StartPoint < 0)
				MDebugSelectEx_SonicActionSettings->StartPoint = 0;
			break;
		}
		case SAS_ClearNum:
		{
			MDebugSelectEx_SonicActionSettings->ClearNum += inc;
			if (MDebugSelectEx_SonicActionSettings->ClearNum < 0)
				MDebugSelectEx_SonicActionSettings->ClearNum = 0;
			break;
		}
		case SAS_LimitTime:
		{
			float fInc = (float)inc;
			MDebugSelectEx_SonicActionSettings->LimitTime += fInc;
			if (MDebugSelectEx_SonicActionSettings->LimitTime < 0)
				MDebugSelectEx_SonicActionSettings->LimitTime = 0;
			MDebugSelectEx_SonicActionSettings->padd1_unk = 0; // #TODO figure out what this is, if it's set it messes with the time limit
			break;
		}
		case SAS_MissionType:
		{
			MDebugSelectEx_SonicActionSettings->MissionType += inc;
			if (MDebugSelectEx_SonicActionSettings->MissionType < -1)
				MDebugSelectEx_SonicActionSettings->MissionType = -1;
			break;
		}
		case SAS_MissionId:
		{
			MDebugSelectEx_SonicActionSettings->MissionId += inc;
			if (MDebugSelectEx_SonicActionSettings->MissionId < 0)
				MDebugSelectEx_SonicActionSettings->MissionId = 0;
			break;
		}
		case SAS_StageType:
		{
			MDebugSelectEx_SonicActionSettings->StageType += inc;
			if (MDebugSelectEx_SonicActionSettings->StageType < 0)
				MDebugSelectEx_SonicActionSettings->StageType = SONICACTIONSETTINGS_NUM_STAGETYPE - 1;

			MDebugSelectEx_SonicActionSettings->StageType %= SONICACTIONSETTINGS_NUM_STAGETYPE;
			break;
		}
		case SAS_UniqId:
		{
			MDebugSelectEx_SonicActionSettings->UniqId += inc;
			if (MDebugSelectEx_SonicActionSettings->UniqId < 0)
				MDebugSelectEx_SonicActionSettings->UniqId = 0;
			break;
		}
		case SAS_SpecialMode:
		{
			MDebugSelectEx_SonicActionSettings->specialMode += inc;
			if (MDebugSelectEx_SonicActionSettings->specialMode < 0)
				MDebugSelectEx_SonicActionSettings->specialMode = SONICACTIONSETTINGS_NUM_SPECIALMODE - 1;

			MDebugSelectEx_SonicActionSettings->specialMode %= SONICACTIONSETTINGS_NUM_SPECIALMODE;
			break;
		}
		case SAS_IsEvil:
		{
			bool state = MDebugSelectEx_SonicActionSettings->isEvil != 0;
			state = !state;
			MDebugSelectEx_SonicActionSettings->isEvil = state;
			break;
		}
		case SAS_EndArea:
		{
			MDebugSelectEx_SonicActionSettings->endArea += inc;
			if (MDebugSelectEx_SonicActionSettings->endArea < 0)
				MDebugSelectEx_SonicActionSettings->endArea = SONICACTIONSETTINGS_NUM_AREAS - 1;

			MDebugSelectEx_SonicActionSettings->endArea %= SONICACTIONSETTINGS_NUM_AREAS;
			MDebugSelectEx_SonicActionSettings->mission_EndArea = MDebugSelectEx_SonicActionSettings->endArea;
			break;
		}
		case SAS_StartArea:
		{
			MDebugSelectEx_SonicActionSettings->startArea += inc;
			if (MDebugSelectEx_SonicActionSettings->startArea < 0)
				MDebugSelectEx_SonicActionSettings->startArea = SONICACTIONSETTINGS_NUM_AREAS - 1;

			MDebugSelectEx_SonicActionSettings->startArea %= SONICACTIONSETTINGS_NUM_AREAS;
			MDebugSelectEx_SonicActionSettings->mission_StartArea = MDebugSelectEx_SonicActionSettings->startArea;
			break;
		}
		case SAS_Stage:
		{
			MDebugSelectEx_SonicActionSettings->stageID += inc;
			if (MDebugSelectEx_SonicActionSettings->stageID < 0)
				MDebugSelectEx_SonicActionSettings->stageID = 0;

			MDebugSelectEx_SonicActionSettings->StageNo = MDebugSelectEx_SonicActionSettings->stageID;
			MDebugSelectEx_SonicActionSettings->specialMode = SASM_Normal;
			MDebugSelectEx_SonicActionSettings->startArea = 0;
			MDebugSelectEx_SonicActionSettings->mission_StartArea = MDebugSelectEx_SonicActionSettings->startArea;
			MDebugSelectEx_SonicActionSettings->endArea = 0;
			MDebugSelectEx_SonicActionSettings->mission_EndArea = MDebugSelectEx_SonicActionSettings->endArea;

			int lowStageID = 0;
			int midStageID = 0;

			if (MDebugSelectEx_SonicActionSettings->stageID > 0)
				lowStageID = MDebugSelectEx_SonicActionSettings->stageID - (MDebugSelectEx_SonicActionSettings->stageID / 10) * 10;

			if (MDebugSelectEx_SonicActionSettings->stageID >= 10)
				midStageID = (MDebugSelectEx_SonicActionSettings->stageID / 10) - ((MDebugSelectEx_SonicActionSettings->stageID / 100) * 10);

			if (midStageID <= 0)
				midStageID = 1;

			if (midStageID > 7)
				midStageID = 7;

			if (MDebugSelectEx_SonicActionSettings->stageID / 100)
			{
				MDebugSelectEx_SonicActionSettings->isEvil = 1;
				if (MDebugSelectEx_SonicActionSettings->stageID == 174)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_DragoonBoss;
				else if (lowStageID == 5)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Boss;
				else if (lowStageID == 6)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Entrance;

				if (MDebugSelectEx_SonicActionSettings->stageID == 175)
					MDebugSelectEx_SonicActionSettings->isEvil = 0;
			}
			else
			{
				MDebugSelectEx_SonicActionSettings->isEvil = 0;
				if (lowStageID == 5)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Boss;
			}

			MDebugSelectEx_SonicActionSettings->adventureMissionIdx = MDebugSelectEx_GetAdventureMissionIdx(MDebugSelectEx_SonicActionSettings->stageID);
			if (MDebugSelectEx_SonicActionSettings->adventureMissionIdx < 0)
				MDebugSelectEx_SonicActionSettings->capitalID = MDebugSelectEx_StageToCapitalMap[MDebugSelectEx_SonicActionSettings->adventureMissionIdx].CapitalNo;
			else
				MDebugSelectEx_SonicActionSettings->capitalID = midStageID - 1;

			break;
		}
		case SAS_Capital:
		{
			MDebugSelectEx_SonicActionSettings->capitalID += inc;
			if (MDebugSelectEx_SonicActionSettings->capitalID < 0)
				MDebugSelectEx_SonicActionSettings->capitalID = SONICACTIONSETTINGS_NUM_CAPITAL - 1;

			MDebugSelectEx_SonicActionSettings->capitalID %= SONICACTIONSETTINGS_NUM_CAPITAL;
			break;
		}
		case SAS_MissionIdx:
		{
			MDebugSelectEx_SonicActionSettings->adventureMissionIdx += inc;
			if (MDebugSelectEx_SonicActionSettings->adventureMissionIdx < 0)
				MDebugSelectEx_SonicActionSettings->adventureMissionIdx = 0;
			break;
		}
		default:
			break;
	}
}

const char* MDebugSelectEx_GetSettingName(int setting, int val)
{
	switch (setting)
	{
		case SAS_SpecialMode:
		{
			switch (val)
			{
				case SASM_Normal:
					return "Normal";
				case SASM_TimeAttack:
					return "TimeAttack";
				case SASM_Survival:
					return "Survival";
				case SASM_DragoonBoss:
					return "EggDragoon"; // no idea about this one...
				case SASM_Boss:
					return "Boss";
				case SASM_Entrance:
					return "Entrance";
			}
			break;
		}

		case SAS_Capital:
		{
			if ((val >= 0) && (val <= 6))
			{
				return MDebugSelectEx_SonicAction_EntranceNames[val];
			}
		}

	}
	return "Unknown";
}


uint32_t*(*MDebugSelectEx_GetInput)(void* unk, int type) = (uint32_t*(*)(void*, int))(0);
uint32_t* MDebugSelectEx_hkGetInput(void* unk, int type)
{
	uint32_t* menuInputs = MDebugSelectEx_GetInput(unk, type);
	uint32_t buttonMask = menuInputs[2];
	if (buttonMask & 0x0008) // R1
		bDoingSonicActionSetup = !bDoingSonicActionSetup;

	if (!bDoingSonicActionSetup)
		return menuInputs;

	
	uint32_t buttonMask2 = menuInputs[4];

	if (buttonMask & 0x0020) // Circle
	{
		MDebugSelectEx_GotoSonicActionMode();
	}

	if (buttonMask2 & 0x1000) // up
	{
		MDebugSelectEx_SonicAction_Selection--;
		if (MDebugSelectEx_SonicAction_Selection < 0)
			MDebugSelectEx_SonicAction_Selection = MDEBUGSELECTEX_NUM_SETTINGS - 1;
	}
	else if (buttonMask2 & 0x4000) // down
	{
		MDebugSelectEx_SonicAction_Selection++;
		MDebugSelectEx_SonicAction_Selection %= MDEBUGSELECTEX_NUM_SETTINGS;
	}

	uint32_t incButtonMask = buttonMask2;
	if (menuInputs[0] & 0x0080) // square = speed boost
		incButtonMask = menuInputs[0];

	if (incButtonMask & 0x8000) // left
	{
		MDebugSelectEx_ChangeSetting(MDebugSelectEx_SonicAction_Selection, -1);
	}
	else if (incButtonMask & 0x2000) // right
	{
		MDebugSelectEx_ChangeSetting(MDebugSelectEx_SonicAction_Selection, 1);
	}

	return controllerDummy;
}



void(*MDebugSelectEx_nnPrint)(int x, int y, const char* fmt, ...) = (void(*)(int, int, const char*, ...))(0);
void MDebugSelectEx_hk_nnPrint(int x, int y, const char* fmt)
{
	if (!bDoingSonicActionSetup)
		return MDebugSelectEx_nnPrint(x, y, fmt);

	const int baseX = 33;
	const int baseCurX = baseX - 3;
	int baseY = 3;
	if (MDebugSelectEx_SonicAction_Selection >= MDEBUGSELECTEX_POS_MISSIONSETTINGS)
		baseY++;

	int posY = MDebugSelectEx_SonicAction_Selection + baseY;

	return MDebugSelectEx_nnPrint(baseCurX, posY, fmt);
}

void MDebugSelectEx_HandleSonicActionSetup()
{
	char stgName[64];

	nnSetPrintColor(0xCCCCCCFF);
	const int baseX = 33;
	const int baseTitleX = baseX - 5;
	const int baseY = 2;
	int posY = baseY;

	MDebugSelectEx_GetStageName(MDebugSelectEx_SonicActionSettings->stageID, stgName);

	nnPrint(baseTitleX, baseY - 1, "< SONIC ACTION SETTINGS >");
	nnPrint(baseTitleX + 2, posY++, "%s", stgName);
	nnPrint(baseX, posY++, "AdvMissionIdx = %d", MDebugSelectEx_SonicActionSettings->adventureMissionIdx);
	nnPrint(baseX, posY++, "Capital = %s", MDebugSelectEx_GetSettingName(SAS_Capital, MDebugSelectEx_SonicActionSettings->capitalID));
	nnPrint(baseX, posY++, "Stage = Stage%03d", MDebugSelectEx_SonicActionSettings->stageID);
	nnPrint(baseX, posY++, "StartArea = %c", MDebugSelectEx_SonicAction_AreaLetters[MDebugSelectEx_SonicActionSettings->startArea]);
	nnPrint(baseX, posY++, "EndArea = %c", MDebugSelectEx_SonicAction_AreaLetters[MDebugSelectEx_SonicActionSettings->endArea]);
	nnPrint(baseX, posY++, "IsEvil = %d", MDebugSelectEx_SonicActionSettings->isEvil);
	nnPrint(baseX, posY++, "Mode = %s", MDebugSelectEx_GetSettingName(SAS_SpecialMode, MDebugSelectEx_SonicActionSettings->specialMode));
	nnPrint(baseTitleX + 2, posY++, "< MISSION SETTINGS >");
	nnPrint(baseX, posY++, "UniqId = %hd", MDebugSelectEx_SonicActionSettings->UniqId);
	nnPrint(baseX, posY++, "StageType = %s", MDebugSelectEx_SonicAction_StageTypes[MDebugSelectEx_SonicActionSettings->StageType]);
	nnPrint(baseX, posY++, "MissionId = %hd", MDebugSelectEx_SonicActionSettings->MissionId);
	nnPrint(baseX, posY++, "MissionType = %hd", MDebugSelectEx_SonicActionSettings->MissionType);
	nnPrint(baseX, posY++, "LimitTime = %.1f", MDebugSelectEx_SonicActionSettings->LimitTime);
	nnPrint(baseX, posY++, "ClearNum = %hd", MDebugSelectEx_SonicActionSettings->ClearNum);
	nnPrint(baseX, posY++, "StartPoint = %hd", MDebugSelectEx_SonicActionSettings->StartPoint);
	nnPrint(baseX, posY++, "StartCamera = %hd", MDebugSelectEx_SonicActionSettings->StartCamera);
	nnPrint(baseX, posY++, "LoadType = %hd", MDebugSelectEx_SonicActionSettings->LoadType);

}

int MDebugSelectEx_UpdateSelectionThingy(MDebugSelect* that, int(*fn)(MDebugSelect*))
{
	int retVal = fn(that);

	if (bDoingSonicActionSetup)
	{
		int posY = 22;

		MDebugSelectEx_HandleSonicActionSetup();

		nnSetPrintColor(0xFFFFFFFF);
		nnPrint(4, posY++, "Press R1 to go back to mode selection");
		nnPrint(4, posY++, "Left/Right to change values");
		nnPrint(4, posY++, "Hold [] for faster changes");
		nnPrint(4, posY++, "Press O to start the stage");
	}
	else
	{
		nnSetPrintColor(0xFFFFFFFF);
		nnPrint(4, 22, "Press R1 to change Sonic Action settings");
	}

	return retVal;
}

int(*MDebugSelect_UpdateSelectionThingy1)(MDebugSelect* that) = (int(*)(MDebugSelect*))(0);
int MDebugSelectEx_hkUpdateSelectionThingy1(MDebugSelect* that)
{
	return MDebugSelectEx_UpdateSelectionThingy(that, MDebugSelect_UpdateSelectionThingy1);
}

int(*MDebugSelect_UpdateSelectionThingy2)(MDebugSelect* that) = (int(*)(MDebugSelect*))(0);
int MDebugSelectEx_hkUpdateSelectionThingy2(MDebugSelect* that)
{
	return MDebugSelectEx_UpdateSelectionThingy(that, MDebugSelect_UpdateSelectionThingy2);
}

void MDebugSelectEx_Init()
{
	uintptr_t loc_F24B0 = 0xF24B0;
	MDebugSelectEx_GetInput = (uint32_t*(*)(void*, int))(minj_GetBranchDestination(loc_F24B0));
	minj_MakeCALL(loc_F24B0, (uintptr_t)&MDebugSelectEx_hkGetInput);

	uintptr_t loc_F23C0 = 0xF23C0;
	MDebugSelect_UpdateSelectionThingy1 = (int(*)(MDebugSelect*))(minj_GetBranchDestination(loc_F23C0));
	minj_MakeCALL(loc_F23C0, (uintptr_t)&MDebugSelectEx_hkUpdateSelectionThingy1);

	uintptr_t loc_F2414 = 0xF2414;
	MDebugSelect_UpdateSelectionThingy2 = (int(*)(MDebugSelect*))(minj_GetBranchDestination(loc_F2414));
	minj_MakeCALL(loc_F2414, (uintptr_t)&MDebugSelectEx_hkUpdateSelectionThingy2);

	uintptr_t loc_F26B0 = 0xF26B0;
	MDebugSelectEx_nnPrint = (void(*)(int, int, const char*, ...))(minj_GetBranchDestination(loc_F26B0));
	minj_MakeCALL(loc_F26B0, (uintptr_t)&MDebugSelectEx_hk_nnPrint);
}