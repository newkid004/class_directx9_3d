#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class practice_19 : public direct3dApplication
{
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };

		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
	};

private:
	LPD3DXEFFECT _effect = nullptr;

	LPDIRECT3DTEXTURE9 _renderBack = nullptr;
	LPDIRECT3DTEXTURE9 _renderTarget = nullptr;
	LPDIRECT3DTEXTURE9 _renderTargetGlow = nullptr;
	LPDIRECT3DTEXTURE9 _renderTargetBlur = nullptr;
	LPDIRECT3DSURFACE9 _depthStensil = nullptr;
	LPDIRECT3DSURFACE9 _depthStensilTemp = nullptr;

	LPD3DXMESH _teapot = nullptr;
	LPD3DXMESH _meshScreen = nullptr;

	LPDIRECT3DTEXTURE9 _textureDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 _textureNormal = nullptr;
	LPDIRECT3DTEXTURE9 _textureBackground = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	void drawBack(void);

private:
	LPDIRECT3DTEXTURE9 createRenderTarget(void);
	LPDIRECT3DSURFACE9 createDepthStensil(void);

	LPD3DXMESH createTeapot(void);
	LPD3DXMESH createMeshScreen(void);

public:
	practice_19(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_19();
};

