#include "BeatMarker.h"
#include "Image.h"
#include "ImageManager.h"

BeatMarker::BeatMarker()
	: dead{ true }, red{}, beatPosition{}, speed{ 3000.f }
{
}

BeatMarker::~BeatMarker()
{
}

void BeatMarker::Init(unsigned int _beatPosition, bool _red)
{
	beatPosition = _beatPosition;
	red = _red;
	if (red)
	{
		image = ImageManager::GetInstance()->AddImage("beat_marker_red", L"Image/UI/TEMP_beat_marker_red.bmp", 6, 32, 1, 1, true, RGB(255, 0, 255));
	}
	else
	{
		image = ImageManager::GetInstance()->AddImage("beat_marker", L"Image/UI/TEMP_beat_marker.bmp", 6, 32, 1, 1, true, RGB(255, 0, 255));
	}

	RECT client;
	GetClientRect(g_hWnd, &client);
	int height = client.bottom - client.top;
	position = { 0.f, (float)height - image->GetHeight()};
	dead = false;
}

void BeatMarker::Release()
{
}

void BeatMarker::Update(unsigned int curPosition)
{
	if (curPosition > beatPosition) dead = true;
	if (dead) return;

	float diff = (float)(beatPosition - curPosition) / speed;
	diff = max(0.f, min(1.f, diff));

	RECT client;
	GetClientRect(g_hWnd, &client);
	float width = client.right - client.left;
	float height = client.bottom - client.top;

	position.x = width * diff;
	position.y = height - image->GetHeight();
}

void BeatMarker::Render(HDC hdc)
{
	if (dead) return;
	if (image)
	{
		RECT client;
		GetClientRect(g_hWnd, &client);
		float center = (client.right - client.left) / 2.f;
		image->FrameRender(hdc, center + position.x, position.y, 0, 0);
		image->FrameRender(hdc, center - position.x, position.y, 0, 0);
	}
}
