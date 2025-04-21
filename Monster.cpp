#include "Monster.h"
#include "ImageManager.h"
#include "Camera.h"
#include "CommonFunction.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "EventManager.h"
#include "PositionManager.h"
#include "Tilemap.h"
#include "Player.h"


void Monster::Init(MONSTERTYPE p)
{
	imageInfo = FindImageInfo(p);
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width * 2, imageInfo.height * 2,
		imageInfo.imageFrameX, imageInfo.ImageFrameY, true, RGB(255, 0, 255));
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

	animationFrame = minFrame;

	EventManager::GetInstance()->BindEvent(EventType::BEATMISS, std::bind(&Monster::OnBeat, this));
	EventManager::GetInstance()->BindEvent(EventType::BEATHIT, std::bind(&Monster::OnBeat, this));
	
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
			
			if (beatCount/2>=moveDelay)
			{

				beatCount = 0;
				int imageFrame = image->GetMaxFrameX();
				
				if (imageFrame > 4)
				{
					minFrame = imageFrame / 2 + 1;
					maxFrame = imageFrame;
				}
			}
		break;
	case MonsterState::ACTIVE:
		if(beatCount / 2 >= moveDelay)//Used for testing; modify before use when connecting to the beat.
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
		
		FPOINT pos = Camera::GetInstance()->GetScreenPos(FPOINT(GetPos()));
		
		image->FrameRender(hdc, pos.x, pos.y - jumpData.height, animationFrame, 0,isLeft);
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

FPOINT Monster::Trace()
{//플레이어의 좌표를 활용해서 플레이어를 추적하는 함수	
	POINT playerIndex = target.lock()->GetTileIndex();
	POINT monsterIndex = GetTileIndex();
	

}

void Monster::Dead()
{
	SetActive(false);
	state = MonsterState::DEAD;
}

void Monster::OnBeat()
{
	beatCount++;
	if (state == MonsterState::IDLE)
	{
		if (beatCount / 2 >= moveDelay)
		{
			state = MonsterState::ACTIVE;
		}
	}
	
	
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
	POINT mIndex = GetTileIndex();
	FPOINT tilePos = GetPos();
	if (_m != MONSTERTYPE::SLIME)
	{
		switch (SetDirection())
		{
		case Direction::UP:
			mIndex.y -= 1;
			break;
		case Direction::DOWM:
			mIndex.y += 1;
			break;
		case Direction::RIGHT:
			isLeft = true;
			mIndex.x += 1;
			break;
		case Direction::LEFT:
			isLeft = false;
			mIndex.x -= 1;
			break;
		}

		if (tileMap.lock()->CanMove(mIndex))
		{
			tilePos = tileMap.lock()->GetTilePos(mIndex);
			//cout << "index: " << mIndex.x << ", " << mIndex.y << " -> tilePos: " << tilePos.x << ", " << tilePos.y << endl;
			SetJumpData(tilePos.x, tilePos.y);
			SetTileIndex(mIndex);
		}
		else
		{
			cout << "Failed" << endl;
		}
	}
	

}

void Monster::SetTileIndex(const POINT& _index)
{
	POINT preIndex = GetTileIndex();

	TileActor::SetTileIndex(_index);
	positionManager.lock()->MovedTileActor(preIndex, shared_from_this());
}

void Monster::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	Teleport(POINT{ 6, 5 });

}

void Monster::Teleport(POINT index)
{
	FPOINT tilePos = tileMap.lock()->GetTilePos(index);
	SetPos(tilePos.x, tilePos.y);
	SetTileIndex(index);
}

void Monster::SetTestJump()
{
	POINT pIndex = GetTileIndex();
	FPOINT tilePos = GetPos();

	if (tileMap.lock()->CanMove(pIndex))
	{

		tilePos = tileMap.lock()->GetTilePos(pIndex);

		cout << "index: " << pIndex.x << ", " << pIndex.y << " -> tilePos: " << tilePos.x << ", " << tilePos.y << endl;

		SetJumpData(tilePos.x, tilePos.y);
		SetTileIndex(pIndex);
	}
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
