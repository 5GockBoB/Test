#pragma once
#include "Execute.h"

class PaladinGame : public Execute
{
public:
	PaladinGame(ExecuteValues* values);
	~PaladinGame();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
	
	void ResizeScreen();
	
private:
	void PaladinCommend();
	void MonsterCommend();

	void AttackCollision();

private:
	class MeshPlane* plane;
	class Paladin* paladin;
	vector<class Vanguard*> monsters;
	
private:
	float rotationSpeed = 2.5f;
	float moveSpeed = 20.0f;
};
