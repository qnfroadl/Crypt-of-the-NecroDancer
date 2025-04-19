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

MonsterManager::MonsterManager()
{

}

MonsterManager::~MonsterManager()
{

}
