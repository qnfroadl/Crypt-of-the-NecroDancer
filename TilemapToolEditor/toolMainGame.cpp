#include "toolMainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "ImageManager.h"
#include "Timer.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

#include "TilemapTool.h"

#include "Camera.h"

HRESULT toolMainGame::Init()
{


	KeyManager::GetInstance()->Init();
	ImageManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();

	SceneManager::GetInstance()->AddScene("TilemapTool", new TilemapTool());

	//Init Camera
	Camera::GetInstance()->SetSize(SIZE{SCENE_WIDTH, SCENE_HEIGHT});


	FPS = 144;

	this->hdc = GetDC(g_hWnd);


	SceneManager::GetInstance()->ChangeScene("TilemapTool");

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(SCENE_WIDTH, SCENE_HEIGHT)))
	{
		MessageBox(g_hWnd, TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);

		return E_FAIL;
	}

	return S_OK;
}

void toolMainGame::Release()
{
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
	SceneManager::GetInstance()->Release();


	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	ReleaseDC(g_hWnd, hdc);
}

void toolMainGame::Update()
{
	KeyManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	Camera::GetInstance()->Update();
}

void toolMainGame::Render()
{
	// 백버퍼에 먼저 복사
	HDC hBackBufferDC = backBuffer->GetMemDC();

	SceneManager::GetInstance()->Render(hBackBufferDC);


	// 백버퍼에 있는 내용을 메인 hdc에 복사
	backBuffer->Render(hdc);

}


LRESULT toolMainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_TIMER:
		//this->Update();
		break;
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		
		break;
	case WM_PAINT:
		//hdc = BeginPaint(g_hWnd, &ps);

		//this->Render(hdc);

		//EndPaint(g_hWnd, &ps);
		break;
	case WM_DESTROY:
		//KillTimer(hWnd, 0);
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
			//case MENU_ID_SAVE:
			//case MENU_ID_LOAD:
			//TilemapMenuClicked(LOWORD(wParam));

			break;
		}
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

toolMainGame::toolMainGame()
{
}

toolMainGame::~toolMainGame()
{
}

