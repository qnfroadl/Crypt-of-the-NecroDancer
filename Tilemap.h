#pragma once
#include "config.h"
#include "TileActor.h"

class Tile;
class Item;
//�̰� ��ü�� Ÿ�� ���ʷ����Ͱ� �� �����ؼ� ���� �־���� ��
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
	bool Destory(Item* item);
	bool Destory(int strong);
	bool CanMove(POINT index);
	void Move(TileActor* actor, POINT index);
	POINT GetSpawnIndex();
	void Load(string filePath);
};

