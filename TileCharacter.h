#pragma once
#include "TileActor.h"
#include "ObservableValue.h"

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
	float dx;
	float dy;
}stJump;


class TileCharacter : public TileActor
{

protected:
	string name;
	POINT dir;
	ObservableValue<int> gold;
	Image* image;
	vector<POINT> range;
	float attack;

	TileMap* tileMap;
	PositionManager* positionManager;

	stJump jumpData; // 점프 데이터

	virtual bool JumpAnim();

public:
	TileCharacter();

	inline string GetName() { return name; }
	inline void SetName(string _name) { name = _name; }

	inline POINT GetDir() { return dir; }
	inline void SetDir(POINT _dir) { dir = _dir; }

	inline int GetGold() { return gold.Get();}
	inline void SetGold(int _gold) { gold.Set(_gold);}
	inline void AddGold(int _gold) { gold.Set(gold.Get() + _gold);}

	inline float GetAttack() { return attack; }
	inline void SetAttack(float _attack) { attack = _attack; }

	virtual void SetJumpData(int dx, int dy);
	void ResetJumpData();

};

