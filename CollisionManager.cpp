#include "CollisionManager.h"
#include "CommonFunction.h"
#include "GameActor.h"

void Collision::SetGameActor(GameActor* actor)
{
    this->actor = actor;
}

GameActor* Collision::GetGameActor()
{
    return actor;
}

CollisionType Collision::GetType()
{
    return this->type;
}

void Collision::SetType(CollisionType type)
{
    this->type = type;
}

void Collision::SetRelativePos(const FPOINT& pos)
{
    this->relativePos = pos;
}

const FPOINT& Collision::GetPos()
{
    FPOINT pos = actor->GetPos();
    return pos + this->relativePos;
}

/*
    CollisionRect 구현부 시작
*/

CollisionRect::CollisionRect()
    :width(10), height(10)
{
    SetType(CollisionType::RECT);
}

const RECT& CollisionRect::GetRect()
{
    return GetRectAtCenter(GetPos().x, GetPos().y, width, height);
}

void CollisionRect::SetWidth(int _width)
{
    this->width = width;
}

void CollisionRect::SetHeight(int _height)
{
    this->height = _height;
}

void CollisionRect::Render(HDC hdc)
{
    if (this->IsActive())
    {
        RenderRectAtCenter(hdc, GetPos().x, GetPos().y, this->width, this->height);
    }
}

CollisionRect* CollisionManager::CreateCollisionRect(CollisionLayer layer, GameActor* actor, int _width, int _height, const FPOINT& _relativePos)
{
    CollisionRect* coll = nullptr;
    if (nullptr != actor)
    {
        coll = rtPool->GetPoolObject();
        //   매번 생성이 아니라, 콜리젼 풀에서 가져오는 방식이라면? 
        coll->SetGameActor(actor);
        coll->SetWidth(_width);
        coll->SetHeight(_height);
        coll->SetRelativePos(_relativePos);
        coll->SetActive(true);

        coll->layer = layer;
        (*layerCollisionMap)[layer].insert(coll);
    }

    return coll;
}

CollisionCircle* CollisionManager::CreateCollisionCircle(CollisionLayer layer, GameActor* actor, int radius, const FPOINT& _relativePos)
{
    CollisionCircle* coll = nullptr;

    if (nullptr != actor)
    {
        coll = ccPool->GetPoolObject();
        coll->SetGameActor(actor);
        coll->SetRadius(radius);
        coll->SetRelativePos(_relativePos);
        coll->SetActive(true);

        coll->layer = layer;
        (*layerCollisionMap)[layer].insert(coll);
    }

    return coll;
}

void CollisionManager::ReleaseCollision(Collision* coll)
{
    if (nullptr != coll)
    {
        layerCollisionMap->at(coll->layer).erase(coll);
       
        coll->SetActive(false);
        switch(coll->GetType())
        {
        case CollisionType::RECT:
            rtPool->ReleaseObject(static_cast<CollisionRect*>(coll));
        break;
        case CollisionType::CIRCLE:
            ccPool->ReleaseObject(static_cast<CollisionCircle*>(coll));
        break;
        }
    }
}

HRESULT CollisionManager::Init()
{
    rtPool = new ObjectPool<CollisionRect>(10);
    ccPool = new ObjectPool<CollisionCircle>(10);

    brush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    pen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
    this->checkCount = 0;

    layerCollisionMap = new unordered_map<CollisionLayer, unordered_set<Collision*>>();

    (*layerCollisionMap)[CollisionLayer::Player].reserve(10);
    (*layerCollisionMap)[CollisionLayer::PlayerAttack].reserve(200);
    (*layerCollisionMap)[CollisionLayer::Enemy].reserve(50);
    (*layerCollisionMap)[CollisionLayer::EnemyAttack].reserve(200);
    (*layerCollisionMap)[CollisionLayer::Item].reserve(10);

    layerMaskMap[CollisionLayer::Player] = uint8_t(CollisionLayer::Enemy) | uint8_t(CollisionLayer::EnemyAttack) | uint8_t(CollisionLayer::Item);
    layerMaskMap[CollisionLayer::PlayerAttack] = uint8_t(CollisionLayer::Enemy);
    layerMaskMap[CollisionLayer::Enemy] = uint8_t(CollisionLayer::Player) | uint8_t(CollisionLayer::PlayerAttack);
    layerMaskMap[CollisionLayer::EnemyAttack] = uint8_t(CollisionLayer::Player);
    layerMaskMap[CollisionLayer::Item] = uint8_t(CollisionLayer::Player);


    collisionFuncsMap[{CollisionType::RECT, CollisionType::RECT}] = [](Collision* c1, Collision* c2) {
        auto* r1 = static_cast<CollisionRect*>(c1);
        auto* r2 = static_cast<CollisionRect*>(c2);
        return RectInRect(r1->GetRect(), r2->GetRect());
        };
    
    collisionFuncsMap[{CollisionType::CIRCLE, CollisionType::CIRCLE}] = [](Collision* c1, Collision* c2) {
        auto* c1c = static_cast<CollisionCircle*>(c1);
        auto* c2c = static_cast<CollisionCircle*>(c2);
        return CircleInCircle(c1c->GetCircle(), c2c->GetCircle());
        };
    
    collisionFuncsMap[{CollisionType::RECT, CollisionType::CIRCLE}] = [](Collision* c1, Collision* c2) {
        auto* r = static_cast<CollisionRect*>(c1);
        auto* c = static_cast<CollisionCircle*>(c2);
        return RectInCircle(r->GetRect(), c->GetCircle());
        };
    
    collisionFuncsMap[{CollisionType::CIRCLE, CollisionType::RECT}] = [](Collision* c1, Collision* c2) {
        auto* c = static_cast<CollisionCircle*>(c1);
        auto* r = static_cast<CollisionRect*>(c2);
        return RectInCircle(r->GetRect(), c->GetCircle());
        };

    return S_OK;

}

