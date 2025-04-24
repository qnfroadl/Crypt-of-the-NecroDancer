#pragma once
#include "config.h"
#include "IPlayerObserver.h"
#include "GameObject.h"

class Image;
class UIBase : public IPlayerObserver, public GameObject
{
protected:
	Image* image;
	FPOINT position;
	FPOINT size;

public:
	UIBase();
	virtual ~UIBase();

	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
};