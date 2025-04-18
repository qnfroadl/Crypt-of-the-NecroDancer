#include "BeatManager.h"
#include "SoundManager.h"
#include "KeyManager.h"

#include <fstream>

void BeatManager::Init()
{
	checkBeat = false;
}

void BeatManager::Release()
{
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
		
		// 반 박자보다 더 느림
		if (curPosition > beat + beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
		}
	}
	else
	{
		checkBeat = false;
	}
}

void BeatManager::StartBeat()
{
	queue<unsigned int> beatQueue;
	
	string beatFileStr = bgmPath[SoundManager::GetInstance()->GetCurrentKeyBgm()] + ".txt";
	ifstream beatFile{ beatFileStr };

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
	checkBeat = true;
}

bool BeatManager::IsHit()
{
	if (!checkBeat) return true; // 로비에서는 박자 체크 안하고 키 누르는 대로 움직일 수 있음

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
			return true;
		}
	}
	return false;
}
