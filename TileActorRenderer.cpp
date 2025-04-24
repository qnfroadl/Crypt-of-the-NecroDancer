#include "TileActorRenderer.h"
#include "EventManager.h"

void TileActorRenderer::AddRendableSource(std::shared_ptr<IRendableTileActor> source)
{
    rendableActors.push_back(source);
}

void TileActorRenderer::OnChangedSightMap(EventData* data)
{
    data = dynamic_cast<ShadowCastingEventData*>(data);
    
    if (data)
    {

    }
}

HRESULT TileActorRenderer::Init()
{
    


    return S_OK;
}

void TileActorRenderer::Render(HDC hdc)
{



}

void TileActorRenderer::SetTileMap(weak_ptr<Tilemap> tileMap)
{

}

void TileActorRenderer::SetPositionManager(weak_ptr<PositionManager> positionManager)
{
}

void TileActorRenderer::OnPlayerMoved()
{

}

void TileActorRenderer::OnTileDestroyed()
{
	
}
