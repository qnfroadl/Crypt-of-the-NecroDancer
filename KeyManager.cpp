#include "KeyManager.h"

HRESULT KeyManager::Init()
{
	keyUp.set();		// true로 세팅.
	keyDown.reset();	// false로 세팅.

	return S_OK;
}

void KeyManager::Release()
{
	Singleton::ReleaseInstance();
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
