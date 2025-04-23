#pragma once
#include "config.h"
#include "GameObject.h"

class Tilemap;
class PlayerManager;
class UIManager;
class PositionManager;
class ItemSpawner;
class MonsterManager;
class ShadowCasting;
class LobbyScene : public GameObject
{
private:
    shared_ptr<Tilemap> map;
	weak_ptr<PlayerManager> playerManager;
	weak_ptr<MonsterManager> monsterManager;
    UIManager* uiManager;
	shared_ptr<PositionManager> positionManager;
	shared_ptr<ItemSpawner> itemSpawner;

	shared_ptr<ShadowCasting> shadowCasting;

    // 검정 배경
    HBRUSH blackBrush;
public:
    virtual HRESULT Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

	void SetPlayerManager(shared_ptr<PlayerManager> playerManager);
    void SetMonsterManager(weak_ptr<MonsterManager> monsterManager);
};