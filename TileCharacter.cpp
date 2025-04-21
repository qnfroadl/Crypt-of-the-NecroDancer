#include "TileCharacter.h"

#include "TimerManager.h"

void TileCharacter::SetJumpData(int dx, int dy)
{
	jumpData.animCurtime = 0 ;
	jumpData.sx = GetPos().x;
	jumpData.sy = GetPos().y;
	jumpData.dx = dx;
	jumpData.dy = dy;

}

bool TileCharacter::JumpAnim()
{
	bool bRet = false;

	// 점프 애니메이션 처리
	// jumpData.sx, jumpData.sy : 시작 위치
	// jumpData.ex, jumpData.ey : 도착 위치
	// jumpData.height : 점프 높이
	float dt = TimerManager::GetInstance()->GetDeltaTime();
	jumpData.animCurtime += dt;

	float jupmY = sinf(3.141592f * dt) * 200;	//jump power 200
	if (jumpData.animCurtime <= jumpData.totalAnimTime / 2)
	{
		jumpData.height += jupmY;
	}
	else
	{
		jumpData.height -= jupmY;
	}

	jumpData.sx += (jumpData.dx - jumpData.sx) * dt * 20;
	jumpData.sy += (jumpData.dy - jumpData.sy) * dt * 20;


	// 점프 종료.
	if (jumpData.totalAnimTime <= jumpData.animCurtime)
	{
		jumpData.sx = jumpData.dx;
		jumpData.sy = jumpData.dy;

		jumpData.height = 0;
		jumpData.animCurtime = 0;
		bRet = true;
	}

	SetPos(jumpData.sx, jumpData.sy);

	return bRet;



}

TileCharacter::TileCharacter()
{
	gold.Set(0);
}
