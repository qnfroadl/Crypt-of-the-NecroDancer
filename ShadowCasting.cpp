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

float ShadowCasting::CalcBrightness(POINT origin, int col, int row)
{
	/*
		거리 구해서, 적당한 밝기값 반환하도록,
		이후에는 렌더러에서 읽어서 밝기값만큼 렌더 하면됨(Render 할 때 투명값을 줄 수도 있겠지만? 밝기값 설정 후 렌더)
	*/

	// 거리 계산
	int dx = col - origin.x;
	int dy = row - origin.y;
	float distance = sqrtf(dx * dx + dy * dy);

	// 밝기 계산 (0.0f ~ 1.0f)
	const float maxDistance = 4.0f; // 최대 밝기가 닿는 거리
	const float minBrightness = 0.1f; // 최소 밝기
	const float maxBrightness = 1.0f;

	if (distance >= maxDistance)
		return minBrightness;

	// 밝기 감소는 선형 또는 제곱 방식 등 선택 가능
	float brightness = maxBrightness - (distance / maxDistance); // 선형 감소
	// float brightness = maxBrightness - (distance * distance) / (maxDistance * maxDistance); // 제곱 감소

	// 최소 밝기 보장
	return max(minBrightness, brightness);
}

ShadowCasting::ShadowCasting()
{
	maxRow = 0;
	maxCol = 0;
	tiles = vector<vector<shared_ptr<Tile>>>();
	sightMap.clear();
	players.clear();
	cameraRect = { 0, 0, 0, 0 };
}

ShadowCasting::~ShadowCasting()
{
}

void ShadowCasting::Init(vector<vector<shared_ptr<Tile>>> _tiles)
{
	tiles = _tiles;

	maxRow = tiles.size();
	maxCol = tiles.at(0).size();
	sightMap.resize(maxRow, vector<pair<bool, float>>(maxCol, make_pair(false, 0.f)));

	EventManager::GetInstance()->BindEvent(this, EventType::PLAYERMOVED, std::bind(&ShadowCasting::Update, this));
	EventManager::GetInstance()->BindEvent(this, EventType::BLOCKDESTROYED, std::bind(&ShadowCasting::Update, this));

	cameraRect = { 0, 0, maxCol - 1, maxRow - 1 };
}

void ShadowCasting::Release()
{
	EventManager::GetInstance()->UnbindEvent(this, EventType::PLAYERMOVED);
	EventManager::GetInstance()->UnbindEvent(this, EventType::BLOCKDESTROYED);
}

void ShadowCasting::Update()
{
	CalculateCameraRect();

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

	EventManager::GetInstance()->AddEvent(EventType::LIGHTINGUPDATED, new ShadowCastingEventData(sightMap), true);
}

void ShadowCasting::Render(HDC hdc)
{
	for (int i = 0; i < maxRow; ++i)
	{
		for (int j = 0; j < maxCol; ++j)
		{
			if (sightMap[i][j].first)
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
			sightMap[i][j].first = false;
			sightMap[i][j].second = 0.f;
		}
	}
}

void ShadowCasting::ComputeShadowMap(POINT playerPos)
{
	int dx[] = { 1, 0, -1, 0 };
	int dy[] = { 0, 1, 0, -1 };

	for (int i = 0; i < 4; ++i)
	{
		Scan(playerPos, Row(1, -1.f, 1.f), dx[i], dy[i]);
	}

	// 플레이어 주변 시야는 항상 보임(절대시야) - 기본 3x3
	// 플레이어 주변 광원 값은 여기서 고려하지 않음

	for (int i = playerPos.y - 1; i <= playerPos.y + 1; ++i)
	{
		for (int j = playerPos.x - 1; j <= playerPos.x + 1; ++j)
		{
			if (i >= 0 && i < sightMap.size() && j >= 0 && j < sightMap[0].size())
			{
				sightMap[i][j].first = true;
				sightMap[i][j].second = 1.f;

			}
		}
	}
}

