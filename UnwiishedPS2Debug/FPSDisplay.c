#include "FPSDisplay.h"
#include "bWare.h"
#include "nnPrint.h"
#include "UnwiishedPS2Debug.h"

unsigned int FrameTimingStartTime = 0;
unsigned int FrameTimingEndTime = 0;

unsigned int RenderTimingStart = 0;
unsigned int RenderTimingEnd = 0;

unsigned int* FrameCounter = (unsigned int*)0x87459C;
unsigned int last_realtime_frames = 0;
float PreviousCpuFrameTime = 0.0f;
float PreviousCpuFrameRate = 0.0f;
float PreviousGpuFrameTime = 0.0f;
float PreviousGpuFrameRate = 0.0f;

unsigned int FPSDisplay_Mode = FPSDISPLAY_MODE_FLAG_ONOFF | FPSDISPLAY_MODE_FLAG_FRAMETIME | FPSDISPLAY_MODE_FLAG_FRAMERATE;

// #TODO: reduce jitter by sampling and calculating averages!

void FPSDisplay_StartFrame()
{
	FrameTimingStartTime = FrameTimingEndTime;
}

void FPSDisplay_EndFrame()
{
	FrameTimingEndTime = bGetTicker();
}

void FPSDisplay_StartDrawTask()
{
	RenderTimingStart = bGetTicker();
}

void FPSDisplay_EndDrawTask()
{
	RenderTimingEnd = bGetTicker();
}

void FPSDisplay_ProfileDrawTask(void* obj, void(*f)(void*))
{
	FPSDisplay_StartDrawTask();
	f(obj);
	FPSDisplay_EndDrawTask();
}

void FPSDisplay_ClearDrawTask()
{
	RenderTimingStart = 0;
	RenderTimingEnd = 0;
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
	float renderFT = 0.0f;
	float renderFR = 0.0f;
	float cpuFT = 0.0f;
	//float cpuFR = 0.0f;
	float dispFR = 0.0f;

	if (!(FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_ONOFF))
		return;

	if ((FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_CONSTANT) || ((*FrameCounter & 8) != (last_realtime_frames & 8)))
	{
		last_realtime_frames = *FrameCounter;

		renderFT = bGetTickerDifference(RenderTimingStart, RenderTimingEnd);
		cpuFT = bGetTickerDifference(FrameTimingStartTime, FrameTimingEndTime);

		//if (renderFT != 0.0f)
		//	PreviousGpuFrameTime = renderFT;
		if (cpuFT < 0.1f)
			cpuFT = 0.1f;
		if (renderFT > 0.1f)
			renderFR = 1000.0f / renderFT;

		PreviousCpuFrameTime = cpuFT;
		PreviousGpuFrameTime = renderFT;
		if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMERATE)
		{
			PreviousCpuFrameRate = 1000.0f / cpuFT;
			PreviousGpuFrameRate = renderFR;
		}
	}

	int Ypos = 0;

	nnSetPrintColor(0xFFFFFFFF);
	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMETIME)
	{
		nnPrint(0, Ypos, "C: %.2f ms", PreviousCpuFrameTime);
		if (PreviousGpuFrameTime)
			nnPrint(13, Ypos, "G: %.2fms", PreviousGpuFrameTime);
		Ypos++;
	}



	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMERATE)
	{
		if ((PreviousCpuFrameRate < PreviousGpuFrameRate) && PreviousGpuFrameTime)
			dispFR = PreviousCpuFrameRate;
		else
			dispFR = PreviousGpuFrameRate;

		nnPrint(0, Ypos, "F: %.2f fps", dispFR);
		Ypos++;
	}
}
