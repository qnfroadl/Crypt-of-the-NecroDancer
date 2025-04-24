#include "MonsterManager.h"
#include "json.hpp"
#include <fstream>
#include <cstdlib> 

using json = nlohmann::json;

HRESULT MonsterManager::Init()
{
	monsterNumber = 8;
	monsterVector.resize(10);
	monsterInfo=GetMonsterDataFromJson();
	for (int i = 0; i < monsterNumber; i++)
	{
		if (monsterVector[i] == nullptr)
		{
			monsterVector[i] = AddMonster(monsterInfo);
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

shared_ptr<Monster> MonsterManager::AddMonster(MonsterImageInfo _info)
{
	shared_ptr<Monster>temp = std::make_shared<Monster>();
	temp->imageInit(_info);
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

MonsterImageInfo MonsterManager::GetMonsterDataFromJson()
{
	std::ifstream file("MonsterData.json");
	if (!file.is_open())
	{
		std::cerr << "Failed to open file." << std::endl;

	}
	json jon;
	file >> jon;
	
	MonsterImageInfo monsterInfo;

	
		monsterInfo.keyName = jon["keyName"].get<std::string>();

		monsterInfo.imagePath = StringToTCHAR(jon["imagePath"].get<std::string>());
		monsterInfo.width = jon["width"].get<int>();
		monsterInfo.height = jon["height"].get<int>();
		monsterInfo.imageFrameX = jon["imageFrameX"].get<int>();
		monsterInfo.ImageFrameY = jon["imageFrameY"].get<int>();
		
		return monsterInfo;
}


TCHAR* MonsterManager::StringToTCHAR(const string& str)
{
   tstring tstr;
   const char* all = str.c_str();
   int len = 1 + strlen(all);
   wchar_t* t = new wchar_t[len];
   if (NULL == t) throw std::bad_alloc();
   size_t convertedChars = 0;
   mbstowcs_s(&convertedChars, t, len, all, _TRUNCATE); 
   return (TCHAR*)t;
}

MonsterManager::MonsterManager()
{

}

MonsterManager::~MonsterManager()
{

}
