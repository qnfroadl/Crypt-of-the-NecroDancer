#pragma once

#include "TileCharacter.h"
typedef struct _stJump
{
    // 출발 위치.
	float sx; 
	float sy; 

    // 점프 높이.
    int jumpSpeed = 40;
    float time = 0.5f; // 총 점프 시간.
    // 도착 위치
    float ex;
    float ey;
}stJump;

enum class PlayerState
{
	IDLE, JUMP, THROWREADY, THROW, 
};

class EventData;
class Player : public TileCharacter
{

private:
    PlayerState state;
	stJump jumpData; // 점프 데이터
    
    int curFrame;
	// Player의 상태   
    string name;
    float hp;
    float maxHP;
	float attack;
    float speed;
    int diamond;
    
    Image* body;    

    void OnBeatHit(EventData* data);             // 비트 성공 시
    void OnBeatMiss(EventData* data);            // 비트 실패 시

	void AnimJump();                            // 점프 동작
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
	
	
    void Jump(int x, int y);            // 점프 (이 기능을 만들어두면 타일에서도 가능함)
    
};
