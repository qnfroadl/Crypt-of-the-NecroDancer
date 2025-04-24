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
	bool checkOnBeat; // ���� üũ

	bool isP1Hit;
	bool isP2Hit;

	BeatMarkerManager* markerManager;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void StartBeat(bool _checkBeat); // zone �� �� ȣ��
	void UpdateBeat();

	void ProcessInput();

	bool IsHit(bool &playerHit);
};

