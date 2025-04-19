#include "Block.h"
#include "Camera.h"
#include "Image.h"
#include "ImageManager.h"

#define TILE_SCALE 4

HRESULT Block::Init()
{
	blocklImage= ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	return S_OK;
}

void Block::Render(HDC hdc, FPOINT center, bool useCamera)
{
	if (!blocklImage) return;

	if (useCamera)
	{
		// �̹� �����ϰ� ī�޶� ���� �Ϸ�� center�� �״�� ���
		blocklImage->RenderScaledImage(
			hdc,
			static_cast<int>(center.x),
			static_cast<int>(center.y),
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			static_cast<float>(TILE_SCALE),
			static_cast<float>(TILE_SCALE),
			true  // �߽� ����
		);
	}
	else
	{
		// �������� ���� ũ�� ����
		blocklImage->FrameRender(
			hdc,
			static_cast<int>(center.x),
			static_cast<int>(center.y),
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			false, true
		);
	}
}

bool Block::Destroy(Item* item)
{
	// ����
    return false;
}

bool Block::Destroy(int strength)
{
	if (strength >= hp)
	{
		return true;
	}
    return false;
}

void Block::SetBlockNum(int _blockNum)
{
	blockNum = _blockNum;
	SetBlockByBlockNum(blockNum);
}

void Block::SetBlockByBlockNum(int _blockNum)
{
	if (_blockNum >= 0 && _blockNum <= 49)
	{
		type =BlockType::NORMAL;
		hp = 1;
	}
	else if (_blockNum >= 50 && _blockNum <= 53)
	{
		type = BlockType::STONE;
		hp = 2;
	}
	else if (_blockNum >= 54 && _blockNum <= 55)
	{
		type = BlockType::CATACOMB;
		hp = 3;
	}
	else if (_blockNum >= 56 && _blockNum <= 57)
	{
		type = BlockType::SHOP;
		hp = 4;
	}
	else if (_blockNum >= 58 && _blockNum <= 60)
	{
		type = BlockType::UNBREAKABLE;
		hp = INT_MAX;
	}
	else if (_blockNum >= 61 && _blockNum <= 62)
	{
		type = BlockType::DOOR;
		hp = 1;
	}
	else type = BlockType::NONE;
}
