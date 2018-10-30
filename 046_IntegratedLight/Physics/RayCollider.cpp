#include "stdafx.h"
#include "RayCollider.h"
#include "./Objects/GameModel.h"

RayCollider::RayCollider(GameModel * model)
	: Collider(model)
	, direction(0, 1, 0)
{
	shape = eRay;
}

RayCollider::~RayCollider()
{
}

void RayCollider::SetDirection(D3DXVECTOR3 & direction)
{
	D3DXVec3Normalize(&this->direction, &direction);
}

D3DXVECTOR3 RayCollider::GetDirection()
{
	return direction;
}
