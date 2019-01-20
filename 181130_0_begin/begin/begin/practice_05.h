#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class practice_05 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1
		};

		D3DXVECTOR3 pos;
		D3DXVECTOR2 UV;
	};

private :
	float _yaw = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;

	LPDIRECT3DPIXELSHADER9 _pixelShader = NULL;
	LPD3DXCONSTANTTABLE _pixelTable = NULL;

	LPDIRECT3DVERTEXSHADER9 _vertexShader = NULL;
	LPD3DXCONSTANTTABLE _constantTable = NULL;

	D3DXHANDLE _texHandle = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);

	LPDIRECT3DPIXELSHADER9 createPixelShader(LPD3DXCONSTANTTABLE *outTable);
	LPDIRECT3DVERTEXSHADER9 createVertexShader(LPD3DXCONSTANTTABLE *outTable);

	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	practice_05(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~practice_05();
};

