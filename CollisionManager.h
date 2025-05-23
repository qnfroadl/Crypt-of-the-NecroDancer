#pragma once
#include "config.h"
#include "GameObject.h"
#include <unordered_map>
#include <map>
#include <functional>
#include <unordered_set>
#include <vector>
#include "Singleton.h"
#include "ObjectPool.h"

class GameActor;
enum class CollisionType
{
	RECT, CIRCLE
};

enum class CollisionLayer : uint8_t
{
	Player = 1,				// 00000001
	PlayerAttack = 1 << 1,	// 00000010
	Enemy = 1 << 2,			// 00000100
	EnemyAttack = 1 << 3,	// 00001000
	Item = 1 << 4,			// 00010000
};

class Collision : public GameObject
{

private:
	GameActor* actor;
	FPOINT relativePos;

	CollisionLayer layer;
	CollisionType type;
	std::function<void(GameActor* obj)> detection;

public:
	virtual ~Collision(){}

	void Bind(std::function<void(GameActor* obj)> func)		// Bind(std::bind(&클래스명:메서드명, 객체포인터)) 또는 람다 사용.
	{
		detection = func;
	}
	
	void SetGameActor(GameActor* obj);
	GameActor* GetGameActor();

	CollisionType GetType();
	void SetType(CollisionType type);

	void SetRelativePos(const FPOINT& pos);
	inline const FPOINT& GetPos();
	friend class CollisionManager;
	
};

class CollisionRect : public Collision
{
private:
	int width;
	int height;
	
public:
	CollisionRect();

	const RECT& GetRect();
	void SetWidth(int _width);
	void SetHeight(int _height);
	void Render(HDC hdc) override;
};

class CollisionCircle : public Collision
{
private:
	int radius;

public:
	CollisionCircle();

	const CIRCLE& GetCircle();
	void SetRadius(int radius);

	void Render(HDC hdc) override;

};


// CollisionManager
using CollisionFunc = std::function<bool(Collision*, Collision*)>;
class CollisionManager : public Singleton<CollisionManager>, public GameObject
{
private:
	int checkCount;
	ObjectPool<CollisionRect>* rtPool;
	ObjectPool<CollisionCircle>* ccPool;

	unordered_map<CollisionLayer, unordered_set<Collision*>>* layerCollisionMap;
	unordered_map<CollisionLayer, uint8_t> layerMaskMap;

	map<std::pair<CollisionType, CollisionType>, CollisionFunc> collisionFuncsMap;

	HBRUSH brush;
	HPEN pen;

	void CollisionDetect(const unordered_set<Collision*>& setColl1, const unordered_set<Collision*>& setColl2);
	void Detect(Collision* c1, Collision* c2);

public:
	CollisionRect* CreateCollisionRect(CollisionLayer layer, GameActor* obj, int _width, int _height, const FPOINT& _relativePos = {0,0});
	CollisionCircle* CreateCollisionCircle(CollisionLayer layer, GameActor* obj, int radius, const FPOINT& _relativePos = { 0,0 });

	void ReleaseCollision(Collision* coll);

	HRESULT Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
	void CollisionDetect();

	CollisionManager& operator =(CollisionManager&) = delete;

	int GetCheckCount();
	int GetCollisionCount();
	int GetActivecollisionCount();
};