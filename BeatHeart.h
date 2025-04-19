#pragma once
#include "UIBase.h"

class BeatHeart : public UIBase
{
private:
	int frameIndex;

public:
	BeatHeart();
	virtual ~BeatHeart();

	void Init();
	virtual void Release() override;
	void Update(bool onBeat);
	virtual void Render(HDC hdc) override;
};

