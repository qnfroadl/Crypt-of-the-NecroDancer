#include "Tile.h"
//#include "Trap.h"
#include "Image.h"
#include "Camera.h"
#include "ImageManager.h"
#include "Block.h"
#include "Trap.h"


#define TILE_SIZE_SCALED (TILE_SIZE * TILE_SCALE)

HRESULT Tile::Init()
{
	tileImage = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"), 234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
	type = TileType::NONE;
	tileNum = 0;
	block = nullptr;
	trap = nullptr;
	light = 0.0f;
	//	rcTile = { 0, 0, 0, 0 };
	return S_OK;
}

HRESULT Tile::Init(int x, int y)
{
	tileImage = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"), 234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
	type = TileType::NONE;
	tileNum = 0;
	block = nullptr;
	trap = nullptr;
	light = 0.0f;

	//// 타일 위치 설정 (타일 좌표 기준)
	//rcTile.left = x * TILE_SIZE;
	//rcTile.top = y * TILE_SIZE;
	//rcTile.right = rcTile.left + TILE_SIZE;
	//rcTile.bottom = rcTile.top + TILE_SIZE;
	pos = {
		(x + 0.5f) * TILE_SIZE * TILE_SCALE,
		(y + 0.5f) * TILE_SIZE * TILE_SCALE
	};
	index = { x, y };
	return S_OK;
}

void Tile::Release()
{
	if (block)
	{
		delete block;
		block = nullptr;
	}
	if (trap)
	{
		delete trap;
		trap = nullptr;
	}
}

void Tile::Render(HDC hdc, bool useCamera)
{
	if (!tileImage) return;

	float renderX = pos.x;
	float renderY = pos.y;

	if (useCamera)
	{
		renderX -= Camera::GetInstance()->GetPos().x;
		renderY -= Camera::GetInstance()->GetPos().y;

		tileImage->RenderScaledImage(
			hdc,
			static_cast<int>(renderX),
			static_cast<int>(renderY),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			static_cast<float>(TILE_SCALE), static_cast<float>(TILE_SCALE),
			true  // 중심 기준
		);
	}
	else
	{
		tileImage->FrameRender(
			hdc,
			static_cast<int>(renderX / TILE_SCALE),
			static_cast<int>(renderY / TILE_SCALE),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			false, true  // 중심 기준 렌더링
		);
	}

	// 트랩
	if (trap)
		trap->Render(hdc, useCamera);

	// 벽
	if (block)
		block->Render(hdc, useCamera);
}
void Tile::OnTile(TileActor* actor)
{
	if (trap)
		trap->OnTriggered(actor);
}

TileType Tile::GetTypeByTileNum(int tileNum)
{
	// 수정해야 함
	if (tileNum >= 0 && tileNum <= 15) return TileType::NORMAL;
	else if (tileNum >= 16 && tileNum <= 31) return TileType::NONE;
	else return TileType::NONE;
}
