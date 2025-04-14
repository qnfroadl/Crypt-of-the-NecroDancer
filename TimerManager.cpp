#include "TimerManager.h"

#include "Timer.h"

void TimerManager::Init()
{
	timer = new Timer();
	timer->Init();
}

void TimerManager::Update()
{
	if (timer)
	{
		timer->Tick();

		elapsedTime += timer->GetDeltaTime();
	}
}

void TimerManager::Render(HDC hdc)
{
	wsprintf(szText, L"FPS: %d", timer->GetFPS());
	TextOut(hdc, WINSIZE_X - 100, 10, szText, wcslen(szText));
}

void TimerManager::Release()
{
	Singleton::ReleaseInstance();
}

unsigned long TimerManager::GetFPS()
{
	return timer->GetFPS();
}

float TimerManager::GetDeltaTime()
{
	return timer->GetDeltaTime();
}
