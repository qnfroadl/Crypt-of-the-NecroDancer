#include "Player.h"

#include "ImageManager.h"

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
	jumpData.time -= dt;

	float jupmY = 2; // sinf(3.141592f * dt);
	jumpData.sy -= jupmY;

	jumpData.sx += (jumpData.ex - jumpData.sx) * dt;
	float y = jumpData.sy + (jumpData.ey - jumpData.sy) * dt;

	// 위로 점프 곡선 추가 (sin곡선)
	

	// 점프 종료.
	if (jumpData.time <= 0)
	{
		state = PlayerState::IDLE;
		jumpData.sx = jumpData.ex;
		y = jumpData.ey;
	}
	
	SetPos(jumpData.sx, y);
}

Player::Player()
	: state{}, hp(100), maxHP(100), attack(1), diamond(0), name("Cadence"), curFrame(0), speed(30)
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
	static float delta = 0;	//1회만 초기화됨.
	
	delta += TimerManager::GetInstance()->GetDeltaTime();

	if (delta >= 0.1f)
	{
		delta = 0;
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
			Jump(GetPos().x - 50, GetPos().y);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RIGHT))
		{
			Jump(GetPos().x + 50, GetPos().y);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_UP))
		{
			Jump(GetPos().x, GetPos().y - 50);
		}
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_DOWN))
		{
			Jump(GetPos().x, GetPos().y + 50);
		}
		break;
	case PlayerState::JUMP:
		AnimJump();
		break;

	}
}

void Player::Render(HDC hdc)
{
	RenderRectAtCenter(hdc, GetPos().x, GetPos().y, 50,50);

	// 캐릭터 몸통
	body->FrameRender(hdc, GetPos().x, GetPos().y, curFrame, 0, false, true);
	// 캐릭터 머리
	image->FrameRender(hdc, GetPos().x, GetPos().y, curFrame, 0, false, true);

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
	jumpData.time = 0.2f;
	// 점프 상태로 변경
	state = PlayerState::JUMP;
}
