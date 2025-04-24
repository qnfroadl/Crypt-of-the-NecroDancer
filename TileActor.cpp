#include "TileActor.h"

#include "KeyManager.h"
#include "CommonFunction.h"
#include "Camera.h"

TileActor::TileActor()
	:index{0,0}
{
	sightState = SightState::INVISIBLE;

	sightInfo.revealed = false;
	sightInfo.visible = false;

	brightnessInfo.staticBrightness = 0.f;
	brightnessInfo.dynamicBrightness = 1.f;

}

void TileActor::Render(HDC hdc)
{
	float screenX = Camera::GetInstance()->GetPos().x;
	float screenY = Camera::GetInstance()->GetPos().y;

	RenderRectAtCenter(hdc, GetPos().x - screenX, GetPos().y - screenY, 50,50);
}

void TileActor::Interact(GameActor* actor)
{

}

const POINT& TileActor::GetTileIndex()
{
	return index;
}

void TileActor::SetTileIndex(const POINT& _index)
{
	index = _index;
}

void TileActor::SetVisible(bool visible)
{
	sightInfo.visible = visible;
	if (visible && false == sightInfo.revealed )
	{
		// 최초 1회. 본 적있다.
		sightInfo.revealed = true;
	}

	if (sightInfo.visible)
	{
		sightState = SightState::VISIBLE;
	}
	else if(sightInfo.revealed)
	{
		sightState = SightState::SHADOW;
	}
	else 
	{
		sightState = SightState::INVISIBLE;
	}
}

const SightInfo& TileActor::GetSightInfo()
{
	return sightInfo;
}

void TileActor::SetBrightnessInfo(const BrightnessInfo& info)
{
	brightnessInfo = info;
}

const BrightnessInfo& TileActor::GetBrightnessInfoInfo()
{
	return brightnessInfo;
}
