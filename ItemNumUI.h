#pragma once
#include "config.h"

class Image;
class ItemNumUI
{
private:
	Image* fontImage;
	FPOINT size;

public:
	ItemNumUI();
	~ItemNumUI();
	virtual void Init();
	virtual void Release();
	virtual void Update();
	void Render(HDC hdc, int num, int startX, int startY);
};

