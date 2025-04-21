#include "ItemNumUI.h"
#include "Image.h"
#include "ImageManager.h"

#include <stack>

ItemNumUI::ItemNumUI()
{
}

ItemNumUI::~ItemNumUI()
{
}

void ItemNumUI::Init()
{
	fontImage = ImageManager::GetInstance()->AddImage("font", L"Image/UI/font.bmp", 864, 13, 95, 1, true, RGB(255, 0, 255));
}

void ItemNumUI::Release()
{
}

void ItemNumUI::Update()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	int clientWidth = client.right - client.left;
	int clientHeight = client.bottom - client.top;

	size = { clientHeight * 0.003f, clientHeight * 0.003f };
}

void ItemNumUI::Render(HDC hdc, int num, int startX, int startY)
{
	if (fontImage)
	{
		int fontWidth = fontImage->GetFrameWidth() * size.x;
		int fontHeight = fontImage->GetFrameHeight() * size.y;

		fontImage->FrameRender(hdc, startX + fontWidth * 0.7f, startY, 88, 0, size.x, size.y, false, true);

		int count = 2;
		if (num == 0)
		{
			fontImage->FrameRender(hdc, startX + count * fontWidth * 0.7f, startY, 0 + 16, 0, size.x, size.y, false, true);

		}
		else
		{
			bool validZero = false;
			stack<int> digits;
			while (num > 0)
			{
				int digit = num % 10;
				digits.push(digit);
				num /= 10;
			}

			while (!digits.empty())
			{
				int digit = digits.top();
				digits.pop();
				if (digit != 0)
				{
					validZero = true;
				}

				if (digit != 0 || validZero)
				{
					fontImage->FrameRender(hdc, startX + count * fontWidth * 0.7f, startY, digit + 16, 0, size.x, size.y, false, true);
					count++;
				}
			}
		}
	}
}
