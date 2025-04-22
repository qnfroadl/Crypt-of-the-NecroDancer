#include "PlayerManager.h"

#include "Player.h"
#include "Tilemap.h"

PlayerManager::PlayerManager()
	:playerCount(1)
{
	
}

PlayerManager::~PlayerManager()
{
	Release();
}

HRESULT PlayerManager::Init()
{
	// 플레이어 수에 따라 플레이어 객체를 생성
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] == nullptr)
		{
			players[i] = make_shared<Player>();
			players[i]->SetPlayerIndex(PlayerIndex(i));
		}

		players[i]->Init();
	}
	
    return S_OK;
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

void PlayerManager::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
    tileMap = _tileMap;
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->SetTileMap(tileMap);
		}
	}

}

void PlayerManager::SetPositionManager(weak_ptr<PositionManager> _positionManager)
{
	positionManager = _positionManager;
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->SetPositionManager(_positionManager);
		}
	}
}

weak_ptr<Player> PlayerManager::GetPlayer(PlayerIndex index)
{
    return players[int(index)];
}

void PlayerManager::BindPlayerObserver(PlayerIndex index, IPlayerObserver* observer)
{
	if (players[int(index)] != nullptr)
	{
		players[int(index)]->AddObserver(observer);
	}
}

void PlayerManager::BindRelease()
{
	for (int i = 0; i < playerCount; i++)
	{
		if (players[i] != nullptr)
		{
			players[i]->BindRelease();
		}
	}
}

