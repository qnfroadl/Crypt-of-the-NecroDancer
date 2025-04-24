#pragma once
#include "UIBase.h"

class Image;
class ItemNumUI;
class PlayerWallet : public UIBase
{
private:
	int coin;
	Image* coinImage;
	
	int diamond;
	Image* diamondImage;

	ItemNumUI* itemNumUI;

public:
	PlayerWallet();
	~PlayerWallet();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void OnPlayerGoldChanged(int gold, bool useGold = false);
	virtual void OnPlayerDiamondChanged(int _diamond);

	void LocateInfo();
};

