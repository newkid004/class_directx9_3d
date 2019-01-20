#pragma once
#include "example.h"

/*
	< ������ ���������� 3���� >
	1. ���� ����				- ����(vertex) ���̴� ���
	2. �����Ͷ����� ����		- ���� -> ǥ�� ��ȯ : �׷��� ī�� ���
	3. �ȼ� ����				= �ȼ� ���̴� ���

	1. ���� ���� :
	  - 3���� �� �ش��ϴ� ��ü�� ������ 2���� �� ǥ���ϱ� ����
		������ ��ġ ���� ������ ����ϴ� ����
	  - ���� ��, �ش� ��ǥ�� ȭ��� ��µǴ� ��ǥ�� �����ϴٰ� �ص� ����

	2. �����Ͷ����� ���� :
	  - ������ ������ ���� ǥ���� ������ ����� ����
	  - ���� ��, ������ ������ �̷�� �ȼ��� ������ ���� �� ����

	3. �ȼ� ���� :
	  - ȭ��� ��� �� �ȼ��� ������ �����ϴ� ����
	  - ���� ��, ������ ���������� ȭ�鿡 ���


	< 3���� ���� ��ǥ�� 2���� �� ��µǱ� ���� ��ġ�� �ܰ� >
	1. ����(��) ����(space)
	2. ����(����) ����
	3. ī�޶�(��) ����
	4. ���� ����
	5. ���� ����
	6. ��ũ�� ����
*/

class example_02 : public example
{
private :
	c3DMatrix	_transM;
	D3DXVECTOR3	_viewPos;
	D3DXVECTOR3	_vertices[4];
	double		_yaw = 0.0;
	double		_pos[2];
	double		_scale;
	std::chrono::system_clock::time_point _prevTime;
		
public :
	void init(void) override;
	LRESULT handleWindowMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public :
	void msgTimer(WPARAM wParam, LPARAM lParam);
	void msgPaint(WPARAM wParam, LPARAM lParam);
	void msgKeyDown(WPARAM wParam, LPARAM lParam);

public:
	example_02(HINSTANCE hInstance, const SIZE & windowSize, int option);
	virtual ~example_02();
};

