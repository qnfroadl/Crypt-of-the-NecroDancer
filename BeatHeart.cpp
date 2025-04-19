#include "BeatHeart.h"
#include "Image.h"
#include "ImageManager.h"

BeatHeart::BeatHeart()
{
}

BeatHeart::~BeatHeart()
{
}

void BeatHeart::Init(queue<unsigned int> beatQueue)
{
	beats = beatQueue;

	image = ImageManager::GetInstance()->AddImage("beat_heart", L"Image/UI/TEMP_beat_heart.bmp", 82, 52, 2, 1, true, RGB(255, 0, 255));
	
	LocateOnCenter();

	frameIndex = 0;
}

void BeatHeart::Release()
{
}

void BeatHeart::Update(unsigned int curPosition)
{
	LocateOnCenter();

	frameIndex = 0;

	if (beats.size())
	{
		unsigned int beat = beats.front();
		if (abs((int)curPosition - (int)beat) <= 30.f)
		{
			frameIndex = 1;
		}

		if (curPosition > beat + 30.f)
		{
			beats.pop();
		}
	}
}

void BeatHeart::Render(HDC hdc)
{
	if (image) image->FrameRender(hdc, position.x, position.y, frameIndex, 0, size.x, size.y, false, true);
}

void BeatHeart::LocateOnCenter()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { 1.3f, 1.3f };
	int imageWidth = image->GetFrameWidth() * size.x;
	int imageHeight = image->GetFrameHeight() * size.y;

	position = { (float)clientWidth * 0.5f, (float)clientHeight - (float)imageHeight * 0.5f };
}
