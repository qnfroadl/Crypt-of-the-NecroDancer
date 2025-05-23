#include "UIBase.h"
#include "Image.h"
#include "ImageManager.h"

UIBase::UIBase()
	: image{}, position{}, size{ 1.f, 1.f }
{
}

UIBase::~UIBase()
{
}

HRESULT UIBase::Init()
{
	return S_OK;
}

void UIBase::Release()
{
}

void UIBase::Update()
{
}

void UIBase::Render(HDC hdc)
{
	if (image)
	{
		image->FrameRender(hdc, position.x, position.y, 0, 0);
	}
}
