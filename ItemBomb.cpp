#include "ItemBomb.h"
#include "Player.h"

ItemBomb::ItemBomb()
:isExplode(false), damage(3.0f)
{
	SetItemType(ItemType::BOMB);
}

ItemBomb::~ItemBomb()
{

}

HRESULT ItemBomb::Init()
{
	return E_NOTIMPL;
}

void ItemBomb::Render(HDC hdc)
{

}

void ItemBomb::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		player->AddBomb(3);
	}
}
