#include "Player.h"

#include "ImageManager.h"
#include "Camera.h"
#include "Weapon.h"
#include "CommonFunction.h"
#include "TimerManager.h"
#include "Tilemap.h"
#include "EventData.h"
#include "EventManager.h"
#include "PositionManager.h"
#include "Shovel.h"
#include "Tile.h"
#include "SoundManager.h"

void Player::OnBeatHit(EventData* data)
{
	//cout << "on beat hit" << endl;

	if (data)
	{
		BeatHitEventData* beatData = static_cast<BeatHitEventData*>(data);
		if (beatData->playerIndex == playerIndex)
		{
			SetJumpData(beatData->inputKey);
			//cout << "beat hit" << endl;

		}
		else
		{
			// 다른 플레이어의 비트 성공 시
		}
	}
}

void Player::OnBeatMiss(EventData* data)
{

	//cout << "on beat miss" << endl;
	// 카메라 흔들기.	
	Camera::GetInstance()->Shake(0.2f, 10);
}

void Player::OnComboReset(EventData* data)
{
	goldMultiple.Set(2);	// 최소 값 2.
}
void Player::OnComboUp(EventData* data)
{
	if (goldMultiple.Get() < 4)
	{
		goldMultiple.Set(goldMultiple.Get() + 1);
	}
}

bool Player::JumpAnim()
{
	if(TileCharacter::JumpAnim())
	{
		state = PlayerState::IDLE;
		return true;
	}
	return false;
}

void Player::SetJumpData(InputKey key)
{
	bool isAttack = false;
	Direction dir = Direction::RIGHT;
	// 키 입력에 따라 점프 방향을 설정
	POINT pIndex = GetTileIndex();
	FPOINT tilePos = GetPos();
	switch (key)
	{
	case InputKey::UP:
		pIndex.y -= 1;
		dir = Direction::UP;
		break;
	case InputKey::DOWN:
		pIndex.y += 1;
		dir = Direction::DOWN;
		break;
	case InputKey::LEFT:
		pIndex.x -= 1;
		isLeft = true;
		dir = Direction::LEFT;
		break;
	case InputKey::RIGHT:
		pIndex.x += 1;
		isLeft = false;
		dir = Direction::RIGHT;
		break;
	}
	
	if (Attack(pIndex, dir))
	{
		// 공격 했다면 턴 종료.
		return;
	}

	if (tileMap.lock()->CanMove(pIndex))
	{
		// 다음 타일로 이동.
		tilePos = tileMap.lock()->GetTilePos(pIndex);

		cout << "index: " << pIndex.x << ", " << pIndex.y << " -> tilePos: " << tilePos.x << ", " << tilePos.y << endl;

		SetJumpData(tilePos.x, tilePos.y);
		SetTileIndex(pIndex);	// 타일 인덱스 선 변경.
		
	}
	else 
	{
		// 벽돌이 있다는 뜻.
		SoundManager::GetInstance()->PlaySoundEffect(ESoundKey::MOV_DIG_FAIL);

		if (shovel)
		{
			tileMap.lock()->InteractTile(pIndex, shovel.get());
		}
		else 
		{
			// 삽도 없는데 부시려고 함. 실패 이벤트.
			EventManager::GetInstance()->AddEvent(EventType::BLOCKDESTROYFAILED, nullptr, true);
			Camera::GetInstance()->Shake(0.2, 5);
		}
	}
}

void Player::SetJumpData(int dx, int dy)
{
	state = PlayerState::JUMP;
	TileCharacter::SetJumpData(dx, dy);
}

void Player::CalcAttackRange(Direction dir, vector<POINT>& range)
{
	for (auto& pt : range)
	{
		switch (dir)
		{
		case Direction::RIGHT:
			break;
		case Direction::DOWN:
			pt = {-pt.y, pt.x};
			break;
		case Direction::LEFT:
			pt = {-pt.x, -pt.y};
			break;
		case Direction::UP:
			pt = { pt.y, -pt.x };
			break;
		}
	}

}

void Player::NotifyAll()
{
	gold.Notify();
	goldMultiple.Notify();
	hp.Notify();
	maxHP.Notify();
	diamond.Notify();
	bombCount.Notify();
}

Player::Player()
	: state{}, attack(1), name("Cadence"), curFrame(0), speed(20), isLeft(false), elapsedTime(0)
{
	hp.Set(3);
	maxHP.Set(3);
	diamond.Set(0);

	SetType(ActorType::PLAYER);
}

Player::~Player()
{
	Release();
}

