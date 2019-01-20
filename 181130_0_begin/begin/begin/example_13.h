#pragma once
#include "kGlobalDefine.h"
#include "direct3dApplication.h"

/*
	< 알파블렌딩 >
	: 색상을 표현하는 4가지 성분 중 알파 채널에 해당하는 값을 이용하여
	  별도의 연산을 통해 반투명한 물체를 렌더링하는 기능

	- DX9의 알파 블렌딩
	: sour / dest color 개념.
	  sour : 픽셀 셰이더가 최종적으로 연산한 색상 값
	  dest : 렌더 타겟에 저장되어 있는 색상 값

	  sourColor * sourOperator + destColor * destOperator = 최종 색상 값

	< 스프라이트 >
	: 3차원 상에 출력되는 2차원 이미지
	  (항상 3차원 물체보다 앞에 그려짐)
*/

class example_13 : public direct3dApplication
{
public :
	struct vertex
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

		D3DXVECTOR3 pos;
		D3DCOLOR diffuse;
	};

private:
	LPD3DXMESH _mesh[2] = { NULL };
	LPD3DXEFFECT _effect = NULL;

	LPD3DXSPRITE _sprite = NULL;
	LPDIRECT3DTEXTURE9 _texture = NULL;

public:
	void init(void) override;
	void update(void) override;
	void draw(void) override;

public :
	LPD3DXMESH createMesh(void);
	LPD3DXEFFECT createEffect(void);
	LPD3DXSPRITE createSprite(void);
	LPDIRECT3DTEXTURE9 createTexture(void);

public:
	example_13(HINSTANCE hInstance, const SIZE & windowSize, int option);
	~example_13();
};

