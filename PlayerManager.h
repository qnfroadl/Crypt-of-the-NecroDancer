#pragma once

#include "GameObject.h"

class Player;
class TileMap;
class PositionManager;
class PlayerManager : GameObject
{

private:
	Player* players[2];
	weak_ptr<PositionManager> positionManager;
	weak_ptr<TileMap> tileMap;

public:
	void SetTileMap(weak_ptr<TileMap> tileMap);
	void SetPositionManager(weak_ptr<PositionManager> positionManager);



};

