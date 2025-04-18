#pragma once
#include "config.h"
#include "GameObject.h"

class Tilemap;
class LobbyScene : public GameObject
{
private:
    Tilemap* map;

public:
    virtual HRESULT Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;
};