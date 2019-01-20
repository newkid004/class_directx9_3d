#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< �� �� >
	- 3D �׷��� �����ӿ�ũ���� ������ ������ �����ϰ� �ִ� ������
	  ��, 3D �׷��� �����ӿ�ũ���� ���Ǽ��迡�� �Ͼ�� ������� ����(���� ���� ��)�� ó���ϱ⿣
	  ���� ���ϰ� �ֱ� ������ ��ü ��ü�� ���������� ��ġ�� ������ �������� �̿��ؼ� ó��(����/���� ���� ��)

	- D39�� ���ԵǴ� ���� ���� ����
	  1. ���ݻ� ��			: Diffuse
	  2. ���ݻ� ��			: Specular
	  3. ����(ȯ��) ����	: Ambient

	  1. ���ݻ�		: ������ ��ü�� �浹���� ��, ���� ����
	  2. ���ݻ�		: ��ü�κ��� �ݻ�� ������ �þ߿� ��ġ�� ����
	  3. ���� ����	: ���� ��ü�� �������� ����

	< �� �� >
	3D �׷��� �����ӿ�ũ �󿡼� ������ ���ԵǾ� �ִ� ������ ������ ��� �ݻ��ų�� �����ϴ� ������
	- �Ӽ�
	  1. ���ݻ�		: ������ ���Ե� ���ݻ� ����
	  2. ���ݻ�		: ������ ���Ե� ���ݻ� ����
	  3. ȯ�� �ݻ�	: ������ ���Ե� ȯ�� ������ �ݻ� ����
	  4. �߻걤		: ������ ������� ��ü ��ü�� �߻��ϴ� ��ü���� ����
	  5. ����		: ���̶������� ����
*/

class baseObject;

class example_09 : public direct3dApplication
{
public:
	/*
		3D �׷��� �����ӿ�ũ���� ������ ���� ������ ����ó���� �ϱ� ���ؼ���
		���� ������ ����(Normal)�� ������ �ݵ�� ���ԵǾ� �־�� ��

		�̴� �׷��� �����ӿ�ũ�� ���������� ���� ������ �ϱ� ���ؼ� 
		������ ������ �̿��ϱ� ���� : ����Ʈ �ڻ��� ��Ģ
	*/
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
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexBuffer = NULL;

	float _yaw = 0.0f;
	baseObject* _objLight = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void updateControl(void);

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer(void);

public:
	example_09(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_09();
};

