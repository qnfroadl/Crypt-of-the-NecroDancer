#pragma once
#include "config.h"
#include "Singleton.h"
#include <queue>

class BeatManager : public Singleton<BeatManager>
{
private:
	queue<unsigned int> beatDatas;
	unsigned int beatBefore;

	bool checkBeat;

public:
	void Init();
	void Release();
	void Update();

	void StartBeat(); // zone �� �� ȣ��

	bool IsHit();
};

