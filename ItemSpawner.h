#pragma once

#include "Singleton.h"
#include "TileItem.h"

//	지정된위치에 아이템을 생성해주는 클래스.
//	객체들을 관리는 하지만, Clear호출시에만 전체적으로 정리.

class ItemSpawner : public Singleton<ItemSpawner>
{
	
private:
	vector<TileItem*> items;

public:
	void SpawnItem(POINT tileIndex, int x, int y, ItemType type);
	void SpawnGold(POINT tileIndex, int x, int y, int gold);
	void Clear();

};