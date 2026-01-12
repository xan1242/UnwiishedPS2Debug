#include "stdbool.h"

#ifndef FPSDISPLAY_H
#define FPSDISPLAY_H

void FPSDisplay_StartFrame();
void FPSDisplay_EndFrame();
void FPSDisplay_Draw();
void FPSDisplay_SetMode(unsigned int mode);
unsigned int FPSDisplay_GetMode();

#define FPSDISPLAY_MODE_FLAG_ONOFF (1 << 1)
#define FPSDISPLAY_MODE_FLAG_CONSTANT (1 << 2)
#define FPSDISPLAY_MODE_FLAG_FRAMETIME (1 << 3)
#define FPSDISPLAY_MODE_FLAG_FRAMERATE (1 << 4)

#endif