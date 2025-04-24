#pragma once

#include "IRendableTileActor.h"
#include "Camera.h"

/*
* 타일맵이 그려져야 할 위치를 보고(시작, 끝)
* 시작 부터 끝까지 타일맵을 렌더 해 나가면서, PositionManager의 TileActor들을 렌더링 한다
* 이때 PositionManager의 TileActor들은. 각각의 z-order (렌더 순위)를 갖고 있으며. 이 z-order에 따라서.
* 렌더하고 다음 타일로 이동한다.
*/
class Tilemap;
class LightingRenderer;
class PositionManager;
class EventData;
class TileActorRenderer : public GameActor
{

private:
	vector<vector<bool>> sightMap;

	shared_ptr<Tilemap> tileMap;
	shared_ptr<PositionManager> positionManager;

	std::vector<std::shared_ptr<IRendableTileActor>> rendableActors;

	void OnChangedSightMap(EventData* data);

public:

	HRESULT Init() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetTileMap(shared_ptr<Tilemap> tileMap); 
	void SetPositionManager(shared_ptr<PositionManager> positionManager);

	void AddRendableSource(std::shared_ptr<IRendableTileActor> source);
	void OnPlayerMoved(); // 또는 OnLightingUpdateTrigger()
	void OnTileDestroyed();


};

