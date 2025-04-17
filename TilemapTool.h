#pragma once
#include "GameObject.h"

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X 9
#define SAMPLE_TILE_Y 6

#define SAMPLE_WALL_X 9
#define SAMPLE_WALL_Y 8

#define TILE_SIZE 26
#define WALL_TILE_WIDTH 24
#define WALL_TILE_HEIGHT 48

#define TILE_X 20
#define TILE_Y 20

enum class SelectedLayer {
	FLOOR,  // 바닥을 선택
	WALL    // 벽을 선택
};

class Image;
class Tile;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	Image* sampleWall;
	vector<vector<Tile*>> tiles;

	RECT rcSampleTile;
	RECT rcSampleWall;
	RECT rcMapTile;

	int selectedTileLX;
	int selectedTileLY;

	int selectedTileRX;
	int selectedTileRY;

	SelectedLayer selectedLayer;
public:
	TilemapTool();
	virtual ~TilemapTool();

	HRESULT Init() override;
	void Release()  override;
	void Update()  override;
	void Render(HDC hdc)  override;

	void Save(string filePath);
	void Load(string filePath);

	void LoadDialog();
	void SaveDialog();

		
};

