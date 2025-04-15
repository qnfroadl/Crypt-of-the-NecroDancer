#pragma once

#include "GameActor.h"

class Character : public GameActor
{
	
private:

public:
	void Update() override;
	void Render(HDC hdc) override;
};

