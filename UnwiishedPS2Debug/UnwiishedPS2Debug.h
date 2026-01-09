#ifndef UNWIISHED_PS2_DEBUG_H
#define UNWIISHED_PS2_DEBUG_H

#include "gprintf.h"
#include <stdbool.h>

#if (defined __DEBUG) || (defined __ENABLELOG)
#define LOG(fmt, ...) gprintf("UnwiishedPS2Debug: " fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

// Definitions

#define CRC_SLES55380 0x8C913264

/** Modes for FlushCache */
#define WRITEBACK_DCACHE  0
#define INVALIDATE_DCACHE 1
#define INVALIDATE_ICACHE 2
#define INVALIDATE_CACHE  3 // Invalidate both data & instruction caches.

// Functions
void UnwiishedPS2Debug_Init();
void UnwiishedPS2Debug_PostInit();
void FlushCache(int op);


#endif