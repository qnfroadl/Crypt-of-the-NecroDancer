#include "TilemapGenerator.h"
#include <fstream>
#include <random>
#include "Tile.h"
#include "Block.h"
#include <functional>

Tilemap* TilemapGenerator::Generate(const string& zoneName, int mapRows, int mapCols)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    LoadMapFiles("map");
    placedStart = false;
    placedEnd = false;
    placedRects.clear();

    Tilemap* tilemap = new Tilemap();
    tilemap->Init(mapRows, mapCols);

    BSPNode* root = new BSPNode{ {0, 0, mapCols, mapRows} };
    SplitSpace(root, 7);

    PlaceRoomsFromBSP(root, tilemap, zoneName);
    ConnectLeafRooms(tilemap, root);
    ConnectAllRooms(tilemap, placedRects);

    AddMapBorder(tilemap);

    DeleteBSPTree(root);
    POINT end = tilemap->GetNextStageIndex();
    Tile* endTile = tilemap->GetTile({ end.y, end.x });
    if (endTile)
        endTile->SetTileNum(24);

    return tilemap;
}
Tilemap* TilemapGenerator::Generate(const string& zoneName)
{
    LoadMapFiles("map");

    // 정해진 BOSS, LOBBY 맵만 로드
    string path;
    if (zoneName == "BOSS") {
        path = "map/BOSS.map";
    }
    else if (zoneName == "LOBBY") {
        path = "map/LOBBY.map";
    }
    else {
        return nullptr; // 해당 zoneName이 아님
    }

    RoomData room = ParseMapFile(path);
    if (room.tiles.empty()) return nullptr;

    Tilemap* tilemap = new Tilemap();
    tilemap->Init(room.rows, room.cols);

    for (int y = 0; y < room.rows; ++y)
    {
        for (int x = 0; x < room.cols; ++x)
        {
            Tile* src = room.tiles[y][x];
            Tile* copy = new Tile();
            copy->Init(x, y);
            if (src->GetTileNum() == 1 && (x + y) % 2 == 1)
                copy->SetTileNum(0);
            else if (src->GetTileNum() == 0 && (x + y) % 2 == 0)
                copy->SetTileNum(1);
            else
                copy->SetTileNum(src->GetTileNum());

            if (src->GetBlock())
            {
                Block* b = new Block();
                b->Init(copy->GetPos(), { x, y });
                b->SetBlockNum(src->GetBlock()->GetBlockNum());
                copy->SetBlock(b);
            }

            tilemap->SetTile(y, x, copy);
        }
    }

    tilemap->SetPlayerStartIndex({ room.playerStart.x, room.playerStart.y });
    tilemap->SetNextStageIndex({ room.nextStage.x, room.nextStage.y });
    return tilemap;
}
void TilemapGenerator::LoadMapFiles(const string& folderPath)
{
    ClearRooms();

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA((folderPath + "\\*.map").c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        string fileName = fd.cFileName;
        string path = folderPath + "\\" + fileName;
        RoomData room = ParseMapFile(path);

        if (room.tiles.empty()) continue; // 파싱 실패

        if (fileName.find("BOSS") != string::npos) {
            zoneBossRoom[fileName] = room;
        }
        else if (fileName.find("SHOP") != string::npos) {
            zoneShopRooms["ZONE1"].push_back(room);
        }
		else if (fileName.find("LOBBY") != string::npos) {
			zoneLobbyRoom["ZONE1"] = room;
		}
        else {
            zoneNormalRooms["ZONE1"].push_back(room);
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

RoomData TilemapGenerator::ParseMapFile(const string& path)
{
    ifstream in(path);
    RoomData room{};
    if (!in.is_open()) return room;

    string header;
    in >> header;
    if (header != "TILEMAP") return room;

    string label;
    int cols, rows;
    in >> label >> cols >> rows;
    room.cols = cols;
    room.rows = rows;
    room.tiles.resize(rows, vector<Tile*>(cols, nullptr));

    int sx, sy, ex, ey;
    in >> label >> sx >> sy;
    room.playerStart = { sx, sy };
    in >> label >> ex >> ey;
    room.nextStage = { ex, ey };

    in >> label; // FLOOR
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int num;
            in >> num;
            Tile* t = new Tile();
            t->Init(x, y);
            t->SetTileNum(num);
            room.tiles[y][x] = t;
        }
    }

    in >> label; // WALL
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            int num;
            in >> num;
            if (num >= 0) {
                Block* b = new Block();
                b->Init(room.tiles[y][x]->GetPos(), { x, y });
                b->SetBlockNum(num);
                room.tiles[y][x]->SetBlock(b);
            }
        }
    }

    return room;
}

