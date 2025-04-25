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

void LevelManager::Release()
{
    EventManager::GetInstance()->UnbindEvent(this, EventType::ENTERZONE);
    EventManager::GetInstance()->UnbindEvent(this, EventType::NEXTLEVEL);
}

void LevelManager::OnNextLevel(EventData* data)
{
    SceneManager::GetInstance()->GetCurrentScene()->Init();
}

void LevelManager::OnEnterZone(EventData* data)
{
    SceneManager::GetInstance()->ChangeScene("LevelScene", "Loading");

}
