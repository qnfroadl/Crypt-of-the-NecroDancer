﻿#include "TileActor.h"

#include "KeyManager.h"
#include "CommonFunction.h"
#include "Camera.h"

void TileActor::Update()
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
