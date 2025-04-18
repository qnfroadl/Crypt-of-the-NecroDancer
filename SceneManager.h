#pragma once
#include <unordered_map>
#include "Singleton.h"
#include "config.h"

class GameObject;
class SceneManager : public Singleton<SceneManager>
{
private:
	unordered_map<string, GameObject*> mapScenes;
	unordered_map<string, GameObject*> mapLoadingScenes;
public:
	SceneManager() {};
	~SceneManager() {};

	static GameObject* currentScene;
	static GameObject* loadingScene;
	static GameObject* nextScene;

	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();

	HRESULT ChangeScene(string key);
	HRESULT ChangeScene(string key, string loadingKey);
	GameObject* AddScene(string key, GameObject* scene);
	GameObject* AddLoadingScene(string key, GameObject* scene);
	GameObject* GetScene(string key);
	GameObject* GetCurrentScene();


};
