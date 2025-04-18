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

		// ��Ŀ ������Ʈ
		for (auto marker : markers)
		{
			beatInterval = max(1, beatInterval);
			if(marker) marker->Update((float)(beat - curPosition) / (float)beatInterval);
		}

		// ����
		if (!checkOnBeat)
		{
			if (abs((int)curPosition - (int)beat) <= 20.f)
			{
				checkOnBeat = true;

				// ��Ŀ Init
				bool red = false;
				if (totalBeats - beatDatas.size() > totalBeats * 0.9f) red = true;
				if (totalBeats - beatDatas.size() < totalBeats)
				{
					markers[totalBeats - beatDatas.size()]->Init(red);
				}
			}
		}

		// �� ���ں��� �� ����
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

	// totalBeatData ����ŭ ��Ŀ ����
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
	if (!checkInputTime) return true; // �κ񿡼��� ���� üũ ���ϰ� Ű ������ ��� ������ �� ����

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
			checkOnBeat = false;
			return true;
		}
	}
	return false;
}
