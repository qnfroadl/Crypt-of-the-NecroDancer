#include "LobbyScene.h"
#include "Tilemap.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "PlayerManager.h"
#include "Player.h"
#include "UIManager.h"
#include "PlayerWallet.h"

HRESULT LobbyScene::Init()
{
    // InitMap
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);
    map = make_shared<Tilemap>();
    map->Init(20, 20);
    map->Load("map/ZONE1_01.map");
    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

    if (playerManager.lock())
    {
        playerManager.lock()->SetTileMap(map);
    }

    uiManager = new UIManager();
	uiManager->Init();

	PlayerWallet* playerCoin = new PlayerWallet();
	playerCoin->Init();
	playerManager.lock()->BindPlayerObserver(PlayerIndex::PLAYER1, playerCoin);
	uiManager->AddUI(playerCoin);

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
    
	if (uiManager)
	{
		uiManager->Render(hdc);
	}
}

void LobbyScene::SetPlayerManager(shared_ptr<PlayerManager> playerManager)
{
    this->playerManager = playerManager;
    if (nullptr != map)
    {
        this->playerManager.lock()->SetTileMap(map);
    }
}

