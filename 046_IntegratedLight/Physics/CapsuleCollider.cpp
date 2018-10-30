#include "stdafx.h"
#include "CapsuleCollider.h"
#include "./Objects/GameModel.h"

CapsuleCollider::CapsuleCollider(GameModel * model)
	: Collider(model)
{
	shape = eCapsule;

	//반지름 설정
	D3DXVECTOR3 center((boundingBox[7].x + boundingBox[0].x) / 2.0f, (boundingBox[7].y + boundingBox[0].y) / 2.0f, (boundingBox[7].z + boundingBox[0].z) / 2.0f);
	
	capsuleRadius = max((boundingBox[7].x - boundingBox[0].x) / 2.0f, (boundingBox[7].z + boundingBox[0].z) / 2.0f);
	radius = max((boundingBox[7].x - boundingBox[0].x) / 2.0f, capsuleRadius);

	//길이 설정
	float point1Y = (boundingBox[7].y - capsuleRadius) > center.y ? boundingBox[7].y - capsuleRadius : center.y;
	float point2Y = (boundingBox[0].y + capsuleRadius) < center.y ? boundingBox[0].y + capsuleRadius : center.y;

	point1 = D3DXVECTOR3(center.x, point1Y, center.z);
	point2 = D3DXVECTOR3(center.x, point2Y, center.z);

	
	//기즈모
	gizmo.DrawGizmoCapsule("CapsuleCollider", model->WorldLink(), point1, point2, capsuleRadius);
}

CapsuleCollider::CapsuleCollider(D3DXMATRIX * transform, float radius, float length)
	: Collider(NULL), capsuleRadius(radius)
{
	shape = eCapsule;

	worldTransform = transform;

	if (length < 0) length = 0;
	point1 = D3DXVECTOR3(0, length / 2.0f, 0);
	point2 = D3DXVECTOR3(0, -length / 2.0f, 0);

	this->radius = length + radius;

	//기즈모
	gizmo.DrawGizmoCapsule("CapsuleCollider", transform, point1, point2, capsuleRadius);
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::SetCapsule(float radius, float length)
{
	if (length < 0) length = 0;
	this->capsuleRadius = radius;
	this->radius = length + radius;

	if (model == NULL)
	{
		point1.y = length / 2.0f;
		point2.y = -length / 2.0f;
	}
	else
	{
		float centerY = (boundingBox[7].y + boundingBox[0].y) / 2;
		point1.y = centerY + length;
		point2.y = centerY - length;
	}
	//기즈모
	gizmo.DrawGizmoCapsule("CapsuleCollider", NULL, point1, point2, capsuleRadius);
}

void CapsuleCollider::GetCapsule(D3DXVECTOR3 * point1, D3DXVECTOR3 * point2, float * radius)
{
	D3DXMATRIX world;
	if (model == NULL)
	{
		world = Transform() * *worldTransform;
		D3DXVECTOR3 scale, transform;
		D3DXQUATERNION rotation;
		D3DXMatrixDecompose(&scale, &rotation, &transform, worldTransform);
		*radius = this->capsuleRadius * Scale().x * scale.x;
	}
	else
	{
		world = Transform() * model->Transformed();
		*radius = this->capsuleRadius * Scale().x * model->Scale().x;
	}
	D3DXVec3TransformCoord(point1, &this->point1, &world);
	D3DXVec3TransformCoord(point2, &this->point2, &world);

}