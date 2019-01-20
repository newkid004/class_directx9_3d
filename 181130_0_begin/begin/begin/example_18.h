#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< ������ �÷��� �� ���� ��� >
	- WinAPI		: PlaySound		( ���� �ͽ� �Ұ��� )
	- 3rd Party SDK	: FMOD
	- DirectX		: Direct Sound

	< Direct Sound >
	- MS���� ���������� �����ϴ� ���� �����ӿ�ũ
	  3���� ���� ����
	  ���� �ͽ� ����

	- ����
	  wav ���˸� ����
*/

class sound;

class example_18 : public direct3dApplication
{
private :
	sound* _soundSE = nullptr;
	sound* _soundBGM = nullptr;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public:
	example_18(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_18();
};