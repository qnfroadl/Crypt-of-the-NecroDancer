#include "Player.h"

#include "ImageManager.h"
#include "Camera.h"

//test
#include "KeyManager.h"
#include "CommonFunction.h"
#include "TimerManager.h"

void Player::OnBeatHit(EventData* data)
{

}

void Player::OnBeatMiss(EventData* data)
{

}

void Player::AnimJump()
{
	// 점프 애니메이션 처리
	// jumpData.sx, jumpData.sy : 시작 위치
	// jumpData.ex, jumpData.ey : 도착 위치
	// jumpData.height : 점프 높이
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	jumpData.animCurtime += dt;

	float jupmY = sinf(3.141592f * dt) * 200;	//jump power 200
	if (jumpData.animCurtime <= jumpData.totalAnimTime / 2)
	{
		jumpData.height += jupmY;
	}
	else
	{
		jumpData.height -= jupmY;
	}

	jumpData.sx += (jumpData.ex - jumpData.sx) * dt * speed;
	jumpData.sy += (jumpData.ey - jumpData.sy) * dt * speed;


	// 점프 종료.
	if (jumpData.totalAnimTime <= jumpData.animCurtime)
	{
		state = PlayerState::IDLE;
		jumpData.sx = jumpData.ex;
		jumpData.sy = jumpData.ey;

		jumpData.height = 0;
		jumpData.animCurtime = 0;
	}
	
	SetPos(jumpData.sx, jumpData.sy);
}

Player::Player()
	: state{}, hp(100), maxHP(100), attack(1), diamond(0), name("Cadence"), curFrame(0), speed(20), isLeft(false), elapsedTime(0)
{
	SetType(ActorType::PLAYER);
}

Player::~Player()
{
	Release();
}

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage(EImageKey::CADENCE_HEAD);	//캐릭터 머리
	body = ImageManager::GetInstance()->FindImage(EImageKey::CADENCE_BODY);		//캐릭터 몸통
	
	if (nullptr == image || nullptr == body)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Player::Update()
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

	if (elapsedTime >= 0.1f)
	{
		elapsedTime = 0;
		curFrame++;
		if (image->GetMaxFrameX() <= curFrame)
		{
			curFrame = 0;
		}
	}
	
	switch (state)
	{
	case PlayerState::IDLE:
		// 키 입력에 따라 플레이어의 위치를 업데이트
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LEFT))
		{
			isLeft = true;
			Jump(GetPos().x - 100, GetPos().y);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RIGHT))
		{
			isLeft = false;
			Jump(GetPos().x + 100, GetPos().y);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_UP))
		{
			Jump(GetPos().x, GetPos().y - 100);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_DOWN))
		{
			Jump(GetPos().x, GetPos().y + 100);
		}
		break;
	case PlayerState::JUMP:
		AnimJump();
		break;

	}
}

void Player::Render(HDC hdc)
{
	FPOINT pos = GetPos();
	pos.x -= Camera::GetInstance()->GetPos().x;
	pos.y -= Camera::GetInstance()->GetPos().y;


	RenderRectAtCenter(hdc, pos.x, pos.y, 50,50);

	// 캐릭터 몸통
	body->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);
	// 캐릭터 머리
	image->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);

}

void Player::Release()
{
}

void Player::Attack()
{
}

void Player::UseItem()
{

}

void Player::TakeDamage(float damage)
{
	hp -= damage;
	if (hp < 0)
	{
		hp = 0;
	}
}

bool Player::IsDead()
{
	return hp <= 0;
}

void Player::Jump(int x, int y)
{
	jumpData.sx = GetPos().x;
	jumpData.sy = GetPos().y;
	jumpData.ex = x;
	jumpData.ey = y;
	// 점프 상태로 변경
	state = PlayerState::JUMP;
}
