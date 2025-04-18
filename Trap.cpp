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
	// 트랩 이미지 리소스 작성 후 추가
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
	// 효과 발동 처리
}