#include "BeatMarkerManager.h"
#include "BeatMarker.h"

BeatMarkerManager::BeatMarkerManager()
{
}

BeatMarkerManager::~BeatMarkerManager()
{
}

void BeatMarkerManager::Init(queue<unsigned int> beatQueue)
{
	beats = beatQueue;
	totalNum = beats.size();

	for (auto marker : markers)
	{
		if (marker)
		{
			marker->SetDead(true);
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		AddMarker();
	}
}

void BeatMarkerManager::Release()
{
	for (auto marker : markers)
	{
		if (marker)
		{
			marker->Release();
			delete marker;
			marker = nullptr;
		}
	}
}

void BeatMarkerManager::Update(unsigned int curPosition)
{
	int activeMarkerNum{};
	for (auto marker : markers)
	{
		if (marker)
		{
			marker->Update(curPosition);
			if (!marker->IsDead()) activeMarkerNum++;
		}
	}
	
	if (activeMarkerNum < 10)
	{
		AddMarker();
	}
}

void BeatMarkerManager::Render(HDC hdc)
{
	for (auto marker : markers)
	{
		if (marker)
		{
			marker->Render(hdc);
		}
	}
}

void BeatMarkerManager::AddMarker()
{
	if (beats.empty()) return;

	bool red = false;
	if (totalNum - beats.size() > totalNum * 0.9f) red = true;

	unsigned int beat = beats.front();
	beats.pop();

	for (auto marker : markers)
	{
		if (marker->IsDead())
		{
			marker->Init(beat, red);
			return;
		}
	}

	BeatMarker* marker = new BeatMarker();
	marker->Init(beat, red);
	markers.push_back(marker);
}
