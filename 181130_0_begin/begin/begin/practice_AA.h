#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class practice_AA : public direct3dApplication
{
private :
	LPD3DXEFFECT _effect = nullptr;
	LPD3DXMESH _mesh = nullptr;
	LPDIRECT3DTEXTURE9 _depthMap = nullptr;
	LPDIRECT3DSURFACE9 _surface = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	LPD3DXMESH createMesh(void);
	LPDIRECT3DTEXTURE9 createDepthMap(void);

	D3DXMATRIXA16 getTexAdj(void);

public:
	practice_AA(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_AA();
};

