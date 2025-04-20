#include "TilemapGenerator.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "Tile.h"
#include "Tilemap.h"
#include "Block.h"

RoomData TilemapGenerator::ParseMapFile(const string& path)
{
    ifstream in(path);
    RoomData room;

    if (!in.is_open()) return room;

    string header;
    in >> header;
    if (header != "TILEMAP") return room;

    string sizeLabel;
    int cols, rows;
    in >> sizeLabel >> cols >> rows;
    room.rows = rows;
    room.cols = cols;
    room.tiles.resize(rows, vector<Tile*>(cols, nullptr));

    string section;
    room.playerStart = { -1, -1 };
    room.nextStage = { -1, -1 };

    in >> section;
    if (section == "PLAYER_START") {
        int sx, sy;
        in >> sx >> sy;
        room.playerStart = { sx, sy };
    }

    in >> section;
    if (section == "NEXT_STAGE") {
        int ex, ey;
        in >> ex >> ey;
        room.nextStage = { ex, ey };
    }

    in >> section; // "FLOOR"
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int tileNum;
            in >> tileNum;

            Tile* tile = new Tile();
            tile->Init(x, y);
            tile->SetTileNum(tileNum);
            room.tiles[y][x] = tile;
        }
    }

    in >> section; // "WALL"
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int wallNum;
            in >> wallNum;

            if (wallNum >= 0) {
                Block* block = new Block();
                block->Init({ (float)x * TILE_SIZE, (float)y * TILE_SIZE }, { x, y });
                block->SetBlockNum(wallNum);
                room.tiles[y][x]->SetBlock(block);
            }
        }
    }

    return room;
}
void TilemapGenerator::LoadMapFiles(const string& folderPath)
{
    ClearRooms();
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA((folderPath + "\\*.map").c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do
    {
        string fileName = fd.cFileName;
        string fullPath = folderPath + "\\" + fileName;
        RoomData room = ParseMapFile(fullPath);

        if (fileName.find("BOSS") != string::npos)
        {
            string zone = fileName.substr(0, fileName.find("_BOSS"));
            zoneBossRoom[zone] = room;
        }
        else if (fileName.find("LOBBY") != string::npos)
        {
            string zone = fileName.substr(0, fileName.find("_LOBBY"));
            zoneLobbyRoom[zone] = room;
        }
        else if (fileName.find("SHOP") != string::npos)
        {
            string zone = fileName.substr(0, fileName.find("_SHOP"));
            zoneShopRooms[zone].push_back(room);
        }
        else
        {
            string zone = fileName.substr(0, fileName.find("_"));
            zoneNormalRooms[zone].push_back(room);
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

void TilemapGenerator::ClearRooms()
{
    zoneNormalRooms.clear();
    zoneShopRooms.clear();
    zoneBossRoom.clear();
    zoneLobbyRoom.clear();
}

Tilemap* TilemapGenerator::Generate(const string& zoneName, int mapRows, int mapCols)
{
    placedStart = false;
    placedEnd = false;

    Tilemap* resultMap = new Tilemap();
    resultMap->Init(mapRows, mapCols);

    RECT wholeArea = { 1, 1, mapCols - 1, mapRows - 1 };
    BSPNode* root = SplitSpace(wholeArea);
    PlaceRoomsFromBSP(resultMap, root, zoneName);
    ConnectLeafRooms(resultMap, root);

    return resultMap;
}

void TilemapGenerator::PlaceRoom(Tilemap* tilemap, const RoomData& room, int startRow, int startCol)
{
    for (int y = 0; y < room.rows; ++y)
    {
        for (int x = 0; x < room.cols; ++x)
        {
            Tile* src = room.tiles[y][x];
            if (!src) continue;

            Tile* tile = new Tile();
            tile->Init(startCol + x, startRow + y);
            tile->SetTileNum(src->GetTileNum());

            if (Block* srcBlock = src->GetBlock())
            {
                Block* block = new Block();
                block->Init(tile->GetPos(), tile->GetTileIndex());
                block->SetBlockNum(srcBlock->GetTileNum());
                tile->SetBlock(block);
            }

            tilemap->SetTile(startRow + y, startCol + x, tile);

            if ((x == room.playerStart.x && y == room.playerStart.y) && !placedStart)
            {
                tilemap->SetPlayerStartIndex({ startCol + x, startRow + y });
                placedStart = true;
            }
            if ((x == room.nextStage.x && y == room.nextStage.y) && !placedEnd)
            {
                tilemap->SetNextStageIndex({ startCol + x, startRow + y });
                placedEnd = true;
            }
        }
    }
}

TilemapGenerator::BSPNode* TilemapGenerator::SplitSpace(const RECT& area, int depth)
{
    const int MIN_ROOM_SIZE = 12;
    const int MAX_DEPTH = 4;

    BSPNode* node = new BSPNode();
    node->area = area;

    if (depth >= MAX_DEPTH || (area.right - area.left < 2 * MIN_ROOM_SIZE) || (area.bottom - area.top < 2 * MIN_ROOM_SIZE))
    {
        int roomLeft = area.left + rand() % ((area.right - area.left) / 4 + 1);
        int roomTop = area.top + rand() % ((area.bottom - area.top) / 4 + 1);
        int roomRight = area.right - rand() % ((area.right - area.left) / 4 + 1);
        int roomBottom = area.bottom - rand() % ((area.bottom - area.top) / 4 + 1);
        node->roomRect = { roomLeft, roomTop, roomRight, roomBottom };
        return node;
    }

    bool splitH = (rand() % 2 == 0);
    if ((area.right - area.left) > (area.bottom - area.top)) splitH = false;
    if ((area.bottom - area.top) > (area.right - area.left)) splitH = true;

    if (splitH)
    {
        int splitY = (area.top + area.bottom) / 2;
        node->left = SplitSpace({ area.left, area.top, area.right, splitY }, depth + 1);
        node->right = SplitSpace({ area.left, splitY, area.right, area.bottom }, depth + 1);
    }
    else
    {
        int splitX = (area.left + area.right) / 2;
        node->left = SplitSpace({ area.left, area.top, splitX, area.bottom }, depth + 1);
        node->right = SplitSpace({ splitX, area.top, area.right, area.bottom }, depth + 1);
    }

    return node;
}

void TilemapGenerator::PlaceRoomsFromBSP(Tilemap* map, BSPNode* node, const string& zoneName)
{
    if (!node) return;

    if (!node->left && !node->right)
    {
        if (!zoneNormalRooms[zoneName].empty())
        {
            RoomData room = zoneNormalRooms[zoneName][rand() % zoneNormalRooms[zoneName].size()];
            int startX = node->roomRect.left;
            int startY = node->roomRect.top;
            PlaceRoom(map, room, startY, startX);
        }
    }

    PlaceRoomsFromBSP(map, node->left, zoneName);
    PlaceRoomsFromBSP(map, node->right, zoneName);
}

void TilemapGenerator::ConnectRooms(Tilemap* tilemap, const POINT& a, const POINT& b)
{
    POINT cur = a;

    auto IsWall = [](Block* block) {
        if (!block) return false;
        BlockType type = block->GetType();
        return type == BlockType::NORMAL || type == BlockType::STONE || type == BlockType::CATACOMB;
        };

    // 1. 수평 이동
    while (cur.x != b.x)
    {
        cur.x += (b.x > cur.x) ? 1 : -1;

        Tile* tile = tilemap->GetTile(cur.y, cur.x);
        if (!tile) continue;

        Block* block = tile->GetBlock();
        if (!block)
        {
            tile->SetTileNum(0); // 바닥
        }
        else if (IsWall(block))
        {
            // 수평 연결 → 세로 도어 (blockNum 62)
            Block* door = new Block();
            door->Init(tile->GetPos(), tile->GetTileIndex());
            door->SetBlockNum(62);
            tile->SetBlock(door);
        }
    }

    // 2. 수직 이동
    while (cur.y != b.y)
    {
        cur.y += (b.y > cur.y) ? 1 : -1;

        Tile* tile = tilemap->GetTile(cur.y, cur.x);
        if (!tile) continue;

        Block* block = tile->GetBlock();
        if (!block)
        {
            tile->SetTileNum(0);
        }
        else if (IsWall(block))
        {
            // 수직 연결 → 가로 도어 (blockNum 61)
            Block* door = new Block();
            door->Init(tile->GetPos(), tile->GetTileIndex());
            door->SetBlockNum(61);
            tile->SetBlock(door);
        }
    }
}
void TilemapGenerator::ConnectLeafRooms(Tilemap* map, BSPNode* node)
{
    if (!node || !node->left || !node->right) return;

    POINT centerA = {
        (node->left->roomRect.left + node->left->roomRect.right) / 2,
        (node->left->roomRect.top + node->left->roomRect.bottom) / 2
    };

    POINT centerB = {
        (node->right->roomRect.left + node->right->roomRect.right) / 2,
        (node->right->roomRect.top + node->right->roomRect.bottom) / 2
    };

    ConnectRooms(map, centerA, centerB);

    ConnectLeafRooms(map, node->left);
    ConnectLeafRooms(map, node->right);
}
