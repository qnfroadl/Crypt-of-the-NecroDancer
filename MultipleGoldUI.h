#pragma once

#include "UIBase.h"

class ItemNumUI;

class MultipleGoldUI : public UIBase
{
private:
	ItemNumUI* itemNumUI;
	int goldMultiple;

public:
	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void OnPlayerGoldMultipleChanged(int multiple) override 
	{ goldMultiple = multiple; }


};

