#include "BossMonster.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "Camera.h"
#include"EventManager.h"
#include"Tilemap.h"
void BossMonster::Init(BossType p)
{
	SettingImageFrameImage();
	imageInfo = FindImageInfo(p);
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width*3 , imageInfo.height*3 ,
		imageInfo.imageFrameX, imageInfo.ImageFrameY, true, RGB(255, 0, 255));
	light = 0;
	moveDelay = 3;
	beatCount = 0;
	elapsedTime = 0;
	changeTime = 0;
	SettingFrame(p);
	SetActive(true);
	animationFrame = minFrame;
	state=BossState::IDLE;
	damage = 90;
	EventManager::GetInstance()->BindEvent(EventType::BEATMISS, std::bind(&BossMonster::OnBeat, this));
	EventManager::GetInstance()->BindEvent(EventType::BEATHIT, std::bind(&BossMonster::OnBeat, this));

}

void BossMonster::Release()
{
}

void BossMonster::Update()
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (elapsedTime > 0.1f)
	{
		animationFrame++;
		if (animationFrame >= maxFrame)
		{
			animationFrame = minFrame;
		}
		elapsedTime = 0.0f;
	}
	switch (state)
	{
	case BossState::IDLE:

		if (beatCount >= moveDelay)
		{
			state = BossState::ACTIVE;
			beatCount = 0;
		}
		break;
	case BossState::ACTIVE:
		if (beatCount >= moveDelay)
		{
			POINT nextIndex = Trace();
			FPOINT nextPos = tileMap.lock()->GetTilePos(nextIndex);
			if (AttackTarget(nextIndex))
			{
				SetJumpData(GetPos().x, GetPos().y);

			}
			else
			{
				SetJumpData(nextPos.x, nextPos.y);
				SetTileIndex(nextIndex);
			}
		}
		break;
	case BossState::JUMP:
		JumpAnim();
		break;
	
	default:
		break;
	}
}

void BossMonster::Render(HDC hdc)
{
	if (IsActive())
	{
		FPOINT pos = Camera::GetInstance()->GetScreenPos(FPOINT(GetPos()));
		image->FrameRender(hdc, pos.x, pos.y - jumpData.height-30, animationFrame, 0, isLeft);
	}
}

bool BossMonster::JumpAnim()
{
	if (TileCharacter::JumpAnim())
	{
		state = BossState::IDLE;
		return true;
	}
	return false;
}

void BossMonster::SetJumpData(int dx, int dy)
{
	state = BossState::JUMP;
	TileCharacter::SetJumpData(dx, dy);
}

void BossMonster::SettingFrame(BossType _bm)
{
	minFrame = 0;
	maxFrame = image->GetMaxFrameX() / 3;
}

void BossMonster::OnBeat()
{
	beatCount++;
	if (state == BossState::IDLE)
	{
		if (beatCount >= moveDelay)
		{
			state = BossState::ACTIVE;
			beatCount = 0;
		}
	}
}

void BossMonster::SettingImageFrameImage()
{
	ImageManager::GetInstance()->AddImage("fire0", L"Image/BossMonster/fire0.bmp", 168, 24, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire1", L"Image/BossMonster/fire1.bmp", 168, 24, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire2", L"Image/BossMonster/fire2.bmp", 168, 24, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire3", L"Image/BossMonster/fire3.bmp", 168, 24, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire4", L"Image/BossMonster/fire4.bmp", 168, 24, 7, 1, true, RGB(255, 0, 255));
	for (int i = 0; i < 5; ++i) {
		fire[i] = ImageManager::GetInstance()->FindImage("fire" + std::to_string(i));
	}
}

void BossMonster::FireImageRender(int i,HDC hdc)
{
	fire[i]->FrameRender(hdc, GetPos().x, GetPos().y - jumpData.height, 0, 0, isLeft);
}

MonsterImageInfo BossMonster::FindImageInfo(BossType _bm)
{
	bossType = _bm;
	auto path = MonsterInfoTable.find(bossType);
	if (path != MonsterInfoTable.end())
		return path->second;
}

BossMonster::BossMonster()
{
}

BossMonster::~BossMonster()
{
}
