#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 셰이더 >
	: 렌더링 파이프라인에서 수행되는 여러 단계 중 특정 단계를 임의로 제어할 수 있는 기능

	1. 정점 셰이더
	2. 픽셀 셰이더

	1. 정점 셰이더
	: 3차원 상에 위치한 물체의 좌표를 2차원 상에 해당하는 좌표로 변환
	  정점 쉐이더에서 출력된 좌표는 최종적으로 화면상에 출력되는 위치

	2. 픽셀 셰이더
	: 화면상에 출력될 최종적인 색상을 계산
	  픽셀 셰이더에서 출력된 색상은 최종적으로 화면상에 반영

	이와 같이 셰이더에 의해 제어되는 렌더링 파이프라인을 프로그래머블 렌더링 파이프라인 이라고 함
	기존의 고정되어 제어가 불가능한 렌더링 파이프라인을 픽스드 렌더링 파이프라인 이라고 함
*/

class example_10 : public direct3dApplication
{
public:
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
		};

		D3DXVECTOR3 pos;
		D3DCOLOR diffuse;
	};

private :
	float _yaw = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;

	// 정점 셰이더
	LPDIRECT3DVERTEXSHADER9 _vertexShader = NULL;
	LPD3DXCONSTANTTABLE _constantTable = NULL;

	// 픽셀 셰이더
	LPDIRECT3DPIXELSHADER9 _pixelShader = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DVERTEXSHADER9 createVertexShader(LPD3DXCONSTANTTABLE *outTable);
	LPDIRECT3DPIXELSHADER9 createPixelShader(void);

public:
	example_10(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_10();
};