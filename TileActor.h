#pragma once

#include "GameActor.h"



class TileActor : public GameActor
{
	
private:
	POINT index;

public:
	void Update() override;
	void Render(HDC hdc) override;

	virtual void Interact(GameActor* actor) override;
};

