#pragma once

#include "TileItem.h"

// 플레이어 한테는 데미지2, 몬스터한테는 4였나? 벽돌은 몇단계 까지 깨지지
class ItemBomb : public TileItem
{

private:
	float damage;
	bool isExplode;
public:
	ItemBomb();
	virtual ~ItemBomb();

	HRESULT Init();			// 멤버 변수의 초기화, 메모리 할당
	void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)
	float GetDamage() { return damage; }

	void Interact(GameActor* actor) override;
};

