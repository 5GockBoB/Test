#include "stdafx.h"
#include "TestScattering.h"
#include "./Landscape/Sky.h"
#include "./Landscape/TerrainRender.h"

TestScattering::TestScattering(ExecuteValues * values)
	: Execute(values)
{
	sky = new Sky(values);
	terrain = new TerrainRender();
}

TestScattering::~TestScattering()
{
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
}

void TestScattering::Update()
{

	sky->Update();
	terrain->Update();
}

void TestScattering::PreRender()
{
	sky->PreRender();
}

void TestScattering::Render()
{
	sky->Render();
	terrain->Render();
}

void TestScattering::PostRender()
{	
}