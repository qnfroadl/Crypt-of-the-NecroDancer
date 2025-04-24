#include "TileActorRenderer.h"
#include "EventManager.h"

void TileActorRenderer::AddRendableSource(std::shared_ptr<IRendableTileActor> source)
{
    rendableActors.push_back(source);
}

HRESULT TileActorRenderer::Init()
{
    


    return S_OK;
}

void TileActorRenderer::Render(HDC hdc)
{



   

}

void TileActorRenderer::OnPlayerMoved()
{

}

void TileActorRenderer::OnTileDestroyed()
{
	
}
