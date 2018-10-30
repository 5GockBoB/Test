#include "stdafx.h"
#include "TestPBR.h"
#include "./Objects/MeshPlane.h"
#include "./Objects/MeshSphere.h"
#include "./Objects/MeshCube.h"
#include "./Objects/MeshBunny.h"

TestPBR::TestPBR(ExecuteValues * values)
	: Execute(values)
{
	shader = new Shader(Shaders + L"200_PBR.hlsl");
	plane = new MeshPlane();
	plane->Scale(10, 1, 10);
	plane->SetShader(shader);
	plane->SetDiffuse(1, 0, 0);

	cube = new MeshCube();
	cube->Scale(3, 3, 3);
	cube->Position(-10, 1.5f, 0);
	cube->SetShader(shader);
	cube->SetDiffuse(0, 1, 0);
	cube->SetDiffuseMap(Textures + L"Wall.png");
	cube->SetNormalMap(Textures + L"Wall_normal.png");

	sphere = new MeshSphere();
	sphere->Scale(3, 3, 3);
	sphere->Position(10, 1.5f, 0);
	sphere->SetShader(shader);
	sphere->SetDiffuse(0, 0, 1);
	sphere->SetDiffuseMap(Textures + L"Floor.png");
	sphere->SetNormalMap(Textures + L"Floor_normal.png");

	sphere2 = new MeshSphere();
	sphere2->Scale(3, 3, 3);
	sphere2->Position(10, 1.5f, 10);
	sphere2->SetShader(shader);
	sphere2->SetDiffuse(1, 1, 1);
	sphere2->SetDiffuseMap(Textures + L"White.png");
	
	bunny = new MeshBunny();
	bunny->Scale(0.02f, 0.02f, 0.02f);
	bunny->Position(0, 5, 0);
	bunny->SetShader(shader);
	bunny->SetDiffuse(1, 1, 1);
	bunny->SetDiffuseMap(Textures + L"White.png");


	lightingPBRBuffer = new LightingPBRBuffer();
}

TestPBR::~TestPBR()
{
	SAFE_DELETE(plane);
	SAFE_DELETE(cube);
	SAFE_DELETE(sphere);
	SAFE_DELETE(sphere2);
	SAFE_DELETE(bunny);

	SAFE_DELETE(shader);
	SAFE_DELETE(lightingPBRBuffer);
}

void TestPBR::Update()
{
	plane->Update();
	cube->Update();
	sphere->Update();
	sphere2->Update();
	bunny->Update();
}

void TestPBR::PreRender()
{
}

void TestPBR::Render()
{
	ImGui::SliderFloat("DiffuseRatio", (float *)&lightingPBRBuffer->Data.DiffuseRatio, 0, 1);
	ImGui::SliderFloat("Roughness", (float *)&lightingPBRBuffer->Data.Roughness, 0, 1);
	ImGui::SliderFloat("Fresnel", (float *)&lightingPBRBuffer->Data.Fresnel, 0, 1);

	lightingPBRBuffer->SetPSBuffer(10);

	plane->Render();
	cube->Render();
	sphere->Render();
	sphere2->Render();
	bunny->Render();
}

void TestPBR::PostRender()
{	
}