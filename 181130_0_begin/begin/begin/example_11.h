#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	 < 이펙트 프레임워크 >
	 : 3차원 물체를 그리기 위해 필요한 여러 설정 값을 별도의 파일 형태로 관리 및 제어할 수 있는 기능

	 - 활용
	   : 셰이더 코드의 상수 버퍼에 접근하기 위한 별도의 객체 관리 불필요

	 - 제어 가능 설정
	   1. 정점 셰이더
	   2. 픽셀 셰이더
	   3. 렌더링 상태

	   1. 정점, 픽셀 셰이더
	   : 정점 / 픽셀 셰이더 코드를 컴파일하기 위한 별도의 로직 작성이 불필요
	     -> 이펙트 프레임워크 내부에서 셰이더 코드를 컴파일 및 지정하는 것이 가능

	   2. 렌더링 상태
	   : 렌더링 상태 변화 시, 해당 상태는 별도의 로직이 필요없이 변경 이전의 상태로 돌려줌
*/

class example_11 : public direct3dApplication
{
public:
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
	float _yaw = 0.0f;
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexBuffer = NULL;

	LPD3DXEFFECT _effect = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer(void);
	LPD3DXEFFECT createEffect(void);

public:
	example_11(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_11();
};

