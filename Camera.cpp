#include "Camera.h"

#include "CommonFunction.h"
#include "Player.h"
#include "TimerManager.h"

Camera::Camera()
{

}
Camera::Camera(int width, int height)
    :viewSize{width, height}
{

}

Camera::~Camera()
{

}

void Camera::SetTarget(weak_ptr<Player> _target)
{
    wptTarget = _target;
}

void Camera::SetSize(SIZE size)
{
    viewSize = size;
}

SIZE Camera::GetSize()
{
    return viewSize;
}

void Camera::Shake(float duration, int intensity)
{
   shakeDuration = duration;
   shakeIntensity = intensity;
}

void Camera::Update()
{
    // 이걸 매 프레임 하는것 보단, 타겟의 위치가 변할때만 따라가는게 좋긴해. Observer.
    if (auto lockedTarget = wptTarget.lock())
    {
        // 타겟이 항상 카메라의 중앙에 오도록.
        float x = lockedTarget->GetPos().x - viewSize.cx / 2;
        float y = lockedTarget->GetPos().y - viewSize.cy / 2;

        if (shakeDuration > 0.0f)
        {
            shakeDuration -= TimerManager::GetInstance()->GetDeltaTime();

            float offsetX = randOffset(randEngine) * shakeIntensity;
            float offsetY = randOffset(randEngine) * shakeIntensity;

            x += offsetX;
            y += offsetY;
        }

        SetPos(x,y);
    }
}

RECT Camera::GetViewRect()
{
	return GetRectAtCenter(GetPos().x, GetPos().y, viewSize.cx, viewSize.cy);
}

FPOINT Camera::GetScreenPos(const FPOINT& worldPos)
{
    FPOINT screenPos;

    screenPos.x = worldPos.x - GetPos().x;
    screenPos.y = worldPos.y - GetPos().y;

    return screenPos;
}
