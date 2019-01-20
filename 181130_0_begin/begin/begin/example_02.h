#pragma once
#include "example.h"

/*
	< 렌더링 파이프라인 3가지 >
	1. 정점 연산				- 정점(vertex) 쉐이더 담당
	2. 레스터라이즈 연산		- 정점 -> 표면 변환 : 그래픽 카드 담당
	3. 픽셀 연산				= 픽셀 쉐이더 담당

	1. 정점 연산 :
	  - 3차원 상에 해당하는 물체의 정보를 2차원 상에 표현하기 위한
		정점의 위치 등의 연산을 담당하는 역할
	  - 연산 후, 해당 좌표는 화면상에 출력되는 좌표와 동일하다고 해도 무방

	2. 레스터라이즈 연산 :
	  - 정점의 정보를 통해 표면의 정보를 만드는 역할
	  - 연산 후, 정점의 정보를 이루는 픽셀의 모음을 얻을 수 있음

	3. 픽셀 연산 :
	  - 화면상에 출력 될 픽셀의 색상을 결정하는 역할
	  - 연산 후, 색상은 최종적으로 화면에 출력


	< 3차원 상의 좌표가 2차원 상에 출력되기 위해 거치는 단계 >
	1. 로컬(모델) 공간(space)
	2. 월드(전역) 공간
	3. 카메라(뷰) 공간
	4. 투영 공간
	5. 정규 공간
	6. 스크린 공간
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

