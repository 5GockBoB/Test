#include "stdafx.h"
#include "Projection.h"
#include "./Viewer/Fixity.h"
#include "./Objects/GameModel.h"

Projection::Projection(ExecuteValues * values)
	: values(values)
{
	shader = new Shader(Shaders + L"042_Projection.hlsl");

	fixity = new Fixity();
	fixity->Position(20, 0, 30);
	fixity->RotationDegree(90, 0);

	//Perspective
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		perspective = new Perspective(1, 1, Math::PI * 0.5f, 0, 100);
	}

	vpBuffer = new ViewProjectionBuffer();
}

Projection::~Projection()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(vpBuffer);
	SAFE_DELETE(perspective);
	SAFE_DELETE(fixity);
}

void Projection::Add(GameModel * obj)
{
	obj->SetShader(Shaders + L"042_Projection.hlsl");
	obj->SetNormalMap(Textures + L"Bricks.png");
	objs.push_back(obj);
}

void Projection::Update()
{
	for (GameModel* obj : objs)
	{
		obj->Update();
	}

	D3DXMATRIX V, P;
	fixity->Matrix(&V);
	vpBuffer->SetView(V);
	perspective->GetMatrix(&P);
	vpBuffer->SetProjection(P);
}

void Projection::PreRender()
{
}

void Projection::Render()
{
	D3DXVECTOR3 position;
	fixity->Position(&position);
	ImGui::SliderFloat3("Proj Position", (float*)&position, -100, 100);
	fixity->Position(position.x, position.y, position.z);

	D3DXVECTOR2 rotation;
	fixity->RotationDegree(&rotation);
	ImGui::SliderFloat2("Proj Rotation", (float*)&rotation, -180, 180);
	fixity->RotationDegree(rotation.x, rotation.y);

	vpBuffer->SetVSBuffer(10);

	for (GameModel* obj : objs)
	{
		obj->Render();
	}
}
