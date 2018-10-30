#pragma once
#include "Collider.h"

class RayCollider : public Collider
{
public:
	RayCollider(class GameModel* model);
	~RayCollider();

	void SetDirection(D3DXVECTOR3& direction);
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR3 direction;
};