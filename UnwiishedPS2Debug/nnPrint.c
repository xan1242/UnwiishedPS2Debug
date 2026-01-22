#include <stdint.h>
#include <string.h>
#include "UnwiishedPS2Debug.h"
#include "includes/minjector.h"
#include "nnRenderStuff.h"
#include "nnPrint.h"
#include "nngPrintTexture.h"
#include "nnFlushPrint.h"
#include "LowLevelPS2.h"

uintptr_t p_nnsPrintNum;
uintptr_t p_nnsPrintMax;
uintptr_t p_nnsPrintBuf;
uintptr_t p_nnsPrintChar;
uintptr_t p_nnsPrintCharSize;
uintptr_t p_nnsPrintCol;
uintptr_t p_nngScreen;

uintptr_t p_qword_768D10;

uintptr_t loc_D0BA4;
uintptr_t loc_D0B5C;

void(*nnPrint_SetVideoMode_1)(void* obj, int mode) = (void(*)(void*, int))(0);
void(*nnPrint_SetVideoMode_2)(void* obj, int mode) = (void(*)(void*, int))(0);
void(*nnPrint_SetVideoMode_3)(void* obj, int mode) = (void(*)(void*, int))(0);
void(*nnPrint_uMainLoopFunc)(void* obj) = (void(*)(void*))(0);

void(*nnPrint)(int x, int y, const char* fmt, ...) = (void(*)(int, int, const char*, ...))(0);
void(*nnSetPrintColor)(unsigned int color) = (void(*)(unsigned int))(0);

#define NUM_ITEMS 1200

char PrintBuf[21 * NUM_ITEMS];
//uint8_t nnsPrintSvrObj[0x10];
//SvrObj nnsPrintSvrObj;
// the buffers NEED to be below the 32MB boundary! This plugin resides above it!
// we're using the cri middleware error message area here....
SvrObj* nnsPrintSvrObj = (SvrObj*)0x836CF0;
uint32_t* nnsPrintZ = (uint32_t*)0x870A6C; // we need an area that is reachable in the gp range!
Vec2 nnsPrintAspect;
Vec2 nnsPrint2d;
void* cb1;
void* cb2;


void nnSetPrintSize(float x, float y)
{
	*(float*)(p_nnsPrintCharSize) = x;
	*(float*)(p_nnsPrintCharSize + 4) = y;
}

size_t nnGetPrintBufferSize(int n)
{
	return 21 * n;
}


