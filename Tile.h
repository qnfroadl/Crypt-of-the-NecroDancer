#pragma once

#include "config.h"
#include "TileActor.h"
#include "IRendableTileActor.h"

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

class Tile : public TileActor, public IRendableTileActor, public enable_shared_from_this<Tile>
{
private:
	TileType type;
	shared_ptr<Block> block;
	shared_ptr<Trap> trap;
	Image* tileImage;
	int tileNum;

public:
	Tile() { SetType(ActorType::TILE);}
	~Tile() {}

	HRESULT Init();
	HRESULT Init(int x, int y);
	void Release();
	void Render(HDC hdc, bool useCamera = true);
	void Update();
	void OnTile(TileActor* actor);
	void OnBeat(bool isCombo);
	virtual void Interact(GameActor* actor) override;
	// 바닥 번호
	int GetTileNum() { return tileNum; }
	void SetTileNum(int _tileNum) {
		tileNum = _tileNum;
		type = GetTypeByTileNum(tileNum);
	}

	// 타일 타입
	TileType GetType() { return type; }
	TileType GetTypeByTileNum(int tileNum);

	// Block
	shared_ptr<Block> GetBlock() { return block; }
	void SetBlock(const shared_ptr<Block>& _block) { block = _block; }

	// Trap
	shared_ptr<Trap> GetTrap() { return trap; }
	void SetTrap(const shared_ptr<Trap>& _trap) { trap = _trap; }

	//// TileImage
	//Image* GetTileImage() const { return tileImage; }
	//void SetTileImage(Image* _image) { tileImage = _image; }

	// 타일 인덱스
	POINT GetTileIndex() { return index; }

	// 렌더링 대상 반환
	virtual vector<shared_ptr<TileActor>> GetRendableTileActors() override;
};
