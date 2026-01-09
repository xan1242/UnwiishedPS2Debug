#ifndef NNRENDERSTUFF_H
#define NNRENDERSTUFF_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
	uint8_t unk1[0x10];
	uint64_t unk2;
	uint8_t unk3[4];
	uint8_t unk4;
	uint8_t pad1[3];
	uint8_t unk5[0x10];
	uint8_t cb_buf1[0xC0];
	uint8_t cb_buf2[0x30];
} SvrObj;

typedef struct
{
	float x;
	float y;
} Vec2;

typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} nnColor;

void nnRenderStuff_Init();
void nvLoadSVRMemoryTexture(void* svrObj, const void* inTex, void* params, int unk1, int unk2, int unk3);
void nvReleaseSVRTexture(void* svrObj);
void* PXSetupCB(void* buf, size_t size, int unk2);
void PXSetCB(void* buf, int unk1, int unk2, uint64_t unk3);
void PXPutCB(void* buf);
void PXReferCB(void* buf);
void* PXBegin(void* buf);
void PXEnd(void* buf);

#endif
