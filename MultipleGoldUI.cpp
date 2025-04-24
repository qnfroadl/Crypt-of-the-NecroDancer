#include "MultipleGoldUI.h"

#include "ItemNumUI.h"

#include "CommonFunction.h"

HRESULT MultipleGoldUI::Init()
{
	itemNumUI = new ItemNumUI();
	itemNumUI->Init();

	goldMultiple = 1;

	return S_OK;
}

void MultipleGoldUI::Release()
{
	itemNumUI->Release();
	delete itemNumUI;
	itemNumUI = nullptr;
}

void MultipleGoldUI::Update()
{
	itemNumUI->Update();

	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	// size = { (float)clientHeight * 0.003f, (float)clientHeight * 0.003f };
	position = { (float)clientWidth * 0.5f - size.x, (float)clientHeight - size.y - 30 };
}


void MultipleGoldUI::Render(HDC hdc)
{
	if (itemNumUI)
	{
		// RenderRectAtCenter(hdc, position.x, position.y, 20, 20);
		itemNumUI->Render(hdc, goldMultiple, position.x, position.y);//);
	}
}
