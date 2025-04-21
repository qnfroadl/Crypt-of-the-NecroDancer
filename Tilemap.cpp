#include "Tilemap.h"  
#include "Tile.h"
#include "Block.h"
#include <fstream>
#include "EventManager.h"

HRESULT Tilemap::Init(int _mapRows, int _mapColumns)
{
	mapRows = _mapRows;
	mapColumns = _mapColumns;
	tiles = vector<vector<Tile*>>(_mapRows, vector<Tile*>(_mapColumns, nullptr));
	/*for (int i = 0; i < mapRows; ++i)
	{
		for (int j = 0; j < mapColumns; ++j)
		{
			tiles[i][j] = new Tile();
			tiles[i][j]->Init(i, j);
		}
	}*/
	EventManager::GetInstance()->BindEvent(EventType::BEAT, std::bind(&Tilemap::OnBeat, this, std::placeholders::_1));
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
				tile->Release();
				delete tile;
			}
		}
		row.clear();
	}
	tiles.clear();
}

void Tilemap::Update()
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			if (tile) 
			{
				tile->Update();
			}
		}
	}
}

void Tilemap::Render(HDC hdc)
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			if (tile)
			{
				tile->Render(hdc);
			}
		}
	}
}

Tile* Tilemap::GetTile(POINT index)
{
	if (index.y >= 0 && index.y < tiles.size() && index.x >= 0 && index.x < tiles[index.y].size())
	{
		return tiles[index.x][index.y];
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
	Tile* tile = GetTile(index);
	return (tile && tile->GetBlock() == nullptr);
}

void Tilemap::Move(TileActor* actor, POINT index) 
{
	if (CanMove(index)) 
	{
		// 액터 이동
	}
}

POINT Tilemap::GetSpawnIndex() 
{
	for (int i = 0; i < tiles.size(); ++i) 
	{
		for (int j = 0; j < tiles[i].size(); ++j)
		{
			if (tiles[i][j] && tiles[i][j]->GetBlock() == nullptr) 
			{
				return { j, i };
			}
		}
	}
	return { -1, -1 };
}

void Tilemap::Load(string filePath)
{
	ifstream in(filePath);
	if (!in.is_open()) {
		MessageBoxA(nullptr, ("맵 파일 열기 실패: " + filePath).c_str(), "에러", MB_OK | MB_ICONERROR);
		return;
	}

	string header;
	in >> header;
	if (header != "TILEMAP") {
		MessageBoxA(nullptr, "맵 파일 헤더가 'TILEMAP'이 아님", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	string sizeLabel;
	int fileCols, fileRows;
	in >> sizeLabel >> fileCols >> fileRows;
	if (fileCols != mapColumns || fileRows != mapRows) {
		MessageBoxA(nullptr, "맵 크기가 현재 설정과 다릅니다", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	string section;
	int playerStartX, playerStartY;
	int nextStageX, nextStageY;
	in >> section;
	if (section != "PLAYER_START") {
		MessageBoxA(nullptr, "PLAYER_START 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}
	in >> playerStartX >> playerStartY;
	startIndex = { playerStartX, playerStartY };
	in >> section;
	if (section != "NEXT_STAGE") {
		MessageBoxA(nullptr, "NEXT_STAGE 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}
	in >> nextStageX >> nextStageY;
	endIndex = { nextStageX, nextStageY };
	in >> section;
	if (section != "FLOOR") {
		MessageBoxA(nullptr, "FLOOR 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapColumns; j++) {
			int tileNum;
			in >> tileNum;
			if (tiles[i][j]) {
				tiles[i][j]->Init(i, j);
				tiles[i][j]->SetTileNum(tileNum);
				if (tileNum == 1 && (tiles[i][j]->GetTileIndex().x + tiles[i][j]->GetTileIndex().y) % 2 == 1)
					tiles[i][j]->SetTileNum(0);
			}
		}
	}

	in >> section;
	if (section != "WALL") {
		MessageBoxA(nullptr, "WALL 섹션 누락", "에러", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapColumns; j++) {
			int wallNum;
			in >> wallNum;
			if (tiles[i][j]) {
				if (wallNum >= 0) {
					Block* block = new Block();
					block->Init(tiles[i][j]->GetPos(), tiles[i][j]->GetTileIndex());
					block->SetBlockNum(wallNum);
					tiles[i][j]->SetBlock(block);
				}
				else {
					tiles[i][j]->SetBlock(nullptr);
				}
			}
		}
	}

	in.close();
}
void Tilemap::OnBeat(bool isCombo)
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			if (tile)
			{
				tile->OnBeat(isCombo);
			}
		}
	}
}

void Tilemap::SetTile(int row, int col, Tile* tile)
{
	if (row < 0 || row >= mapRows || col < 0 || col >= mapColumns) return;

	if (tiles[row][col])
	{
		tiles[row][col]->Release();
		delete tiles[row][col];
	}

	tiles[row][col] = tile;
}