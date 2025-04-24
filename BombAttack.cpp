#include "BombAttack.h"

#include "ImageManager.h"
#include "EventManager.h"
#include "TimerManager.h"
#include "Camera.h"
#include "EffectManager.h"
#include "BombEffect.h"


HRESULT BombAttack::Init()
{
	image = ImageManager::GetInstance()->AddImage("bomb_start", L"Image/Player/item/bomb.bmp", 120 * 3, 48 * 3, 5, 2, true, RGB(255, 0, 255));
	

	// 설치되고 3비트면 터짐.

	// 
	EventManager::GetInstance()->BindEvent(this, EventType::BEAT, bind(&BombAttack::OnNextStep, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATHIT, bind(&BombAttack::OnNextStep, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATMISS, bind(&BombAttack::OnNextStep, this));

	damage = 4;
	step = 0;		// 폭발 단계.
	isExplode = false;

	return S_OK;
}

void BombAttack::Update()
{
	if (isExplode)
	{
		
	}
	else if (5 <= step)
	{
		isExplode = true;
		EffectManager::GetInstance()->AddEffect(make_shared<BombEffect>(GetPos()));
		// 상호작용도 요청 해야하는데?

	}
}

void BombAttack::Render(HDC hdc)
{
	if (SightState::INVISIBLE == sightState)
	{
		return;
	}

	FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());

	if (false == isExplode)
	{
		image->FrameRender(hdc, pos.x, pos.y, step, SightState::SHADOW == sightState ? 1 : 0, false, true);
	}
	
}

void BombAttack::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEAT);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATHIT);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATMISS);
}

void BombAttack::OnNextStep()
{
	// 다음 단계.
	step++;

}

void BombAttack::Interact(GameActor* actor)
{
	if (IsActive() )
	{
		


	}
}
