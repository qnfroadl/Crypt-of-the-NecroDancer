#pragma once
#include "config.h"
#include "TileActor.h"

class Tile;
class Item;
//이게 전체맵 타일 제너레이터가 방 조합해서 여기 넣어줘야 함
class Tilemap : public TileActor
{
private:
	vector<vector<Tile*>> tiles;
	int mapRows;
	int mapColumns;
public:
	HRESULT Init(int _mapRows, int _mapColumns);
	void Release();
	void Update();
	void Render(HDC hdc);

	Tile* GetTile(int row, int column);
	FPOINT GetTilePos(POINT index);

	void SetTile(int row, int col, Tile* tile);

	bool Destory(Item* item);
	bool Destory(int strong);
	bool CanMove(POINT index);
	void Move(TileActor* actor, POINT index);
	POINT GetSpawnIndex();
	void Load(string filePath);

};

