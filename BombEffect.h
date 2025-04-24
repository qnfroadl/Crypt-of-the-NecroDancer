#pragma once

#include "Effect.h"

#include "Camera.h"
#include "TimerManager.h"
class Image;
class BombEffect : public Effect
{
private:
    int frame = 0;
    Image* image;
public:
    BombEffect(FPOINT pos);
    void Update() override;

    void Render(HDC hdc) override;

};

