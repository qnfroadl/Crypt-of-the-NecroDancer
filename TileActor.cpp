#include "TileActor.h"

#include "KeyManager.h"
#include "CommonFunction.h"
#include "Camera.h"

void TileActor::Update()
{
	if (KeyManager::GetInstance()->IsStayKeyDown('W'))
	{
		AddPos(0,-5);
	}
	else if(KeyManager::GetInstance()->IsStayKeyDown('A'))
	{
		AddPos(-5, 0);
	}
	else if (KeyManager::GetInstance()->IsStayKeyDown('S'))
	{
		AddPos(0, 5);
	}
	else if (KeyManager::GetInstance()->IsStayKeyDown('D'))
	{
		AddPos(5, 0);
	}
}

void TileActor::Render(HDC hdc)
{
	float screenX = Camera::GetInstance()->GetPos().x;
	float screenY = Camera::GetInstance()->GetPos().y;

	RenderRectAtCenter(hdc, GetPos().x - screenX, GetPos().y - screenY, 50,50);
}
