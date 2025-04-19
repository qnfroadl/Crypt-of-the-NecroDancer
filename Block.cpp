#include "Block.h"
#include "Camera.h"
#include "Image.h"
#include "ImageManager.h"

#define TILE_SIZE_SCALED (TILE_SIZE * TILE_SCALE)

HRESULT Block::Init()
{
	blocklImage = ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	return S_OK;
}

HRESULT Block::Init(FPOINT _pos, POINT _index)
{
	blocklImage= ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	pos = _pos;
	index = _index;
	return S_OK;
}

void Block::Render(HDC hdc, bool useCamera)
{
	if (!blocklImage) return;

	float drawX = pos.x * TILE_SIZE;
	float drawY = pos.y * TILE_SIZE;

	if (useCamera)
	{
		drawX *= TILE_SCALE;
		drawY *= TILE_SCALE;
		drawX -= Camera::GetInstance()->GetPos().x;
		drawY -= Camera::GetInstance()->GetPos().y;

		int centerX = static_cast<int>(drawX + TILE_SIZE_SCALED / 2);
		int centerY = static_cast<int>(drawY + TILE_SIZE_SCALED / 2);

		blocklImage->RenderScaledImage(
			hdc,
			centerX,
			centerY,
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			static_cast<float>(TILE_SCALE),
			static_cast<float>(TILE_SCALE),
			true
		);
	}
	else
	{
		int centerX = static_cast<int>(drawX + TILE_SIZE / 2);
		int centerY = static_cast<int>(drawY + TILE_SIZE / 2);

		blocklImage->FrameRender(
			hdc,
			centerX,
			centerY,
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			false, true
		);
	}
}
bool Block::Destroy(Item* item)
{
	// ¾ÆÁ÷
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
