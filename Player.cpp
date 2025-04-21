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

void Player::OnBeatHit(EventData* data)
{
	cout << "on beat hit" << endl;

	if (data)
	{
		BeatHitEventData* beatData = static_cast<BeatHitEventData*>(data);
		if (beatData->playerIndex == playerIndex)
		{
			SetJumpData(beatData->inputKey);
			cout << "beat hit" << endl;

		}
		else
		{
			// 다른 플레이어의 비트 성공 시
		}
	}
}

void Player::OnBeatMiss(EventData* data)
{
	OutputDebugStringW(L"test\n");

	cout << "on beat miss" << endl;
	// 카메라 흔들기.	
	Camera::GetInstance()->Shake(0.5f, 10);
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
	// 키 입력에 따라 점프 방향을 설정
	POINT pIndex = GetTileIndex();
	FPOINT tilePos = GetPos();
	switch (key)
	{	
	case InputKey::UP:
		pIndex.y -= 1;
		//SetJumpData(tilePos.x, tilePos.y - 100);
		break;
	case InputKey::DOWN:
		pIndex.y += 1;
		//SetJumpData(tilePos.x, tilePos.y + 100);

		break;
	case InputKey::LEFT:
		pIndex.x -= 1;
		isLeft = true;
		//SetJumpData(tilePos.x - 100, tilePos.y);

		break;
	case InputKey::RIGHT:
		pIndex.x += 1;
		isLeft = false;
		//SetJumpData(tilePos.x + 100, tilePos.y);

		break;
	}
	
	if (tileMap.lock()->CanMove(pIndex))
	{

		tilePos = tileMap.lock()->GetTilePos(pIndex);

		cout << "index: " << pIndex.x << ", " << pIndex.y << " -> tilePos: " << tilePos.x << ", " << tilePos.y << endl;

		SetJumpData(tilePos.x, tilePos.y);
		SetTileIndex(pIndex);
	}

}

void Player::SetJumpData(int dx, int dy)
{
	state = PlayerState::JUMP;
	TileCharacter::SetJumpData(dx, dy);
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

	bomb.Bind([&](const int& preValue, const int& value)
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
	POINT preIndex = GetTileIndex();

	TileActor::SetTileIndex(_index);
	positionManager.lock()->MovedTileActor(preIndex, shared_from_this());


}

void Player::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	
	// test code , ing..., TODO 임시. 시작위치 지정.
	Teleport(POINT{ 5,4 });
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

void Player::AddWeapon(Weapon* weapon)
{
	//  검집이 있는게 아니면, 기존 무기를 떨어뜨리고, 새 무기를 장착 한다.


}
