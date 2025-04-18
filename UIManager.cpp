#include "UIManager.h"
#include "ImageManager.h"
#include "UIBase.h"
#include "UIBeatMarker.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Init()
{
}

void UIManager::Release()
{
	for (auto& ui : uiList)
	{
		if (ui)
		{
			ui->Release();
			delete ui;
			ui = nullptr;
		}
	}
}

void UIManager::Update()
{
	for (auto& ui : uiList)
	{
		if (ui)
		{
			ui->Update();
		}
	}
}

void UIManager::Render(HDC hdc)
{
	for (auto& ui : uiList)
	{
		if (ui)
		{
			ui->Render(hdc);
		}
	}
}
