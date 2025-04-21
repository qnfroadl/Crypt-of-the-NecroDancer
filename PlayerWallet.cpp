#include "PlayerWallet.h"
#include "Image.h"
#include "ImageManager.h"
#include "ItemNumUI.h"

PlayerWallet::PlayerWallet()
{
}

PlayerWallet::~PlayerWallet()
{
}

void PlayerWallet::Init()
{
	coinImage = ImageManager::GetInstance()->AddImage("coinUI", L"Image/UI/hud_coins.bmp", 24, 24, 1, 1, true, RGB(255, 0, 255));
	diamondImage = ImageManager::GetInstance()->AddImage("diamondUI", L"Image/UI/diamond.bmp", 25, 24, 1, 1, true, RGB(255, 0, 255));
	
	itemNumUI = new ItemNumUI();
	itemNumUI->Init();
	
	coin = 0;
	diamond = 0;
	LocateInfo();
}

void PlayerWallet::Release()
{
	if (itemNumUI)
	{
		itemNumUI->Release();
		delete itemNumUI;
		itemNumUI = nullptr;
	}
}

void PlayerWallet::Update()
{
	LocateInfo();
	if (itemNumUI)
	{
		itemNumUI->Update();
	}
}

void PlayerWallet::Render(HDC hdc)
{
	float width = 0;
	float height = 0;
	if (coinImage)
	{
		coinImage->FrameRender(hdc, position.x, position.y, 0, 0, size.x, size.y, false, true);
		width = coinImage->GetFrameWidth() * size.x;
		height = coinImage->GetFrameHeight() * size.y;
	}

	if (itemNumUI)
	{
		itemNumUI->Render(hdc, coin, position.x + width * 0.5f, position.y);
	}

	if (diamondImage)
	{
		diamondImage->FrameRender(hdc, position.x, position.y + height, 0, 0, size.x, size.y, false, true);
		width = diamondImage->GetFrameWidth() * size.x;
	}

	if (itemNumUI)
	{
		itemNumUI->Render(hdc, diamond, position.x + width * 0.5f, position.y + height);
	}
}

void PlayerWallet::OnPlayerGoldChanged(int gold, bool useGold)
{
	coin = gold;
}

void PlayerWallet::OnPlayerDiamondChanged(int _diamond)
{
	diamond = _diamond;
}

void PlayerWallet::LocateInfo()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { clientHeight * 0.003f, clientHeight * 0.003f };

	position = { clientWidth - coinImage->GetFrameWidth() * size.x * 2.5f, coinImage->GetFrameHeight() * size.y * 0.5f };
}
