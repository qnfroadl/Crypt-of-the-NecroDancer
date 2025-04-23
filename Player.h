#pragma once

#include "ObservableValue.h"
#include "TileCharacter.h"
#include "IPlayerObserver.h"

class Weapon;
class ItemBomb;

class IPlayerObserver;
enum class PlayerState
{
	IDLE, JUMP, THROWREADY, THROW, DEAD,
};

class EventData;
class Tilemap;
class TileItem;
class Shovel;

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
	ObservableValue<int> bombCount;

	float attack;
    float speed;
    bool isLeft;

    Image* body;    

    // 무기, 갑옷, 삽, 횃불, 소모품
    shared_ptr<Weapon> weapon;
    shared_ptr<TileItem> armor;
    shared_ptr<Shovel> shovel;
    shared_ptr<TileItem> touch;
    shared_ptr<TileItem> consumableItem;

    // 참조
    weak_ptr<Tilemap> tileMap;
	weak_ptr<PositionManager> positionManager;

    void OnBeatHit(EventData* data);             // 비트 성공 시
    void OnBeatMiss(EventData* data);            // 비트 실패 시
    void OnComboReset(EventData* data);         // 콤보 리셋.
    void OnComboUp(EventData* data);            // 콤보 업

    bool JumpAnim() override;
    void SetJumpData(InputKey key);
    void SetJumpData(int dx, int dy) override;
    void CalcAttackRange(Direction dir, vector<POINT>& range);
    void NotifyAll();
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

	void AddObserver(IPlayerObserver* observer);
    void BindRelease();

    void SetPlayerIndex(PlayerIndex index) { playerIndex = index;}
    PlayerIndex GetPlayerIndex() {return playerIndex;}
	void SetName(string name) { this->name = name; }

    bool Attack(POINT index, Direction dir);                    //  공격
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
	
	void AddWeapon(shared_ptr<Weapon> weapon);
	
    void AddBomb(int bomb) { this->bombCount.Set(this->bombCount.Get() + bomb); }
   
};
