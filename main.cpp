// main.cpp

#include "config.h"
#include "MainGame.h"
#include "TimerManager.h"
#include "Timer.h"

HINSTANCE g_hInstance;	// 프로그램 인스턴스 핸들
HWND g_hWnd;
LPCWSTR g_lpszClassName = (LPCWSTR)TEXT("Crypt of the NecroDancer - 포텐업 2기 김미현, 문경원, 이주현, 차승근");
MainGame g_mainGame;
POINT g_ptMouse;	// 마우스 좌표
unordered_map<pair<PlayerIndex, InputKey>, int, pair_hash> g_mapKey;

ULONG_PTR gdiplusToken;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	LPSTR lpszCmdParam, int nCmdShow)
{
	// gdi+ 초기화
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	g_hInstance = hInstance;

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbClsExtra = 2068;
	wndClass.cbWndExtra = 2068;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SHIELD));


	//// 윈도우를 생성하기 위한 데이터 셋팅
	//WNDCLASS wndClass;
	//wndClass.cbClsExtra = 0;
	//wndClass.cbWndExtra = 0;
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndClass.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
	//wndClass.hIcon = LoadIcon(g_hInstance, IDI_APPLICATION);
	//wndClass.hInstance = g_hInstance;
	//wndClass.lpfnWndProc = WndProc;		// 함수의 이름은 메모리주소이다.
	//wndClass.lpszClassName = g_lpszClassName;
	//wndClass.lpszMenuName = NULL;
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;	// | : 비트연산자

	//RegisterClass(&wndClass);
	RegisterClassEx(&wndClass);

	//g_hWnd = CreateWindow(g_lpszClassName, g_lpszClassName,
	//	WS_OVERLAPPEDWINDOW, 50, 50, WINSIZE_X, WINSIZE_Y,
	//	NULL, NULL, g_hInstance, NULL);

	RECT rcWindowSize = { 0, 0, SCENE_WIDTH, SCENE_HEIGHT };
	AdjustWindowRect(&rcWindowSize, WS_OVERLAPPEDWINDOW, FALSE);
	int width = rcWindowSize.right - rcWindowSize.left;
	int height = rcWindowSize.bottom - rcWindowSize.top;

	g_hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, g_lpszClassName, g_lpszClassName,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 50, 50, width, height,
		NULL, NULL, g_hInstance, NULL);
	
	g_mainGame.Init();
	ShowWindow(g_hWnd, nCmdShow);

	TimerManager::GetInstance()->Init();

	// 프레임 제한을 위한 타이머.
	Timer fpsTimer;
	fpsTimer.Init();
	float deltaTime = 0.f;

	MSG message;

	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if(message.message == WM_QUIT) break;	//닫기버튼
			
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			fpsTimer.Tick();
			deltaTime += fpsTimer.GetDeltaTime();
			if ( 1.0f <= deltaTime * g_mainGame.GetFPS())
			{
				deltaTime = 0;

				TimerManager::GetInstance()->Update();
				g_mainGame.Update();
				g_mainGame.Render();
			}
		}
	}

	g_mainGame.Release();
	TimerManager::GetInstance()->Release();

	// gdi+ 종료
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return message.wParam;
}

LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}
