#pragma once

#include "GameActor.h"

class EventData;
class LevelManager : public GameActor
{
	
private:
	int zone;		// 
	int curLevel;	// 
	string keyName;	//현재 레벨을 갖고 있는 키
	// 레벨 정보를 갖고 있어야함.

public:
	HRESULT Init() override;
	void Release() override;

	void OnNextLevel(EventData* data);
	void OnEnterZone(EventData* data);

};

