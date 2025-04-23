#include "LobbyScene.h"
#include "Tilemap.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "PlayerManager.h"
#include "Player.h"

#include "UIManager.h"
#include "PlayerWallet.h"
#include "PlayerHp.h"

#include "PositionManager.h"
#include "ItemSpawner.h"

#include "EventManager.h"
#include "EventData.h"

#include "MonsterManager.h"
#include "TilemapGenerator.h"

HRESULT LobbyScene::Init()
{
    // InitMap
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);

    if (nullptr == levelManager)
    {
        levelManager = make_unique<LevelManager>();
        levelManager->Init();
    }

    map = make_shared<Tilemap>();
    map = make_shared<Tilemap>(*(TilemapGenerator::GetInstance()->Generate("LOBBY")));
	
    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

	positionManager = make_shared<PositionManager>();
	itemSpawner = make_shared<ItemSpawner>();
    itemSpawner->Init();
	itemSpawner->SetPositionManager(positionManager);

    uiManager = new UIManager();
	uiManager->Init();

	PlayerWallet* playerCoin = new PlayerWallet();
	playerCoin->Init();
	uiManager->AddUI(playerCoin);

	PlayerHp* playerHp = new PlayerHp();
	playerHp->Init();
	playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerHp);
	uiManager->AddUI(playerHp);


    if (playerManager.lock())
    {
        playerManager.lock()->SetPositionManager(positionManager);
        playerManager.lock()->SetTileMap(map);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerCoin);
    }

   

    // Test
	EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({1,1}, map->GetTilePos({ 1,1 }), ItemType::GOLD, 50));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 1,2 }, map->GetTilePos({ 1,2 }), ItemType::GOLD, 100));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 1,2 }, map->GetTilePos({ 1,2 }), ItemType::BOMB, 3));
    // EventManager::GetInstance()->AddEvent(EventType::SPAWNWEAPON, new  );

    return S_OK;
}

void LobbyScene::Release()
{
	if (uiManager)
	{
		uiManager->Release();
		delete uiManager;
		uiManager = nullptr;
	}

    if (map) {
        map->Release();
        map = nullptr;
    }

    playerManager.lock()->BindRelease();

    DeleteObject(blackBrush);

}

void LobbyScene::Update()
{
    Camera::GetInstance()->Update();

	if (uiManager)
	{
		uiManager->Update();
	}
	playerManager.lock()->Update();
	
}

void LobbyScene::Render(HDC hdc)
{
    FPOINT pos = playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1).lock()->GetPos();
    FPOINT cPos = Camera::GetInstance()->GetPos();
    // 검은색 배경으로 초기화    
    RenderFillRectAtCenter(hdc, blackBrush,
        pos.x - cPos.x, pos.y - cPos.y,
        SCENE_WIDTH, SCENE_HEIGHT);

    if (map)
    {
        map->Render(hdc);
    }
    
	if (positionManager)
	{
		positionManager->Render(hdc);
	}

	if (uiManager)
	{
		uiManager->Render(hdc);
	}
	playerManager.lock()->Render(hdc);
	

}

void LobbyScene::SetPlayerManager(shared_ptr<PlayerManager> playerManager)
{
    this->playerManager = playerManager;
    

    if (positionManager)
    {
		this->playerManager.lock()->SetPositionManager(positionManager);
    }
    if (nullptr != map)
    {
        this->playerManager.lock()->SetTileMap(map);
    }
}



