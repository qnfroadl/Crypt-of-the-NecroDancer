#include "ItemGold.h"
#include "Player.h"
#include "ImageManager.h"

ItemGold::ItemGold(int gold)
{
	SetGold(gold);
	SetItemType(ItemType::GOLD);
}

ItemGold::~ItemGold()
{

}

HRESULT ItemGold::Init()
{
	image = ImageManager::GetInstance()->FindImage("coin1");

	return S_OK;
}

void ItemGold::Render(HDC hdc)
{
	if (image)
	{
		image->FrameRender(hdc, GetPos().x, GetPos().y,0, 0, false, true);

		// 
	}
	else 
	{
		// 검정 돈
		image->FrameRender(hdc, GetPos().x, GetPos().y, 0, 1, false, true);
	}
}

void ItemGold::SetGold(int gold)
{
	// 채소 골드는 1
	if (gold <= 0)
	{
		gold = 1;
	}

	this->gold = gold;

	if (gold <= 9)
	{
		image = ImageManager::GetInstance()->FindImage("gold" + gold);
	}
	else {
		image = ImageManager::GetInstance()->FindImage("gold10");
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
