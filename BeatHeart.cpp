#include "BeatHeart.h"
#include "Image.h"
#include "ImageManager.h"

BeatHeart::BeatHeart()
{
}

BeatHeart::~BeatHeart()
{
}

void BeatHeart::Init()
{
	image = ImageManager::GetInstance()->AddImage("beat_heart", L"Image/UI/TEMP_beat_heart.bmp", 82, 52, 2, 1, true, RGB(255, 0, 255));
	
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { 1.3f, 1.3f };
	int imageWidth = image->GetFrameWidth() * size.x;
	int imageHeight = image->GetFrameHeight() * size.y;

	position = { (float)clientWidth * 0.5f, (float)clientHeight - (float)imageHeight * 0.5f };
	frameIndex = 0;
}

void BeatHeart::Release()
{
}

void BeatHeart::Update(bool onBeat)
{
	if (onBeat) frameIndex = 1;
	else frameIndex = 0;
}

void BeatHeart::Render(HDC hdc)
{
	if (image) image->FrameRender(hdc, position.x, position.y, frameIndex, 0, size.x, size.y, false, true);
}
