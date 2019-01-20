#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 윈도우 플랫폼 내 사운드 재생 >
	- WinAPI		: PlaySound		( 사운드 믹싱 불가능 )
	- 3rd Party SDK	: FMOD
	- DirectX		: Direct Sound

	< Direct Sound >
	- MS에서 공식적으로 제공하는 사운드 프레임워크
	  3차원 사운드 지원
	  사운드 믹싱 지원

	- 단점
	  wav 포맷만 지원
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