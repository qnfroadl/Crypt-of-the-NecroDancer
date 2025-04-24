#include "BombAttack.h"

#include "ImageManager.h"
#include "EventManager.h"
#include "TimerManager.h"
#include "Camera.h"
#include "EffectManager.h"
#include "BombEffect.h"
#include "Block.h"
#include "Tile.h"
#include "Player.h"

HRESULT BombAttack::Init()
{
	image = ImageManager::GetInstance()->AddImage("bomb_start", L"Image/Player/item/bomb.bmp", 120 * 3, 48 * 3, 5, 2, true, RGB(255, 0, 255));
	

	// 설치되고 3비트면 터짐.

	// 
	EventManager::GetInstance()->BindEvent(this, EventType::BEAT, bind(&BombAttack::OnNextStep, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATHIT, bind(&BombAttack::OnNextStep, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BEATMISS, bind(&BombAttack::OnNextStep, this));

	damage = 4;
	step = 0;		// 폭발 단계.
	isExplode = false;

	return S_OK;
}

void BombAttack::Update()
{
	if (isExplode)
	{
		
	}
	else if (5 <= step)
	{
		isExplode = true;
		EffectManager::GetInstance()->AddEffect(make_shared<BombEffect>(GetPos()));
		// 상호작용도 요청 해야하는데?

		vector<POINT> range = {
				{-1, -1}, {0, -1}, {1, -1},
				{-1,  0}, {0 , 0}, {1,  0},
				{-1,  1}, {0,  1}, {1,  1}
			};

		// 즉발
		EventManager::GetInstance()->AddEvent(EventType::INTERACT, new InteractEventData(shared_from_this(), range), true);
		SetActive(false);
	}
}

void BombAttack::Render(HDC hdc)
{
	if (SightState::INVISIBLE == sightState)
	{
		return;
	}

	FPOINT pos = Camera::GetInstance()->GetScreenPos(GetPos());

	if (false == isExplode)
	{
		image->FrameRender(hdc, pos.x, pos.y, step, SightState::SHADOW == sightState ? 1 : 0, false, true);
	}
	
}

void BombAttack::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEAT);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATHIT);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEATMISS);
}

void BombAttack::OnNextStep()
{
	// 다음 단계.
	step++;

}

void BombAttack::Interact(GameActor* actor)
{
	if (IsActive() && isExplode)
	{
		if (ActorType::TILE == actor->GetType())
		{
			Tile* tile = static_cast<Tile*>(actor);
			shared_ptr<Block> block = tile->GetBlock();

			if (block)
			{
				if (block->CanDestroy(4))
				{
					block->Destroy();
					tile->SetBlock(nullptr);
					EventManager::GetInstance()->AddEvent(EventType::BLOCKDESTROYED, nullptr, true);
				}
			}
		}
		else if (ActorType::ITEM == actor->GetType())
		{
			TileItem* item = static_cast<TileItem*>(actor);
			item->Destroy();
		}
		else if (ActorType::PLAYER == actor->GetType())
		{
			Player* player = static_cast<Player*>(actor);
			player->TakeDamage(2);
		}

	}
}