void TilemapGenerator::ClearRooms()
{
    auto deleteRoomData = [](RoomData& room) {
        for (auto& row : room.tiles) {
            for (auto& tile : row) {
                if (tile) {
                    if (tile->GetBlock()) {
                        delete tile->GetBlock();
                        tile->SetBlock(nullptr);
                    }
                    tile->Release(); // 혹시 정의되어 있다면
                    delete tile;
                }
            }
        }
        room.tiles.clear();
        };

    for (auto& pair : zoneNormalRooms) {
        for (RoomData& r : pair.second) deleteRoomData(r);
    }
    for (auto& pair : zoneShopRooms) {
        for (RoomData& r : pair.second) deleteRoomData(r);
    }
    for (auto& pair : zoneBossRoom) {
        deleteRoomData(pair.second);
    }
    for (auto& pair : zoneLobbyRoom) {
        deleteRoomData(pair.second);
    }

    zoneNormalRooms.clear();
    zoneShopRooms.clear();
    zoneBossRoom.clear();
    zoneLobbyRoom.clear();
}

void TilemapGenerator::SplitSpace(BSPNode* node, int depth)
{
    if (depth <= 0) return;

    const int minRoomSize = 7;  // 최소 방이 들어갈 수 있는 크기
    const int minSplitSize = 10; // 기존 split 한계

    int width = node->area.right - node->area.left;
    int height = node->area.bottom - node->area.top;

    if (width < minRoomSize || height < minRoomSize) return; //  방 불가

    bool horizontal = (width < height);
    if ((horizontal && height < minSplitSize * 2) || (!horizontal && width < minSplitSize * 2)) return;

    int split;
    if (horizontal) {
        int minY = node->area.top + minSplitSize;
        int maxY = node->area.bottom - minSplitSize;
        if (maxY <= minY) return;
        split = rand() % (maxY - minY) + minY;

        node->left = new BSPNode();
        node->right = new BSPNode();
        node->left->area = { node->area.left, node->area.top, node->area.right, split };
        node->right->area = { node->area.left, split, node->area.right, node->area.bottom };
    }
    else {
        int minX = node->area.left + minSplitSize;
        int maxX = node->area.right - minSplitSize;
        if (maxX <= minX) return;
        split = rand() % (maxX - minX) + minX;

        node->left = new BSPNode();
        node->right = new BSPNode();
        node->left->area = { node->area.left, node->area.top, split, node->area.bottom };
        node->right->area = { split, node->area.top, node->area.right, node->area.bottom };
    }

    SplitSpace(node->left, depth - 1);
    SplitSpace(node->right, depth - 1);
}
void TilemapGenerator::PlaceRoomsFromBSP(BSPNode* node, Tilemap* tilemap, const string& zoneName)
{
    vector<BSPNode*> leaves;

    // 리프 노드 수집
    function<void(BSPNode*)> collectLeaves = [&](BSPNode* n) {
        if (!n->left && !n->right)
            leaves.push_back(n);
        else {
            if (n->left) collectLeaves(n->left);
            if (n->right) collectLeaves(n->right);
        }
        };
    collectLeaves(node);

    // 무작위 섞기
    random_device rd;
    mt19937 g(rd());
    shuffle(leaves.begin(), leaves.end(), g);

    // 방 배치
    bool shopPlaced = false;

    for (BSPNode* leaf : leaves)
    {
        RoomData room;
        bool isShopRoom = false;

        if (!shopPlaced && !zoneShopRooms[zoneName].empty()) {
            room = zoneShopRooms[zoneName][rand() % zoneShopRooms[zoneName].size()];
            shopPlaced = true;
            isShopRoom = true;
        }
        else {
            vector<RoomData>& normalRooms = zoneNormalRooms[zoneName];
            if (normalRooms.empty()) continue;
            room = normalRooms[rand() % normalRooms.size()];
        }

        int rw = room.cols;
        int rh = room.rows;

        int aw = leaf->area.right - leaf->area.left;
        int ah = leaf->area.bottom - leaf->area.top;
        int offsetX = leaf->area.left + (aw - rw) / 2;
        int offsetY = leaf->area.top + (ah - rh) / 2;

        RECT tryRect = { offsetX, offsetY, offsetX + rw, offsetY + rh };
        if (offsetX + rw > tilemap->GetWidth() || offsetY + rh > tilemap->GetHeight())
            continue;

        for (const RECT& r : placedRects)
        {
            if (IsOverlapping(tryRect, r)) goto skip;
        }

        leaf->roomRect = tryRect;
        placedRects.push_back(tryRect);

        for (int y = 0; y < rh; ++y)
        {
            for (int x = 0; x < rw; ++x)
            {
                Tile* src = room.tiles[y][x];
                Tile* copy = new Tile();

                int globalX = offsetX + x;
                int globalY = offsetY + y;

                copy->Init(globalX, globalY); //  Init은 (x, y)
                if (src->GetTileNum() == 1 && (globalX + globalY) % 2 == 1)
                    copy->SetTileNum(0);
                else if (src->GetTileNum() == 0 && (globalX + globalY) % 2 == 0)
                    copy->SetTileNum(1);
                else
                    copy->SetTileNum(src->GetTileNum());

                if (src->GetBlock())
                {
                    Block* b = new Block();
                    b->Init(copy->GetPos(), { globalX, globalY }); //  (x, y)
                    b->SetBlockNum(src->GetBlock()->GetBlockNum());
                    copy->SetBlock(b);
                }

                tilemap->SetTile(globalY, globalX, copy); //  tilemap은 (y, x)
            }
        }

        // 시작/종료 위치 설정
        if (!isShopRoom)
        {
            int cx = (tryRect.left + tryRect.right) / 2;
            int cy = (tryRect.top + tryRect.bottom) / 2;
            if (!placedStart)
            {
                tilemap->SetPlayerStartIndex({ cx, cy });
                placedStart = true;
            }
            else if (!placedEnd)
            {
                tilemap->SetNextStageIndex({ cx, cy });
                placedEnd = true;
            }
        }

    skip:
        continue;
    }
}

