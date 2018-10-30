#include "stdafx.h"
#include "TestBlend.h"
#include "./Objects/MeshQuad.h"

TestBlend::TestBlend(ExecuteValues * values)
	: Execute(values), selected(0)
{
	quad1 = new MeshQuad();
	quad1->SetShader(new Shader(Shaders + L"041_BaseTexture.hlsl"));
	quad1->SetDiffuseMap(Textures + L"Floor.png");

	quad2 = new MeshQuad();
	quad2->SetShader(new Shader(Shaders + L"041_BaseTexture.hlsl"));
	quad2->SetDiffuseMap(Textures + L"BlendTest.png");
	quad2->Position(0, 0, -0.001f);

	for (int i = 0; i < 10; ++i)
		blendState[i] = new BlendState();

	blendState[1]->BlendEnable(true);

	blendState[2]->BlendEnable(true);
	blendState[2]->AlphaToCoverageEnable(true);

	//µ¡¼ÀÇÕ¼º
	blendState[3]->BlendEnable(true);
	blendState[3]->DestBlend(D3D11_BLEND_ONE);
	blendState[3]->SrcBlend(D3D11_BLEND_SRC_ALPHA);
	blendState[3]->BlendOp(D3D11_BLEND_OP_ADD);

	//»¬¼ÀÇÕ¼º
	blendState[4]->BlendEnable(true);
	blendState[4]->DestBlend(D3D11_BLEND_ONE);
	blendState[4]->SrcBlend(D3D11_BLEND_SRC_ALPHA);
	blendState[4]->BlendOp(D3D11_BLEND_OP_REV_SUBTRACT);

	//°ö¼ÀÇÕ¼º
	blendState[5]->BlendEnable(true);
	blendState[5]->DestBlend(D3D11_BLEND_SRC_COLOR);
	blendState[5]->SrcBlend(D3D11_BLEND_ZERO);
	blendState[5]->BlendOp(D3D11_BLEND_OP_ADD);

	//Á¦°öÇÕ¼º
	blendState[6]->BlendEnable(true);
	blendState[6]->DestBlend(D3D11_BLEND_DEST_COLOR);
	blendState[6]->SrcBlend(D3D11_BLEND_ZERO);
	blendState[6]->BlendOp(D3D11_BLEND_OP_ADD);

	//³×°ÅÆ¼³×ÀÌ¼Ç Æ÷ÁöÆ¼ºê ÇÕ¼º
	blendState[7]->BlendEnable(true);
	blendState[7]->DestBlend(D3D11_BLEND_ZERO);
	blendState[7]->SrcBlend(D3D11_BLEND_INV_DEST_COLOR);
	blendState[7]->BlendOp(D3D11_BLEND_OP_ADD);

	//ÅØ½ºÃÄ1
	blendState[8]->BlendEnable(true);
	blendState[8]->DestBlend(D3D11_BLEND_ONE);
	blendState[8]->SrcBlend(D3D11_BLEND_ZERO);
	blendState[8]->BlendOp(D3D11_BLEND_OP_ADD);

	//ÅØ½ºÃÄ2
	blendState[9]->BlendEnable(true);
	blendState[9]->DestBlend(D3D11_BLEND_ZERO);
	blendState[9]->SrcBlend(D3D11_BLEND_ONE);
	blendState[9]->BlendOp(D3D11_BLEND_OP_ADD);
}

TestBlend::~TestBlend()
{
	for (int i = 0; i < 10; ++i)
		SAFE_DELETE(blendState[i]);
	SAFE_DELETE(quad1);
	SAFE_DELETE(quad2);
}

void TestBlend::Update()
{
	quad1->Update();
	quad2->Update();
}

void TestBlend::PreRender()
{
}

void TestBlend::Render()
{
	ImGui::SliderInt("AlphaBlend", &selected, 0, 9);
	quad1->Render();

	blendState[selected]->OMSetBlendState();
	quad2->Render();
	blendState[0]->OMSetBlendState();
}

void TestBlend::PostRender()
{	
}
