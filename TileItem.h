#pragma once

#include "TileActor.h"

class Player;
class TileItem : public TileActor
{

protected:
	Player* owner;

public:
	TileItem();
	virtual ~TileItem();
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void SetOwner(Player* owner) { this->owner = owner; }
	Player* GetOwner() { return owner; }

	// 바로 사용되는 아이템은. 상호 작용시 바로 써질거고,
	// 장착 되는 아이템은. 상호 작용시 플레이어에게 장착 되겄지.
	// Interact = Pickup과 같다. 아이템과 상호작용 한다는것은 줍거나, 장착하거나 사용하거나. 그렇고 그런거지.
	virtual void Interact(GameActor* actor) override;	
	

	virtual void Drop() = 0;
	virtual void Equip() = 0;
	virtual void UnEquip() = 0;
	virtual void Use() = 0;

};

