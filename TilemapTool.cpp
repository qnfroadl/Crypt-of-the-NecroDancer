#include "TilemapTool.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"
#include "KeyManager.h"
#include "Tile.h"
#include "Block.h"
#include <commdlg.h>
#include <fstream>
#include "Torch.h"

TilemapTool::TilemapTool()
	: sampleTile(nullptr), sampleWall(nullptr), currentTool(ToolType::NONE),
	mapSize(20) {
}

TilemapTool::~TilemapTool() {}

HRESULT TilemapTool::Init()
{
	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);

	selectedTileLX = selectedTileLY = selectedTileRX = selectedTileRY = 0;

	sampleTile = ImageManager::GetInstance()->AddImage("TILE", TEXT("../Image/Tiles.bmp"), 234, 156, SAMPLE_TILE_X, SAMPLE_TILE_Y, true, RGB(255, 0, 255));
	sampleWall = ImageManager::GetInstance()->AddImage("WALL", TEXT("../Image/Walls.bmp"), 216, 336, SAMPLE_WALL_X, SAMPLE_WALL_Y, true, RGB(255, 0, 255));
	ImageManager::GetInstance()->AddImage("TORCH", TEXT("../Image/Torch.bmp"), 24, 24, 4, 1, true, RGB(255, 0, 255));

	rcSampleTile = { TILEMAPTOOL_X - sampleTile->GetWidth(), 0, TILEMAPTOOL_X, sampleTile->GetHeight() };
	rcSampleWall = { TILEMAPTOOL_X - sampleWall->GetWidth(), rcSampleTile.bottom + 10, TILEMAPTOOL_X, rcSampleTile.bottom + 10 + sampleWall->GetHeight() };

	rcFloorEraser = { TILEMAPTOOL_X - 300, rcSampleWall.bottom + 60, TILEMAPTOOL_X - 200, rcSampleWall.bottom + 100 };
	rcWallEraser = { TILEMAPTOOL_X - 150, rcSampleWall.bottom + 60, TILEMAPTOOL_X - 50, rcSampleWall.bottom + 100 };
	rcSaveBtn = { TILEMAPTOOL_X - 300, rcWallEraser.bottom + 40, TILEMAPTOOL_X - 200, rcWallEraser.bottom + 80 };
	rcLoadBtn = { TILEMAPTOOL_X - 150, rcWallEraser.bottom + 40, TILEMAPTOOL_X - 50, rcWallEraser.bottom + 80 };
	rcSizeUpBtn = { 200, rcFloorEraser.top-30, 300, rcFloorEraser.bottom-40 };
	rcSizeDownBtn = { 50, rcFloorEraser.top-30, 150, rcFloorEraser.bottom-40 };
	rcMapTile = { 0, 0, mapSize * TILE_SIZE, mapSize * TILE_SIZE };
	rcPlayerStartBtn = { 50, rcSizeDownBtn.bottom + 10, 150, rcSizeDownBtn.bottom + 40 };
	rcNextStageBtn = { 200, rcSizeDownBtn.bottom + 10, 300, rcSizeDownBtn.bottom + 40 };
	rcTorchBtn = { 50, rcNextStageBtn.bottom + 10, 150, rcNextStageBtn.bottom + 40 };

	tiles = vector<vector<shared_ptr<Tile>>>(mapSize, vector<shared_ptr<Tile>>(mapSize, nullptr));

	for (int i = 0; i < mapSize; i++) 
	{
		for (int j = 0; j < mapSize; j++) 
		{
			auto tile = make_shared<Tile>();
			tile->Init(i, j);
			tile->SetTileNum(-1);
			tiles[j][i] = tile;
		}
	}
	return S_OK;
}

void TilemapTool::Release()
{
	if (!tiles.empty())
	{
		for (int j = 0; j < mapSize; j++)
		{
			for (int i = 0; i < mapSize; i++)
			{
				if (tiles[j][i])
					tiles[j][i]->Release();  // 메모리 해제는 shared_ptr이 자동 처리
			}
		}
		tiles.clear();
	}
}

