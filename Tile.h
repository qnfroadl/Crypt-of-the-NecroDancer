#pragma once
#include "config.h"

enum class TileType
{
	NONE,
	NORMAL,
};
class Block;
class Trap;
class GameActor;
class Tile
{
private:
	TileType type;
	Block* block;
	Trap* trap;
	float light;
	int tileNum;
	RECT rcTile;

public:
	void Init();
	void Render(HDC hdc);
	void OnTile(GameActor* actor);

	// �ٴ�
	int GetTileNum() { return tileNum; }
	void SetTileNum(int _tileNum) { tileNum = _tileNum; }

	// ��ġ
	RECT GetRcTile() { return rcTile; }
	void SetRcTile(RECT _rcTile) { rcTile = _rcTile; }

	// ��� (��)
	Block* GetBlock() { return block; }
	void SetBlock(Block* _block) { block = _block; }

	// �ٴ� Ÿ��
	TileType GetType() { return type; }
	void SetType(TileType _type) { type = _type; }
};

