#include "PlayerCoin.h"
#include "Image.h"
#include "ImageManager.h"
#include "ItemNumUI.h"

PlayerCoin::PlayerCoin()
{
}

PlayerCoin::~PlayerCoin()
{
}

void PlayerCoin::Init()
{
	image = ImageManager::GetInstance()->AddImage("coinUI", L"Image/UI/hud_coins.bmp", 24, 24, 1, 1, true, RGB(255, 0, 255));
	itemNumUI = new ItemNumUI();
	itemNumUI->Init();

	coin = 0;
	LocateCoinInfo();
}

void PlayerCoin::Release()
{
	if (itemNumUI)
	{
		itemNumUI->Release();
		delete itemNumUI;
		itemNumUI = nullptr;
	}
}

void PlayerCoin::Update()
{
	LocateCoinInfo();
	if (itemNumUI)
	{
		itemNumUI->Update();
	}
}

void PlayerCoin::Render(HDC hdc)
{
	float width = 0;
	if (image)
	{
		image->FrameRender(hdc, position.x, position.y, 0, 0, size.x, size.y, false, true);
		width = image->GetFrameWidth() * size.x;
	}

	if (itemNumUI)
	{
		itemNumUI->Render(hdc, coin, position.x + width * 0.5f, position.y);
	}
}

void PlayerCoin::OnPlayerGoldChanged(int gold, bool useGold)
{
	coin = gold;
}

void PlayerCoin::LocateCoinInfo()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { clientHeight * 0.003f, clientHeight * 0.003f };

	position = { clientWidth - image->GetFrameWidth() * size.x * 2.5f, image->GetFrameHeight() * size.y * 0.5f };
}
