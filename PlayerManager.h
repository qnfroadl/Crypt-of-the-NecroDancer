#pragma once

#include "GameObject.h"

#include "IPlayerObserver.h"

class Player;
class TileMap;
class PositionManager;
class PlayerManager : GameObject
{

private:
	int playerCount;
	shared_ptr<Player> players[2];
	weak_ptr<PositionManager> positionManager;
	weak_ptr<TileMap> tileMap;

public:
	PlayerManager();
	~PlayerManager();
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	void SetTileMap(weak_ptr<TileMap> tileMap);
	void SetPositionManager(weak_ptr<PositionManager> positionManager);

	weak_ptr<Player> GetPlayer(PlayerIndex index);
	void BindPlayerObserver(PlayerIndex index, IPlayerObserver* observer);
	

};

