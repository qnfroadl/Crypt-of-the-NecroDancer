#pragma once
#include "TileActor.h"
class Image;
class TileMap;
class PositionManager;
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


};

