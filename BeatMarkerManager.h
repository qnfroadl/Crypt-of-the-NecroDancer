#pragma once
#include "config.h"

#include <queue>

class BeatMarker;
class BeatMarkerManager
{
private:
	vector<BeatMarker*> markers;
	queue<unsigned int> beats;
	int totalNum;

public:
	BeatMarkerManager();
	~BeatMarkerManager();

	void Init(queue<unsigned int> beatQueue);
	void Release();
	void Update(unsigned int curPosition);
	void Render(HDC hdc);

	void AddMarker();
};

