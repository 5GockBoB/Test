#pragma once

class Perspective
{
public:
	Perspective(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);
	~Perspective();

	void GetMatrix(D3DXMATRIX* mat);
	void Set(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);

	float GetWidth() { return width; }
	float GetHeight() { return height; }
	float GetFov() { return fov; }
	float GetZn() { return zn; }
	float GetZf() { return zf; }
	void SetZf(float zf = 1000.0f) { Set(width, height, fov, zn, zf); }
private:
	float width, height;
	float fov;
	float aspect;
	float zn, zf;

	D3DXMATRIX projection;
};