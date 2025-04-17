#pragma once
#include "TileActor.h"
class Image;
class TileMap;
class PositionManager;


typedef struct _stJump
{
	// 출발 위치.
	float sx;
	float sy;

	// 점프중인 높이.
	float height = 0;   // 이만큼 렌더할때 빼줘야함.
	float animCurtime = 0.0f;    // 애니메이션 진행 시간
	float totalAnimTime = 0.2f;  // 총 점프 시간.
	// 도착 위치
	float ex;
	float ey;
}stJump;

class TileCharacter : public TileActor
{

protected:
	string name;
	POINT dir;
	int gold;
	Image* image;
	vector<POINT> range;
	float attack;

	TileMap* tileMap;
	PositionManager* positionManager;

	stJump jumpData; // 점프 데이터

	virtual bool JumpAnim();

public:
	inline string GetName() { return name; }
	inline void SetName(string _name) { name = _name; }

	inline POINT GetDir() { return dir; }
	inline void SetDir(POINT _dir) { dir = _dir; }

	inline int GetGold() { return gold; }
	inline void SetGold(int _gold) { gold = _gold; }
	inline void AddGold(int _gold) { gold += _gold; }

	inline float GetAttack() { return attack; }
	inline void SetAttack(float _attack) { attack = _attack; }

	virtual void Jump(int x, int y);

};