void nnInitPrint(char* buffer, int max, void* printTex)
{
	nnColor* col = (nnColor*)p_nnsPrintCol;
	uint16_t texParams[4];

	if (!*(int*)p_nnsPrintMax)
	{
		if (printTex)
		{
			nvLoadSVRMemoryTexture(nnsPrintSvrObj, printTex, NULL, 0, 0, 0);
		}
		else
		{
			texParams[0] = 0x6608;
			texParams[2] = 128;
			texParams[3] = 128;
			texParams[1] = 0;
			nvLoadSVRMemoryTexture(nnsPrintSvrObj, nngPrintTexture, texParams, 0, 0, 0);
		}
		nnsPrintSvrObj->unk4 &= 0xE7;
	}

	nnSetPrintSize(16.0f, 16.0f);
	*(uintptr_t*)(p_nnsPrintChar) = (uintptr_t)buffer + 20 * max;
	col->r = 96;
	col->g = 96;
	col->b = 96;
	col->a = 128;
	nnsPrintAspect.x = 16.0f;
	nnsPrintAspect.y = 16.0f;
	*(int*)p_nnsPrintMax = max;
	*(char**)p_nnsPrintBuf = buffer;

	float unk_screenval_1 = *(float*)(p_nngScreen + 8);
	float unk_screenval_2 = *(float*)(p_nngScreen + 0x1C);
	float unk_screenval_3 = *(float*)(p_nngScreen + 0xC);
	float unk_screenval_4 = *(float*)(p_nngScreen + 0x20);

	float smth1 = -unk_screenval_1 * unk_screenval_2;
	float smth2 = -unk_screenval_3 * unk_screenval_4;

	//LOG("smth1 (hex): 0x%x\n", *(uint32_t*)&smth1);
	//LOG("smth2 (hex): 0x%x\n", *(uint32_t*)&smth2);

	if (smth1 == -160.0f)
		nnsPrintAspect.x = 16.0f * 0.5f;
	if (smth1 == -256.0f)
		nnsPrintAspect.x = nnsPrintAspect.x * 0.8f;
	if (smth2 == -112.0f)
		nnsPrintAspect.y = nnsPrintAspect.y * 0.5f;
	if (smth2 == -128.0f)
		nnsPrintAspect.y = nnsPrintAspect.y * 0.5714286f;
	if (smth2 == -192.0f)
		nnsPrintAspect.y = nnsPrintAspect.y * 0.85714287f;
	if (smth2 == -256.0f)
		nnsPrintAspect.y = nnsPrintAspect.y * 1.1428572f;

	nnsPrint2d.x = 16.0f * (smth1 + 2048.0f);
	nnsPrint2d.y = 16.0f * (smth2 + 2048.0f);
	cb1 = PXSetupCB(nnsPrintSvrObj->cb_buf1, 0x90, 8);
	//LOG("PXSetupCB cb1 = 0x%x\n", cb1);
	PXSetCB(cb1, 0, 8, 5);
	PXSetCB(cb1, 1, 71, 0x5000D);
	PXSetCB(cb1, 2, 78, *(uint64_t*)p_qword_768D10);
	PXSetCB(cb1, 3, 66, 68);
	PXSetCB(cb1, 4, 59, 0x8000000000LL);
	PXSetCB(cb1, 5, 20, nnsPrintSvrObj->unk2);
	PXSetCB(cb1, 6, 6, *(uint64_t*)&nnsPrintSvrObj->unk3);
	PXSetCB(cb1, 7, 27, 86);

	cb2 = PXSetupCB(nnsPrintSvrObj->cb_buf2, 0x30, 2);
	//LOG("PXSetupCB cb2 = 0x%x\n", cb2);
}

void nnExitPrint()
{
	nvReleaseSVRTexture(nnsPrintSvrObj);
	*(int*)p_nnsPrintMax = 0;
}

#ifndef __INTELLISENSE__
__attribute__((noinline)) void nnPrint_hkSetVideoMode(void* obj, int mode, void(*fn)(void*, int))
{
	asm volatile("");
	nnExitPrint();
	fn(obj, mode);
	nnInitPrint(PrintBuf, NUM_ITEMS, NULL);
	nnSetPrintSize(10, 13);
}
#endif

void nnPrint_hkSetVideoMode_1(void* obj, int mode)
{
	return nnPrint_hkSetVideoMode(obj, mode, nnPrint_SetVideoMode_1);
}

void nnPrint_hkSetVideoMode_2(void* obj, int mode)
{
	return nnPrint_hkSetVideoMode(obj, mode, nnPrint_SetVideoMode_2);
}

void nnPrint_hkSetVideoMode_3(void* obj, int mode)
{
	return nnPrint_hkSetVideoMode(obj, mode, nnPrint_SetVideoMode_3);
}

void nnPrint_InitDebugPrint(int sizeX, int sizeY)
{
	nnInitPrint(PrintBuf, NUM_ITEMS, NULL);
	nnSetPrintSize((float)sizeX, (float)sizeY);
}

void nnPrint_PXReferCB()
{
	return PXReferCB(cb1);
}

void nnPrint_PXSetCB(void* buf, int unk1, int unk2, uint64_t unk3)
{
	return PXSetCB(cb2, unk1, unk2, unk3);
}

void nnPrint_PXPutCB()
{
	return PXPutCB(cb2);
}


uintptr_t nnPrint_DerefGP(uintptr_t loc)
{
	uintptr_t gp = minj_GetGP();
	return gp + *(int16_t*)loc;
}

void nnPrint_MainLoopHook(void* obj)
{
	nnPrint_uMainLoopFunc(obj);

	//nnSetPrintColor(0xFFFFFFFF);
	//nnPrint(1, 1, "THIS IS A TEST BLABLABLA");

	nnFlushPrint();
}

