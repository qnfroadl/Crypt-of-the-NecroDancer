#include "MonsterManager.h"


HRESULT MonsterManager::Init()
{
	monsterNumber = 1;
	monsterVector.resize(10);
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] == nullptr)
		{
			monsterVector[i] = AddMonster(MONSTERTYPE::SKELETON);
			//monsterVector[i] = AddBossMonster(BossType::Dragon);
		}
	}
	return S_OK;
}

void MonsterManager::Update()
{
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->Update();
		}
	}
}

void MonsterManager::Render(HDC hdc)
{
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->Render(hdc);
		}
	}
}

void MonsterManager::Release()
{
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->Release();
			monsterVector[i] = nullptr;
		}
	}
}

shared_ptr<Monster> MonsterManager::AddMonster(MONSTERTYPE _type)
{
	shared_ptr<Monster>temp = std::make_shared<Monster>();
	temp->Init(_type);
	return temp;
}

shared_ptr<BossMonster> MonsterManager::AddBossMonster(BossType _type)
{
	shared_ptr<BossMonster>temp = std::make_shared<BossMonster>();
	temp->Init(_type);
	return temp;
}

void MonsterManager::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetTileMap(_tileMap);
		}
	}	
}

void MonsterManager::SetPositionManager(weak_ptr<PositionManager> _positionManager)
{
	positionManager = _positionManager;
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetPositionManager(_positionManager);
		}
	}
}

void MonsterManager::SetPlayer(weak_ptr<Player> _player)
{
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetPlayer(_player);
		}
	}
}

MonsterManager::MonsterManager()
{

}

MonsterManager::~MonsterManager()
{

}
