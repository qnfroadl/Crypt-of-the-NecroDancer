#pragma once
#include "TileActor.h"
#include "Image.h"

enum class TrapType
{
	NONE,
	SPIKE,
	POISON,
	FIRE
};

class Trap : public TileActor
{
private:
	TrapType type;
	int tileNum;
	Image* trapImage;

public:
	Trap();
	~Trap();

	HRESULT Init();
	void Render(HDC hdc, bool useCamera);
	void OnTriggered(TileActor* actor);

	void SetTileNum(int num);
	int GetTileNum();

	void SetType(TrapType t);
	TrapType GetType();
};