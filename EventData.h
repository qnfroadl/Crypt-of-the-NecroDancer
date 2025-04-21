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

class ItemSpawnEventData : public EventData
{
public:
	POINT index;
	FPOINT pos;
	ItemType type;
	int value;	// 골드량 같은거 넣어줄 변수.

	ItemSpawnEventData(POINT index, FPOINT pos, ItemType type, int value)
		: index(index), pos(pos), type(type), value(value) {};

};

class TileActor;
class TileActorPositionEventData : public EventData
{
public:
	POINT preIndex;						// 이전 위치.
	shared_ptr<TileActor> actor;		// 현 위치는 액터가 갖고있것지.
	
	TileActorPositionEventData(POINT preIndex, shared_ptr<TileActor> actor)
		: preIndex(preIndex), actor(actor) {};

};