#pragma once

#include "TileItem.h"

class ItemGold : public TileItem
{

private:
	int gold;

public:
	ItemGold(int gold = 1);
	virtual ~ItemGold();

	HRESULT Init();			// 멤버 변수의 초기화, 메모리 할당
	void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)

	void SetGold(int gold);
	int GetGold() { return gold; }
	void Interact(GameActor* actor) override;

};

