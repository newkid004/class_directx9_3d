#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 파티클 >
	- 화면 상에 출력되는 작은 입자를 이용해 시간의 흐름에 따라 각종 속성을 변경함으로서
	  안개와 같은 현상을 표현하는 그래픽 기법

	< DirectX9 에서 파티클의 표현 방법 >
	- 포인트 타입의 정점
	- 사각형의 메쉬를 이용

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

