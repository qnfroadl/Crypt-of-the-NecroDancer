#include "ItemBomb.h"
#include "Player.h"
#include "ImageManager.h"
#include "Camera.h"

ItemBomb::ItemBomb()
	:image(nullptr)
{
	SetItemType(ItemType::BOMB);
}

ItemBomb::~ItemBomb()
{

}

HRESULT ItemBomb::Init()
{
	image = ImageManager::GetInstance()->AddImage("bomb_drop", L"Image/Player/item/bomb_3.bmp", 24 * 3, 48 * 3,1, 2, true, RGB(255, 0, 255));
	return S_OK;
}

void ItemBomb::Render(HDC hdc)
{
	FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());
	if (image)
	{
		image->FrameRender(hdc, pos.x, pos.y, 0, sightState == SightState::VISIBLE? 0 : 1, false, true);
	}
}

void ItemBomb::Release()
{

}

void ItemBomb::Interact(GameActor* actor)
{
	if (false == HasOwner() && actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		SetOwner(player);
		player->AddBomb(shared_from_this(), 3);		
	}
}
