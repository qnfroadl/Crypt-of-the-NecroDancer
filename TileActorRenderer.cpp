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
    std::vector<std::shared_ptr<TileActor>> yRender;

    // 모든 IRendableTileActor에서 렌더 가능한 TileActor 수집
    for (auto& source : rendableActors)
    {
        auto actors = source->GetRendableTileActors();
        yRender.insert(yRender.end(), actors.begin(), actors.end());
    }

    // Y 좌표를 기준으로 정렬 (y가 작을수록 먼저 그리기)
    std::sort(yRender.begin(), yRender.end(), [](const std::shared_ptr<TileActor>& a, const std::shared_ptr<TileActor>& b)
        {
            return a->GetPos().y < b->GetPos().y;       // t가 작은 값이 앞으로.
        });

    // 정렬된 순서대로 렌더링
    for (auto& actor : yRender)
    {
        actor->Render(hdc);
    }

}

void TileActorRenderer::OnPlayerMoved()
{

}

void TileActorRenderer::OnTileDestroyed()
{
	
}
