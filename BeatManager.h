#pragma once
#include "config.h"
#include <queue>

class Image;
class BeatMarkerManager;
class BeatManager
{
private:
	queue<unsigned int> beatDatas;
	unsigned int beatBefore;

	bool checkInputTime;
	bool checkOnBeat; // 정박 체크

	bool active2p;

	bool isP1Hit;
	bool isP2Hit;

	BeatMarkerManager* markerManager;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void SetActive2P(bool _active2p) { active2p = _active2p; isP2Hit = !active2p; }
	void StartBeat(bool _checkBeat); // zone 들어갈 때 호출
	void UpdateBeat();

	void ProcessInput();

	bool IsHit(bool &playerHit);
};

