#pragma once
#include"GameObject.h"
#include "Monster.h"
class Monster;
class TileMap;
class PositionManager;
class MonsterManager :public GameObject
{
private:
	vector <shared_ptr<Monster>>monsterVector;
	weak_ptr<PositionManager> positionManager;
	weak_ptr<TileMap> tileMap;

	int monsterNumber;
public:

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	shared_ptr<Monster> AddMonster(MONSTERTYPE _type);
	inline void SetpoitionManager(weak_ptr<PositionManager> _positionManager) { positionManager = _positionManager; }
	inline void SetTileMap(weak_ptr<TileMap>_tileMap) { tileMap = _tileMap; }
	inline void SetMonsterNumber(int _monsterNumber) { monsterNumber = _monsterNumber; }
	inline int GetMonsterNumber() { return monsterNumber; }


	MonsterManager();
	virtual ~MonsterManager();
};
