#pragma once

#include <string>

enum class EventType;
class EventData;
class Event 
{

public:    
    int priority;
    EventType type;
    EventData* data;
    
    // priority가 클수록 먼저 나와야 하니까 maxHeap 방식
    bool operator > (const Event* other) const {
        return priority > other->priority; // 작은 게 뒤로
    }
};
