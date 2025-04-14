#include "BattleScene.h"

#include "ImageManager.h"
#include "CommonFunction.h"
HRESULT BattleScene::Init()
{
	background = ImageManager::GetInstance()->AddImage("Background", L"Image/BackGround.bmp", 600,800);
	if (nullptr == background)
	{
		return E_FAIL;
	}
	
	Sleep(1000);
	SetClientRect(g_hWnd, 600, 800);

	return S_OK;
}

void BattleScene::Update()
{
	
}

void BattleScene::Render(HDC hdc)
{
	background->Render(hdc);

}

void BattleScene::Release()
{

}
