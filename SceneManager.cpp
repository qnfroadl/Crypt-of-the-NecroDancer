﻿#include "SceneManager.h"
#include "GameObject.h"
#include <thread>

GameObject* SceneManager::currentScene = nullptr;
GameObject* SceneManager::loadingScene = nullptr;
GameObject* SceneManager::nextScene = nullptr;

void LoadingThread()
{
	if (SUCCEEDED(SceneManager::nextScene->Init()))
	{
		SceneManager::currentScene = SceneManager::nextScene;
		SceneManager::loadingScene->Release();
		SceneManager::loadingScene = nullptr;
		SceneManager::nextScene = nullptr;
	}

}

void SceneManager::Init()
{
	
}

void SceneManager::Update()
{
	if (nullptr != currentScene)
	{
		currentScene->Update();
	}
}

void SceneManager::Render(HDC hdc)
{
	if (nullptr != loadingScene)
	{
		loadingScene->Render(hdc);
		return;
	}

	if (nullptr != currentScene)
	{
		currentScene->Render(hdc);
	}
}

void SceneManager::Release()
{
	auto it = mapScenes.begin();
	while (it != mapScenes.end())
	{
		it->second->Release();
		delete it->second;
		
		it++;
	}
	mapScenes.clear();

	ReleaseInstance();
}

HRESULT SceneManager::ChangeScene(string key)
{
	auto it = mapScenes.find(key);
	if (it != mapScenes.end())
	{
		if (it->second == currentScene)
		{
			return S_OK;
		}

		nextScene = it->second;
		if (SUCCEEDED(nextScene->Init()))
		{
			if (currentScene != nullptr)
			{
				currentScene->Release();
			}

			currentScene = nextScene;
			nextScene = nullptr;
		}
	}
	else
	{
		cout << "Scene not found: " << key << endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT SceneManager::ChangeScene(string key, string loadingKey)
{
	if (nullptr != loadingScene)
	{
		return E_FAIL;
	}

	auto it = mapScenes.find(key);
	if (it != mapScenes.end())
	{
		if (it->second == currentScene)
		{
			return S_OK;
		}

		// 로딩 씬 찾아서 
		auto itLoading = mapLoadingScenes.find(loadingKey);
		if (itLoading == mapLoadingScenes.end())
		{
			return ChangeScene(key);
		}


		if (SUCCEEDED(itLoading->second->Init()))
		{
			if (currentScene != nullptr)
			{
				currentScene->Release();
			}

			currentScene = itLoading->second;
			loadingScene = itLoading->second;
			nextScene = it->second;
			// 다음씬을 초기화 할 쓰레드 생성.
			
			std::thread([]()
				{
					LoadingThread();
				}).detach();

		}

	}
	else
	{
		cout << "Scene not found: " << key << endl;
		return E_FAIL;
	}

	return S_OK;
}

GameObject* SceneManager::AddScene(string key, GameObject* scene)
{
	if (nullptr == scene)
	{
		return nullptr;
	}

	auto it = mapScenes.find(key);
	if (it != mapScenes.end())
	{
		return it->second;
	}
	else
	{
		mapScenes.insert(make_pair(key, scene));
		return scene;
	}
}

GameObject* SceneManager::AddLoadingScene(string key, GameObject* scene)
{
	if (nullptr == scene)
	{
		return nullptr;
	}

	auto it = mapLoadingScenes.find(key);
	if (it != mapLoadingScenes.end())
	{
		return it->second;
	}
	
	mapLoadingScenes.insert(make_pair(key, scene));
	return scene;
	
}

GameObject* SceneManager::GetScene(string key)
{
	auto it = mapScenes.find(key);
	if (it != mapScenes.end())
	{
		return it->second;
	}

	return nullptr;
}

GameObject* SceneManager::GetCurrentScene()
{
	return currentScene;
}
