#include "Tile.h"
//#include "Trap.h"
#include "Image.h"
#include "Camera.h"
#include "ImageManager.h"
#include "Block.h"
#include "Trap.h"
#include "CommonFunction.h"

#define TILE_SIZE_SCALED (TILE_SIZE * TILE_SCALE)

HRESULT Tile::Init()
{
	tileImage = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"), 234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
	type = TileType::NONE;
	tileNum = -1;
	block = nullptr;
	trap = nullptr;
	light = 0.0f;
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

	pos = {
		x * TILE_SIZE * TILE_SCALE + (TILE_SIZE * TILE_SCALE) / 2.0f,
		y * TILE_SIZE * TILE_SCALE + (TILE_SIZE * TILE_SCALE) / 2.0f
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
	/*if (tileNum >= 0 && tileNum <= 15) return TileType::NORMAL;
	else if (tileNum >= 16 && tileNum <= 31) return TileType::NONE;
	else return TileType::NONE;*/
	if (tileNum == 0) return TileType::DARK_DIRT;
	else if (tileNum == 1) return TileType::BRIGHT_DIRT;
	//else if (tileNum == 2) return TileType::COMBO1_DIRT;
	//else if (tileNum == 3) return TileType::COMBO2_DIRT;
	else if (tileNum == 10) return TileType::COMBO1_DIRT;
	else if (tileNum == 11) return TileType::COMBO2_DIRT;
	else return TileType::NONE;
}

void Tile::SetBlock(Block* _block)
{
	if (_block == nullptr)
	{
		block = nullptr;
		return;
	}
	//if (block)
	//{
	//	delete block;
	//	block = nullptr;
	//}
	block = _block;
}

void Tile::Interact(GameActor* actor)
{
	

	// 나의 할일?
}

void Tile::OnBeat(bool isCombo)
{
	if (isCombo)
	{
		// 밝은 -> 어두운 -> 콤보(짝수 초록, 홀수 핑크)
		switch (type)
		{
		case TileType::BRIGHT_DIRT:
			SetTileNum(0); // 어두운 것으로 변경
			break;
		case TileType::DARK_DIRT:
			if ((index.x + index.y) % 2 == 0)
			{
				SetTileNum(10); // 콤보1로 변경
			}
			else
			{
				SetTileNum(11); // 콤보2로 변경
			}
			break;
		case TileType::COMBO1_DIRT:
		case TileType::COMBO2_DIRT:
			SetTileNum(1); // 밝은 것으로 변경
			break;
		case TileType::NONE:
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case TileType::BRIGHT_DIRT:
			SetTileNum(0); //어두운거로 바꿔야함
			break;
		case TileType::DARK_DIRT:
			SetTileNum(1); //밝은거로 바꿔야함
			break;
		case TileType::COMBO1_DIRT:
			SetTileNum(0);
			break;
		case TileType::COMBO2_DIRT:
			SetTileNum(1);
			break;
		case TileType::NONE:
		default:
			break;
		}
	}
}
