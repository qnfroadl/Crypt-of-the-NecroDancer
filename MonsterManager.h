#pragma once
#include"GameObject.h"
#include "Monster.h"
#include "BossMonster.h"

class Tilemap;
class PositionManager;
class Player;
class BossMonster;
typedef std::basic_string<TCHAR> tstring;

class MonsterManager :public GameObject
{
private:
	vector <shared_ptr<Monster>>monsterVector;
	weak_ptr<PositionManager> positionManager;
	weak_ptr<Tilemap> tileMap;
	//vector<MonsterImageInfo>vecImageInfo;
	vector<MonsterImageInfo> vecMonsterInfo;
	int monsterNumber;
	shared_ptr<BossMonster>temp;
public:

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
	
	void SpwanBossMonster();
	shared_ptr<Monster> AddMonster(MONSTERTYPE _type);
	shared_ptr<Monster> AddMonster(MonsterImageInfo _info);
	shared_ptr<BossMonster> AddBossMonster(BossType _type);
	inline void SetMonsterNumber(int _monsterNumber) { monsterNumber = _monsterNumber; }
	inline int GetMonsterNumber() { return monsterNumber; }

	void SetTp();
	void SetTileMap(weak_ptr<Tilemap>_tileMap);
	void SetPositionManager(weak_ptr<PositionManager> _positionManager);
	void SetPlayer(weak_ptr<Player> _player);
	void GetMonsterDataFromJson();
	TCHAR* StringToTCHAR(const string& str);
	
	MonsterManager();
	virtual ~MonsterManager();
};
