#pragma once

enum Shape
{
	eRay,
	eSphere,
	eCube,
	eCapsule,
	eCount
};

class Collider
{
public:
	Collider(class GameModel* model);
	virtual ~Collider();

	virtual void Update();
	virtual void Render();

	void Scale(D3DXVECTOR3& scale);
	D3DXVECTOR3 Scale();
	void Translation(D3DXVECTOR3& translation);
	D3DXVECTOR3 Translation();
	void Rotation(D3DXVECTOR3& rotationDegree);
	D3DXVECTOR3 Rotation();

	D3DXMATRIX Transform();

	void GetBoundingBox(D3DXVECTOR3* boundingBox);
	
	void SetRadius(float radius) { this->radius = radius; }
	float GetRadius() { return radius; }

	Shape GetType() { return shape; }
	void SetActive(bool bOn = true) { this->bOn = bOn; }
	void SetGizmoRender(bool bOn = true) { gizmo.SetActive(bOn); }

protected:
	//로컬 트랜스폼
	D3DXVECTOR3 scale;
	D3DXVECTOR3 translation;
	D3DXVECTOR3 rotation;

protected:
	class GameModel* model;
	D3DXMATRIX* worldTransform;

	Shape shape;
	D3DXVECTOR3 boundingBox[8];
	float radius;
	bool bOn;

	Gizmo gizmo;
};