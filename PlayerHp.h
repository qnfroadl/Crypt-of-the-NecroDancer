#pragma once
#include "UIBase.h"

#include <queue>

class PlayerHp : public UIBase
{
private:
	float maxHp;
	float curHp;

	queue<unsigned int> beats;

	int beatAnimIndex;
	bool beatAnim;

public:
	PlayerHp();
	~PlayerHp();

	void Init();
	virtual void Release() override;
	virtual void Update() override;
	void Render(HDC hdc);

	virtual void OnPlayerHPChanged(float hp) override;
	virtual void OnPlayerMaxHPChanged(float maxHP) override;

	void AdjustSize();

	void LoadBeats();
	void CheckBeats();
};

