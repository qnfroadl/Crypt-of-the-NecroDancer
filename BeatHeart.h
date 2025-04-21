#pragma once
#include "UIBase.h"

#include <queue>

class BeatHeart : public UIBase
{
private:
	int frameIndex;
	queue<unsigned int> beats; // 심장은 플레이어의 비트 성공/미스와 관계없이 정박에 뛰어야 함

	float alpha;
	int angle;

public:
	BeatHeart();
	virtual ~BeatHeart();

	void Init(queue<unsigned int> beatQueue);
	virtual void Release() override;
	void Update(unsigned int curPosition);
	virtual void Render(HDC hdc) override;

	void LocateOnCenter(); // 화면 크기가 변경돼도 가운데에 위치하도록
};

