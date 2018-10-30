#pragma once
#include "Collider.h"

class CubeCollider : public Collider
{
public:
	CubeCollider(class GameModel* model);
	~CubeCollider();

	void GetAABBBox(D3DXVECTOR3* aabbBox);
};