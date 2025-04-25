#pragma once

#include "TileItem.h"

// 플레이어 한테는 데미지2, 몬스터한테는 4였나? 벽돌은 몇단계 까지 깨지지
class ItemBomb : public TileItem, public enable_shared_from_this<ItemBomb>
{

private:
	Image* image;
	
public:
	ItemBomb();
	~ItemBomb();

	HRESULT Init() override;			// 멤버 변수의 초기화, 메모리 할당
	void Render(HDC hdc)  override;	// 프레임 단위로 출력(이미지, 텍스트 등)
	void Release() override;

	void Interact(GameActor* actor) override;

};