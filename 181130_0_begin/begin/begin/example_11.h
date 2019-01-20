#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	 < ����Ʈ �����ӿ�ũ >
	 : 3���� ��ü�� �׸��� ���� �ʿ��� ���� ���� ���� ������ ���� ���·� ���� �� ������ �� �ִ� ���

	 - Ȱ��
	   : ���̴� �ڵ��� ��� ���ۿ� �����ϱ� ���� ������ ��ü ���� ���ʿ�

	 - ���� ���� ����
	   1. ���� ���̴�
	   2. �ȼ� ���̴�
	   3. ������ ����

	   1. ����, �ȼ� ���̴�
	   : ���� / �ȼ� ���̴� �ڵ带 �������ϱ� ���� ������ ���� �ۼ��� ���ʿ�
	     -> ����Ʈ �����ӿ�ũ ���ο��� ���̴� �ڵ带 ������ �� �����ϴ� ���� ����

	   2. ������ ����
	   : ������ ���� ��ȭ ��, �ش� ���´� ������ ������ �ʿ���� ���� ������ ���·� ������
*/

class example_11 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_NORMAL
		};

		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
	};

private :
	float _yaw = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexBuffer = NULL;

	LPD3DXEFFECT _effect = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer(void);
	LPD3DXEFFECT createEffect(void);

public:
	example_11(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_11();
};

