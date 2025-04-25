#include "BeatHeart.h"
#include "Image.h"
#include "ImageManager.h"
#include "EventManager.h"
#include "ItemNumUI.h"

BeatHeart::BeatHeart()
	: frameIndex{}
{
}

BeatHeart::~BeatHeart()
{
}

HRESULT BeatHeart::Init()
{
	image = ImageManager::GetInstance()->AddImage("beat_heart", L"Image/UI/TEMP_beat_heart.bmp", 82, 52, 2, 1, true, RGB(255, 0, 255), true);
	
	LocateOnCenter();

	frameIndex = 0;


	EventManager::GetInstance()->BindEvent(this, EventType::BEAT, std::bind(&BeatHeart::BeatStart, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATEND, std::bind(&BeatHeart::BeatEnd, this));

	return S_OK;
}

void BeatHeart::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEAT);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATEND);

}

void BeatHeart::Update()
{
	LocateOnCenter();
}

void BeatHeart::Render(HDC hdc)
{
	int height = 0;
	int width = 0;
	if (image) 
	{
		image->FrameRender(hdc, position.x, position.y, frameIndex, 0, size.x, size.y, false, true);
		height = image->GetFrameHeight();
		width = image->GetFrameWidth() / 2;
	}

}

void BeatHeart::LocateOnCenter()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { (float)clientHeight * 0.003f, (float)clientHeight * 0.003f };
	int imageWidth = image->GetFrameWidth() * size.x; 
	int imageHeight = image->GetFrameHeight() * size.y;

	position = { (float)clientWidth * 0.5f, (float)clientHeight - (float)imageHeight * 0.6f };
}
