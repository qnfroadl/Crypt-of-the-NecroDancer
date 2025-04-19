#include "BeatMarkerManager.h"
#include "BeatMarker.h"
#include "Beatheart.h"

BeatMarkerManager::BeatMarkerManager()
	: markers{}, beats{}, totalNum{},
	heart{}
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

	if (!heart)
	{
		heart = new BeatHeart();
	}
	heart->Init();
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

	if (heart)
	{
		heart->Release();
		delete heart;
		heart = nullptr;
	}
}

void BeatMarkerManager::Update(unsigned int curPosition, bool checkOnBeat)
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

	if (heart)
	{
		heart->Update(checkOnBeat);
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

	if (heart)
	{
		heart->Render(hdc);
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
