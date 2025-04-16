#include "EventManager.h"

int EventManager::GetPriority(EventType type)
{
	int priority = 100;

	switch (type) 
	{
		case EventType::BEAT: 
		case EventType::BEATHIT: 
		case EventType::BEATMISS: 
			priority = 1;

		case EventType::BEATEND: 
			priority = 2;

	}

	return priority;
}

void EventManager::BindEvent(EventType type, std::function<void(EventData*)> func)
{
	funcs[type].push_back(func); // std::move(func)
}

void EventManager::AddEvent(EventType type, EventData* data)
{
	// ObjectPool 사용 To do - 출발준비
	Event* event = new Event();
	event->type = type;
	event->data = data;
	event->priority = GetPriority(type);

	queEvents.push(event);
}

void EventManager::Update()
{
	if (false == queEvents.empty())
	{
		Event* event = queEvents.top();
		queEvents.pop();

		// 이벤트 노티
		auto it = funcs[event->type].begin();

		while (it != funcs[event->type].end())
		{
			(*it)(event->data);
		}
		
		delete event;
	}
	
}
