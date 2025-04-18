#include "LobbyScene.h"
#include "Tilemap.h"
#include "Camera.h"

HRESULT LobbyScene::Init()
{
    map = new Tilemap();
    map->Init(20, 20);
    map->Load("Map/ZONE_01.map");

    return S_OK;
}

void LobbyScene::Release()
{
    if (map) {
        map->Release();
        delete map;
        map = nullptr;
    }
}

void LobbyScene::Update()
{
    Camera::GetInstance()->Update();
}

void LobbyScene::Render(HDC hdc)
{
    // 검은색 배경으로 초기화
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    RECT clientRect = { 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y };
    FillRect(hdc, &clientRect, blackBrush);
    DeleteObject(blackBrush);

    if (map)
        map->Render(hdc);
}

