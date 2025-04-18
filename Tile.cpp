#include "Tile.h"
//#include "Trap.h"
#include "Image.h"
#include "Camera.h"
#include "ImageManager.h"
#include "Block.h"
#include "Trap.h"

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

	FPOINT center = {
		rcTile.left + TILE_SIZE / 2.0f,
		rcTile.top + TILE_SIZE / 2.0f
	};

	// 게임일 경우 카메라 좌표 보정
	if (useCamera)
	{
		center.x -= Camera::GetInstance()->GetPos().x;
		center.y -= Camera::GetInstance()->GetPos().y;
	}

	// 바닥
	tileImage->FrameRender(hdc,
		static_cast<int>(center.x), static_cast<int>(center.y),
		tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
		false, true);

	//// 트랩
	//if (trap)
	//	trap->Render(hdc, center);

	// 벽
	if (block)
		block->Render(hdc, center);
}

void Tile::OnTile(TileActor* actor)
{
	//if (trap)
	//	trap->ontriggered(actor);
}

TileType Tile::GetTypeByTileNum(int tileNum)
{
	// 수정해야 함
	if (tileNum >= 0 && tileNum <= 15) return TileType::NORMAL;
	else if (tileNum >= 16 && tileNum <= 31) return TileType::NONE;
	else return TileType::NONE;
}