void ShadowCasting::Scan(POINT origin, Row row, int dx, int dy)
{
	shared_ptr<Tile> prevRowTile = nullptr;
	shared_ptr<Tile> prevColTile = nullptr;
	shared_ptr<Tile> prevRowPrevColTile = nullptr;
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

		POINT prevRowTilePos = tilePos;
		POINT prevRowPrevColTilePos = tilePos;
		if (dx == 0 && dy != 0)
		{
			prevRowTilePos.y -= dy;
			prevRowPrevColTilePos.y -= dy;
			prevRowPrevColTilePos.x -= 1;
		}
		else if (dx != 0 && dy == 0)
		{
			prevRowTilePos.x -= dx;
			prevRowPrevColTilePos.x -= dx;
			prevRowPrevColTilePos.y -= 1;
		}

		if (IsOutOfRange(tilePos)) continue;
		shared_ptr<Tile> tile = tiles[tilePos.y][tilePos.x];
		if (tile == nullptr) continue;

		shared_ptr<Tile> prevRowTile = nullptr;
		if (!IsOutOfRange(prevRowTilePos))
		{
			prevRowTile = tiles[prevRowTilePos.y][prevRowTilePos.x];
		}

		shared_ptr<Tile> prevRowPrevColTile = nullptr;
		if (!IsOutOfRange(prevRowPrevColTilePos))
		{
			prevRowPrevColTile = tiles[prevRowPrevColTilePos.y][prevRowPrevColTilePos.x];
		}

		if (IsWall(tile) || IsSymmetric(row, rowData))
		{
			Reveal(tile, origin);
		}
		if (IsWall(prevColTile) && IsFloor(tile))
		{
			if (!IsWall(prevRowTile))
			{
				row.start = ((float)rowData.y * 2.f - 1) / ((float)rowData.x * 2.f);
			}
			else
			{
				UnReveal(tile);
			}
		}
		if (IsFloor(prevColTile) && IsWall(tile))
		{
			if (!IsWall(prevRowPrevColTile))
			{
				Row nexRow = row.GetNextRow();
				nexRow.end = ((float)rowData.y * 2.f - 1) / ((float)rowData.x * 2.f);
				Scan(origin, nexRow, dx, dy);
			}
			else
			{
				UnReveal(prevColTile);
			}
		}

		prevColTile = tile;
	}
	
	if (IsFloor(prevColTile))
	{
		if (sightMap[prevColTile->GetTileIndex().y][prevColTile->GetTileIndex().x].first == true)
		{
			Scan(origin, row.GetNextRow(), dx, dy);
		}
	}
}

void ShadowCasting::Reveal(shared_ptr<Tile> tile, POINT origin)
{
	if (!tile) return;
	int row = tile->GetTileIndex().y;
	int col = tile->GetTileIndex().x;

	if (IsOutOfRange({ col, row })) return;

	sightMap[row][col].first = true;

	// 시야에 보이는 애들만 밝기가 적용될 예정
	sightMap[row][col].second = CalcBrightness(origin, col, row);
}

void ShadowCasting::UnReveal(shared_ptr<Tile> tile)
{
	if (!tile) return;
	int row = tile->GetTileIndex().y;
	int col = tile->GetTileIndex().x;

	if (IsOutOfRange({ col, row })) return;

	sightMap[row][col].first = false;
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

bool ShadowCasting::IsSymmetric(Row row, POINT rowData) // 지금 무조건 true 아닌가...?
{
	return (rowData.y >= (float)row.depth * row.start) && (rowData.y <= (float)row.depth * row.end);
}

bool ShadowCasting::IsOutOfRange(POINT pos)
{
	// check camera range
	if (pos.x < cameraRect.left || pos.x > cameraRect.right) return true;
	if (pos.y < cameraRect.top || pos.y > cameraRect.bottom) return true;

	// check map range
	if (pos.x < 0 || pos.x >= maxCol) return true;
	if (pos.y < 0 || pos.y >= maxRow) return true;

	return false;
}

void ShadowCasting::CalculateCameraRect()
{
	// 급하니까 냅다 가져다 씁니다~~
	FPOINT cameraPos = Camera::GetInstance()->GetPos();
	RECT viewRect = Camera::GetInstance()->GetViewRect();
	float scaledTileSize = TILE_SIZE * TILE_SCALE;

	// 카메라 좌상단 좌표가 포함된 타일 인덱스
	int leftTile = static_cast<int>(cameraPos.x / scaledTileSize);
	int topTile = static_cast<int>(cameraPos.y / scaledTileSize);

	// 화면에 보이는 타일 개수 (뷰의 폭/높이를 타일 크기로 나눔)
	int tilesOnScreenX = static_cast<int>(ceil((viewRect.right - viewRect.left) / scaledTileSize));
	int tilesOnScreenY = static_cast<int>(ceil((viewRect.bottom - viewRect.top) / scaledTileSize));

	// 마진
	int marginX = 2;
	int marginY = 2;

	// 보여줄 타일 범위 계산
	cameraRect.left = max(0, leftTile - marginX);
	cameraRect.top = max(0, topTile - marginY);
	cameraRect.right = min(maxCol - 1, leftTile + tilesOnScreenX + marginX);
	cameraRect.bottom = min(maxRow - 1, topTile + tilesOnScreenY + marginY);
}
