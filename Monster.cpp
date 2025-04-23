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
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width * 3, imageInfo.height * 3,
		imageInfo.imageFrameX, imageInfo.ImageFrameY, true, RGB(255, 0, 255));
	attackImage = ImageManager::GetInstance()->AddImage("Monster_Attack", TEXT("Image/Monster/swipe_enemy.bmp"),135*3, 24*3,
		5, 1, true, RGB(255, 0, 255));
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
	damage = 0.5;
	animationFrame = minFrame;
	attackAnimationFrame = 0;

	EventManager::GetInstance()->BindEvent(this, EventType::BEATMISS, std::bind(&Monster::OnBeat, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATHIT, std::bind(&Monster::OnBeat, this));
	
}

void Monster::Release()
{

}

void Monster::Update()
{
	if (false == IsActive())
	{
		return;
	}


	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (isAttack)
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
			animationFrame = minFrame;
		}
		elapsedTime = 0.0f;
	}
	switch (state)
	{
	case MonsterState::IDLE:
			
			if (beatCount>moveDelay)
			{
				state = MonsterState::ACTIVE;
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

		if(beatCount  >= moveDelay)//Used for testing; modify before use when connecting to the beat.
		{
			POINT nextIndex = Trace();
			FPOINT nextPos = tileMap.lock()->GetTilePos(nextIndex);
			if (isAttack=AttackTarget(nextIndex))
			{
				SetJumpData(GetPos().x, GetPos().y);
				
			}
			else
			{
				SetJumpData(nextPos.x, nextPos.y);
				SetTileIndex(nextIndex);
			}
			//Patrol(50,GetMonsterType());
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
		if (isAttack)
		{
			FPOINT attackPos = Camera::GetInstance()->GetScreenPos(FPOINT(target.lock()->GetPos()));
			attackImage->FrameRender(hdc, attackPos.x, attackPos.y, attackAnimationFrame, 0, isLeft);
		}
	}
}

void Monster::SettingFrame(MONSTERTYPE _m)
{
	if (_m == MONSTERTYPE::SLIME)
		maxFrame = image->GetMaxFrameX();
	else
		maxFrame = image->GetMaxFrameX() / 2;
	minFrame = 0;
	beatCount = 0;
}

POINT Monster::Trace()
{
	POINT playerIndex = target.lock()->GetTileIndex();
	POINT monsterIndex = GetTileIndex();
	vector<POINT> range;
	for (const POINT& changeDir : dir)
	{
		POINT nextIndex = { monsterIndex.x + changeDir.x, monsterIndex.y + changeDir.y };
		if (tileMap.lock()->CanMove(nextIndex))
		{
			range.push_back(nextIndex);
		}
	}
	POINT bestMove = monsterIndex;
	int minDist = INT_MAX;
	for (const POINT& p : range) {
		int dist = abs(p.x - playerIndex.x) + abs(p.y - playerIndex.y);
		if (dist < minDist) {
			minDist = dist;
			bestMove = p;
			if (p.x - playerIndex.x > 0)
				isLeft = false;
			else if(p.x - playerIndex.x < 0)
				isLeft = true;
		}
	}
	

	return bestMove;

}

void Monster::Dead()
{
	SetActive(false);
	state = MonsterState::DEAD;
	positionManager.lock()->RemoveTileActor(shared_from_this(), true, GetTileIndex());
}

void Monster::OnBeat()
{
	beatCount++;
}

bool Monster::AttackTarget(POINT _nextIndex)
{

	POINT playerIndex = target.lock()->GetTileIndex();
	if (_nextIndex.x == playerIndex.x && _nextIndex.y == playerIndex.y)
	{
		
		target.lock()->TakeDamage(damage);
		return true;
	}
	return false;

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
		case Direction::DOWN:
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
	:curHP(1), maxHP(1)
{
	SetType(ActorType::MONSTER);
}

Monster::~Monster()
{
}

void Monster::TakeDamage(int damage)
{
	curHP -= damage;
	if (curHP <= 0)
	{
		curHP = 0;
		Dead();
	}
}

bool Monster::IsDead()
{
	return MonsterState::DEAD == state ? true : false;
}
