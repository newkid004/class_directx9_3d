#include "example_09.h"
#include "managerList.h"

#include "baseObject.h"

example_09::example_09(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_09::~example_09()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
	SAFE_DELETE(_objLight);
}

void example_09::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_indexBuffer = createIndexBuffer();

	_objLight = new baseObject;
}

void example_09::update(void)
{
	direct3dApplication::update();

	_yaw += 90.0f * MN_TIME->getDeltaTime();

	updateControl();
}

void example_09::draw(void)
{
	direct3dApplication::draw();

	// 레스터라이즈 단계에서 정점에 포함되어있는 법선 정보가 보간에 의해서
	// 왜곡된 단위 벡터의 값을 다시 정규화 시키는 옵션
	MN_DEV->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// 정반사 연산을 활성화
	MN_DEV->SetRenderState(D3DRS_SPECULARENABLE, true);

	/*
		DX9이 제공하는 쉐이딩 모델
		1. Flat		: 단색 쉐이딩 처리
		2. Gouraud	: 정점 단위 쉐이딩 처리
		3. Phong	: 픽셀 단위 쉐이딩 처리
	*/
	// 픽셀 단위의 색상 연산 옵션 활성화
	MN_DEV->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);

	// world
	D3DXMATRIXA16 mWorld;
	D3DXMatrixRotationY(&mWorld, D3DXToRadian(_yaw));
	MN_DEV->SetTransform(D3DTS_WORLD, &mWorld);

	// light
	/*
		1. 방향광(Direction)
		2. 점광(Point)
		3. 집중광(Spot)

		1. 방향광
		- 빛의 근원지가 무한대로 멀리 있다고 가정하는 가상의 광원 모델
		  3차원 공간상에 존재하는 모든 물체들이 동일한 방향으로 빛을 비춤

		2. 점광
		- 빛의 근원지를 기준으로 사방으로 퍼져나가는 광원 모델로써 빛의 근원지의 위치에 따라
		  3차원 공간 상에 존재하는 물체들이 받는 광원의 방향이 달라짐

		3. 집중광
		- 현실상의 손전등과 비슷한 광원 모델로써 빛의 근원지로부터 깔때기 모양으로 광원이 퍼짐
	*/
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	light.Direction = _objLight->getDirectForward();
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	/*
		DX9은 내부적으로 인덱스의 개념을 통해 여러 광원 모델을 속성 지정 및 활성화가 가능함
		여러 광원 정보를 설정하면 렌더링 파이프라인 연산 결과에 따라 여러 광원에 의해 산출된
		최종적인 색상 정보를 혼합하는 것이 가능
	*/
	MN_DEV->LightEnable(0, true);
	MN_DEV->SetLight(0, &light);

	// material
	/*
		재질의 색상 정보 : 광원에 포함되어 있는 색상 정보의 반사 정도
		Ex. light * material
	*/
	D3DMATERIAL9 material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	material.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Power = 50.0f;
	MN_DEV->SetMaterial(&material);

	// draw
	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetIndices(_indexBuffer);
	MN_DEV->SetFVF(vertex::FVF);

	MN_DEV->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 5, 0, 4);
}

void example_09::updateControl(void)
{
	_objLight->update();

	if (MN_KEY->keyDown(DIK_LEFT))	_objLight->rotateY(200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_RIGHT))	_objLight->rotateY(-200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_UP))	_objLight->rotateX(-200.0f * MN_TIME->getDeltaTime(), false);
	if (MN_KEY->keyDown(DIK_DOWN))	_objLight->rotateX(200.0f * MN_TIME->getDeltaTime(), false);
}

LPDIRECT3DVERTEXBUFFER9 example_09::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;

	MN_DEV->CreateVertexBuffer(
		sizeof(vertex) * 5,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, -1);
		v[1].pos = D3DXVECTOR3( 0,  1,  0);
		v[2].pos = D3DXVECTOR3( 1, -1, -1);
		v[3].pos = D3DXVECTOR3(-1, -1,  1);
		v[4].pos = D3DXVECTOR3( 1, -1,  1);

		float nDis = 0.707f;
		v[0].normal = D3DXVECTOR3(-nDis, nDis, -nDis);
		v[1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		v[2].normal = D3DXVECTOR3( nDis, nDis, -nDis);
		v[3].normal = D3DXVECTOR3(-nDis, nDis,  nDis);
		v[4].normal = D3DXVECTOR3( nDis, nDis,  nDis);

		result->Unlock();
	}

	return result;
}

LPDIRECT3DINDEXBUFFER9 example_09::createIndexBuffer(void)
{
	LPDIRECT3DINDEXBUFFER9 result = NULL;

	MN_DEV->CreateIndexBuffer(
		sizeof(WORD) * 12,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	WORD* i;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&i, 0)))
	{
		int index = -1;
		i[++index] = 0;	i[++index] = 1;	i[++index] = 2;
		i[++index] = 4;	i[++index] = 1;	i[++index] = 3;
		i[++index] = 3;	i[++index] = 1;	i[++index] = 0;
		i[++index] = 2;	i[++index] = 1;	i[++index] = 4;

		result->Unlock();
	}

	return result;
}
