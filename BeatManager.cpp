#include "BeatManager.h"
#include "SoundManager.h"
#include "KeyManager.h"
#include "UIBeatMarker.h"

#include <fstream>

void BeatManager::Init()
{
	checkInputTime = false;
}

void BeatManager::Release()
{
	for (auto marker : markers)
	{
		marker->Release();
		delete marker;
		marker = nullptr;
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

	if (beatDatas.size() > 0)
	{
		unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;

		// 마커 업데이트
		for (auto marker : markers)
		{
			beatInterval = max(1, beatInterval);
			if(marker) marker->Update((float)(beat - curPosition) / (float)beatInterval);
		}

		// 정박
		if (!checkOnBeat)
		{
			if (abs((int)curPosition - (int)beat) <= 20.f)
			{
				checkOnBeat = true;

				// 마커 Init
				bool red = false;
				if (totalBeats - beatDatas.size() > totalBeats * 0.9f) red = true;
				if (totalBeats - beatDatas.size() < totalBeats)
				{
					markers[totalBeats - beatDatas.size()]->Init(red);
				}
			}
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

void BeatManager::Render(HDC hdc)
{
	if (checkInputTime)
	{
		for (auto marker : markers)
		{
			marker->Render(hdc);
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

	totalBeats = beatDatas.size();

	// totalBeatData 수만큼 마커 생성
	int curSize = markers.size();
	if (curSize < totalBeats)
	{
		for (int i = 0; i < totalBeats - curSize; ++i)
		{
			markers.push_back(new UIBeatMarker());
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
