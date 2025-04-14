#pragma once

#include "GameObject.h"

class Image;
class BattleScene : public GameObject
{
private:
	Image* background;

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	BattleScene() {};
	virtual ~BattleScene() {};	
};

