#ifndef NNPRINT_H
#define NNPRINT_H

#include <stdint.h>

void nnPrint_Init();
void nnPrint_InitDebugPrint(int sizeX, int sizeY);

extern void(*nnSetPrintColor)(unsigned int color);
extern void(*nnPrint)(int x, int y, const char* fmt, ...);

#endif
