#include "stdafx.h"
#include "MeshPlane.h"

MeshPlane::MeshPlane()
	: GameModel(Models + L"Plane/", L"Plane.material", Models + L"Plane/", L"Plane.mesh")
{
	SetShader(Shaders + L"028_SpotLight.hlsl");
}

MeshPlane::~MeshPlane()
{
	SAFE_DELETE(shader)
}

void MeshPlane::Update()
{
	__super::Update();
}

void MeshPlane::Render()
{
	__super::Render();
}
