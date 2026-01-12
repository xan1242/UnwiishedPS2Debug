#include "FPSDisplay.h"
#include "bWare.h"
#include "nnPrint.h"

unsigned int FrameTimingStartTime;
unsigned int FrameTimingEndTime;
unsigned int* FrameCounter = (unsigned int*)0x87459C;
unsigned int last_realtime_frames;
float PreviousCpuFrameTime;
float PreviousCpuFrameRate;

unsigned int FPSDisplay_Mode = FPSDISPLAY_MODE_FLAG_ONOFF | FPSDISPLAY_MODE_FLAG_FRAMETIME | FPSDISPLAY_MODE_FLAG_FRAMERATE;


void FPSDisplay_StartFrame()
{
	FrameTimingStartTime = FrameTimingEndTime;
}

void FPSDisplay_EndFrame()
{
	FrameTimingEndTime = bGetTicker();
}

void FPSDisplay_SetMode(unsigned int mode)
{
	FPSDisplay_Mode = mode;
}

unsigned int FPSDisplay_GetMode()
{
	return FPSDisplay_Mode;
}


void FPSDisplay_Draw()
{
	float cpuFT;

	if (!(FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_ONOFF))
		return;

	if ((FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_CONSTANT) || ((*FrameCounter & 8) != (last_realtime_frames & 8)))
	{
		last_realtime_frames = *FrameCounter;

		cpuFT = bGetTickerDifference(FrameTimingStartTime, FrameTimingEndTime);

		if (cpuFT < 0.1f)
			cpuFT = 0.1f;

		PreviousCpuFrameTime = cpuFT;
		if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMERATE)
			PreviousCpuFrameRate = 1000.0f / cpuFT;
	}

	int Ypos = 0;

	nnSetPrintColor(0xFFFFFFFF);
	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMETIME)
	{
		nnPrint(0, Ypos, "C: %.2f ms", PreviousCpuFrameTime);
		Ypos++;
	}

	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMERATE)
	{
		nnPrint(0, Ypos, "F: %.2f fps", PreviousCpuFrameRate);
		Ypos++;
	}
}
