#pragma once
#include "GameObject.h"

class Image;
class MainMenuScene : public GameObject
{
private:
	Image* mainMenu;
	Image* pressToContinue;
	float alpha;
	float elapsedTime;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

