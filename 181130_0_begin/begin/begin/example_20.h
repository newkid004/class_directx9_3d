#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< �븻 ���� >
	- 3���� ��ü�� ������ ó���ϴ� ���������� �����ϰ� �ִ� �븻���� ����
	  ������ ���� ���� ��� �븻���� �������� ������ ó��
*/

class cameraControlable;

class example_20 : public direct3dApplication
{
public :
	struct vertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 uv;
	};

private :
	cameraControlable* c = nullptr;	

	LPD3DXEFFECT _effect = nullptr;
	LPD3DXMESH _mesh = nullptr;
	LPDIRECT3DTEXTURE9 _textureDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 _textureNormal = nullptr;
	
	float _distort = 0.03f;
	int _drawType = 0;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;
	void drawUI(void) override;

private :
	LPD3DXMESH createMesh(void);

public:
	example_20(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_20();
};

