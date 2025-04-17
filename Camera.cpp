#include "Camera.h"

#include "CommonFunction.h"
#include "Player.h"

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

void Camera::Update()
{
    // 이걸 매 프레임 하는것 보단, 타겟의 위치가 변할때만 따라가는게 좋긴해. Observer.
    if (auto lockedTarget = wptTarget.lock())
    {
        // 타겟이 항상 카메라의 중앙에 오도록.
        const float x = lockedTarget->GetPos().x - viewSize.cx / 2;
        const float y = lockedTarget->GetPos().y - viewSize.cy / 2;

        SetPos(x,y);
    }
}

RECT Camera::GetViewRect()
{
	return GetRectAtCenter(GetPos().x, GetPos().y, viewSize.cx, viewSize.cy);
}
