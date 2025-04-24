#pragma once
#include "config.h"
#include "TileActor.h"
#include "IRendableTileActor.h"

class Image;
class Torch : public TileActor, public IRendableTileActor, public enable_shared_from_this<Torch>
{
private:
	Image* torchImage;
	float elapsedTime;
	int curFrame;
public:
	Torch() { SetType(ActorType::TORCH); }
	~Torch() {}
	HRESULT Init();
	HRESULT Init(FPOINT _pos, POINT _index);
	void Update();
	void Render(HDC hdc, bool useCamera = true);
	virtual vector<shared_ptr<TileActor>> GetRendableTileActors() override;
};

