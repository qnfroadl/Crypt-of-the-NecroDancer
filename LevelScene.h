#pragma once
#include "config.h"
#include "GameObject.h"
class TileActorRenderer;
class Tilemap;
class PlayerManager;
class UIManager;
class PositionManager;
class ItemSpawner;
class MonsterManager;
class ShadowCasting;
class BeatManager;
class LevelScene : public GameObject
{
private:
    shared_ptr<TileActorRenderer> renderer;
    shared_ptr<Tilemap> map;
    weak_ptr<PlayerManager> playerManager;
    shared_ptr<MonsterManager> monsterManager;
    shared_ptr<UIManager> uiManager;
    shared_ptr<PositionManager> positionManager;
    shared_ptr<ItemSpawner> itemSpawner;

    shared_ptr<ShadowCasting> shadowCasting;
    shared_ptr<BeatManager> beatManager;

    // 검정 배경
    HBRUSH blackBrush;
public:
    virtual HRESULT Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    void SetPlayerManager(shared_ptr<PlayerManager> playerManager);
};