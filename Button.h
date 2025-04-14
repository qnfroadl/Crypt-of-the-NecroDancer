#pragma once

#include "GameActor.h"
#include <functional>

enum class ButtonState
{
	None, Down, Up
};
class Image;
class Button : public GameActor
{

private:
	ButtonState state;
	Image* image;
	RECT rc;
	
	function<void()> func;

public:
	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
	
	void Bind(function<void()> func);
	void Clicked();
};