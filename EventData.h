#pragma once

#include "config.h"

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
	BeatHitEventData(PlayerIndex  playerIndex, InputKey inputKey) : playerIndex(playerIndex), inputKey(inputKey) {};
	~BeatHitEventData() {};

};