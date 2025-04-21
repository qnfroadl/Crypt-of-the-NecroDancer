#pragma once
#include "UIBase.h"

#include <queue>

class BeatHeart : public UIBase
{
private:
	int frameIndex;
	queue<unsigned int> beats; // ������ �÷��̾��� ��Ʈ ����/�̽��� ������� ���ڿ� �پ�� ��

	float alpha;
	int angle;

public:
	BeatHeart();
	virtual ~BeatHeart();

	void Init(queue<unsigned int> beatQueue);
	virtual void Release() override;
	void Update(unsigned int curPosition);
	virtual void Render(HDC hdc) override;

	void LocateOnCenter(); // ȭ�� ũ�Ⱑ ����ŵ� ����� ��ġ�ϵ���
};

