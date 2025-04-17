#pragma once

#include "GameActor.h"

class TileActor : public GameActor
{
	
private:
	string name;
	float maxHp;
	float hp;
	POINT index;

public:
	void Update() override;
	void Render(HDC hdc) override;

	void SetHP(float hp);
	void SetMaxHP(float hp);

	float GetHP();
	float GetMaxHP();

	const POINT& GetTileIndex();
	void SetTileIndex(const POINT& index);


};

