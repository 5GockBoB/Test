#include "stdafx.h"
#include "MeshSphere.h"

MeshSphere::MeshSphere()
	: GameModel(Materials + L"Sphere/", L"Sphere.material", Models + L"Sphere/", L"Sphere.mesh")
{
	SetShader(Shaders + L"034_NormalMap.hlsl");
}

MeshSphere::~MeshSphere()
{
	SAFE_DELETE(shader)
}

void MeshSphere::Update()
{
	__super::Update();
}

void MeshSphere::Render()
{
	__super::Render();
}
