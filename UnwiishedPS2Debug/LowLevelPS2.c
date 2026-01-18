//
// Low level PS2 stuff goes here...
//

#include "LowLevelPS2.h"
#include "includes/minjector.h"
#include <stdint.h>

void (*_FlushCache)(int op) = (void(*)(int))(0);
void FlushCache(int op)
{
	if (_FlushCache)
		return _FlushCache(op);
}

//int(*_RemoveIntcHandler)(int cause, int handler_id) = (int(*)(int, int))(0);
//int RemoveIntcHandler(int cause, int handler_id)
//{
//	if (_RemoveIntcHandler)
//		return _RemoveIntcHandler(cause, handler_id);
//	return -1;
//}
//
//int(*_AddIntcHandler)(int cause, void* handler_func, int next) = (int(*)(int, void*, int))(0);
//int AddIntcHandler(int cause, void* handler_func, int next)
//{
//	if (_AddIntcHandler)
//		return _AddIntcHandler(cause, handler_func, next);
//	return -1;
//}
//
//int(*_DisableIntc)(int cause) = (int(*)(int))(0);
//int DisableIntc(int cause)
//{
//	if (_DisableIntc)
//		return _DisableIntc(cause);
//	return -1;
//}
//
//int(*_EnableIntc)(int cause) = (int(*)(int))(0);
//int EnableIntc(int cause)
//{
//	if (_EnableIntc)
//		return _EnableIntc(cause);
//	return -1;
//}

void LowLevelPS2_Init()
{
	uintptr_t loc_D01F0 = 0xD01F0;
	_FlushCache = (void(*)(int))(minj_GetBranchDestination(loc_D01F0));

	//uintptr_t loc_671DB8 = 0x671DB8;
	//_RemoveIntcHandler = (int(*)(int, int))(minj_GetBranchDestination(loc_671DB8));
	//
	//uintptr_t loc_672CA0 = 0x672CA0;
	//_AddIntcHandler = (int(*)(int, void*, int))(minj_GetBranchDestination(loc_672CA0));
	//
	//uintptr_t loc_672C80 = 0x672C80;
	//_DisableIntc = (int(*)(int))(minj_GetBranchDestination(loc_672C80));
	//
	//uintptr_t loc_672CAC = 0x672CAC;
	//_EnableIntc = (int(*)(int))(minj_GetBranchDestination(loc_672CAC));
}