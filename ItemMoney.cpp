#include "ItemMoney.h"
#include "Player.h"
#include "ImageManager.h"

ItemMoney::ItemMoney(int gold)
	: gold(gold)
{
	SetItemType(ItemType::GOLD);
}

ItemMoney::~ItemMoney()
{

}

HRESULT ItemMoney::Init()
{
	image;


	return S_OK;
}

void ItemMoney::Render(HDC hdc)
{
	if (image)
	{
		image->RenderCenter(hdc, GetPos().x, GetPos().y);
	}
}

void ItemMoney::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		player->AddGold(gold);
	}
}
