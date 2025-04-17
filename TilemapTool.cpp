#include "TilemapTool.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"
#include "KeyManager.h"
#include <fstream>
#include <sstream>
#include "Tile.h"
#include "Block.h"

// 외부 또는 멤버 변수
SelectedLayer selectedLayer = SelectedLayer::FLOOR;
int selectedTileLX = 0;
int selectedTileLY = 0;

TilemapTool::TilemapTool()
	: sampleTile(nullptr),
	sampleWall(nullptr)
{
}

TilemapTool::~TilemapTool()
{
}

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);

	sampleTile = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"), 234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
	sampleWall = ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 384, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));

	rcSampleTile = { TILEMAPTOOL_X - sampleTile->GetWidth(), 0, TILEMAPTOOL_X, sampleTile->GetHeight() };
	rcSampleWall = { TILEMAPTOOL_X - sampleWall->GetWidth(), sampleTile->GetHeight() + 10, TILEMAPTOOL_X, sampleTile->GetHeight() + 10 + sampleWall->GetHeight() };
	rcMapTile = { 0, 0, TILE_X * TILE_SIZE, TILE_Y * TILE_SIZE };

	tiles = vector<vector<Tile*>>(TILE_Y, vector<Tile*>(TILE_X, nullptr));

	for (int i = 0; i < TILE_X; i++)
	{
		for (int j = 0; j < TILE_Y; j++)
		{
			tiles[j][i] = new Tile();
			tiles[j][i]->SetTileNum(0);

			RECT rc = { i * TILE_SIZE, j * TILE_SIZE, i * TILE_SIZE + TILE_SIZE, j * TILE_SIZE + TILE_SIZE };
			tiles[j][i]->SetRcTile(rc);
		}
	}
	return S_OK;
}

void TilemapTool::Release()
{
	for (int j = 0; j < TILE_Y; j++)
	{
		for (int i = 0; i < TILE_X; i++)
		{
			delete tiles[j][i];
			tiles[j][i] = nullptr;
		}
	}
	tiles.clear();
}

void TilemapTool::Update()
{
	// 바닥 타일 샘플 선택
	if (PtInRect(&rcSampleTile, g_ptMouse))
	{
		bool leftClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON);
		bool rightClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON);

		if (leftClick || rightClick) {
			int x = g_ptMouse.x - rcSampleTile.left;
			int y = g_ptMouse.y - rcSampleTile.top;

			int tileX = x / TILE_SIZE;
			int tileY = y / TILE_SIZE;

			selectedLayer = SelectedLayer::FLOOR;

			if (leftClick) {
				selectedTileLX = tileX;
				selectedTileLY = tileY;
			}
			else if (rightClick) {
				selectedTileRX = tileX;
				selectedTileRY = tileY;
			}
		}
	}
	// 벽 타일 샘플 선택
	else if (PtInRect(&rcSampleWall, g_ptMouse))
	{
		bool leftClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON);
		bool rightClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON);

		if (leftClick || rightClick) {
			int x = g_ptMouse.x - rcSampleWall.left;
			int y = g_ptMouse.y - rcSampleWall.top;

			int tileX = x / WALL_TILE_WIDTH;
			int tileY = y / WALL_TILE_HEIGHT;

			selectedLayer = SelectedLayer::WALL;

			if (leftClick) {
				selectedTileLX = tileX;
				selectedTileLY = tileY;
			}
			else if (rightClick) {
				selectedTileRX = tileX;
				selectedTileRY = tileY;
			}
		}
	}
	// 맵에 배치
	else if (PtInRect(&rcMapTile, g_ptMouse)) {
		bool leftClick = KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON);
		bool rightClick = KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON);

		if (leftClick || rightClick) {
			int x = g_ptMouse.x / TILE_SIZE;
			int y = g_ptMouse.y / TILE_SIZE;

			if (x >= TILE_X || y >= TILE_Y) return;

			Tile* tile = tiles[y][x];

			if (selectedLayer == SelectedLayer::FLOOR) {
				int tileNum = (leftClick ?
					selectedTileLX + selectedTileLY * SAMPLE_TILE_X :
					selectedTileRX + selectedTileRY * SAMPLE_TILE_X);
				tile->SetTileNum(tileNum);
				tile->SetType(TileType::NORMAL);
			}
			else if (selectedLayer == SelectedLayer::WALL) {
				int tileNum = (leftClick ?
					selectedTileLX + selectedTileLY * SAMPLE_WALL_X :
					selectedTileRX + selectedTileRY * SAMPLE_WALL_X);
				Block* block = new Block();
				block->SetTileNum(tileNum);
				tile->SetBlock(block);
			}
		}
	}
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);

	// 바닥 먼저
	for (int i = 0; i < TILE_X; i++) {
		for (int j = 0; j < TILE_Y; j++) {
			Tile* tile = tiles[j][i];
			RECT rc = tile->GetRcTile();

			int centerX = rc.left + TILE_SIZE / 2;
			int centerY = rc.top + TILE_SIZE / 2;

			int tileNum = tile->GetTileNum();
			sampleTile->FrameRender(hdc, centerX, centerY,
				tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
				false, true);
		}
	}

	// 그 다음 벽
	for (int i = 0; i < TILE_X; i++) {
		for (int j = 0; j < TILE_Y; j++) {
			Tile* tile = tiles[j][i];
			RECT rc = tile->GetRcTile();

			int centerX = rc.left + TILE_SIZE / 2;
			int centerY = rc.top + TILE_SIZE / 2;

			Block* block = tile->GetBlock();
			if (block) {
				int wallNum = block->GetTileNum();
				sampleWall->FrameRender(hdc, centerX, centerY,
					wallNum % SAMPLE_WALL_X, wallNum / SAMPLE_WALL_X,
					false, true);
			}
		}
	}

	// 샘플 이미지 그리기
	sampleTile->Render(hdc, rcSampleTile.left, rcSampleTile.top);
	sampleWall->Render(hdc, rcSampleWall.left, rcSampleWall.top);

	// 선택된 미리보기
	int previewX = TILEMAPTOOL_X - sampleTile->GetWidth() + TILE_SIZE / 2;
	int previewY = rcSampleWall.bottom + 50;

	if (selectedLayer == SelectedLayer::FLOOR)
		sampleTile->FrameRender(hdc, previewX, previewY, selectedTileLX, selectedTileLY, false, true);
	else if (selectedLayer == SelectedLayer::WALL)
		sampleWall->FrameRender(hdc, previewX, previewY, selectedTileLX, selectedTileLY, false, true);
}