#pragma once
#include "config.h"
#include "GameActor.h"

class Item;
class Block : public GameActor
{
private:
	int tileNum;
public:
	Block() {};
	virtual ~Block() {};

	void Render(HDC hdc) {};
	bool Destroy(Item* item);
	bool Destroy(int string);

	void SetTileNum(int _tileNum) { tileNum = _tileNum; }
	int GetTileNum() { return tileNum; }
};

