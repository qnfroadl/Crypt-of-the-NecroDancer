#pragma once

#include "IRendableTileActor.h"
#include "Camera.h"

/*
* 
	IRenableTileActor를 벡터로 갖고 있을거고.
	IRenableTileActor를 추가 할 수 있는 함수도 필요 하겠지?
	
	TileActorRenderer 클래스에서 할 일은. 
	 - 카메라에 보이는 곳의 객체들만 그릴건데. 카메라에 보이는 것보다 가로100, 세로100만큼 더 넓게 그릴거야 (active 변경 해줘야겠지)
	 - 라이팅 렌더링 (플레이어 이동 or 벽돌 파괴 이벤트를 받으면 라이팅 계산 할거고) 
	     -> 카메라에 보이는 객체들중 라이팅 컴포넌트를 가진 객체를 찾는다.
	     -> 라이팅 컴포넌트


	- 매 프레임 y축 기반으로 렌더링 순서를 정렬.

*/

class LightingRenderer;
class TileActorRenderer : public GameActor
{

private:
	std::vector<std::shared_ptr<IRendableTileActor>> rendableActors;

public:

	HRESULT Init() override;
	void Render(HDC hdc) override;

	/*
		PositionManager (PlayerManager, MonsterManager + Item까지 포함되어 있는 포지션 매니저)
		Tilemap,


		LightingRenderer에서 어떻게 할 지는 모르겠다.
	*/
	void AddRendableSource(std::shared_ptr<IRendableTileActor> source);
	void OnPlayerMoved(); // 또는 OnLightingUpdateTrigger()
	void OnTileDestroyed();


};

