#pragma once

#include "TileItem.h"
#include "ImageManager.h"
#include "Player.h"
#include "WeaponMaterial.h"
class Monster;
class Player;

enum class DamageType
{
    NORMAL, PIERCING, PHASING,
};
enum class WeaponType
{
	DAGGER, BROADSWORD,
};

class Weapon : public TileItem
{
private:
	int curFrame;
	
    Image* dropedImage;
	Image* swipeImage;

    DamageType damageType; 
	WeaponType weaponType;
    vector<POINT> range;
	std::unique_ptr<WeaponMaterial> material;

	void InitRange();
	void InitImage();
	const string& GetWeaponTypeName();

public:
	Weapon(DamageType damageType, WeaponType weaponType);
	virtual ~Weapon(){}

	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update();		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(HDC hdc);	// 프레임 단위로 출력(이미지, 텍스트 등)

	void SetWeaponType(DamageType damageType, WeaponType weaponType);
	void SetMaterial(std::unique_ptr<WeaponMaterial> material);

	void Interact(GameActor* actor) override;

	void Attack();
	
	// 유리무기인 경우에는 유리조각 스폰.
	void Destroy();

};