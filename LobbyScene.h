#pragma once
#include "config.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "TileActorRenderer.h"

class Tilemap;
class PlayerManager;
class UIManager;
class PositionManager;
class ItemSpawner;
class MonsterManager;
class ShadowCasting;
class BeatManager;
class TileActorRenderer;

class LobbyScene : public GameObject
{
private:
    unique_ptr<TileActorRenderer> renderer;
    unique_ptr<LevelManager> levelManager;

    shared_ptr<Tilemap> map;
	weak_ptr<PlayerManager> playerManager;
	
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