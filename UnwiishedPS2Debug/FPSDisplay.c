#include "FPSDisplay.h"
#include "bWare.h"
#include "nnPrint.h"
#include "UnwiishedPS2Debug.h"
#include "includes/minjector.h"

unsigned int FrameTimingStartTime = 0;
unsigned int FrameTimingEndTime = 0;

unsigned int RenderTimingStart = 0;
unsigned int RenderTimingEnd = 0;

unsigned int LastVBlankTime1 = 0;
unsigned int LastVBlankTime2 = 0;

float PreviousVSyncTime = 0.0f;
float PreviousVSyncRate = 0.0f;

//unsigned int* FrameCounter = (unsigned int*)0x87459C;
unsigned int* FrameCounter = (unsigned int*)0x873C50;
unsigned int last_realtime_frames = 0;
float PreviousCpuFrameTime = 0.0f;
float PreviousCpuFrameRate = 0.0f;
float PreviousGpuFrameTime = 0.0f;
float PreviousGpuFrameRate = 0.0f;

unsigned int FPSDisplay_Mode = FPSDISPLAY_MODE_FLAG_ONOFF | FPSDISPLAY_MODE_FLAG_FRAMETIME | FPSDISPLAY_MODE_FLAG_FRAMERATE;

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

void FPSDisplay_MeasureVSyncTime()
{
	LastVBlankTime1 = LastVBlankTime2;
	LastVBlankTime2 = bGetTicker();
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

float FPSDisplay_GetFPS()
{
	// get minimum FPS
	float smallest = PreviousCpuFrameRate;
	if (PreviousVSyncRate < smallest)
		smallest = PreviousVSyncRate;
	if ((PreviousGpuFrameRate < smallest) && PreviousGpuFrameTime)
		smallest = PreviousGpuFrameRate;

	return smallest;
}

// TODO: values may need a sample buffer to reduce jitter and show the real values properly...

void FPSDisplay_Draw()
{
	float vsyncFT = 0.0f;
	//float vsyncFR = 0.0f;
	float renderFT = 0.0f;
	float renderFR = 0.0f;
	float cpuFT = 0.0f;
	//float cpuFR = 0.0f;
	//float dispFR = 0.0f;

	if (!(FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_ONOFF))
		return;

	if ((FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_CONSTANT) || ((*FrameCounter & 8) != (last_realtime_frames & 8)))
	{
		last_realtime_frames = *FrameCounter;

		renderFT = bGetTickerDifference(RenderTimingStart, RenderTimingEnd);
		cpuFT = bGetTickerDifference(FrameTimingStartTime, FrameTimingEndTime);
		vsyncFT = bGetTickerDifference(LastVBlankTime1, LastVBlankTime2);

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
		if (vsyncFT)
		{
			PreviousVSyncTime = vsyncFT;
			PreviousVSyncRate = 1000.0f / vsyncFT;
		}
	}

	int Ypos = 0;
	int Xpos = 0;

	nnSetPrintColor(0xFFFFFFFF);
	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMETIME)
	{
		nnPrint(Xpos, Ypos, "C: %.2fms", PreviousCpuFrameTime);
		Xpos += 11;
		nnPrint(Xpos, Ypos, "V: %.2fms", PreviousVSyncTime);
		Xpos += 11;
		if (PreviousGpuFrameTime)
			nnPrint(Xpos, Ypos, "G: %.2fms", PreviousGpuFrameTime);
		Ypos++;
	}

	if (FPSDisplay_Mode & FPSDISPLAY_MODE_FLAG_FRAMERATE)
	{
		nnPrint(0, Ypos, "F: %.2f fps", FPSDisplay_GetFPS());
		Ypos++;
	}
}
