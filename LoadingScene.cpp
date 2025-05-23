﻿#include "LoadingScene.h"

#include "ImageManager.h"
#include "CommonFunction.h"


HRESULT LoadingScene::Init()
{ 
    bg = ImageManager::GetInstance()->AddImage("Loading", L"Image/loading.bmp", 852, 480);
    SetClientRect(g_hWnd, 852, 480);
    return S_OK;
}

void LoadingScene::Update()
{
}

void LoadingScene::Render(HDC hdc)
{
    if (nullptr != bg)
    {
        bg->Render(hdc);
    }
}

void LoadingScene::Release()
{
}