void nnPrint_PatchFlushPrint()
{
	unsigned char* _nnFlushPrint = (unsigned char*)(&nnFlushPrint);

	// fix up nnFlushPrint for Unleashed (was taken from Riders ZG)
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[8], (uintptr_t)&nnPrint_PXReferCB);
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[0x10], (uintptr_t)&PXBegin);
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[0x2B4], (uintptr_t)&PXEnd);
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[0x2D0], (uintptr_t)&nnPrint_PXSetCB);
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[0x2FC], (uintptr_t)&nnPrint_PXSetCB);
	minj_MakeCALL((uintptr_t)&_nnFlushPrint[0x304], (uintptr_t)&nnPrint_PXPutCB);
	// patch up GP ptrs...
	// nnsPrintNum
	*(int16_t*)(&_nnFlushPrint[0x18]) = -0x4084;
	*(int16_t*)(&_nnFlushPrint[0x274]) = -0x4084;
	*(int16_t*)(&_nnFlushPrint[0x2D4]) = -0x4084;
	// nnsPrintChar
	*(int16_t*)(&_nnFlushPrint[0x50]) = -0x4098;
	// nnsPrintBuf
	*(int16_t*)(&_nnFlushPrint[0x54]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0xB4]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0xC4]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0xD4]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0xE4]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0x198]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0x1A8]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0x1B8]) = -0x409C;
	*(int16_t*)(&_nnFlushPrint[0x1C8]) = -0x409C;
	// nngGsZbuf
	*(uint16_t*)(&_nnFlushPrint[0x2D8]) = 0x77;
	*(uint16_t*)(&_nnFlushPrint[0x2DC]) = 0x77;
	*(uint16_t*)(&_nnFlushPrint[0x2E0]) = 0x8D04;
	*(uint16_t*)(&_nnFlushPrint[0x2E4]) = 0x8D00;
	// nngGsTest
	*(uint16_t*)(&_nnFlushPrint[0x2BC]) = 0x77;
	*(uint16_t*)(&_nnFlushPrint[0x2C4]) = 0x8D28;
	// nnsPrintZ
	*(int16_t*)(&_nnFlushPrint[0x178]) = -0x7584;
	*(int16_t*)(&_nnFlushPrint[0x260]) = -0x7584;

	// nnsPrint2d & nnsPrintAspect
	// calculate the halves correctly
	uintptr_t p_nnsPrintAspect = (uintptr_t)&nnsPrintAspect;
	uint16_t p_nnsPrintAspect_Upper = (p_nnsPrintAspect >> 16) & 0xFFFF;
	uint16_t p_nnsPrintAspect_Lower = (p_nnsPrintAspect) & 0xFFFF;
	if (p_nnsPrintAspect_Lower > 0x7FFF)
		p_nnsPrintAspect_Upper += 1;

	// write lui t2, p_nnsPrintAspect_Upper now... (we'll assume the same for nnsPrint2d)
	*(uint32_t*)(&_nnFlushPrint[0x9C]) = 0x3C0A0000 | p_nnsPrintAspect_Upper;
	// lwc f01, p_nnsPrintAspect_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0xF8]) = 0xC5410000 | p_nnsPrintAspect_Lower;
	// lwc f02, p_nnsPrintAspect_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0x1DC]) = 0xC5420000 | p_nnsPrintAspect_Lower;
	// y value
	uint16_t p_nnsPrintAspect_Y_Lower = ((uintptr_t) & (nnsPrintAspect.y)) & 0xFFFF;
	// lwc f01, p_nnsPrintAspect_Y_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0x138]) = 0xC5410000 | p_nnsPrintAspect_Y_Lower;
	// lwc f02, p_nnsPrintAspect_Y_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0x220]) = 0xC5420000 | p_nnsPrintAspect_Y_Lower;

	uintptr_t p_nnsPrint2d = (uintptr_t)&nnsPrint2d;
	uint16_t p_nnsPrint2d_Lower = (p_nnsPrint2d) & 0xFFFF;
	// lwc f00, p_nnsPrint2d_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0xF4]) = 0xC5400000 | p_nnsPrint2d_Lower;
	*(uint32_t*)(&_nnFlushPrint[0x1D8]) = 0xC5400000 | p_nnsPrint2d_Lower;
	// y value
	uint16_t p_nnsPrint2d_Y_Lower = ((uintptr_t)&nnsPrint2d.y) & 0xFFFF;
	// lwc f00, p_nnsPrintAspect_Y_Lower(t2)
	*(uint32_t*)(&_nnFlushPrint[0x134]) = 0xC5400000 | p_nnsPrint2d_Y_Lower;
	*(uint32_t*)(&_nnFlushPrint[0x21C]) = 0xC5400000 | p_nnsPrint2d_Y_Lower;
}

