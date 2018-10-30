#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(class GameModel* model);
	CapsuleCollider(D3DXMATRIX* transform, float radius, float length);
	~CapsuleCollider();

	void SetCapsule(float radius, float length);
	void GetCapsule(D3DXVECTOR3* point1, D3DXVECTOR3* point2, float* radius);

private:
	D3DXVECTOR3 point1, point2;
	float capsuleRadius;
};