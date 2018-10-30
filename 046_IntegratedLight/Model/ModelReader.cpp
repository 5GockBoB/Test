#include "stdafx.h"
#include "Model.h"
#include "ModelMesh.h"
#include "ModelMeshPart.h"
#include "ModelBone.h"
#include "../Utilities/Xml.h"
#include "../Utilities/BinaryFile.h"

void Model::BoundingBox(D3DXVECTOR3 * boundingBox)
{
	for (UINT i = 0; i < 8; ++i)
		boundingBox[i] = this->boundingBox[i];
}

void Model::ReadMaterial(wstring folder, wstring file)
{
	Models::LoadMaterial(folder + file, &materials);
}

void Model::ReadMesh(wstring folder, wstring file)
{
	Models::LoadMesh(folder + file, &bones, &meshes);

	BindingBone();
	BindingMesh();

	MakeBoundingBox();
}

void Model::MakeBoundingBox()
{
	D3DXVECTOR3 min, max;
	//큰값 작은값을 미리 넣어준다.
	min = { 10000, 10000, 10000 };
	max = { -10000, -10000, -10000 };

	D3DXVECTOR3 result[8];

	for (ModelMesh* mesh : meshes)
	{
		ModelBone* bone = mesh->ParentBone();
		D3DXMATRIX boneWorld = bone->Global();

		for (ModelMeshPart* part : mesh->meshParts)
		{
			for (VertexTextureNormalTangentBlend vertex : part->vertices)
			{
				D3DXVECTOR3 position = vertex.Position;
				D3DXVec3TransformCoord(&position, &position, &boneWorld);

				min.x = (min.x < position.x) ? min.x : position.x;
				min.y = (min.y < position.y) ? min.y : position.y;
				min.z = (min.z < position.z) ? min.z : position.z;
				max.x = (max.x > position.x) ? max.x : position.x;
				max.y = (max.y > position.y) ? max.y : position.y;
				max.z = (max.z > position.z) ? max.z : position.z;
			}
		}
	}

	boundingBox[0] = D3DXVECTOR3(min.x, min.y, min.z);
	boundingBox[1] = D3DXVECTOR3(min.x, max.y, min.z);
	boundingBox[2] = D3DXVECTOR3(max.x, min.y, min.z);
	boundingBox[3] = D3DXVECTOR3(max.x, max.y, min.z);
	boundingBox[4] = D3DXVECTOR3(min.x, min.y, max.z);
	boundingBox[5] = D3DXVECTOR3(min.x, max.y, max.z);
	boundingBox[6] = D3DXVECTOR3(max.x, min.y, max.z);
	boundingBox[7] = D3DXVECTOR3(max.x, max.y, max.z);
}

void Model::BindingBone()
{
	this->root = bones[0];
	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->childs.push_back(bone);
		}
		else
		{
			bone->parent = NULL;
		}
	}
}

void Model::BindingMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->parentBoneIndex == bone->index)
			{
				mesh->parentBone = bone;
				break;
			}
		}

		for (ModelMeshPart* part : mesh->meshParts)
		{
			for (Material* material : materials)
			{
				if (material->Name() == part->materialName)
				{
					part->material = material;
					break;
				}
			}
		}

		//vertex, index buffer를 만든다.
		mesh->Binding();
	}
}

/////////////	Models	////////////////

map<wstring, vector<Material *>> Models::materialMap;

void Models::LoadMaterial(wstring file, vector<Material*>* materials)
{
	//처음 불러온다면 파일로 부터 자료를 읽어온다.
	if (materialMap.count(file) < 1)
		ReadMaterialData(file);

	//불러왔던 자료가 저장되어있던 곳에서 복사해온다.
	for (Material* material : materialMap[file])
	{
		Material* temp = NULL;
		material->Clone((void **)&temp);

		materials->push_back(temp);
	}
}

void Models::ReadMaterialData(wstring file)
{
	vector<Material *> materials;

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLError error = document->LoadFile(String::ToString(file).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* matNode = root->FirstChildElement();
	//material을 전부 찾는다.
	do
	{
		Xml::XMLElement* node = NULL;

		Material* material = new Material();

		//Name
		node = matNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));

		//DiffuseColor
		node = node->NextSiblingElement();

		D3DXCOLOR dxColor;
		Xml::XMLElement* color = node->FirstChildElement();
		dxColor.r = color->FloatText();
		color = color->NextSiblingElement();
		dxColor.g = color->FloatText();
		color = color->NextSiblingElement();
		dxColor.b = color->FloatText();
		color = color->NextSiblingElement();
		dxColor.a = color->FloatText();
		material->SetDiffuse(dxColor);

		wstring texture, directory;
		//DiffuseTexture
		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());

		if (texture.length() > 0)
		{
			directory = Path::GetDirectoryName(file);
			material->SetDiffuseMap(directory + texture);
		}

		//NormalMapTexture
		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());

		if (texture.length() > 0)
		{
			directory = Path::GetDirectoryName(file);
			material->SetNormalMap(directory + texture);
		}

		//DiffuseTexture
		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());

		if (texture.length() > 0)
		{
			directory = Path::GetDirectoryName(file);
			material->SetSpecularMap(directory + texture);
		}

		materials.push_back(material);
		matNode = matNode->NextSiblingElement();
	} while (matNode != NULL);
	materialMap[file] = materials;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

map<wstring, Models::MeshData> Models::meshDataMap;
void Models::LoadMesh(wstring file, vector<ModelBone*>* bones, vector<ModelMesh*>* meshes)
{
	if (meshDataMap.count(file) < 1)
		ReadMeshData(file);

	MeshData data = meshDataMap[file];
	for (size_t i = 0; i < data.Bones.size(); i++)
	{
		ModelBone* bone = NULL;
		data.Bones[i]->Clone((void **)&bone);

		bones->push_back(bone);
	}

	for (size_t i = 0; i < data.Meshes.size(); i++)
	{
		ModelMesh* mesh = NULL;
		data.Meshes[i]->Clone((void**)&mesh);		

		meshes->push_back(mesh);
	}
}

void Models::ReadMeshData(wstring file)
{
	BinaryReader* r = new BinaryReader();
	r->Open(file);


	UINT count = 0;
	count = r->UInt();
	vector<ModelBone *> bones;

	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = String::ToWString(r->String());
		bone->parentIndex = r->Int();
		bone->local = r->Matrix();
		bone->global = r->Matrix();

		bones.push_back(bone);
	}

	count = r->UInt();
	vector<ModelMesh *> meshes;
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = String::ToWString(r->String());
		mesh->parentBoneIndex = r->Int();

		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->parent = mesh;
			meshPart->materialName = String::ToWString(r->String());

			//VertexData
			{
				UINT count = r->UInt();
				meshPart->vertices.assign(count, VertexTextureNormalTangentBlend());

				void* ptr = (void *)&(meshPart->vertices[0]);
				r->Byte(&ptr, sizeof(VertexTextureNormalTangentBlend) * count);
			}

			//IndexData
			{
				UINT count = r->UInt();
				meshPart->indices.assign(count, UINT());

				void* ptr = (void *)&(meshPart->indices[0]);
				r->Byte(&ptr, sizeof(UINT) * count);
			}

			mesh->meshParts.push_back(meshPart);
		}//for(k)

		meshes.push_back(mesh);
	}//for(i)

	r->Close();
	SAFE_DELETE(r);

	//크기가 자주 변하지 않는 vector는 assign을 해준다.
	MeshData data;
	data.Bones.assign(bones.begin(), bones.end());
	data.Meshes.assign(meshes.begin(), meshes.end());
	meshDataMap[file] = data;
}
