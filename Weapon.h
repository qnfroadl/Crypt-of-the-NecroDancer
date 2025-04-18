#pragma once

#include "TileActor.h"
#include "WeaponData.h"

class Monster;
class Player;

enum class DamageType
{
    NORMAL, PIERCING, PHASING,
};

// NORMAL, BLOOD, GLASS, GOLDEN, OBSIDIAN, TITANIUM
class IMaterialProperty
{
public:
	virtual int ModifyDamage(int baseDamage) = 0;
	virtual void OnAttackMonster(Player* owner, Monster* monster) = 0;
};

// 대거, 브로드소드, 
class IWeaponType
{

};



class Weapon : TileActor
{
    DamageType damageType;
    std::unique_ptr<IWeaponType> type;
    std::unique_ptr<IMaterialProperty> material;

public:
    void SetMaterial(std::unique_ptr<IMaterialProperty> newMat) 
    {
        material = std::move(newMat);
    }

    void SetForm(std::unique_ptr<IWeaponType> newForm)
    {
        type = std::move(newForm);
    }


};