void TilemapGenerator::ConnectLeafRooms(Tilemap* tilemap, BSPNode* node)
{
    if (!node || !node->left || !node->right) return;

    ConnectLeafRooms(tilemap, node->left);
    ConnectLeafRooms(tilemap, node->right);

    RECT a = node->left->roomRect;
    RECT b = node->right->roomRect;

    POINT bestA = {}, bestB = {};
    int minDist = INT_MAX;

    for (int ay = a.top; ay < a.bottom; ++ay)
    {
        for (int ax = a.left; ax < a.right; ++ax)
        {
            for (int by = b.top; by < b.bottom; ++by)
            {
                for (int bx = b.left; bx < b.right; ++bx)
                {
                    int dist = abs(ax - bx) + abs(ay - by);
                    if (dist < minDist)
                    {
                        bestA = { ax, ay }; //  (x, y)
                        bestB = { bx, by }; //  (x, y)
                        minDist = dist;
                    }
                }
            }
        }
    }

    CreateCorridor(tilemap, bestA, bestB); //  (x, y)

    // 문 방향 계산
    POINT dirAtoB = { bestB.x - bestA.x, bestB.y - bestA.y };
    if (dirAtoB.x != 0) dirAtoB.x = (dirAtoB.x > 0) ? 1 : -1;
    if (dirAtoB.y != 0) dirAtoB.y = (dirAtoB.y > 0) ? 1 : -1;

    PlaceDoor(tilemap, bestA, dirAtoB);             //  좌표 그대로
    PlaceDoor(tilemap, bestB, { -dirAtoB.x, -dirAtoB.y }); //  반대 방향
}

