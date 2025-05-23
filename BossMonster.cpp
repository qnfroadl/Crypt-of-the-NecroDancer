﻿#include "BossMonster.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "Camera.h"
#include"EventManager.h"
#include"Tilemap.h"
#include "Player.h"
#include"Tile.h"
void BossMonster::Init(BossType p)
{
	SettingImageFrameImage();
	imageInfo = FindImageInfo(p);
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width*3 , imageInfo.height*3 ,
		imageInfo.imageFrameX, imageInfo.ImageFrameY, true, RGB(255, 0, 255));
	attackImage = ImageManager::GetInstance()->AddImage("Monster_Attack", TEXT("Image/Monster/swipe_enemy.bmp"), 135*3, 24*3,
		5, 1, true, RGB(255, 0, 255));
	
	state=BossState::IDLE;
	damage = 1;
	
	
	moveDelay = 3;
	beatCount = 0;
	
	SettingFrame(p);
	SetActive(true);

	breathRange = 0;
	animationFrame = minFrame;

	attackAnimationFrame = 0;
	blastAnimFrame = 0;
	elapsedTime = 0;
	changeTime = 0;
	
	isLeft = false;
	isBlast = false;
	isAttack = false;
	EventManager::GetInstance()->BindEvent(this, EventType::BEATMISS, std::bind(&BossMonster::OnBeat, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATHIT, std::bind(&BossMonster::OnBeat, this));


}

void BossMonster::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATMISS);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATHIT);
}

void BossMonster::Update()
{
	if (false == IsActive())
	{
		return;
	}

	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (isBlast)
	{
		if (elapsedTime > 0.09f)
		{
			blastAnimFrame++;
			if (blastAnimFrame >= fire[0]->GetMaxFrameX())
			{
				isBlast = false;
				blastAnimFrame = 0;
				SettingFrame(bossType);
			}
		}
	}
	if(isAttack)
	{
		if (elapsedTime > 0.09f)
		{
			attackAnimationFrame++;
			if (attackAnimationFrame >= attackImage->GetMaxFrameX())
			{
				isAttack = false;
				attackAnimationFrame = 0;
			}
		}
	}
	if (elapsedTime > 0.1f)
	{
		animationFrame++;
		if (animationFrame >= maxFrame)
		{
			if (state == BossState::Skill)
			{	
				
				//AttackTarget();
				state = BossState::IDLE;
				SettingFrame(bossType);
			}
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
			textX = target.lock()->GetTileIndex().x - GetTileIndex().x;
			textY = target.lock()->GetTileIndex().y - GetTileIndex().y;
			if ((textX >= -5 && textX < 0) && textY == 0&&!isLeft)
			{
				
				state = BossState::Skill;
				break;
			}
			else if ((textX > 0 && textX <= 5) && textY == 0 && isLeft)
			{
				
				state = BossState::Skill;
				break;
			}
			else {
				POINT nextIndex = Trace();
				FPOINT nextPos = tileMap.lock()->GetTilePos(nextIndex);
				if (isAttack=Monster::AttackTarget(nextIndex))
				{
					SetJumpData(GetPos().x, GetPos().y);
				}
				else
				{
					SetJumpData(nextPos.x, nextPos.y);
					SetTileIndex(nextIndex);
				}
			}
		}

		break;
	case BossState::JUMP:
		JumpAnim();
		break;
	case BossState::Skill:
		
		if (!isBlast&&animationFrame >=5 )
		{
			isBlast = true;
		}
		minFrame = 3;
		maxFrame = image->GetMaxFrameX();
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
		image->FrameRender(hdc, pos.x, pos.y - jumpData.height-30, animationFrame, 0,isLeft);
		if (isBlast)
		{
			FireImageRender(7, hdc, pos, blastAnimFrame);
		}
		if (isAttack)
		{
			FPOINT attackPos = Camera::GetInstance()->GetScreenPos(FPOINT(target.lock()->GetPos()));
			attackImage->FrameRender(hdc, attackPos.x, attackPos.y-10, attackAnimationFrame, 0, isLeft);
		}
		
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
	FindWall(isLeft, 5);
}

void BossMonster::SettingImageFrameImage()
{
	ImageManager::GetInstance()->AddImage("fire0", L"Image/BossMonster/fire0.bmp", 168*3, 24*2, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire1", L"Image/BossMonster/fire1.bmp", 168*3, 24*2, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire2", L"Image/BossMonster/fire2.bmp", 168*3, 24*2, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire3", L"Image/BossMonster/fire3.bmp", 168*3, 24*2, 7, 1, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("fire4", L"Image/BossMonster/fire4.bmp", 168*3, 24*2, 7, 1, true, RGB(255, 0, 255));
	for (int i = 0; i < 5; ++i) {													  
		fire[i] = ImageManager::GetInstance()->FindImage("fire" + std::to_string(i));
	}
}

void BossMonster::FireImageRender(int index, HDC hdc, FPOINT pos, int animationFrame)
{
	int frameWidth = fire[0]->GetWidth() / fire[0]->GetMaxFrameX();

	for (int i = 0; i < breathRange; i++) 
	{
		if (!isLeft)
		{
			if(i==0)
				fire[i]->FrameRender(hdc, pos.x - (frameWidth * (i + 1)), pos.y - jumpData.height,animationFrame, 0, true);
			else
				fire[i]->FrameRender(hdc, pos.x - (frameWidth * (i + 1)) , pos.y - jumpData.height, animationFrame, 0, false);
		}
		else
		{
			fire[i]->FrameRender(hdc, pos.x + (frameWidth * (i + 1)), pos.y - jumpData.height, animationFrame, 0, false);
		}
	}
}

void BossMonster::AttackTarget()
{
	if (isBlast)
	{
		textX = target.lock()->GetTileIndex().x - this->GetTileIndex().x;
		textY = target.lock()->GetTileIndex().y - this->GetTileIndex().y;
		if (((textX >= -5 && textX < 0) || (textX > 0 && textX <= 5)) && textY == 0)
		{
			target.lock()->TakeDamage(damage);
		}
	}
}

void BossMonster::FindWall(bool isLeft, int maxRange)
{

	/*breathRange = 0;
	if (_isLeft)
	{
		for (int i = 0; i < 5; i++)
		{

			if(tileMap.lock()->GetTile({ GetTileIndex().x - i, GetTileIndex().y })->GetBlock()!=nullptr)
			{
				return;
			}
			breathRange++;
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{

			if (tileMap.lock()->GetTile({ GetTileIndex().x + i, GetTileIndex().y })->GetBlock() != nullptr)
			{
				return;
			}
			breathRange++;
		}
	}
}*/
	breathRange = 0;




	auto map = tileMap.lock();
	if (!map) return;

	int direction = isLeft ? -1 : 1;
	POINT current = GetTileIndex();

	for (int i = 1; i <= maxRange; ++i)
	{
		POINT checkIndex = { current.x + direction * i, current.y };
		auto tile = map->GetTile(checkIndex);

		if (!tile || tile->GetBlock() != nullptr)
			return;

		breathRange++;
	}
	cout << "breathRange: " << breathRange << endl;
}

void BossMonster::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	
	Teleport(POINT({6,6}));
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
