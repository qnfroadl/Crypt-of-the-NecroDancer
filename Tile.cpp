#include "Tile.h"
#include "Image.h"
#include "Camera.h"
#include "ImageManager.h"
#include "Block.h"
#include "Trap.h"
#include "CommonFunction.h"
#include "EventManager.h"

#define TILE_SIZE_SCALED (TILE_SIZE * TILE_SCALE)

HRESULT Tile::Init()
{
	tileImage = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"),
		234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255), true);

	type = TileType::NONE;
	tileNum = -1;
	block = nullptr;
	trap = nullptr;

	return S_OK;
}

HRESULT Tile::Init(int x, int y)
{
	tileImage = ImageManager::GetInstance()->AddImage("TILE", TEXT("Image/Tiles.bmp"),
		234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255), true);

	type = TileType::NONE;
	tileNum = 0;
	block = nullptr;
	trap = nullptr;

	pos = {
		x * TILE_SIZE * TILE_SCALE + (TILE_SIZE * TILE_SCALE) / 2.0f,
		y * TILE_SIZE * TILE_SCALE + (TILE_SIZE * TILE_SCALE) / 2.0f
	};
	index = { x, y };

	return S_OK;
}

void Tile::Release()
{
	//block = nullptr;
	//trap = nullptr;
	//tileImage = nullptr;
}

void Tile::Render(HDC hdc, bool useCamera)
{
	if (!tileImage) return;

	if (false == sightInfo.revealed && useCamera)
	{

		
		return;
	}

	float renderX = pos.x;
	float renderY = pos.y;

	if (useCamera)
	{
		renderX -= Camera::GetInstance()->GetPos().x;
		renderY -= Camera::GetInstance()->GetPos().y;

		//tileImage->RenderScaledImage(
		//	hdc,
		//	static_cast<int>(renderX),
		//	static_cast<int>(renderY),
		//	tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
		//	static_cast<float>(TILE_SCALE), static_cast<float>(TILE_SCALE),
		//	true
		//);
		float brightness = GetBrightness();
		tileImage->FrameRender(
			hdc,
			static_cast<int>(renderX),
			static_cast<int>(renderY),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			static_cast<float>(TILE_SCALE), static_cast<float>(TILE_SCALE),
			false, true, brightness, 0.0f
		);
	}
	else
	{
		tileImage->FrameRender(
			hdc,
			static_cast<int>(renderX / TILE_SCALE),
			static_cast<int>(renderY / TILE_SCALE),
			tileNum % SAMPLE_TILE_X, tileNum / SAMPLE_TILE_X,
			false, true
		);
	}

	if (trap) trap->Render(hdc, useCamera);
	if (block) 
	{
		block->SetBrightnessInfo(GetBrightnessInfoInfo());
		block->SetSightInfo(GetSightInfo());
		block->Render(hdc, useCamera);
	}
}

void Tile::Update()
{
	if (block) block->Update();
}

void Tile::OnTile(TileActor* actor)
{
	/*if (trap) trap->OnTriggered(actor);*/
	if (tileNum == 24)
	{
		EventManager::GetInstance()->AddEvent(EventType::ENTERZONE, nullptr);
	}
	//if (tileNum == 25)
	//{
	//	EventManager::GetInstance()->AddEvent(EventType::NEXTLEVEL, nullptr);
	//}
}

TileType Tile::GetTypeByTileNum(int tileNum)
{
	if (tileNum == 0) return TileType::DARK_DIRT;
	else if (tileNum == 1) return TileType::BRIGHT_DIRT;
	else if (tileNum == 10) return TileType::COMBO1_DIRT;
	else if (tileNum == 11) return TileType::COMBO2_DIRT;
	else return TileType::NONE;
}

void Tile::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		OnTile(dynamic_cast<TileActor*>(actor));
	}
}

void Tile::OnBeat(bool isCombo)
{
	if (isCombo)
	{
		switch (type)
		{
		case TileType::BRIGHT_DIRT:
			SetTileNum((index.x + index.y) % 2 == 0 ? 10 : 11);
			break;
		case TileType::DARK_DIRT:
			SetTileNum(1);
			break;
		case TileType::COMBO1_DIRT:
		case TileType::COMBO2_DIRT:
			SetTileNum(1);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case TileType::BRIGHT_DIRT:
			SetTileNum(0);
			break;
		case TileType::COMBO2_DIRT:
		SetTileNum((index.x + index.y) % 2 == 0 ? 1 : 0);
			break;
		case TileType::DARK_DIRT:
			SetTileNum(1);
			break;
		case TileType::COMBO1_DIRT:
			SetTileNum((index.x + index.y) % 2 == 0 ? 1 : 0);
			break;
		default:
			break;
		}
	}
}

vector<shared_ptr<TileActor>> Tile::GetRendableTileActors()
{
	vector<shared_ptr<TileActor>> result;
	result.push_back(shared_from_this());
	return result;
}
