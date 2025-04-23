#pragma once

#include "Singleton.h"
#include "TileItem.h"
#include "IRendableTileActor.h"
#include <memory>
//	지정된위치에 아이템을 생성만 해주는 클래스. 포지션 매니저에게 위임한다.

class EventData;
class PositionManager;
class Tilemap;
class ItemSpawner : public GameActor
{
	
private:
	weak_ptr<PositionManager> positionManager;
	weak_ptr<Tilemap> tileMap;

	void OnSpawnItem(EventData* data);
	void OnSpawnWeapon(EventData* data);

public:

	HRESULT Init() override;
	void Release() override;

	void SpawnItem(POINT tileIndex,ItemType type, int value);
	void SetPositionManager(weak_ptr<PositionManager> positionManager);
	void SetTileMap(weak_ptr<Tilemap> _tileMap);
};