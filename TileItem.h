#pragma once

#include "TileActor.h"

class Player;
class Image;
class TileItem : public TileActor
{

private:
	ItemType type;
	Player* owner;

public:
	TileItem();
	virtual ~TileItem();
	
	void SetItemType(ItemType type) { this->type = type; }
	ItemType GetItemType() { return type; }

	void SetOwner(Player* owner) { this->owner = owner; }
	Player* GetOwner() { return owner; }
	bool HasOwner() { return owner != nullptr; }

	virtual void Render(HDC hdc) override;

	// 바로 사용되는 아이템은. 상호 작용시 바로 써질거고,
	// 장착 되는 아이템은. 상호 작용시 플레이어에게 장착 되겄지.
	// Interact = Pickup과 같다. 아이템과 상호작용 한다는것은 줍거나, 장착하거나 사용하거나. 그렇고 그런거지.
	virtual void Interact(GameActor* actor) override;	
	
	virtual void Drop();
	virtual void Equip();
	virtual void Use();
	
};

