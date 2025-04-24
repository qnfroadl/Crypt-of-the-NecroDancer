#include "ItemGold.h"
#include "Player.h"
#include "ImageManager.h"
#include "Camera.h"
#include "SoundManager.h"

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
	image = ImageManager::GetInstance()->FindImage("gold1");

	return S_OK;
}

void ItemGold::Render(HDC hdc)
{
	if (sightState == SightState::INVISIBLE)
	{
		return;
	}

	if (IsActive())
	{
		FPOINT screenPos = Camera::GetInstance()->GetScreenPos(GetPos());

		if (image)
		{
			image->FrameRender(hdc, screenPos.x, screenPos.y, 0, sightState == SightState::VISIBLE ? 0 : 1, false, true);
		}
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
		image = ImageManager::GetInstance()->FindImage(string("coin") + std::to_string(gold));
	}
	else if(gold < 30)
	{
		image = ImageManager::GetInstance()->FindImage("coin10");
	}
	else if (gold < 50)
	{
		image = ImageManager::GetInstance()->FindImage("coin30");
	}
	else if (gold < 100)
	{
		image = ImageManager::GetInstance()->FindImage("coin50");
	}
	else 
	{
		image = ImageManager::GetInstance()->FindImage("coin100");
	}

}

void ItemGold::Interact(GameActor* actor)
{
	if (IsActive() && actor->GetType() == ActorType::PLAYER)
	{
		Player* player = static_cast<Player*>(actor);
		player->AddGold(gold);

		SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL);
		SetActive(false);
	}
}
