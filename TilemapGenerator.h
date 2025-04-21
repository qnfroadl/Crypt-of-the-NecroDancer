#pragma once
#include "Singleton.h"
#include "Tilemap.h"

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
        RECT roomRect;
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
    vector<RECT> placedRects;
public:
    Tilemap* Generate(const string& zoneName, int mapRows, int mapCols);
    Tilemap* Generate(const string& zoneName);

private:
    void LoadMapFiles(const string& folderPath);
    RoomData ParseMapFile(const string& path);
    void ClearRooms();

    void SplitSpace(BSPNode* node, int depth);
    void DeleteBSPTree(BSPNode* node);

    bool IsOverlapping(const RECT& a, const RECT& b);

    void ConnectAllRooms(Tilemap* tilemap, const vector<RECT>& rooms);

    void PlaceRoomsFromBSP(BSPNode* root, Tilemap* tilemap, const string& zoneName);
    void ConnectLeafRooms(Tilemap* tilemap, BSPNode* node);

    void AddMapBorder(Tilemap* tilemap);

    void CreateCorridor(Tilemap* tilemap, POINT from, POINT to);
    void PlaceDoor(Tilemap* tilemap, POINT pos, POINT direction);
};