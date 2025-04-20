#pragma once
#include "UIBase.h"

class BeatMarker : public UIBase
{
private:
	bool dead;
	bool red;
	unsigned int beatPosition;
	float speed;

public:
	BeatMarker();
	virtual ~BeatMarker();

	void Init(unsigned int _beatPosition, bool _red=false);
	virtual void Release() override;
	void Update(unsigned int curPosition);
	virtual void Render(HDC hdc) override;

	void SetDead(bool _dead) { dead = _dead; }
	bool IsDead() { return dead; }

	void LocateMarker(unsigned int curPosition);
};

