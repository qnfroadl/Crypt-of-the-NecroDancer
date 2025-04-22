#include "KeyManager.h"
#include "conio.h"
#include "TimerManager.h"

HRESULT KeyManager::Init()
{
	keyUp.set();		// true로 세팅.
	keyDown.reset();	// false로 세팅.

	expireTime = 0.05f;

	return S_OK;
}

void KeyManager::Release()
{
	Singleton::ReleaseInstance();
}

void KeyManager::Update()
{
	// 플레이어의 입력키에 대해서만 키버퍼에 저장
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

	for (auto key : g_mapKey)
	{
		if (IsOnceKeyDown(key.second))
		{
			keyBuffer.push_back({ elapsedTime,key.second });
		}
	}

	while (!keyBuffer.empty() && (elapsedTime > keyBuffer.front().first + expireTime * 2.f))
	{
		keyBuffer.pop_front();
	}
}

bool KeyManager::IsOnceKeyDown(int key)
{
	// 함수 호출 시점에 가상키 VK_??? 가 어떤 상태인지 확인.
	// 1. 0x0000: 이전 프레임에 누른적이 없고 호출 시점에도 눌려있지 않음.
	// 2. 0x0001: 이전 프레임에 누른적이 있고, 호출 시점에는 눌려있지 않음.
	// 3. 0x8000: 이전 프레임에 누른적이 없고, 호출 시점에는 눌려있음.
	// 4. 0x8001: 이전 프레임에 누른적이 있고, 호출 시점에는 눌려있음.
	
	if (GetAsyncKeyState(key) & 0x8000)
	{
		// 현재 눌려있다.
		if (false == keyDown[key])
		{
			keyDown[key] = true;
			return true;
		}
	}
	else 
	{
		keyDown[key] = false;
	}

	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		keyUp[key] = false;
		return false;
	}
	
	keyUp[key] = true;
	return true;
	
	
	

	// if (false == IsOnceKeyDown(key))
	// {
	// 	keyUp[key] = true;
	// 	return true;
	// }
	// 	
	// keyUp[key] = false;
	// return false;
}

bool KeyManager::IsStayKeyDown(int key)
{

	if (GetAsyncKeyState(key) & 0x8000)
	{
		return true;
	}
	
	return false;
}

InputKey KeyManager::GetInputKey(int up, int down, int left, int right)
{
	if (keyBuffer.empty()) return InputKey::NONE;
	if (keyBuffer.back().first + expireTime >= elapsedTime) return InputKey::NONE;

	InputKey keys[4]{};
	int index[4]{ -1,-1,-1,-1 };
	int cnt{};

	for (int i = 0; i < keyBuffer.size(); ++i)
	{
		if (keyBuffer[i].second == up)
		{
			keys[cnt] = InputKey::UP;
			index[cnt] = i;
			cnt++;
		}
		else if (keyBuffer[i].second == down)
		{
			keys[cnt] = InputKey::DOWN;
			index[cnt] = i;
			cnt++;
		}
		else if (keyBuffer[i].second == left)
		{
			keys[cnt] = InputKey::LEFT;
			index[cnt] = i;
			cnt++;
		}
		else if (keyBuffer[i].second == right)
		{
			keys[cnt] = InputKey::RIGHT;
			index[cnt] = i;
			cnt++;
		}
		if (cnt == 4) break;
	}

	for (int i = 3; i >= 0; --i)
	{
		if (index[i] != -1)
		{
			keyBuffer.erase(keyBuffer.begin() + index[i]);
		}
	}
	
	return keys[0] | keys[1];
}
