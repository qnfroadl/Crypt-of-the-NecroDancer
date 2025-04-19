#include "BeatManager.h"

#include "SoundManager.h"
#include "KeyManager.h"
#include "EventManager.h"
#include "EventData.h"

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
	ProcessInput();

	UpdateBeat();

	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	// ��Ŀ ������Ʈ
	if (markerManager)
	{
		markerManager->Update(curPosition);
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
		int beat = beatDatas.front();
		int beatInterval = beat - beatBefore;

		// ����
		if (!checkOnBeat)
		{
			if (abs((int)curPosition - (int)beat) <= 30.f)
			{
				checkOnBeat = true;
				EventManager::GetInstance()->AddEvent(EventType::BEAT, nullptr);
				// beat�� ���� beat�� ������ �� beathit�� ���� ������ �ȵ�
			}
		}

		if (curPosition > beat + 30.f)
		{
			checkOnBeat = false;
		}

		// �� ���ں��� �� ����
		if (curPosition > beat + beatInterval / 2.f)
		{
			beatDatas.pop();
			beatBefore = beat;
			checkOnBeat = false;
			EventManager::GetInstance()->AddEvent(EventType::BEATMISS, nullptr);
		}
	}
}

void BeatManager::ProcessInput()
{
	InputKey pressedKeyP1 = InputKey::NONE;
	InputKey pressedKeyP2 = InputKey::NONE;

	int up = g_mapKey[{PlayerIndex::PLAYER1, InputKey::UP}];
	int down = g_mapKey[{PlayerIndex::PLAYER1, InputKey::DOWN}];
	int left = g_mapKey[{PlayerIndex::PLAYER1, InputKey::LEFT}];
	int right = g_mapKey[{PlayerIndex::PLAYER1, InputKey::RIGHT}];
	pressedKeyP1 = KeyManager::GetInstance()->GetInputKey(up, down, left, right);

	up = g_mapKey[{PlayerIndex::PLAYER2, InputKey::UP}];
	down = g_mapKey[{PlayerIndex::PLAYER2, InputKey::DOWN}];
	left = g_mapKey[{PlayerIndex::PLAYER2, InputKey::LEFT}];
	right = g_mapKey[{PlayerIndex::PLAYER2, InputKey::RIGHT}];
	pressedKeyP2 = KeyManager::GetInstance()->GetInputKey(up, down, left, right);

	if ((int)pressedKeyP1 | (int)pressedKeyP2)
	{
		//cout << (int)pressedKeyP1 << ' ' << (int)pressedKeyP2 << endl;
	}

	// event ����
	if ((pressedKeyP1 != InputKey::NONE) || (pressedKeyP2 != InputKey::NONE))
	{
		bool hit = IsHit();
		if (pressedKeyP1 != InputKey::NONE)
		{
			BeatHitEventData* event = new BeatHitEventData(PlayerIndex::PLAYER1, pressedKeyP1);
			EventManager::GetInstance()->AddEvent(hit ? EventType::BEATHIT : EventType::BEATMISS, event);
		}
		if (pressedKeyP2 != InputKey::NONE)
		{
			BeatHitEventData* event = new BeatHitEventData(PlayerIndex::PLAYER2, pressedKeyP2);
			EventManager::GetInstance()->AddEvent(hit ? EventType::BEATHIT : EventType::BEATMISS, event);
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
