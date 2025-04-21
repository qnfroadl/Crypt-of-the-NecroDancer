// config.h

#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <unordered_map>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "ActorDef.h"

using namespace std;

/*
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
*/

#define SCENE_WIDTH 1920
#define SCENE_HEIGHT 1080

#define TILEMAPTOOL_X 1200
#define TILEMAPTOOL_Y 800
#define WINSIZE_X	TILEMAPTOOL_X
#define WINSIZE_Y	TILEMAPTOOL_Y

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

#define POINT_UP	POINT{0, -1}
#define POINT_DOWN	POINT{0, 1}
#define POINT_LEFT	POINT{-1, 0}
#define POINT_RIGHT POINT{1, 0}

#define ANIM_FRAME_MS 0.1f

#define SAMPLE_TILE_X 9
#define SAMPLE_TILE_Y 6

#define SAMPLE_WALL_X 9
#define SAMPLE_WALL_Y 7

#define TILE_SIZE 26
#define WALL_TILE_WIDTH 24
#define WALL_TILE_HEIGHT 48

#define TILE_SCALE 3

enum class PlayerIndex
{
	PLAYER1 = 0,
	PLAYER2 = 1,
};

enum class ItemType
{
	DIAMOND, GOLD, BOMB, WEAPON, ARMOR, POTION, HEART, KEY
};

enum class InputKey
{
	NONE = 0,

	UP = 1 , 
	DOWN = 2,  //1 << 1, 
	LEFT = 4,  //1 << 2, 
	RIGHT = 8, //1 << 3,
	
	UPDOWN = UP | DOWN,			//3
	DOWNLEFT = DOWN | LEFT,		//6
	DOWNRIGHT = DOWN | RIGHT,	//10
	UPLEFT = UP | LEFT,			//5	
	UPRIGHT = UP | RIGHT,		//9
	LEFTRIGHT = LEFT | RIGHT,	//12
};
inline InputKey operator|(InputKey a, InputKey b)
{
	return static_cast<InputKey>(static_cast<int>(a) | static_cast<int>(b));
}
inline InputKey operator&(InputKey a, InputKey b)
{
	return static_cast<InputKey>(static_cast<int>(a) & static_cast<int>(b));
}

enum class GameTag
{
	
};

typedef struct tagCIRCLE
{
	float x;
	float y;
	int radius;

}CIRCLE;
typedef struct tagFPOINT
{
	float x;
	float y;

	struct tagFPOINT operator+ (struct tagFPOINT newPos)
	{
		float x = this->x + newPos.x;
		float y = this->y + newPos.y;
		

		return tagFPOINT{x, y};
	}
} FPOINT;

/*
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (std::pair<T1, T2> const& v) const {
		auto h1 = std::hash<T1>{}(v.first);
		auto h2 = std::hash<T2>{}(v.second);
		return h1 ^ h2;
	}
};
extern unordered_map<pair<PlayerIndex, InputKey>, int, pair_hash> g_mapKey; // 플레이어의 인풋키 매핑

//
// 테스트용 콘솔창 띄우기
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif