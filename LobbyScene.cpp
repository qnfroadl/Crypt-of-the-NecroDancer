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

#include "BeatManager.h"
#include "SoundManager.h"

HRESULT LobbyScene::Init()
{
    SoundManager::GetInstance()->LoadSound();

    // InitMap
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);

   
   //map = make_shared<Tilemap>();
    map = make_shared<Tilemap>(*(TilemapGenerator::GetInstance()->Generate("LOBBY")));
    map->PrintTorchSpots();
    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

	positionManager = make_shared<PositionManager>();
    positionManager->Init();
	itemSpawner = make_shared<ItemSpawner>();
    itemSpawner->Init();
	itemSpawner->SetPositionManager(positionManager);
    itemSpawner->SetTileMap(map);

    uiManager = make_shared<UIManager>();
	uiManager->Init();

	PlayerWallet* playerWallet = new PlayerWallet();
	playerWallet->Init();
	uiManager->AddUI(playerWallet);

	PlayerHp* playerHp = new PlayerHp();
	playerHp->Init();
	uiManager->AddUI(playerHp);

    shadowCasting = make_shared<ShadowCasting>();
    shadowCasting->Init(map->GetTiles());

    int playerCount = 1;
    if (playerManager.lock())
    {
        playerManager.lock()->SetPositionManager(positionManager);
        playerManager.lock()->SetTileMap(map);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerWallet);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerHp);
        shadowCasting->AddPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
		playerCount = playerManager.lock()->GetPlayerCount();
    }


    SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::LOBBY);
    beatManager = make_shared<BeatManager>();
    beatManager->Init();
    beatManager->StartBeat(false);
    beatManager->SetActive2P(playerCount == 1 ? false : true);

    renderer = make_unique<TileActorRenderer>();
    renderer->Init();
    renderer->SetPositionManager(positionManager);
    renderer->SetTileMap(map);

    shadowCasting->Update();

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
		uiManager = nullptr;
	}
	if (itemSpawner)
	{
		itemSpawner->Release();
		itemSpawner = nullptr;
	}
    if (map) {
        map->Release();
        map = nullptr;
    }

    if (positionManager)
    {
        positionManager->Release();
        positionManager = nullptr;
    }
    

	if (beatManager)
	{
		beatManager->Release();
		beatManager = nullptr;
	}

    

    playerManager.lock()->BindRelease();

    DeleteObject(blackBrush);

}

void LobbyScene::Update()
{
    if (map)
    {
		map->Update();
    }
	if (uiManager)
	{
		uiManager->Update();
	}

    positionManager->Update();
	// playerManager.lock()->Update();

    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F3))
    {
        SoundManager::GetInstance()->SetTempo(0.8f);
    }
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F4))
    {
        SoundManager::GetInstance()->SetTempo(1.2f);
    }

    if (beatManager)
    {
		beatManager->Update();
    }
}

void LobbyScene::Render(HDC hdc)
{
    FPOINT pos = playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1).lock()->GetPos();
    FPOINT cPos = Camera::GetInstance()->GetPos();
    
    // 검은색 배경으로 초기화    (플레이어의 스크린보스를 중심으로)
    RenderFillRectAtCenter(hdc, blackBrush, pos.x - cPos.x, pos.y - cPos.y, SCENE_WIDTH, SCENE_HEIGHT);

    // 타일, 액터들 렌더링.
    if (renderer)
    {
        renderer->Render(hdc);
    }

    if (beatManager)
    {
        beatManager->Render(hdc);
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