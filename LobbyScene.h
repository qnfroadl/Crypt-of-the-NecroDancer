#pragma once
#include "config.h"
#include "GameObject.h"

class Tilemap;
class PlayerManager;
class LobbyScene : public GameObject
{
private:
    shared_ptr<Tilemap> map;
	weak_ptr<PlayerManager> playerManager;

    // 검정 배경
    HBRUSH blackBrush;
public:
    virtual HRESULT Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

	void SetPlayerManager(shared_ptr<PlayerManager> playerManager);
};