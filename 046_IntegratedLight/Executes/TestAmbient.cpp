#include "stdafx.h"
#include "TestAmbient.h"
#include "./Objects/MeshPlane.h"
#include "./Objects/MeshSphere.h"
#include "./Objects/MeshCube.h"
#include "./Objects/MeshBunny.h"

TestAmbient::TestAmbient(ExecuteValues * values)
	: Execute(values)
{
	shader = new Shader(Shaders + L"046_Lighting.hlsl");
	
	plane = new MeshPlane();
	plane->Scale(10, 1, 10);
	plane->SetShader(shader);
	plane->SetDiffuse(1, 1, 1);

	cube = new MeshCube();
	cube->Scale(3, 3, 3);
	cube->Position(-10, 1.5f, 0);
	cube->SetShader(shader);
	cube->SetDiffuse(0, 1, 0);

	sphere = new MeshSphere();
	sphere->Scale(3, 3, 3);
	sphere->Position(10, 1.5f, 0);
	sphere->SetShader(shader);
	sphere->SetDiffuse(0, 0, 1);

	sphere2 = new MeshSphere();
	sphere2->Scale(3, 3, 3);
	sphere2->Position(10, 1.5f, 10);
	sphere2->SetShader(shader);
	sphere2->SetDiffuse(1, 1, 1);
	
	bunny = new MeshBunny();
	bunny->Scale(0.02f, 0.02f, 0.02f);
	bunny->Position(0, 5, 0);
	bunny->SetShader(shader);
	bunny->SetDiffuse(1, 1, 1);
	bunny->SetDiffuseMap(Textures + L"White.png");

	//CreateLightBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		//DEFAULT gpu, cpu에서 읽고 쓰기 가능
		//IMMUTABLE gpu에서 읽기 가능
		//DYNAMIC gpu에서 읽기, cpu에서 쓰기 가능 ->updatesubresource 불가능 map으로만 가능
		//STAGE gpu에서 cpu로 값 전달이 필요할때
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.ByteWidth = sizeof(LightData);

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &lightBuffer);
		assert(SUCCEEDED(hr));
	}
}

TestAmbient::~TestAmbient()
{
	SAFE_DELETE(plane);
	SAFE_DELETE(cube);
	SAFE_DELETE(sphere);
	SAFE_DELETE(sphere2);
	SAFE_DELETE(bunny);

	SAFE_DELETE(shader);

	SAFE_RELEASE(lightBuffer);
}

void TestAmbient::Update()
{
	plane->Update();
	cube->Update();
	sphere->Update();
	sphere2->Update();
	bunny->Update();
}

void TestAmbient::PreRender()
{
}

void TestAmbient::Render()
{
	//ImGui::Separator();
	//ImGui::SliderFloat3("AmbientFloor", (float *)&ambientBuffer->Data.Floor, 0, 1);
	//ImGui::SliderFloat3("AmbientCeil", (float *)&ambientBuffer->Data.Ceil, 0, 1);
	//ImGui::ColorEdit3("DirectionColor", (float *)&ambientBuffer->Data.Color);
	//
	//ImGui::Separator();
	//ImGui::SliderFloat("SpecularExp", (float *)&specularBuffer->Data.Exp, 1, 100);
	//ImGui::SliderFloat("SpecularIntensity", (float *)&specularBuffer->Data.Intensity, 0, 10);
	//
	//ImGui::Separator();
	//ImGui::SliderFloat3("PointLightPosition", (float *)&pointLightBuffer->Data.Position,-50, 50);
	//ImGui::SliderFloat("PointLightRange", (float *)&pointLightBuffer->Data.Range, 0, 1);
	//ImGui::ColorEdit3("PointLightColor", (float *)&pointLightBuffer->Data.Color);
	//
	//ImGui::Separator();
	//ImGui::SliderFloat3("SpotLightPosition", (float *)&spotLightBuffer->Data.Position, -50, 50);
	//ImGui::SliderFloat3("SpotLightDirection", (float *)&spotLightBuffer->Data.Direction, -1, 1);
	//ImGui::SliderFloat("SpotLightRange", (float *)&spotLightBuffer->Data.Range, 0, 100);
	//ImGui::SliderFloat("SpotLightOutter", (float *)&spotLightBuffer->Data.Outer, 1, 100);
	//ImGui::SliderFloat("SpotLightInner", (float *)&spotLightBuffer->Data.Inner, 1, 100);
	//ImGui::ColorEdit3("SpotLightColor", (float *)&spotLightBuffer->Data.Color);
	//
	//ImGui::Separator();
	//ImGui::SliderFloat3("CapsuleLightPosition", (float *)&capsuleLightBuffer->Data.Position, -50, 50);
	//ImGui::SliderFloat3("CapsuleLightDirection", (float *)&capsuleLightBuffer->Data.Direction, -1, 1);
	//ImGui::SliderFloat("CapsuleLightRange", (float *)&capsuleLightBuffer->Data.Range, 0, 100);
	//ImGui::SliderFloat("CapsuleLightLength", (float *)&capsuleLightBuffer->Data.Length, 1, 100);
	//ImGui::ColorEdit3("CapsuleLightColor", (float *)&capsuleLightBuffer->Data.Color);
	
	plane->Render();
	cube->Render();
	sphere->Render();
	sphere2->Render();
	bunny->Render();
}

void TestAmbient::PostRender()
{	
}