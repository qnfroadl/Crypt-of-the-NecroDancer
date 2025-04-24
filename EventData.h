#pragma once

#include "config.h"
#include "TileActor.h"

class EventData
{

public:
	EventData() {};
	virtual ~EventData() {};

};

class BeatHitEventData : public EventData
{
	//  비트 성공 시 Player와 몬스터에게 전달되는 데이터
	//  Player는 이 데이터를 통해서 비트 성공 시 행동을 정의한다.
public:
	PlayerIndex playerIndex;	//  Player의 인덱스
	InputKey inputKey;	//  이동하려는 방향.
	BeatHitEventData(PlayerIndex  playerIndex, InputKey inputKey) 
	: playerIndex(playerIndex), inputKey(inputKey) {};

	~BeatHitEventData() {};
};

class SpawnItemEventData : public EventData
{
public:
	POINT index;
	ItemType type;
	int value;	// 골드량 같은거 넣어줄 변수.

	SpawnItemEventData(POINT index, ItemType type, int value)
		: index(index), type(type), value(value) {};

};

class SpawnWeaponEventdata : public EventData
{
public:
	POINT index;

	DamageType damageType;
	WeaponMaterialType material;
	WeaponType weaponType;

	SpawnWeaponEventdata(POINT _index, DamageType dType, WeaponMaterialType mType, WeaponType wType)
		:index(_index),damageType(dType), material(mType), weaponType(wType)
	{

	}
};

class ShadowCastingEventData : public EventData
{
public:
	vector<vector<bool>>& sightMap;

	ShadowCastingEventData(vector<vector<bool>>& _sightMap)
	:sightMap(_sightMap)
	{

	}

};


class TileActor;
class InteractEventData : public EventData
{
public:
	shared_ptr<TileActor> actor;		// 현 위치는 액터가 갖고있것지.
	vector<POINT> range;

	InteractEventData(shared_ptr<TileActor> _actor, vector<POINT> _range)
		:  actor(_actor),range(_range) {};

};
