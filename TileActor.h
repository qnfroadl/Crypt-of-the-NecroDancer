#pragma once

#include "GameActor.h"

enum class SightState
{
	INVISIBLE,	// 아예 렌더 안됨.
	VISIBLE, // BrightnessInfo 읽어서 밝기에 따라 투명도 변화.
	SHADOW	// 그림자 상태로 보이게 렌더.
};

class TileActor : public GameActor
{
	
protected:
	SightState sightState;
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

	void SetVisible(bool visible);
	void SetSightInfo(const SightInfo& sightInfo);
	const SightInfo& GetSightInfo();

	virtual void SetDynamicBrightness(float brightness);
	void SetStaticBrightness(float brightness);
	float GetDynamicBrightness();
	float GetStaticBrightness();

	float GetBrightness();

	void SetBrightnessInfo(const BrightnessInfo& info);
	const BrightnessInfo& GetBrightnessInfoInfo();

};

