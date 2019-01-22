#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_25 : public direct3dApplication
{
public:
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

private :
	LPD3DXEFFECT _effect = nullptr;

	LPDIRECT3DTEXTURE9 _renderTarget = nullptr;
	LPDIRECT3DTEXTURE9 _renderTargetGlow = nullptr;
	LPDIRECT3DSURFACE9 _depthStensil = nullptr;

	LPD3DXMESH _teapot = nullptr;
	LPD3DXMESH _meshScreen = nullptr;

	LPDIRECT3DTEXTURE9 _textureDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 _textureNormal = nullptr;
	LPDIRECT3DTEXTURE9 _textureBackground = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private:
	LPDIRECT3DTEXTURE9 createRenderTarget(void);
	LPDIRECT3DSURFACE9 createDepthStensil(void);

	LPD3DXMESH createTeapot(void);
	LPD3DXMESH createMeshScreen(void);
public:
	example_25(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_25();
};

