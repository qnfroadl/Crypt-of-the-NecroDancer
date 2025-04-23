#pragma once
#include "GameObject.h"
#include <memory>
#include <vector>

enum class SelectedLayer {
	FLOOR,
	WALL
};

enum class ToolType {
	NONE,
	FLOOR_TILE,
	WALL_TILE,
	FLOOR_ERASER,
	WALL_ERASER,
	SET_PLAYER_START,
	SET_NEXT_STAGE
};

class Image;
class Tile;

class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	Image* sampleWall;
	vector<vector<shared_ptr<Tile>>> tiles;

	RECT rcSampleTile, rcSampleWall, rcLeftClick, rcRightClick;
	RECT rcFloorEraser, rcWallEraser, rcMapTile;
	RECT rcSaveBtn;
	RECT rcLoadBtn;
	RECT rcResizeBtn;

	int selectedTileLX, selectedTileLY, selectedTileRX, selectedTileRY;
	int mapSize;

	RECT rcSizeUpBtn;
	RECT rcSizeDownBtn;
	SelectedLayer selectedLayer;
	ToolType currentTool;
	int playerStartX = 0, playerStartY = 0;
	int nextStageX = 0, nextStageY = 0;
	RECT rcPlayerStartBtn;
	RECT rcNextStageBtn;

public:
	TilemapTool();
	virtual ~TilemapTool();

	HRESULT Init() override;
	void Release() override;
	void Update() override;
	void Render(HDC hdc) override;

	void Save(string filePath);
	void Load(string filePath);

	void LoadDialog();
	void SaveDialog();
};
