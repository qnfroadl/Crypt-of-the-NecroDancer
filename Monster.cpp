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
	monsterType = p;

	isLeft = true;
	elapsedTime = 0;
	changeTime = 0;
	SettingFrame(p);
	SetActive(true);
	player = nullptr;
	animationFrame = minFrame;

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

			if (changeTime >= 3.0f)
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
			Patrol(50,GetMonsterType());
		}
		break;
	case MonsterState::JUMP:
		JumpAnim();
		SettingFrame(GetMonsterType());
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

void Monster::SettingFrame(MONSTERTYPE _m)
{
	if (_m == MONSTERTYPE::SLIME)
		maxFrame = image->GetMaxFrameX();
	else
		maxFrame = image->GetMaxFrameX() / 2;
	minFrame = 0;
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

void Monster::Patrol(int _pos, MONSTERTYPE _m)
{
	int changeX = this->GetPos().x;
	int changeY = this->GetPos().y;
	if (_m != MONSTERTYPE::SLIME)
	{
		switch (SetDirection())
		{
		case Direction::UP:
			changeY -= _pos;
			break;
		case Direction::DOWM:
			changeY += _pos;
			break;
		case Direction::RIGHT:
			isLeft = true;
			changeX += _pos;
			break;
		case Direction::LEFT:
			isLeft = false;
			changeX -= _pos;
			break;
		}
	}
	SetJumpData(changeX, changeY);
}

Direction Monster::SetDirection()
{
	return static_cast<Direction>(rand() % 4);
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
