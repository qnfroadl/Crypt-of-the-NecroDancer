#include "Shovel.h"

#include "Block.h"
#include "Tile.h"
#include "EventManager.h"
#include "Camera.h"

HRESULT Shovel::Init()
{
	SetShovelType(ShovelType::NORMAL);

	return S_OK;
}

void Shovel::Update()
{

}

void Shovel::Render(HDC hdc)
{

}

void Shovel::SetShovelType(ShovelType _shovelType)
{
	switch (_shovelType)
	{
		case ShovelType::NORMAL:
		case ShovelType::BLOOD:
		case ShovelType::OBSIDIAN:
			hardness = 1;
			break;
		case ShovelType::GLASS:
			hardness = 3;
			break;
		case ShovelType::TITANIUM:
			hardness = 2;
			break;
		case ShovelType::SHARD:
			hardness = 1;
			break;
	}

	shovelType = _shovelType;

}

void Shovel::Interact(GameActor* actor)
{
	if (ActorType::TILE == actor->GetType())
	{
		Tile* tile = static_cast<Tile*>(actor);

		Block* block = tile->GetBlock();
		// 벽돌의 경도가, 삽의 경도보다 작거나 같다 -> 뿌셔뿌셔. 먹고싶다
		if (block && block->GetHardness() <= hardness)
		{
			block->Destroy();
			tile->SetBlock(nullptr);
		}
		else 
		{
			// 벽돌이 너무 튼튼해서, 실패 이벤트.
			EventManager::GetInstance()->AddEvent(EventType::BLOCKDESTROYFAILED, nullptr, true);
			Camera::GetInstance()->Shake(0.2, 5);

		}
	}
}
