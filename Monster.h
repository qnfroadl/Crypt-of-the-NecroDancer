﻿#pragma once
#include "TileCharacter.h"

class Player;
class Tilemap;
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


typedef struct MonsterImageInfo {
	string keyName;
	const TCHAR* imagePath;
	int width;
	int height;
	int imageFrameX;
	int ImageFrameY;
};

class Monster:public TileCharacter, public enable_shared_from_this<Monster>
{
private:
	MONSTERTYPE monsterType;
	MonsterImageInfo imageInfo;
	MonsterState state;
	
	int maxHP;
	int curHP;


#pragma region Image Box
	unordered_map<MONSTERTYPE, MonsterImageInfo>MonsterInfoTable =
	{
		{ MONSTERTYPE::SKELETON,      { "Skeleton",TEXT("Image/Monster/SkeletoN_Normal _and_dark.bmp"),195,50, 8,2}},
		{ MONSTERTYPE::SLIME,         { "Slime",TEXT("Image/Monster/slime_green_normal.bmp"),104,26, 4,1}},
		{ MONSTERTYPE::ARMERSKELETON, { "ArmerSkeleton",TEXT("Image/Monster/armoredskeleton_Normal.bmp"),144,24,8,1}},

	};
#pragma endregion 이미지 정보 Table
protected:
	int beatCount;
	int moveDelay;
	int minFrame;
	int maxFrame;
	int animationFrame;
	int attackAnimationFrame;
	float elapsedTime;
	float changeTime;
	bool isLeft;
	bool isAttack;
	float damage;
	vector<POINT>dir = { {0,1},{0,-1},{-1,0},{1,0} };
	weak_ptr<Player> target;
	weak_ptr<Tilemap> tileMap;
	weak_ptr<PositionManager> positionManager;
	Image* attackImage;

public:
	void Init(MONSTERTYPE p);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	void imageInit(const MonsterImageInfo& _p);
	
	POINT Trace();
	void Dead();
	void OnBeat();
	bool AttackTarget(POINT _nextIndex);
	bool JumpAnim() override;
	void SetJumpData(int dx, int dy)override;



	void Patrol(int _pos, MONSTERTYPE _m);
	void Teleport(POINT index);

	//void BindMovePatten 
	void SettingFrame(MONSTERTYPE _m);
	void SetRange(vector<POINT>_range) { range= _range; }
	void SetTileIndex(const POINT& _index)override;
	void SetTileMap(weak_ptr<Tilemap> _tileMap);
	void SetPositionManager(weak_ptr<PositionManager>_positionManager) { this->positionManager = _positionManager; }
	void SetTestJump();
	void SetPlayer(weak_ptr<Player> _player) { target = _player; }
	Direction SetDirection();
	MonsterImageInfo FindImageInfo(MONSTERTYPE _m);
	MONSTERTYPE GetMonsterType() const { return monsterType; }
	
	Monster();
	virtual ~Monster();

	void TakeDamage(int damage);
	bool IsDead();
};