void TilemapGenerator::CreateCorridor(Tilemap* tilemap, POINT from, POINT to)
{
    POINT cur = from;
    bool horizontalFirst = rand() % 2 == 0;
    bool isHorizontal = (from.y == to.y);

    auto carve = [&](int x, int y)
        {
            Tile* tile = tilemap->GetTile({ x, y });
            if (!tile)
            {
                tile = new Tile();
                tile->Init(x, y); //  (x, y)
                tilemap->SetTile(y, x, tile); //  (y, x)
            }

            int tileNum = ((x + y) % 2 == 0) ? 1 : 0;
            tile->SetTileNum(tileNum);
            if (tile->GetBlock()) {
                delete tile->GetBlock();
                tile->SetBlock(nullptr);
            }

            // 양옆 벽 만들기
            POINT sideOffsets[2];
            if (isHorizontal) {
                sideOffsets[0] = { 0, -1 }; // 위
                sideOffsets[1] = { 0,  1 }; // 아래
            }
            else {
                sideOffsets[0] = { -1, 0 }; // 왼쪽
                sideOffsets[1] = { 1, 0 };  // 오른쪽
            }

            for (int i = 0; i < 2; ++i)
            {
                int nx = x + sideOffsets[i].x;
                int ny = y + sideOffsets[i].y;
                Tile* neighbor = tilemap->GetTile({ nx, ny });
                if (!neighbor)
                {
                    neighbor = new Tile();
                    neighbor->Init(nx, ny); //  (x, y)
                    int tileNum = ((nx + ny) % 2 == 0) ? 1 : 0;
                    neighbor->SetTileNum(tileNum);
                    tilemap->SetTile(ny, nx, neighbor); //  (y, x)
                }
                if (!neighbor->GetBlock())
                {
                    Block* wall = new Block();
                    wall->Init(neighbor->GetPos(), { nx, ny });
                    wall->SetBlockNum(rand() % 18);
                    neighbor->SetBlock(wall);
                }
            }
        };

    if (horizontalFirst)
    {
        while (cur.x != to.x) { carve(cur.x, cur.y); cur.x += (to.x > cur.x ? 1 : -1); }
        while (cur.y != to.y) { carve(cur.x, cur.y); cur.y += (to.y > cur.y ? 1 : -1); }
    }
    else
    {
        while (cur.y != to.y) { carve(cur.x, cur.y); cur.y += (to.y > cur.y ? 1 : -1); }
        while (cur.x != to.x) { carve(cur.x, cur.y); cur.x += (to.x > cur.x ? 1 : -1); }
    }
    carve(to.x, to.y);

    // 복도 양 끝 옆 벽 보장
    POINT ends[2] = { from, to };
    for (int i = 0; i < 2; ++i)
    {
        int cx = ends[i].x;
        int cy = ends[i].y;
        POINT sideOffsets[2];
        if (isHorizontal) {
            sideOffsets[0] = { 0, -1 };
            sideOffsets[1] = { 0,  1 };
        }
        else {
            sideOffsets[0] = { -1, 0 };
            sideOffsets[1] = { 1, 0 };
        }

        for (int j = 0; j < 2; ++j)
        {
            int nx = cx + sideOffsets[j].x;
            int ny = cy + sideOffsets[j].y;
            Tile* neighbor = tilemap->GetTile({ nx, ny });
            if (!neighbor)
            {
                neighbor = new Tile();
                neighbor->Init(nx, ny); //  (x, y)
                int tileNum = ((nx + ny) % 2 == 0) ? 1 : 0;
                neighbor->SetTileNum(tileNum);
                tilemap->SetTile(ny, nx, neighbor); //  (y, x)
            }
            if (!neighbor->GetBlock())
            {
                Block* wall = new Block();
                wall->Init(neighbor->GetPos(), { nx, ny });
                wall->SetBlockNum(rand() % 18);
                neighbor->SetBlock(wall);
            }
        }
    }
}
void TilemapGenerator::PlaceDoor(Tilemap* tilemap, POINT doorPos, POINT direction)
{
    // 중심 타일 보장
    Tile* tile = tilemap->GetTile({ doorPos.x, doorPos.y }); //  (y, x)
    if (!tile)
    {
        tile = new Tile();
        tile->Init(doorPos.x, doorPos.y); //  (x, y)
        tilemap->SetTile(doorPos.y, doorPos.x, tile); //  (y, x)
    }

    int tileNum = ((doorPos.x + doorPos.y) % 2 == 0) ? 1 : 0;
    tile->SetTileNum(tileNum);

    if (tile->GetBlock())
    {
        delete tile->GetBlock();
        tile->SetBlock(nullptr);
    }

    // 문 설치
    Block* door = new Block();
    door->Init(tile->GetPos(), doorPos); //  (x, y)
    door->SetBlockNum((direction.x != 0) ? 62 : 61);
    tile->SetBlock(door);

    // 앞뒤는 반드시 비워야 함
    int backX = doorPos.x + direction.x;
    int backY = doorPos.y + direction.y;
    Tile* forward = tilemap->GetTile({ backX, backY });
    if (forward && forward->GetBlock()) {
        delete forward->GetBlock();
        forward->SetBlock(nullptr);
    }

    int frontX = doorPos.x - direction.x;
    int frontY = doorPos.y - direction.y;
    Tile* backward = tilemap->GetTile({ frontX, frontY });
    if (backward && backward->GetBlock()) {
        delete backward->GetBlock();
        backward->SetBlock(nullptr);
    }

    // 좌우 벽은 반드시 있어야 함
    int leftX = doorPos.x - direction.y;
    int leftY = doorPos.y + direction.x;
    int rightX = doorPos.x + direction.y;
    int rightY = doorPos.y - direction.x;

    POINT wallSpots[2] = { { leftX, leftY }, { rightX, rightY } };
    for (int i = 0; i < 2; ++i) {
        int wx = wallSpots[i].x;
        int wy = wallSpots[i].y;
        Tile* wallTile = tilemap->GetTile({ wx, wy }); //  (y, x)
        if (!wallTile)
        {
            wallTile = new Tile();
            wallTile->Init(wx, wy); //  (x, y)
            tilemap->SetTile(wy, wx, wallTile); //  (y, x)
        }

        int tileNum = ((wx + wy) % 2 == 0) ? 1 : 0;
        wallTile->SetTileNum(tileNum);

        if (!wallTile->GetBlock())
        {
            Block* wall = new Block();
            wall->Init(wallTile->GetPos(), { wx, wy }); //  (x, y)
            wall->SetBlockNum(rand() % 18);
            wallTile->SetBlock(wall);
        }
    }
}

