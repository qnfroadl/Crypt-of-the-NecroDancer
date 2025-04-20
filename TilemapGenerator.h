#pragma once
#include "Singleton.h"
#include "Tilemap.h"
using namespace std;

struct RoomData
{
    int rows, cols;
    POINT playerStart;
    POINT nextStage;
    vector<vector<Tile*>> tiles;
};

class TilemapGenerator : public Singleton<TilemapGenerator>
{
private:
    struct PlacedRoomInfo
    {
        POINT center;
        POINT start;
        POINT end;
    };

    struct BSPNode
    {
        RECT area;
        RECT roomRect;
        BSPNode* left = nullptr;
        BSPNode* right = nullptr;
    };

    unordered_map<string, vector<RoomData>> zoneNormalRooms;
    unordered_map<string, vector<RoomData>> zoneShopRooms;
    unordered_map<string, RoomData> zoneBossRoom;
    unordered_map<string, RoomData> zoneLobbyRoom;

    bool placedStart = false;
    bool placedEnd = false;
public:
    Tilemap* Generate(const string& zoneName, int mapRows, int mapCols);
    void LoadMapFiles(const string& folderPath);
    RoomData ParseMapFile(const string& path);
    void PlaceRoom(Tilemap* tilemap, const RoomData& room, int startRow, int startCol);
    void ConnectRooms(Tilemap* tilemap, const POINT& a, const POINT& b);
    void ClearRooms();

    BSPNode* SplitSpace(const RECT& area, int depth = 0);
    void PlaceRoomsFromBSP(Tilemap* map, BSPNode* node, const string& zoneName);
    void ConnectLeafRooms(Tilemap* map, BSPNode* node);



};