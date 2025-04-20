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
			true  // �߽� ����
		);
	}
	else
	{
		tileImage->FrameRender(
			hdc,
			static_cast<int>(renderX / TILE_SCALE),
			static_cast<int>(renderY / TILE_SCALE),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			false, true  // �߽� ���� ������
		);
	}

	// Ʈ��
	if (trap)
		trap->Render(hdc, useCamera);

	// ��
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
	// �����ؾ� ��
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

void Tile::OnBeat(bool isCombo)
{
	if (isCombo)
	{
		// ���� -> ��ο� -> �޺�(¦�� �ʷ�, Ȧ�� ��ũ)
		switch (type)
		{
		case TileType::BRIGHT_DIRT:
			SetTileNum(0); // ��ο� ������ ����
			break;
		case TileType::DARK_DIRT:
			if ((index.x + index.y) % 2 == 0)
			{
				// ¦���� ��
				SetTileNum(10); // �޺�1�� ����
			}
			else
			{
				// Ȧ���� ��
				SetTileNum(11); // �޺�2�� ����
			}
			break;
		case TileType::COMBO1_DIRT:
		case TileType::COMBO2_DIRT:
			SetTileNum(1); // ���� ������ ����
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
			SetTileNum(0); //��ο�ŷ� �ٲ����
			break;
		case TileType::DARK_DIRT:
			SetTileNum(1); //�����ŷ� �ٲ����
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
