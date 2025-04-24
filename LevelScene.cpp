#include "LevelScene.h"
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

#include "SoundManager.h"
#include "BeatManager.h"

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
    itemSpawner = make_shared<ItemSpawner>();
    itemSpawner->Init();
    itemSpawner->SetPositionManager(positionManager);

    uiManager = make_shared<UIManager>();
    uiManager->Init();

    PlayerWallet* playerCoin = new PlayerWallet();
    playerCoin->Init();
    uiManager->AddUI(playerCoin);

    PlayerHp* playerHp = new PlayerHp();
    playerHp->Init();
    playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerHp);
    uiManager->AddUI(playerHp);

    shadowCasting = make_shared<ShadowCasting>();
    shadowCasting->Init(map->GetTiles());

    if (playerManager.lock())
    {
        playerManager.lock()->SetPositionManager(positionManager);
        playerManager.lock()->SetTileMap(map);
        playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerCoin);
        shadowCasting->AddPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
    }

    if (monsterManager.lock())
    {
        monsterManager.lock()->SetPositionManager(positionManager);
        monsterManager.lock()->SetTileMap(map);
        monsterManager.lock()->SetPlayer(playerManager.lock()->GetPlayer(PlayerIndex::PLAYER1));
    }


    SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::ZONE1_1, ESoundKey::ZONE1_1_SHOPKEEPER_M);
	beatManager = make_shared<BeatManager>();
    beatManager->Init();
    beatManager->StartBeat(true);

    return S_OK;
}

void LevelScene::Release()
{
    if (uiManager)
    {
        uiManager->Release();
        uiManager = nullptr;
    }

    if (map) {
        map->Release();
        map = nullptr;
    }

    if (beatManager)
    {
        beatManager->Release();
        beatManager = nullptr;
    }

	playerManager.lock()->BindRelease();
    DeleteObject(blackBrush);

}

void LevelScene::Update()
{
    map->UpdateVisuable();
    if (uiManager)
    {
        uiManager->Update();
    }
    playerManager.lock()->Update();
    monsterManager.lock()->Update();

    // test (actually update when (playermoved, blockdestroyed event) occurs)
    if (KeyManager::GetInstance()->IsOnceKeyDown('L'))
    {
        shadowCasting->Update();
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

    if (map)
    {
        map->Render(hdc);
    }

    if (positionManager)
    {
        positionManager->Render(hdc);
    }

    if (beatManager)
    {
        beatManager->Render(hdc);
    }

    if (uiManager)
    {
        uiManager->Render(hdc);
    }
	playerManager.lock()->Render(hdc);
	monsterManager.lock()->Render(hdc);

    // test render
    shadowCasting->Render(hdc);
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

void LevelScene::SetMonsterManager(weak_ptr<MonsterManager> monsterManager)
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

