#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< Mesh >
	: 3차원 물체를 표현하기 위한 정점 버퍼와 인덱스 버퍼를 포함하는 컨테이너
*/

class example_12 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
		};

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 uv;
	};

private:
	LPD3DXEFFECT _effect = NULL;

	LPD3DXMESH _mesh = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

	float _yaw = 0.0f;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	LPD3DXEFFECT createEffect(void);
	LPD3DXMESH createMesh(void);
	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	example_12(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_12();
};

