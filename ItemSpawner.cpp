#include "ItemSpawner.h"

#include "ItemGold.h"
#include "ItemBomb.h"
#include "EventManager.h"
#include "PositionManager.h"

void ItemSpawner::OnSpawnItem(EventData* data)
{
	if (data)
	{
		SpawnItemEventData* itemData = static_cast<SpawnItemEventData*>(data);
		SpawnItem(itemData->index, itemData->pos, itemData->type, itemData->value);
	}
}

HRESULT ItemSpawner::Init()
{
	EventManager::GetInstance()->BindEvent(EventType::SPAWNITEM, std::bind(&ItemSpawner::OnSpawnItem, this, std::placeholders::_1));
	return S_OK;
}

void ItemSpawner::SpawnItem(POINT tileIndex, FPOINT pos, ItemType type, int value)
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
