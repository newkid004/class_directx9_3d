#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< ���̴� >
	: ������ ���������ο��� ����Ǵ� ���� �ܰ� �� Ư�� �ܰ踦 ���Ƿ� ������ �� �ִ� ���

	1. ���� ���̴�
	2. �ȼ� ���̴�

	1. ���� ���̴�
	: 3���� �� ��ġ�� ��ü�� ��ǥ�� 2���� �� �ش��ϴ� ��ǥ�� ��ȯ
	  ���� ���̴����� ��µ� ��ǥ�� ���������� ȭ��� ��µǴ� ��ġ

	2. �ȼ� ���̴�
	: ȭ��� ��µ� �������� ������ ���
	  �ȼ� ���̴����� ��µ� ������ ���������� ȭ��� �ݿ�

	�̿� ���� ���̴��� ���� ����Ǵ� ������ ������������ ���α׷��Ӻ� ������ ���������� �̶�� ��
	������ �����Ǿ� ��� �Ұ����� ������ ������������ �Ƚ��� ������ ���������� �̶�� ��
*/

class example_10 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 pos;
		D3DCOLOR diffuse;
	};

private :
	float _yaw = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;

	// ���� ���̴�
	LPDIRECT3DVERTEXSHADER9 _vertexShader = NULL;
	LPD3DXCONSTANTTABLE _constantTable = NULL;

	// �ȼ� ���̴�
	LPDIRECT3DPIXELSHADER9 _pixelShader = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DVERTEXSHADER9 createVertexShader(LPD3DXCONSTANTTABLE *outTable);
	LPDIRECT3DPIXELSHADER9 createPixelShader(void);

public:
	example_10(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_10();
};