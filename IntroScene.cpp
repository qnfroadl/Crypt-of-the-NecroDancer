#include "IntroScene.h"
#include "CommonFunction.h"
#include "Image.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "SoundManager.h"
#include "SceneManager.h"

#include <thread>

HRESULT IntroScene::Init()
{
	SetClientRect(g_hWnd, SCENE_WIDTH, SCENE_HEIGHT);
	introCutscene = ImageManager::GetInstance()->AddImage("cutscene1", L"Image/UI/Intro/cutscene1.bmp", 280800, 1080, 195, 1, true, RGB(255, 0, 255), true);
	ImageManager::GetInstance()->AddImage("cutscene2", L"Image/UI/Intro/cutscene2.bmp", 322560, 1080, 224, 1, true, RGB(255, 0, 255), true);
	SoundManager::GetInstance()->AddSound(ESoundKey::INTRO, "Sound/Bgm/intro.ogg", true);
	SoundManager::GetInstance()->PlaySoundBgm(ESoundKey::INTRO);

	cutscene1End = false;

	std::thread([&]()
		{
			SoundManager::GetInstance()->LoadSound();
		}).detach();

    return S_OK;
}

void IntroScene::Release()
{
}

void IntroScene::Update()
{
	elapsedTime += 10.f * TimerManager::GetInstance()->GetDeltaTime();
	int tempIndex = frameIndex;
	frameIndex = (int)elapsedTime;

	if (introCutscene)
	{
		frameIndex %= introCutscene->GetMaxFrameX();
	}

	if (frameIndex < tempIndex) // anim end
	{
		if (!cutscene1End)
		{
			cutscene1End = true;
			introCutscene = ImageManager::GetInstance()->FindImage("cutscene2");
			frameIndex = elapsedTime = 0;
		}
		else
		{
			SceneManager::GetInstance()->ChangeScene("MainMenuScene");
		}
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("MainMenuScene");
	}
}

void IntroScene::Render(HDC hdc)
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	float clientWidth = client.right - client.left;
	float clientHeight = client.bottom - client.top;

	if (introCutscene)
	{
		float size = clientWidth / (float)introCutscene->GetFrameWidth();
		introCutscene->FrameRender(hdc, clientWidth / 2.f, clientHeight / 2.f, frameIndex, 0, size, size, false, true);
	}
}
