#pragma once

class ModelBone;
class ModelMesh;
class Model
{
public:
	Model();
	~Model();

	UINT MaterialCount() { return materials.size(); }
	vector<Material *>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

	UINT BoneCount() { return bones.size(); }
	vector<ModelBone *>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh *>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }
	ModelMesh* MeshByName(wstring name);

	//바운딩 박스 array[8]
	void BoundingBox(D3DXVECTOR3* boundingBox);
	D3DXVECTOR3* BoundingBox() { return boundingBox; }

	void ReadMaterial(wstring folder, wstring file);
	void ReadMesh(wstring folder, wstring file);

	void CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms);
	void CopyGlobalBoneTo(vector<D3DXMATRIX>& transforms, D3DXMATRIX& w);
	void CopyGlobalBoneToName(wstring name, D3DXMATRIX* boneOut);

private:
	void MakeBoundingBox();

	void BindingBone();
	void BindingMesh();

private:
	class ModelBone* root;

	vector<Material *> materials;
	vector<class ModelMesh *> meshes;
	vector<class ModelBone *> bones;

	D3DXVECTOR3 boundingBox[8];

private:
	class BoneBuffer : public ShaderBuffer
	{
	public:
		BoneBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			for (int i = 0; i < 128; i++)
				D3DXMatrixIdentity(&Data.Bones[i]);
			Data.UseBlend = false;
		}
			
		void Bones(D3DXMATRIX* m, UINT count)
		{
			memcpy(Data.Bones, m, sizeof(D3DXMATRIX) * count);
			for (UINT i = 0; i < count; i++)
				//행우선, 열우선을 바꿔주는 전치함수 - 쉐이더는 열우선
				D3DXMatrixTranspose(&Data.Bones[i], &Data.Bones[i]);
		}

		void UseBlend(bool val)
		{
			Data.UseBlend = (int)val;
		}

	private:
		struct Struct
		{
			D3DXMATRIX Bones[128];
			int UseBlend;

			float Padding[3];
		} Data;
	};

	BoneBuffer* buffer;

public:
	BoneBuffer* Buffer() { return buffer; }
};

class Models
{
public:
	friend class Model;

public:
	static void Craete();
	static void Delete();

private:
	static void LoadMaterial(wstring file, vector<Material *>* materials); //material file 모델파일 아님
	static void ReadMaterialData(wstring file);

	static void LoadMesh(wstring file, vector<class ModelBone *>* bones, vector<class ModelMesh *>* meshes);
	static void ReadMeshData(wstring file);
private:
	static map<wstring, vector<Material *>> materialMap;

	struct MeshData
	{
		vector<class ModelBone *> Bones;
		vector<class ModelMesh *> Meshes;
	};

	static map<wstring, MeshData> meshDataMap;
};