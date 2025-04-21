#pragma once
#include "config.h"

class UIBase;
class UIManager
{
private:
	vector<UIBase*> uiList;

public:
	UIManager();
	~UIManager();

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void AddUI(UIBase* ui) { uiList.push_back(ui); }
};

