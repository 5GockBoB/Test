#include "stdafx.h"
#include "MapToolCreateObject.h"
#include "./Objects/MeshCube.h"
#include "./Objects/MeshSphere.h"
#include "./Objects/GameTree.h"

CreateObjCallback CreateObjectType[ObjType_End] =
{
	CreateCube,
	CreateSphere,
	CreateTree
};

GameModel * CreateCube(ExecuteValues* values, D3DXVECTOR3 position, D3DXVECTOR3 scale)
{
	GameModel* model = new MeshCube();
	model->Scale(scale);
	model->Position(position.x, position.y + scale.y * 0.5f, position.z);
	return model;
}

GameModel * CreateSphere(ExecuteValues* values, D3DXVECTOR3 position, D3DXVECTOR3 scale)
{
	GameModel* model = new MeshSphere();
	model->Scale(scale);
	model->Position(position.x, position.y + scale.y * 0.5f, position.z);
	return model;
}

GameModel * CreateTree(ExecuteValues* values, D3DXVECTOR3 position, D3DXVECTOR3 scale)
{
	GameModel* model = new GameTree(values);
	model->Scale(scale.x, scale.y, 1.0f);
	model->Position(position.x, position.y + scale.y * 0.5f, position.z);
	return model;
}
