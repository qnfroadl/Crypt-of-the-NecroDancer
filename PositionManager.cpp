// PositionManager.cpp
#include "PositionManager.h"
#include "TileActor.h"

PositionManager::PositionManager()
{
  
}

PositionManager::~PositionManager()
{
    Clear();
}

vector<shared_ptr<TileActor>> PositionManager::GetRendableTileActors()
{
    vector<shared_ptr<TileActor>> rendableActors;
	for (auto& pair : posMap)
	{
		for (auto& actor : pair.second)
		{
			rendableActors.push_back(actor);
		}
	}
	return rendableActors;


}

void PositionManager::AddTileActor(const std::shared_ptr<TileActor>& _actor)
{
    const POINT& index = _actor->GetTileIndex();

    const auto& actors = posMap[index];
    for (const auto& actor : actors)
    {
        if (actor->IsActive())
        {
            actor->Interact(_actor.get());
            _actor->Interact(actor.get());
        }
        
    }

    posMap[index].push_back(_actor);
}

void PositionManager::MovedTileActor(POINT preIndex, const std::shared_ptr<TileActor>& actor)
{
    RemoveTileActor(actor, true, preIndex);  // 이전 위치 지우기
	AddTileActor(actor);  // 새로운 위치 추가
}

void PositionManager::RemoveTileActor(const std::shared_ptr<TileActor>& actor, bool isUseIndex/* = false */, POINT index/* = {0,0}*/)
{
    POINT pos;

	if (isUseIndex)
	{
		pos = index;
	}
	else
	{
        pos = actor->GetTileIndex();
	}
    

    auto it = posMap.find(pos);
    if (it != posMap.end())
    {
        auto& vec = it->second;
        auto vecIt = std::remove(vec.begin(), vec.end(), actor);
        if (vecIt != vec.end())
        {
            vec.erase(vecIt, vec.end());
            if (vec.empty())
            {
                posMap.erase(it);
            }
        }
    }
}

shared_ptr<TileActor> PositionManager::GetActor(POINT index, ActorType type)
{
    auto it = posMap[index].begin();
    while (it != posMap[index].end())
    {
        if (type == (*it)->GetType() && (*it)->IsActive())
        {
            return *it;
        }
        it++;
    }

    return nullptr;
}

std::vector<std::shared_ptr<TileActor>> PositionManager::GetActorsAt(const POINT& pos)
{
    auto it = posMap.find(pos);
    if (it != posMap.end())
    {
        return it->second;
    }
    return {};
}

std::vector<std::shared_ptr<TileActor>> PositionManager::GetActorsAt(const POINT& pos, ActorType type)
{
    vector<shared_ptr<TileActor>> actors;

    auto it = posMap.find(pos);
    if (it != posMap.end())
    {
        for (auto actor : it->second)
        {
            if (type == actor->GetType())
            {
                actors.push_back(actor);
            }
        }
    }

    return actors;
}

void PositionManager::Render(HDC hdc)
{
    // 테스트용.
    auto it = posMap.begin();
    while (it != posMap.end())
    {
		auto& vec = it->second;
		for (auto& actor : vec)
		{
			actor->Render(hdc);
		}
		it++;
    }

}

void PositionManager::Clear()
{
    posMap.clear();
}
