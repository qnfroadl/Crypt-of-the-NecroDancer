#include "Weapon.h"
#include "WeaponMaterial.h"

#include "Player.h"
#include "Camera.h"
#include "GameActor.h"

void Weapon::InitRange()
{
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
	image = ImageManager::GetInstance()->FindImage(string("swipe_") + GetWeaponTypeName() + "_" + material->GetMaterialName());

}
const string& Weapon::GetWeaponTypeName()
{
	string name;
	switch (weaponType)
	{
	case WeaponType::DAGGER:
		name = "dagger";
	case WeaponType::BROADSWORD:
		name = "broadsword";
	}

	return name;
}

Weapon::Weapon(DamageType damageType, WeaponType weaponType)
	: damageType(damageType), weaponType(weaponType)
{

	// "dagger" ", "broadsword"
	// "normal", "blood",  "glass" ,"gold", "obsidian", "titanium"
	InitImage();
	InitRange();
}

HRESULT Weapon::Init()
{
	return E_NOTIMPL;
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
	if (nullptr == owner && IsActive())
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

void Weapon::SetMaterial(std::unique_ptr<WeaponMaterial> material)
{
	this->material = std::move(material);
	InitImage();
	InitRange();
}

void Weapon::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		owner = static_cast<Player*>(actor);
		owner->AddWeapon(this);
	}
}


void Weapon::Attack()
{
}

void Weapon::Destroy()
{
}
