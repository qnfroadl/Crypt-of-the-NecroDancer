#include "Trap.h"
#include "Image.h"
#include "ImageManager.h"

Trap::Trap()
	: type(TrapType::NONE), tileNum(0), trapImage(nullptr)
{
}

Trap::~Trap()
{
}

HRESULT Trap::Init()
{
	// Ʈ�� �̹��� ���ҽ� �ۼ� �� �߰�
	//trapImage = ImageManager::GetInstance()->AddImage();
	return S_OK;
}

void Trap::Render(HDC hdc, FPOINT center)
{
	if (!trapImage) return;

	/*trapImage->FrameRender(hdc,
		static_cast<int>(center.x),
		static_cast<int>(center.y),
		tileNum % TRAP_TILE_X,
		tileNum / TRAP_TILE_X,
		false, true);*/
}

void Trap::OnTriggered(TileActor* actor)
{
	// ȿ�� �ߵ� ó��
}