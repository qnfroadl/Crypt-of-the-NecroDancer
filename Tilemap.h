#pragma once
#include "config.h"
#include "GameActor.h"

class TileActor;
class Tile;
class Item;
//이게 전체맵 타일 제너레이터가 방 조합해서 여기 넣어줘야 함
class Tilemap : public GameActor
{
private:
	vector<vector<shared_ptr<Tile>>> tiles;
	int mapRows;
	int mapColumns;
	bool isCombo;
	POINT startIndex;
	POINT endIndex;

public:
	HRESULT Init(int _mapRows, int _mapColumns);
	void Release();
	void Update();
	void Render(HDC hdc);

	shared_ptr<Tile> GetTile(POINT index);

	FPOINT GetTilePos(POINT index);

	void SetTile(int row, int col, const shared_ptr<Tile>& tile);

	bool Destory(Item* item);
	bool Destory(int strong);
	bool CanMove(POINT index);
	void Move(TileActor* actor, POINT index);
	POINT GetSpawnIndex();
	void Load(string filePath);
	void OnBeat(bool isCombo);
	POINT GetPlayerStartIndex() { return startIndex; }
	POINT GetNextStageIndex() { return endIndex; }
	void SetPlayerStartIndex(POINT p) { startIndex = p; }
	void SetNextStageIndex(POINT p) { endIndex = p;}
	int GetWidth() { return mapColumns; }
	int GetHeight() { return mapRows; }

	bool InteractTile(POINT index, GameActor* actor);

	vector<shared_ptr<TileActor>> GetRendableTileActors();
	void UpdateActiveTiles(POINT playerIndex);
};

