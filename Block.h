﻿#pragma once
#include "config.h"
#include "TileActor.h"
#include "IRendableTileActor.h"


enum class BlockType
{
	NONE,
	NORMAL,       // 강도 1
	STONE,        // 강도 2
	CATACOMB,     // 강도 3
	SHOP,         // 강도 4
	DOOR,         // 강도 1, 부수는 이펙트 없음
	UNBREAKABLE   // 절대 안 부서짐
};

class Item;
class Image;
class Torch;
class Block : public TileActor, public IRendableTileActor, public enable_shared_from_this<Block>
{
private:
	int blockNum;
	int maxHP;
	int hp;
	BlockType type;
	Image* blocklImage;
	shared_ptr<Torch> torch;

public:
	Block() { SetType(ActorType::BLOCK); }
	~Block() {}

	HRESULT Init();
	HRESULT Init(FPOINT _pos, POINT _index);
	void Render(HDC hdc, bool useCamera = true);
	void Update() override;	

	void Destroy();
	bool CanDestroy(int strength);

	void SetBlockNum(int _blockNum);
	int GetBlockNum() { return blockNum; }

	void SetBlockByBlockNum(int WallNum);
	BlockType GetType() { return type; }

	int GetHardness() { return maxHP; }

	// 렌더링 대상 반환
	virtual vector<shared_ptr<TileActor>> GetRendableTileActors() override;

	void SetTorch(shared_ptr<Torch> t) { torch = t; }
	shared_ptr<Torch> GetTorch() const { return torch; }
};