#pragma once

#include "TileActor.h"

class EventData;
class Player : public TileActor
{

private:
    int playerIndex;
    int coin;
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

    void Attack();                    //  공격
    void UseItem();                   // 아이템 사용

    int GetPlayerIndex();
    void SetPlayerIndex();

    void AddCoin(int coin);
    void AddDiamond(int diamon);
    int GetCoin();
    int GetDiamond();

    void TakeDamage(float damage);
    bool IsDead();

};
