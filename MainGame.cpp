#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "CollisionManager.h"
#include "ImageManager.h"
#include "Timer.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

#include "LoadingScene.h"
#include "TilemapTool.h"
#include "BattleScene.h"
#include "Button.h"
#include "AstarScene.h"
#include "Camera.h"
#include "EventManager.h"

#include "SoundManager.h"
#include "BeatManager.h"

#define MENU_ID_SAVE 1
#define MENU_ID_LOAD 2

void MainGame::UpdateCollisionInfo()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_F1))
	{
		bRenderCollision = !bRenderCollision;
	}

	static float elapsedTime = 0;

	if (bRenderCollision)
	{
		elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

		if (1 < elapsedTime)	//1초에 한번만 해
		{
			elapsedTime = 0;
			collCount = CollisionManager::GetInstance()->GetCollisionCount();
			activeCollCount = CollisionManager::GetInstance()->GetActivecollisionCount();
			collCheckCount = CollisionManager::GetInstance()->GetCheckCount();
		}
	}
}

void MainGame::ItemSpawnSimulation()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LSHIFT))
	{
		SceneManager::GetInstance()->ChangeScene("TilemapTool", "Loading");
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RSHIFT))
	{
		SceneManager::GetInstance()->ChangeScene("AstarScene", "Loading");
	}
}

void MainGame::TilemapMenuClicked(WORD id)
{
	TilemapTool* tool = dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene());
	if (nullptr != tool)
	{
		switch (id)
		{
		case MENU_ID_SAVE:
			tool->Save();
			break;
		case MENU_ID_LOAD:
			tool->Load();
			break;
		}
	}
}

HRESULT MainGame::Init()
{
	CollisionManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	ImageManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();

	TilemapTool* tool = new TilemapTool();
	SceneManager::GetInstance()->AddScene("TilemapTool", tool);
	SceneManager::GetInstance()->AddScene("BattleScene", new BattleScene());
	SceneManager::GetInstance()->AddScene("AstarScene", new AstarScene());
	SceneManager::GetInstance()->AddLoadingScene("Loading", new LoadingScene());

	SceneManager::GetInstance()->ChangeScene("AstarScene");

	//Init Camera
	Camera::GetInstance()->SetSize(SIZE{600, 600});
	Camera::GetInstance()->SetTarget(&testPlayer);

	//Test EventManager
	EventManager::GetInstance()->AddEvent(EventType::BEAT, nullptr);
	EventManager::GetInstance()->AddEvent(EventType::BEATHIT, nullptr);
	EventManager::GetInstance()->AddEvent(EventType::BEATEND, nullptr);

	// Test. SoundManager
	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::ZONE1_1, ESoundKey::ZONE1_1_SHOPKEEPER_M);
	SoundManager::GetInstance()->ChangeVolumeBgm(0.1f);
	BeatManager::GetInstance()->Init();
	BeatManager::GetInstance()->StartBeat();

	FPS = 144;

	this->hdc = GetDC(g_hWnd);

	btn = new Button();
	btn->Init();
	
	btn->Bind(std::bind(&TilemapTool::Load, tool));
	

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(WINSIZE_X, WINSIZE_Y)))
	{
		MessageBox(g_hWnd, TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);

		return E_FAIL;
	}

	return S_OK;
}

void MainGame::Release()
{
	CollisionManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
	SceneManager::GetInstance()->Release();

	// Test. SoundManager
	SoundManager::GetInstance()->Release();
	BeatManager::GetInstance()->Release();

	if (btn)
	{
		btn->Release();
	}

	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	CollisionManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	// SceneManager::GetInstance()->
	btn->Update();

	Camera::GetInstance()->Update();


	UpdateCollisionInfo();
	ItemSpawnSimulation();

	//test 
	testPlayer.Update();
	EventManager::GetInstance()->Update();
	
	// Test. SoundManager
	SoundManager::GetInstance()->Update();
	if (KeyManager::GetInstance()->IsOnceKeyDown('M'))
	{
		SoundManager::GetInstance()->ChangeSoundBgmShopkeeper();
	}
	BeatManager::GetInstance()->Update();
}

void MainGame::Render()
{
	// 백버퍼에 먼저 복사
	HDC hBackBufferDC = backBuffer->GetMemDC();

	SceneManager::GetInstance()->Render(hBackBufferDC);

	btn->Render(hBackBufferDC);
	if (bRenderCollision)
	{
		CollisionManager::GetInstance()->Render(hBackBufferDC);
		wsprintf(szText, TEXT("CollCount: %d, Active: %d Check: %d"), collCount, activeCollCount, collCheckCount);
		TextOut(hBackBufferDC, 5, 10, szText, wcslen(szText));
	}

	testPlayer.Render(hBackBufferDC);

	// 백버퍼에 있는 내용을 메인 hdc에 복사
	backBuffer->Render(hdc);

}


LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenuBar;        // 메뉴 바
	HMENU hFileMenu;  // File 메뉴 (드롭다운)
	switch (iMessage)
	{
	case WM_CREATE:
		 hMenuBar = CreateMenu();         // 메뉴 바
		 hFileMenu = CreatePopupMenu();   // File 메뉴 (드롭다운)
		 
		 // 메뉴 항목 추가
		 AppendMenu(hFileMenu, MF_STRING, MENU_ID_SAVE, L"Save");
		 AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
		 AppendMenu(hFileMenu, MF_STRING, MENU_ID_LOAD, L"Load");
		 
		 // File 메뉴를 메뉴바에 추가
		 AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, L"TilemapTool");
		 
		 SetMenu(hWnd, hMenuBar);  // 윈도우에 메뉴 설정

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
			case MENU_ID_SAVE:
			case MENU_ID_LOAD:
			TilemapMenuClicked(LOWORD(wParam));

			break;
		}
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

