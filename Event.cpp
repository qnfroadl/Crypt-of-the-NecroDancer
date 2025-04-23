#include "Event.h"

#include "EventData.h"

Event::~Event()
{
	if (data)
	{
		delete data;
	}
}
