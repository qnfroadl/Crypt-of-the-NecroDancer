#include "LevelManager.h"
#include "EventManager.h"
#include "EventData.h"
#include "SceneManager.h"

HRESULT LevelManager::Init()
{
    curLevel = 0;
    zone = 1;

    EventManager::GetInstance()->BindEvent(this, EventType::ENTERZONE, bind(&LevelManager::OnEnterZone, this, placeholders::_1));
    EventManager::GetInstance()->BindEvent(this, EventType::NEXTLEVEL, bind(&LevelManager::OnNextLevel, this, placeholders::_1));

    return S_OK;
}

void LevelManager::OnNextLevel(EventData* data)
{
    
}

void LevelManager::OnEnterZone(EventData* data)
{
    SceneManager::GetInstance()->ChangeScene("LevelScene", "Loading");

}