void nnPrint_Init()
{
	uintptr_t loc_5333D4 = 0x5333D4;
	p_nnsPrintNum = nnPrint_DerefGP(loc_5333D4);

	uintptr_t loc_5333D8 = 0x5333D8;
	p_nnsPrintMax = nnPrint_DerefGP(loc_5333D8);

	uintptr_t loc_5333F8 = 0x5333F8;
	p_nnsPrintBuf = nnPrint_DerefGP(loc_5333F8);

	uintptr_t loc_5334F4 = 0x5334F4;
	p_nnsPrintChar = nnPrint_DerefGP(loc_5334F4);

	uintptr_t loc_533404 = 0x533404;
	p_nnsPrintCharSize = nnPrint_DerefGP(loc_533404);

	uintptr_t loc_53347C = 0x53347C;
	p_nnsPrintCol = nnPrint_DerefGP(loc_53347C);

	uintptr_t loc_521830 = 0x521830;
	uintptr_t loc_52186C = 0x52186C;
	p_nngScreen = minj_GetPtr(loc_521830, loc_52186C);

	uintptr_t loc_521998 = 0x521998;
	p_qword_768D10 = minj_GetPtr(loc_521998, loc_521998 + 4);

	loc_D0BA4 = 0xD0BA4;
	loc_D0B5C = 0xD0B5C;

	uintptr_t loc_4FF7B0 = 0x4FF7B0;
	nnPrint_uMainLoopFunc = (void(*)(void*))(minj_GetBranchDestination(loc_4FF7B0));
	minj_MakeCALL(loc_4FF7B0, (uintptr_t)&nnPrint_MainLoopHook);

	uintptr_t loc_50132C = 0x50132C;
	nnPrint_SetVideoMode_1 = (void(*)(void*, int))(minj_GetBranchDestination(loc_50132C));
	minj_MakeCALL(loc_50132C, (uintptr_t)&nnPrint_hkSetVideoMode_1);

	uintptr_t loc_5013AC = 0x5013AC;
	nnPrint_SetVideoMode_2 = (void(*)(void*, int))(minj_GetBranchDestination(loc_5013AC));
	minj_MakeCALL(loc_5013AC, (uintptr_t)&nnPrint_hkSetVideoMode_2);

	uintptr_t loc_501430 = 0x501430;
	nnPrint_SetVideoMode_3 = (void(*)(void*, int))(minj_GetBranchDestination(loc_501430));
	minj_MakeCALL(loc_501430, (uintptr_t)&nnPrint_hkSetVideoMode_3);

	uintptr_t loc_25D270 = 0x25D270;
	nnPrint = (void(*)(int, int, const char*, ...))(minj_GetBranchDestination(loc_25D270));

	uintptr_t loc_25D25C = 0x25D25C;
	nnSetPrintColor = (void(*)(unsigned int))(minj_GetBranchDestination(loc_25D25C));

	nnPrint_PatchFlushPrint();

	memset(nnsPrintSvrObj, 0, sizeof(SvrObj));
	*nnsPrintZ = 0xFFFFFF0;

	// bind squirrel nnPrint
	uintptr_t loc_308DE0 = 0x308DE0;
	minj_MakeJMP(loc_308DE0, (uintptr_t)nnPrint);

	FlushCache(INVALIDATE_CACHE);

	//LOG("cb_buf1 = 0x%x\n", &nnsPrintSvrObj->cb_buf1[0]);
	//LOG("cb_buf2 = 0x%x\n", &nnsPrintSvrObj->cb_buf2[0]);
	//LOG("nnsPrintAspect = 0x%x\n", &nnsPrintAspect);
	//LOG("nnsPrint2d = 0x%x\n", &nnsPrint2d);
}