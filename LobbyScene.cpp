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

#include "ShadowCasting.h"
#include "KeyManager.h"

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
    itemSpawner->SetTileMap(map);

    uiManager = new UIManager();
	uiManager->Init();

	PlayerWallet* playerWallet = new PlayerWallet();
	playerWallet->Init();
	uiManager->AddUI(playerWallet);

	PlayerHp* playerHp = new PlayerHp();
	playerHp->Init();
	uiManager->AddUI(playerHp);

    shadowCasting = make_shared<ShadowCasting>();
    shadowCasting->Init(map->GetTiles());

    if (playerManager.lock())
    {
        playerManager.lock()->SetPositionManager(positionManager);
        playerManager.lock()->SetTileMap(map);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerWallet);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerHp);
        shadowCasting->AddPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
    }

   
    // Test
	EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({1,1}, ItemType::GOLD, 50));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 1,2 }, ItemType::GOLD, 100));
    EventManager::GetInstance()->AddEvent(EventType::SPAWNITEM, new SpawnItemEventData({ 1,3 }, ItemType::BOMB, 3));
    
    EventManager::GetInstance()->AddEvent(EventType::SPAWNWEAPON, 
        new SpawnWeaponEventdata({ 1,4 }, DamageType::NORMAL, WeaponMaterialType::NORMAL, WeaponType::BROADSWORD));

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

    // test (actually update when (playermoved, blockdestroyed event) occurs)
	if (KeyManager::GetInstance()->IsOnceKeyDown('L'))
	{
	    shadowCasting->Update();
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
	playerManager.lock()->Render(hdc);

    // test render
	shadowCasting->Render(hdc);
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



