#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< �ؽ��� >
	: �׷��� ī�尡 ��� �� �� �ִ� ���ҽ�
	  ( ��������, ����, ���� ���� ���� ������ ���� �� ������
	    ����� �����Ϳ� ���� ��Ī�ϴ� ��Ī�� �޶��� )

	1. ��ǻ�� ��	: ���� ������ ����� �ؽ���
	2. �븻 ��		: ������ ������ ����� �ؽ���
	3. ����Ʈ ��	: ������ ���� ������ ����� �ؽ���
	4. ���� ��		: ����(Y��)�� ������ ����� �ؽ���
	5. ����ŧ�� ��	: ���̶������� ������ ����� �ؽ���
	
	- �ؽ��� ���͸�(���ø�) �ɼ�
	  : ���� 3���� �� ��µǴ� ��ü�� ũ��� �ؽ����� ũ�Ⱑ �ٸ� ���
	    ��� �������� ���� �ؽ����� �����͸� ���� �� �� ������

	  1. MIN_FILTER : ��ü�� ũ�Ⱑ �ؽ����� ũ�⺸�� ���� ���
	  2. MAG_FILTER : ��ü�� ũ�Ⱑ �ؽ����� ũ�⺸�� Ŭ ���
	  3. MIP_FILTER : �Ӹ����� �ؽ����� ũ�⸦ ����

	- �ؽ��� ��巹�� ���
	  : UV��ǥ�� 0 ~ 1�� ������ ��� ��� �ش� ��ǥ�� ������� �����Ұ��� �����ϴ� �ɼ� ( UV�ִϸ��̼� �� )

	  1. wrap	: �ݺ���
	  2. clamp	: �ش� uv�� ������ �ȼ� ����
	  3. mirror	: wrap���� �Ѿ�� �ؽ��ĸ� ����
	

	< ������ >
	: 3���� ���� �� ��ġ�� ��ü�� �ٸ� Ư�� ����� ���������� �ֽ��ϴ� ���� �ǹ�.

	1. ��ü ������
	2. Y�� ������

	1. ��ü ������
	: �ֽ��ϴ� ��ü�� �ٶ󺸴� ����� ��� ����
	  �ش� ��ü�� ȸ�� ���θ� ��Ÿ���� ��� ���� ���Ͱ� ������� 
	  ( ���� �󿡼� ��ü�� �ٶ󺸴� ����� ���� �� ���� )

	2. Y�� ������
	: ��ü �����忡�� Y�࿡ �ش��ϴ� ���� ���͸� �������

*/

class example_08 : public direct3dApplication
{
public:
	//	�ؽ��� ����� ���ؼ� ���� ������ UV��ǥ�� ���� ������ ���ԵǾ�� ��	
	//	UV��ǥ : �ش� ���� �ؽ����� � �κ��� ����� ������ ���(0 ~ 1, �»�ܢ� ����)
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
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	example_08(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_08();
};

