#pragma once

#include "GameActor.h"

class TileActor : public GameActor
{
	
protected:
	POINT index;

public:
	TileActor();
	virtual ~TileActor() = default;
	void Render(HDC hdc) override;

	virtual void Interact(GameActor* actor) override;

	const POINT& GetTileIndex();
	virtual void SetTileIndex(const POINT& index);
};

