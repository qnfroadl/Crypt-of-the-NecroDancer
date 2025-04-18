#include "UIBeatMarker.h"
#include "Image.h"
#include "ImageManager.h"

UIBeatMarker::UIBeatMarker()
	: dead{true}, red {}
{
}

UIBeatMarker::~UIBeatMarker()
{
}

void UIBeatMarker::Init(bool _red)
{
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

void UIBeatMarker::Release()
{
}

void UIBeatMarker::Update(float diff)
{
	if (dead) return;
	if (diff <= 0) dead = true;
	RECT client;
	GetClientRect(g_hWnd, &client);
	float width = (client.right - client.left);
	position.x = width * diff;
	//position.x += diff;
}

void UIBeatMarker::Render(HDC hdc)
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
