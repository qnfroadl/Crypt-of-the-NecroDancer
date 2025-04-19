#include "BeatManager.h"
#include "SoundManager.h"
#include "KeyManager.h"
#include "BeatMarkerManager.h"

#include <fstream>

void BeatManager::Init()
{
	checkInputTime = false;

	markerManager = new BeatMarkerManager();
}

void BeatManager::Release()
{
	if (markerManager)
	{
		markerManager->Release();
		delete markerManager;
		markerManager = nullptr;
	}

	ReleaseInstance();
}

void BeatManager::Update()
{
	// IsHit 함수는 InputManager에서 사용합니다
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE)) // inputManager에서 처리
	{
		if (IsHit())
		{
			SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL); // hit일 때 수행할 코드
		}
	}

	UpdateBeat();

	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	// 마커 업데이트
	if (markerManager)
	{
		markerManager->Update(curPosition, checkOnBeat);
	}
}

void BeatManager::Render(HDC hdc)
{
	if (checkInputTime)
	{
		if (markerManager)
		{
			markerManager->Render(hdc);
		}

	}
}

void BeatManager::StartBeat(bool _checkBeat)
{
	queue<unsigned int> beatQueue;
	
	string beatFileStr = bgmPath[SoundManager::GetInstance()->GetCurrentKeyBgm()] + ".txt";
	ifstream beatFile{ beatFileStr };

	int beatBefore{};
	if (beatFile)
	{
		int beat{};
		while (!beatFile.eof())
		{
			beatFile >> beat;
			beatQueue.push(beat);

		}
		beatDatas = beatQueue;
	}

	beatBefore = 0;
	checkInputTime = _checkBeat;
	checkOnBeat = false;

	if (markerManager)
	{
		markerManager->Init(beatQueue);
	}
}

void BeatManager::UpdateBeat()
{
	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	if (beatDatas.size() > 0)
	{
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;

		// 정박
		if (!checkOnBeat)
		{
			if (abs((int)curPosition - (int)beat) <= 30.f)
			{
				checkOnBeat = true;
			}
		}

		if (curPosition > beat + 30.f)
		{
			checkOnBeat = false;
		}

		// 반 박자보다 더 느림
		if (curPosition > beat + beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
			checkOnBeat = false;
		}
	}
}

bool BeatManager::IsHit()
{
	if (!checkInputTime) return true; // 로비에서는 박자 체크 안하고 키 누르는 대로 움직일 수 있음

	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	if (beatDatas.size() > 0)
	{
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;
		// 반 박자 빠르거나 느리게까지 성공
		if (abs((int)curPosition - (int)beat) <= beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
			checkOnBeat = false;
			return true;
		}
	}
	return false;
}
