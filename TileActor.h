#pragma once

#include "GameActor.h"

class TileActor : public GameActor
{
	
protected:
	POINT index;

public:
	void Render(HDC hdc) override;

	virtual void Interact(GameActor* actor) override;

	const POINT& GetTileIndex();
	void SetTileIndex(const POINT& index);
};

