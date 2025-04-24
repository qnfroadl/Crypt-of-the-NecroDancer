#pragma once

#include "GameActor.h"


class TileActor : public GameActor
{
	
protected:
	SightInfo sightInfo;
	BrightnessInfo brightnessInfo;

	POINT index;
	
public:
	TileActor();
	virtual ~TileActor() = default;
	void Render(HDC hdc) override;

	virtual void Interact(GameActor* actor) override;

	const POINT& GetTileIndex();
	virtual void SetTileIndex(const POINT& index);

	void SetSightInfo(const SightInfo& info);
	const SightInfo& GetSightInfo();

	void SetBrightnessInfo(const BrightnessInfo& info);
	const BrightnessInfo& GetBrightnessInfoInfo();

};

