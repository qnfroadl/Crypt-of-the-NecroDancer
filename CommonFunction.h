﻿// CommonFunction.h
/*
	inline: 컴파일러에게 함수 호출을 최적화 하도록 요청하는 키워드
	일반적인 함수들은 함수 호출 시에 매개변수를 스택메모리 저장, 
	함수 시작주소를 호출, 반환주소로 이동
	// main.cpp
	int result = 3 + 6;

	inline int Add(int a, int b)
	{
		return a + b;
	}
*/
#pragma once

#define NOMINMAX

#include "config.h"
#include <math.h>
#include <minwindef.h>
#include <algorithm>


inline float GetDistance(FPOINT p1, FPOINT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;

	return sqrtf(x * x + y * y);
}
inline float GetDistance(POINT p1, POINT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;

	return sqrtf(x * x + y * y);
}
inline float GetDistance(float x1, float y1, float x2, float y2)
{
	float x = x1 - x2;
	float y = y1 - y2;

	return sqrtf(x * x + y * y);
}


inline RECT GetRect(int left, int top, int width, int height)
{
	RECT rc{ left, top, left + width, top + height };
	return rc;
}

inline RECT GetRectAtCenter(int x, int y, int width, int height)
{
	RECT rc{ x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2) };
	return rc;
}

inline void RenderStar(HDC hdc, int posX, int posY)
{
	MoveToEx(hdc, posX + 60, posY + 20, NULL);
	LineTo(hdc, posX - 60, posY + 20);
	LineTo(hdc, posX + 40, posY - 60);
	LineTo(hdc, posX, posY + 60);
	LineTo(hdc, posX - 40, posY - 60);
	LineTo(hdc, posX + 60, posY + 20);
}

// 좌상단 기준
inline void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderRectAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Rectangle(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

// 가운데 기준
inline void RenderFillRectAtCenter(HDC hdc, HBRUSH brush, int centerX, int centerY, int width, int height)
{
	RECT rt = GetRectAtCenter(centerX, centerY, width, height);
	FillRect(hdc, &rt, brush);
}

// 좌상단 기준
inline void RenderEllipse(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderEllipseAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Ellipse(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

inline bool PointInRect(POINT ptMouse, RECT rc)
{
	if (ptMouse.x < rc.left || ptMouse.x > rc.right
		|| ptMouse.y < rc.top || ptMouse.y > rc.bottom)
	{
		return false;
	}
	return true;
}

inline bool RectInRect(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left || rc1.left > rc2.right
		|| rc1.top > rc2.bottom || rc1.bottom < rc2.top)
	{
		return false;
	}
	return true;
}

inline bool RectInCircle(RECT rt, CIRCLE cc)
{
	
	// 원의 중심이 사각형 내부에 있는 경우
	if (cc.x >= rt.left && cc.x <= rt.right &&
		cc.y >= rt.top && cc.y <= rt.bottom)
	{
		return true;
	}

	// 2. 사각형 꼭짓점 중 하나라도 원 내부에 있으면 → 충돌
	POINT corners[4] = {
		{ rt.left,  rt.top },
		{ rt.right, rt.top },
		{ rt.left,  rt.bottom },
		{ rt.right, rt.bottom }
	};

	for (int i = 0; i < 4; ++i)
	{
		float dx = (float)corners[i].x - cc.x;
		float dy = (float)corners[i].y - cc.y;
		if ((dx * dx + dy * dy) <= (cc.radius * cc.radius))
			return true;
	}
	
	// 3. 원이 사각형에 걸쳐 있는 경우 (원과 사각형 경계가 겹치는 경우)
	// → 중심에서 사각형 가장 가까운 점 찾기
	//std::max(10,2);

	float closestX = 10;// std::max((float)rt.left, std::min(cc.x, (float)rt.right));
	float closestY = 10; //std::max((float)rt.top, std::min(cc.y, (float)rt.bottom));
	float dx = closestX - cc.x;
	float dy = closestY - cc.y;

	if ((dx * dx + dy * dy) <= (cc.radius * cc.radius))
		return true;

	return false;
	
}

inline bool CircleInCircle(CIRCLE c1, CIRCLE c2)
{
	// c1.x, c1.y
	float distance = GetDistance(c1.x, c1.y, c2.x, c2.y);
	
	if (distance < c1.radius + c2.radius)
	{
		return false;
	}

	return true;
}



inline void UpdateRect(RECT& rc, FPOINT pt)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	rc.left = pt.x - (width / 2);
	rc.right = rc.left + width;
	rc.top = pt.y - (height / 2);
	rc.bottom = rc.top + height;
}

inline float GetAngle(FPOINT start, FPOINT end)
{
	float x = end.x - start.x;
	float y = end.y - start.y;

	 return -atan2f(y, x);	// 라디안
}

inline bool IsOutofScreen(const FPOINT& pos)
{
	if (pos.x < 0 || pos.x > WINSIZE_X || pos.y < 0 || pos.y > WINSIZE_Y)
	{
		return true;
	}

	return false;
}

inline bool IsOutofScreen(const RECT& rt, int padding)
{
	if (rt.left < 0 + padding || WINSIZE_X  < rt.right + padding
	|| rt.top < 0 + padding || WINSIZE_Y < rt.bottom + padding)
	{
		return true;
	}

	return false;
}

inline bool IsOutofScreen(const RECT& rt, int paddingX, int paddingUpY, int paddingDownY)
{
	if (rt.left < 0 + paddingX || WINSIZE_X < rt.right + paddingX
		|| rt.top < 0 + paddingUpY || WINSIZE_Y < rt.bottom + paddingDownY)
	{
		return true;
	}

	return false;
}

inline void SetClientRect(HWND hWnd, int width, int height)
{
	RECT rc;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	SetRect(&rc, 0,0, width, height);

	AdjustWindowRectEx(&rc, style, GetMenu(hWnd) != NULL, exStyle);

	if (style & WS_VSCROLL)
	{
		rc.right += GetSystemMetrics(SM_CXVSCROLL);
	}

	if (style & WS_HSCROLL)
	{
		rc.bottom += GetSystemMetrics(SM_CYHSCROLL);
	}

	SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}