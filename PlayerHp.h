#pragma once
#include "UIBase.h"

class PlayerHp : public UIBase
{
private:
	float maxHp;
	float curHp;

	int beatAnimIndex;
	bool beatAnim;
	float elapsedTime;

public:
	PlayerHp();
	~PlayerHp();

	void Init();
	virtual void Release() override;
	virtual void Update() override;
	void Render(HDC hdc);

	virtual void OnPlayerHPChanged(float hp) override;
	virtual void OnPlayerMaxHPChanged(float maxHP) override;

	void SetBeatAnim(bool anim);
	void AdjustSize();
};

