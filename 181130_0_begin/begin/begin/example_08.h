#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 텍스쳐 >
	: 그래픽 카드가 사용 할 수 있는 리소스
	  ( 색상정보, 높이, 방향 등의 여러 정보를 넣을 수 있으며
	    저장된 데이터에 따라 지칭하는 명칭이 달라짐 )

	1. 디퓨즈 맵	: 색상 정보가 저장된 텍스쳐
	2. 노말 맵		: 법선의 정보가 저장된 텍스쳐
	3. 라이트 맵	: 광원의 세기 정보가 저장된 텍스쳐
	4. 높이 맵		: 높이(Y축)의 정보가 저장된 텍스쳐
	5. 스펙큘러 맵	: 하이라이팅의 정보가 저장된 텍스쳐
	
	- 텍스쳐 필터링(샘플링) 옵션
	  : 실제 3차원 상에 출력되는 물체의 크기와 텍스쳐의 크기가 다를 경우
	    어느 연산으로 최종 텍스쳐의 데이터를 추출 할 지 결정함

	  1. MIN_FILTER : 물체의 크기가 텍스쳐의 크기보다 작을 경우
	  2. MAG_FILTER : 물체의 크기가 텍스쳐의 크기보다 클 경우
	  3. MIP_FILTER : 밉맵으로 텍스쳐의 크기를 결정

	- 텍스쳐 어드레스 모드
	  : UV좌표가 0 ~ 1의 범위를 벗어날 경우 해당 좌표를 어떤식으로 보정할건지 결정하는 옵션 ( UV애니메이션 등 )

	  1. wrap	: 반복됨
	  2. clamp	: 해당 uv의 마지막 픽셀 색상
	  3. mirror	: wrap에서 넘어가며 텍스쳐를 반전
	

	< 빌보드 >
	: 3차원 공간 상에 위치한 물체가 다른 특정 대상을 지속적으로 주시하는 것을 의미.

	1. 전체 빌보드
	2. Y축 빌보드

	1. 전체 빌보드
	: 주시하는 물체를 바라보는 결과를 얻기 위해
	  해당 물체의 회전 여부를 나타내는 모든 기저 벡터가 영향받음 
	  ( 구형 상에서 물체를 바라보는 결과를 얻을 수 있음 )

	2. Y축 빌보드
	: 전체 빌보드에서 Y축에 해당하는 기저 벡터만 영향받음

*/

class example_08 : public direct3dApplication
{
public:
	//	텍스쳐 출력을 위해선 정점 정보에 UV좌표에 대한 정보가 포함되어야 함	
	//	UV좌표 : 해당 정점 텍스쳐의 어떤 부분을 출력할 것인지 명시(0 ~ 1, 좌상단↖ 기준)
	struct vertex
	{
		enum
		{
			FVF = D3DFVF_XYZ | D3DFVF_TEX1
		};

		D3DXVECTOR3 pos;
		D3DXVECTOR2 UV;
	};

private :
	LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

private :
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer(void);
	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	example_08(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_08();
};

