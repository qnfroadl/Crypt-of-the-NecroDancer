﻿#pragma once
#include "config.h"
#include "GameActor.h"

class TileActor;
class Tile;
class Item;
class EventData;
//이게 전체맵 타일 제너레이터가 방 조합해서 여기 넣어줘야 함
class Tilemap : public GameActor
{
private:
	vector<vector<shared_ptr<Tile>>> tiles;
	vector<POINT> spawnPoints;
	vector<POINT> torchSpots;
	int mapRows;
	int mapColumns;
	bool isCombo;
	POINT startIndex;
	POINT endIndex;

	POINT leftTop;
	POINT rightBottom;

	void OnInteract(EventData* data);
public:
	HRESULT Init(int _mapRows, int _mapColumns);
	void Release();
	void Update();
	void Render(HDC hdc);

	shared_ptr<Tile> GetTile(POINT index);  
    const vector<vector<shared_ptr<Tile>>>& GetTiles() { return tiles; }

	FPOINT GetTilePos(POINT index);

	void SetTile(int row, int col, const shared_ptr<Tile>& tile);

	bool Destory(Item* item);
	bool Destory(int strong);
	bool CanMove(POINT index);
	void Move(TileActor* actor, POINT index);
	POINT GetSpawnIndex();
	void Load(string filePath);
	void OnBeat();
	POINT GetPlayerStartIndex() { return startIndex; }
	POINT GetNextStageIndex() { return endIndex; }
	void SetPlayerStartIndex(POINT p) { startIndex = p; }
	void SetNextStageIndex(POINT p) { endIndex = p;}
	int GetWidth() { return mapColumns; }
	int GetHeight() { return mapRows; }

	bool InteractTile(POINT index, GameActor* actor);

	vector<shared_ptr<TileActor>> GetRendableTileActors();
	void UpdateActiveTiles(POINT playerIndex);
	
	POINT GetLeftTop() { return leftTop; }
	POINT GetRightBottom() { return rightBottom; }

	void UpdateVisuable();
	void ApplyTorchLighting();
	void RemoveTorchLightingAt(POINT torchIndex);
	void AddSpawnPoint(POINT spawn) { spawnPoints.push_back(spawn); }

	void PrintSpawnPoints()
	{
		for (const auto& spawn : spawnPoints)
		{
			cout << "Spawn Point: (" << spawn.x << ", " << spawn.y << ")" << endl;
		}
	}
	void PrintTorchSpots()
	{
		for (const auto& torch : torchSpots)
		{
			cout << "Torch Spot: (" << torch.x << ", " << torch.y << ")" << endl;
		}
	}

	void SetIsCombo(bool _isCombo) { isCombo = _isCombo;}

	void ClearSpawnPoints() { spawnPoints.clear(); }
	void ClearTorchSpots() { torchSpots.clear(); }
	void AddTorchSpot(POINT spot) { torchSpots.push_back(spot); }
	
	void OnCombo() { isCombo = true; }
	void OffCombo() { isCombo = false; }
};

