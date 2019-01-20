#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< ���ĺ��� >
	: ������ ǥ���ϴ� 4���� ���� �� ���� ä�ο� �ش��ϴ� ���� �̿��Ͽ�
	  ������ ������ ���� �������� ��ü�� �������ϴ� ���

	- DX9�� ���� ����
	: sour / dest color ����.
	  sour : �ȼ� ���̴��� ���������� ������ ���� ��
	  dest : ���� Ÿ�ٿ� ����Ǿ� �ִ� ���� ��

	  sourColor * sourOperator + destColor * destOperator = ���� ���� ��

	< ��������Ʈ >
	: 3���� �� ��µǴ� 2���� �̹���
	  (�׻� 3���� ��ü���� �տ� �׷���)
*/

class example_13 : public direct3dApplication
{
public :
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3 pos;
		D3DCOLOR diffuse;
	};

private:
	LPD3DXMESH _mesh[2] = { NULL };
	LPD3DXEFFECT _effect = NULL;

	LPD3DXSPRITE _sprite = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPD3DXMESH createMesh(void);
	LPD3DXEFFECT createEffect(void);
	LPD3DXSPRITE createSprite(void);
	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	example_13(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_13();
};

