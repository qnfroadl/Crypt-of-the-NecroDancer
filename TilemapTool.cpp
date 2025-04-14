#include "TilemapTool.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"
#include "KeyManager.h"
#include <fstream>
#include <sstream>

TilemapTool::TilemapTool()
	:sampleTile(nullptr)
{
}

TilemapTool::~TilemapTool()
{
}

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);
	
	sampleTile = ImageManager::GetInstance()->AddImage("배틀시티", TEXT("Image/maptiles.bmp"), 640, 288, SAMPLE_TILE_X, SAMPLE_TILE_Y, false);
	rcSampleTile.left = TILEMAPTOOL_X - sampleTile->GetWidth();
	rcSampleTile.top = 0;
	rcSampleTile.right = TILEMAPTOOL_X;
	rcSampleTile.bottom = sampleTile->GetHeight();

	rcMapTile.left = 0;
	rcMapTile.right = TILE_X * TILE_SIZE;
	rcMapTile.top = 0;
	rcMapTile.bottom = TILE_Y * TILE_SIZE;

	for (int i = 0; i < TILE_X; i++)
	{
		for (int j = 0; j < TILE_Y; j++)
		{
			tilesInfo[i + j * TILE_X].frameX = 3;
			tilesInfo[i + j * TILE_X].frameY = 0;

			tilesInfo[i + j * TILE_X].rc.left = i * TILE_SIZE;
			tilesInfo[i + j * TILE_X].rc.top = j * TILE_SIZE;
			tilesInfo[i + j * TILE_X].rc.right = i * TILE_SIZE + TILE_SIZE;
			tilesInfo[i + j * TILE_X].rc.bottom = j * TILE_SIZE + TILE_SIZE;

		}
	}

	return S_OK;
}

void TilemapTool::Release()
{
}

void TilemapTool::Update()
{
	if (PtInRect(&rcSampleTile, g_ptMouse)) //PointInRect(POINT{x,y}, this->rcSampleTile))
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
		{
			int x = g_ptMouse.x - rcSampleTile.left;
			int y = g_ptMouse.y - rcSampleTile.top;
			// 범위 안에 있으니, 어떤 타일을 선택 했는지 확인.
			selectedTileX = (x / TILE_SIZE);
			selectedTileY = (y / TILE_SIZE);
		}
	}
	else if (PtInRect(&rcMapTile, g_ptMouse))
	{
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON))
		{
			int infoIndex = (g_ptMouse.x / TILE_SIZE) + (g_ptMouse.y / TILE_SIZE) * TILE_X;

			tilesInfo[infoIndex].frameX = selectedTileX;
			tilesInfo[infoIndex].frameY = selectedTileY;
		}
	}
	
}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, BLACKNESS);

	for (int i = 0; i < TILE_X * TILE_Y; i++)
	{
		// 맵 그리기
		sampleTile->FrameRender(hdc, tilesInfo[i].rc.left, tilesInfo[i].rc.top, tilesInfo[i].frameX, tilesInfo[i].frameY, false, false);
	}

	// 우측 상단에 샘플타일들 나열하기.
	sampleTile->Render(hdc, TILEMAPTOOL_X - sampleTile->GetWidth(), 0);

	// 선택된 타일
	sampleTile->FrameRender(hdc, TILEMAPTOOL_X - sampleTile->GetWidth(), sampleTile->GetHeight() + 100, selectedTileX, selectedTileY);
}

void TilemapTool::Save()
{
	ofstream file;
	int x;
	int y;
	file.open("savemap.dat");
	if (file.is_open())
	{
		
		for (int j = 0; j < TILE_Y; j++)
		{
			for (int i = 0; i < TILE_X; i++)
			{
				x = tilesInfo[i + j * TILE_X].frameX;
				y = tilesInfo[i + j * TILE_X].frameY;

				file << y * SAMPLE_TILE_X + x << " ";
			}
			file << "\n";
		}

		file.close();
	}
}

void TilemapTool::Load()
{
	ifstream file("savemap.dat");
	int index = 0;
	int lineNum = 0;

	string line;
	if (!file.is_open())
	{
		return;
	}

	while(getline(file, line))
	{
		stringstream fileInfo(line);

		for (int i = 0; i < TILE_X; i++)
		{
			fileInfo >> index;

			tilesInfo[i + lineNum * TILE_X].frameX = index % SAMPLE_TILE_X;
			tilesInfo[i + lineNum * TILE_X].frameY = index / SAMPLE_TILE_X;

		}
		
		lineNum++;
	}

	file.close();
}
