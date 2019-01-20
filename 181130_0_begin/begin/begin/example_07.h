#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class baseObject;

class example_07 : public direct3dApplication
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
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer;

	baseObject* _cube;
	baseObject* _board;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	void drawCube(const D3DXMATRIXA16 & mWorld);

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer();

public:
	example_07(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_07();
};

