#pragma once
#include "GameObject.h"

class Image;
class IntroScene : public GameObject
{
private:
	Image* introCutscene;
	bool cutscene1End;

	int frameIndex;
	float elapsedTime;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

