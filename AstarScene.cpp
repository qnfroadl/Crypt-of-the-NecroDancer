#include "AstarScene.h"

#include "CommonFunction.h"
#include "TimerManager.h"
#include "KeyManager.h"

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	this->idX = idX;
	this->idY = idY;

	center.x = idX * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);
	center.y = idY * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);

	rc.left = idX * ASTAR_TILE_SIZE;
	rc.right = rc.left + ASTAR_TILE_SIZE;
	rc.top = idY * ASTAR_TILE_SIZE;
	rc.bottom = rc.top + ASTAR_TILE_SIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	// totalCost = 0.0f;

	type = AstarTileType::None;

	parentTile = nullptr;

	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);
}

void AstarTile::Update()
{
}
#include "Camera.h"

void AstarTile::Render(HDC hdc)
{
	float x = Camera::GetInstance()->GetPos().x;
	float y = Camera::GetInstance()->GetPos().y;

	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	RECT drawRc = rc;
	drawRc.left -= x;
	drawRc.right -= x;
	drawRc.top -= y;
	drawRc.bottom -= y;

	FillRect(hdc, &drawRc, hBrush);
	Rectangle(hdc, drawRc.left, drawRc.top, drawRc.right, drawRc.bottom);

	SelectObject(hdc, hOldBrush);
}

void AstarTile::SetColor(COLORREF color)
{
	this->color = color;
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(color);
}

void AstarTile::SetParent(AstarTile* parent)
{
	this->parentTile = parent;
}

AstarTile* AstarTile::GetParent()
{
	return this->parentTile;
}

POINT AstarTile::GetCenter()
{
	return this->center;
}

float AstarTile::GetFromStart()
{
	return costFromStart;	// g : 시작점부터 현재 노드까지의 비용
}

float AstarTile::GetToDest()
{
	return costToGoal;		// h : 현재 노드부터 목적지까지의 예상비용
}

float AstarTile::GetStartToDest()
{
	return costFromStart + costToGoal;;		// f : g + h
}

void AstarTile::SetFromStart(float value)
{
	this->costFromStart = value;
}

void AstarTile::SetToDest(float value)
{
	this->costToGoal = value;
}

/*
	
*/

inline bool AstarScene::IsValidIndex(int x, int y)
{
	if (x < 0 || ASTAR_TILE_COUNT <= x || y < 0 || ASTAR_TILE_COUNT <= y)
	{
		return false;
	}
	return true;	
}

bool AstarScene::IsValidTile(AstarTile* tile)
{
	if (AstarTileType::Wall == tile->GetType())
	{
		return false;
	}
	return true;
}

bool AstarScene::FindTile(vector<AstarTile*>& list, AstarTile* tile)
{
	auto it = std::find(list.begin(), list.end(), tile);
	if (it != list.end())
	{
		return true;
	}
	return false;
}

float AstarScene::StartToTile(AstarTile* tile)
{ 
	AstarTile* parentTile = tile->GetParent();
	
	if (parentTile)
	{
		float fromStart = parentTile->GetFromStart();	//fromStart = 을 안해줘서 미친 ㅋ
		
		POINT ptParent = parentTile->GetCenter();
		POINT ptTile = tile->GetCenter();

		return fromStart + GetDistance(ptParent, ptTile);
	}
	
	return 0.f;
}

// 가장 심플하게 휴리스틱?
float AstarScene::TileToDest(AstarTile* tile)
{
	POINT ptDest = this->destTile->GetCenter();
	POINT ptTile = tile->GetCenter();
	return GetDistance(ptDest, ptTile);
}

AstarTile* AstarScene::GetNextTile()
{
	AstarTile* tile = nullptr;

	auto it = min_element(openList.begin(), openList.end(), [&](AstarTile* a, AstarTile* b)
		{
			return a->GetStartToDest() < b->GetStartToDest();
		});

	if (it != openList.end())
	{
		tile = *it;
	}


	// 열린 목록에서, f값이 가장 작은 노드를 선택하여.
	// float totalCost = 9999;	// std::numeric_limits<float>::max()
	// 
	// auto it = openList.begin();
	// 
	// while (it != openList.end())
	// {
	// 	float total = (*it)->GetStartToDest();	// ToDest로 했으니 이상하게 나오지
	// 	if (total < totalCost)
	// 	{
	// 		tile = *it;
	// 		totalCost = total;
	// 	}
	// 
	// 	it++;
	// }
	// 
	return tile;
}

void AstarScene::OpenToClose(AstarTile* tile)
{
	// 오픈 리스트에서 제거하고, 
	auto it = std::find(openList.begin(), openList.end(), tile);
	if (it != openList.end())
	{
		openList.erase(it);
	}
	
	// 클로즈 리스트로 이동
	closeList.push_back(tile);

}

