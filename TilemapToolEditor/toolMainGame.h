#pragma once
#include "GameObject.h"

class Image;
class Timer;
class TilemapTool;

class toolMainGame : public GameObject
{
private:
	Image* backBuffer;
	HDC hdc;
	PAINTSTRUCT ps;
	//HANDLE hTimer;
	int FPS;
public:
	HRESULT Init();	// override (부모클래스와 같은 함수이름, 로직을 다르게 구현하고 싶을 때)
					// <-> overload (같은 함수 이름, 매개변수 타입과 갯수가 다르면 다른 함수로 처리)
	void Release();	
	void Update();	
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	toolMainGame();
	~toolMainGame();

	inline int GetFPS() { return this->FPS; }
};

