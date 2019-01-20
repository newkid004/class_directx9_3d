#include "example_08.h"
#include "managerList.h"

example_08::example_08(HINSTANCE hInstance, const SIZE & windowSize, int option)
	: direct3dApplication(hInstance, windowSize, option)
{
}

example_08::~example_08()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_texture);
}

void example_08::init(void)
{
	direct3dApplication::init();

	_vertexBuffer = createVertexBuffer();
	_texture = createTexture();
}

void example_08::update(void)
{
	direct3dApplication::update();
}

void example_08::draw(void)
{
	direct3dApplication::draw();

	MN_DEV->SetRenderState(D3DRS_LIGHTING, false);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_NONE);

	// -------------------- //

	MN_DEV->SetStreamSource(0, _vertexBuffer, 0, sizeof(vertex));
	MN_DEV->SetFVF(vertex::FVF);

	// 텍스쳐는 스테이지 개념으로서 렌더링 파이프라인에서 이용하기 위해 대기 상태를 성정하는 것이 가능
	// -> 여러 텍스쳐를 하나로 혼합하는것이 가능
	MN_DEV->SetTexture(0, _texture);

	// 필터링
	MN_DEV->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MN_DEV->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// 어드레스 모드
	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	MN_DEV->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);

	MN_DEV->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// -------------------- //

	MN_DEV->SetRenderState(D3DRS_LIGHTING, true);
	MN_DEV->SetRenderState(D3DRS_CULLMODE, D3DCULL::D3DCULL_CCW);
}

LPDIRECT3DVERTEXBUFFER9 example_08::createVertexBuffer(void)
{
	LPDIRECT3DVERTEXBUFFER9 result = NULL;
	MN_DEV->CreateVertexBuffer(sizeof(vertex) * 6,
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&result,
		NULL);

	vertex* v = NULL;
	if (SUCCEEDED(result->Lock(0, 0, (void**)&v, 0)))
	{
		v[0].pos = D3DXVECTOR3(-1, -1, 0);
		v[1].pos = D3DXVECTOR3(-1,  1, 0);
		v[2].pos = D3DXVECTOR3( 1,  1, 0);

		v[3].pos = D3DXVECTOR3(-1, -1, 0);
		v[4].pos = D3DXVECTOR3( 1,  1, 0);
		v[5].pos = D3DXVECTOR3( 1, -1, 0);

		v[0].UV = D3DXVECTOR2(0, 10);
		v[1].UV = D3DXVECTOR2(0, 0);
		v[2].UV = D3DXVECTOR2(10, 0);

		v[3].UV = D3DXVECTOR2(0, 10);
		v[4].UV = D3DXVECTOR2(10, 0);
		v[5].UV = D3DXVECTOR2(10, 10);

		result->Unlock();
	}
	return result;
}

LPDIRECT3DTEXTURE9 example_08::createTexture(void)
{
	LPDIRECT3DTEXTURE9 result = NULL;

	/*
		D3DXCreateTextureFromFile 함수 : 내부적으로 기본 옵션을 지정해서 텍스쳐를 생성
		
		1. 텍스쳐 정방 사이즈 지정 ( 2의 배수 )
		2. 텍스쳐 밉맵 지정 ( 원근 결과에 따라 텍스쳐의 필터링 품질을 올리기 위한 옵션 )
	*/

	D3DXCreateTextureFromFile(MN_DEV,
		TEXT("resource/texture/texture.png"),
		&result);

	return result;
}
