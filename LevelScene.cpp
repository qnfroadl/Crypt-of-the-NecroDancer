#include "LevelScene.h"
#include "Tilemap.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "PlayerManager.h"
#include "Player.h"

#include "UIManager.h"
#include "PlayerWallet.h"
#include "PlayerHp.h"
#include "MultipleGoldUI.h"

#include "PositionManager.h"
#include "ItemSpawner.h"

#include "EventManager.h"
#include "EventData.h"

#include "MonsterManager.h"
#include "TilemapGenerator.h"

#include "ShadowCasting.h"
#include "KeyManager.h"

#include "SoundManager.h"
#include "BeatManager.h"
#include "TileActorRenderer.h"

HRESULT LevelScene::Init()
{
    // InitMap
    SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);
    //map = make_shared<Tilemap>();
    //map->Init(20, 20);
    //map->Load("map/ZONE1_01.map");
    map = make_shared<Tilemap>(*(TilemapGenerator::GetInstance()->Generate("ZONE1", 30, 30)));
    //map->PrintSpawnPoints();
    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

    positionManager = make_shared<PositionManager>();
    positionManager->Init();
    itemSpawner = make_shared<ItemSpawner>();
    itemSpawner->Init();
    itemSpawner->SetPositionManager(positionManager);
    itemSpawner->SetTileMap(map);

    uiManager = make_shared<UIManager>();
    uiManager->Init();

    PlayerWallet* playerCoin = new PlayerWallet();
    playerCoin->Init();
    uiManager->AddUI(playerCoin);

    PlayerHp* playerHp = new PlayerHp();
    playerHp->Init();
    uiManager->AddUI(playerHp);

    MultipleGoldUI* multipleGold = new MultipleGoldUI();
    multipleGold->Init();
    uiManager->AddUI(multipleGold);


    shadowCasting = make_shared<ShadowCasting>();
    shadowCasting->Init(map->GetTiles());

	int playerCount = 1;
    if (playerManager.lock())
    {
        playerManager.lock()->SetPositionManager(positionManager);
        playerManager.lock()->SetTileMap(map);
        shadowCasting->AddPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));

        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerCoin);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerHp);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, multipleGold);
        playerCount = playerManager.lock()->GetPlayerCount();
    }

    monsterManager = make_shared<MonsterManager>();
    monsterManager->Init();
    monsterManager->SetPositionManager(positionManager);
    monsterManager->SetTileMap(map);
    monsterManager->SetPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
    
    SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::ZONE1_1, ESoundKey::ZONE1_1_SHOPKEEPER_M);
	beatManager = make_shared<BeatManager>();
    beatManager->Init();
    beatManager->StartBeat(true);
    beatManager->SetActive2P(playerCount == 1 ? false : true);

    renderer = make_shared<TileActorRenderer>();
    renderer->Init();
    renderer->SetPositionManager(positionManager);
    renderer->SetTileMap(map);
    shadowCasting->Update();

    return S_OK;
}

void LevelScene::Release()
{
    if (uiManager)
    {
        uiManager->Release();
        uiManager = nullptr;
    }

    if (positionManager)
    {
        positionManager->Release();
        positionManager = nullptr;
    }

    if (map) {
        map->Release();
        map = nullptr;
    }
    if (itemSpawner)
    {
        itemSpawner->Release();
        itemSpawner = nullptr;
    }
    if (beatManager)
    {
        beatManager->Release();
        beatManager = nullptr;
    }

    if (renderer)
    {
        renderer->Release();
        renderer = nullptr;
    }

	playerManager.lock()->BindRelease();
    DeleteObject(blackBrush);

}

void LevelScene::Update()
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

void LevelScene::Render(HDC hdc)
{
    FPOINT pos = playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1).lock()->GetPos();
    FPOINT cPos = Camera::GetInstance()->GetPos();
    // 검은색 배경으로 초기화    
    RenderFillRectAtCenter(hdc, blackBrush,
        pos.x - cPos.x, pos.y - cPos.y,
        SCENE_WIDTH, SCENE_HEIGHT);

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

void LevelScene::SetPlayerManager(shared_ptr<PlayerManager> playerManager)
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
