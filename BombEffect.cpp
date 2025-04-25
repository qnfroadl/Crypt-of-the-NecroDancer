#include "BombEffect.h"

#include "ImageManager.h"

BombEffect::BombEffect(FPOINT pos)
{
    SetPos(pos);
    duration = 0.3f;

    image = ImageManager::GetInstance()->AddImage("bomb_explosion", L"Image/Player/item/3x3_explosion.bmp", 592 * 3, 74 * 3, 8, 1, true, RGB(255, 0, 255));
}

void BombEffect::Update()
{
    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    elapsed += deltaTime;
    frame = static_cast<int>((elapsed / duration) * 8); // 6프레임 애니메이션
}

void BombEffect::Render(HDC hdc)
{
    FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());

    image->FrameRender(hdc, pos.x, pos.y, frame, 0, false, true);
    // ex: ImageManager::Get()->Find("explosion")->RenderFrame(hdc, position, frame);
}