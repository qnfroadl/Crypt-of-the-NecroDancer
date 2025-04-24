#pragma once
#include "GameObject.h"

// Tile and Monster need this
//struct SightInfo
//{
//	bool revealed;
//	bool visible;
//};

class Row
{
public:
	Row(int _depth, float _start, float _end)
		: depth(_depth), start(_start), end(_end) {
	}

	int depth;
	float start;
	float end;

	vector<POINT> GetDepthColume();
	Row GetNextRow();
};

class Tile;
class Player;
class ShadowCasting : public GameObject
{
private:
	int maxRow;
	int maxCol;
	vector<vector<bool>> sightMap; // 현재 시야에 보임 / 아님
	vector<vector<shared_ptr<Tile>>> tiles;

	vector<weak_ptr<Player>> players; // 2인 대비

	RECT cameraRect;

public:
	ShadowCasting();
	~ShadowCasting();

	void Init(vector<vector<shared_ptr<Tile>>> _tiles);
	void Release();
	void Update();
	void Render(HDC hdc);
	
	void AddPlayer(weak_ptr<Player> player);
	vector<vector<bool>>& GetSightMap() { return sightMap; }

	void InitShadowMap();
	void ComputeShadowMap(POINT playerPos);
	
	void Scan(POINT origin, Row row, int dx, int dy);
	void Reveal(shared_ptr<Tile> tile);
	void UnReveal(shared_ptr<Tile> tile);
	bool IsWall(shared_ptr<Tile> tile);
	bool IsFloor(shared_ptr<Tile> tile);
	bool IsSymmetric(Row row, POINT rowData);

	bool IsOutOfRange(POINT pos);

	void CalculateCameraRect();
};

