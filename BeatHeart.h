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

	void LocateOnCenter(); // ȭ�� ũ�Ⱑ ����ŵ� ����� ��ġ�ϵ���
};

