#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< Mesh >
	: 3���� ��ü�� ǥ���ϱ� ���� ���� ���ۿ� �ε��� ���۸� �����ϴ� �����̳�
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

