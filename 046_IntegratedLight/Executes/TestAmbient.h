#pragma once
#include "Execute.h"

class TestAmbient : public Execute
{
public:
	TestAmbient(ExecuteValues* values);
	~TestAmbient();

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
	ID3D11Buffer* lightBuffer;
	
private:
	struct LightData
	{
		D3DXVECTOR4 PositionX;
		D3DXVECTOR4 PositionY;
		D3DXVECTOR4 PositionZ;

		D3DXVECTOR4 DirectionX;
		D3DXVECTOR4 DirectionY;
		D3DXVECTOR4 DirectionZ;

		D3DXVECTOR4 Range;

		D3DXVECTOR4 SpotOuter;
		D3DXVECTOR4 SpotInner;

		D3DXVECTOR4 CapsuleLength;

		D3DXVECTOR4 ColorR;
		D3DXVECTOR4 ColorG;
		D3DXVECTOR4 ColorB;
	};

	//class LightBuffer : public ShaderBuffer
	//{
	//public:
	//	LightBuffer() : ShaderBuffer(&Data, sizeof(Data))
	//	{
	//		Data.PositionX = D3DXVECTOR4(0, 0, 0, 0);
	//		Data.PositionY = D3DXVECTOR4(0, 0, 0, 0);
	//		Data.PositionZ = D3DXVECTOR4(0, 0, 0, 0);
	//		
	//		Data.DirectionX = D3DXVECTOR4(0, 0, 0, 0);
	//		Data.DirectionY = D3DXVECTOR4(0, 0, 0, 0);
	//		Data.DirectionZ = D3DXVECTOR4(0, 0, 0, 0);
	//		
	//		Data.Range = D3DXVECTOR4(0, 0, 0, 0);
	//		
	//		Data.SpotOuter = D3DXVECTOR4(0, 0, 0, 0);
	//		Data.SpotInner = D3DXVECTOR4(0, 0, 0, 0);
	//		
	//		Data.CapsuleLength = D3DXVECTOR4(0, 0, 0, 0);
	//		
	//		Data.ColorR = D3DXVECTOR4(1, 1, 1, 1);
	//		Data.ColorG = D3DXVECTOR4(1, 1, 1, 1);
	//		Data.ColorB = D3DXVECTOR4(1, 1, 1, 1);
	//	}
	//
	//	enum class ELightType
	//	{
	//		Point, Spot, Capsule
	//	};
	//
	//	struct LightDesc
	//	{
	//		ELightType Type;
	//		D3DXVECTOR3 Position;
	//		D3DXVECTOR3 Direction;
	//		float Range;
	//		float Length;
	//		float Outer;
	//		float Inner;
	//		D3DXVECTOR3 Color;
	//	};
	//
	//	
	//};
	//LightBuffer* lightBuffer;
};