void TilemapTool::Update()
{
	bool leftClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON);
	bool rightClick = KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON);

	if (PtInRect(&rcSaveBtn, g_ptMouse) && leftClick) {
		SaveDialog();
	}
	else if (PtInRect(&rcLoadBtn, g_ptMouse) && leftClick) {
		LoadDialog();
	}

	if (PtInRect(&rcSampleTile, g_ptMouse)) {
		if (leftClick || rightClick) {
			currentTool = ToolType::FLOOR_TILE;
			int x = g_ptMouse.x - rcSampleTile.left;
			int y = g_ptMouse.y - rcSampleTile.top;
			int tileX = x / TILE_SIZE;
			int tileY = y / TILE_SIZE;
			if (leftClick) selectedTileLX = tileX, selectedTileLY = tileY;
			else selectedTileRX = tileX, selectedTileRY = tileY;
		}
	}
	else if (PtInRect(&rcSampleWall, g_ptMouse)) {
		if (leftClick || rightClick) {
			currentTool = ToolType::WALL_TILE;
			int x = g_ptMouse.x - rcSampleWall.left;
			int y = g_ptMouse.y - rcSampleWall.top;
			int tileX = x / WALL_TILE_WIDTH;
			int tileY = y / WALL_TILE_HEIGHT;
			if (leftClick) selectedTileLX = tileX, selectedTileLY = tileY;
			else selectedTileRX = tileX, selectedTileRY = tileY;
		}
	}
	else if (PtInRect(&rcFloorEraser, g_ptMouse) && leftClick) currentTool = ToolType::FLOOR_ERASER;
	else if (PtInRect(&rcWallEraser, g_ptMouse) && leftClick) currentTool = ToolType::WALL_ERASER;

	else if (PtInRect(&rcMapTile, g_ptMouse)) {
		bool lStay = KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON);
		bool rStay = KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON);
		if (lStay || rStay) {
			int x = g_ptMouse.x / TILE_SIZE;
			int y = g_ptMouse.y / TILE_SIZE;
			if (x >= mapSize || y >= mapSize) return;
			switch (currentTool) {
			case ToolType::FLOOR_TILE: {
				int tileNum = (lStay ? selectedTileLX + selectedTileLY * SAMPLE_TILE_X : selectedTileRX + selectedTileRY * SAMPLE_TILE_X);
				tiles[y][x]->SetTileNum(tileNum);
				break;
			}
			case ToolType::WALL_TILE: {
				int tileNum = (lStay ? selectedTileLX + selectedTileLY * SAMPLE_WALL_X : selectedTileRX + selectedTileRY * SAMPLE_WALL_X);
				tiles[y][x]->SetBlock(nullptr);

				auto block = make_shared<Block>();
				block->Init(tiles[y][x]->GetPos(), tiles[y][x]->GetTileIndex());
				block->SetBlockNum(tileNum);
				tiles[y][x]->SetBlock(block);
				break;
			}
			case ToolType::FLOOR_ERASER:
				tiles[y][x]->SetTileNum(-1);
				break;
			case ToolType::WALL_ERASER:
				tiles[y][x]->SetBlock(nullptr);
				break;
			case ToolType::SET_PLAYER_START:
				playerStartX = x;
				playerStartY = y;
				break;

			case ToolType::SET_NEXT_STAGE:
				nextStageX = x;
				nextStageY = y;
				break;
			case ToolType::PLACE_TORCH: {
				auto block = tiles[y][x]->GetBlock();
				if (block) {
					auto torch = make_shared<Torch>();
					torch->Init(tiles[y][x]->GetPos(), tiles[y][x]->GetTileIndex());
					block->SetTorch(torch);
				}
				break;
			}
			default:
				break;
			}
		}
	}
	else if (PtInRect(&rcSizeUpBtn, g_ptMouse) && leftClick) {
		if (mapSize < 20) {
			Release();
			mapSize++;
			Init();
		}
	}
	else if (PtInRect(&rcSizeDownBtn, g_ptMouse) && leftClick) {
		if (mapSize > 3) {
			Release();
			mapSize--;
			Init();
		}
	}
	else if (PtInRect(&rcPlayerStartBtn, g_ptMouse) && leftClick) {
		currentTool = ToolType::SET_PLAYER_START;
	}
	else if (PtInRect(&rcNextStageBtn, g_ptMouse) && leftClick) {
		currentTool = ToolType::SET_NEXT_STAGE;
	}
	else if (PtInRect(&rcTorchBtn, g_ptMouse) && leftClick) {
		currentTool = ToolType::PLACE_TORCH;
	}

}

