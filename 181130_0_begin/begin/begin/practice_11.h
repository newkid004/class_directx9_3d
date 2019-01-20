#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

using namespace std;

class practice_11 : public direct3dApplication
{
public:
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

private:
	SIZE _mapSize;
	SIZE _cellSize;
	SIZE _totalSize;

	LPD3DXMESH _mesh = nullptr;
	LPD3DXMESH _meshNormalize = nullptr;

	LPD3DXMESH _currentMesh = nullptr;

	LPDIRECT3DTEXTURE9 _texture = nullptr;
	LPDIRECT3DTEXTURE9 _viewTexture = nullptr;

	float _heightScale = 0.0f;
	vector<float> _vHeightList;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;
	void drawUI(void) override;

private:
	void updateControl(void);

private:
	LPD3DXMESH createMesh(void);
	LPDIRECT3DTEXTURE9 createTexture(void);
	LPDIRECT3DTEXTURE9 createViewTexture(void);
	void normalizeHeight(void);

public:
	practice_11(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_11();
};

