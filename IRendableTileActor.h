#pragma once

#include "TileActor.h"

class IRendableTileActor
{
public:
	virtual vector<shared_ptr<TileActor>> GetRendableTileActors() = 0;

};