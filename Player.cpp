#include "Player.h"

Player::Player()
{
	SetType(ActorType::PLAYER);
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	return E_NOTIMPL;
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

int Player::GetPlayerIndex()
{
	return 0;
}

void Player::SetPlayerIndex()
{
}

void Player::AddCoin(int coin)
{
}

void Player::AddDiamond(int diamon)
{
}

int Player::GetCoin()
{
	return 0;
}

int Player::GetDiamond()
{
	return 0;
}

void Player::TakeDamage(float damage)
{
}

bool Player::IsDead()
{
	return GetHP() <= 0;
}
