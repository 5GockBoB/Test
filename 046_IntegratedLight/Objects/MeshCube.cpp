#include "stdafx.h"
#include "MeshCube.h"

MeshCube::MeshCube()
	: GameModel(Materials + L"Cube/", L"Cube.material", Models + L"Cube/", L"Cube.mesh")
{
	SetShader(Shaders + L"016_NormalMap.hlsl");
}

MeshCube::~MeshCube()
{
	SAFE_DELETE(shader)
}

void MeshCube::Update()
{
	__super::Update();
}

void MeshCube::Render()
{
	__super::Render();
}