HRESULT Player::Init()
{
	image = ImageManager::GetInstance()->FindImage(EImageKey::CADENCE_HEAD);	//캐릭터 머리
	body = ImageManager::GetInstance()->FindImage(EImageKey::CADENCE_BODY);		//캐릭터 몸통
	
	if (nullptr == image || nullptr == body)
	{
		return E_FAIL;
	}

	EventManager::GetInstance()->BindEvent(this, EventType::BEATHIT, std::bind(&Player::OnBeatHit, this, std::placeholders::_1));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATMISS, std::bind(&Player::OnBeatMiss, this, std::placeholders::_1));

	EventManager::GetInstance()->BindEvent(this, EventType::BLOCKDESTROYFAILED, std::bind(&Player::OnComboReset, this, std::placeholders::_1));


	// 기본삽정도는 줘야지.
	shovel = make_shared<Shovel>();
	shovel->Init();

	// 기본무기
	auto tempWeapon = make_shared<Weapon>();
	tempWeapon->Init();
	tempWeapon->Interact(this);

	#pragma region Bind

	gold.Bind([&](const int& preValue, const int& value)
		{
			for (auto observer : observers)
			{
				observer->OnPlayerGoldChanged(value, preValue < value ? false : true);
			}
		});

	goldMultiple.Bind([&](const int& preValue, const int& value) 
	{
		for (auto observer : observers)
		{
			observer->OnPlayerGoldMultipleChanged(value);
		}
	});

	hp.Bind([&](const float& preValue, const float& value)
		{
			for (auto observer : observers)
			{
				observer->OnPlayerHPChanged(value);
			}
		});

	maxHP.Bind([&](const float& preValue, const float& value)
		{
			for (auto observer : observers)
			{
				observer->OnPlayerMaxHPChanged(value);
			}
		});

	diamond.Bind([&](const int& preValue, const int& value)
		{
			for (auto observer : observers)
			{
				observer->OnPlayerDiamondChanged(value);
			}
		});

	bombCount.Bind([&](const int& preValue, const int& value)
		{
			for (auto observer : observers)
			{
				observer->OnPlayerBombChanged(value);
			}
		});
	
#pragma endregion

	return S_OK;
}

void Player::Update()
{
	if (false == IsActive())
	{
		return;
	}

	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

	if (elapsedTime >= 0.1f)
	{
		elapsedTime = 0;
		curFrame++;
		if (image->GetMaxFrameX() <= curFrame)
		{
			curFrame = 0;
		}
	}
	
	switch (state)
	{
	case PlayerState::IDLE:
		break;
	case PlayerState::JUMP:
		JumpAnim();
		break;
	}

	if (weapon)
	{
		weapon->Update();
	}
}

void Player::Render(HDC hdc)
{
	if (IsActive())
	{
		FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());

		// 렌더 할 때 점프데이터의 높이만큼 빼서 렌더 해줘야 점프처럼 보인다
		// 캐릭터 몸통
		body->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);
		// 캐릭터 머리
		image->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);

		if (weapon)
		{
			weapon->Render(hdc);
		}
	}

}

void Player::Release()
{

}

void Player::SetTileIndex(const POINT& _index)
{
	POINT preIndex = GetTileIndex();	// 이전 타일인덱스 가져오기.
	TileActor::SetTileIndex(_index);	// 타일 인덱스 업데이트
	
	positionManager.lock()->MovedTileActor(preIndex, shared_from_this());	// 변경된 내용 포지션 매니저에 알리기.
	tileMap.lock()->InteractTile(_index, this);
	EventManager::GetInstance()->AddEvent(EventType::PLAYERMOVED, nullptr);

}

void Player::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	Teleport(tileMap.lock()->GetPlayerStartIndex());
}

void Player::Teleport(POINT index)
{
	SetPlayerState(PlayerState::IDLE);

	// 플레이어의 위치를 타일맵의 타일에 맞춰서 이동
	FPOINT tilePos = tileMap.lock()->GetTilePos(index);
	SetPos(tilePos.x, tilePos.y);
	SetTileIndex(index);
	cout << "Teleport : " << tilePos.x << ", " << tilePos.y << endl;
}

void Player::AddObserver(IPlayerObserver* observer)
{
	 if (observer) 
	 { 
		observers.push_back(observer);
		NotifyAll();
	 } 
}

void Player::BindRelease()
{
	observers.clear();
}

bool Player::Attack(POINT index, Direction dir)
{
	// 무기가 있고, 공격 할 수 있는 몬스터가 범위내에 있는지 확인하고, 있으면 때리기.

	bool isAttack = false;

	if (weapon)
	{
		shared_ptr<Weapon> wp = static_pointer_cast<Weapon>(weapon);
		vector<POINT> range = wp->GetRange();
		CalcAttackRange(dir, range);

		shared_ptr<TileActor> monster;

		for (auto atkPoint : range)
		{
			atkPoint.x += GetTileIndex().x;
			atkPoint.y += GetTileIndex().y;

			monster = positionManager.lock()->GetActor(atkPoint, ActorType::MONSTER);
			if (monster)
			{
				weapon->Swipe(monster->GetPos(), dir);
				weapon->Interact(monster.get());
				isAttack = true;
			}
		}

	}

	return isAttack;
}

void Player::UseItem()
{

}

void Player::TakeDamage(float damage)
{
	Camera::GetInstance()->Shake(0.2f, 10);

	float dmamgedHp = hp.Get() - damage;
	if (dmamgedHp <= 0)
	{
		dmamgedHp = 0;
		SetActive(false);
	}
	
	hp.Set(dmamgedHp);
	
}

bool Player::IsDead()
{
	return hp.Get() <= 0;
}

void Player::AddWeapon(shared_ptr<Weapon> _weapon)
{
	// 새로 줏은 무기를 포지션매니저에서 제거한다.
	if (positionManager.lock())
	{
		positionManager.lock()->RemoveTileActor(_weapon);
	}


	// 기존 무기를 떨어뜨리고, 새 무기를 장착 한다.
	if (weapon)
	{
		FPOINT pos = tileMap.lock()->GetTilePos(GetTileIndex());
		weapon->SetTileIndex(GetTileIndex());
		weapon->SetPos(pos);
		positionManager.lock()->AddTileActor(weapon);
		weapon->Drop();
	}
	
	weapon = _weapon;
}

void Player::SetPlayerState(PlayerState _state)
{
	if (state != _state && state == PlayerState::JUMP)
	{
		// 스테이트가 바뀌는데? 이전스테이트가 점프다.
		// 점프 데이터 리셋.
		ResetJumpData();
	}


	state = _state;
}
