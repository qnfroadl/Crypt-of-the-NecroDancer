#pragma once
#include "UIBase.h"

class ItemNumUI;
class PlayerCoin : public UIBase
{
private:
	int coin;
	ItemNumUI* itemNumUI;

public:
	PlayerCoin();
	~PlayerCoin();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void OnPlayerGoldChanged(int gold, bool useGold = false);

	void LocateCoinInfo();
};

