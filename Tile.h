#pragma once
#include "config.h"
#include "TileActor.h"
enum class TileType
{
	NONE,
	NORMAL,
};

class Block;
class Trap;
class Image;
class Tile : public TileActor
{
private:
	TileType type;
	Block* block;
	Trap* trap;
	float light;
	int tileNum;
	//RECT rcTile;
	Image* tileImage;
public:
	Tile() {};
	~Tile() {};
	HRESULT Init();
	HRESULT Init(int x, int y);
	void Render(HDC hdc, bool useCamera = true);
	void Release();
	void OnTile(TileActor* actor);

	// �ٴ�
	int GetTileNum() { return tileNum; }
	void SetTileNum(int _tileNum) { 
		tileNum = _tileNum; 
		type = GetTypeByTileNum(_tileNum);
	}

	// Ÿ�� Ÿ��
	TileType GetType() { return type; }
	TileType GetTypeByTileNum(int tileNum);

	//// ��ġ
	//RECT GetRcTile() { return rcTile; }
	//void SetRcTile(RECT _rcTile) { rcTile = _rcTile; }

	// ��� (��)
	Block* GetBlock() { return block; }
	void SetBlock(Block* _block) { block = _block; }
};

