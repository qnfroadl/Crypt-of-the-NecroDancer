#pragma once

#include "GameObject.h"

class Image;
class LoadingScene : public GameObject
{

private:
	Image* bg;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;

	~LoadingScene() {};
};

