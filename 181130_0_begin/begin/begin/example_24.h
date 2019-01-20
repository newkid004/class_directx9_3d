#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_24 : public direct3dApplication
{
private :
	LPD3DXEFFECT _effect = nullptr;

	D3DXVECTOR3 _torusPos;
	LPD3DXMESH _meshTorus = nullptr;
	LPD3DXMESH _meshDisc = nullptr;

	LPDIRECT3DTEXTURE9 _renderTarget = nullptr;
	LPDIRECT3DSURFACE9 _depthStensil = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	LPD3DXMESH createTorus(void);
	LPD3DXMESH createDisc(void);
	LPDIRECT3DTEXTURE9 createRenderTarget(void);

public:
	example_24(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_24();
};