void CollisionManager::Update()
{
    checkCount = 0;
    CollisionDetect();   
}

void CollisionManager::Render(HDC hdc)
{
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);

    for (const auto& pair : (*layerCollisionMap))
    {
        for (Collision* col : pair.second)
        {
            if (col->IsActive())
            {
                col->Render(hdc);
            }
        }
    }

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
}

void CollisionManager::Release()
{
    // layerMap
    // 남아있는 콜리젼 해제

    for (const auto& pair : (*layerCollisionMap))
    {
        for (Collision* col : pair.second)
        {
            col->Release();
            delete col;
        }
    }
    layerCollisionMap->clear();

    DeleteObject(brush);
    DeleteObject(pen);

    Singleton::ReleaseInstance();
}

void CollisionManager::CollisionDetect()
{
    uint8_t mask;
    for (const auto& pair : *layerCollisionMap)
    {
        mask = layerMaskMap[pair.first];

        for (const auto& pair2 : *layerCollisionMap)
        {
            // 검사 해야하는 레이어라면
            if (mask & uint8_t(pair2.first))    
            {
                CollisionDetect(pair.second, pair2.second);
            }
        }
    }
}

int CollisionManager::GetCheckCount()
{
    return this->checkCount;
}

int CollisionManager::GetCollisionCount()
{
    int collisionCount = 0;
    auto it = layerCollisionMap->begin();
    while (it != layerCollisionMap->end())
    {
        collisionCount += it->second.size();
        it++;
    }
    return collisionCount;
}

int CollisionManager::GetActivecollisionCount()
{
    // 성능 문제되면 호출 금지.
    int collisionCount = 0;
    for (const auto& pair : *layerCollisionMap)
    {
        for (const auto& col : pair.second)
        {
            if (col->IsActive())
            {
                collisionCount++;
            }
        }
    }
   
    return collisionCount;
}

void CollisionManager::CollisionDetect(const unordered_set<Collision*>& setColl1, const  unordered_set<Collision*>& setColl2)
{
    for (auto c1 : setColl1)
    {
        for (auto c2 : setColl2)
        {
            if (c1->IsActive() && c2->IsActive())
            {
                checkCount++;
                Detect(c1, c2);
            }
        }
    }
}

void CollisionManager::Detect(Collision* c1, Collision* c2)
{
    auto type1 = c1->GetType();
    auto type2 = c2->GetType();

    auto key = std::make_pair(type1, type2);

    auto it = collisionFuncsMap.find(key);
    if (it != collisionFuncsMap.end())
    {
        if (it->second(c1, c2)) {
            c1->detection(c2->actor);
            c2->detection(c1->actor);
        }
    }
}

/*
    CollisionCircle
*/

CollisionCircle::CollisionCircle()
{
    SetType(CollisionType::CIRCLE);
}

const CIRCLE& CollisionCircle::GetCircle()
{
    return CIRCLE{GetPos().x, GetPos().y, this->radius};
}

void CollisionCircle::SetRadius(int radius)
{
    this->radius = radius;
}

void CollisionCircle::Render(HDC hdc)
{
     RenderEllipseAtCenter(hdc, GetPos().x, GetPos().y, radius, radius);
}
