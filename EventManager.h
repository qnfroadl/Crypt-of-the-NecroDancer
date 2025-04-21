#pragma once

#include <unordered_map>
#include <queue>
#include "Singleton.h"
#include <functional>
#include "Event.h"
#include "EventData.h"

class EventData;
enum class EventType
{
	BEAT, BEATHIT, BEATMISS, BEATEND, 
	SPAWNITEM,

	ACTORMOVED, ACTORREMOVED,
};


struct CompareEvent 
{
	bool operator()(Event* a, Event* b) const 
	{
		return a->priority > b->priority; // 작은 우선순위 먼저.
	}
};

class EventManager : public Singleton<EventManager>
{

private:
	std::unordered_map<EventType, std::vector<std::function<void(EventData*)>>> funcs;
	std::priority_queue<Event*, std::vector<Event*>, CompareEvent> queEvents;	//작은 값의 이벤트 타입부터 순차 처리.

	int GetPriority(EventType type);

public:
	void BindEvent(EventType type, std::function<void(EventData*)>);
	void AddEvent(EventType type, EventData* data, bool now = false);

	void Update();

};

