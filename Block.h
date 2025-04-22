#pragma once
#include "config.h"
#include "TileActor.h"

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

class Block : public TileActor
{
private:
	int blockNum;         // 벽 타일 번호 (샘플에서의 인덱스)
	int maxHP;
	int hp;
	BlockType type;      // 벽 종류 (강도, 특성)
	Image* blocklImage;
	
public:
	Block() { SetType(ActorType::BLOCK); };
	~Block() {};

	HRESULT Init();
	HRESULT Init(FPOINT _pos, POINT _index);
	void Render(HDC hdc, bool useCamera = true);

	void Destroy();     // 아이템 기준 파괴 가능 여부
	bool Destroy(int strength);   // 정수로 강도 비교

	void SetBlockNum(int _blockNum);
	int GetBlockNum() { return blockNum; }

	void SetBlockByBlockNum(int WallNum);
	BlockType GetType() { return type; }

	int GetHardness() {return maxHP;}

};