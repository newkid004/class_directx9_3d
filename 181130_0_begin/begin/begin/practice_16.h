#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class practice_16 : public direct3dApplication
{
private:
	LPD3DXMESH _sky = nullptr;
	LPD3DXMESH _mesh = nullptr;
	LPD3DXEFFECT _effect = nullptr;
	LPDIRECT3DTEXTURE9 _texture = nullptr;
	LPDIRECT3DCUBETEXTURE9 _textureCube = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private:
	LPD3DXMESH createMesh(void);
	LPDIRECT3DCUBETEXTURE9 createTextureCube(void);

public:
	practice_16(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_16();
};

