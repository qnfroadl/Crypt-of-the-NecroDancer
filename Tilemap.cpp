#include "Tilemap.h"
#include "Tile.h"
#include "Block.h"
#include <fstream>
#include "EventManager.h"
#include "Camera.h"
#include "Torch.h"

HRESULT Tilemap::Init(int _mapRows, int _mapColumns)
{
	mapRows = _mapRows;
	mapColumns = _mapColumns;
	tiles = vector<vector<shared_ptr<Tile>>>(_mapRows, vector<shared_ptr<Tile>>(_mapColumns, nullptr));
	spawnPoints = {};

	leftTop = { 0, 0 };
	rightBottom = { mapColumns - 1, mapRows - 1 };
	isCombo = false;
	EventManager::GetInstance()->BindEvent(this, EventType::BEAT, std::bind(&Tilemap::OnBeat, this, placeholders::_1));


	return S_OK;
}

void Tilemap::Release()
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			if (tile)
			{
				tile->Release(); // 해제만 호출, delete 안 함
			}
		}
		row.clear();
	}
	tiles.clear();
	// 이벤트 언바인드 추가
	EventManager::GetInstance()->UnbindEvent(this, EventType::BEAT);
}

void Tilemap::Update()
{
	// 나중에 이벤트로 바꿔야 함
	UpdateVisuable();

	for (int y = leftTop.y; y <= rightBottom.y; ++y)
	{
		if (y < 0 || y >= mapRows) continue;

		for (int x = leftTop.x; x <= rightBottom.x; ++x)
		{
			if (x < 0 || x >= mapColumns) continue;
			tiles[y][x]->Update();
		}
	}
}

void Tilemap::Render(HDC hdc)
{
	for (int y = leftTop.y; y <= rightBottom.y; ++y)
	{
		if (y < 0 || y >= mapRows) continue;

		for (int x = leftTop.x; x <= rightBottom.x; ++x)
		{
			if (x < 0 || x >= mapColumns) continue;
			tiles[y][x]->Render(hdc, true);
		}
	}

}

shared_ptr<Tile> Tilemap::GetTile(POINT index)
{
	if (index.y >= 0 && index.y < tiles.size() &&
		index.x >= 0 && index.x < tiles[index.y].size())
	{
		return tiles[index.y][index.x];
	}
	return nullptr;
}

FPOINT Tilemap::GetTilePos(POINT index)
{
	return tiles[index.y][index.x]->GetPos();
}

bool Tilemap::Destory(Item* item)
{
	return false;
}

bool Tilemap::Destory(int strong)
{
	return false;
}

bool Tilemap::CanMove(POINT index)
{
	if (tiles.empty()) return false;
	shared_ptr<Tile> tile = GetTile(index);
	return (tile && tile->GetBlock() == nullptr);
}

void Tilemap::Move(TileActor* actor, POINT index)
{
	if (CanMove(index))
	{
		// 이동 처리
	}
}

POINT Tilemap::GetSpawnIndex()
{
	if (spawnPoints.empty()) return { -1, -1 };
	srand(time(nullptr));
	int randomIndex = rand() % spawnPoints.size();
	POINT spawnPoint = spawnPoints[randomIndex];
	spawnPoints.erase(spawnPoints.begin() + randomIndex);
	return spawnPoint;
}

