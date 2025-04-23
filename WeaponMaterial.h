#pragma once

#include "config.h"
class Player;
class Weapon;


// 데미지 타입은 한번 정해지면 변하지 않음. 
// 무기의 종류도 변하지 않음. 
// 변하는건 재질뿐
class player;
class WeaponMaterial
{
private:
	WeaponMaterialType materialType;
	int damage;

	void InitDamage();
public:
	WeaponMaterial(WeaponMaterialType materialType);

	virtual string GetMaterialName();
	void OnEnemyKilled(Player* player);
	void OnPlayerHit(Player* player, Weapon* weapon);

	virtual ~WeaponMaterial() = default;

	int GetDamage();
};