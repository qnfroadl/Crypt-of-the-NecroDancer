#include "WeaponMaterial.h"

#include "Player.h"
#include "Weapon.h"

void WeaponMaterial::InitDamage()
{
	switch (materialType)
	{
	case WeaponMaterialType::NORMAL:
	case WeaponMaterialType::BLOOD:
	case WeaponMaterialType::GOLDEN:	// 골드먹고 다음 비트까지 공격력999.
	case WeaponMaterialType::OBSIDIAN:	// 콤보에 따라 변화.
		damage = 1;
		break;
	case WeaponMaterialType::GLASS:
		damage = 4;
		break;
	case WeaponMaterialType::TITANIUM:
		damage = 2; // 고정.
		break;
	default:
		damage = 1;

	}

}

WeaponMaterial::WeaponMaterial(WeaponMaterialType materialType)
	: materialType(materialType)

{
	InitDamage();
}

string WeaponMaterial::GetMaterialName()
{
	switch (materialType)
	{
	case WeaponMaterialType::NORMAL:
		return string("normal");
	case WeaponMaterialType::BLOOD:
		return string("blood");
	case WeaponMaterialType::GLASS:
		return string("glass");
	case WeaponMaterialType::GOLDEN:
		return string("golden");
	case WeaponMaterialType::OBSIDIAN:
		return string("obsidian");
	case WeaponMaterialType::TITANIUM:
		return string("titanium");
	}
	return string("normal");
}

void WeaponMaterial::OnEnemyKilled(Player* player)
{
}

void WeaponMaterial::OnPlayerHit(Player* player, Weapon* weapon)
{
}

int WeaponMaterial::GetDamage()
{
	return damage;
}

void WeaponMaterial::SetType(WeaponMaterialType type)
{
	materialType = type;

	// 이것저것 초기화.

	InitDamage();
}
