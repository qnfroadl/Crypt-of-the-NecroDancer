#include "BeatManager.h"
#include "SoundManager.h"
#include "KeyManager.h"

#include <fstream>

void BeatManager::Init()
{
	tempoRate = 1.f;
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
		if (abs((int)curPosition - (int)beat) < 20.f / tempoRate)
		{
			SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL);
			beatDatas.pop();
		}
		if (curPosition > beat + 20.f / tempoRate)
		{
			beatDatas.pop();
		}
	}
}

void BeatManager::StartBeat()
{
	queue<int> beatQueue;
	
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

	start = true;
}
