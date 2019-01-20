#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_23 : public direct3dApplication
{
public :
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

private :
	LPD3DXEFFECT _effect = nullptr;
	LPD3DXMESH _mesh = nullptr;
	LPD3DXMESH _meshScreen = nullptr;
	LPDIRECT3DTEXTURE9 _texture = nullptr;
	LPDIRECT3DCUBETEXTURE9 _textureCube = nullptr;

	LPDIRECT3DTEXTURE9 _renderOutline = nullptr;
	LPDIRECT3DTEXTURE9 _renderTarget = nullptr;
	LPDIRECT3DSURFACE9 _depthStensil = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	LPD3DXMESH createMesh(void);
	LPD3DXMESH createMeshScreen(void);
	LPDIRECT3DTEXTURE9 createRenderTarget(void);
	LPDIRECT3DSURFACE9 createDepthStensil(void);

public:
	example_23(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_23();
};

