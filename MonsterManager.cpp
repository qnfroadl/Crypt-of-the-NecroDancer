#include "MonsterManager.h"
#include "json.hpp"
#include <fstream>
#include <cstdlib>
#include "BossMonster.h"

using json = nlohmann::json;

HRESULT MonsterManager::Init()
{
	monsterNumber = 8;
	
	GetMonsterDataFromJson();
	for (int i = 0; i < vecMonsterInfo.size(); i++)
	{
		
			monsterVector.push_back(AddMonster(vecMonsterInfo[i]));
			//monsterVector = AddBossMonster(BossType::Dragon);
		
	}
	monsterVector.push_back(AddBossMonster(BossType::Dragon));
	return S_OK;
}

void MonsterManager::Update()
{
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->Update();
		}
	}
}

void MonsterManager::Render(HDC hdc)
{
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->Render(hdc);
		}
	}
}

void MonsterManager::Release()
{
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetActive(false);
			monsterVector[i]->Release();
			monsterVector[i] = nullptr;
		}
	}
}

void MonsterManager::SpwanBossMonster()
{
	monsterVector.resize(15);
	for (int i = 0; i < monsterVector.size(); i++) {

	monsterVector[i] =make_shared<Monster>();
	monsterVector[i]->Init(MONSTERTYPE::SKELETON);

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

void MonsterManager::SetTp()
{
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			
			monsterVector[i]->Teleport(POINT({ 1+i%5,1 }));
			if (i > 5&&i<=10)
			{
				
				monsterVector[i]->Teleport(POINT({ 1,6+i%5 }));
			}
			if (i > 10)
			{
				monsterVector[i]->Teleport(POINT({ 6+i%5,1 }));
			}
			
		}
	}
}

void MonsterManager::SetTileMap(weak_ptr<Tilemap> _tileMap)
{
	tileMap = _tileMap;
	for (int i = 0; i < monsterVector.size(); i++)
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
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetPositionManager(_positionManager);
		}
	}
}

void MonsterManager::SetPlayer(weak_ptr<Player> _player)
{
	for (int i = 0; i < monsterVector.size(); i++)
	{
		if (monsterVector[i] != nullptr)
		{
			monsterVector[i]->SetPlayer(_player);
		}
	}
}

void MonsterManager::GetMonsterDataFromJson()
{
	std::ifstream file("MonsterData.json");
	if (!file.is_open())
	{
		std::cerr << "Failed to open file." << std::endl;

	}
	json jon;
	file >> jon;
	
	MonsterImageInfo monsterInfo;

     for (const auto& item : jon["monster"])
	{
		monsterInfo.keyName = item["keyName"].get<std::string>();

		monsterInfo.imagePath = StringToTCHAR(item["imagePath"].get<std::string>());
		monsterInfo.width = item["width"].get<int>();
		monsterInfo.height = item["height"].get<int>();
		monsterInfo.imageFrameX = item["imageFrameX"].get<int>();
		monsterInfo.ImageFrameY = item["imageFrameY"].get<int>();
		vecMonsterInfo.push_back(monsterInfo);
	
	 }
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
