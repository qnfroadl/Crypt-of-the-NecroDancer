#pragma once

#include "TileCharacter.h"

class EventData;
class Player : public TileCharacter
{

private:
	// Player의 상태   
    string name;
    float hp;
    float maxHP;
	float attack;
    
    int diamond;

    void OnBeatHit(EventData* data);             // 비트 성공 시
    void OnBeatMiss(EventData* data);            // 비트 실패 시

public:
    Player();
    virtual ~Player();

    HRESULT Init() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Release() override;

	void SetName(string name) { this->name = name; }

    void Attack();                    //  공격
    void UseItem();                   // 아이템 사용

    void TakeDamage(float damage);
    bool IsDead();

	void SetHP(float hp) { this->hp = hp; }
	float GetHP() { return this->hp; }

    void SetMaxHP(float maxHP) { this->maxHP = maxHP; }
	float GetMaxHP() { return this->maxHP; }

	float GetAttack() { return this->attack; }
	void SetAttack(float attack) { this->attack = attack; }

	void SetDiamond(int diamond) { this->diamond = diamond; }
    void AddDiamond(int diamond) { this->diamond += diamond; }
	int GetDiamond() { return this->diamond; }
	
};
