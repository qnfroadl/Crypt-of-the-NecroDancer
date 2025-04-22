#pragma once
#include "config.h"
#include "TileActor.h"
enum class TileType
{
	NONE,
	BRIGHT_DIRT,
	DARK_DIRT,
	COMBO1_DIRT,
	COMBO2_DIRT,
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
	Image* tileImage;

public:
	Tile() { SetType(ActorType::TILE); };
	~Tile() {};
	HRESULT Init();
	HRESULT Init(int x, int y);
	void Render(HDC hdc, bool useCamera = true);
	void Release();
	void OnTile(TileActor* actor);

	// 바닥
	int GetTileNum() { return tileNum; }
	void SetTileNum(int _tileNum) { 
		tileNum = _tileNum; 
		type = GetTypeByTileNum(_tileNum);
	}

	// 타일 타입
	TileType GetType() { return type; }
	TileType GetTypeByTileNum(int tileNum);

	// 블록 (벽)
	Block* GetBlock() { return block; }
	void SetBlock(Block* _block);
	

	POINT GetTileIndex() { return index; }

	virtual void Interact(GameActor* actor) override;


	void OnBeat(bool isCombo);
};

