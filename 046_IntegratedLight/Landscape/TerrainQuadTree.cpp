#include "stdafx.h"
#include "TerrainQuadTree.h"
#include "Terrain.h"

#include "../Viewer/Frustum.h"

const UINT TerrainQuadTree::DivideCount = 32 * 32; // 1024

TerrainQuadTree::TerrainQuadTree(ExecuteValues* values, Terrain* terrain)
	: values(values), frustum(frustum), drawCount(0)
{
	material = new Material(Shaders + L"034_Terrain.hlsl");
	material->SetDiffuseMap(Textures + L"Dirt.png");
	material->SetDetailMap(Textures + L"DetailMap.png");

	worldBuffer = new WorldBuffer();
	lineBuffer = new LineBuffer();

	frustum = new Frustum(values, 1000);

	UINT vertexCount = terrain->VertexCount();
	triangleCount = vertexCount / 3;

	vertices = new VertexTextureNormalTangentSplatting[vertexCount];
	terrain->CopyVertices((void *)vertices);

	float centerX = 0.0f, centerZ = 0.0f, width = 0.0f;
	CalcMeshDimensions(vertexCount, centerX, centerZ, width);

	parent = new NodeType();
	CreateTreeNode(parent, centerX, centerZ, width);

	SAFE_DELETE_ARRAY(vertices);

	heightMap = terrain->HeightMap();
	SAFE_DELETE(terrain);

	rasterizer[0] = new RasterizerState();
	rasterizer[1] = new RasterizerState();
	rasterizer[1]->FillMode(D3D11_FILL_WIREFRAME);
}

TerrainQuadTree::~TerrainQuadTree()
{
	SAFE_DELETE(heightMap);

	SAFE_DELETE(rasterizer[0]);
	SAFE_DELETE(rasterizer[1]);

	SAFE_DELETE(frustum);

	DeleteNode(parent);

	SAFE_DELETE(parent);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(lineBuffer);
	SAFE_DELETE(material);
}

void TerrainQuadTree::Update()
{
	frustum->Update();
}

void TerrainQuadTree::Render()
{
	drawCount = 0;
	RenderNode(parent);
	//frustum->Render();
}

void TerrainQuadTree::RenderNode(NodeType * node)
{
	D3DXVECTOR3 center(node->X, 0, node->Z);
	float d = node->Width / 2.0f;

	if (frustum->ContainCube(center, d) == false)
		return;

	UINT count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->Childs[i] != NULL)
		{
			count++;

			RenderNode(node->Childs[i]);
		}
	}

	// 자식이 있으면 부모는 그릴 필요없어서 pass 시킨거
	if (count != 0)
		return;

	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();
	lineBuffer->SetPSBuffer(10);

	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &node->VertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(node->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//rasterizer[1]->RSSetState();

	UINT indexCount = node->TriangleCount * 3;
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	
	rasterizer[0]->RSSetState();

	drawCount += node->TriangleCount;
}

void TerrainQuadTree::DeleteNode(NodeType * node)
{
	for (int i = 0; i < 4; i++)
	{
		if(node->Childs[i] != NULL)
			DeleteNode(node->Childs[i]);

		SAFE_DELETE(node->Childs[i]);
	}

	SAFE_RELEASE(node->VertexBuffer);
	SAFE_RELEASE(node->IndexBuffer);
}

void TerrainQuadTree::CalcMeshDimensions(UINT vertexCount, float & centerX, float & centerZ, float & meshWidth)
{
	centerX = centerZ = 0.0f;

	for (UINT i = 0; i < vertexCount; i++)
	{
		centerX += vertices[i].Position.x;
		centerZ += vertices[i].Position.z;
	}

	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	float maxWidth = 0.0f;
	float maxDepth = 0.0f;

	float minWidth = fabsf(vertices[0].Position.x - centerX);
	float minDepth = fabsf(vertices[0].Position.z - centerZ);

	for (UINT i = 0; i < vertexCount; i++)
	{
		float width = fabsf(vertices[i].Position.x - centerX);
		float depth = fabsf(vertices[i].Position.z - centerZ);

		if (width > maxWidth) maxWidth = width;
		if (depth > maxDepth) maxDepth = depth;
		if (width < minWidth) minWidth = width;
		if (depth < minDepth) minDepth = depth;
	}

	float maxX = (float)max(fabsf(minWidth), fabsf(maxWidth));
	float maxZ = (float)max(fabsf(minDepth), fabsf(maxDepth));

	// mesh 최대 직경 계산
	meshWidth = max(maxX, maxZ) * 2.0f;
}

