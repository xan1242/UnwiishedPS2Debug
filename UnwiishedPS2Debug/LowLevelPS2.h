//
// Low level PS2 stuff goes here...
//

#ifndef LOWLEVELPS2_H
#define LOWLEVELPS2_H

/** Modes for FlushCache */
#define WRITEBACK_DCACHE  0
#define INVALIDATE_DCACHE 1
#define INVALIDATE_ICACHE 2
#define INVALIDATE_CACHE  3 // Invalidate both data & instruction caches.

void FlushCache(int op);
//int RemoveIntcHandler(int cause, int handler_id);
//int AddIntcHandler(int cause, void* handler_func, int next);
//int DisableIntc(int cause);
//int EnableIntc(int cause);

void LowLevelPS2_Init();

#endif