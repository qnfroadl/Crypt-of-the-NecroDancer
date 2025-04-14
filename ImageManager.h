#pragma once

#include "config.h"
#include "Singleton.h"
#include "Image.h"
#include <map>

/*
	배열 : 인덱스 -> 데이터
	stl::vector : 인덱스 -> 데이터
	std::map : 키 -> 데이터

	레드-블랙 트리 구현 : 삽입, 삭제 ,검색 O(logN) 시간복잡도
*/

enum class EImageKey
{
	Player,
	PickupBomb, PickupPower,
	PlayerAttackDefault,
	PlayerAttackMissile,
	CanonShot,

	// 캐논 각도별 이미지
	Canon_210, Canon_250, Canon_270, Canon_290, Canon_330,

	// Boss_Ship
	ShipBase, ShipDestroyed,
	// Boss_Bird
	BirdAppear1, BirdAppear2, BirdBase, BirdDestroyLeft, BirdDestroyRight, BirdDestroy, BirdPropeller,
	PlayerBomb,

	ExplosionSmall, ExplosionNormal, ExplosionBig, ShotImpact,

	// Enemy
	FlyingEnemy, MidBossBasic, MidBossUpgrade, BeeCopter,

	// Missile
	MidBossStar, MidBossPyramid,

	//Background
	BG_SeaBottom, BG_SeaLoop,
};

class Image;
class ImageManager : public Singleton<ImageManager>
{

private:
	map<string, Image*> mapImages;
	map<EImageKey, Image*> mapImages2;

public:
	void Init();
	void Release();

	Image* AddImage(const string& key, const wchar_t* filePath, int width, int height,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);

	Image* AddImage(const string& key, const wchar_t* filePath, int width, int height,
		int maxFrameX, int maxFrameY, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	void DeleteImage(const string& key);

	Image* FindImage(const string& key);

	Image* AddImage(EImageKey key, const wchar_t* filePath, int width, int height,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);
	
	Image* AddImage(EImageKey key, const wchar_t* filePath, int width, int height,
		int maxFrameX, int maxFrameY, bool isTransparent = FALSE, COLORREF transColor = FALSE);
	
	void DeleteImage(EImageKey key);
	
	Image* FindImage(EImageKey key);


};
