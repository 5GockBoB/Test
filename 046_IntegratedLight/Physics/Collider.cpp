#include "stdafx.h"
#include "Collider.h"
#include "./Objects/GameModel.h"

Collider::Collider(GameModel * model)
	: model(model), worldTransform(NULL), bOn(true)
	, scale(1, 1, 1), translation(0, 0, 0), rotation(0, 0, 0)
{
	if (model != NULL)
		model->BoundingBox(boundingBox);
}

Collider::~Collider()
{
}

void Collider::Update()
{
	gizmo.Update(Transform());
}

void Collider::Render()
{
	gizmo.Render();
}

void Collider::Scale(D3DXVECTOR3 & scale)
{
	this->scale = scale;
}

D3DXVECTOR3 Collider::Scale()
{
	return scale;
}

void Collider::Translation(D3DXVECTOR3 & translation)
{
	this->translation = translation;
}

D3DXVECTOR3 Collider::Translation()
{
	return translation;
}

void Collider::Rotation(D3DXVECTOR3 & rotationDegree)
{
	this->rotation = rotationDegree;
}

D3DXVECTOR3 Collider::Rotation()
{
	return rotation;
}

D3DXMATRIX Collider::Transform()
{
	D3DXMATRIX S, T, R;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, translation.x, translation.y, translation.z);
	D3DXMatrixRotationYawPitchRoll(&R, Math::ToRadian(rotation.y), Math::ToRadian(rotation.x), Math::ToRadian(rotation.z));
	return S * T * R;
}

void Collider::GetBoundingBox(D3DXVECTOR3 * boundingBox)
{
	if (model == NULL) return;
	for (UINT i = 0; i < 8; i++)
		boundingBox[i] = this->boundingBox[i];
}