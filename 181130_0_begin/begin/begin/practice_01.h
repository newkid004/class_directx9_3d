#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class practice_01 : public direct3dApplication
{
public :
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 pos;
		D3DCOLOR color;
	};

private :
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexHard = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexSoft = NULL;

	D3DXVECTOR3 _position;

public :
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	void drawCube(LPDIRECT3DINDEXBUFFER9 indexBuffer, D3DXMATRIX & mTrans);

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createHardCube(void);
	LPDIRECT3DINDEXBUFFER9 createSoftCube(void);

public:
	practice_01(HINSTANCE hInstance, const SIZE & windowSize, int option) : direct3dApplication(hInstance, windowSize, option) {};
	virtual ~practice_01();
};

