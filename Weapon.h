﻿#pragma once

#include "TileItem.h"
#include "ImageManager.h"
#include "Player.h"
#include "WeaponMaterial.h"
class Monster;
class Player;

class Weapon : public TileItem, public enable_shared_from_this<Weapon>
{
private:
	int curFrame;
	float elapsedTime;
	bool bSwipe;
	FPOINT swipePos;
	float swipeAngle;

    Image* dropedImage;
	Image* swipeImage;

    DamageType damageType; 
	WeaponType weaponType;
    vector<POINT> range;
	shared_ptr<WeaponMaterial> material;

	void InitResources();

	void InitRange();
	void InitImage();
	string GetWeaponTypeName();

public:
	Weapon();
	virtual ~Weapon(){}

	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update();		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)

	void SetWeaponType(DamageType damageType, WeaponType weaponType);
	void SetMaterial(WeaponMaterialType type);

	void Interact(GameActor* actor) override;

	void Attack(Monster* monster);
	void Swipe(FPOINT pos, Direction dir);
	// 유리무기인 경우에는 유리조각 스폰.
	// void Destroy();

	vector<POINT> GetRange();
};