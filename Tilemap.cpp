#include "Tilemap.h"  
#include "Tile.h"
#include "Block.h"
#include <fstream>

HRESULT Tilemap::Init(int _mapRows, int _mapColumns)
{
	mapRows = _mapRows;
	mapColumns = _mapColumns;
	tiles = vector<vector<Tile*>>(_mapRows, vector<Tile*>(_mapColumns, nullptr));
	for (int i = 0; i < mapRows; ++i)
	{
		for (int j = 0; j < mapColumns; ++j)
		{
			tiles[i][j] = new Tile();
			tiles[i][j]->Init();
		}
	}
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

Tile* Tilemap::GetTile(int row, int column)
{
	if (row >= 0 && row < tiles.size() && column >= 0 && column < tiles[row].size()) 
	{
		return tiles[row][column];
	}
	return nullptr;
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
	Tile* tile = GetTile(index.y, index.x);
	return (tile && tile->GetBlock() == nullptr);
}

void Tilemap::Move(TileActor* actor, POINT index) 
{
	if (CanMove(index)) 
	{
		// ���� �̵�
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
		MessageBoxA(nullptr, ("�� ���� ���� ����: " + filePath).c_str(), "����", MB_OK | MB_ICONERROR);
		return;
	}

	string header;
	in >> header;
	if (header != "TILEMAP") {
		MessageBoxA(nullptr, "�� ���� ����� 'TILEMAP'�� �ƴ�", "����", MB_OK | MB_ICONERROR);
		return;
	}

	string sizeLabel;
	int fileCols, fileRows;
	in >> sizeLabel >> fileCols >> fileRows;
	if (fileCols != mapColumns || fileRows != mapRows) {
		MessageBoxA(nullptr, "�� ũ�Ⱑ ���� ������ �ٸ��ϴ�", "����", MB_OK | MB_ICONERROR);
		return;
	}

	// FLOOR ����
	string section;
	in >> section;
	if (section != "FLOOR") {
		MessageBoxA(nullptr, "FLOOR ���� ����", "����", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapColumns; j++) {
			int tileNum;
			in >> tileNum;
			if (tiles[i][j]) {
				tiles[i][j]->SetTileNum(tileNum);
				RECT rc = {
					j * TILE_SIZE,
					i * TILE_SIZE,
					j * TILE_SIZE + TILE_SIZE,
					i * TILE_SIZE + TILE_SIZE
				};
				tiles[i][j]->SetRcTile(rc);
			}
		}
	}

	// WALL ����
	in >> section;
	if (section != "WALL") {
		MessageBoxA(nullptr, "WALL ���� ����", "����", MB_OK | MB_ICONERROR);
		return;
	}

	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapColumns; j++) {
			int wallNum;
			in >> wallNum;
			if (tiles[i][j]) {
				if (wallNum >= 0) {
					Block* block = new Block();
					block->Init();
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
