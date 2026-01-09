//#include <stdio.h>
#include "includes/pcsx2f_api.h"
#include "includes/minjector.h"
#include "gprintf.h"
#include "UnwiishedPS2Debug.h"

int CompatibleCRCList[] = { CRC_SLES55380 };

void(*sub_E34A0)(void* obj) = (void(*)(void*))(0);
void(*sub_E3720)(void* obj) = (void(*)(void*))(0);

void early_init()
{
    // get the game's printf function address for debugging purposes
    gprintf_init();
}

void sub_E34A0_hook(void* that)
{
    UnwiishedPS2Debug_Init();
    sub_E34A0(that);
}

void sub_E3720_hook(void* that)
{
    UnwiishedPS2Debug_PostInit();
    sub_E3720(that);
}


void init()
{
    early_init();

    LOG("UnwiishedPS2Debug Plugin Init\n");

    uintptr_t loc_4FF6A0 = 0x4FF6A0;
    sub_E34A0 = (void(*)(void*))(minj_GetBranchDestination(loc_4FF6A0));
    minj_MakeCALL(loc_4FF6A0, (uintptr_t)&sub_E34A0_hook);

    uintptr_t loc_4FF758 = 0x4FF758;
    sub_E3720 = (void(*)(void*))(minj_GetBranchDestination(loc_4FF758));
    minj_MakeCALL(loc_4FF758, (uintptr_t)&sub_E3720_hook);
}

int main()
{
    return 0;
}