#pragma once

#include <bitset>
#include "config.h"
#include "Singleton.h"

#include <deque>

#define MAX_KEY_COUNT 256
using namespace std;

class KeyManager : public Singleton<KeyManager>
{
	
private:
	bitset<MAX_KEY_COUNT> keyUp;
	bitset<MAX_KEY_COUNT> keyDown;

	deque<pair<float, int>> keyBuffer;
	float elapsedTime;
	float expireTime;

public:
	HRESULT Init();
	void Release();
	void Update();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);

	InputKey GetInputKey(int up, int down, int left, int right);

	void SetKeyDown(int key, bool state) { this->keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) {this->keyUp.set(key, state); }

};