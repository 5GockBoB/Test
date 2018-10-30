#include "stdafx.h"
#include "TestProjection.h"
#include "./Objects/GameAnimModel.h"
#include "./Objects/MeshPlane.h"
#include "./Lights/Projection.h"

TestProjection::TestProjection(ExecuteValues * values)
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

	projection = new Projection(values);
	projection->Add(hanjo);
	projection->Add(plane);
}

TestProjection::~TestProjection()
{
	SAFE_DELETE(projection);
	SAFE_DELETE(plane);
	SAFE_DELETE(hanjo);
}

void TestProjection::Update()
{
	projection->Update();
	//plane->Update();
	//hanjo->Update();
}

void TestProjection::PreRender()
{
}

void TestProjection::Render()
{
	projection->Render();
	//plane->Render();
	//hanjo->Render();
}

void TestProjection::PostRender()
{	
}
