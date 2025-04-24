#include "TileActor.h"

#include "KeyManager.h"
#include "CommonFunction.h"
#include "Camera.h"

TileActor::TileActor()
	:index{0,0}
{

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

void TileActor::SetSightInfo(const SightInfo& _sightInfot)
{
	sightInfo = _sightInfot;
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
