#pragma once

#include "TileItem.h"

// 벽돌 파괴용 삽.

enum class ShovelType
{
	NORMAL, BLOOD, GLASS, SHARD, OBSIDIAN, TITANIUM
};

class Image;
class Shovel : public TileItem
{

private:
	Image* image;
	ShovelType shovelType;
	int hardness;

public:
	
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;

	void SetShovelType(ShovelType shovelType);
	virtual void Interact(GameActor* actor) override;


};