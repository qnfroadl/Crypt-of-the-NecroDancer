#include "Button.h"

#include "ImageManager.h"
#include "KeyManager.h"
#include "CommonFunction.h"

HRESULT Button::Init()
{
    state = ButtonState::None;
    image = ImageManager::GetInstance()->AddImage("Button", TEXT("Image/button.bmp"), 122, 62, 1,2);
    if (nullptr == image)
    {
        return E_FAIL;
    }

    SetPos(700,500);
    this->rc = GetRect(GetPos().x, GetPos().y,122, 31);

    return S_OK;
}

void Button::Update()
{
    
    if (PtInRect(&rc, g_ptMouse))
    {
        if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
        {
            // func();
            state = ButtonState::Down;
           
        }
        else if (ButtonState::Down == state && KeyManager::GetInstance()->IsOnceKeyUp(VK_LBUTTON))
        {
            state = ButtonState::Up;
            Clicked();
        }
    }

}

void Button::Render(HDC hdc)
{
    int yIndex = 0;
    switch (state)
    {
        case ButtonState::None:
        case ButtonState::Up:
            yIndex = 1;
        break;

        case ButtonState::Down:
            yIndex = 0;
        break;

    }

    image->FrameRender(hdc, GetPos().x, GetPos().y, 0, yIndex, false, false);
}

void Button::Release()
{
}

void Button::Bind(function<void()> func)
{
    this->func = func;
}

void Button::Clicked()
{
    if (nullptr != func)
    {
        func();
    }
    
}
