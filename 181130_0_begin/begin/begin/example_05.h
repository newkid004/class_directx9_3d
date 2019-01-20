#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

class example_05 : public direct3dApplication
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
	LPDIRECT3DINDEXBUFFER9 _indexBuffer = NULL;

	float _yaw = 0.0f;
	float _pitch = 0.0f;

public:
	void init(void) override;
	LRESULT handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void update(void) override;
	void draw(void) override;

private:
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer(void);

	void drawQuad(D3DXVECTOR3 & position, D3DXVECTOR3 & rotation);

public:
	example_05(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_05();
};

