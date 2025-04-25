#include "TileItem.h"

#include "Player.h"

TileItem::TileItem()
	: owner(nullptr)
{
	SetType(ActorType::ITEM);
}

TileItem::~TileItem()
{

}

void TileItem::Render(HDC hdc)
{
// 아이템 이미지 렌더링
	// 카메라 위치에 따라 아이템 위치 조정
	if (false == HasOwner() && IsActive())
	{
		// 아이템이 위 아래로 움직이는 렌더링 애니메이션.


	}
}

void TileItem::Interact(GameActor* actor)
{
	if(actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		switch (type)
		{
		case ItemType::DIAMOND:
			player->AddDiamond(1);
			break;

		default:
			break;
		}
	}
}

void TileItem::Destroy()
{
	SetActive(false);
}

void TileItem::Drop()
{
	owner = nullptr;
}

void TileItem::Equip()
{
}

void TileItem::Use()
{
}
