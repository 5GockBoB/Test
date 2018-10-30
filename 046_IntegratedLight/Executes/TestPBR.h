#pragma once
#include "Execute.h"

class TestPBR : public Execute
{
public:
	TestPBR(ExecuteValues* values);
	~TestPBR();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	class MeshPlane* plane;
	class MeshCube* cube;
	class MeshSphere* sphere;
	class MeshSphere* sphere2;
	class MeshBunny* bunny;

	Shader* shader;

private:
	class LightingPBRBuffer : public ShaderBuffer
	{
	public:
		LightingPBRBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.DiffuseRatio = 0.5f;
			Data.Roughness = 0.0f;
			Data.Fresnel = 0.0f;
		}

		struct Struct
		{
			float DiffuseRatio;
			float Roughness;
			float Fresnel;

			float Padding;
		} Data;
	};
	LightingPBRBuffer* lightingPBRBuffer;
};