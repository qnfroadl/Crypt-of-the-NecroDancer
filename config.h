// config.h

#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

/*
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
*/

#define TILEMAPTOOL_X 1200
#define TILEMAPTOOL_Y 800
#define WINSIZE_X	TILEMAPTOOL_X
#define WINSIZE_Y	TILEMAPTOOL_Y

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

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