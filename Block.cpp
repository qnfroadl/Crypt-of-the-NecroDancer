#include "Block.h"
#include "Camera.h"
#include "Image.h"
#include "ImageManager.h"
#include "Torch.h"

#define TILE_SIZE_SCALED (TILE_SIZE * TILE_SCALE)

HRESULT Block::Init()
{
	blocklImage = ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	torch = nullptr;
	return S_OK;
}

HRESULT Block::Init(FPOINT _pos, POINT _index)
{
	blocklImage= ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	pos = _pos;
	index = _index;
	torch = nullptr;
	return S_OK;
}

void Block::Render(HDC hdc, bool useCamera)
{
	if (!blocklImage) return;

	float centerX = pos.x;
	float centerY = pos.y;

	if (useCamera)
	{
		centerX -= Camera::GetInstance()->GetPos().x;
		centerY -= Camera::GetInstance()->GetPos().y;

		blocklImage->FrameRender(
			hdc,
			static_cast<int>(centerX),
			static_cast<int>(centerY),
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			static_cast<float>(TILE_SCALE),
			static_cast<float>(TILE_SCALE),
			false, true  // 중심 기준 
		);
	}
	else
	{
		blocklImage->FrameRender(
			hdc,
			static_cast<int>(centerX / TILE_SCALE),
			static_cast<int>(centerY / TILE_SCALE),
			blockNum % SAMPLE_WALL_X,
			blockNum / SAMPLE_WALL_X,
			false, true  // 중심 기준
		);
	}
	if (torch)
		torch->Render(hdc, useCamera);
}
void Block::Update()
{
	if (torch)
		torch->Update();
}
void Block::Destroy()
{
	// 삽 아이콘 띄우면서 없어져버려.
	


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
	else
	{
		type = BlockType::NONE;
	}

	maxHP = hp;
}

vector<shared_ptr<TileActor>> Block::GetRendableTileActors()
{
	vector<shared_ptr<TileActor>> result;
	result.push_back(shared_from_this());
	return result;
}