void Tilemap::Load(string filePath)
{
	ifstream in(filePath);
	if (!in.is_open()) 
	{
		MessageBoxA(nullptr, ("맵 파일 열기 실패: " + filePath).c_str(), "에러", MB_OK | MB_ICONERROR);
		return;
	}

	string header;
	in >> header;
	if (header != "TILEMAP") 
	{
		MessageBoxA(nullptr, "맵 파일 헤더가 'TILEMAP'이 아님", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	string sizeLabel;
	int fileCols, fileRows;
	in >> sizeLabel >> fileCols >> fileRows;
	if (fileCols != mapColumns || fileRows != mapRows) 
	{
		MessageBoxA(nullptr, "맵 크기가 현재 설정과 다릅니다", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	// Tile을 새로 생성
	for (int x = 0; x < mapColumns; ++x)
	{
		for (int y = 0; y < mapRows; ++y)
		{
			if (tiles[y][x])
			{
				tiles[y][x]->Release();
			}

			auto tile = make_shared<Tile>();
			tile->Init(x, y);
			tiles[y][x] = tile;
		}
	}

	// PLAYER_START
	string section;
	int playerStartX, playerStartY;
	in >> section;
	if (section != "PLAYER_START") {
		MessageBoxA(nullptr, "PLAYER_START 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}
	in >> playerStartX >> playerStartY;
	startIndex = { playerStartX, playerStartY };

	// NEXT_STAGE
	int nextStageX, nextStageY;
	in >> section;
	if (section != "NEXT_STAGE") {
		MessageBoxA(nullptr, "NEXT_STAGE 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}
	in >> nextStageX >> nextStageY;
	endIndex = { nextStageX, nextStageY };

	// FLOOR
	in >> section;
	if (section != "FLOOR") {
		MessageBoxA(nullptr, "FLOOR 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	for (int x = 0; x < mapColumns; ++x) {
		for (int y = 0; y < mapRows; ++y) {
			int tileNum;
			in >> tileNum;

			auto tile = tiles[y][x];
			tile->SetTileNum(tileNum);

			if (tileNum == 1 && (x + y) % 2 == 1)
				tile->SetTileNum(0);
		}
	}

	// WALL
	in >> section;
	if (section != "WALL") {
		MessageBoxA(nullptr, "WALL 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	for (int x = 0; x < mapColumns; ++x) {
		for (int y = 0; y < mapRows; ++y) {
			int wallNum;
			in >> wallNum;

			auto tile = tiles[y][x];

			if (tile->GetBlock()) 
			{
				tile->SetBlock(nullptr);
			}

			if (wallNum >= 0) 
			{
				auto block = make_shared<Block>();
				block->Init(tile->GetPos(), tile->GetTileIndex());
				block->SetBlockNum(wallNum);
				tile->SetBlock(block);
			}
		}
	}

	in.close();
}

void Tilemap::OnBeat(bool isCombo)
{
	for (int y = leftTop.y; y <= rightBottom.y; ++y)
	{
		if (y < 0 || y >= mapRows) continue;

		for (int x = leftTop.x; x <= rightBottom.x; ++x)
		{
			if (x < 0 || x >= mapColumns) continue;
			tiles[y][x]->OnBeat(isCombo);
		}
	}
}

bool Tilemap::InteractTile(POINT index, GameActor* actor)
{
	shared_ptr<Tile> tile = GetTile(index);
	if (tile)
	{
		tile->Interact(actor);
		actor->Interact(tile.get());
		return true;
	}
	return false;
}

void Tilemap::SetTile(int row, int col, const shared_ptr<Tile>& tile)
{
	if (row < 0 || row >= mapRows || col < 0 || col >= mapColumns) return;

	if (tiles[row][col])
	{
		tiles[row][col]->Release();
	}

	tiles[row][col] = tile;
}

vector<shared_ptr<TileActor>> Tilemap::GetRendableTileActors()
{
	vector<shared_ptr<TileActor>> result;

	for (int y = 0; y < mapRows; ++y)
	{
		for (int x = 0; x < mapColumns; ++x)
		{
			auto tile = tiles[y][x];
			if (!tile) continue;

			auto tileActors = tile->GetRendableTileActors();
			result.insert(result.end(), tileActors.begin(), tileActors.end());

			auto block = tile->GetBlock();
			if (block)
			{
				auto blockActors = block->GetRendableTileActors();
				result.insert(result.end(), blockActors.begin(), blockActors.end());
			}
		}
	}

	return result;
}


void Tilemap::UpdateActiveTiles(POINT playerIndex)
{
	FPOINT cameraPos = Camera::GetInstance()->GetPos();
	int camLeft = static_cast<int>(cameraPos.x);
	int camTop = static_cast<int>(cameraPos.y);

	int visibleCols = WINSIZE_X / TILE_SIZE;
	int visibleRows = WINSIZE_Y / TILE_SIZE;

	// +-2범위까지
	int left = camLeft / TILE_SIZE - 2;
	int top = camTop / TILE_SIZE - 2;
	int right = camLeft / TILE_SIZE + visibleCols + 2;
	int bottom = camTop / TILE_SIZE + visibleRows + 2;

	for (int y = 0; y < mapRows; ++y)
	{
		for (int x = 0; x < mapColumns; ++x)
		{
			bool inView = (x >= left && x <= right && y >= top && y <= bottom);

			shared_ptr<Tile> tile = tiles[y][x];
			if (!tile) continue;

			tile->SetActive(inView);

			if (tile->GetBlock())
				tile->GetBlock()->SetActive(inView);
		}
	}
}

void Tilemap::UpdateVisuable()
{
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
	leftTop.x = max(0, leftTile - marginX);
	leftTop.y = max(0, topTile - marginY);
	rightBottom.x = min(mapColumns - 1, leftTile + tilesOnScreenX + marginX);
	rightBottom.y = min(mapRows - 1, topTile + tilesOnScreenY + marginY);
	//cout << leftTop.x << ", " << leftTop.y << " ~ " << rightBottom.x << ", " << rightBottom.y << endl;
}

void Tilemap::ApplyTorchLighting()
{
	for (const auto& torch : torchSpots)
	{
		for (int dy = -3; dy <= 3; ++dy)
		{
			for (int dx = -3; dx <= 3; ++dx)
			{
				int nx = torch.x + dx;
				int ny = torch.y + dy;

				if (nx < 0 || ny < 0 || nx >= mapColumns || ny >= mapRows)
					continue;

				int dist = abs(dx) + abs(dy);
				if (dist > 3) continue;

				float lightAdd = 0.0f;
				switch (dist)
				{
				case 0: lightAdd = 1.0f; break;
				case 1: lightAdd = 0.6f; break;
				case 2: lightAdd = 0.3f; break;
				case 3: lightAdd = 0.1f; break;
				}

				shared_ptr<Tile> tile = tiles[ny][nx];
				if (!tile) continue;

				tile->SetStaticBrightness(tile->GetStaticBrightness() + lightAdd);

				if (shared_ptr<Block> block = tile->GetBlock())
				{
					block->SetStaticBrightness(block->GetStaticBrightness() + lightAdd);

					if (shared_ptr<Torch> torch = block->GetTorch())
					{
						torch->SetStaticBrightness(torch->GetStaticBrightness() + lightAdd);
					}
				}
			}
		}
	}
}

void Tilemap::RemoveTorchLightingAt(POINT torchIndex)
{
	for (int dy = -3; dy <= 3; ++dy)
	{
		for (int dx = -3; dx <= 3; ++dx)
		{
			int nx = torchIndex.x + dx;
			int ny = torchIndex.y + dy;

			if (nx < 0 || ny < 0 || nx >= mapColumns || ny >= mapRows)
				continue;

			int dist = abs(dx) + abs(dy);
			if (dist > 3) continue;

			float lightSub = 0.0f;
			switch (dist)
			{
			case 0: lightSub = 1.0f; break;
			case 1: lightSub = 0.6f; break;
			case 2: lightSub = 0.3f; break;
			case 3: lightSub = 0.1f; break;
			}

			shared_ptr<Tile> tile = tiles[ny][nx];
			if (!tile) continue;

			tile->SetStaticBrightness(tile->GetStaticBrightness() - lightSub);

			if (shared_ptr<Block> block = tile->GetBlock())
			{
				block->SetStaticBrightness(block->GetStaticBrightness() - lightSub);

				if (shared_ptr<Torch> torch = block->GetTorch())
				{
					torch->SetStaticBrightness(torch->GetStaticBrightness() - lightSub);
				}
			}
		}
	}
	torchSpots.erase(
		std::remove_if(torchSpots.begin(), torchSpots.end(),
			[&](const POINT& pt) {
				return pt.x == torchIndex.x && pt.y == torchIndex.y;
			}),
		torchSpots.end()
	);
}