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
#include "PlayerManager.h"
#include "MonsterManager.h"

#include "SoundManager.h"
#include "BeatManager.h"

#include "LobbyScene.h"
#include "LevelScene.h"

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
	else if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
	{
		LobbyScene* scene = static_cast<LobbyScene*>(SceneManager::GetInstance()->GetScene("LobbyScene"));
		if (scene)
		{
			SceneManager::GetInstance()->ChangeScene("LobbyScene", "Loading");
		}
		

	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RSHIFT))
	{
		SceneManager::GetInstance()->ChangeScene("AstarScene", "Loading");
	}
	else if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
	{
			SceneManager::GetInstance()->ChangeScene("LevelScene", "Loading");
	}
}

//void MainGame::TilemapMenuClicked(WORD id)
//{
//	TilemapTool* tool = dynamic_cast<TilemapTool*>(SceneManager::GetInstance()->GetCurrentScene());
//	if (nullptr != tool)
//	{
//		switch (id)
//		{
//		case MENU_ID_SAVE:
//			tool->Save();
//			break;
//		case MENU_ID_LOAD:
//			tool->Load();
//			break;
//		}
//	}
//}

void MainGame::InitResource()
{
	// 필수적으로 로딩되는 이미지들 초기화 하기
	const static int BASE_SCALE = 4;

	ImageManager::GetInstance()->AddImage(EImageKey::CADENCE_HEAD, L"Image/Player/cadence_heads.bmp", 96 * BASE_SCALE,  48 * BASE_SCALE, 4, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage(EImageKey::CADENCE_BODY, L"Image/Player/cadence_bodys.bmp", 96 * BASE_SCALE , 240 * BASE_SCALE, 4, 10, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("coin1", L"Image/Player/item/resource_coin1.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin2", L"Image/Player/item/resource_coin2.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin3", L"Image/Player/item/resource_coin3.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin4", L"Image/Player/item/resource_coin4.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin5", L"Image/Player/item/resource_coin5.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin6", L"Image/Player/item/resource_coin6.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin7", L"Image/Player/item/resource_coin7.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin8", L"Image/Player/item/resource_coin8.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin9", L"Image/Player/item/resource_coin9.bmp",  24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin10", L"Image/Player/item/resource_coin10.bmp", 24* BASE_SCALE, 48* BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin30", L"Image/Player/item/resource_coin30.bmp", 24 * BASE_SCALE, 48 * BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin50", L"Image/Player/item/resource_coin50.bmp", 24 * BASE_SCALE, 48 * BASE_SCALE, 1, 2, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("coin100", L"Image/Player/item/resource_coin100.bmp", 24 * BASE_SCALE, 48 * BASE_SCALE, 1, 2, true, RGB(255, 0, 255));

}

void MainGame::InitKeyMapping()
{
	g_mapKey.insert({ { PlayerIndex::PLAYER1, InputKey::UP }, VK_UP });
	g_mapKey.insert({ { PlayerIndex::PLAYER1, InputKey::DOWN }, VK_DOWN });
	g_mapKey.insert({ { PlayerIndex::PLAYER1, InputKey::LEFT }, VK_LEFT });
	g_mapKey.insert({ { PlayerIndex::PLAYER1, InputKey::RIGHT }, VK_RIGHT });

	g_mapKey.insert({ { PlayerIndex::PLAYER2, InputKey::UP }, 'W' });
	g_mapKey.insert({ { PlayerIndex::PLAYER2, InputKey::DOWN }, 'S'});
	g_mapKey.insert({ { PlayerIndex::PLAYER2, InputKey::LEFT }, 'A'});
	g_mapKey.insert({ { PlayerIndex::PLAYER2, InputKey::RIGHT }, 'D'});
}

HRESULT MainGame::Init()
{
	InitResource();
	InitKeyMapping();

	CollisionManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	ImageManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();

	playerManager = std::make_shared<PlayerManager>();
	playerManager->Init();
	monsterManager = std::make_shared<MonsterManager>();
	monsterManager->Init();


	SceneManager::GetInstance()->AddScene("TilemapTool", new TilemapTool());
	SceneManager::GetInstance()->AddScene("BattleScene", new BattleScene());
	SceneManager::GetInstance()->AddScene("AstarScene", new AstarScene());
	SceneManager::GetInstance()->AddScene("AstarScene", new AstarScene());

	LobbyScene* lobby = new LobbyScene();
	lobby->SetPlayerManager(playerManager);
	
	SceneManager::GetInstance()->AddScene("LobbyScene", lobby);

	LevelScene* level = new LevelScene();
	level->SetPlayerManager(playerManager);
	level->SetMonsterManager(monsterManager);
	SceneManager::GetInstance()->AddScene("LevelScene", level);

	SceneManager::GetInstance()->AddLoadingScene("Loading", new LoadingScene());

	//Init Camera
	Camera::GetInstance()->SetSize(SIZE{SCENE_WIDTH, SCENE_HEIGHT});

	//Test EventManager
	EventManager::GetInstance()->AddEvent(EventType::BEAT, nullptr);
	EventManager::GetInstance()->AddEvent(EventType::BEATHIT, nullptr);
	EventManager::GetInstance()->AddEvent(EventType::BEATEND, nullptr);

	// Test. SoundManager
	SoundManager::GetInstance()->Init();
	SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::ZONE1_1, ESoundKey::ZONE1_1_SHOPKEEPER_M);
	SoundManager::GetInstance()->ChangeVolumeBgm(0.1f);


	

	

	Camera::GetInstance()->SetTarget(playerManager->GetPlayer(PlayerIndex::PLAYER1));

	FPS = 144;

	this->hdc = GetDC(g_hWnd);

	//btn = new Button();
	//btn->Init();
	//
	//btn->Bind(std::bind(&TilemapTool::Load, tool));
	
	// 로비 씬 로딩.
	
	SceneManager::GetInstance()->ChangeScene("LobbyScene");
	//SceneManager::GetInstance()->ChangeScene("LevelScene");


	backBuffer = new Image();
	if (FAILED(backBuffer->Init(SCENE_WIDTH, SCENE_HEIGHT)))
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
	KeyManager::GetInstance()->Update();

	CollisionManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	// SceneManager::GetInstance()->
	//btn->Update();
	
	/*playerManager->Update();
	monsterManager->Update();*/
	Camera::GetInstance()->Update();

	UpdateCollisionInfo();
	ItemSpawnSimulation();

	//test 
	EventManager::GetInstance()->Update();
	
	// Test. SoundManager
	SoundManager::GetInstance()->Update();
	if (KeyManager::GetInstance()->IsOnceKeyDown('M'))
	{
		SoundManager::GetInstance()->ChangeSoundBgmShopkeeper();
	}
}

void MainGame::Render()
{
	// 백버퍼에 먼저 복사
	HDC hBackBufferDC = backBuffer->GetMemDC();

	SceneManager::GetInstance()->Render(hBackBufferDC);
	
	//playerManager->Render(hBackBufferDC);
	//monsterManager->Render(hBackBufferDC);
	//btn->Render(hBackBufferDC);
	if (bRenderCollision)
	{
		// CollisionManager::GetInstance()->Render(hBackBufferDC);
		// wsprintf(szText, TEXT("CollCount: %d, Active: %d Check: %d"), collCount, activeCollCount, collCheckCount);
		// TextOut(hBackBufferDC, 5, 10, szText, wcslen(szText));

		swprintf_s(szFPS, TEXT("FPS: %d, deltaTime: %lf"), TimerManager::GetInstance()->GetFPS(), TimerManager::GetInstance()->GetDeltaTime());
		TextOut(hBackBufferDC, 5, 10, szFPS, wcslen(szFPS));

	}


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
			//case MENU_ID_SAVE:
			//case MENU_ID_LOAD:
			//TilemapMenuClicked(LOWORD(wParam));

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

