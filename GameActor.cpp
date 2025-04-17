#include "GameActor.h"

GameActor::GameActor()
:type(ActorType::None), pos{0,0}
{
	
}

GameActor::~GameActor()
{

}

const FPOINT& GameActor::GetPos()
{
	return this->pos;
}

void GameActor::SetPos(const float& _x, const float& _y)
{
	this->pos.x = _x;
	this->pos.y = _y;
}

void GameActor::SetPos(const FPOINT& _pos)
{
	this->pos = _pos;
}

void GameActor::AddPos(const float& _x, const float& _y)
{
	this->pos.x += _x;
	this->pos.y += _y;
}

void GameActor::AddPos(const FPOINT& _pos)
{
	AddPos(_pos.x, _pos.y);
}

ActorType GameActor::GetType()
{
	return this->type;
}

void GameActor::SetType(ActorType _type)
{
	this->type = _type;
}

void GameActor::Interact(GameActor* actor)
{

}