void TerrainQuadTree::CreateTreeNode(NodeType * node, float positionX, float positionZ, float width)
{
	node->X = positionX;
	node->Z = positionZ;
	node->Width = width;

	node->TriangleCount = 0;

	node->VertexBuffer = NULL;
	node->IndexBuffer = NULL;

	for (UINT i = 0; i < 4; i++)
		node->Childs[i] = NULL;

	UINT triangles = ContainTriangleCount(positionX, positionZ, width);

	// Case 1 : 남은 갯수가 없을 때
	if (triangles == 0)
		return;
	
	// Case 2 : 더 작은 노드로 분할
	if (triangles > DivideCount)
	{
		for (UINT i = 0; i < 4; i++)
		{
			float offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			UINT count = ContainTriangleCount((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			if (count > 0)
			{
				node->Childs[i] = new NodeType();

				CreateTreeNode(node->Childs[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			}
		}
		return;
	}

	// Case 3 : 남은 갯수가 있을 때
	node->TriangleCount = triangles;

	UINT vertexCount = triangles * 3;
	node->Vertices = new VertexTextureNormalTangentSplatting[vertexCount];
	UINT* indices = new UINT[vertexCount];

	UINT index = 0, vertexIndex = 0;
	for (UINT i = 0; i < triangleCount; i++)
	{
		if (IsTriangleContained(i, positionX, positionZ, width) == true)
		{
			vertexIndex = i * 3;

			node->Vertices[index].Position = this->vertices[vertexIndex].Position;
			node->Vertices[index].Uv = this->vertices[vertexIndex].Uv;
			node->Vertices[index].Normal = this->vertices[vertexIndex].Normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			node->Vertices[index].Position = this->vertices[vertexIndex].Position;
			node->Vertices[index].Uv = this->vertices[vertexIndex].Uv;
			node->Vertices[index].Normal = this->vertices[vertexIndex].Normal;
			indices[index] = index;
			index++;

			vertexIndex++;
			node->Vertices[index].Position = this->vertices[vertexIndex].Position;
			node->Vertices[index].Uv = this->vertices[vertexIndex].Uv;
			node->Vertices[index].Normal = this->vertices[vertexIndex].Normal;
			indices[index] = index;
			index++;
		}
	}

	//CreateVertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormalTangentSplatting) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		// &vertices 이건 객체의 주소 0번째거를 넣어야함
		data.pSysMem = &node->Vertices[0];

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &node->VertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//CreateIndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * vertexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &indices[0];

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &node->IndexBuffer);
		assert(SUCCEEDED(hr));
	}

	SAFE_DELETE_ARRAY(indices);
}

UINT TerrainQuadTree::ContainTriangleCount(float positionX, float positionZ, float width)
{
	UINT count = 0;

	for (UINT i = 0; i < triangleCount; i++)
	{
		if (IsTriangleContained(i, positionX, positionZ, width) == true)
			count++;
	}

	return count;
}

bool TerrainQuadTree::IsTriangleContained(UINT index, float positionX, float positionZ, float width)
{
	float radius = width / 2.0f;
	UINT vertexIndex = index * 3;
	float x1 = vertices[vertexIndex].Position.x;
	float z1 = vertices[vertexIndex].Position.z;
	vertexIndex++;

	float x2 = vertices[vertexIndex].Position.x;
	float z2 = vertices[vertexIndex].Position.z;
	vertexIndex++;

	float x3 = vertices[vertexIndex].Position.x;
	float z3 = vertices[vertexIndex].Position.z;

	float minimumX = min(x1, min(x2, x3));
	if (minimumX > (positionX + radius))
		return false;

	float maximumX = max(x1, max(x2, x3));
	if (maximumX < (positionX - radius))
		return false;

	float minimumZ = min(z1, min(z2, z3));
	if (minimumZ >(positionZ + radius))
		return false;

	float maximumZ = max(z1, max(z2, z3));
	if (maximumZ < (positionZ - radius))
		return false;

	return true;
}
