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

	// Ÿ�� ��ġ ���� (Ÿ�� ��ǥ ����)
	rcTile.left = x * TILE_SIZE;
	rcTile.top = y * TILE_SIZE;
	rcTile.right = rcTile.left + TILE_SIZE;
	rcTile.bottom = rcTile.top + TILE_SIZE;

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

	RECT rc = rcTile;
	if (useCamera)
{
	rc.left *= TILE_SCALE;
	rc.top *= TILE_SCALE;
	rc.left -= static_cast<int>(Camera::GetInstance()->GetPos().x);
	rc.top -= static_cast<int>(Camera::GetInstance()->GetPos().y);

	// �߽� ��ǥ�� ����
	int centerX = rc.left + TILE_SIZE_SCALED / 2;
	int centerY = rc.top + TILE_SIZE_SCALED / 2;

	// �ٴ� (Ȯ�� ������, �߽� ����)
	tileImage->RenderScaledImage(hdc,
		centerX, centerY,
		tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
		static_cast<float>(TILE_SCALE), static_cast<float>(TILE_SCALE), true);
}
	else
	{
		// �ٴ� (���� ũ��)
		FPOINT center = {
			rc.left + TILE_SIZE / 2.0f,
			rc.top + TILE_SIZE / 2.0f
		};

		tileImage->FrameRender(hdc,
			static_cast<int>(center.x), static_cast<int>(center.y),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			false, true);
	}

	// �߽� ��ǥ ��� (Ʈ��, ��Ͽ�)
	FPOINT center = {
		static_cast<float>(rc.left + (useCamera ? TILE_SIZE_SCALED : TILE_SIZE) / 2),
		static_cast<float>(rc.top + (useCamera ? TILE_SIZE_SCALED : TILE_SIZE) / 2)
	};

	// Ʈ��
	if (trap)
		trap->Render(hdc, center, useCamera);

	// ��
	if (block)
		block->Render(hdc, center, useCamera);
}

void Tile::OnTile(TileActor* actor)
{
	if (trap)
		trap->OnTriggered(actor);
}

TileType Tile::GetTypeByTileNum(int tileNum)
{
	// �����ؾ� ��
	if (tileNum >= 0 && tileNum <= 15) return TileType::NORMAL;
	else if (tileNum >= 16 && tileNum <= 31) return TileType::NONE;
	else return TileType::NONE;
}
