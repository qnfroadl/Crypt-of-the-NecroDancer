#include "Block.h"
#include "Camera.h"
#include "Image.h"
#include "ImageManager.h"

HRESULT Block::Init()
{
	blocklImage= ImageManager::GetInstance()->AddImage("WALL", TEXT("Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	return S_OK;
}

void Block::Render(HDC hdc, FPOINT center)
{
	if (!blocklImage) return;

	blocklImage->FrameRender(
		hdc,
		center.x,
		center.y,
		blockNum % SAMPLE_WALL_X,
		blockNum / SAMPLE_WALL_X,
		false, true // �߽� ����
	);
}

bool Block::Destroy(Item* item)
{
    return false;
}

bool Block::Destroy(int strength)
{
	if (strength >= hp)
	{
		// TODO. �μ���
	}
    return false;
}

BlockType Block::GetTypeByBlockNum(int WallNum)
{
	//���� ����
	if (blockNum >= 0 && blockNum <= 15) return BlockType::NORMAL;
}
