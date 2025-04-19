#pragma once
#include "GameObject.h"

enum class SelectedLayer {
	FLOOR,  // 바닥을 선택
	WALL    // 벽을 선택
};
enum class ToolType {
	NONE,
	FLOOR_TILE,
	WALL_TILE,
	FLOOR_ERASER,
	WALL_ERASER
};
class Image;
class Tile;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	Image* sampleWall;
	vector<vector<Tile*>> tiles;

	RECT rcSampleTile, rcSampleWall, rcLeftClick, rcRightClick;
	RECT rcFloorEraser, rcWallEraser, rcMapTile;
	RECT rcSaveBtn;
	RECT rcLoadBtn;


	int selectedTileLX, selectedTileLY, selectedTileRX, selectedTileRY;

	SelectedLayer selectedLayer;
	ToolType currentTool;

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

