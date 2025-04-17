#pragma once
#include "config.h"
#include "Singleton.h"
#include <queue>

class BeatManager : public Singleton<BeatManager>
{
private:
	float tempoRate;
	queue<int> beatDatas;
	bool start;

public:
	void Init();
	void Release();
	void Update();

	void StartBeat();
};

