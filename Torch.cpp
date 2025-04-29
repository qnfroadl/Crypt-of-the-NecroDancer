#include "Torch.h"
#include "ImageManager.h"
#include "Camera.h"
#include "Image.h"
#include "TimerManager.h"
HRESULT Torch::Init()
{
	torchImage = ImageManager::GetInstance()->AddImage("TORCH", TEXT("Image/Torch.bmp"), 
		48, 26, 4, 1, true, RGB(255, 0, 255), true);

	curFrame = 0;
	elapsedTime = 0;
    return S_OK;
}

HRESULT Torch::Init(FPOINT _pos, POINT _index)
{
	torchImage = ImageManager::GetInstance()->AddImage("TORCH", TEXT("Image/Torch.bmp"),
		48, 26, 4, 1, true, RGB(255, 0, 255), true);

	curFrame = 0;
	elapsedTime = 0;
    pos = _pos;
    index = _index;
    return S_OK;
}

void Torch::Update()
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

	if (elapsedTime >= 0.1f)
	{
		elapsedTime = 0;
		curFrame++;
		if (torchImage->GetMaxFrameX() <= curFrame)
		{
			curFrame = 0;
		}
	}
}

void Torch::Render(HDC hdc, bool useCamera)
{
	float centerX = pos.x;
	float centerY = pos.y - 20.0f * TILE_SCALE;
	if (useCamera)
	{
		centerX -= Camera::GetInstance()->GetPos().x;
		centerY -= Camera::GetInstance()->GetPos().y;

		float brightness = GetBrightness();
		if (false == GetSightInfo().visible)
		{
			brightness = 0.3;
		}

		torchImage->FrameRender(
			hdc,
			static_cast<int>(centerX),
			static_cast<int>(centerY),
			curFrame, 0,
			static_cast<float>(TILE_SCALE),
			static_cast<float>(TILE_SCALE),
			false, true, brightness, 0.0f
		);
	}
	else
	{
		torchImage->FrameRender(
			hdc,
			static_cast<int>(centerX / TILE_SCALE),
			static_cast<int>(centerY / TILE_SCALE),
			curFrame,
			0,
			false, true
		);
	}
}

vector<shared_ptr<TileActor>> Torch::GetRendableTileActors()
{
	vector<shared_ptr<TileActor>> rendableActors;
	rendableActors.push_back(shared_from_this());
	return rendableActors;
}
