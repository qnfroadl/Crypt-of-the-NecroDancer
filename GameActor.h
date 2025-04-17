#pragma once

#include "GameObject.h"
#include "config.h"

class GameActor : public GameObject
{
	
private:
	ActorType type;	
	FPOINT pos;

public:
	GameActor();
	virtual ~GameActor();

	const FPOINT& GetPos();

	void SetPos(const float& x, const float& y);
	void SetPos(const FPOINT& pos);

	void AddPos(const float& x, const float& y);
	void AddPos(const FPOINT& pos);

	ActorType GetType();
	void SetType(ActorType _type);

	virtual void Interact(GameActor* actor);
};