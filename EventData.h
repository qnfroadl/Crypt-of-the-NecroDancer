#pragma once

#include "config.h"

class EventData
{

public:
	EventData();
	virtual ~EventData();

};

class BeatHitEventData : public EventData
{

public:
	POINT dir;	//  이동하려는 방향.
	
	~BeatHitEventData();


};