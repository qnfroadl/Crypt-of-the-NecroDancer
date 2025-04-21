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

void PositionManager::AddTileActor(const std::shared_ptr<TileActor>& actor)
{
    const POINT& index = actor->GetTileIndex();
    posMap[index].push_back(actor);
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

std::vector<std::shared_ptr<TileActor>> PositionManager::GetActorsAt(const POINT& pos)
{
    auto it = posMap.find(pos);
    if (it != posMap.end())
    {
        return it->second;
    }
    return {};
}

void PositionManager::Clear()
{
    posMap.clear();
}
