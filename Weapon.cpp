#include "Weapon.h"
#include "WeaponMaterial.h"

#include "Player.h"
#include "Camera.h"
#include "GameActor.h"
#include "Monster.h"
#include "ImageManager.h"

void Weapon::InitRange()
{
	range.clear();
	switch (weaponType)
	{
	case WeaponType::DAGGER:
		range.push_back({ 1, 0 });
		break;
	case WeaponType::BROADSWORD:
		range.push_back({ 1, -1 });
		range.push_back({ 1, 0 });
		range.push_back({ 1, 1 });
		break;
	default:
		break;
	}
}
void Weapon::InitImage()
{
	string swipeKey = string("swipe_") + GetWeaponTypeName() + "_" + material->GetMaterialName();

	swipeImage = ImageManager::GetInstance()->FindImage(swipeKey);
	dropedImage = ImageManager::GetInstance()->FindImage(string("weapon_") + GetWeaponTypeName() + "_" + material->GetMaterialName());
}
string Weapon::GetWeaponTypeName()
{
	string name;
	switch (weaponType)
	{
	case WeaponType::DAGGER:
		name = "dagger";
		break;
	case WeaponType::BROADSWORD:
		name = "broadsword";
		break;
	}

	return name;
}

Weapon::Weapon()
	: damageType(DamageType::NORMAL), weaponType(WeaponType::DAGGER)
{

	// "dagger" ", "broadsword"
	// "normal", "blood",  "glass" ,"gold", "obsidian", "titanium"
	
}

HRESULT Weapon::Init()
{
	SetMaterial(new WeaponMaterial(WeaponMaterialType::NORMAL));
	InitImage();
	InitRange();
	return S_OK;
}

void Weapon::Release()
{
}

void Weapon::Update()
{   
	
}

void Weapon::Render(HDC hdc)
{
	// 무기 이미지 렌더링
	// 카메라 위치에 따라 무기 위치 조정
	// 주인이 없을때 => 바닥에 있어야 함.
	if (false == HasOwner() && IsActive())
	{

	}
}

void Weapon::SetWeaponType(DamageType damageType, WeaponType weaponType)
{
	this->damageType = damageType;
	this->weaponType = weaponType;
	InitImage();
	InitRange();
}

void Weapon::SetMaterial(WeaponMaterial* material)
{
	this->material = material;
	InitImage();
	InitRange();
}

void Weapon::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		SetOwner(static_cast<Player*>(actor));
		GetOwner()->AddWeapon(shared_from_this());
	}
	else if (actor->GetType() == ActorType::MONSTER)
	{
		Attack(static_cast<Monster*>(actor));
	}
}


void Weapon::Attack(Monster* monster)
{
	monster->TakeDamage(material->GetDamage());
	
	if (monster->IsDead())
	{
		// 추가 로 뭔가 해야해.
	}

}

void Weapon::Destroy()
{

}

vector<POINT> Weapon::GetRange()
{
	return range;
}
