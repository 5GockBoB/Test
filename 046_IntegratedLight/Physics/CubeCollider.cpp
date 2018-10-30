#include "stdafx.h"
#include "CubeCollider.h"
#include "./Objects/GameModel.h"

CubeCollider::CubeCollider(GameModel * model)
	: Collider(model)
{
	shape = eCube;
}

CubeCollider::~CubeCollider()
{
}

void CubeCollider::GetAABBBox(D3DXVECTOR3 * aabbBox)
{
	aabbBox[0] = D3DXVECTOR3(10000, 10000, 10000);
	aabbBox[1] = D3DXVECTOR3(-10000, -10000, -10000);

	D3DXVECTOR3 worldBox[8];
	D3DXMATRIX world = model->Transformed();

	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&worldBox[i], &boundingBox[i], &world);

		if (aabbBox[0].x > worldBox[i].x)
			aabbBox[0].x = worldBox[i].x;
		else if (aabbBox[1].x < worldBox[i].x)
			aabbBox[1].x = worldBox[i].x;

		if (aabbBox[0].y > worldBox[i].y)
			aabbBox[0].y = worldBox[i].y;
		else if (aabbBox[1].y < worldBox[i].y)
			aabbBox[1].y = worldBox[i].y;

		if (aabbBox[0].z > worldBox[i].z)
			aabbBox[0].z = worldBox[i].z;
		else if (aabbBox[1].z < worldBox[i].z)
			aabbBox[1].z = worldBox[i].z;
	}

	D3DXVECTOR3 distance = aabbBox[1] - aabbBox[0];

	radius = distance.x - distance.x;
	radius = (radius > distance.y) ? radius : distance.y;
	radius = (radius > distance.z) ? radius : distance.z;
}
