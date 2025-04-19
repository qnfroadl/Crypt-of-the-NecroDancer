#pragma once
#include "GameObject.h"

//test
#include "TileActor.h"
//L,J,H test
#include "Monster.h"
class Image;
class Timer;
class TilemapTool;
class Button;
class PlayerManager;


class MainGame : public GameObject
{
private:
	Image* backBuffer;
	HDC hdc;
	PAINTSTRUCT ps;
	//HANDLE hTimer;
	
	wchar_t szText[128];
	int FPS;
	
	Button* btn;
	bool bRenderCollision;

	// 콜리젼매니저 체크용
	int collCount;
	int activeCollCount;
	int collCheckCount;

	void UpdateCollisionInfo();
	void ItemSpawnSimulation();

	//void TilemapMenuClicked(WORD id);

	void InitResource();

	//test
	shared_ptr<PlayerManager> playerManager;
	Monster* testMoster;
public:
	HRESULT Init();	// override (부모클래스와 같은 함수이름, 로직을 다르게 구현하고 싶을 때)
					// <-> overload (같은 함수 이름, 매개변수 타입과 갯수가 다르면 다른 함수로 처리)
	void Release();	
	void Update();	
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();

	inline int GetFPS() { return this->FPS; }
};

