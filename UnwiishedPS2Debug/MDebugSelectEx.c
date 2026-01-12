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
};


uint32_t controllerDummy[4] = { 0 };

SonicActionSettings* MDebugSelectEx_SonicActionSettings = (SonicActionSettings*)0x87F830;

void MDebugSelectEx_GotoSonicActionMode()
{
	void* manager = UnwiishedPS2Debug_uGetModeManagerThingy();
	if (!manager)
		return;
	UnwiishedPS2Debug_uSetNextGameModeThingy(manager, 4);
	*(int*)p_ModeUpdateTrigger = 1;
}

int MDebugSelectEx_GetMissionID(int stageID)
{
	switch (stageID)
	{
		case 174: // Egg Dragoon
			return 87;
		case 155: // Dark Guardian
			return 76;
		case 145: // Dark Gaia Phoenix
			return 70;
		case 125: // Dark Moray
			return 58;
		case 75: // Dark Gaia
			return 46;
		case 65: // Egg Lancer
			return 40;
		case 35: // Egg Devil Ray
			return 22;
		case 15: // Egg Beetle
			return 10;
		default:
			break;
	}
	return 0;
}

void MDebugSelectEx_GetStageName(int stageID, char* outBuffer)
{
	switch (stageID)
	{
		case 174:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Evil Boss", MDebugSelectEx_SonicAction_BossNames[6]);
			return;
		case 155:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Evil Boss", MDebugSelectEx_SonicAction_BossNames[4]);
			return;
		case 145:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Evil Boss", MDebugSelectEx_SonicAction_BossNames[3]);
			return;
		case 125:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Evil Boss", MDebugSelectEx_SonicAction_BossNames[1]);
			return;
		case 75:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Sonic Boss", MDebugSelectEx_SonicAction_BossNames[7]);
			return;
		case 65:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Sonic Boss", MDebugSelectEx_SonicAction_BossNames[5]);
			return;
		case 35:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Sonic Boss", MDebugSelectEx_SonicAction_BossNames[2]);
			return;
		case 15:
			UnwiishedPS2Debug_sprintf(outBuffer, "[%s] %s", "Sonic Boss", MDebugSelectEx_SonicAction_BossNames[0]);
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

	const char* hiName = "[Sonic]";
	if (hiStageID >= 1)
		hiName = "[Evil]";

	const char* midName = "Unknown";
	if ((midStageID > 0) && (midStageID <= SONICACTIONSETTINGS_NUM_CAPITAL))
		midName = MDebugSelectEx_SonicAction_StageNames[midStageID - 1];

	UnwiishedPS2Debug_sprintf(outBuffer, "%s %s Act %d", hiName, midName, lowStageID);
	return;
}


void MDebugSelectEx_ChangeSetting(int setting, int inc)
{
	switch (setting)
	{
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
		case SAS_StartArea:
		{
			MDebugSelectEx_SonicActionSettings->startArea += inc;
			if (MDebugSelectEx_SonicActionSettings->startArea < 0)
				MDebugSelectEx_SonicActionSettings->startArea = SONICACTIONSETTINGS_NUM_AREAS - 1;

			MDebugSelectEx_SonicActionSettings->startArea %= SONICACTIONSETTINGS_NUM_AREAS;
			break;
		}
		case SAS_Stage:
		{
			MDebugSelectEx_SonicActionSettings->stageID += inc;
			if (MDebugSelectEx_SonicActionSettings->stageID < 0)
				MDebugSelectEx_SonicActionSettings->stageID = 0;

			MDebugSelectEx_SonicActionSettings->specialMode = SASM_Normal;
			MDebugSelectEx_SonicActionSettings->startArea = 0;

			int lowStageID = 0;
			int midStageID = 0;

			if (MDebugSelectEx_SonicActionSettings->stageID > 0)
				lowStageID = MDebugSelectEx_SonicActionSettings->stageID - (MDebugSelectEx_SonicActionSettings->stageID / 10) * 10;

			if (MDebugSelectEx_SonicActionSettings->stageID >= 10)
				midStageID = (MDebugSelectEx_SonicActionSettings->stageID / 10) - ((MDebugSelectEx_SonicActionSettings->stageID / 100) * 10);

			if (MDebugSelectEx_SonicActionSettings->stageID / 100)
			{
				MDebugSelectEx_SonicActionSettings->isEvil = 1;
				if (MDebugSelectEx_SonicActionSettings->stageID == 174)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_DragoonBoss;
				else if (lowStageID == 5)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Boss;
				else if (lowStageID == 6)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Entrance;
			}
			else
			{
				MDebugSelectEx_SonicActionSettings->isEvil = 0;
				if (lowStageID == 5)
					MDebugSelectEx_SonicActionSettings->specialMode = SASM_Boss;
			}

			if (midStageID <= 0)
				midStageID = 1;

			if (midStageID > 7)
				midStageID = 7;


			MDebugSelectEx_SonicActionSettings->capitalID = midStageID - 1;
			MDebugSelectEx_SonicActionSettings->missionID = MDebugSelectEx_GetMissionID(MDebugSelectEx_SonicActionSettings->stageID);

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
		case SAS_Mission:
		{
			MDebugSelectEx_SonicActionSettings->missionID += inc;
			if (MDebugSelectEx_SonicActionSettings->missionID < 0)
				MDebugSelectEx_SonicActionSettings->missionID = 0;
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
				case 0:
					return "Normal";
				case 1:
					return "Unknown (1)";
				case 2:
					return "Unknown (2)";
				case 3:
					return "EggDragoon"; // no idea about this one...
				case 4:
					return "Boss";
				case 5:
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
			MDebugSelectEx_SonicAction_Selection = MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS - 1;
	}
	else if (buttonMask2 & 0x4000) // down
	{
		MDebugSelectEx_SonicAction_Selection++;
		MDebugSelectEx_SonicAction_Selection %= MDEBUGSELECTEX_NUM_SONICACTIONSETTINGS;
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
	const int baseY = 2;

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
	nnPrint(baseX, posY++, "Mission = %d", MDebugSelectEx_SonicActionSettings->missionID);
	nnPrint(baseX, posY++, "Capital = %s", MDebugSelectEx_GetSettingName(SAS_Capital, MDebugSelectEx_SonicActionSettings->capitalID));
	nnPrint(baseX, posY++, "Stage = Stage%03d", MDebugSelectEx_SonicActionSettings->stageID);
	nnPrint(baseX, posY++, "StartArea = %c", MDebugSelectEx_SonicAction_AreaLetters[MDebugSelectEx_SonicActionSettings->startArea]);
	nnPrint(baseX, posY++, "IsEvil = %d", MDebugSelectEx_SonicActionSettings->isEvil);
	nnPrint(baseX, posY++, "Mode = %s", MDebugSelectEx_GetSettingName(SAS_SpecialMode, MDebugSelectEx_SonicActionSettings->specialMode));
	nnPrint(baseTitleX + 2, ++posY, "%s", stgName);

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