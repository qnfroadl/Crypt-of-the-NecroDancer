#include "ShadowCasting.h"
#include "Tile.h"
#include "EventManager.h"
#include "Player.h"
#include "Camera.h"

vector<POINT> Row::GetDepthColume()
{
	vector<POINT> rowData;
	int min_col = round(depth * start + 0.5f);
	int max_col = round(depth * end - 0.5f);

	for (int i = min_col; i <= max_col; ++i)
	{
		rowData.push_back({ depth, i });
	}

	return rowData;
}

Row Row::GetNextRow()
{
	return Row(depth + 1, start, end);
}

ShadowCasting::ShadowCasting()
{
	maxRow = 0;
	maxCol = 0;
	tiles = vector<vector<shared_ptr<Tile>>>();
	sightMap.clear();
	players.clear();
}

ShadowCasting::~ShadowCasting()
{
}

void ShadowCasting::Init(vector<vector<shared_ptr<Tile>>> _tiles)
{
	tiles = _tiles;

	maxRow = tiles.size();
	maxCol = tiles.at(0).size();
	sightMap.resize(maxRow, vector<bool>(maxCol, false));

	EventManager::GetInstance()->BindEvent(EventType::PLAYERMOVED, std::bind(&ShadowCasting::Update, this));
	EventManager::GetInstance()->BindEvent(EventType::BLOCKDESTROYED, std::bind(&ShadowCasting::Update, this));
}

void ShadowCasting::Release()
{
}

void ShadowCasting::Update()
{
	InitShadowMap();

	for (auto& player : players)
	{
		if (auto p = player.lock())
		{
			// 플레이어가 위치한 타일 기준 시야 계산
			POINT pos = p->GetTileIndex();
			ComputeShadowMap(pos);
		}
	}
}

void ShadowCasting::Render(HDC hdc)
{
	for (int i = 0; i < maxRow; ++i)
	{
		for (int j = 0; j < maxCol; ++j)
		{
			if (sightMap[i][j])
			{
				FPOINT pos = tiles[i][j]->GetPos();
				float renderX = pos.x - Camera::GetInstance()->GetPos().x;
				float renderY = pos.y - Camera::GetInstance()->GetPos().y;
				// 시야에 보이는 타일
				Ellipse(hdc, renderX - 5, renderY - 5, renderX + 5, renderY + 5);
				//tiles[i][j]->Render(hdc);
			}
		}
	}
}

void ShadowCasting::AddPlayer(weak_ptr<Player> player)
{
	players.push_back(player);
}

void ShadowCasting::InitShadowMap()
{
	int row = sightMap.size();
	int col = sightMap[0].size();
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			sightMap[i][j] = false;
		}
	}
}

void ShadowCasting::ComputeShadowMap(POINT playerPos)
{
	// 플레이어 주변 시야는 항상 보임(절대시야) - 기본 3x3
	// 플레이어 주변 광원 값은 여기서 고려하지 않음

	for (int i = playerPos.y - 1; i <= playerPos.y + 1; ++i)
	{
		for (int j = playerPos.x - 1; j <= playerPos.x + 1; ++j)
		{
			if (i >= 0 && i < sightMap.size() && j >= 0 && j < sightMap[0].size())
			{
				sightMap[i][j] = true;
			}
		}
	}

	int dx[] = { 1, 0, -1, 0 };
	int dy[] = { 0, 1, 0, -1 };

	for (int i = 0; i < 4; ++i)
	{
		Scan(playerPos, Row(1, -1.f, 1.f), dx[i], dy[i]);
	}
}

void ShadowCasting::Scan(POINT origin, Row row, int dx, int dy)
{
	shared_ptr<Tile> prevTile = nullptr;
	for (const POINT& rowData : row.GetDepthColume())
	{
		POINT tilePos = origin;
		if (dx == 0 && dy !=0)
		{
			tilePos.y += rowData.x * dy;
			tilePos.x += rowData.y;
		}
		else if (dx != 0 && dy == 0)
		{
			tilePos.x += rowData.x * dx;
			tilePos.y += rowData.y;
		}

		if (tilePos.x < 0 || tilePos.x >= maxCol || tilePos.y < 0 || tilePos.y >= maxRow) continue;
		shared_ptr<Tile> tile = tiles[tilePos.y][tilePos.x];
		if (tile == nullptr) continue;

		if (IsWall(tile) || IsSymmetric(row, rowData))
		{
			Reveal(tile);
		}
		if (IsWall(prevTile) && IsFloor(tile))
		{
			row.start = ((float)rowData.y * 2.f - 1) / ((float)rowData.x * 2.f);
		}
		if (IsFloor(prevTile) && IsWall(tile))
		{
			Row nexRow = row.GetNextRow();
			nexRow.end = ((float)rowData.y * 2.f - 1) / ((float)rowData.x * 2.f);
			Scan(origin, nexRow, dx, dy);
		}

		prevTile = tile;
	}
	if (IsFloor(prevTile))
	{
		Scan(origin, row.GetNextRow(), dx, dy);
	}
	return;
}

void ShadowCasting::Reveal(shared_ptr<Tile> tile)
{
	if (!tile) return;
	int row = tile->GetTileIndex().y;
	int col = tile->GetTileIndex().x;

	if (col < 0 || col >= maxCol || row < 0 || row >= maxRow) return;

	sightMap[row][col] = true;
}

bool ShadowCasting::IsWall(shared_ptr<Tile> tile)
{
	if (!tile) return false;

	return tile->GetBlock() != nullptr;
}

bool ShadowCasting::IsFloor(shared_ptr<Tile> tile)
{
	if (!tile) return false;

	return tile->GetBlock() == nullptr;
}

bool ShadowCasting::IsSymmetric(Row row, POINT rowData)
{
	return (rowData.y >= (float)row.depth * row.start) && (rowData.y <= (float)row.depth * row.end);
}
