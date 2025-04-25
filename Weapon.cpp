#include "Weapon.h"
#include "WeaponMaterial.h"

#include "Player.h"
#include "Camera.h"
#include "GameActor.h"
#include "Monster.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "EventManager.h"

void Weapon::InitResources()
{
	// 이렇게 로드하지말고, 필요 할 때만 로드하게 만들었으면 좋았을걸.
	const float weaponScale = 2.0;
	const float swipeScale = 2.0f;
#pragma region WeaponResources
	ImageManager::GetInstance()->AddImage("weapon_broadsword_blood",
	TEXT("Image/Player/weapon/weapon_broadsword_blood.bmp"), 24 * weaponScale,48 * weaponScale, 1, 2, true, RGB(255,0,255));

	ImageManager::GetInstance()->AddImage("weapon_broadsword_glass",
		TEXT("Image/Player/weapon/weapon_broadsword_glass.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_broadsword_golden",
		TEXT("Image/Player/weapon/weapon_broadsword_golden.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_broadsword_normal",
		TEXT("Image/Player/weapon/weapon_broadsword_normal.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_broadsword_obsidian",
		TEXT("Image/Player/weapon/weapon_broadsword_obsidian.bmp"), 24 * weaponScale, 48 * weaponScale, 3, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_broadsword_titanium",
		TEXT("Image/Player/weapon/weapon_broadsword_titanium.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_blood",
		TEXT("Image/Player/weapon/weapon_dagger_blood.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_glass",
		TEXT("Image/Player/weapon/weapon_dagger_glass.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_golden",
		TEXT("Image/Player/weapon/weapon_dagger_golden.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_normal",
		TEXT("Image/Player/weapon/weapon_dagger_normal.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_obsidian",
		TEXT("Image/Player/weapon/weapon_dagger_obsidian.bmp"), 24 * weaponScale, 48 * weaponScale, 3, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_dagger_titanium",
		TEXT("Image/Player/weapon/weapon_dagger_titanium.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));

	ImageManager::GetInstance()->AddImage("weapon_glass",
		TEXT("Image/Player/weapon/weapon_glass.bmp"), 24 * weaponScale, 48 * weaponScale, 1, 2, true, RGB(255, 0, 255));
#pragma endregion

#pragma region SwipeResources
	ImageManager::GetInstance()->AddImage("swipe_broadsword_blood",
		TEXT("Image/Player/swipe/swipe_broadsword_blood.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_broadsword_glass",
		TEXT("Image/Player/swipe/swipe_broadsword_glass.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_broadsword_gold",
		TEXT("Image/Player/swipe/swipe_broadsword_gold.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_broadsword_normal",
		TEXT("Image/Player/swipe/swipe_broadsword_normal.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_broadsword_obsidian",
		TEXT("Image/Player/swipe/swipe_broadsword_obsidian.bmp"), 72 * swipeScale, 216 * swipeScale, 3, 3, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_broadsword_titanium",
		TEXT("Image/Player/swipe/swipe_broadsword_titanium.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_blood",
		TEXT("Image/Player/swipe/swipe_dagger_blood.bmp"), 72 * swipeScale, 24 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_glass",
		TEXT("Image/Player/swipe/swipe_dagger_glass.bmp"), 72 * swipeScale, 24 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_golden",
		TEXT("Image/Player/swipe/swipe_dagger_golden.bmp"), 72 * swipeScale, 24 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_normal",
		TEXT("Image/Player/swipe/swipe_dagger_normal.bmp"), 72 * swipeScale, 24 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_obsidian",
		TEXT("Image/Player/swipe/swipe_dagger_obsidian.bmp"), 72 * swipeScale, 72 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

	ImageManager::GetInstance()->AddImage("swipe_dagger_titanium",
		TEXT("Image/Player/swipe/swipe_dagger_titanium.bmp"), 72 * swipeScale, 24 * swipeScale, 3, 1, true, RGB(255, 0, 255), true);

#pragma endregion


}

void Weapon::InitRange()
{
	range.clear();
	switch (weaponType)
	{
	case WeaponType::DAGGER:
		range.push_back({ 1, 0 });
		break;
	case WeaponType::BROADSWORD:
		range.push_back({ 1, -1 });
		range.push_back({ 1, 0 });
		range.push_back({ 1, 1 });
		break;
	default:
		break;
	}
}
void Weapon::InitImage()
{
	string swipeKey = string("swipe_") + GetWeaponTypeName() + "_" + material->GetMaterialName();

	swipeImage = ImageManager::GetInstance()->FindImage(swipeKey);
	dropedImage = ImageManager::GetInstance()->FindImage(string("weapon_") + GetWeaponTypeName() + "_" + material->GetMaterialName());
}
string Weapon::GetWeaponTypeName()
{
	string name;
	switch (weaponType)
	{
	case WeaponType::DAGGER:
		name = "dagger";
		break;
	case WeaponType::BROADSWORD:
		name = "broadsword";
		break;
	}

	return name;
}

Weapon::Weapon()
	: damageType(DamageType::NORMAL), weaponType(WeaponType::DAGGER), bSwipe(false), elapsedTime(0), curFrame(0)
{
	SetItemType(ItemType::WEAPON);
	InitResources();
}

HRESULT Weapon::Init()
{
	elapsedTime = 0.f;
	curFrame = 0;
	weaponType = WeaponType::DAGGER;
	material = std::make_shared<WeaponMaterial>();
	SetMaterial(WeaponMaterialType::NORMAL);

	InitImage();
	InitRange();
	return S_OK;
}

void Weapon::Release()
{

}

void Weapon::Update()
{   
	
}

void Weapon::Render(HDC hdc)
{
	// 무기 이미지 렌더링
	// 카메라 위치에 따라 무기 위치 조정
	// 주인이 없을때 => 바닥에 있어야 함.
	if (false == HasOwner() && IsActive())
	{
		if (sightState == SightState::INVISIBLE)
		{
			return;
		}

		FPOINT screenPos = Camera::GetInstance()->GetScreenPos(GetPos());

		dropedImage->FrameRender(hdc, screenPos.x, screenPos.y, 0, sightState == SightState::VISIBLE ? 0 : 1, false, true);
	}
	else if (bSwipe)
	{
		elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
		if (0.1 <= elapsedTime)
		{
			elapsedTime = 0;
			curFrame++;
		}

		if (curFrame < swipeImage->GetMaxFrameX())
		{
			FPOINT screenPos = Camera::GetInstance()->GetScreenPos(swipePos);
			swipeImage->FrameRender(hdc, screenPos.x, screenPos.y, curFrame, 0, 4, 4, false, true, 1.f, swipeAngle);
		}
		else {
			bSwipe = false;
			curFrame = 0;
		}
	}
}

void Weapon::SetWeaponType(DamageType damageType, WeaponType weaponType)
{
	this->damageType = damageType;
	this->weaponType = weaponType;
	InitImage();
	InitRange();
}

void Weapon::SetMaterial(WeaponMaterialType type)
{
	material->SetType(type);
	InitImage();
	InitRange();
}

void Weapon::Interact(GameActor* actor)
{
	if (actor->GetType() == ActorType::PLAYER)
	{
		SetOwner(static_cast<Player*>(actor));
		GetOwner()->AddWeapon(shared_from_this());
	}
	else if (actor->GetType() == ActorType::MONSTER)
	{
		Attack(static_cast<Monster*>(actor));
	}
}


void Weapon::Attack(Monster* monster)
{
	monster->TakeDamage(material->GetDamage());
	
	if (monster->IsDead())
	{
		EventManager::GetInstance()->AddEvent(EventType::COMBOSTART,nullptr, false);
	}

}

void Weapon::Swipe(FPOINT pos, Direction dir)
{
	bSwipe = true;	// 상태 변경.
	swipePos = pos;
	switch (dir)
	{
		case Direction::RIGHT:
			swipeAngle = 0;
			break;
		case Direction::LEFT:
			swipeAngle = 180;
			break;
		case Direction::UP:
			swipeAngle = 270;
			break;
		case Direction::DOWN:
			swipeAngle = 90;
			break;
	}
	
}

vector<POINT> Weapon::GetRange()
{
	return range;
}
