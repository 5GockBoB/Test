#pragma once
#include "Execute.h"

class TestProjection : public Execute
{
public:
	TestProjection(ExecuteValues* values);
	~TestProjection();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	class GameAnimModel* hanjo;
	class MeshPlane* plane;

	class Projection* projection;
};