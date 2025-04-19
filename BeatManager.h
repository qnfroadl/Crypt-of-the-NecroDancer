#pragma once
#include "config.h"
#include "Singleton.h"
#include <queue>

class Image;
class BeatMarkerManager;
class BeatManager : public Singleton<BeatManager>
{
private:
	queue<unsigned int> beatDatas;
	unsigned int beatBefore;

	bool checkInputTime;
	bool checkOnBeat; // ���� üũ

	BeatMarkerManager* markerManager;

public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void StartBeat(bool _checkBeat); // zone �� �� ȣ��
	void UpdateBeat();

	bool IsHit();
};

