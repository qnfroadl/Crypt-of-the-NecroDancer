#include "ItemSpawner.h"

#include "ItemGold.h"
#include "ItemBomb.h"
void ItemSpawner::SpawnItem(POINT tileIndex, int x, int y, ItemType type)
{
	TileItem* item = nullptr;
	switch (type)
	{
	case ItemType::GOLD:
		item = new ItemGold();
		break;
	case ItemType::DIAMOND:
		item = new TileItem();
		item->SetItemType(ItemType::DIAMOND);
		break;
	case ItemType::BOMB:
		item = new ItemBomb();
		break;
	default:
		break;
	}
	if (item)
	{
		item->SetPos(x, y);
		item->SetTileIndex(tileIndex);
		items.push_back(item);
	}
}

void ItemSpawner::SpawnGold(POINT tileIndex, int x, int y, int gold)
{

}

void ItemSpawner::Clear()
{
	// 소유주가 없는 아이템을. 제거.
	// 포지션 매니저에서 제거,
	// 아이템 매니저에서 제거.
	// 인스턴스 삭제.

}
