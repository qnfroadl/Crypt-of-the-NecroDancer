#pragma once
#include "TileCharacter.h"

class Player;

enum class MonsterState
{
	IDLE,
	ACTIVE,// "The player will take damage if there is an overlap in this state."
	JUMP,
	DEAD,
};
enum class MONSTERTYPE
{
	SKELETON,
	SLIME,
	ARMERSKELETON,
};
enum class Direction
{
	UP, DOWM, LEFT, RIGHT
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
	MONSTERTYPE monsterType;
	MonsterState state;
	float light;
	int beatCount;
	int moveDelay;
	int minFrame;
	int maxFrame;
	int animationFrame;
	float elapsedTime;
	Player* player;
	MonsterImageInfo imageInfo;
	float changeTime;
	bool isLeft;
	bool isFront;
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
	void SettingFrame(MONSTERTYPE _m);
	void SetRange(vector<POINT>_range) { range= _range; }
	void Trace();
	void Dead();
	void OnBeat();
	void AttackTarget();
	bool JumpAnim() override;
	void SetJumpData(int dx, int dy)override;
	void Patrol(int _pos, MONSTERTYPE _m);
	
	Direction SetDirection();
	MonsterImageInfo FindImageInfo(MONSTERTYPE _m);
	MONSTERTYPE GetMonsterType() const { return monsterType; }

	Monster();
	virtual ~Monster();


};

