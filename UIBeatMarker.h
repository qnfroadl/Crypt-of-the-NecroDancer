#pragma once
#include "UIBase.h"

class UIBeatMarker : public UIBase
{
private:
	bool dead;
	bool red;

public:
	UIBeatMarker();
	virtual ~UIBeatMarker();

	void Init(bool _red=false);
	virtual void Release() override;
	void Update(float diff);
	virtual void Render(HDC hdc) override;

	bool GetIsDead() { return dead; }
};

