#pragma once

#include "GameObject.h"
#include <Array>

// 샘플 타일 정보
// 640 * 288
#define SAMPLE_TILE_X 20
#define SAMPLE_TILE_Y 9
#define TILE_SIZE 32

#define TILE_X 20
#define TILE_Y 20

typedef struct tagTile
{
	RECT rc;
	int frameX;
	int frameY;

}TileInfo;

class Image;
class TilemapTool : public GameObject
{
private:
	Image* sampleTile;
	std::array<TileInfo, TILE_X * TILE_Y> tilesInfo;

	RECT rcSampleTile;
	RECT rcMapTile;

	int selectedTileX;
	int selectedTileY;

public:
	TilemapTool();
	virtual ~TilemapTool();

	HRESULT Init() override;
	void Release()  override;
	void Update()  override;
	void Render(HDC hdc)  override;

	void Save();
	void Load();

		
};