void TilemapGenerator::AddMapBorder(Tilemap* tilemap)
{
    int w = tilemap->GetWidth();
    int h = tilemap->GetHeight();

    // nullptr 옆에 있는 타일 경계에 벽 생성
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Tile* center = tilemap->GetTile({ x, y }); //  (y, x)
            if (!center) continue;

            POINT offsets[4] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
            for (int i = 0; i < 4; ++i)
            {
                int nx = x + offsets[i].x;
                int ny = y + offsets[i].y;

                if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;

                Tile* neighbor = tilemap->GetTile({ nx, ny }); //  (y, x)
                if (!neighbor)
                {
                    Tile* wallTile = new Tile();
                    wallTile->Init(nx, ny); //  (x, y)
                    int tileNum = ((nx + ny) % 2 == 0) ? 1 : 0;
                    wallTile->SetTileNum(tileNum);

                    Block* wall = new Block();
                    wall->Init(wallTile->GetPos(), { nx, ny }); //  (x, y)
                    wall->SetBlockNum(rand() % 18);
                    wallTile->SetBlock(wall);

                    tilemap->SetTile(ny, nx, wallTile); //  (y, x)
                }
            }
        }
    }

    // 최외곽 테두리 벽 60번으로 설정
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            bool isEdge = (x == 0 || x == w - 1 || y == 0 || y == h - 1);
            if (!isEdge) continue;

            Tile* tile = tilemap->GetTile({ x, y });
            if (!tile)
            {
                tile = new Tile();
                tile->Init(x, y); //  (x, y)
                int tileNum = ((x + y) % 2 == 0) ? 1 : 0;
                tile->SetTileNum(tileNum);
                tilemap->SetTile(y, x, tile); //  (y, x)
            }

            if (tile->GetBlock())
            {
                delete tile->GetBlock();
                tile->SetBlock(nullptr);
            }

            Block* b = new Block();
            b->Init(tile->GetPos(), { x, y }); //  (x, y)
            b->SetBlockNum(60);
            tile->SetBlock(b);
        }
    }
}
void TilemapGenerator::DeleteBSPTree(BSPNode* node)
{
    if (!node) return;
    DeleteBSPTree(node->left);
    DeleteBSPTree(node->right);
    delete node;
}

