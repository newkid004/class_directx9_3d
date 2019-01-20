#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< ��Ű�� �ִϸ��̼� >
	: ������ �� �ִϸ��̼��� ���� ������ �����ϱ� ���� ���� �ִϸ��̼� ���
	  ���� �����ӿ� ���� ���� ���ӵ� ������ ����ġ�� ������ �޾�
	  �ڿ������� ���������� �̾��ִ� �ִϸ��̼� ���
*/

class skinnedMesh;

class example_16 : public direct3dApplication
{
private :
	skinnedMesh* _skinnedMesh = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void updateControl(void);

private :
	skinnedMesh* createSkinnedMesh(void);

public:
	example_16(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_16();
};

