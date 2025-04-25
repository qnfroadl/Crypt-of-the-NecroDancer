#pragma once

#include "TileItem.h"

class BombAttack : public TileItem, public enable_shared_from_this<BombAttack>
{
private:
	Image* image;
	
	float damage;	// 플레이어는 절반 타격.
	int step;		// 폭발 단계.
	bool isExplode;

public:

	HRESULT Init();			// 멤버 변수의 초기화, 메모리 할당
	void Update();
	void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)
	void Release();

	void OnNextStep();
	void Interact(GameActor* actor) override;


};

