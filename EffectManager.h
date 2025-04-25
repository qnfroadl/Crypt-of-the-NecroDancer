#pragma once

#include "Singleton.h"
#include "GameActor.h"
#include "Effect.h"
#include <algorithm>

class Effect;
class EffectManager : public Singleton<EffectManager>
{
private:
    std::vector<std::shared_ptr<Effect>> effects;

public:
    void AddEffect(std::shared_ptr<Effect> effect)
    {
        effects.push_back(effect);
    }

    HRESULT Init()
    {

        return S_OK;
    }
    void Update()
    {
        for (auto it = effects.begin(); it != effects.end(); )
        {
            (*it)->Update();
            if ((*it)->IsFinished())
            {
                it = effects.erase(it);
                continue;
            }
            
            ++it;
                
        }
    }

    void Render(HDC hdc)
    {
        std::sort(effects.begin(), effects.end(), [](const auto& a, const auto& b) 
            {
                return a->GetPos().y < b->GetPos().y;
            });

        for (const auto& effect : effects)
        {
            effect->Render(hdc);
        }
            
    }

    void Release()
    {
        ReleaseInstance();
    }
};