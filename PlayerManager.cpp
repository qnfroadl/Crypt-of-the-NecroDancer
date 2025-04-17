#include "PlayerManager.h"

#include "Player.h"

PlayerManager::PlayerManager()
	:playerCount(1)
{
	

	
}

PlayerManager::~PlayerManager()
{
}

HRESULT PlayerManager::Init()
{
	// 플레이어 수에 따라 플레이어 객체를 생성
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] == nullptr)
		{
			players[i] = make_shared<Player>();
		}

		players[i]->Init();
	}
	
    return E_NOTIMPL;
}

void PlayerManager::Update()
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->Update();
		}
	}
}

void PlayerManager::Render(HDC hdc)
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->Render(hdc);
		}
	}
}

void PlayerManager::Release()
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->Release();
			players[i] = nullptr;
		}
	}
}

void PlayerManager::SetTileMap(weak_ptr<TileMap> _tileMap)
{
    tileMap = tileMap;
}

void PlayerManager::SetPositionManager(weak_ptr<PositionManager> _positionManager)
{
	positionManager = _positionManager;
}

weak_ptr<Player> PlayerManager::GetPlayer(PlayerIndex index)
{
    return players[int(index)];
}
