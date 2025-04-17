#pragma once
#include "TileCharacter.h"

class Player;
enum class State
{
	IDLE,
	ACTIVE,
	DIE,
};
enum class MONSTERTYPE
{
	SKELETON,
	SLIME,
	ARMERSKELETON,
};

typedef struct MonsterImageInfo {
	string keyName;
	const TCHAR* imagePath;
	int width;
	int height;
	int imageFrameX;
	int ImageFrameY;
};

class Monster:public TileCharacter
{
private:
	State state;
	float light;
	int beatCount;
	int moveDelay;
	int animationFrame;
	int elapsedFrame;
	float elapsedTime;
	Player* player;
	MonsterImageInfo imageInfo;
#pragma region Image Box
	unordered_map<MONSTERTYPE, MonsterImageInfo>MonsterInfoTable =
	{
		{ MONSTERTYPE::SKELETON,      { "Skeleton",TEXT("Image/Monster/skeleton_Normal.bmp"),195,25, 8,1}},
		{ MONSTERTYPE::SLIME,         { "Slime",TEXT("Image/Monster/slime_green_normal.bmp"),104,26, 4,1}},
		{ MONSTERTYPE::ARMERSKELETON, { "ArmerSkeleton",TEXT("Image/Monster/armoredskeleton_Normal.bmp"),144,24,8,1}},

	};
#pragma endregion 이미지 정보 Table

public:
	void Init(MONSTERTYPE p);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;


	//void BindMovePatten 
	void SetRange(vector<POINT>_range) { range= _range; }
	void Trace();
	void Dead();
	void OnBeat();

	MonsterImageInfo FindImageInfo(MONSTERTYPE _m);
	Monster();
	virtual ~Monster();

};

