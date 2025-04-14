#pragma once

#include "config.h"
#pragma comment(lib, "Winmm.lib")

class Timer
{

private:
	bool bHardWare;				// 고성능 타이머를 사용할 수 있는지 여부?
	float timeScale;			// 경과 진동수를 초당 진동수로 나눠서 진동수를 시간으로 환산하기 위한 값
	
	float timeElapsed;			// 시간 차이
	__int64 curTime;			// 현재 시간.
	__int64 lastTime;			// 이전 시간.
	__int64 periodFrequency;	// 고성능 타이머 주파수 (초당 진동수)

	float fpsTimeElapsed;		// 프레임 경과 시간.
	unsigned long fpsFrameCount;// 초당 프레임.
	unsigned long FPS;			// ?

public:
	void Init();
	void Tick();	// update처럼 매 프레임 호출 해줘야함.
	float GetCurTime();

	unsigned long GetFPS();
	float GetDeltaTime();
};

