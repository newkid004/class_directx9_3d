#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< ��ƼŬ >
	- ȭ�� �� ��µǴ� ���� ���ڸ� �̿��� �ð��� �帧�� ���� ���� �Ӽ��� ���������μ�
	  �Ȱ��� ���� ������ ǥ���ϴ� �׷��� ���

	< DirectX9 ���� ��ƼŬ�� ǥ�� ��� >
	- ����Ʈ Ÿ���� ����
	- �簢���� �޽��� �̿�

*/

class particlePoint;
class particleQuad;

class example_19 : public direct3dApplication
{
private :
	particleQuad* _particleSystem = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	particleQuad* createParticleSystem(void);

public:
	example_19(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_19();
};

