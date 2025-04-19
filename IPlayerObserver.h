#pragma once
class IPlayerObserver
{

public:
	virtual ~IPlayerObserver() = default;
	// Player의 상태 변화에 대한 알림

public:
	// for ui or 다른 무언가.
	virtual void OnPlayerGoldChanged(int gold, bool useGold = false) {}
	virtual void OnPlayerDiamondChanged(int diamond) {}
	virtual void OnPlayerHPChanged(float hp) {}
	virtual void OnPlayerMaxHPChanged(float maxHP) {}

	virtual void OnPlayerShovelChanged(){}
	virtual void OnPlayerWeaponChanged(bool isThrowable){}
	virtual void OnPlayerBombChanged(int bomb) {}

	virtual void OnPlayerItemChanged(int itemType) {}	// 소모품 후추
	
};

