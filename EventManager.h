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
	BEAT/*정박 타이밍 시작*/, BEATEND/*정박 타이밍 끝*/, BEATHIT, BEATMISS, SONGEND,
	SPAWNITEM, SPAWNWEAPON,

	PLAYERMOVED,	// 플레이어 이동 됨 이벤트. 
	BLOCKDESTROYED,	// 벽돌 파괴 됨 이벤트.
	BLOCKDESTROYFAILED,	// 벽돌 파괴 실패 이벤트.

	ENTERZONE,	// 어떤 존에 입장하는지. 이벤트.
	NEXTLEVEL,	// 다음층으로 이동하는 이벤트.

	COMBOSTART,	// 콤보 시작.
	COMBOFAILED,	// 콤보 실패.

	LIGHTINGUPDATED,	// 라이팅이 업데이트 되었음을 알림.

	INTERACT, 
};


struct CompareEvent 
{
	bool operator()(Event* a, Event* b) const 
	{
		return a->priority > b->priority; // 작은 우선순위 먼저.
	}
};
struct BoundFunc
{
    GameObject* obj;
    std::function<void(EventData*)> func;
};

class EventManager : public Singleton<EventManager>
{

private:
	std::unordered_map<EventType, std::vector<BoundFunc>> funcs;
	std::priority_queue<Event*, std::vector<Event*>, CompareEvent> queEvents;	//작은 값의 이벤트 타입부터 순차 처리.

	int GetPriority(EventType type);

public:
	void BindEvent(GameObject* obj, EventType type, std::function<void(EventData*)>);
	void UnbindEvent(GameObject* obj, EventType type);

	void AddEvent(EventType type, EventData* data, bool now = false);

	void Update();

};

