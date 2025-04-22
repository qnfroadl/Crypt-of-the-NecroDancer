#pragma once

/*
	타일위에서 이동하는 모든 액터들의 정보를 통합해 놓은 곳.
	Player는 타Player, Monster와 Item이 특정 타일에 있는지 확인하고
	Monster의 경우는 Player들, 타 Monster들이 특정 타일에 있는지 확인하여 이동을 제어 하거나 공격 할 대상이 있음을 알 수 있다.
*/

// PositionManager.h
#pragma once

#include <vector>
#include <memory>
#include <map>
#include <set>
#include <windows.h>
#include "IRendableTileActor.h"

struct PointCompare
{
    bool operator()(const POINT& a, const POINT& b) const
    {
        if (a.y != b.y) 
        {
            return a.y < b.y;
        }
        
        return a.x < b.x;
    }
};

class TileActor;
class PositionManager : public IRendableTileActor
{
private:
    std::map<POINT, std::vector<std::shared_ptr<TileActor>>, PointCompare> posMap;
    
public:
    PositionManager();
    ~PositionManager();

    vector<shared_ptr<TileActor>> GetRendableTileActors() override;

    void AddTileActor(const std::shared_ptr<TileActor>& actor);
    void MovedTileActor(POINT preIndex, const std::shared_ptr<TileActor>& actor);
    void RemoveTileActor(const std::shared_ptr<TileActor>& actor, bool isUseIndex = false , POINT index = {0,0});
    shared_ptr<TileActor> GetActor(POINT index, ActorType type);

    std::vector<std::shared_ptr<TileActor>> GetActorsAt(const POINT& pos);

    void Render(HDC hdc);

    void Clear();

};

