#include "TileActorRenderer.h"
#include "EventManager.h"

#include "Tilemap.h"
#include "Tile.h"
#include "PositionManager.h"

void TileActorRenderer::AddRendableSource(std::shared_ptr<IRendableTileActor> source)
{
    rendableActors.push_back(source);
}

void TileActorRenderer::OnChangedSightMap(EventData* data)
{
    ShadowCastingEventData* sightData = dynamic_cast<ShadowCastingEventData*>(data);
    
    if (sightData)
    {
        // 시야값 복사.
        sightMap = sightData->sightMap;
    }
}

HRESULT TileActorRenderer::Init()
{
    EventManager::GetInstance()->BindEvent(this, EventType::LIGHTINGUPDATED, std::bind(&TileActorRenderer::OnChangedSightMap, this, placeholders::_1));


    return S_OK;
}

void TileActorRenderer::Release()
{
    EventManager::GetInstance()->UnbindEvent(this, EventType::LIGHTINGUPDATED);
}

void TileActorRenderer::Render(HDC hdc)
{
    bool sightVisible = false;
    float brightness = 0.f;

    POINT leftTop = tileMap->GetLeftTop();
    POINT rightBottom = tileMap->GetRightBottom();

    const vector<vector<shared_ptr<Tile>>>& tiles = tileMap->GetTiles();

    for (int y = leftTop.y; y <= rightBottom.y; ++y)
    {
        for (int x = leftTop.x; x <= rightBottom.x; ++x)
        {
            // 타일 렌더.
            sightVisible = sightMap[y][x].first;
            brightness = sightMap[y][x].second;

            tiles[y][x]->SetVisible(sightVisible);
            tiles[y][x]->Render(hdc, true);
            std::vector<std::shared_ptr<TileActor>> actors = positionManager->GetActorsAt(POINT{x,y});
            
            for (auto actor : actors)
            {
                // 일단은 타일 위의 액터들 렌더. 순서 무시.
                actor->SetVisible(sightVisible);
                actor->Render(hdc);
            }
        }
    }
}

void TileActorRenderer::SetTileMap(shared_ptr<Tilemap> _tileMap)
{
    tileMap = _tileMap;
}

void TileActorRenderer::SetPositionManager(shared_ptr<PositionManager> _positionManager)
{
    positionManager = _positionManager;
}

void TileActorRenderer::OnPlayerMoved()
{

}

void TileActorRenderer::OnTileDestroyed()
{
	
}