void TilemapTool::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, TILEMAPTOOL_X, TILEMAPTOOL_Y, WHITENESS);
	// 격자 그리기 (가로줄)
	HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200)); // 연한 회색 선
	HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

	for (int y = 0; y <= mapSize; ++y) {
		MoveToEx(hdc, 0, y * TILE_SIZE, NULL);
		LineTo(hdc, mapSize * TILE_SIZE, y * TILE_SIZE);
	}

	// 격자 그리기 (세로줄)
	for (int x = 0; x <= mapSize; ++x) {
		MoveToEx(hdc, x * TILE_SIZE, 0, NULL);
		LineTo(hdc, x * TILE_SIZE, mapSize * TILE_SIZE);
	}

	SelectObject(hdc, oldPen);
	DeleteObject(gridPen);
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			tiles[j][i]->Render(hdc, false);
			int cx = i * TILE_SIZE + TILE_SIZE / 2;
			int cy = j * TILE_SIZE + TILE_SIZE / 2;

			if (i == playerStartX && j == playerStartY) {
				HBRUSH old = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
				Ellipse(hdc, cx - 6, cy - 6, cx + 6, cy + 6);
				DeleteObject(SelectObject(hdc, old));
			}
			if (i == nextStageX && j == nextStageY) {
				HBRUSH old = (HBRUSH)SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
				Rectangle(hdc, cx - 6, cy - 6, cx + 6, cy + 6);
				DeleteObject(SelectObject(hdc, old));
			}
		}
	}

	sampleTile->Render(hdc, rcSampleTile.left, rcSampleTile.top);
	sampleWall->Render(hdc, rcSampleWall.left, rcSampleWall.top);

	// 버튼
	Rectangle(hdc, rcFloorEraser.left, rcFloorEraser.top, rcFloorEraser.right, rcFloorEraser.bottom);
	DrawText(hdc, TEXT("타일지우개"), -1, &rcFloorEraser, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	Rectangle(hdc, rcWallEraser.left, rcWallEraser.top, rcWallEraser.right, rcWallEraser.bottom);
	DrawText(hdc, TEXT("벽지우개"), -1, &rcWallEraser, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	Rectangle(hdc, rcSizeUpBtn.left, rcSizeUpBtn.top, rcSizeUpBtn.right, rcSizeUpBtn.bottom);
	DrawText(hdc, TEXT("크기 +"), -1, &rcSizeUpBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	Rectangle(hdc, rcSizeDownBtn.left, rcSizeDownBtn.top, rcSizeDownBtn.right, rcSizeDownBtn.bottom);
	DrawText(hdc, TEXT("크기 -"), -1, &rcSizeDownBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	Rectangle(hdc, rcPlayerStartBtn.left, rcPlayerStartBtn.top, rcPlayerStartBtn.right, rcPlayerStartBtn.bottom);
	DrawText(hdc, TEXT("플레이어위치"), -1, &rcPlayerStartBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	Rectangle(hdc, rcNextStageBtn.left, rcNextStageBtn.top, rcNextStageBtn.right, rcNextStageBtn.bottom);
	DrawText(hdc, TEXT("다음스테이지"), -1, &rcNextStageBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 샘플 타일
	int previewLX = TILEMAPTOOL_X - sampleTile->GetWidth() - 100;
	int previewLY = rcSampleWall.top;

	if (currentTool == ToolType::FLOOR_TILE)
		sampleTile->FrameRender(hdc, previewLX, previewLY, selectedTileLX, selectedTileLY, false, true);
	else if (currentTool == ToolType::WALL_TILE)
		sampleWall->FrameRender(hdc, previewLX, previewLY, selectedTileLX, selectedTileLY, false, true);

	// 저장 버튼
	Rectangle(hdc, rcSaveBtn.left, rcSaveBtn.top, rcSaveBtn.right, rcSaveBtn.bottom);
	DrawText(hdc, TEXT("저장"), -1, &rcSaveBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 불러오기 버튼
	Rectangle(hdc, rcLoadBtn.left, rcLoadBtn.top, rcLoadBtn.right, rcLoadBtn.bottom);
	DrawText(hdc, TEXT("불러오기"), -1, &rcLoadBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// Render()에서 버튼 표시
	Rectangle(hdc, rcTorchBtn.left, rcTorchBtn.top, rcTorchBtn.right, rcTorchBtn.bottom);
	DrawText(hdc, TEXT("횃불 배치"), -1, &rcTorchBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}



void TilemapTool::Save(string filePath)
{
	ofstream out(filePath);
	if (!out.is_open()) return;

	out << "TILEMAP" << endl;
	out << "SIZE " << mapSize << " " << mapSize << endl;

	out << "PLAYER_START " << playerStartX << " " << playerStartY << endl;
	out << "NEXT_STAGE " << nextStageX << " " << nextStageY << endl;

	out << "FLOOR" << endl;
	for (int j = 0; j < mapSize; j++) 
	{
		for (int i = 0; i < mapSize; i++) 
		{
			out << tiles[j][i]->GetTileNum() << " ";
		}
		out << endl;
	}

	out << "WALL" << endl;
	for (int j = 0; j < mapSize; j++) 
	{
		for (int i = 0; i < mapSize; i++) 
		{
			auto block = tiles[j][i]->GetBlock();
			if (block) out << block->GetBlockNum() << " ";
			else out << -1 << " ";
		}
		out << endl;
	}

	out << "TORCH" << endl;
	for (int j = 0; j < mapSize; j++) {
		for (int i = 0; i < mapSize; i++) {
			auto block = tiles[j][i]->GetBlock();
			if (block && block->GetTorch()) out << 1 << " ";
			else out << 0 << " ";
		}
		out << endl;
	}

	out.close();
}

void TilemapTool::Load(string filePath)
{
	ifstream in(filePath);
	if (!in.is_open()) return;

	string header;
	in >> header;
	if (header != "TILEMAP") return;

	string sizeLabel;
	int newWidth, newHeight;
	in >> sizeLabel >> newWidth >> newHeight;

	string section;
	in >> section;
	if (section == "PLAYER_START") 
	{
		in >> playerStartX >> playerStartY;
	}

	in >> section;
	if (section == "NEXT_STAGE") 
	{
		in >> nextStageX >> nextStageY;
	}

	// 기존 타일 제거
	for (int j = 0; j < tiles.size(); j++) 
	{
		for (int i = 0; i < tiles[j].size(); i++) 
		{
			if (tiles[j][i])
				tiles[j][i]->Release();
		}
	}
	tiles.clear();

	// 새 타일 배열 생성 (shared_ptr)
	tiles.resize(newHeight, vector<shared_ptr<Tile>>(newWidth, nullptr));
	for (int j = 0; j < newHeight; j++) 
	{
		for (int i = 0; i < newWidth; i++) 
		{
			auto tile = make_shared<Tile>();
			tile->Init(i, j);
			tiles[j][i] = tile;
		}
	}
	mapSize = newWidth;

	// FLOOR
	in >> section;
	if (section == "FLOOR") 
	{
		for (int j = 0; j < newHeight; j++) 
		{
			for (int i = 0; i < newWidth; i++) 
			{
				int tileNum;
				in >> tileNum;
				tiles[j][i]->SetTileNum(tileNum);
			}
		}
	}

	// WALL
	in >> section;
	if (section == "WALL") {
		for (int j = 0; j < newHeight; j++) 
		{
			for (int i = 0; i < newWidth; i++) 
			{
				int wallNum;
				in >> wallNum;

				tiles[j][i]->SetBlock(nullptr); // 기존 블록 제거

				if (wallNum >= 0) 
				{
					auto block = make_shared<Block>();
					block->Init(tiles[j][i]->GetPos(), tiles[j][i]->GetTileIndex());
					block->SetBlockNum(wallNum);	
					tiles[j][i]->SetBlock(block);
				}
			}
		}
	}

	in >> section;
	if (section == "TORCH") {
		for (int j = 0; j < newHeight; j++) {
			for (int i = 0; i < newWidth; i++) {
				int torchFlag;
				in >> torchFlag;
				auto block = tiles[j][i]->GetBlock();
				if (torchFlag == 1 && block) {
					auto torch = make_shared<Torch>();
					torch->Init(tiles[j][i]->GetPos(), tiles[j][i]->GetTileIndex());
					block->SetTorch(torch);
				}
			}
		}
	}

	in.close();
}

void TilemapTool::LoadDialog()
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[MAX_PATH] = TEXT("");

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = TEXT("Tilemap Files (*.map)\0*.map\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = TEXT("map");

	if (GetOpenFileName(&ofn)) 
	{
		std::wstring ws(ofn.lpstrFile);
		std::string path(ws.begin(), ws.end());

		Load(path);
	}
}


void TilemapTool::SaveDialog()
{
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[MAX_PATH] = TEXT("");

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = TEXT("Tilemap Files (*.map)\0*.map\0All Files (*.*)\0*.*\0");
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = TEXT("map");

	if (GetSaveFileName(&ofn)) 
	{
		std::wstring ws(ofn.lpstrFile);
		std::string path(ws.begin(), ws.end());

		Save(path);  // 실제 저장 함수 호출
	}
}