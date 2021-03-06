#pragma once

class RenderTarget
{
public:
	RenderTarget(struct ExecuteValues* values, UINT width = 0, UINT height = 0);
	~RenderTarget();

	void Set(D3DXCOLOR clear = 0xFF000000);

	ID3D11ShaderResourceView* GetSRV() { return srv; }
	ID3D11RenderTargetView* GetRTV() { return rtv; }
	ID3D11DepthStencilView* GetDSV() { return dsv; }

	void SaveRtvTexture(wstring saveFile);
	

private:
	struct ExecuteValues* values;

	UINT width, height;

	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;

	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;

	class Viewport* viewport;
};
