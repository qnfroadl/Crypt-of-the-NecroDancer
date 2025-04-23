#pragma once
#include "config.h"
#include "IPlayerObserver.h"

class Image;
class UIBase : public IPlayerObserver
{
protected:
	Image* image;
	FPOINT position;
	FPOINT size;

public:
	UIBase();
	virtual ~UIBase();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
};