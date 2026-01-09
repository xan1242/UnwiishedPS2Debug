#include "gprintf.h"
#include "includes/minjector.h"

int(*gprintf)(const char*, ...) = (int(*)(const char*, ...))(0);

#if (defined __DEBUG) || (defined __ENABLELOG)
static uintptr_t loc_66FFDC = 0x66FFDC;
#endif

void gprintf_init()
{
#if (defined __DEBUG) || (defined __ENABLELOG)
	uintptr_t addr = minj_GetBranchDestination(loc_66FFDC);
	if (!addr)
		return;

	gprintf = (int(*)(const char*, ...))(addr);
#endif
}
