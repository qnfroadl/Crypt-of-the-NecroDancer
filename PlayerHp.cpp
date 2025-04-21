#include "PlayerHp.h"
#include "Image.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SoundManager.h"

#include <fstream>


PlayerHp::PlayerHp()
{
}

PlayerHp::~PlayerHp()
{
}

void PlayerHp::Init()
{
	image = ImageManager::GetInstance()->AddImage("heart", L"Image/UI/heart_sheet.bmp", 72, 22, 3, 1, true, RGB(255, 0, 255));

	maxHp = 7.f;
	curHp = 6.5f;

	LoadBeats();
}

void PlayerHp::Release()
{
}

void PlayerHp::Update()
{
	AdjustSize();

	CheckBeats();
}

void PlayerHp::Render(HDC hdc)
{
	if (image)
	{
		float heartContainerCount = maxHp;
		float hp = curHp;
		int frameIndex = 0;
		int y = ceil(heartContainerCount / 5.f);

		for (int i = 0; i < y; ++i)
		{
			int x = min(5, heartContainerCount);
			heartContainerCount -= x;
			for (int j = 0; j < x; ++j)
			{
				// 하트 이미지 결정 꽉, 반, 빈
				if (hp >= 1.f)
				{
					frameIndex = 0;
					hp -= 1.f;
				}
				else if (hp >= 0.5f)
				{
					frameIndex = 1;
					hp -= 0.5f;
				}
				else frameIndex = 2;

				// 위치 결정
				int heartX = position.x + image->GetFrameHeight() * size.y * 1.15f * (j + 5 - x);
				int heartY = position.y + image->GetFrameHeight() * size.y * 1.15f * i;

				if (((i * 5 + j) == beatAnimIndex) && beatAnim)
				{
					image->FrameRender(hdc, heartX, heartY, frameIndex, 0, size.x * 1.1f, size.y * 1.1f, false, true);
				}
				else
				{
					image->FrameRender(hdc, heartX, heartY, frameIndex, 0, size.x, size.y, false, true);
				}
			}
		}
	}
}

void PlayerHp::OnPlayerHPChanged(float hp)
{
	curHp = hp;
}

void PlayerHp::OnPlayerMaxHPChanged(float maxHP)
{
	maxHp = maxHP;
}

void PlayerHp::AdjustSize()
{
	RECT client;
	GetClientRect(g_hWnd, &client);
	float clientWidth = client.right - client.left;
	float clientHeight = client.bottom - client.top;

	size = { clientHeight * 0.003f, clientHeight * 0.003f };
	float heartWidth = (float)image->GetFrameWidth() * size.x;
	float heartHeight = (float)image->GetFrameHeight() * size.y;

	// position 은 하트의 시작위치(좌상단)
	position = { clientWidth - heartWidth * 1.3f * 6.5f, heartHeight * 0.7f };
}

void PlayerHp::LoadBeats()
{
	queue<unsigned int> beatQueue;

	string beatFileStr = bgmPath[SoundManager::GetInstance()->GetCurrentKeyBgm()] + ".txt";
	ifstream beatFile{ beatFileStr };

	int beatBefore{};
	if (beatFile)
	{
		int beat{};
		while (!beatFile.eof())
		{
			beatFile >> beat;
			beatQueue.push(beat);

		}
		beats = beatQueue;
	}
}

void PlayerHp::CheckBeats()
{
	unsigned int curPosition = SoundManager::GetInstance()->GetBgmPosition();
	if (beats.size() > 0)
	{
		int beat = beats.front();
		// 정박
		if (abs((int)curPosition - (int)beat) <= 30.f)
		{
			beatAnim = true;
		}

		if (curPosition > beat + 30.f)
		{
			beats.pop();

			beatAnim = false;
			beatAnimIndex++;
			beatAnimIndex %= (int)ceil(curHp);
		}
	}
}
