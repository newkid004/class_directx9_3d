#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 광 원 >
	- 3D 그래픽 프레임워크에서 색상의 정보를 포함하고 있는 데이터
	  단, 3D 그래픽 프레임워크에선 현실세계에서 일어나는 사실적인 광원(전역 광원 모델)을 처리하기엔
	  많은 부하가 있기 때문에 물체 자체를 직접적으로 비치는 광원의 정보만을 이용해서 처리(간접/직접 광원 모델)

	- D39에 포함되는 광원 색상 종류
	  1. 난반사 색			: Diffuse
	  2. 정반사 색			: Specular
	  3. 전역(환경) 광원	: Ambient

	  1. 난반사		: 광원이 물체에 충돌했을 때, 밝음 정도
	  2. 정반사		: 물체로부터 반사된 광원이 시야에 비치는 정도
	  3. 전역 광원	: 공간 전체에 비춰지는 정도

	< 재 질 >
	3D 그래픽 프레임워크 상에서 광원에 포함되어 있는 색상의 정보를 어떻게 반사시킬지 결정하는 데이터
	- 속성
	  1. 난반사		: 광원에 포함된 난반사 정도
	  2. 정반사		: 광원에 포함된 정반사 정도
	  3. 환경 반사	: 광원에 포함된 환경 광원의 반사 정도
	  4. 발산광		: 광원에 상관없이 물체 자체가 발산하는 자체적인 색상
	  5. 광택		: 하이라이팅의 세기
*/

class baseObject;

class example_09 : public direct3dApplication
{
public:
	/*
		3D 그래픽 프레임워크에서 광원에 의한 색상의 연산처리를 하기 위해서는
		정점 정보의 법선(Normal)의 정보가 반드시 포함되어 있어야 함

		이는 그래픽 프레임워크가 내부적으로 광원 연산을 하기 위해서 
		법선의 정보를 이용하기 때문 : 램버트 코사인 법칙
	*/
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
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DINDEXBUFFER9 _indexBuffer = NULL;

	float _yaw = 0.0f;
	baseObject* _objLight = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

	void updateControl(void);

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DINDEXBUFFER9 createIndexBuffer(void);

public:
	example_09(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_09();
};

