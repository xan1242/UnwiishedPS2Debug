#ifndef UNWIISHED_PS2_DEBUG_H
#define UNWIISHED_PS2_DEBUG_H

#include "gprintf.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if (defined __DEBUG) || (defined __ENABLELOG)
#define LOG(fmt, ...) gprintf("UnwiishedPS2Debug: " fmt, ##__VA_ARGS__)
#define LOG_NOHEADER(fmt, ...) gprintf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#define LOG_NOHEADER(fmt, ...)
#endif

// Definitions

#define CRC_SLES55380 0x8C913264

/** Modes for FlushCache */
#define WRITEBACK_DCACHE  0
#define INVALIDATE_DCACHE 1
#define INVALIDATE_ICACHE 2
#define INVALIDATE_CACHE  3 // Invalidate both data & instruction caches.

#define DISPLAYFPS_MODE_OFF 0
#define DISPLAYFPS_MODE_ON 1
#define DISPLAYFPS_MODE_ON_CONSTANT 2
#define DISPLAYFPS_MODE_COUNT 3

#define DISPLAYFPS_TYPE_ALL 0
#define DISPLAYFPS_TYPE_FT 1
#define DISPLAYFPS_TYPE_FPS 2
#define DISPLAYFPS_TYPE_COUNT 3

typedef struct
{
	void* unkptr1;
	size_t size1;
	size_t size2;
	size_t numAlloc;
	void* unkptr2;
	void* unkptr3;
	void* unkptr4;
	void* unkptr5;
} HeapAlloc;


// Functions
void UnwiishedPS2Debug_Init();
void UnwiishedPS2Debug_PostInit();
void FlushCache(int op);

extern void* (*UnwiishedPS2Debug_uGetModeManagerThingy)();
extern void(*UnwiishedPS2Debug_uSetNextGameModeThingy)(void* manager, int mode);
extern int(*UnwiishedPS2Debug_sprintf)(char* buf, const char* fmt, ...);
extern uintptr_t p_ModeUpdateTrigger;

#endif