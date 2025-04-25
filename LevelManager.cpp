#include "LevelManager.h"
#include "EventManager.h"
#include "EventData.h"
#include "SceneManager.h"
#include "LevelScene.h"

HRESULT LevelManager::Init()
{
    curLevel = 0;
    zone = 0;
    keyName = "Lobby";
   
    EventManager::GetInstance()->BindEvent(this, EventType::ENTERZONE, bind(&LevelManager::OnEnterZone, this, placeholders::_1));
    
    return S_OK;
}

void LevelManager::Release()
{
    EventManager::GetInstance()->UnbindEvent(this, EventType::ENTERZONE);

}

void LevelManager::OnNextLevel(EventData* data)
{
    SceneManager::GetInstance()->GetCurrentScene()->Init();
}

void LevelManager::OnEnterZone(EventData* data)
{
    if (keyName == "Lobby")
    {
        SceneManager::GetInstance()->ChangeScene("LevelScene", "Loading");
        keyName = "LevelScene";
        zone++;
    }
    else if (keyName == "LevelScene")
    {
        LevelScene* clearScene = (LevelScene*)(SceneManager::GetInstance()->GetCurrentScene());
        clearScene->Release();


        clearScene->Init(zone);
        


        if (keyName == "LevelScene" && zone == 1)
        {
            SceneManager::GetInstance()->ChangeScene("BossScene", "Loading");
            keyName = "BossScene";
        }
        zone++;
    }
}
