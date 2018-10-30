#include "stdafx.h"
#include "TestShadow.h"
#include "./Objects/GameAnimModel.h"
#include "./Objects/MeshPlane.h"
#include "./Lights/Shadow.h"

TestShadow::TestShadow(ExecuteValues * values)
	: Execute(values)
{
	hanjo = new GameAnimModel(Models + L"Kachujin/", L"Kachujin.material", Models + L"Kachujin/", L"Kachujin.mesh");
	hanjo->AddClip(Models + L"Kachujin/Samba_Dance.anim");
	hanjo->Play(0, true, 0.0f, 10.0f);
	hanjo->SetShader(Shaders + L"035_Animation.hlsl");
	hanjo->Scale(0.025f, 0.025f, 0.025f);

	plane = new MeshPlane();
	plane->SetShader(Shaders + L"028_SpotLight.hlsl");
	plane->Scale(10, 1, 10);
	plane->SetDiffuse(1, 1, 1, 1);
	plane->SetDiffuseMap(Textures + L"White.png");

	shadow = new Shadow(values);
	shadow->Add(hanjo);
	shadow->Add(plane);
}

TestShadow::~TestShadow()
{
	SAFE_DELETE(shadow);
	SAFE_DELETE(plane);
	SAFE_DELETE(hanjo);
}

void TestShadow::Update()
{
	shadow->Update();
	//plane->Update();
	//hanjo->Update();
}

void TestShadow::PreRender()
{
	shadow->PreRender();
}

void TestShadow::Render()
{
	shadow->Render();
	//plane->Render();
	//hanjo->Render();
}

void TestShadow::PostRender()
{	
}