HRESULT AstarScene::Init()
{
	SetClientRect(g_hWnd, ASTAR_TILE_COUNT * ASTAR_TILE_SIZE, ASTAR_TILE_COUNT * ASTAR_TILE_SIZE);

	bSimple = false;
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Init(j, i);
		}
	}

	startTile = &(map[3][3]);
	startTile->SetColor(RGB(255, 0, 0));
	startTile->SetType(AstarTileType::Start);

	currTile = startTile;

	destTile = &(map[10][19]);
	destTile->SetColor(RGB(0, 0, 255));
	destTile->SetType(AstarTileType::End);

	return S_OK;
}

void AstarScene::Release()
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Release();
		}
	}

	openList.clear();
	closeList.clear();
}

void AstarScene::Update()
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// 시작이나 목적지가 아닐 때
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(100, 150, 100));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}

	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();

	if (0.1 <= elapsedTime)
	{
		elapsedTime = 0;
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_SPACE))
		{
			FindPath();
		}
	}
}

void AstarScene::Render(HDC hdc)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Render(hdc);
		}
	}
}

void AstarScene::FindPath()
{
	if (currTile)
	{
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(currTile);
		
		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		currTile = GetNextTile();

		// 도착여부 판단
		if (currTile == destTile)
		{
			// 도착.
			currTile = nullptr;
			PrintPath();
			return;
		}
		else 
		{
			// 3) 그렇지 않으면, 현재 노드를 열린 목록에서 제거하고 닫힌 목록에 추가.
			OpenToClose(currTile);
		}
		

		// 반복
		// FindPath();
	}
}

void AstarScene::PrintPath()
{
	// destTile의 parent로부터 하나하나 경로를 찾아간다.
	
	AstarTile* tile = destTile;

	destTile->SetColor(RGB(0,0,255));

	tile = tile->GetParent();
	while (nullptr != tile && tile != startTile)
	{
		tile->SetColor(RGB(0, 255, 255));
		tile = tile->GetParent();
	}

	startTile->SetColor(RGB(255,0,0));
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	// curTile => 현재노드 n
    /*
		4		1		5
		0	curTile		2
		7		3		6
	*/
	static int nearX[8] = { -1, 0, 1, 0 ,-1, 1, 1, -1};
	static int nearY[8] = { 0, -1, 0, 1 , -1, -1, 1, 1};
	static pair<POINT, POINT> wallCheck[4] = {  {POINT{0,-1}, POINT{1,0}},
												{POINT{-1, 0}, POINT{0,1}}, 
												{POINT{0,-1}, POINT{-1,0}}, 
												{POINT{1,0}, POINT{-1,0}}};

	// 주변의 타일들을 OpenList에 넣는다.
	if (currTile)
	{
		int curX = currTile->GetidX();
		int curY = currTile->GetidY();

		int mx = 0;
		int my = 0;
		// 주변 타일들을 탐색 하면서. 해당 타일이 닫힌 목록에 없고, 이동 가능한 타일이라면.
		// 열린목록에 이미 있다면, 더 작은 g(n)값을 가질수 있으면, g(m), f(m) 갱신.
		// 열린목록에 없다면, g(m), h(m), f(m) 구한다. 부모노드 n으로 .

		for (int i = 0; i < (bSimple? 4 : 8); i++)
		{
			mx = curX + nearX[i];
			my = curY + nearY[i];

			if (IsValidIndex(mx, my))
			{
				AstarTile* nearTile = &map[my][mx];
				
				if (IsValidTile(nearTile) && false == FindTile(closeList, nearTile)) // 닫힌 목록에 있으면 무시
				{
					if (4 <= i)
					{
						int cx = mx + wallCheck[i - 4].first.x;
						int cy = my + wallCheck[i - 4].first.y;
						int dx = mx + wallCheck[i - 4].second.x;
						int dy = my + wallCheck[i - 4].second.y;
						if (false == IsValidTile(&map[cy][cx]) && false == IsValidTile(&map[dy][dx]))
						{
							// 대각 이동하려하는데, 양옆이 막혀있다.
							continue;
						}
					}

					nearTile->SetColor(RGB(255, 0, 255));

					// 열린 목록에 없으면?
					if (false == FindTile(openList, nearTile))
					{
						// 열린 목록에 추가.
						openList.push_back(nearTile);
						// 부모 노드 설정.
						nearTile->SetParent(currTile);
						nearTile->SetFromStart(this->StartToTile(nearTile));
						nearTile->SetToDest(this->TileToDest(nearTile));
					}
					else
					{	//열린 목록에 있으면?
						// 현재 경로를 통해서(currTile을 통해서) 더 짧은 경로를 구했다면? 갱신.
						if (currTile->GetFromStart() + GetDistance(nearTile->GetCenter(), currTile->GetCenter()) < nearTile->GetFromStart())
						{
							nearTile->SetParent(currTile);
							nearTile->SetFromStart(this->StartToTile(nearTile));

							// nearTile->SetToDest(this->TileToDest(nearTile));	어차피 똑같
						}
					}
				}
				

			}

			
		}



	}

}
