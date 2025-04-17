#include "BeatManager.h"
#include "SoundManager.h"
#include "KeyManager.h"

#include <fstream>

void BeatManager::Init()
{
	start = false;
}

void BeatManager::Release()
{
	ReleaseInstance();
}

void BeatManager::Update()
{
	if (!start) return;
	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();

	if (beatDatas.size() > 0)
	{
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;
		// 플레이어 입력 반 박자 빠르거나 느리게까지 성공
		if (abs((int)curPosition - (int)beat) <= beatInterval / 2.f)
		{
			if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
			{
				SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL);
				beatDatas.pop();
				beatBefore = beat;
			}
		}
		// 반 박자보다 더 느림
		else if (curPosition > beat + beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
		}
	}
	else
	{
		start = false;
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
	start = true;
}
