#pragma once
#include "Monster.h"

enum class BossType {
	Dragon,
	DeathMetal 
};
enum class BossState {
	IDLE,
	ACTIVE,
	JUMP,
	Skill,
	DEAD,
};
class BossMonster :public Monster
{
private:
	Image* fire[5];
	BossType bossType;
	MonsterImageInfo imageInfo;
	BossState state;
	bool isBlast;
	int blastAnimFrame;
	int textX;
	int textY;
	unordered_map<BossType, MonsterImageInfo>MonsterInfoTable =
	{
		{ BossType::Dragon,		{ "Dragon",TEXT("Image/BossMonster/dragon_red.bmp"),427,102, 7,2}},
		{ BossType::DeathMetal, { "DeathMetal",TEXT("Image/Monster/slime_green_normal.bmp"),104,26, 4,1}}
	};
public:
	void Init(BossType p);
	void Release()override;
	void Update()override;
	void Render(HDC hdc)override;

	bool JumpAnim() override;
	void SetJumpData(int dx, int dy)override;
	void SettingFrame(BossType _bm);
	
	void OnBeat();
	void SettingImageFrameImage();
	void FireImageRender(int index, HDC hdc, FPOINT pos, int animationFrame);
	void AttackTarget();
	bool FindWall(bool _isLeft);
	MonsterImageInfo FindImageInfo(BossType _bm);
	BossMonster();
	virtual ~BossMonster();
	
};