bool TilemapGenerator::IsOverlapping(const RECT& a, const RECT& b)
{
    return (a.left < b.right && a.right > b.left &&
        a.top < b.bottom && a.bottom > b.top);
}

void TilemapGenerator::ConnectAllRooms(Tilemap* tilemap, const vector<RECT>& rooms)
{
    int n = static_cast<int>(rooms.size());
    vector<int> group(n);
    for (int i = 0; i < n; ++i) group[i] = i;

    function<int(int)> find = [&](int x) {
        if (group[x] != x) group[x] = find(group[x]);
        return group[x];
        };

    auto merge = [&](int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) group[b] = a;
        };

    auto getEdgePoint = [](const RECT& r) -> vector<POINT> {
        vector<POINT> points;
        for (int x = r.left; x < r.right; ++x) {
            points.push_back({ x, r.top });
            points.push_back({ x, r.bottom - 1 });
        }
        for (int y = r.top; y < r.bottom; ++y) {
            points.push_back({ r.left, y });
            points.push_back({ r.right - 1, y });
        }
        return points;
        };

    bool shopConnected = false;

    while (true)
    {
        vector<pair<int, int>> toConnect;
        int minDist = INT_MAX;
        POINT bestA = {}, bestB = {};

        for (int i = 0; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (find(i) == find(j)) continue;

                // 상점 중복 연결 방지
                if (shopConnected && (zoneShopRooms["ZONE1"].size() > 0) &&
                    (i == 0 || j == 0)) continue;

                vector<POINT> edgesA = getEdgePoint(rooms[i]);
                vector<POINT> edgesB = getEdgePoint(rooms[j]);

                for (const POINT& a : edgesA)
                {
                    for (const POINT& b : edgesB)
                    {
                        int dist = abs(a.x - b.x) + abs(a.y - b.y);
                        if (dist < minDist)
                        {
                            minDist = dist;
                            bestA = a; //  (x, y)
                            bestB = b; //  (x, y)
                            toConnect = { {i, j} };
                        }
                    }
                }
            }
        }

        if (toConnect.empty()) break;

        int a = toConnect[0].first;
        int b = toConnect[0].second;

        CreateCorridor(tilemap, bestA, bestB); //  전달은 (x, y)

        POINT dir = { bestB.x - bestA.x, bestB.y - bestA.y };
        if (dir.x != 0) dir.x = (dir.x > 0 ? 1 : -1);
        if (dir.y != 0) dir.y = (dir.y > 0 ? 1 : -1);

        PlaceDoor(tilemap, bestA, dir);
        PlaceDoor(tilemap, bestB, { -dir.x, -dir.y });

        if ((a == 0 || b == 0) && zoneShopRooms["ZONE1"].size() > 0)
            shopConnected = true;

        merge(a, b);
    }
}