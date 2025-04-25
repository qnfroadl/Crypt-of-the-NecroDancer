#include "MainMenuScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "SoundManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

HRESULT MainMenuScene::Init()
{
	mainMenu = ImageManager::GetInstance()->AddImage("mainMenu", L"Image/UI/Intro/mainmenu.bmp", 480, 270, 1, 1, true, RGB(255, 0, 255));
	pressToContinue = ImageManager::GetInstance()->AddImage("pressToContinue", L"Image/UI/Intro/continue.bmp", 220, 18, 1, 1, true, RGB(255, 0, 255), true);

	SoundManager::GetInstance()->AddSound(ESoundKey::MAINMENU, "Sound/Bgm/main_menu.ogg", true);
	SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::MAINMENU);

	return S_OK;
}

void MainMenuScene::Release()
{
}

void MainMenuScene::Update()
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	alpha = 0.5f * (sinf(elapsedTime) + 1.0f);

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("LobbyScene");
	}
}

void MainMenuScene::Render(HDC hdc)
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	float clientWidth = client.right - client.left;
	float clientHeight = client.bottom - client.top;
	
	if (mainMenu)
	{
		float size = clientWidth / (float)mainMenu->GetFrameWidth();
		mainMenu->FrameRender(hdc, clientWidth / 2, clientHeight / 2, 0, 0, size, size, false, true);
	}

	if (pressToContinue)
	{
		float size = clientWidth * 0.002f;
		pressToContinue->FrameRender(hdc, clientWidth / 2, clientHeight - (float)pressToContinue->GetFrameHeight() * size * 1.2f, 0, 0, size, size, false, true, alpha, 0);
	}
}
