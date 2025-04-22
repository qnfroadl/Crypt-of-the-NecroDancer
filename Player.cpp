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
	Camera::GetInstance()->Shake(0.5f, 10);
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

	// 키 입력에 따라 점프 방향을 설정
	POINT pIndex = GetTileIndex();
	FPOINT tilePos = GetPos();
	switch (key)
	{
	case InputKey::UP:
		pIndex.y -= 1;
		break;
	case InputKey::DOWN:
		pIndex.y += 1;
		break;
	case InputKey::LEFT:
		pIndex.x -= 1;
		isLeft = true;
		break;
	case InputKey::RIGHT:
		pIndex.x += 1;
		isLeft = false;
		break;
	}
	
	// 무기가 있고, 공격 할 수 있는 몬스터가 범위내에 있는지 확인하고, 있으면 때리기.
	if (weapon)
	{
		shared_ptr<Weapon> wp = static_pointer_cast<Weapon>(weapon);
		vector<POINT> range = wp->GetRange();
		CalcAttackRange(key, range);

		shared_ptr<TileActor> monster;

		for (auto atkPoint : range)
		{
			monster = positionManager.lock()->GetActor(pIndex, ActorType::MONSTER);
			if (monster)
			{
				weapon->Interact(monster.get());
				isAttack = true;
			}
		}

	}
	
	if (isAttack)
	{
		return;
	}

	// 공격 했다면 리턴.


	// 사실 캔 무브 체크하기 전에. 공격체크부터 해야함.
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

void Player::CalcAttackRange(InputKey key, vector<POINT>& range)
{
	for (auto& pt : range)
	{
		switch (key)
		{
		case InputKey::RIGHT:
			break;
		case InputKey::DOWN:
			pt = {-pt.y, pt.x};
			break;
		case InputKey::LEFT:
			pt = {-pt.x, -pt.y};
			break;
		case InputKey::UP:
			pt = { pt.y, -pt.x };
			break;
		}
	}

}

Player::Player()
	: state{}, attack(1), name("Cadence"), curFrame(0), speed(20), isLeft(false), elapsedTime(0)
{
	hp.Set(100);
	maxHP.Set(100);
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

	EventManager::GetInstance()->BindEvent(EventType::BEATHIT, std::bind(&Player::OnBeatHit, this, std::placeholders::_1));
	EventManager::GetInstance()->BindEvent(EventType::BEATMISS, std::bind(&Player::OnBeatMiss, this, std::placeholders::_1));

	EventManager::GetInstance()->BindEvent(EventType::BLOCKDESTROYFAILED, std::bind(&Player::OnComboReset, this, std::placeholders::_1));


	// 기본삽정도는 줘야지.
	shovel = make_shared<Shovel>();
	shovel->Init();

	// 기본무기
	weapon = make_shared<Weapon>();
	weapon->Init();

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
}

void Player::Render(HDC hdc)
{
	
	FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());
	
	RenderRectAtCenter(hdc, pos.x, pos.y, 80,80);

	// 렌더 할 때 점프데이터의 높이만큼 빼서 렌더 해줘야 점프처럼 보인다
	// 캐릭터 몸통
	body->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);
	// 캐릭터 머리
	image->FrameRender(hdc, pos.x, pos.y - jumpData.height, curFrame, 0, isLeft, true);

}

void Player::Release()
{

}

void Player::SetTileIndex(const POINT& _index)
{
	POINT preIndex = GetTileIndex();	// 이전 타일인덱스 가져오기.
	TileActor::SetTileIndex(_index);	// 타일 인덱스 업데이트
	positionManager.lock()->MovedTileActor(preIndex, shared_from_this());	// 변경된 내용 포지션 매니저에 알리기.
	
}

void Player::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	Teleport(tileMap.lock()->GetPlayerStartIndex());
}

void Player::Teleport(POINT index)
{
	// 플레이어의 위치를 타일맵의 타일에 맞춰서 이동
	FPOINT tilePos = tileMap.lock()->GetTilePos(index);
	SetPos(tilePos.x, tilePos.y);
	SetTileIndex(index);
}

void Player::Attack()
{

}

void Player::UseItem()
{

}

void Player::TakeDamage(float damage)
{
	float dmamgedHp = hp.Get() - damage;
	if (dmamgedHp < 0)
	{
		dmamgedHp = 0;
	}

	hp.Set(dmamgedHp);
	
}

bool Player::IsDead()
{
	return hp.Get() <= 0;
}

void Player::AddWeapon(shared_ptr<Weapon> _weapon)
{
	// 기존 무기를 떨어뜨리고, 새 무기를 장착 한다.
	if (weapon)
	{
		weapon->SetTileIndex(GetTileIndex());
		positionManager.lock()->AddTileActor(weapon);
		weapon->Drop();
	}
	

	weapon = _weapon;
}
