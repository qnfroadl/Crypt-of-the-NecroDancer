#include "LobbyScene.h"
#include "Tilemap.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "PlayerManager.h"
#include "Player.h"
#include "TilemapGenerator.h"
#include "Block.h"
#include "Tile.h"
HRESULT LobbyScene::Init()
{
    // InitMap
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);
    //map = make_shared<Tilemap>();
    //map->Init(20, 20);
    //map->Load("map/ZONE1_01.map");
    blackBrush = CreateSolidBrush(RGB(0, 0, 0));


    map = make_shared<Tilemap>(*(TilemapGenerator::GetInstance()->Generate("ZONE1", 30, 30)));

 //   // 디버깅용 맵 출력
	//for (int y = 0; y < 40; ++y) {
	//	for (int x = 0; x < 50; ++x) {
	//		Tile* tile = map->GetTile(y, x);
	//		if (!tile) { cout << ' '; continue; }

	//		Block* block = tile->GetBlock();
	//		if (block) {
	//			int num = block->GetBlockNum();
	//			if (num == 61) cout << '|'; // 가로 문
	//			else if (num == 62) cout << '-'; // 세로 문
	//			else cout << '#'; // 일반 벽
	//		}
	//		else {
	//			cout << '.'; // 바닥
	//		}
	//	}
	//	cout << '\n';
	//}
    playerManager.lock()->SetTileMap(map);
	return S_OK;
}

void LobbyScene::Release()
{
    if (map) {
        map->Release();
        map = nullptr;
    }

    DeleteObject(blackBrush);

}

void LobbyScene::Update()
{
    Camera::GetInstance()->Update();
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
    
}

void LobbyScene::SetPlayerManager(shared_ptr<PlayerManager> playerManager)
{
    this->playerManager = playerManager;
}

