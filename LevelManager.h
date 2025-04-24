#pragma once

#include "GameActor.h"

class EventData;
class LevelManager : public GameActor
{
	
private:
	int zone;		// 
	int curLevel;	// 

	// 레벨 정보를 갖고 있어야함.

public:
	HRESULT Init() override;
	void Release() override;
	void OnNextLevel(EventData* data);
	void OnEnterZone(EventData* data);

};

