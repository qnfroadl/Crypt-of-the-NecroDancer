#include "ItemGold.h"
#include "Player.h"
#include "ImageManager.h"

ItemGold::ItemGold(int gold)
	: gold(gold)
{
	SetItemType(ItemType::GOLD);
}

ItemGold::~ItemGold()
{

}

HRESULT ItemGold::Init()
{
	image = ImageManager::GetInstance()->FindImage(EImageKey::CADENCE);


	return S_OK;
}

void ItemGold::Render(HDC hdc)
{
	if (image)
	{
		image->RenderCenter(hdc, GetPos().x, GetPos().y);
	}
}

void ItemGold::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		player->AddGold(gold);
	}
}
