#include "ItemSpawner.h"

#include "ItemGold.h"
#include "ItemBomb.h"
#include "EventManager.h"
#include "PositionManager.h"
#include "Tilemap.h"
#include "Weapon.h"
#include "EventData.h"

void ItemSpawner::OnSpawnItem(EventData* data)
{
	if (data)
	{
		SpawnItemEventData* itemData = static_cast<SpawnItemEventData*>(data);
		switch (itemData->type)
		{
		case ItemType::GOLD:
		case ItemType::DIAMOND:
		case ItemType::BOMB:
			SpawnItem(itemData->index, itemData->type, itemData->value);
			break;
		}
		
	}
}

void ItemSpawner::OnSpawnWeapon(EventData* data)
{
	shared_ptr<Weapon> item;
	
	if (data)
	{
		item = make_shared<Weapon>();
		item->Init();
		SpawnWeaponEventdata* weaponData = static_cast<SpawnWeaponEventdata*>(data);	
		item->SetWeaponType(weaponData->damageType, weaponData->weaponType);
		item->SetMaterial(weaponData->material);

		FPOINT pos = tileMap.lock()->GetTilePos(weaponData->index);
		item->SetTileIndex(weaponData->index);
		item->SetItemType(ItemType::WEAPON);
		item->SetPos(pos);

		positionManager.lock()->AddTileActor(item);		
	}
}

HRESULT ItemSpawner::Init()
{
	EventManager::GetInstance()->BindEvent(this, EventType::SPAWNITEM, std::bind(&ItemSpawner::OnSpawnItem, this, std::placeholders::_1));
	EventManager::GetInstance()->BindEvent(this, EventType::SPAWNWEAPON, std::bind(&ItemSpawner::OnSpawnWeapon, this, std::placeholders::_1));

	return S_OK;
}

void ItemSpawner::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::SPAWNITEM);
	EventManager::GetInstance()->UnbindEvent(this, EventType::SPAWNWEAPON);

}

void ItemSpawner::SpawnItem(POINT tileIndex,ItemType type, int value)
{
	shared_ptr<TileItem> item = nullptr;
	switch (type)
	{
	case ItemType::GOLD:
		item = std::make_shared<ItemGold>(value);
		break;
	case ItemType::DIAMOND:
		item = std::make_shared<TileItem>();
		break;
	case ItemType::BOMB:
		item = std::make_shared<ItemBomb>();
		break;
	default:
		break;
	}
	
	if (item)
	{
		item->Init();
		FPOINT pos = tileMap.lock()->GetTilePos(tileIndex);

		item->SetTileIndex(tileIndex);
		item->SetItemType(type);
		item->SetPos(pos);

		positionManager.lock()->AddTileActor(item);
	}
}

void ItemSpawner::SetPositionManager(weak_ptr<PositionManager> _positionManager)
{
	positionManager = _positionManager;
}

void ItemSpawner::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
}
