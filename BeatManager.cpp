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
	// IsHit �Լ��� InputManager���� ����մϴ�
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE)) // inputManager���� ó��
	{
		if (IsHit())
		{
			SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL); // hit�� �� ������ �ڵ�
		}
	}

	if (beatDatas.size() > 0)
	{
		unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;
		
		// �� ���ں��� �� ����
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
	if (!checkBeat) return true; // �κ񿡼��� ���� üũ ���ϰ� Ű ������ ��� ������ �� ����

	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	if (beatDatas.size() > 0)
	{
		unsigned int beat = beatDatas.front();
		unsigned int beatInterval = beat - beatBefore;
		// �� ���� �����ų� �����Ա��� ����
		if (abs((int)curPosition - (int)beat) <= beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
			return true;
		}
	}
	return false;
}
