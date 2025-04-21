#pragma once

#include "ObservableValue.h"
#include "TileCharacter.h"
#include "IPlayerObserver.h"

class Weapon;
class IPlayerObserver;
enum class PlayerState
{
	IDLE, JUMP, THROWREADY, THROW, 
};

class EventData;
class Tilemap;
class Player : public TileCharacter, public enable_shared_from_this<Player>
{

private:
	vector<IPlayerObserver*> observers;
    
    PlayerIndex playerIndex;
    PlayerState state;
	
    float elapsedTime;
    int curFrame;

	// Player의 상태
    string name;
    ObservableValue<int> goldMultiple;
    ObservableValue<float> hp;
    ObservableValue<float> maxHP;
    ObservableValue<int> diamond;
	ObservableValue<int> bomb;

	float attack;
    float speed;
    bool isLeft;

    Image* body;    

    // 참조
    weak_ptr<Tilemap> tileMap;
	weak_ptr<PositionManager> positionManager;

    void OnBeatHit(EventData* data);             // 비트 성공 시
    void OnBeatMiss(EventData* data);            // 비트 실패 시

    bool JumpAnim() override;
    void SetJumpData(InputKey key);
    void SetJumpData(int dx, int dy) override;

public:
    Player();
    virtual ~Player();

    HRESULT Init() override;
    void Update() override;
    void Render(HDC hdc) override;
    void Release() override;

	void SetTileIndex(const POINT& _index) override;

    void SetTileMap(weak_ptr<Tilemap> _tileMap);
    void SetPositionManager(weak_ptr<PositionManager> positionManager) { this->positionManager = positionManager; }
	void Teleport(POINT index);

	void AddObserver(IPlayerObserver* observer) { if (observer) {observers.push_back(observer); }}
    void SetPlayerIndex(PlayerIndex index) { playerIndex = index;}
    PlayerIndex GetPlayerIndex() {return playerIndex;}
	void SetName(string name) { this->name = name; }

    void Attack();                    //  공격
    void UseItem();                   // 아이템 사용

    void TakeDamage(float damage);
    bool IsDead();

	void SetHP(float hp) { this->hp.Set(hp); }
	float GetHP() { return this->hp.Get(); }

    void SetMaxHP(float maxHP) { this->maxHP.Set(maxHP); }
	float GetMaxHP() { return this->maxHP.Get(); }

	float GetAttack() { return this->attack; }
	void SetAttack(float attack) { this->attack = attack; }

	void SetDiamond(int diamond) { this->diamond.Set(diamond); }
    void AddDiamond(int diamond) { this->diamond.Set(this->diamond.Get() + diamond); }
	int GetDiamond() { return this->diamond.Get(); }
	
	void AddWeapon(Weapon* weapon);
	void AddBomb(int bomb) { this->bomb.Set(this->bomb.Get() + bomb); }
    
};
