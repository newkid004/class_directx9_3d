#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< X ���� >
	: directX9���� ���������� �����Ǹ� 3���� �𵨸��� ǥ�� ������ ���� ����

	X ���Ͽ��� ǥ���� �� �ִ� ������ ����
	- �𵨸� ( ��ü�� �̷�� �ִ� ���� )
	- ���� ( ��ü�� ������ �ϴ� ������ )
	- ��Ű�� �ִϸ��̼� ( ��ü�� �ִϸ��̼� �ϱ����� ������

*/

using namespace std;

class staticMesh;

class example_15 : public direct3dApplication
{
private :
	staticMesh* _staticMesh;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	staticMesh* createStaticMesh(void);

public:
	example_15(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_15();
};

