#include <stdint.h>
#include <string.h>
#include "UnwiishedPS2Debug.h"
#include "includes/minjector.h"
#include "nnRenderStuff.h"

void(*_nvLoadSVRMemoryTexture)(void* svrObj, const void* inTex, void* params, int unk1, int unk2, int unk3) = (void(*)(void*, const void*, void*, int, int, int))(0);
void(*_nvReleaseSVRTexture)(void* svrObj) = (void(*)(void*))(0);
void*(*_PXSetupCB)(void* buf, size_t size, int unk2) = (void*(*)(void*, size_t, int))(0);
void(*_PXReferCB)(void* buf) = (void(*)(void*))(0);
void*(*_PXBegin)(void* buf) = (void*(*)(void*))(0);
void(*_PXEnd)(void* buf) = (void(*)(void*))(0);
void(*_PXSetCB)(void* buf, int unk1, int unk2, uint64_t unk3) = (void(*)(void*, int, int, uint64_t))(0);
void(*_PXPutCB)(void* buf) = (void(*)(void*))(0);

void nvLoadSVRMemoryTexture(void* svrObj, const void* inTex, void* params, int unk1, int unk2, int unk3)
{
	if (_nvLoadSVRMemoryTexture)
		return _nvLoadSVRMemoryTexture(svrObj, inTex, params, unk1, unk2, unk3);
}

void nvReleaseSVRTexture(void* svrObj)
{
	if (_nvReleaseSVRTexture)
		return _nvReleaseSVRTexture(svrObj);
}

void* PXSetupCB(void* buf, size_t size, int unk2)
{
	if (_PXSetupCB)
		return _PXSetupCB(buf, size, unk2);
	return NULL;
}

void PXReferCB(void* buf)
{
	if (_PXReferCB)
		return _PXReferCB(buf);
}

void PXSetCB(void* buf, int unk1, int unk2, uint64_t unk3)
{
	if (_PXSetCB)
		return _PXSetCB(buf, unk1, unk2, unk3);
}

void PXPutCB(void* buf)
{
	if (_PXPutCB)
		return _PXPutCB(buf);
}

void* PXBegin(void* buf)
{
	if (_PXBegin)
		return _PXBegin(buf);
	return NULL;
}

void PXEnd(void* buf)
{
	if (_PXEnd)
		return _PXEnd(buf);
}

void nnRenderStuff_Init()
{
	uintptr_t loc_500044 = 0x500044;
	_PXSetupCB = (void*(*)(void*, size_t, int))(minj_GetBranchDestination(loc_500044));

	uintptr_t loc_500060 = 0x500060;
	_PXSetCB = (void(*)(void*, int, int, uint64_t))(minj_GetBranchDestination(loc_500060));

	uintptr_t loc_52B308 = 0x52B308;
	_PXPutCB = (void(*)(void*))(minj_GetBranchDestination(loc_52B308));

	uintptr_t loc_4FE960 = 0x4FE960;
	_PXReferCB = (void(*)(void*))(minj_GetBranchDestination(loc_4FE960));

	uintptr_t loc_527D74 = 0x527D74;
	_PXBegin = (void*(*)(void*))(minj_GetBranchDestination(loc_527D74));

	uintptr_t loc_527EA8 = 0x527EA8;
	_PXEnd = (void(*)(void*))(minj_GetBranchDestination(loc_527EA8));

	uintptr_t loc_51EB98 = 0x51EB98;
	_nvLoadSVRMemoryTexture = (void(*)(void*, const void*, void*, int, int, int))(minj_GetBranchDestination(loc_51EB98));

	uintptr_t loc_51EC9C = 0x51EC9C;
	_nvReleaseSVRTexture = (void(*)(void*))(minj_GetBranchDestination(loc_51EC9C));
}