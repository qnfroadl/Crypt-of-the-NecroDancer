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
HRESULT LobbyScene::Init()
{
    // InitMap
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);
    map = make_shared<Tilemap>();
    map->Init(20, 20);
    map->Load("map/ZONE1_01.map");
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

    if (monsterManager.lock())
    {
        monsterManager.lock()->SetPositionManager(positionManager);
        monsterManager.lock()->SetTileMap(map);
		monsterManager.lock()->SetPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
    }

    // Test
   

	EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({3,3}, map->GetTilePos({ 3,3 }), ItemType::GOLD, 1));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 4,4 }, map->GetTilePos({ 4,4 }), ItemType::GOLD, 2));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 5,5 }, map->GetTilePos({ 5,5 }), ItemType::GOLD, 3));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 6,6 }, map->GetTilePos({ 6,6 }), ItemType::GOLD, 4));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 7,7 }, map->GetTilePos({ 7,7 }), ItemType::GOLD, 10));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 9,9 }, map->GetTilePos({ 9,9 }), ItemType::GOLD, 10));

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

    DeleteObject(blackBrush);

}

void LobbyScene::Update()
{
    Camera::GetInstance()->Update();

	if (uiManager)
	{
		uiManager->Update();
	}
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

void LobbyScene::SetMonsterManager(weak_ptr<MonsterManager> monsterManager)
{
	this->monsterManager = monsterManager;
	if (positionManager)
	{
		this->monsterManager.lock()->SetPositionManager(positionManager);
	}
	if (nullptr != map)
	{
		this->monsterManager.lock()->SetTileMap(map);
	}
}

