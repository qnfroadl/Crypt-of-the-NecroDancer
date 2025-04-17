#include "Player.h"

#include "ImageManager.h"

void Player::OnBeatHit(EventData* data)
{

}

void Player::OnBeatMiss(EventData* data)
{

}

Player::Player()
	: hp(100), maxHP(100), attack(1), diamond(0), name("Cadence")
{
	SetType(ActorType::PLAYER);
}

Player::~Player()
{
	Release();
}

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage(name);	//캐릭터이름.
	if (nullptr == image)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Player::Update()
{

}

void Player::Render(HDC hdc)
{

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
