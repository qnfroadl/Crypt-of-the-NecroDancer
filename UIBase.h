#pragma once
#include "config.h"

class Image;
class UIBase
{
protected:
	Image* image;
	FPOINT position;
	FPOINT size;

public:
	UIBase();
	virtual ~UIBase();

	virtual void Init(string imageName, FPOINT _position);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
};