#include "Monster.h"
#include "ImageManager.h"
#include "Camera.h"
#include "TimerManager.h"
#include "KeyManager.h"

void Monster::Init(MONSTERTYPE p)
{
	imageInfo = FindImageInfo(p);
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width*2,imageInfo.height*2,
		imageInfo.imageFrameX, imageInfo.ImageFrameY,true,RGB(255,0,255));
	state = MonsterState::IDLE;
	light = 0;
	moveDelay = 3;
	beatCount = 0;
	minFrame = 0;
	animationFrame = minFrame;
	
	elapsedTime = 0;
	changeTime = 0;
	SetActive(true);
	maxFrame = image->GetMaxFrameX()/2;
	player = nullptr;
}

void Monster::Release()
{

}

void Monster::Update()
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
	case MonsterState::IDLE:
			changeTime+= TimerManager::GetInstance()->GetDeltaTime();

			if (changeTime >= 2.0f)
			{
				state = MonsterState::ACTIVE;
				changeTime = 0;
				int imageFrame = image->GetMaxFrameX();
				
				if (imageFrame > 4)
				{
					minFrame = imageFrame / 2 + 1;
					maxFrame = imageFrame;
				}
			}
		break;
	case MonsterState::ACTIVE:
		changeTime += TimerManager::GetInstance()->GetDeltaTime();
		if(changeTime>=2.0f )//Used for testing; modify before use when connecting to the beat.
		{
			if (monsterType == MONSTERTYPE::SLIME)
			{
				SetJumpData(GetPos().x, GetPos().y);
				changeTime = 0;
			}
			else
			{
				SetJumpData(GetPos().x - 50, GetPos().y);
				changeTime = 0;
			}
		}
		break;
	case MonsterState::JUMP:
		JumpAnim();
		minFrame = 0;
		maxFrame = image->GetMaxFrameX() / 2;
		break;
	}
		
}

void Monster::Render(HDC hdc)
{
	if (IsActive())
	{
		FPOINT pos = GetPos();
		pos.x -= Camera::GetInstance()->GetPos().x;
		pos.y -= Camera::GetInstance()->GetPos().y;

		image->FrameRender(hdc, pos.x, pos.y - jumpData.height, animationFrame, 0);
	}
}

void Monster::Trace()
{

}

void Monster::Dead()
{
	SetActive(false);
	state = MonsterState::DEAD;
}

void Monster::OnBeat()
{
	
}

void Monster::AttackTarget()
{

}

bool Monster::JumpAnim()
{
	if (TileCharacter::JumpAnim())
	{
		state = MonsterState::IDLE;
		return true;
	}
	return false;
}

void Monster::SetJumpData(int dx, int dy)
{
	state = MonsterState::JUMP;
	TileCharacter::SetJumpData(dx, dy);
}

MonsterImageInfo Monster::FindImageInfo(MONSTERTYPE _m)
{
	monsterType = _m;
	auto path = MonsterInfoTable.find(_m);
	if (path != MonsterInfoTable.end())
		return path->second;
}

Monster::Monster()
{
}

Monster::~Monster()
{
}
