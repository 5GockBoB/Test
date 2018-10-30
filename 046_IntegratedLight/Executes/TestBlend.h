#pragma once
#include "Execute.h"

class TestBlend : public Execute
{
public:
	TestBlend(ExecuteValues* values);
	~TestBlend();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen() {}

private:
	class MeshQuad* quad1;
	class MeshQuad* quad2;

	BlendState* blendState[10];

	int selected;
};