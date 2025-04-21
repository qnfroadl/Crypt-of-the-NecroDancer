#pragma once

#include "Singleton.h"
#include "TileItem.h"
#include "IRendableTileActor.h"
#include <memory>
//	지정된위치에 아이템을 생성만 해주는 클래스. 포지션 매니저에게 위임한다.

class EventData;
class PositionManager;
class ItemSpawner : public GameActor
{
	
private:
	weak_ptr<PositionManager> positionManager;

	void OnSpawnItem(EventData* data);
public:

	HRESULT Init() override;
	void SpawnItem(POINT tileIndex, FPOINT pos, ItemType type, int value);
	void SetPositionManager(weak_ptr<PositionManager> positionManager);
};