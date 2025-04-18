#include "Monster.h"
#include "ImageManager.h"
#include "Camera.h"
#include "TimerManager.h"

void Monster::Init(MONSTERTYPE p)
{
	imageInfo = FindImageInfo(p);
	image = ImageManager::GetInstance()->AddImage(imageInfo.keyName, imageInfo.imagePath, imageInfo.width*2,imageInfo.height*2,
		imageInfo.imageFrameX, imageInfo.ImageFrameY,true,RGB(255,0,255));
	state = MonsterState::IDLE;
	light = 0;
	moveDelay = 3;
	beatCount = 0;
	
	animationFrame = 0;
	elapsedFrame = 0;
	elapsedTime = 0;
	
	player = nullptr;
}

void Monster::Release()
{

}

void Monster::Update()
{
	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (elapsedTime > 0.1f)
	{
		animationFrame++;
		if (animationFrame >= image->GetMaxFrameX())
		{
			animationFrame = 0;
		}
		elapsedTime = 0.0f;
	}
		
}

void Monster::Render(HDC hdc)
{
	float screenX = Camera::GetInstance()->GetPos().x;
	float screenY = Camera::GetInstance()->GetPos().y;
	image->FrameRender(hdc,200, 200, animationFrame, 0);
}

void Monster::Trace()
{

}

void Monster::Dead()
{

}

void Monster::OnBeat()
{

}

MonsterImageInfo Monster::FindImageInfo(MONSTERTYPE _m)
{
	auto path = MonsterInfoTable.find(_m);
	if (path != MonsterInfoTable.end())
		return path->second;
}

Monster::Monster()
{
}

Monster::~Monster()
{
}
