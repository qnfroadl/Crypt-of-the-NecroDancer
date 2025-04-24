#pragma once
#include "UIBase.h"
#include "GameObject.h"

#include <queue>

class BeatHeart : public UIBase
{
private:
	int frameIndex;

public:
	BeatHeart();
	virtual ~BeatHeart();

	virtual HRESULT Init() override;
	virtual void Release() override;
	void Update();
	virtual void Render(HDC hdc) override;

	void BeatStart() { frameIndex = 1; }
	void BeatEnd() { frameIndex = 0; }

	void LocateOnCenter(); // 화면 크기가 변경돼도 가운데에 위치하도록
};

