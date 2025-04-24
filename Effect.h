#pragma once

#include "GameActor.h"

class Effect : public GameActor
{
protected:
    float duration; // 지속 시간 (초)
    float elapsed = 0.0f;

public:
    virtual ~Effect() {}

    bool IsFinished() const { return elapsed >= duration; }
};