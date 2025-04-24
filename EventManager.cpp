#include "EventManager.h"
#include "config.h"
#include <iostream>

int EventManager::GetPriority(EventType type)
{
	int priority = 100;

	switch (type) 
	{
		case EventType::BEATEND: // 정박 타이밍 끝
			priority = 0;
			break;
		case EventType::BEAT: 
		case EventType::BEATHIT: 
		case EventType::BEATMISS: 
			priority = 1;
			break;
		case EventType::SONGEND: 
			priority = 2;
			break;
		case EventType::PLAYERMOVED:
			priority = 3;
			break;
	}

	return priority;
}


void EventManager::BindEvent(GameObject* _obj, EventType type, std::function<void(EventData*)> func)
{
	funcs[type].push_back(BoundFunc{ _obj, func});
}

void EventManager::UnbindEvent(GameObject* obj, EventType type)
{
	auto it = funcs[type].begin();
	while (it != funcs[type].end())
	{
		if (it->obj == obj)
		{
			it = funcs[type].erase(it);
			continue;
		}
		it++;
	}
	
}

void EventManager::AddEvent(EventType type, EventData* data, bool now)
{
	Event* event = new Event();
	event->type = type;
	event->data = data;
	event->priority = GetPriority(type);

	if (now)
	{
		auto it = funcs[type].begin();
		while (it != funcs[type].end())
		{
			(*it).func(data);
			it++;
		}

		delete event;
	}
	else 
	{
		queEvents.push(event);
	}
	
	// std::cout << "EventManager::AddEvent : " << (int)type << std::endl;
}

void EventManager::Update()
{
	if (false == queEvents.empty())
	{
		Event* event = queEvents.top();
		queEvents.pop();

		// 이벤트 노티, 매 프레임당 최대 10개의 이벤트만 처리한다. (일단은 로컬변수로 처리)
		auto it = funcs[event->type].begin();

		int maxCount = 10;
		int count = 0;
		while (it != funcs[event->type].end() && count < maxCount)
		{
			(*it).func(event->data);
			
			it++;
			count++;
		}
		
		delete event;
	}
	
}
